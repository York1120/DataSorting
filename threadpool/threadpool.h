#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <mutex>

// class ThreadPool {
// private:
//     std::vector<std::thread> workers;
//     std::queue<std::function<void()>> tasks;
    
//     std::mutex mutex_;
//     std::condition_variable condition_;
//     bool stop;
// public:
//     // 构造函数接受一个参数 numThreads，表示线程池中的线程数量。
//     // 首先，创建了 numThreads 个工作线程，并将它们添加到 workers 容器中。每个工作线程都会循环执行以下操作：
//     // 等待条件变量 condition_ 的通知，当有任务时或者线程池停止时才继续执行。
//     // 取出队列 tasks 中的任务，并执行该任务
//     ThreadPool(size_t numThreads) : stop(false) {
//         for (size_t i = 0; i < numThreads; ++i) {
//             workers.emplace_back([this]() { // 和push_back效果一样，但是移动构造而不是拷贝构造
//                 while (true) {
//                     std::function<void()> task;
//                     {
//                         std::unique_lock<std::mutex> lock(mutex_);
//                         condition_.wait(lock, [this]() {
//                             return stop || !tasks.empty();
//                         });
                        
//                         if (stop && tasks.empty()) {
//                             return;
//                         }
                        
//                         task = std::move(tasks.front());
//                         tasks.pop();
//                     }
                    
//                     task();
//                 }
//             });
//         }
//     }

//     // 析构函数。首先，将 stop 设置为 true，表示线程池停止工作。
//     // 然后，通过 condition_.notify_all() 通知所有工作线程停止等待并退出。
//     // 最后，使用 worker.join() 等待所有工作线程执行完毕。
//     ~ThreadPool() {
//         {
//             std::unique_lock<std::mutex> lock(mutex_);
//             stop = true;
//         }
        
//         condition_.notify_all();
        
//         for (std::thread& worker : workers) {
//             worker.join();
//         }
//     }

//     template<class F, class... Args>
//     void enqueue(F&& f, Args&&... args) {
//         {
//             std::unique_lock<std::mutex> lock(mutex_);
            
//             tasks.emplace([f, args...]() {
//                 f(args...);
//             });
//         }
        
//         condition_.notify_one();
//     }
// };

#ifndef THREADPOOL_H
#define THREADPOOL_H



class ThreadPool {
public:
    ThreadPool(int numThreads) : stop(false) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();

        for (std::thread& thread : threads) {
            thread.join();
        }
    }

    template <class F, class... Args>
    void enqueue(F&& f, Args&&... args) {
        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([task]() { task(); });
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

#endif // THREADPOOL_H