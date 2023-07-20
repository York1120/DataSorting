#include <iostream>
#include <fstream>
#include <mysql/mysql.h>
using namespace std;

string DB_HOST = "172.17.0.3"; // 数据库IP地址
string DB_USER = "root"; // 数据库用户名
string DB_PASS = "root"; // 数据库密码
string DB_NAME = "test"; // 数据库名
string TABLE_NAME = "files"; // 数据库表名

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // 初始化连接句柄
    conn = mysql_init(NULL);

    // 连接到 MySQL 服务器
    if (!mysql_real_connect(conn, DB_HOST.c_str(), DB_USER.c_str(), DB_PASS.c_str(), DB_NAME.c_str(), 0, NULL, 0)) {
        std::cout << "连接失败: " << mysql_error(conn) << std::endl;
        return 1;
    }

    // 执行查询语句
    string selectTableQuery="SELECT * FROM " + TABLE_NAME + ";";
    if (mysql_query(conn, selectTableQuery.c_str())) {
        std::cout << "查询失败: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return 1;
    }

   // 获取查询结果
    res = mysql_use_result(conn);

    // 连接查询结果的字符串
    std::string resultStr;

    while ((row = mysql_fetch_row(res))) {
        // row[0]是id主键，row[1]是FILE_NAME字段
        std::string column = row[1];
        // link为点击后转到的链接
        std::string link = "http://localhost:84" + column; // 加上 URL 前缀
        // 在web页面中只显示column的部分，即<a href=\链接\>后的部分，点击后则转到链接
        resultStr += "<a href=\"" + link + "\">" + column + "</a><br>"; 
    }

    // 将连接后的字符串保存到 HTML 文件
    std::ofstream file("./file.html");
    // 将 <html><body>、resultStr 以及 </body></html> 拼接成一个完整的 HTML 页面，并将该页面的内容写入到指定的文件中。
    file << "<html><body>" << resultStr << "</body></html>";
    file.close();

    // 释放资源
    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}