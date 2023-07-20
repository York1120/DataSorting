#include "SqlCreate.h"
#include <mutex>

// 这个线程池应该设置为2n+1，典型的IO密集型任务
ThreadPool sqlPool(24);

locker myLocker;

sql_create::sql_create(const string &host, const string &user, const string &passwd, const string &dbname, const string &tbname)
{
    m_DB_HOST = host;
    m_DB_USER = user;
    m_DB_PASSWORD = passwd;
    m_DB_NAME = dbname;
    m_TABLE_NAME = tbname;
};

// 初始化mysql
void sql_create::initSql(MYSQL *conn)
{
    // 连接到数据库
    if (!mysql_real_connect(conn, m_DB_HOST.c_str(), m_DB_USER.c_str(), m_DB_PASSWORD.c_str(), m_DB_NAME.c_str(), 0, nullptr, 0))
    {
        std::cerr << "Failed to connect to MySQL database: " << mysql_error(conn) << std::endl;
        std::cerr << "Please create database: create database " << m_DB_NAME << ";" << std::endl;
        mysql_close(conn);
        return;
    }

    // 如果数据库表已经存在，则删除原先的表再重新构建
    string dropTableQuery = "DROP TABLE IF EXISTS " + m_TABLE_NAME + ";";
    if (mysql_query(conn, dropTableQuery.c_str()) != 0)
    {
        std::cerr << "Failed to drop old table: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return;
    }
}

// 创建数据库和表
void sql_create::createTable(MYSQL *conn)
{

    string query = "CREATE TABLE IF NOT EXISTS " + m_TABLE_NAME + " ("
                                                                  "WD INT PRIMARY KEY,"
                                                                  "FILE_PATH VARCHAR(2550) NOT NULL,"
                                                                  "CREATION_TIME VARCHAR(255) NOT NULL,"
                                                                  "FILE_SIZE BIGINT NOT NULL"
                                                                  ");";
    if (mysql_query(conn, query.c_str()) != 0)
    {
        std::cerr << "Failed to create table: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        exit(1);
    }
}

// 插入文件信息到数据库
void sql_create::insertFileData(MYSQL *conn, const int &wd, const std::string &filepath, const std::time_t &creationTime, const std::uintmax_t &fileSize, const string &tableName)
{
    // 一定要加锁！要不然会两个线程取插入同一条数据
    myLocker.lock();
    ostringstream oss;
    oss << put_time(std::localtime(&creationTime), "%Y-%m-%d %X");
    std::string formattedCreationTime = oss.str();

    std::string query = "INSERT INTO " + tableName + " (WD, FILE_PATH, CREATION_TIME, FILE_SIZE) VALUES ('" + to_string(wd) + "', '" + filepath + "', '" + formattedCreationTime + "', " + to_string(fileSize) + ");";

    int rc = mysql_query(conn, query.c_str());
    if (rc != 0)
    {
        std::cerr << "MySQL error: " << mysql_error(conn) << std::endl;
        exit(1);
    }
    // 写日志
    AsyncLogger::getInstance().start();
    AsyncLogger::getInstance().append(query + " success\n");
    myLocker.unlock();
}

// 删除文件信息从数据库
void sql_create::deleteFileData(MYSQL *conn, const std::string &filepath)
{
    std::string query = "DELETE FROM " + m_TABLE_NAME + " WHERE FILE_PATH = '" + filepath + "';";

    int rc = mysql_query(conn, query.c_str());
    if (rc != 0)
    {
        std::cerr << "MySQL error: " << mysql_error(conn) << std::endl;
        exit(1);
    }
}

// 遍历文件夹的所有文件，设置监听，存入数据库，分别写入日志
void sql_create::traverseDirectory(MYSQL *conn, const std::vector<std::string> &folderPaths, int inotifyFd, const string &tableName)
{

    for (const auto &folderPath : folderPaths)
    {
        for (const auto &entry : fs::recursive_directory_iterator(folderPath))
        {
            if (fs::is_regular_file(entry.status()))
            {
                // 将 setInotify 作为任务提交给线程池，这样就不需要等待某个线程处理完
                // 也就是说，可以不需要等这个文件夹遍历完，设置完监听，直接将它交给一个线程处理，然后继续遍历下一个文件夹
                sqlPool.enqueue(setInotify, conn, entry, inotifyFd, tableName);
            }
        }
    }
}

// 设置inotify监控，插入数据库，写日志
void sql_create::setInotify(MYSQL *conn, const std::experimental::filesystem::v1::__cxx11::directory_entry &entry, int inotifyFd, const string &tableName)
{
    string filepath = entry.path().string();
    auto createTime = fs::last_write_time(entry);
    time_t fileCreateTime = std::chrono::system_clock::to_time_t(createTime);
    uintmax_t fileSize = fs::file_size(entry);
    int wd = inotify_add_watch(inotifyFd, filepath.c_str(), IN_ALL_EVENTS);
    // 插入到数据库
    sqlPool.enqueue(insertFileData, conn, wd, filepath, fileCreateTime, fileSize, tableName);
    // 写日志
    string logData = "wd=" + to_string(wd) + " filePath=" + filepath + "\n";
    AsyncLogger::getInstance().start();
    AsyncLogger::getInstance().append(logData);
    // 这里如果单独开线程写反而更慢
    // sqlPool.enqueue([logData]() {
    //     cout<<"logData: "<<logData<<endl;
    //     // 向日志中添加消息
    //     AsyncLogger::getInstance().start();
    //     AsyncLogger::getInstance().append(logData);
    // });
}
