#include <string>
#include <vector>
#include <unordered_map>
#include <sys/inotify.h>
#include <iostream>
using namespace std;


#ifndef INOTIFY_H
#define INOTIFY_H

void displayInotifyEvent(struct inotify_event *e) // 传入path参数用于后面输出
{
    if (e->mask & IN_CREATE)
        cout << "在监听目录内创建了"
             << "wd=" << e->wd << "文件/目录" << endl;
    if (e->mask & IN_DELETE)
        cout << "在监听目录内删除了"
             << "wd=" << e->wd << "文件/目录" << endl;
    if (e->mask & IN_DELETE_SELF)
        cout << "wd=" << e->wd << "文件被删除" << endl;
    if (e->mask & IN_MODIFY)
        cout << "wd=" << e->wd << "文件被修改" << endl;
    if (e->mask & IN_MOVE_SELF)
        cout << "wd=" << e->wd << "文件本身被移动" << endl;
    if (e->mask & IN_MOVE)
        cout << "wd=" << e->wd << "文件被移动" << endl;

    return;
}


#endif