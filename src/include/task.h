#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <iostream>
#include <thread>

namespace TaskBase
{
    class Task;

    using TaskPtr = std::shared_ptr<Task>;

    class Task : public std::enable_shared_from_this<Task>
    {
    public:
        explicit Task(std::function<void()> routine);

        void execute();
        void addDependency(const TaskPtr& task);
        void addSubscriber(const TaskPtr& task);
        bool isTaskReady() const;
        void waitForTaskReady();

    private:
        std::function<void()> _routine;
        std::vector<TaskPtr> _dependencies;
        std::vector<TaskPtr> _subscribers;
        std::atomic<unsigned int> _unfulfilledDependencies {0};
        std::mutex _lock;
        std::condition_variable _waitReady;

        void notifyTaskFinished(const TaskPtr& task);
    };
}

#endif  // TASK_H
