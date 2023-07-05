#include "SqlCreate.h"

sql_create::sql_create(string& host, string& user, string& passwd, string& db_name, string& table_name, string& folder_path){
        DB_HOST=host;
        DB_USER=user;
        DB_PASSWORD=passwd;
        DB_NAME=db_name;
        TABLE_NAME=table_name;
        FOLDER_PATH=folder_path;
    };

void sql_create::createTable(MYSQL* conn){
    std::string query = "CREATE TABLE IF NOT EXISTS " + TABLE_NAME + " ("
                        "ID INT AUTO_INCREMENT PRIMARY KEY,"
                        "FILE_PATH VARCHAR(255) NOT NULL,"
                        "CREATION_TIME VARCHAR(255) NOT NULL,"
                        "FILE_SIZE BIGINT NOT NULL"
                        ");";
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Failed to create table: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        exit(1);
    }
}


void sql_create::storeFilesToDatabase(const std::string& folderPath, MYSQL* conn) {
    DIR* dir;
    struct dirent* entry;
    
    // 打开指定文件夹，并将实例保存在dir变量中
    dir = opendir(folderPath.c_str());
    if (dir == nullptr) {
        std::cerr << "Failed to open directory: " << folderPath << std::endl;
        return;
    }

    // 循环遍历文件夹中的每个条目，直到读取完所有条目为止
    while ((entry = readdir(dir)) != nullptr) {
        // 将当前条目的文件名存储在fileName变量中
        const std::string fileName = entry->d_name;
        // 检查文件名不是当前目录"."或上级目录"..""
        if (fileName != "." && fileName != "..") {
            // 构建文件的完整路径，通过将所在目录路径folderPath与文件名fileName拼接而成
            const std::string filePath = folderPath + "/" + fileName;
            // fileStat是用于存储文件状态信息的结构体变量
            struct stat fileStat;
            
            // 使用stat()函数获取文件的状态信息，并将结果存储在fileStat变量中，成功返回0
            if (stat(filePath.c_str(), &fileStat) == 0) {
                // 检查文件的模式是否是普通文件，必须要是普通文件才能调用fileStat.st_ctime和fileStat.st_size！
                if (S_ISREG(fileStat.st_mode)) {
                    // 将文件路径插入到数据库表中
                    string fileCreationTime=ctime(&fileStat.st_ctime);
                    string fileSize=to_string(fileStat.st_size);
                    string query = "INSERT INTO " + TABLE_NAME + " ("
                    "FILE_PATH, CREATION_TIME, FILE_SIZE) VALUES ('" + filePath + "', '" 
                    + fileCreationTime + "', " + fileSize + ");";
                    // 执行对应SQL语句
                    if (mysql_query(conn, query.c_str()) != 0) {
                        std::cerr << "Failed to execute query: " << mysql_error(conn) << std::endl;
                        exit(1);
                    }
                }
                // 检查是否是文件夹
                else if (S_ISDIR(fileStat.st_mode)) {
                    // 如果是文件夹，调用自身递归处理子文件夹
                    storeFilesToDatabase(filePath, conn);
                }
            }
        }
    }
    
    closedir(dir);
}