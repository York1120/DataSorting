#include <string>
#include <vector>
#include <unordered_map>
#include <sys/inotify.h>
#include <iostream>
using namespace std;

#ifndef CONFIG_H
#define CONFIG_H

extern const string DB_HOST = "172.17.0.1";                                     // 数据库IP地址
extern const string DB_USER = "root";                                           // 数据库用户名
extern const string DB_PASSWORD = "root";                                       // 数据库密码
extern const string DB_NAME = "test";                                           // 数据库名
extern const string TABLE_NAME = "files";                                       // 数据库表名
extern const std::vector<std::string> directories = {"/windows/test", "/root"}; // 要遍历的文件夹

#endif
