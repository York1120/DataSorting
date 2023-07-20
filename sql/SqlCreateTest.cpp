#include <unistd.h> // sleep函数头文件
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "SqlCreate.h"
#include "../config.h"
#include "../threadpool/threadpool.h"
#include "../inotify/inotify.h"

#define SHM_KEY 12345
#define TABLE_SIZE 100

int main()
{
    MYSQL *conn;
    sql_create mydb(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, TABLE_NAME);

    // 初始化MYSQL类
    conn = mysql_init(nullptr);
    if (conn == nullptr)
    {
        std::cerr << "Failed to initialize MySQL connection" << std::endl;
        return 1;
    }

    // 初始化mysql连接
    mydb.initSql(conn);

    // 创建存储文件路径的数据库表
    mydb.createTable(conn);

    // 创建 inotify 实例
    int inotifyFd = inotify_init();

    // 创建含有3个线程的线程池，一个用于监控目录、更新数据库，一个用于遍历目录
    ThreadPool pool(3);

    pool.enqueue(sql_create::traverseDirectory, conn, directories, inotifyFd, TABLE_NAME);

    cout << "结束" << endl;
    // buf的大小要注意，要留够空间！
    char buf[1024000];
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    while (true)
    {
        numRead = read(inotifyFd, buf, sizeof(buf)); // 读取事件
        if (numRead == -1)
        {
            cerr << "Failed to read events" << endl;
            break;
        }

        cout << "Read " << numRead << " bytes from inotify fd " << inotifyFd << endl; // 显示读取的字节数
        for (p = buf; p < buf + numRead;)
        {
            event = (struct inotify_event *)p; // 获取事件
            displayInotifyEvent(event);        // 显示事件信息
            p += sizeof(struct inotify_event) + event->len;
        }
    }

    // // 创建共享内存
    // int SHARED_MEMORY_SIZE = 100 * 1024 * 1024; // 共享内存大小(100m)
    // void* sharedMemory = mmap(nullptr, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // if (sharedMemory == MAP_FAILED) {
    //     std::cerr << "Failed to create shared memory" << std::endl;
    //     return 1;
    // }

    // unordered_map<int,string> path_map;
    // Logger* logger = Logger::getInstance();

    // LogData* log = static_cast<LogData*>(logger->sharedMem);

    // // 创建多个子进程
    // for (const auto& directory : directories) {
    //     pid_t pid = fork();

    //     if (pid < 0) {
    //         std::cerr << "Failed to create child process." << std::endl;
    //         return 1;
    //     } else if (pid == 0) {
    //         // 给每个子进程都创建mysql连接
    //         MYSQL* conn_child = mysql_init(NULL);
    //         if (!mysql_real_connect(conn_child, DB_HOST.c_str(), DB_USER.c_str(), DB_PASSWORD.c_str(), DB_NAME.c_str(), 0, nullptr, 0)) {
    //             std::cerr << "Failed to connect to database: " << mysql_error(conn_child) << std::endl;
    //             return 1;
    //         }

    //         // 子进程遍历目录，存储文件到数据库
    //         mydb.traverseDirectory(conn_child,directory,inotifyFd,logger,log);
    //         mysql_close(conn_child);
    //         exit(0);
    //     }else{
    //         // 父进程逻辑
    //         cout<<"这一步写父进程逻辑"<<endl;
    //         // 父进程等待子进程执行完毕，回收子进程，才能开始遍历inotify表
    //         wait(nullptr);

    //         // 回收子进程后父进程一直监听
    //         // buf的大小要注意，要留够空间！
    //         char buf[1024000];
    //         ssize_t numRead;
    //         char *p;
    //         struct inotify_event *event;

    //         while (true)
    //         {
    //             numRead = read(inotifyFd, buf, 1024); // 读取事件
    //             if (numRead == -1)
    //             {
    //                 cerr << "Failed to read events" << endl;
    //                 break;
    //             }

    //             cout << "Read " << numRead << " bytes from inotify fd " << inotifyFd << endl; // 显示读取的字节数
    //             for (p = buf; p < buf + numRead;)
    //             {
    //                 event = (struct inotify_event *)p; // 获取事件
    //                 displayInotifyEvent(event, path_map); // 显示事件信息
    //                 p += sizeof(struct inotify_event) + event->len;
    //             }
    //         }

    //     }
    // }

    // // 释放共享内存
    // munmap(sharedMemory, SHARED_MEMORY_SIZE);

    // 关闭数据库连接
    mysql_close(conn);

    return 0;
}