// SendMail.cpp
 
#include "SendMail.h"
 
#pragma comment(lib, "ws2_32.lib")
 
#ifdef __linux__
#define Sleep sleep
#define closesocket close
 
#define strcpy_s(buffer, size, arg) \
	do{ strcpy(buffer, arg); } while (0) 
 
#define sprintf_s(buffer, size, arg...) \
	do{ sprintf(buffer, ##arg); } while (0) 
#endif // __linux__
 
// 定义了一个结构体Base64Date6，用来存储Base64编码数据。
struct Base64Date6
{
  unsigned int d4 : 6;
  unsigned int d3 : 6;
  unsigned int d2 : 6;
  unsigned int d1 : 6;
};
 
// 用于将一个字符转换为Base64字符。
char Mail::ConvertToBase64(char uc)
{
  if (uc < 26)
  {
    return 'A' + uc;
  }
  if (uc < 52)
  {
    return 'a' + (uc - 26);
  }
  if (uc < 62)
  {
    return '0' + (uc - 52);
  }
  if (uc == 62)
  {
    return '+';
  }
  return '/';
}
 
// base64的实现,用于将输入的字符串进行Base64编码。
void Mail::EncodeBase64(char *dbuf, char *buf128, int len)
{
  struct Base64Date6 *ddd = NULL;
  int      i = 0;
  char     buf[256] = { 0 };
  char     *tmp = NULL;
  char     cc = '\0';
 
  memset(buf, 0, 256);
  strcpy_s(buf, 256, buf128);
 
  for (i = 1; i <= len / 3; i++)
  {
    tmp = buf + (i - 1) * 3;
    cc = tmp[2];
    tmp[2] = tmp[0];
    tmp[0] = cc;
    ddd = (struct Base64Date6 *)tmp;
    dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
    dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
    dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
    dbuf[(i - 1) * 4 + 3] = ConvertToBase64((unsigned int)ddd->d4);
  }
  if (len % 3 == 1)
  {
    tmp = buf + (i - 1) * 3;
    cc = tmp[2];
    tmp[2] = tmp[0];
    tmp[0] = cc;
    ddd = (struct Base64Date6 *)tmp;
    dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
    dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
    dbuf[(i - 1) * 4 + 2] = '=';
    dbuf[(i - 1) * 4 + 3] = '=';
  }
  if (len % 3 == 2)
  {
    tmp = buf + (i - 1) * 3;
    cc = tmp[2];
    tmp[2] = tmp[0];
    tmp[0] = cc;
    ddd = (struct Base64Date6 *)tmp;
    dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
    dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
    dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
    dbuf[(i - 1) * 4 + 3] = '=';
  }
  return;
}
// 发送邮件
void Mail::SendMail(char *smtpAddr, char *emailTo, const char *body,char* emailFrom, char* emailKey)
{
  int   sockfd = { 0 };
  char  buf[1500] = { 0 };
  char  rbuf[1500] = { 0 };
  char  login[128] = { 0 };
  char  pass[128] = { 0 };
  
  struct sockaddr_in their_addr = { 0 };
#ifdef WIN32
  WSADATA WSAData;
  WSAStartup(MAKEWORD(2, 2), &WSAData);
#endif // WIN32
  memset(&their_addr, 0, sizeof(their_addr));
 
  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons(25);  // 一般是25端口不需要改
  hostent* hptr = gethostbyname(smtpAddr);     // SMTP服务器地址
#ifdef WIN32
  memcpy(&their_addr.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);
  printf("IP of smpt.163.com is : %d:%d:%d:%d\n",
	  their_addr.sin_addr.S_un.S_un_b.s_b1,
	  their_addr.sin_addr.S_un.S_un_b.s_b2,
	  their_addr.sin_addr.S_un.S_un_b.s_b3,
	  their_addr.sin_addr.S_un.S_un_b.s_b4);
#else
  memcpy(&their_addr.sin_addr.s_addr, hptr->h_addr_list[0], hptr->h_length);
  printf("IP of smpt.qq.com is : %d:%d:%d:%d\n",
	  their_addr.sin_addr.s_addr & 0XFF,
	  their_addr.sin_addr.s_addr>>8 & 0XFF,
	  their_addr.sin_addr.s_addr>>16 & 0XFF,
	  their_addr.sin_addr.s_addr>>24 & 0XFF);
#endif
  // 连接邮件服务器，如果连接后没有响应，则2 秒后重新连接
  sockfd = OpenSocket((struct sockaddr *)&their_addr);
  memset(rbuf, 0, 1500);
  while (recv(sockfd, rbuf, 1500, 0) == 0)
  {
    cout << "reconnect..." << endl;
    Sleep(2);
    sockfd = OpenSocket((struct sockaddr *)&their_addr);
    memset(rbuf, 0, 1500);
  }
 
  cout << rbuf << endl;
 
  // EHLO
  memset(buf, 0, 1500);
  sprintf_s(buf, 1500, "EHLO HYL-PC\r\n"); // 发送 EHLO 命令
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);  // 接收回复
  cout << "EHLO REceive: " << rbuf << endl;
 
  //发送命令: AUTH LOGIN 请求登陆！
  memset(buf, 0, 1500);
  sprintf_s(buf, 1500, "AUTH LOGIN\r\n");   // 发送 AUTH LOGIN 命令
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);  // 接收回复
  cout << "Auth Login Receive: " << rbuf << endl;
 
  // USER 发送自己账户的用户名的base64编码
  memset(buf, 0, 1500);
  sprintf_s(buf, 1500, "%s",emailFrom);// 邮箱账号
  memset(login, 0, 128);
  EncodeBase64(login, buf, strlen(buf)); // 对账号进行base64编码
  sprintf_s(buf, 1500, "%s\r\n", login); // 格式化发送命令
  send(sockfd, buf, strlen(buf), 0); // 发送命令
  cout << "Base64 UserName: " << buf << endl;
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);  // 接收回复
  cout << "User Login Receive: " << rbuf << endl;
 
  // PASSWORD 发送自己的密码的base64编码
  sprintf_s(buf, 1500, "%s",emailKey);// 邮箱密码
  memset(pass, 0, 128);
  EncodeBase64(pass, buf, strlen(buf)); // 对密码进行base64编码
  sprintf_s(buf, 1500, "%s\r\n", pass);  // 格式化发送命令
  send(sockfd, buf, strlen(buf), 0);  // 发送命令
  cout << "Base64 Password: " << buf << endl;
 
  //认证授权确认
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);  // 接收回复
  cout << "Send Password Receive: " << rbuf << endl;
 
  // MAIL FROM 开始发送邮件，先发送邮件说明:MAIL FROM 
  memset(buf, 0, 1500);
  sprintf_s(buf, 1500, "MAIL FROM: <%s>\r\n",emailFrom); //此处要和发邮件的邮箱保持一致
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout << "set Mail From Receive: " << rbuf << endl;
 
  // RCPT TO 第一个收件人,发送目的邮箱说明:RCPT TO
  sprintf_s(buf, 1500, "RCPT TO:<%s>\r\n", emailTo);  // 设置收件人邮箱
  send(sockfd, buf, strlen(buf), 0);  // 发送命令
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);  // 接收回复
  cout << "Tell Sendto Receive: " << rbuf << endl;
 
  // DATA 准备开始发送邮件内容
  /*
    发送数据格式：
     From: **_test@163.com
     To: **_a_test@163.com
     Subject: 测试一下
     MIME-Version: 1.0
  */
  sprintf_s(buf, 1500, "DATA\r\n");  // 准备发送邮件内容
  send(sockfd, buf, strlen(buf), 0);  // 发送命令
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);  // 接收回复
  cout << "Send Mail Prepare Receive: " << rbuf << endl;
 
  // 发送邮件内容，\r\n.\r\n内容结束标记
  sprintf_s(buf, 1500, "%s\r\n.\r\n", body);  // 发送邮件内容
  send(sockfd, buf, strlen(buf), 0); // 发送命令
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);  // 接收回复
  cout << "Send Mail Receive: " << rbuf << endl;
 
  // QUIT quit命令关闭与服务器的连接
  sprintf_s(buf, 1500, "QUIT\r\n"); // 发送退出命令
  send(sockfd, buf, strlen(buf), 0);
  memset(rbuf, 0, 1500);
  recv(sockfd, rbuf, 1500, 0);
  cout << "Quit Receive: " << rbuf << endl;
 
  //清理工作
  closesocket(sockfd);  // 关闭socket
#ifdef WIN32
  WSACleanup();
#endif
  return;
}
// 打开TCP Socket连接
int Mail::OpenSocket(struct sockaddr *addr)
{
  int sockfd = 0;
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    cout << "Open sockfd(TCP) error!" << endl;
    exit(-1);    
  }
  if (connect(sockfd, addr, sizeof(struct sockaddr)) < 0)
  {
    cout << "Connect sockfd(TCP) error!" << endl;
    exit(-1);    
  }
  return sockfd;
}