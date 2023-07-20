// SendMail.h
#ifndef _SEND_MAIL_H_
#define _SEND_MAIL_H_
 
#ifdef WIN32
#include <windows.h>
#include <WinSock.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <stdio.h>
#include <iostream>
 
using namespace std;
 
class Mail{
public:
    Mail(){};
    ~Mail(){};
    // 协议中加密部分使用的是base64方法
    char ConvertToBase64(char c6); // 把6位字符转换为Base64编码字符
    void EncodeBase64(char *dbuf, char *buf128, int len); // 对缓冲区进行Base64编码
    void SendMail(char *smtpAddr, char *emailTo, const char *body, char* emailFrom, char* emailKey); // 发送邮件
    int OpenSocket(struct sockaddr *addr);  // 打开socket套接字

};

 
#endif