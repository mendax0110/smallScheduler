#include "../src/include/task.h"
#include "../src/include/schedule.h"
#include <mutex>
//#include <shared_ptr.h>
#include <memory>

using namespace TaskBase;
using namespace std;

Task::Task(std::function<void()> routine) : _routine(routine) {}

void Task::execute()
{
    _routine();
    std::lock_guard<std::mutex> lock(_lock);
    for (const auto& subscriber : _subscribers)
    {
        subscriber->notifyTaskFinished(shared_from_this());
    }
}

void Task::addDependency(const TaskPtr& task)
{
    _unfulfilledDependencies++;
    task->addSubscriber(shared_from_this());
}

void Task::addSubscriber(const TaskPtr& task)
{
    std::lock_guard<std::mutex> lock(_lock);
    _subscribers.push_back(task);
}

bool Task::isTaskReady() const
{
    return _unfulfilledDependencies == 0;
}

void Task::waitForTaskReady()
{
    std::unique_lock<std::mutex> lock(_lock);
    _waitReady.wait(lock, [this] { return isTaskReady(); });
}

void Task::notifyTaskFinished(const TaskPtr& task)
{
    std::lock_guard<std::mutex> lock(_lock);
    _unfulfilledDependencies--;
    if (isTaskReady())
    {
        _waitReady.notify_all();
    }
}
