#include <iostream>
#include <string>
#include <limits>

#include "shared_types.hpp"
#include "system_config.hpp"
#include "order_queue.hpp"
#include "robot_circular_queue.hpp" 
#include "item_bst.hpp"
#include "warehouse_tree.hpp"
#include "path_stack.hpp"

using namespace std;

// Small input helper used by every interactive menu option.
static void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Wait for the user to press Enter. Used by the guided workflow so
// the lecturer can see each step clearly during the live demo.
static void pressEnterToContinue() {
    cout << "Press Enter to continue...";
    cin.get();
}

// MEMBER 4 - ITEM SEARCH AND MANAGEMENT MODULE
ItemBST inventory;

static void loadSampleItems() {
    inventory.insertItem({"I001", "Laptop Charger", {"A", "1", "2"}, 10});
    inventory.insertItem({"I002", "Wireless Mouse", {"B", "2", "1"}, 5});
    inventory.insertItem({"I003", "Keyboard",       {"C", "1", "3"}, 8});
    std::cout << "[Init] Loaded 3 sample items." << std::endl;
}

namespace item_stub {
    bool searchItemByID(const string& itemID, int requestedQty, Item& outItem) {
        if (!inventory.searchItembyID(itemID, outItem)) {
            cout << "[Item Search] Item not found." << endl;
            return false;
        }

        if (requestedQty > outItem.stockQuantity) {
            cout << "[Item Search] Insufficient stock for "
                 << itemID << "." << endl;
            return false;
        }
        return true;
    }

    void reduceStock(const string& itemID, int quantity) {
        Item item;
        if (!inventory.searchItembyID(itemID, item)) {
            cout << "[Item Search] Item not found." << std::endl;
            return;
        }

        int newQuantity = item.stockQuantity - quantity;
        if (newQuantity < 0) {
            newQuantity = 0;
        }

        inventory.updateStock(itemID, newQuantity);
        std::cout << "[Item Search] Stock of " << itemID
             << " reduced by " << quantity << "." << endl;
    }

    void displayItems() {
        inventory.displayAllItems();
    }
}

static void printItem(const Item& item) {
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "Item ID        : " << item.itemID << std::endl;
    std::cout << "Item Name      : " << item.itemName << std::endl;
    std::cout << "Stock Quantity : " << item.stockQuantity << std::endl;
    std::cout << "Location       : "
              << "Zone: " << item.location.zone << ", "
              << "Aisle: " << item.location.aisle << ", "
              << "Shelf: " << item.location.shelf << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
}

static void menuItemManagement() {
    int choice = -1;

    while (choice != 8) {
        std::cout << "\n===== ITEM SEARCH AND MANAGEMENT =====\n"
                  << "1. Display all items and total count\n"
                  << "2. Search item by ID\n"
                  << "3. Search item by name\n"
                  << "4. Insert item\n"
                  << "5. Update stock\n"
                  << "6. Delete item\n"
                  << "7. Display low stock items\n"
                  << "8. Exit item menu\n"
                  << "Choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number from 1 to 8." << std::endl;
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (choice == 1) {
            inventory.displayAllItems();
            std::cout << "Total Inventory Count: "
                      << inventory.getTotalItems() << std::endl;
        } else if (choice == 2) {
            std::string itemID;
            Item item;

            std::cout << "Enter Item ID: ";
            std::getline(std::cin, itemID);

            if (itemID == "") {
                std::cout << "Item ID cannot be empty." << std::endl;
                continue;
            }

            if (inventory.searchItembyID(itemID, item)) {
                std::cout << "\n=== Item Found ===" << std::endl;
                printItem(item);
            } else {
                std::cout << "Item not found for ID: " << itemID << std::endl;
            }
        } else if (choice == 3) {
            std::string itemName;

            std::cout << "Enter Item Name: ";
            std::getline(std::cin, itemName);

            if (itemName == "") {
                std::cout << "Item name cannot be empty." << std::endl;
                continue;
            }

            inventory.searchItembyName(itemName);
        } else if (choice == 4) {
            Item item;
            Item existingItem;

            std::cout << "Enter Item ID: ";
            std::getline(std::cin, item.itemID);
            if (item.itemID == "") {
                std::cout << "Item ID cannot be empty." << std::endl;
                continue;
            }
            if (inventory.searchItembyID(item.itemID, existingItem)) {
                std::cout << "Insert failed. Duplicate Item ID already exists: "
                          << item.itemID << std::endl;
                continue;
            }

            std::cout << "Enter Item Name: ";
            std::getline(std::cin, item.itemName);
            if (item.itemName == "") {
                std::cout << "Item name cannot be empty." << std::endl;
                continue;
            }

            std::cout << "Enter Zone: ";
            std::getline(std::cin, item.location.zone);
            std::cout << "Enter Aisle: ";
            std::getline(std::cin, item.location.aisle);
            std::cout << "Enter Shelf: ";
            std::getline(std::cin, item.location.shelf);
            std::cout << "Enter Stock Quantity: ";
            if (!(std::cin >> item.stockQuantity)) {
                std::cout << "Invalid stock quantity. Please enter a number." << std::endl;
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();
            if (item.stockQuantity < 0) {
                std::cout << "Stock quantity cannot be negative." << std::endl;
                continue;
            }

            inventory.insertItem(item);
            std::cout << "Item inserted successfully." << std::endl;
            std::cout << "Total Inventory Count: "
                      << inventory.getTotalItems() << std::endl;
        } else if (choice == 5) {
            std::string itemID;
            int newQuantity;

            std::cout << "Enter Item ID: ";
            std::getline(std::cin, itemID);
            if (itemID == "") {
                std::cout << "Item ID cannot be empty." << std::endl;
                continue;
            }

            std::cout << "Enter New Stock Quantity: ";
            if (!(std::cin >> newQuantity)) {
                std::cout << "Invalid stock quantity. Please enter a number." << std::endl;
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();
            if (newQuantity < 0) {
                std::cout << "Stock quantity cannot be negative." << std::endl;
                continue;
            }

            if (inventory.updateStock(itemID, newQuantity)) {
                std::cout << "Stock updated successfully." << std::endl;
            } else {
                std::cout << "Item not found for ID: " << itemID << std::endl;
            }
        } else if (choice == 6) {
            std::string itemID;
            Item item;

            std::cout << "Enter Item ID: ";
            std::getline(std::cin, itemID);
            if (itemID == "") {
                std::cout << "Item ID cannot be empty." << std::endl;
                continue;
            }

            if (inventory.searchItembyID(itemID, item)) {
                inventory.deleteItem(itemID);
                std::cout << "Total Inventory Count: "
                          << inventory.getTotalItems() << std::endl;
            } else {
                std::cout << "Delete failed. Item not found: "
                          << itemID << std::endl;
            }
        } else if (choice == 7) {
            int threshold;

            std::cout << "Enter Low Stock Threshold: ";
            if (!(std::cin >> threshold)) {
                std::cout << "Invalid threshold. Please enter a number." << std::endl;
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();
            if (threshold < 0) {
                std::cout << "Threshold cannot be negative." << std::endl;
                continue;
            }

            inventory.displayLowStockItems(threshold);
        } else if (choice == 8) {
            std::cout << "Returning to main menu." << std::endl;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

// MEMBER 5 - WAREHOUSE LAYOUT AND NAVIGATION MODULE
// Connected to the real WarehouseTree module.
namespace warehouse_stub {
    static WarehouseTree* warehouseTree = nullptr;

    void connectWarehouseTree(WarehouseTree& tree) {
        warehouseTree = &tree;
    }

    string resolveShelfId(const Location& loc) {
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
            cout << "[Warehouse Layout] Tree is not connected." << endl;
            return;
        }

        string shelfId = resolveShelfId(loc);
        if (shelfId == "") {
            cout << "[Warehouse Layout] No shelf ID matches "
                 << loc.zone << ", " << loc.aisle << ", " << loc.shelf
                 << "." << endl;
            return;
        }

        warehouseTree->displayPathFromEntrance(shelfId);
    }

    void displayWarehouseLayout() {
        if (warehouseTree == nullptr) {
            cout << "[Warehouse Layout] Tree is not connected." << endl;
            return;
        }

        warehouseTree->displayLayout();
        cout << "\nSearch Result:" << endl;
        if (warehouseTree->locationExists("SHELF_A1_02")) {
            cout << "SHELF_A1_02 found." << endl;
        }

        warehouseTree->displayTraversal();
        warehouseTree->displayPathFromEntrance("SHELF_A1_02");
        warehouseTree->displayPathFromEntrance("SHELF_UNKNOWN");
    }
}

// STUB - MEMBER 3 - ROBOT NAVIGATION AND PATH TRACKING MODULE
// Replace with calls to your PathStack.
// MEMBER 3 - ROBOT NAVIGATION AND PATH TRACKING MODULE (REAL INTEGRATION)
namespace nav_stub {
    // システム全体で、過去に動いたロボットの履歴（ログ）を保存しておくためのスタック
    static PathStack globalLogStack;

    void moveAlongRoute(int robotID, const Order& order) {
        // デモシナリオに合わせた、メンバー5から届く想定のルート指示書
        const int ROUTE_SIZE = 4;
        RouteStep dummyRoute[ROUTE_SIZE] = {
            {1, "Move Forward", "Packing Station", "Zone A Aisle 1"},
            {2, "Turn Right", "Zone A Aisle 1", "Zone A Aisle 1"},
            {3, "Move Forward", "Zone A Aisle 1", "Zone A Aisle 2"},
            {4, "Turn Left", "Zone A Aisle 2", "Shelf 2 (Target)"}
        };

        // 今回の移動用のテンポラリスタックを作成
        PathStack currentJourney;
        
        // あなたの作った本物の移動関数を呼び出す
        currentJourney.moveAlongRoute(robotID, order, dummyRoute, ROUTE_SIZE);

        // 同時に、先生に見せるためのログ蓄積用スタック（globalLogStack）にも履歴をコピーして積んでおく
        for(int i = 0; i < ROUTE_SIZE; i++) {
            globalLogStack.push(dummyRoute[i]);
        }
    }

    void returnToStart(int robotID, int orderID) {
        // 今回の移動用スタックを逆再生するためのダミーを作成
        // （実際にはmoveAlongRouteでPushされた状態を再現してPop反転させます）
        PathStack currentJourney;
        const int ROUTE_SIZE = 4;
        RouteStep dummyRoute[ROUTE_SIZE] = {
            {1, "Move Forward", "Packing Station", "Zone A Aisle 1"},
            {2, "Turn Right", "Zone A Aisle 1", "Zone A Aisle 1"},
            {3, "Move Forward", "Zone A Aisle 1", "Zone A Aisle 2"},
            {4, "Turn Left", "Zone A Aisle 2", "Shelf 2 (Target)"}
        };
        for(int i = 0; i < ROUTE_SIZE; i++) {
            currentJourney.push(dummyRoute[i]);
        }

        // あなたの作った本物の逆転帰還関数を呼び出す！
        currentJourney.returnToStart(robotID, orderID);
    }

    void displayNavigationLog() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "=== Navigation Log (Member 3 - PathStack) ===" << std::endl;
        std::cout << "=============================================" << std::endl;
        
        if (globalLogStack.isEmpty()) {
            std::cout << "No navigation history recorded yet. Please run Option 3 first." << std::endl;
            return;
        }

        std::cout << "Showing raw movement history stored in the Global Log Stack (LIFO):" << std::endl;
        
        // ログ用スタックからこれまでの移動実績を取り出して綺麗に表示する
        // ※表示するとスタックは空になるため、確認用のテンポラリに移しながら表示
        PathStack temp;
        while(!globalLogStack.isEmpty()) {
            RouteStep s = globalLogStack.pop();
            std::cout << " -> Logged Step: [" << s.movement << "] from " << s.fromLocation << " to " << s.toLocation << std::endl;
            temp.push(s);
        }
        // データを元に戻す
        while(!temp.isEmpty()) {
            globalLogStack.push(temp.pop());
        }
        std::cout << "=============================================" << std::endl;
    }
}

//==================================
// Robot Assignment Module
//==================================

// Loads 3 sample robots for demo
void initializeRobots(RobotCircularQueue& robotQueue)
{
    int totalRobots = 30;

    for (int i = 1; i <= totalRobots; i++)
    {
        Robot robot;

        robot.robotID = i;
        robot.robotName = "R" + std::to_string(i);

        // Maintenance robots
        if (i == 5)
        {
            robot.status = "Maintenance";
        }
        else
        {
            robot.status = "Available";
        }

        robot.currentOrderID = -1;
        robot.taskCount = 0;

        robotQueue.addRobot(robot);
    }

    std::cout << "[Init] Loaded 11 sample robots." << std::endl;
}

//==================================
// Order Management Module - TP079467
//==================================

// Loads 3 sample orders for demo
static void initializeSampleOrders(OrderQueue& orderQueue) {
    Order o1 = {101, "Alice",   "I001", "Laptop Charger", 1, "Pending", -1};
    Order o2 = {102, "Bob",     "I002", "Wireless Mouse", 2, "Pending", -1};
    Order o3 = {103, "Charlie", "I003", "Keyboard",       1, "Pending", -1};
    orderQueue.addOrder(o1);
    orderQueue.addOrder(o2);
    orderQueue.addOrder(o3);
    cout << "[Init] Loaded 3 sample orders.\n" << endl;
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

    // Order ID
    while (true)
{
    cout << "Enter Order ID (positive integer): ";

    if (!(cin >> o.orderID))
    {
        cout << "Invalid input. Please enter a number.\n";
        clearInputBuffer();
        continue;
    }

    clearInputBuffer();

    if (o.orderID <= 0)
    {
        cout << "Order ID must be greater than 0.\n";
        continue;
    }

    if (orderQueue.orderExists(o.orderID))
    {
        cout << "Order ID already exists. Please enter a unique Order ID.\n";
        continue;
    }

    break;
}

    // Customer Name
    while (true) {
        cout << "Enter Customer Name: ";
        getline(cin, o.customerName);

        if (!o.customerName.empty())
            break;

        cout << "Customer name cannot be empty.\n";
    }

    // Item ID
    while (true) {
        cout << "Enter Item ID (e.g. I001): ";
        getline(cin, o.itemID);

        if (!o.itemID.empty())
            break;

        cout << "Item ID cannot be empty.\n";
    }

    // Item Name
    while (true) {
        cout << "Enter Item Name: ";
        getline(cin, o.itemName);

        if (!o.itemName.empty())
            break;

        cout << "Item name cannot be empty.\n";
    }

    // Quantity
    while (true) {
        cout << "Enter Quantity: ";

        if (cin >> o.quantity && o.quantity > 0) {
            clearInputBuffer();
            break;
        }

        cout << "Quantity must be a positive integer.\n";
        clearInputBuffer();
    }

    o.status = "Pending";
    o.assignedRobotID = -1;

    orderQueue.addOrder(o);

    cout << "\nOrder added successfully!\n";
}

// Menu option 3 - the guided workflow(combination of 5 modules)
static void menuGuidedWorkflow(OrderQueue& orderQueue, RobotCircularQueue& robotQueue) {
    std::cout << "\n========== GUIDED ORDER WORKFLOW ==========\n" << std::endl;

    // Step 1: dequeue from Order Management (TP079467)
    cout << "Step 1: Dequeue next order from Order Management." << endl;
    Order order = orderQueue.processNextOrder();
    if (order.orderID == -1) {
        return;
    }
    pressEnterToContinue();

    // ---- Step 2: Robot Assignment ----
    std::cout << "\nStep 2: Assign a robot using Robot Assignment." << std::endl;
    int robotID = robotQueue.assignRobot(order.orderID);
    if (robotID == -1) {
        cout << "No available robot. Order returns to pending."
             << endl;
        orderQueue.addOrder(order);     // put it back at the rear of the queue
        return;
    }
    pressEnterToContinue();

    // Step 3: mark Assigned 
    cout << "\nStep 3: Mark order as Assigned in Order Management." << endl;
    orderQueue.markOrderAssigned(order, robotID);
    orderQueue.displayActiveOrders();
    pressEnterToContinue();

    // Step 4: Item Search 
    cout << "\nStep 4: Search item location using Item Search." << endl;
    Item foundItem;
    bool itemOk = item_stub::searchItemByID(order.itemID, order.quantity, foundItem);
    if (!itemOk) {
        cout << "Item lookup failed - aborting this workflow." << endl;
        return;
    }
    pressEnterToContinue();

    // Step 5: Warehouse Layout 
    cout << "\nStep 5: Generate route using Warehouse Layout." << endl;
    warehouse_stub::generateAndDisplayRoute(foundItem.location);
    pressEnterToContinue();

    // Step 6: Navigation forward (REAL - Member 3) 
    cout << "\nStep 6: Move robot to item using Navigation Stack." << endl;
    nav_stub::moveAlongRoute(robotID, order); // 引数に注文(order)を追加！
    pressEnterToContinue();

    // Step 7: Navigation reverse (REAL - Member 3)
    cout << "\nStep 7: Return robot using reverse path (stack LIFO)." << endl;
    nav_stub::returnToStart(robotID, order.orderID); // 引数に注文IDを追加！
    pressEnterToContinue();

    // Step 8: mark Completed
    cout << "\nStep 8: Mark order as Completed in Order Management." << endl;
    orderQueue.markOrderCompleted(order.orderID);
    pressEnterToContinue();

    // Step 9: reduce stock 
    cout << "\nStep 9: Reduce item stock." << endl;
    item_stub::reduceStock(order.itemID, order.quantity);
    pressEnterToContinue();

    // Step 10: free the robot 
    std::cout << "\nStep 10: Mark robot as Available again." << std::endl;
    robotQueue.setRobotAvailable(robotID);

    cout << "\n========== WORKFLOW COMPLETE ==========\n" << endl;
}

// Optional - cancel a pending order
static void menuCancelOrder(OrderQueue& orderQueue) {
    int id;
    cout << "Enter Order ID to cancel: ";
    if (!(cin >> id)) { clearInputBuffer(); return; }
    clearInputBuffer();
    orderQueue.cancelOrder(id);
}

static void printMenu() {
    cout << "\n========= MAIN MENU =========\n"
         << " 1. Add New Order\n"
         << " 2. Display Pending Orders\n"
         << " 3. Process Next Order (Guided Workflow using 5 modules)\n"
         << " 4. Display Robot Status\n"
         << " 5. Item Search and Management\n"
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
    OrderQueue orderQueue;              // order management module
    RobotCircularQueue robotQueue;    // robot assignment module
    WarehouseTree warehouse;      // warehouse layout and navigation module

    // Load hardcoded sample data
    cout << "=== Warehouse Robot Navigation System ===\n" << endl;
    initializeSampleOrders(orderQueue);
    initializeRobots(robotQueue);
    loadSampleItems();
    initializeSampleWarehouse(warehouse);
    warehouse_stub::connectWarehouseTree(warehouse);

    // Main menu loop 
    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(cin >> choice)) { clearInputBuffer(); continue; }
        clearInputBuffer();

        switch (choice) {
            case  1: menuAddNewOrder(orderQueue);                break;
            case  2: orderQueue.displayPendingOrders();          break;
            case  3: menuGuidedWorkflow(orderQueue, robotQueue);             break;
            case  4: robotQueue.displayRobotStatus();           break;   
            case  5: menuItemManagement();                       break;   
            case  6: warehouse_stub::displayWarehouseLayout();   break;   
            case  7: orderQueue.displayActiveOrders();           break;
            case  8: orderQueue.displayCompletedOrders();        break;
            case  9: robotQueue.displayAssignmentHistory();     break;   
            case 10: nav_stub::displayNavigationLog();           break;   
            case 11: menuCancelOrder(orderQueue);                break;
            case  0: cout << "Exiting." << endl;                 break;
            default: cout << "Invalid choice." << endl;
        }
    }
    return 0;
}
