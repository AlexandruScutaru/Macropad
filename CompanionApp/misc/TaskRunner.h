#pragma once 

#include <QCoreApplication>
#include <QMetaObject>

#include <concepts>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>


namespace async {
    class TaskRunner {
    public:
        explicit TaskRunner(unsigned int threadCount = std::thread::hardware_concurrency()) {
            if (threadCount == 0) {
                threadCount = 1;
            }

            for (auto i = 0; i < threadCount; i++) {
                mWorkers.emplace_back([this](std::stop_token stoken) {
                    while (!stoken.stop_requested()) {
                        std::function<void()> task;
                        
                        {
                            std::unique_lock lock(mMutex);
                            mCV.wait(lock, stoken, [this]() { return !mTasks.empty(); });

                            if (stoken.stop_requested() && mTasks.empty()) {
                                return;
                            }

                            if (!mTasks.empty()) {
                                task = std::move(mTasks.front());
                                mTasks.pop();
                            }
                        }

                        if (task) {
                            task();
                        }
                    }
                });
            }
        }

        ~TaskRunner() {
            stop(); 
        }

        template <typename Callable>
        requires std::invocable<Callable>
        void run(Callable&& task) {
            {
                std::scoped_lock lock(mMutex);
                mTasks.emplace(std::forward<Callable>(task));
            }

            mCV.notify_one();
        }

        template <typename Callable>
        requires std::invocable<Callable>
        auto mainThreadProxy(Callable&& callback, QObject* context = qApp) {
            return [context, callback = std::forward<Callable>(callback)]<typename... Args>(Args&&... args) {
                auto task = [callback, argsTuple = std::make_tuple(std::forward<Args>(args)...)]() {
                    std::apply(callback, argsTuple);
                };

                // queues the task on the context's thread.
                // if called from another thread, Qt packages this into an event and processes it in the main UI loop.
                QMetaObject::invokeMethod(context, std::move(task), Qt::QueuedConnection);
            };
        }

        void stop() {
            {
                std::scoped_lock lock(mMutex);
                std::queue<std::function<void()>> emptyQueue;
                std::swap(mTasks, emptyQueue);
            }
        
            for (auto& worker : mWorkers) {
                worker.request_stop();
            }

            mCV.notify_all();
        }

    private:
        std::vector<std::jthread> mWorkers;
        std::queue<std::function<void()>> mTasks;
        std::mutex mMutex;
        std::condition_variable_any mCV;

    };
}
