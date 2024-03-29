#ifndef SCHDEULE_H
#define SCHDEULE_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

#include "task.h"

namespace Schedule
{
    class Scheduler
    {
    public:
        void addTask(TaskBase::TaskPtr task);
        void scheduleTaskAfter(TaskBase::TaskPtr task, unsigned int milliseconds);
        void scheduleTaskPeriodically(TaskBase::TaskPtr task, unsigned int milliseconds, unsigned int limit);
        bool isSchedulerFinished() const;
        void waitForSchedulerCompletion();
        std::vector<TaskBase::TaskPtr> getTasks();

    private:
        unsigned int _pendingTasks = 0;
        std::vector<TaskBase::TaskPtr> _tasks;
        std::mutex _lock;
        std::condition_variable _waitForCompletion;

        void notifyTaskCompletion(TaskBase::TaskPtr task);
    };
}  

#endif  // SCHDEULE_H
