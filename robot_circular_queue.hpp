#ifndef ROBOT_CIRCULAR_QUEUE_HPP
#define ROBOT_CIRCULAR_QUEUE_HPP

#include "shared_types.hpp"

struct RobotNode
{
    Robot robot;
    RobotNode *next;
};

struct AssignmentHistoryNode
{
    int orderID;
    int robotID;
    int assignmentNumber;
    AssignmentHistoryNode *next;
};

class RobotCircularQueue
{
private:
    RobotNode *tail;
    RobotNode *current;

    AssignmentHistoryNode *historyHead;
    AssignmentHistoryNode *historyTail;

    int robotCount;
    int assignmentCounter;

    RobotNode *findRobotByID(int robotID); // O(n)
    void recordAssignment(int orderID, int robotID); // O(1)

    // Display
    void displayAssignmentHistoryReverse(AssignmentHistoryNode* node, int display_limit = 10) const; // O(m)

    void clearRobots(); // O(n)
    void clearHistory(); // O(m)

public:
    RobotCircularQueue(); // O(1)
    ~RobotCircularQueue(); // O(n + m)

    // Robot management
    void addRobot(const Robot &robot); // O(1)

    // Assignment
    int assignRobot(int orderID); // O(n) worst case O(1) best case

    // Status management
    void setRobotAvailable(int robotID); // O(n) worst case O(1) best case
    void setRobotBusy(int robotID); // O(n) worst case O(1) best case
    void setRobotMaintenance(int robotID); // O(n) worst case O(1) best case

    // Display
    void displayRobotStatus() const; // O(n)
    void displayAssignmentHistory() const; // O(m)

    // Utility
    bool isEmpty() const; // O(1)
    int getRobotCount() const; // O(1)
};

#endif

// LEGENDS
// n = number of robots
// m = number of assignment history records