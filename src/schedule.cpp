#include "../src/include/schedule.h"
#include "../src/include/task.h"

using namespace Schedule;
using namespace TaskBase;

void Scheduler::addTask(TaskBase::TaskPtr task)
{
    std::lock_guard<std::mutex> lock(_lock);
    _tasks.push_back(task);
    _pendingTasks++;
}

void Scheduler::scheduleTaskAfter(TaskBase::TaskPtr task, unsigned int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    addTask(task);
}

void Scheduler::scheduleTaskPeriodically(TaskBase::TaskPtr task, unsigned int milliseconds, unsigned int limit)
{
    for (unsigned int i = 0; i < limit; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        addTask(task);
    }
}

bool Scheduler::isSchedulerFinished() const
{
    bool finished = _pendingTasks == 0;

    if (finished)
        std::cout << "Scheduler finished: " << _pendingTasks << std::endl;
    else
        std::cout << "Scheduler not finished: " << _pendingTasks << std::endl;

    return finished;
}

void Scheduler::waitForSchedulerCompletion()
{
    std::unique_lock<std::mutex> lock(_lock);
    std::cout << "Waiting for scheduler completion" << std::endl;
    _waitForCompletion.wait(lock, [this] { return isSchedulerFinished(); });
    std::cout << "Scheduler completed" << std::endl;
}

void Scheduler::notifyTaskCompletion(TaskBase::TaskPtr task)
{
    std::lock_guard<std::mutex> lock(_lock);
    _pendingTasks--;
    if (isSchedulerFinished())
    {
        _waitForCompletion.notify_all();
    }
}

std::vector<TaskBase::TaskPtr> Scheduler::getTasks()
{
    std::lock_guard<std::mutex> lock(_lock);
    return _tasks;
}