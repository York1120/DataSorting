#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring>
using namespace std;


class sql_create
{
public:
    sql_create(string& host, string& user, string& passwd, string& db_name, string& table_name, string& folder_path);
    ~sql_create(){};
    // 递归地将指定文件夹内的所有文件存储到数据库中
    void storeFilesToDatabase(const std::string& folderPath, MYSQL* conn);
    void createTable(MYSQL* conn);
    // void sql_init(string host, string user, string passwd, string db_name, string table_name);

    
    string DB_HOST;
    string DB_USER;
    string DB_PASSWORD;
    string DB_NAME;
    string TABLE_NAME;
    string FOLDER_PATH;

};