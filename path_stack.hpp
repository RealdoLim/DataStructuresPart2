/* TP074942
 * Name: Taichi
 * Task: Member 3 - Robot Navigation and Tracking (Stack Module)
 * * Description:
 * Developed a custom Stack structure using a Linked List instead of arrays.
 * This module records the robot's movement data when traveling to the shelf
 * and automatically creates the reverse path to bring the robot back safely.
 */

#ifndef PATH_STACK_HPP
#define PATH_STACK_HPP

#include "shared_types.hpp"
#include <string>

struct StackNode {
    RouteStep step;     
    StackNode* next;
};

class PathStack {
private:
    StackNode* topNode;

public:
    PathStack();
    ~PathStack();

    void push(RouteStep step);
    RouteStep pop(); 
    bool isEmpty() const;

    bool moveAlongRoute(int robotID, const Order& order, const RouteStep* routeArray, int routeSize);
    void returnToStart(int robotID, int orderID);
    
    void handleObstacle();      
};

#endif