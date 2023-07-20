#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <ctime>
#include <fstream>
#include <thread>
#include <fcntl.h>

using namespace std;


#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <condition_variable>
#include <unistd.h>
using namespace std;

// 在这段代码中，使用了两个智能指针unique_ptr：currentBuffer和nextBuffer。
// currentBuffer和nextBuffer是Buffer类的对象指针，它们用于管理Buffer对象的生命周期。
// 在AsyncLogger类的构造函数中，使用new关键字创建了Buffer对象，并利用unique_ptr将其包装起来。
// 这样做可以确保在退出作用域或异常发生时，Buffer对象会被正确地销毁并释放其占用的内存。
// 在append函数中，通过调用std::move来移动currentBuffer到buffersList中，保证了缓冲区的所有权转移给了buffersList。
// 然后根据情况，将nextBuffer赋值给currentBuffer，或创建一个新的Buffer对象并赋值给currentBuffer。
// 在flushLog函数中，通过调用std::move将待刷新的缓冲区移动到局部变量bufferToFlush中，从而确保只有当前线程访问该缓冲区。
// 然后，在交换缓冲区之前，使用reset函数释放内存，以确保在交换之后正确地销毁旧的缓冲区。


// 缓冲区大小
const int BUFFER_SIZE = 1024;    

class AsyncLogger
{
private:
    // 私有类 Buffer，表示日志缓冲区。
    // Buffer的数据成员 data (string类型) 用于存储日志内容，logLenth (size_t类型) 表示当前日志内容的长度
    class Buffer
    {
    public:
        Buffer() : logLenth(0)
        {
            // 初始化成员变量，并预留 BUFFER_SIZE 大小的容量
            data.reserve(BUFFER_SIZE);
        }

        void append(const std::string &log)
        {
            // 如果当前位置加上要追加的日志长度不超过 BUFFER_SIZE
            if (logLenth + log.size() <= BUFFER_SIZE)
            {
                // 在缓冲区的末尾追加日志
                data.append(log);
                // 更新当前位置的值
                logLenth += log.size();
            }
        }

        // 通过把data和logLenth封装在私有变量中，并且函数加上const修饰，保证不会改变成员变量的值
        size_t size() const
        {
            // 返回缓冲区中已存储的字节数
            return logLenth;
        }

        const std::string &getData() const
        {
            // 返回缓冲区中的数据
            return data;
        }

    private:
        string data; // 存储日志数据的字符串
        size_t logLenth; 
    };

    // 在构造函数中，初始化了当前缓冲区和下一个缓冲区，并创建了一个刷新线程（写线程）。
    AsyncLogger() : currentBuffer(new Buffer), nextBuffer(new Buffer),
                    writeThread(&AsyncLogger::flushLog, this) {}

    AsyncLogger(const AsyncLogger &) = delete;            // 禁止拷贝构造
    AsyncLogger &operator=(const AsyncLogger &) = delete; // 禁止赋值运算符

    // flushLog() 函数是刷新线程的入口函数，在日志类被创建时就被调用，在构造函数中
    // 在循环中，刷新线程会等待直到有日志内容需要写入或者异步日志记录器停止运行。
    // 当有日志内容需要写入时，它会获取待刷新的缓冲区并进行日志写入操作
    void flushLog();

    // writeLogToFile() 函数用于将日志内容写入磁盘的文件中
    // 是创建一个文件流对象，用于以追加模式打开名为 "log.txt" 的文件
    void writeLogToFile(const string &log);

    // using BufferPtr = std::unique_ptr<Buffer>; // 使用 unique_ptr 智能指针来管理 Buffer 对象
    // using BufferList = std::list<BufferPtr>;   // 使用 list 容器存储 BufferPtr 对象

    unique_ptr<Buffer> currentBuffer; // 当前正在使用的缓冲区
    unique_ptr<Buffer> nextBuffer;    // 下一个可用的缓冲区
    list<unique_ptr<Buffer>> buffersList;      // 存储已填满的缓冲区

    std::thread writeThread;      // 刷新线程对象，用于将日志写入磁盘
    std::mutex mutex;                // 互斥锁，用于保护访问共享资源
    std::condition_variable condV; // 条件变量，用于线程间的同步和通信
    bool isRunning = false;            // 表示程序是否正在运行

public:
    // 懒汉式单例模式获取实例
    static AsyncLogger &getInstance()
    {
        static AsyncLogger instance;
        return instance;
    }

    // append() 函数，用于向日志缓冲区中添加日志内容。
    void append(const std::string &log);

    // start() 函数用于启动异步日志记录器
    void start();

    // stop() 函数用于停止异步日志记录器。该函数会设置运行标志为 false，唤醒等待的刷新线程并等待其结束
    void stop();
};

#endif // LOG_H
