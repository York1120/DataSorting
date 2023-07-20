#include <unistd.h> // sleep函数头文件
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sql/SqlCreate.h"
#include "config.h"
#include "threadpool/threadpool.h"
#include "inotify/inotify.h"

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

    
    // 关闭数据库连接
    mysql_close(conn);

    return 0;
}