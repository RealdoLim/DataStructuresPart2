// HOW MEMBERS 2-5 PLUG IN THEIR MODULES:
//   1. Add  #include "your_module.hpp"  to the includes section.
//   2. Create your module's object near the top of main(), e.g.
//        RobotCircularQueue robotQueue;
//   3. Find your STUB BLOCK below (search for "STUB - MEMBER X")
//      and replace the body of each stub function with calls
//      to your module's real functions. Keep the function
//      signatures unchanged so main() still compiles.
//   4. Delete the inline sample data inside your stub once
//      your module loads its own sample data.

#include <iostream>
#include <string>
#include <limits>

#include "shared_types.hpp"
#include "system_config.hpp"
#include "order_queue.hpp"      
#include "warehouse_tree.hpp"

// Members 2-5: add your includes here when ready.
// #include "robot_circular_queue.hpp"   // Member 2
// #include "path_stack.hpp"             // Member 3
// #include "item_bst.hpp"               // Member 4

// Small input helper used by every interactive menu option.
static void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Wait for the user to press Enter. Used by the guided workflow so
// the lecturer can see each step clearly during the live demo.
static void pressEnterToContinue() {
    std::cout << "Press Enter to continue...";
    std::cin.get();
}


// =====================================================================
// STUB - MEMBER 2 - ROBOT ASSIGNMENT MODULE
// Replace these stub functions with calls to your RobotCircularQueue.
// =====================================================================
namespace robot_stub {
    // Rotates between R1 and R2 to mimic the circular queue idea.
    // R3 is "Maintenance" so we skip it - exactly what the real
    // circular queue will do.
    static int rotationPointer = 1;     // 1 or 2

    int assignRobot(int orderID) {
        int assigned = rotationPointer;
        rotationPointer = (rotationPointer == 1) ? 2 : 1;     // toggle
        std::cout << "[Robot Assignment - STUB] Robot R" << assigned
                  << " assigned to Order " << orderID << "." << std::endl;
        return assigned;
    }

    void setRobotAvailable(int robotID) {
        std::cout << "[Robot Assignment - STUB] Robot R" << robotID
                  << " set back to Available." << std::endl;
    }

    void displayRobotStatus() {
        std::cout << "\n=== Robot Status (STUB - Member 2) ===" << std::endl;
        std::cout << "R1  Available" << std::endl;
        std::cout << "R2  Available" << std::endl;
        std::cout << "R3  Maintenance" << std::endl;
    }

    void displayAssignmentHistory() {
        std::cout << "\n=== Assignment History (STUB - Member 2) ===" << std::endl;
        std::cout << "(Member 2 will print the real history here.)" << std::endl;
    }
}

// STUB - MEMBER 4 - ITEM SEARCH AND MANAGEMENT MODULE
// Replace with calls to your ItemBST.
namespace item_stub {
    // Three hardcoded items matching the demo scenario (plan §10).
    // Returns true if found and fills outItem; false otherwise.
    bool searchItemByID(const std::string& itemID, int requestedQty, Item& outItem) {
        if (itemID == "I001") {
            outItem = {"I001", "Laptop Charger", {"Zone A", "Aisle 1", "Shelf 2"}, 10};
        } else if (itemID == "I002") {
            outItem = {"I002", "Wireless Mouse", {"Zone B", "Aisle 2", "Shelf 1"}, 5};
        } else if (itemID == "I003") {
            outItem = {"I003", "Keyboard",       {"Zone C", "Aisle 1", "Shelf 3"}, 8};
        } else {
            std::cout << "[Item Search - STUB] Item not found." << std::endl;
            return false;
        }
        if (requestedQty > outItem.stockQuantity) {
            std::cout << "[Item Search - STUB] Insufficient stock for "
                      << itemID << "." << std::endl;
            return false;
        }
        std::cout << "[Item Search - STUB] Found " << outItem.itemName
                  << " at " << outItem.location.zone << ", "
                  << outItem.location.aisle << ", "
                  << outItem.location.shelf << "." << std::endl;
        return true;
    }

    void reduceStock(const std::string& itemID, int quantity) {
        std::cout << "[Item Search - STUB] Stock of " << itemID
                  << " reduced by " << quantity << "." << std::endl;
    }

    void displayItems() {
        std::cout << "\n=== Item List (STUB - Member 4) ===" << std::endl;
        std::cout << "I001  Laptop Charger   Zone A, Aisle 1, Shelf 2   Stock: 10" << std::endl;
        std::cout << "I002  Wireless Mouse   Zone B, Aisle 2, Shelf 1   Stock: 5"  << std::endl;
        std::cout << "I003  Keyboard         Zone C, Aisle 1, Shelf 3   Stock: 8"  << std::endl;
    }
}

// MEMBER 5 - WAREHOUSE LAYOUT AND NAVIGATION MODULE
// Connected to the real WarehouseTree module.
namespace warehouse_stub {
    static WarehouseTree* warehouseTree = nullptr;

    void connectWarehouseTree(WarehouseTree& tree) {
        warehouseTree = &tree;
    }

    std::string resolveShelfId(const Location& loc) {
        if (loc.zone == "Zone A" && loc.aisle == "Aisle 1" && loc.shelf == "Shelf 2") {
            return "SHELF_A1_02";
        }
        if (loc.zone == "Zone A" && loc.aisle == "Aisle 1" && loc.shelf == "Shelf 1") {
            return "SHELF_A1_01";
        }
        if (loc.zone == "Zone A" && loc.aisle == "Aisle 2" && loc.shelf == "Shelf 1") {
            return "SHELF_A2_01";
        }
        if (loc.zone == "Zone B" && loc.aisle == "Aisle 1" && loc.shelf == "Shelf 1") {
            return "SHELF_B1_01";
        }
        return "";
    }

    void generateAndDisplayRoute(const Location& loc) {
        if (warehouseTree == nullptr) {
            std::cout << "[Warehouse Layout] Tree is not connected." << std::endl;
            return;
        }

        std::string shelfId = resolveShelfId(loc);
        if (shelfId == "") {
            std::cout << "[Warehouse Layout] No shelf ID matches "
                      << loc.zone << ", " << loc.aisle << ", " << loc.shelf
                      << "." << std::endl;
            return;
        }

        warehouseTree->displayPathFromEntrance(shelfId);
    }

    void displayWarehouseLayout() {
        if (warehouseTree == nullptr) {
            std::cout << "[Warehouse Layout] Tree is not connected." << std::endl;
            return;
        }

        warehouseTree->displayLayout();
        std::cout << "\nSearch Result:" << std::endl;
        if (warehouseTree->locationExists("SHELF_A1_02")) {
            std::cout << "SHELF_A1_02 found." << std::endl;
        }

        warehouseTree->displayTraversal();
        warehouseTree->displayPathFromEntrance("SHELF_A1_02");
        warehouseTree->displayPathFromEntrance("SHELF_UNKNOWN");
    }
}

// STUB - MEMBER 3 - ROBOT NAVIGATION AND PATH TRACKING MODULE
// Replace with calls to your PathStack.
namespace nav_stub {
    // Pretend forward movements (plan §10 demo).
    void moveAlongRoute(int robotID) {
        std::cout << "[Navigation - STUB] Robot R" << robotID
                  << " forward path:" << std::endl;
        std::cout << "   Move Forward -> Turn Left -> Move Forward -> Pick Item"
                  << std::endl;
    }

    // Pretend reverse using stack LIFO (plan §10).
    void returnToStart(int robotID) {
        std::cout << "[Navigation - STUB] Robot R" << robotID
                  << " reverse path:" << std::endl;
        std::cout << "   Move Backward -> Turn Right -> Move Backward"
                  << std::endl;
    }

    void displayNavigationLog() {
        std::cout << "\n=== Navigation Log (STUB - Member 3) ===" << std::endl;
        std::cout << "(Member 3 will print the real log here.)" << std::endl;
    }
}

// Order Management Module

// Loads 3 sample orders for demo
static void initializeSampleOrders(OrderQueue& orderQueue) {
    Order o1 = {101, "Alice",   "I001", "Laptop Charger", 1, "Pending", -1};
    Order o2 = {102, "Bob",     "I002", "Wireless Mouse", 2, "Pending", -1};
    Order o3 = {103, "Charlie", "I003", "Keyboard",       1, "Pending", -1};
    orderQueue.addOrder(o1);
    orderQueue.addOrder(o2);
    orderQueue.addOrder(o3);
    std::cout << "[Init] Loaded 3 sample orders.\n" << std::endl;
}

// Loads the sample hierarchy required for Member 5's live demo.
static void initializeSampleWarehouse(WarehouseTree& warehouse) {
    warehouse.addLocation("ENTRANCE", "ZONE_A", "Zone A", "Zone");
    warehouse.addLocation("ENTRANCE", "ZONE_B", "Zone B", "Zone");

    warehouse.addLocation("ZONE_A", "AISLE_A1", "Aisle A1", "Aisle");
    warehouse.addLocation("ZONE_A", "AISLE_A2", "Aisle A2", "Aisle");
    warehouse.addLocation("ZONE_B", "AISLE_B1", "Aisle B1", "Aisle");

    warehouse.addLocation("AISLE_A1", "SHELF_A1_01", "Shelf A1-01", "Shelf");
    warehouse.addLocation("AISLE_A1", "SHELF_A1_02", "Shelf A1-02", "Shelf");
    warehouse.addLocation("AISLE_A2", "SHELF_A2_01", "Shelf A2-01", "Shelf");
    warehouse.addLocation("AISLE_B1", "SHELF_B1_01", "Shelf B1-01", "Shelf");
}

// Menu option 1 - interactive manual add.
static void menuAddNewOrder(OrderQueue& orderQueue) {
    Order o;
    std::cout << "Enter Order ID (int): ";
    if (!(std::cin >> o.orderID)) { clearInputBuffer(); return; }
    clearInputBuffer();

    std::cout << "Enter Customer Name: ";
    std::getline(std::cin, o.customerName);

    std::cout << "Enter Item ID (e.g. I001): ";
    std::getline(std::cin, o.itemID);

    std::cout << "Enter Item Name: ";
    std::getline(std::cin, o.itemName);

    std::cout << "Enter Quantity: ";
    if (!(std::cin >> o.quantity)) { clearInputBuffer(); return; }
    clearInputBuffer();

    o.status          = "Pending";
    o.assignedRobotID = -1;
    orderQueue.addOrder(o);
}

// Menu option 3 - the guided workflow(combination of 5 modules)
static void menuGuidedWorkflow(OrderQueue& orderQueue) {
    std::cout << "\n========== GUIDED ORDER WORKFLOW ==========\n" << std::endl;

    // Step 1: dequeue from Order Management
    std::cout << "Step 1: Dequeue next order from Order Management." << std::endl;
    Order order = orderQueue.processNextOrder();
    if (order.orderID == -1) {
        return;
    }
    pressEnterToContinue();

    // ---- Step 2: Robot Assignment (STUB - Member 2) ----
    std::cout << "\nStep 2: Assign a robot using Robot Assignment." << std::endl;
    int robotID = robot_stub::assignRobot(order.orderID);
    if (robotID == -1) {
        std::cout << "No available robot. Order returns to pending."
                  << std::endl;
        orderQueue.addOrder(order);     // put it back at the rear of the queue
        return;
    }
    pressEnterToContinue();

    // Step 3: mark Assigned 
    std::cout << "\nStep 3: Mark order as Assigned in Order Management." << std::endl;
    orderQueue.markOrderAssigned(order, robotID);
    orderQueue.displayActiveOrders();
    pressEnterToContinue();

    // Step 4: Item Search (STUB - Member 4)
    std::cout << "\nStep 4: Search item location using Item Search." << std::endl;
    Item foundItem;
    bool itemOk = item_stub::searchItemByID(order.itemID, order.quantity, foundItem);
    if (!itemOk) {
        std::cout << "Item lookup failed - aborting this workflow." << std::endl;
        return;
    }
    pressEnterToContinue();

    // Step 5: Warehouse Layout (STUB - Member 5)
    std::cout << "\nStep 5: Generate route using Warehouse Layout." << std::endl;
    warehouse_stub::generateAndDisplayRoute(foundItem.location);
    pressEnterToContinue();

    // Step 6: Navigation forward (STUB - Member 3) 
    std::cout << "\nStep 6: Move robot to item using Navigation Stack." << std::endl;
    nav_stub::moveAlongRoute(robotID);
    pressEnterToContinue();

    // Step 7: Navigation reverse (STUB - Member 3)
    std::cout << "\nStep 7: Return robot using reverse path (stack LIFO)." << std::endl;
    nav_stub::returnToStart(robotID);
    pressEnterToContinue();

    // Step 8: mark Completed
    std::cout << "\nStep 8: Mark order as Completed in Order Management." << std::endl;
    orderQueue.markOrderCompleted(order.orderID);
    pressEnterToContinue();

    // Step 9: reduce stock (STUB - Member 4) 
    std::cout << "\nStep 9: Reduce item stock." << std::endl;
    item_stub::reduceStock(order.itemID, order.quantity);
    pressEnterToContinue();

    // Step 10: free the robot (STUB - Member 2)
    std::cout << "\nStep 10: Mark robot as Available again." << std::endl;
    robot_stub::setRobotAvailable(robotID);

    std::cout << "\n========== WORKFLOW COMPLETE ==========\n" << std::endl;
}

// Optional - cancel a pending order
static void menuCancelOrder(OrderQueue& orderQueue) {
    int id;
    std::cout << "Enter Order ID to cancel: ";
    if (!(std::cin >> id)) { clearInputBuffer(); return; }
    clearInputBuffer();
    orderQueue.cancelOrder(id);
}

static void printMenu() {
    std::cout << "\n========= MAIN MENU =========\n"
              << " 1. Add New Order\n"
              << " 2. Display Pending Orders\n"
              << " 3. Process Next Order (Guided Workflow using 5 modules)\n"
              << " 4. Display Robot Status\n"
              << " 5. Display Item List\n"
              << " 6. Display Warehouse Layout\n"
              << " 7. Display Active Orders\n"
              << " 8. Display Completed Orders\n"
              << " 9. Display Assignment History\n"
              << "10. Display Navigation Log\n"
              << "11. Cancel an Order (optional)\n"
              << " 0. Exit\n"
              << "Choice: ";
}

int main() {
    // ----- Create each module's object here -----
    OrderQueue orderQueue;       // order management module
    WarehouseTree warehouse;      // warehouse layout and navigation module

    // Members 2-5: instantiate your objects here when ready, e.g.
    // RobotCircularQueue robotQueue;
    // ItemBST            itemTree;
    // (PathStack lives inside the navigation workflow, not here.)

    // Load hardcoded sample data
    std::cout << "=== Warehouse Robot Navigation System ===\n" << std::endl;
    initializeSampleOrders(orderQueue);
    initializeSampleWarehouse(warehouse);
    warehouse_stub::connectWarehouseTree(warehouse);
    // Members 2/4: load your sample robots/items here.

    // Main menu loop 
    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) { clearInputBuffer(); continue; }
        clearInputBuffer();

        switch (choice) {
            case  1: menuAddNewOrder(orderQueue);                break;
            case  2: orderQueue.displayPendingOrders();          break;
            case  3: menuGuidedWorkflow(orderQueue);             break;
            case  4: robot_stub::displayRobotStatus();           break;   // Member 2
            case  5: item_stub::displayItems();                  break;   // Member 4
            case  6: warehouse_stub::displayWarehouseLayout();   break;   // Member 5
            case  7: orderQueue.displayActiveOrders();           break;
            case  8: orderQueue.displayCompletedOrders();        break;
            case  9: robot_stub::displayAssignmentHistory();     break;   // Member 2
            case 10: nav_stub::displayNavigationLog();           break;   // Member 3
            case 11: menuCancelOrder(orderQueue);                break;
            case  0: std::cout << "Exiting." << std::endl;       break;
            default: std::cout << "Invalid choice." << std::endl;
        }
    }
    return 0;
}
