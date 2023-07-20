#include <iostream>
#include <httplib.h>
#include <mysql/mysql.h>

void handle_request(const httplib::Request& req, httplib::Response& res) {

    // 建立 MySQL 连接
    MYSQL mysql;
    mysql_init(&mysql);

    if (!mysql_real_connect(&mysql, "172.17.0.3", "root", "root", "test", 0, NULL, 0)) {
        std::cerr << "Failed to connect to database: " << mysql_error(&mysql) << std::endl;
        return ;
    }

    std::string input = req.get_param_value("query");

    // 执行查询语句
    std::string sqlquery = "select FILE_PATH from files where FILE_PATH like '%"+input+"%';";
    // std::string sqlquery = "select * from files;";
    if (mysql_query(&mysql, sqlquery.c_str())) {
        std::cerr << "Failed to execute query: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        return ;
    }

    // 获取查询结果
    MYSQL_RES* result = mysql_store_result(&mysql);
    if (!result) {
        std::cerr << "Failed to get query result: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        return ;
    }

    std::string response;

    // 输出符合条件的结果
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < mysql_num_fields(result); ++i) {
            response += "<p>";
            response += "<a href=\"http://localhost:84";
            response += row[i];
            response += "\" target=\"_blank\">图片:";
            response += row[i];
            response += "</a>";
            response += "</p>";
        }
    }

    // 释放资源并关闭连接
    mysql_free_result(result);
    mysql_close(&mysql);

    // 发回响应页面
    res.set_content(response, "text/html");
}

int main() {
    httplib::Server svr;
    svr.Get("/search", handle_request);
    svr.listen("localhost", 9006);

    return 0;
}