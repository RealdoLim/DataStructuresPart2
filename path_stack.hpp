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