#ifndef SHARED_TYPES_HPP
#define SHARED_TYPES_HPP

#include <string>

// Location - where an item physically sits in the warehouse.
struct Location {
    std::string zone;
    std::string aisle;
    std::string shelf;
};

// Order - the core record that flows through the system.
// Created by Order Management, then read by every other module via main.cpp.
struct Order {
    int orderID;
    std::string customerName;     // chosen over customerID per group decision
    std::string itemID;
    std::string itemName;
    int quantity;
    std::string status;           // "Pending" / "Assigned" / "Completed" / "Cancelled"
    int assignedRobotID;          // -1 when no robot is assigned yet
};

struct Robot {
    int robotID;
    std::string robotName;
    std::string status;           // "Available" / "Busy" / "Maintenance"
    int currentOrderID;           // -1 when not handling any order
    int taskCount;
};

struct Item {
    std::string itemID;
    std::string itemName;
    Location location;
    int stockQuantity;
};

struct RouteStep {
    int stepNumber;
    std::string movement;         // "Move Forward", "Turn Left", "Turn Right", "Pick Item", "Move Backward"
    std::string fromLocation;
    std::string toLocation;
};

#endif