#ifndef ORDER_QUEUE_HPP
#define ORDER_QUEUE_HPP

#include "shared_types.hpp"

// order_queue.hpp
// MODULE 1 - ORDER MANAGEMENT (Realdo Aginda Lim TP079467)

// Main data structure: Linked-List-based Queue (pendingOrders).
// Supporting structures: two singly-linked lists for active and completed orders.

// One node type is reused by all three internal linked structures to keep the code small and consistent.
struct OrderNode {
    Order data;
    OrderNode* next;
};

class OrderQueue {
private:
    // Pending Order Queue (FIFO) 
    OrderNode* pendingFront;   // dequeue end - next order to be processed
    OrderNode* pendingRear;    // enqueue end - most recently added order
    int pendingCount;

    // Active Orders (assigned, not yet completed)
    OrderNode* activeHead;
    int activeCount;

    // Completed Orders (Completed or Cancelled) 
    OrderNode* completedHead;
    int completedCount;

    // Internal helper used by the destructor.
    void clearList(OrderNode*& head);

public:
    OrderQueue();
    ~OrderQueue();

    // Pending Queue Operations 
    void  addOrder(const Order& order);     // enqueue       O(1)
    Order processNextOrder();               // dequeue front O(1)
    bool  isEmpty() const;
    int   getPendingCount() const;
    void  displayPendingOrders() const;     // O(n)

    // Active Order Operations 
    // Called by main.cpp after Robot Assignment succeeds.
    void  markOrderAssigned(Order order, int robotID);
    void  displayActiveOrders() const;
    Order getActiveOrderByID(int orderID, bool& found);  

    // Completion 
    // Called by main.cpp AFTER navigation finishes.
    void  markOrderCompleted(int orderID);
    void  displayCompletedOrders() const;

    // Cancellation 
    bool  cancelOrder(int orderID);
};

#endif