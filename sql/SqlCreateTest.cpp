#include "SqlCreate.h"

string DB_HOST = "172.17.0.3"; // 数据库IP地址
string DB_USER = "root"; // 数据库用户名
string DB_PASS = "root"; // 数据库密码
string DB_NAME = "test"; // 数据库名
string TABLE_NAME = "files"; // 数据库表名
string FOLDER_PATH = "/windows"; // 遍历文件夹路径



int main() {
    MYSQL* conn;
    sql_create mydb(DB_HOST, DB_USER, DB_PASS, DB_NAME, TABLE_NAME, FOLDER_PATH);

    // 初始化 MySQL 连接
    conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "Failed to initialize MySQL connection" << std::endl;
        return 1;
    }

    

    // 连接到数据库
    if (!mysql_real_connect(conn, DB_HOST.c_str(), DB_USER.c_str(), DB_PASS.c_str(), DB_NAME.c_str(), 0, nullptr, 0)) {
        std::cerr << "Failed to connect to MySQL database: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return 1;
    }

    // 如果数据库表已经存在，则删除原先的表再重新构建
    string dropTableQuery = "DROP TABLE IF EXISTS " + TABLE_NAME + ";";
    if (mysql_query(conn, dropTableQuery.c_str()) != 0) {
        std::cerr << "Failed to drop old table: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return 1;
    }

    // 创建存储文件路径的数据库表
    mydb.createTable(conn);


    // 存储文件到数据库
    mydb.storeFilesToDatabase(FOLDER_PATH, conn);

    // 关闭数据库连接
    mysql_close(conn);

    return 0;
}