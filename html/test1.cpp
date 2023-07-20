#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// 处理HTTP请求，并返回响应
std::string processRequest(const std::string& request) {
    // 在这里实现你的逻辑，处理请求并生成响应
    std::string response = "Hello, World!"+request+";";
    return response;
}

// 处理HTTP请求并返回响应
std::string handleRequest(const std::string& request) {
    std::string response;

    // 处理GET请求
    if (request.substr(0, 3) == "GET") {
        response = processRequest(request);
    } else {
        response = "Invalid request method.";
    }

    // 构建HTTP响应报文
    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(response.length()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" + response;

    return httpResponse;
}

int main() {
    // 创建套接字
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    // 绑定地址和端口
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(9006);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind." << std::endl;
        close(serverSocket);
        return -1;
    }

    // 开始监听
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Failed to listen." << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server started. Listening on port 9006" << std::endl;

    while (true) {
        // 接受连接
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            std::cerr << "Failed to accept connection." << std::endl;
            close(serverSocket);
            return -1;
        }

        // 读取请求
        char buffer[4096];
        std::string request;

        ssize_t bytesRead;
        do {
            bytesRead = read(clientSocket, buffer, sizeof(buffer));
            if (bytesRead > 0) {
                request += std::string(buffer, bytesRead);
            }
        } while (bytesRead == sizeof(buffer));

        // 处理请求并发送响应
        std::string response = handleRequest(request);
        write(clientSocket, response.c_str(), response.length());
        
        // 关闭连接
        close(clientSocket);
    }

    // 关闭套接字
    close(serverSocket);

    return 0;
}
