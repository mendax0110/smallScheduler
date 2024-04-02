#include "../src/include/task.h"
#include "../src/include/schedule.h"
#include <mutex>
#include <memory>

using namespace TaskBase;
using namespace std;

Task::Task(function<void()> routine) : _routine(routine) {}

void Task::execute()
{
    _routine();
    lock_guard<mutex> lock(_lock);
    for (const auto& subscriber : _subscribers)
    {
        subscriber->notifyTaskFinished(shared_from_this());
    }
}

void Task::addDep(const TaskPtr& task)
{
    _unfulfilledDependencies++;
    task->addSubs(shared_from_this());
}

void Task::addSubs(const TaskPtr& task)
{
    lock_guard<mutex> lock(_lock);
    _subscribers.push_back(task);
}

bool Task::isTaskReady() const
{
    return _unfulfilledDependencies == 0;
}

void Task::waitForTaskReady()
{
    unique_lock<mutex> lock(_lock);
    _waitReady.wait(lock, [this] { return isTaskReady(); });
}

void Task::notifyTaskFinished(const TaskPtr& task)
{
    lock_guard<mutex> lock(_lock);
    _unfulfilledDependencies--;
    if (isTaskReady())
    {
        _waitReady.notify_all();
    }
}