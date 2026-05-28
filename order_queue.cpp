#include "order_queue.hpp"
#include "system_config.hpp"
#include <iostream>
#include <iomanip>

// order_queue.cpp
// MODULE 1 - ORDER MANAGEMENT (Member 1)

// Constructor: every pointer starts at nullptr,
// every counter starts at 0.

OrderQueue::OrderQueue() {
    pendingFront = nullptr;
    pendingRear  = nullptr;
    pendingCount = 0;

    activeHead  = nullptr;
    activeCount = 0;

    completedHead  = nullptr;
    completedCount = 0;
}

// Destructor: free every node in every list so the program does not leak memory at shutdown.

OrderQueue::~OrderQueue() {
    clearList(pendingFront);
    pendingRear  = nullptr;
    pendingCount = 0;

    clearList(activeHead);
    activeCount = 0;

    clearList(completedHead);
    completedCount = 0;
}

// Helper: walk a linked list and delete every node.
void OrderQueue::clearList(OrderNode*& head) {
    while (head != nullptr) {
        OrderNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

// addOrder  -  enqueue at the rear of pendingOrders.
// Time complexity: O(1)  thanks to the rear pointer.

void OrderQueue::addOrder(const Order& order) {
    // Allocate a new node and copy the incoming order into it.
    OrderNode* newNode = new OrderNode;
    newNode->data = order;
    newNode->data.status          = "Pending";  // enforce correct initial state
    newNode->data.assignedRobotID = -1;         // -1 = no robot yet
    newNode->next = nullptr;

    if (pendingRear == nullptr) {
        // Queue is empty - new node is both the front and the rear.
        pendingFront = newNode;
        pendingRear  = newNode;
    } else {
        // Hook the new node onto the rear, then advance the rear pointer.
        pendingRear->next = newNode;
        pendingRear       = newNode;
    }
    pendingCount++;

    std::cout << "[OrderQueue] Order " << order.orderID
              << " added to pending queue." << std::endl;

    // Overload warning but still accept the order.
    if (pendingCount >= MAX_PENDING_WARNING) {
        std::cout << "[OrderQueue] System overload warning: "
                  << "pending order threshold reached ("
                  << pendingCount << "/" << MAX_PENDING_WARNING << ")."
                  << std::endl;
    }
}

// processNextOrder  -  dequeue the front order.
// Time complexity: O(1)
// If the queue is empty, returns a sentinel Order with orderID = -1 so main.cpp can detect failure.

Order OrderQueue::processNextOrder() {
    if (isEmpty()) {
        std::cout << "[OrderQueue] No pending orders available for processing."
                  << std::endl;
        Order empty;
        empty.orderID          = -1;
        empty.quantity         = 0;
        empty.assignedRobotID  = -1;
        return empty;
    }

    OrderNode* temp = pendingFront;
    Order dequeued  = temp->data;          // copy the value out before deletion

    pendingFront = pendingFront->next;     // advance the front pointer
    if (pendingFront == nullptr) {
        // Queue is now empty - keep the rear consistent.
        pendingRear = nullptr;
    }
    delete temp;
    pendingCount--;

    std::cout << "[OrderQueue] Processing order " << dequeued.orderID
              << " (" << dequeued.itemName << ")." << std::endl;
    return dequeued;
}

bool OrderQueue::isEmpty() const {
    return pendingFront == nullptr;
}

int OrderQueue::getPendingCount() const {
    return pendingCount;
}

// displayPendingOrders  -  walk the queue from front to rear (FIFO order) and print every order.
// Time complexity: O(n)

void OrderQueue::displayPendingOrders() const {
    std::cout << "\n=== Pending Orders (" << pendingCount << ") ===" << std::endl;
    if (pendingFront == nullptr) {
        std::cout << "(no pending orders)" << std::endl;
        return;
    }
    std::cout << std::left
              << std::setw(8)  << "OrderID"
              << std::setw(18) << "Customer"
              << std::setw(8)  << "ItemID"
              << std::setw(20) << "ItemName"
              << std::setw(6)  << "Qty"
              << std::setw(12) << "Status"
              << std::endl;

    OrderNode* curr = pendingFront;
    while (curr != nullptr) {
        std::cout << std::left
                  << std::setw(8)  << curr->data.orderID
                  << std::setw(18) << curr->data.customerName
                  << std::setw(8)  << curr->data.itemID
                  << std::setw(20) << curr->data.itemName
                  << std::setw(6)  << curr->data.quantity
                  << std::setw(12) << curr->data.status
                  << std::endl;
        curr = curr->next;
    }
}

// markOrderAssigned
// Called by main.cpp AFTER Robot Assignment succeeds.
// Stamps the order with status "Assigned" + robotID and inserts it at the HEAD of activeOrders.
// Time complexity: O(1)

void OrderQueue::markOrderAssigned(Order order, int robotID) {
    order.status          = "Assigned";
    order.assignedRobotID = robotID;

    OrderNode* newNode = new OrderNode;
    newNode->data = order;
    newNode->next = activeHead;             // insert-at-head, O(1)
    activeHead    = newNode;
    activeCount++;

    std::cout << "[OrderQueue] Order " << order.orderID
              << " marked as Assigned to Robot " << robotID << "." << std::endl;
}

void OrderQueue::displayActiveOrders() const {
    std::cout << "\n=== Active (Assigned) Orders (" << activeCount << ") ===" << std::endl;
    if (activeHead == nullptr) {
        std::cout << "(no active orders)" << std::endl;
        return;
    }
    std::cout << std::left
              << std::setw(8)  << "OrderID"
              << std::setw(18) << "Customer"
              << std::setw(20) << "ItemName"
              << std::setw(6)  << "Qty"
              << std::setw(12) << "Status"
              << std::setw(8)  << "RobotID"
              << std::endl;

    OrderNode* curr = activeHead;
    while (curr != nullptr) {
        std::cout << std::left
                  << std::setw(8)  << curr->data.orderID
                  << std::setw(18) << curr->data.customerName
                  << std::setw(20) << curr->data.itemName
                  << std::setw(6)  << curr->data.quantity
                  << std::setw(12) << curr->data.status
                  << std::setw(8)  << curr->data.assignedRobotID
                  << std::endl;
        curr = curr->next;
    }
}

// getActiveOrderByID  -  backup / checking only.
// Linear search over the active list.
// `found` is set to true when the order is located.

Order OrderQueue::getActiveOrderByID(int orderID, bool& found) {
    OrderNode* curr = activeHead;
    while (curr != nullptr) {
        if (curr->data.orderID == orderID) {
            found = true;
            return curr->data;
        }
        curr = curr->next;
    }
    found = false;
    Order empty;
    empty.orderID = -1;
    return empty;
}

// markOrderCompleted
// Locates the order in activeOrders, unlinks it, sets status = "Completed", and re-inserts it at the head of completedOrders. Time complexity: O(n) on active list.

void OrderQueue::markOrderCompleted(int orderID) {
    OrderNode* curr = activeHead;
    OrderNode* prev = nullptr;

    while (curr != nullptr && curr->data.orderID != orderID) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        std::cout << "[OrderQueue] Order not found." << std::endl;
        return;
    }

    // Unlink from active list.
    if (prev == nullptr) {
        activeHead = curr->next;            // removing the head
    } else {
        prev->next = curr->next;
    }
    activeCount--;

    // Re-stamp the status and push the same node onto completedOrders.
    curr->data.status = "Completed";
    curr->next        = completedHead;
    completedHead     = curr;
    completedCount++;

    std::cout << "[OrderQueue] Order " << orderID
              << " marked as Completed." << std::endl;
}

void OrderQueue::displayCompletedOrders() const {
    std::cout << "\n=== Completed Orders (" << completedCount << ") ===" << std::endl;
    if (completedHead == nullptr) {
        std::cout << "(no completed orders)" << std::endl;
        return;
    }
    std::cout << std::left
              << std::setw(8)  << "OrderID"
              << std::setw(18) << "Customer"
              << std::setw(20) << "ItemName"
              << std::setw(6)  << "Qty"
              << std::setw(12) << "Status"
              << std::setw(8)  << "RobotID"
              << std::endl;

    OrderNode* curr = completedHead;
    while (curr != nullptr) {
        std::cout << std::left
                  << std::setw(8)  << curr->data.orderID
                  << std::setw(18) << curr->data.customerName
                  << std::setw(20) << curr->data.itemName
                  << std::setw(6)  << curr->data.quantity
                  << std::setw(12) << curr->data.status
                  << std::setw(8)  << curr->data.assignedRobotID
                  << std::endl;
        curr = curr->next;
    }
}

// cancelOrder
// Searches pendingOrders for orderID. If found, unlinks the node from the queue and moves it to completedOrders with status "Cancelled". Returns true if cancelled.
// Time complexity: O(n) over pending orders.

bool OrderQueue::cancelOrder(int orderID) {
    OrderNode* curr = pendingFront;
    OrderNode* prev = nullptr;

    while (curr != nullptr && curr->data.orderID != orderID) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        std::cout << "[OrderQueue] Order not found." << std::endl;
        return false;
    }

    // Unlink from the pending queue.
    if (prev == nullptr) {
        // Removing the front of the queue.
        pendingFront = curr->next;
        if (pendingFront == nullptr) {
            pendingRear = nullptr;          // queue is now empty
        }
    } else {
        prev->next = curr->next;
        if (curr == pendingRear) {
            // Removing the rear of the queue - update the rear pointer.
            pendingRear = prev;
        }
    }
    pendingCount--;

    // Move the same node into the completed list, marked Cancelled.
    curr->data.status = "Cancelled";
    curr->next        = completedHead;
    completedHead     = curr;
    completedCount++;

    std::cout << "[OrderQueue] Order " << orderID
              << " has been cancelled." << std::endl;
    return true;
}