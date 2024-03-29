#include "../src/include/schedule.h"
#include "../src/include/task.h"

using namespace Schedule;
using namespace TaskBase;
using namespace std;

void Scheduler::addTask(TaskBase::TaskPtr task)
{
    lock_guard<mutex> lock(_lock);
    _tasks.push_back(task);
    _pendingTasks++;
}

void Scheduler::scheduleTaskAfter(TaskBase::TaskPtr task, unsigned int milliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
    addTask(task);
}

void Scheduler::scheduleTaskPeriodically(TaskBase::TaskPtr task, unsigned int milliseconds, unsigned int limit)
{
    for (unsigned int i = 0; i < limit; ++i)
    {
        this_thread::sleep_for(chrono::milliseconds(milliseconds));
        addTask(task);
    }
}

bool Scheduler::isSchedulerFinished() const
{
    bool finished = _pendingTasks == 0;

    if (finished)
    {
        cout << "Scheduler finished: " << _pendingTasks << endl;
        return finished;
    }
    else
    {
        cout << "Scheduler not finished: " << _pendingTasks << endl;
        return false;
    }
}

void Scheduler::waitForSchedulerCompletion()
{
    unique_lock<mutex> lock(_lock);
    cout << "Waiting for scheduler completion" << endl;
    _waitForCompletion.wait(lock, [this] { return isSchedulerFinished(); });
    cout << "Scheduler completed" << endl;
}

void Scheduler::notifyTaskCompletion(TaskBase::TaskPtr task)
{
    lock_guard<mutex> lock(_lock);
    _pendingTasks--;
    if (isSchedulerFinished())
    {
        _waitForCompletion.notify_all();
    }
}

std::vector<TaskBase::TaskPtr> Scheduler::getTasks()
{
    lock_guard<mutex> lock(_lock);
    return _tasks;
}