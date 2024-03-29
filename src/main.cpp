#include "../src/include/schedule.h"
#include "../src/include/task.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void addTask(Schedule::Scheduler& scheduler)
{
    string taskDescr;
    cout << "Enter task description: ";
    getline(cin, taskDescr);

    auto task = make_shared<TaskBase::Task>([=]() {
        cout << "Task: " << taskDescr << " executed" << endl;
    });

    scheduler.addTask(task);
}

void menu()
{
    std::string menu = "1. Add task\n"
                       "2. Schedule task after\n"
                       "3. Schedule task periodically\n"
                       "4. Wait for scheduler completion\n"
                       "5. Exit\n";
    cout << menu;
}

int main()
{
    Schedule::Scheduler scheduler;
    int choice;

    do
    {
        menu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice)
        {
            case 1:
                addTask(scheduler);
                break;
            case 2:
                {
                    unsigned int milliseconds;
                    cout << "Enter milliseconds to wait before executing the task: ";
                    cin >> milliseconds;
                    cin.ignore();

                    addTask(scheduler);
                    scheduler.scheduleTaskAfter(scheduler.getTasks().back(), milliseconds);
                }
                break;
            case 3:
                {
                    unsigned int milliseconds, limit;
                    cout << "Enter milliseconds interval for periodic execution:";
                    cin >> milliseconds;
                    cin.ignore();
                    cout << "Enter limit for periodic execution:";
                    cin >> limit;
                    cin.ignore();

                    addTask(scheduler);
                    scheduler.scheduleTaskPeriodically(scheduler.getTasks().back(), milliseconds, limit);
                }
                break;
            case 4:
                scheduler.waitForSchedulerCompletion();
                cout << "Scheduler finished" << endl;
                break;
            case 5:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice" << endl;
        }
    } while (choice != 5);
}