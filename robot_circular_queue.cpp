/* Ryan TP082768

Task completed:
    Implemented a circular singly linked list to assign tasks to warehouse robots.
    Implemented round-robin(basically circular) robot assignment for fair task distribution.
    Implemented robot status management (Available, Busy, Maintenance).
    Implemented assignment history tracking using a singly linked list.
    Implemented recursive reverse traversal for displaying recent assignment history.
    Implemented robot workload tracking through task count monitoring.
    Implemented paginated robot status display for scalability with large robot fleets.
    Implemented input validation and error handling for user interactions.
*/


#include "robot_circular_queue.hpp"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

RobotCircularQueue::RobotCircularQueue()
{
    tail = nullptr;
    current = nullptr;

    historyHead = nullptr;
    historyTail = nullptr;

    robotCount = 0;
    assignmentCounter = 0;
}

RobotCircularQueue::~RobotCircularQueue()
{
    clearRobots();
    clearHistory();
}

// Private
RobotNode* RobotCircularQueue::findRobotByID(int robotID)
{
    if (tail == nullptr)
        return nullptr;

    RobotNode* temp = tail->next;

    do
    {
        if (temp->robot.robotID == robotID)
            return temp;

        temp = temp->next;

    } while (temp != tail->next);

    return nullptr;
}

void RobotCircularQueue::recordAssignment(int orderID, int robotID)
{
    AssignmentHistoryNode* newNode = new AssignmentHistoryNode;

    assignmentCounter++;

    newNode->orderID = orderID;
    newNode->robotID = robotID;
    newNode->assignmentNumber = assignmentCounter;
    newNode->next = nullptr;

    if (historyHead == nullptr)
    {
        historyHead = newNode;
        historyTail = newNode;
    }
    else
    {
        historyTail->next = newNode;
        historyTail = newNode;
    }
}

void RobotCircularQueue::displayAssignmentHistoryReverse(AssignmentHistoryNode* node, int display_limit) const
{
    if (node == nullptr)
        return;

    displayAssignmentHistoryReverse(node->next);

    if (node->assignmentNumber >= (historyTail->assignmentNumber - display_limit))
    {
        cout << "#"
             << node->assignmentNumber
             << " Order "
             << node->orderID
             << " -> Robot "
             << node->robotID
             << endl;
    }
}

void RobotCircularQueue::clearHistory()
{
    while (historyHead != nullptr)
    {
        AssignmentHistoryNode* temp = historyHead;

        historyHead = historyHead->next;

        delete temp;
    }

    historyTail = nullptr;
}

void RobotCircularQueue::clearRobots()
{
    if (tail == nullptr)
    {
        return;
    }

    RobotNode* head = tail->next;

    tail->next = nullptr;

    while (head != nullptr)
    {
        RobotNode* temp = head;

        head = head->next;

        delete temp;
    }

    tail = nullptr;
    current = nullptr;
    robotCount = 0;
}

// Public
void RobotCircularQueue::addRobot(const Robot& robot)
{
    RobotNode* newNode = new RobotNode;
    newNode->robot = robot;

    if (tail == nullptr)
    {
        tail = newNode;
        current = newNode;

        newNode->next = newNode;
    }
    else
    {
        newNode->next = tail->next;
        tail->next = newNode;
        tail = newNode;
    }

    robotCount++;
}

int RobotCircularQueue::assignRobot(int orderID)
{
    if (current == nullptr) {
        cout << "[RobotQueue] No available robot found." << endl;
        return -1;
    }

    RobotNode* start = current;

    do
    {
        // Visualization for tracking purpose
        cout << "[RobotQueue] Checking Robot R"
            << current->robot.robotID
            << " (" << current->robot.status << ")"
            << endl;

        // Assigning logic
        if (current->robot.status == "Available")
        {
            current->robot.status = "Busy";
            current->robot.currentOrderID = orderID;
            current->robot.taskCount++;

            int assignedRobotID = current->robot.robotID;

            // Record for history purpose
            recordAssignment(orderID, assignedRobotID);

            current = current->next;

            cout << "[RobotQueue] Robot R"
                << assignedRobotID
                << " assigned to Order "
                << orderID
                << "." << endl;

            return assignedRobotID;
        }

        current = current->next;

    } while (current != start);

    return -1;
}

void RobotCircularQueue::setRobotAvailable(int robotID)
{
    RobotNode* robotNode = findRobotByID(robotID);

    if (robotNode != nullptr)
    {
        robotNode->robot.status = "Available";
        robotNode->robot.currentOrderID = -1;
    }
}

void RobotCircularQueue::setRobotBusy(int robotID)
{
    RobotNode* robotNode = findRobotByID(robotID);

    if (robotNode != nullptr)
    {
        robotNode->robot.status = "Busy";
    }
}

void RobotCircularQueue::setRobotMaintenance(int robotID)
{
    RobotNode* robotNode = findRobotByID(robotID);

    if (robotNode != nullptr)
    {
        robotNode->robot.status = "Maintenance";
    }
}

// void RobotCircularQueue::displayRobotStatus() const
// {
//     if (tail == nullptr)
//     {
//         cout << "[RobotQueue] No robots available.\n";
//         return;
//     }
//
//     RobotNode* temp = tail->next;
//
//     cout << endl << "=== " << "Robots Status (" << robotCount << ") ===";
//     cout << endl << left
//         << setw(5)  << "ID"
//         << setw(10) << "Name"
//         << setw(15) << "Status"
//         << setw(15) << "Current Order"
//         << setw(15) << "Tasks Completed"
//         << endl;
//
//     do
//     {
//         cout << left
//             << setw(5)  << temp->robot.robotID
//             << setw(10) << temp->robot.robotName
//             << setw(15) << temp->robot.status
//             << setw(15) << (temp->robot.currentOrderID == -1? "(none)" : to_string(temp->robot.currentOrderID))
//             << setw(10) << temp->robot.taskCount
//             << endl;
//
//         temp = temp->next;
//
//     } while (temp != tail->next);
// }

void RobotCircularQueue::displayRobotStatus() const
{
    if (tail == nullptr)
    {
        cout << "[RobotQueue] No robots available.\n";
        return;
    }

    const int robots_per_page = 10;
    int totalPages = (robotCount + robots_per_page - 1) / robots_per_page;
    int currentPage = 1;
    char choice;

    do
    {
        string title = "Robot Status";
        int titlePadding = (60 - title.length()) / 2;

        cout << string(60, '-') << endl;
        cout << string(titlePadding, ' ')
            << title
            << endl;

        cout << setw(20)
            << "Page " << currentPage << "/" << totalPages
            << " | Total Robots: " << robotCount
            << endl;

        cout << string(60, '-') << endl;
        cout << left
            << setw(5)  << "ID"
            << setw(10) << "Name"
            << setw(15) << "Status"
            << setw(15) << "Current Order"
            << setw(15) << "Tasks"
            << endl;

        cout << string(60, '-') << endl;

        int startIndex = (currentPage - 1) * robots_per_page;
        RobotNode* temp = tail->next;

        for (int i = 0; i < startIndex; i++)
        {
            temp = temp->next;
        }

        int remaining = robotCount - startIndex;

        int robotsToDisplay =
            (remaining < robots_per_page)
            ? remaining
            : robots_per_page;

        for (int i = 0; i < robotsToDisplay; i++)
        {
            cout << left
                << setw(5)  << temp->robot.robotID
                << setw(10) << temp->robot.robotName
                << setw(15) << temp->robot.status
                << setw(15)
                << (temp->robot.currentOrderID == -1
                    ? "(none)"
                    : to_string(temp->robot.currentOrderID))
                << setw(15)
                << temp->robot.taskCount
                << endl;

            temp = temp->next;
        }

        cout << endl;

        if (totalPages > 1)
        {
            cout << "[N] Next Page" << endl;
            cout << "[P] Previous Page" << endl;
            cout << "[G] Go To Page" << endl;
        }

        cout << "[Q] Quit" << endl;
        cout << "Choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        choice = static_cast<char>(toupper(choice));

        switch (choice)
        {
        case 'N':
            // Next Page - Logic
            if (currentPage < totalPages)
            { currentPage++; }
            else
            { cout << "Already at last page.\n"; }
            break;
        case 'P':
            // Previous Page - Logic
            if (currentPage > 1)
            { currentPage--; }
            else
            { cout << "Already at first page.\n"; }
            break;
        case 'G':
            {
                // Restrict user from bypassing option (Useful for condition where there's only one page)
                if (totalPages == 1)
                {
                    cout << "Already at last page.\n"; 
                    break;
                }
                
                // Go To Page - Logic
                int page;
                cout << "Enter page number: "; 
                if (!(cin >> page))
                {
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                if (page >= 1 && page <= totalPages)
                { currentPage = page; }
                else
                { cout << "Invalid page number.\n"; }
                break;
            }
        case 'Q':
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 'Q');
}

void RobotCircularQueue::displayAssignmentHistory() const
{
    if (historyHead == nullptr)
    {
        cout << endl
            << "[RobotQueue] No assignment history.\n";
        return;
    }

    const int display_limit = 10;

    cout << "\n=== Assignment History ("
         << assignmentCounter
         << " Assignments) ==="
         << endl;

    cout << "Showing latest "
         << (assignmentCounter < display_limit
             ? assignmentCounter
             : display_limit)
         << " assignments\n\n";

    displayAssignmentHistoryReverse(historyHead, display_limit);
}

bool RobotCircularQueue::isEmpty() const
{
    return tail == nullptr;
}

int RobotCircularQueue::getRobotCount() const
{
    return robotCount;
}
