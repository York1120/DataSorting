#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <mysql/mysql.h>

// 处理请求的具体逻辑
std::string processQuery(const std::string& input) {
    // 建立 MySQL 连接
    MYSQL mysql;
    mysql_init(&mysql);

    if (!mysql_real_connect(&mysql, "172.17.0.3", "root", "root", "test", 0, NULL, 0)) {
        std::cerr << "Failed to connect to database: " << mysql_error(&mysql) << std::endl;
        exit(0);
    }


    // 执行查询语句
    std::string sqlquery = "select FILE_PATH from files where FILE_PATH like '%"+input+"%';";
    // std::string sqlquery = "select * from files;";
    if (mysql_query(&mysql, sqlquery.c_str())) {
        std::cerr << "Failed to execute query: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        exit(0);
    }

    // 获取查询结果
    MYSQL_RES* result = mysql_store_result(&mysql);
    if (!result) {
        std::cerr << "Failed to get query result: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        exit(0);
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
    return response;
}

int main() {
    // 创建套接字
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 设置端口可复用
    int enableReuse = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(enableReuse)) == -1) {
        std::cerr << "Failed to set SO_REUSEADDR option" << std::endl;
        close(serverSocket);
        return 1;
    }

    // 绑定地址和端口
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(9006);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind address" << std::endl;
        close(serverSocket);
        return 1;
    }

    // 监听连接
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Failed to listen" << std::endl;
        close(serverSocket);
        return 1;
    }

    while (true) {
        // 接受连接
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            std::cerr << "Failed to accept connection" << std::endl;
            close(serverSocket);
            return 1;
        }

        // 读取请求
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead == -1) {
            std::cerr << "Failed to read request" << std::endl;
            close(clientSocket);
            close(serverSocket);
            return 1;
        }

        // 解析查询参数
        std::string request(buffer);
        std::size_t start = request.find("query=");
        std::size_t end = request.find(" ", start);
        std::string query = request.substr(start + 6, end - (start + 6));

        // 处理查询并生成响应
        std::string response = processQuery(query);

        // 发送响应
        std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + response;
        ssize_t bytesSent = send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
        if (bytesSent == -1) {
            std::cerr << "Failed to send response" << std::endl;
            close(clientSocket);
            close(serverSocket);
            return 1;
        }

        // 关闭连接
        close(clientSocket);
    }

    // 关闭服务器套接字
    close(serverSocket);

    return 0;
}
