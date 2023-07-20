#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <functional>
#include <condition_variable>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <sstream>   // ostringstream头文件
#include <iomanip>   // put_time函数头文件
#include <algorithm> // find函数头文件
#include <chrono>
#include <thread>
#include <experimental/filesystem>
#include <filesystem>
#include <unordered_map>
#include <sys/inotify.h>
#include "../log/log.h"
#include "../threadpool/threadpool.h"
#include "../lock/locker.h"
using namespace std;
namespace fs = std::experimental::filesystem;

class sql_create
{
public:
    sql_create(const string &host, const string &user, const string &passwd, const string &dbname, const string &tbname);
    ~sql_create(){};

    // 监听文件描述符和路径映射表
    // unordered_map<int,string> path_map;
    // 初始化mysql
    void initSql(MYSQL *conn);

    // 创建数据库表
    void createTable(MYSQL *conn);
    // 删除文件信息从数据库
    void deleteFileData(MYSQL *conn, const std::string &filepath);
    // 插入文件信息到数据库
    static void insertFileData(MYSQL *conn, const int &wd, const std::string &filepath, const std::time_t &creationTime, const std::uintmax_t &fileSize, const string &tableName);
    // filesystem遍历所有文件，存入数据库，并设置监听
    static void traverseDirectory(MYSQL *conn, const std::vector<std::string> &folderPaths, int inotifyFd, const string &tableName);
    // 设置inotify监控，插入数据库
    static void setInotify(MYSQL *conn, const std::experimental::filesystem::v1::__cxx11::directory_entry &entry, int inotifyFd, const string &tableName);

private:
    string m_DB_HOST;
    string m_DB_USER;
    string m_DB_PASSWORD;
    string m_DB_NAME;
    string m_TABLE_NAME;
};