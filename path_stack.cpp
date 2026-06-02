#include "path_stack.hpp"
#include <iostream>

PathStack::PathStack() {
    topNode = nullptr;
}

PathStack::~PathStack() {
    while (!isEmpty()) {
        pop();
    }
}

void PathStack::push(RouteStep step) {
    StackNode* newNode = new StackNode();
    newNode->step = step;
    newNode->next = topNode;
    topNode = newNode;
}

RouteStep PathStack::pop() {
    if (isEmpty()) {
        std::cout << "Error: Stack is empty!" << std::endl;
        return RouteStep();
    }
    
    StackNode* temp = topNode;
    RouteStep poppedStep = temp->step;
    topNode = topNode->next;
    delete temp;
    return poppedStep;
}

bool PathStack::isEmpty() const {
    return topNode == nullptr;
}

bool PathStack::moveAlongRoute(int robotID, const Order& order, const RouteStep* routeArray, int routeSize) {
    if (routeSize == 0 || routeArray == nullptr) {
        std::cout << "[Navigation Error] No route available for robot movement." << std::endl;
        return false;
    }

    std::cout << "\n--- [Module 3] Robot R" << robotID << " Navigation: Moving to Item ---" << std::endl;
    std::cout << "Robot Name: R" << robotID << " is fetching item: " << order.itemName << " for Order ID: " << order.orderID << std::endl;

    for (int i = 0; i < routeSize; i++) {
        RouteStep currentStep = routeArray[i];
        
        std::cout << "-> " << currentStep.movement 
                  << " (From: " << currentStep.fromLocation 
                  << " To: " << currentStep.toLocation << ")" << std::endl;

        push(currentStep);

        if (currentStep.movement == "Obstacle") {
            handleObstacle();
            return false;
        }
    }

    std::cout << "SUCCESS: Robot reached the destination. Item picked!" << std::endl;
    return true;
}

void PathStack::returnToStart(int robotID, int orderID) {
    std::cout << "\n--- [Module 3] Path Tracking: Returning to Packing Station ---" << std::endl;
    std::cout << "Robot R" << robotID << " is reversing its path using Stack (LIFO)..." << std::endl;

    while (!isEmpty()) {
        RouteStep forwardStep = pop();
        std::string reverseMovement = "";

        if (forwardStep.movement == "Move Forward") {
            reverseMovement = "Move Backward";
        } else if (forwardStep.movement == "Move Backward") {
            reverseMovement = "Move Forward";
        } else if (forwardStep.movement == "Turn Left") {
            reverseMovement = "Turn Right";
        } else if (forwardStep.movement == "Turn Right") {
            reverseMovement = "Turn Left";
        } else if (forwardStep.movement == "Pick Item") {
            continue;
        } else {
            reverseMovement = "Reverse of " + forwardStep.movement;
        }

        std::cout << "<- Pop from Stack: [" << forwardStep.movement 
                  << "] -> Action Taken: " << reverseMovement 
                  << " (Returning from " << forwardStep.toLocation 
                  << " to " << forwardStep.fromLocation << ")" << std::endl;
    }

    std::cout << "SUCCESS: Robot R" << robotID << " returned to Packing Station safely! (Stack is empty)" << std::endl;
}

void PathStack::handleObstacle() {
    std::cout << "[Alert] Obstacle detected! Backtracking to previous safe step." << std::endl;
    if (!isEmpty()) {
        RouteStep lastStep = pop();
        std::cout << "Backtracked from: " << lastStep.toLocation << " to: " << lastStep.fromLocation << std::endl;
    }
}