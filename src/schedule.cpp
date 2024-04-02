#include "../src/include/schedule.h"
#include "../src/include/task.h"

using namespace Schedule;
using namespace TaskBase;
using namespace std;

Scheduler::Scheduler() : _lock(), _pendingTasks() {}

void Scheduler::addTask(TaskBase::TaskPtr task)
{
    {
        lock_guard<mutex> lock(_lock);
        _tasks.push_back(task);
        _pendingTasks++;
    }
    _waitForCompletion.notify_all();
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
        {
            lock_guard<mutex> lock(_lock);
            if (!isSchedulerFinished())
            {
                addTask(task);
            }
            else
            {
                break;
            }
        }
    }
}

bool Scheduler::isSchedulerFinished()
{
    bool finished = _pendingTasks == 0;

    if (finished)
        cout << "Scheduler finished" << endl;
    else
        cout << "Scheduler not finished" << endl;

    return finished;
}

void Scheduler::waitForSchedulerCompletion()
{
    unique_lock<mutex> lock(_lock);
    cout << "Waiting for scheduler completion" << endl;
    _waitForCompletion.wait_for(lock, chrono::seconds(10), [this] { return isSchedulerFinished(); });
    cout << "Scheduler completed" << endl;
}

void Scheduler::notifyTaskCompletion(TaskBase::TaskPtr task)
{
    bool notify = false;
    {
        lock_guard<mutex> lock(_lock);
        if (_pendingTasks > 0)
        {
            _pendingTasks--;
            notify = isSchedulerFinished();
        }
    }
    if (notify)
    {
        _waitForCompletion.notify_all();
    }
}

vector<TaskBase::TaskPtr> Scheduler::getTasks()
{
    lock_guard<mutex> lock(_lock);
    return _tasks;
}
