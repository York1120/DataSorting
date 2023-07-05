// SendMailTest.cpp
 
#include "SendMail.h"
 
#include <iostream> 
#include <string>
 
int main()
{
  Mail mail;
  char EmailTo[] = "mynameisyork@163.com";  //此处是送达的邮箱
  char EmailFrom[] = "1102427144@qq.com"; //发送邮箱
  char EmailKey[] = "wtxacamrehsaigjf";       //发送邮箱的密码
  char SmtpAddr[] = "smtp.qq.com"; //SMTP服务器地址
  
  string EmailContents = "From: \"NAS大文件超时提醒\"<"+string(EmailFrom)+">\r\n"
              + string("To: \"dasiy\"<"+string(EmailTo)+">\r\n")
              + "Subject: 文件清理提醒\r\n\r\n"
              + "您的大文件(超过10G)已超过两个月未清理,请及时清理\n";
  printf("send comment:\n%s", EmailContents.c_str());
  mail.SendMail(SmtpAddr, EmailTo, EmailContents.c_str(), EmailFrom, EmailKey);
  getchar();
  return 0;
}