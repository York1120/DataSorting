#include "log.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getCurrentTimeAsString()
{
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 将时间点转换为时间结构体
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // 转换为本地时间
    std::tm *localTime = std::localtime(&currentTime);

    // 格式化时间字符串
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

// flushLog() 函数是刷新线程的入口函数，在日志类被创建时就被调用，在构造函数中
// 在循环中，刷新线程会等待直到有日志内容需要写入或者异步日志记录器停止运行。
// 当有日志内容需要写入时，它会获取待刷新的缓冲区并进行日志写入操作
void AsyncLogger::flushLog()
{
    while (isRunning)
    {
        std::unique_lock<std::mutex> lock(mutex);
        // 等待条件变量满足条件
        condV.wait(lock, [this]()
                   { return !buffersList.empty() || !isRunning; });
        // 如果有待刷新的缓冲区
        if (!buffersList.empty())
        {
            // 获取待刷新的缓冲区
            unique_ptr<Buffer> bufferToFlush = std::move(buffersList.front());
            // 从缓冲区队列中移除已刷新的缓冲区
            buffersList.pop_front();
            lock.unlock();

            // 将日志数据写入磁盘
            writeLogToFile(bufferToFlush->getData());

            // 交换缓冲区
            std::lock_guard<std::mutex> swapLock(mutex);
            if (bufferToFlush == currentBuffer)
            {
                currentBuffer.reset(new Buffer);
            }
        }
    }
}

// writeLogToFile() 函数用于将日志内容写入磁盘的文件中
// 是创建一个文件流对象，用于以追加模式打开名为 "log.txt" 的文件
void AsyncLogger::writeLogToFile(const string &log)
{
    // cout<<"正在写入日志到磁盘"<<endl;
    // ios_base::app 表示以追加模式打开文件，即如果文件已经存在，新的内容将会添加到文件的末尾而不是覆盖原有内容。
    ofstream file("./log/log.txt", ios_base::app);
    if (file.is_open())
    {
        file << log;
        file.close();
    }
    else
    {
        std::cerr << "Failed to open log file." << std::endl;
    }
}

// append() 函数，用于向日志缓冲区中添加日志内容。
void AsyncLogger::append(const std::string &log)
{
    std::lock_guard<std::mutex> lock(mutex);
    string currentTime = getCurrentTimeAsString();
    // 如果当前缓冲区已满
    if (currentTime.size() + currentBuffer->size() + log.size() > BUFFER_SIZE)
    {
        // 将当前缓冲区移动到缓冲区列表中
        buffersList.push_back(std::move(currentBuffer));
        // 如果下一个缓冲区存在
        if (nextBuffer)
        {
            currentBuffer = std::move(nextBuffer);
        }
        else
        {
            // 如果下一个缓冲区不存在,创建一个新的缓冲区
            currentBuffer.reset(new Buffer);
        }
        // 唤醒等待的刷新线程
        condV.notify_one();
    }
    // 将日志内容添加到当前缓冲区
    currentBuffer->append(currentTime + " " + log);
}

// start() 函数用于启动异步日志记录器
void AsyncLogger::start()
{
    isRunning = true;
}

// stop() 函数用于停止异步日志记录器。该函数会设置运行标志为 false，唤醒等待的刷新线程并等待其结束
void AsyncLogger::stop()
{
    isRunning = false;
    // 通知条件变量，唤醒在该条件上等待的线程
    condV.notify_one();
    // 等待刷新线程的结束
    writeThread.join();
}