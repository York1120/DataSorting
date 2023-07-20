#include "log.h"
#include <ctime>
#include <unistd.h>

// g++ logTest.cpp log.cpp -o app -pthread

int main()
{
    AsyncLogger::getInstance().start();

    // 模拟多个线程写日志
    std::vector<std::thread> threads;
    for (int i = 0; i < 50; ++i)
    {
        // 创建一个新线程并添加到线程向量中
        threads.emplace_back([i]()
                             {
            for (int j = 0; j < 100; ++j) {
                // 向日志中添加消息
                AsyncLogger::getInstance().append("Thread " + to_string(i) + ": Log message " + to_string(j) + "\n");
            } });
    }

    // sleep(5);

    // 等待所有线程完成
    for (auto &thread : threads)
    {
        thread.join();
    }

    AsyncLogger::getInstance().stop();

    return 0;
}

