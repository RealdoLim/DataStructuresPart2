#include "item_bst.hpp"
#include <cctype>

namespace {
    bool nameSearchFound = false;
    bool lowStockFound = false;

    void displaySeparator() {
        std::cout << "------------------------------------------------------------" << std::endl;
    }

    std::string toLowercase(const std::string& text) {
        std::string lowered = text;

        for (int i = 0; i < static_cast<int>(lowered.length()); i++) {
            lowered[i] = static_cast<char>(
                std::tolower(static_cast<unsigned char>(lowered[i]))
            );
        }

        return lowered;
    }

    void displayItem(const Item& item) {
        std::cout << "Item ID        : " << item.itemID << std::endl;
        std::cout << "Item Name      : " << item.itemName << std::endl;
        std::cout << "Stock Quantity : " << item.stockQuantity << std::endl;
        std::cout << "Location       : "
                  << "Zone: " << item.location.zone << ", "
                  << "Aisle: " << item.location.aisle << ", "
                  << "Shelf: " << item.location.shelf << std::endl;
        displaySeparator();
    }
}

// Constructor
ItemBST::ItemBST() {
    root = nullptr;
}

// Destructor
ItemBST::~ItemBST() {
    destroyTree(root);
    root = nullptr;
}

// Recursively inserts a new item based on itemID.
// Duplicate item IDs are not inserted because itemID is the BST key.
ItemBST::Node* ItemBST::insert(Node* node, const Item& item) {
    if (node == nullptr) {
        return new Node(item);
    }

    if (item.itemID < node->data.itemID) {
        node->left = insert(node->left, item);
    } else if (item.itemID > node->data.itemID) {
        node->right = insert(node->right, item);
    } else {
        std::cout << "Insert skipped. Duplicate Item ID already exists: "
                  << item.itemID << std::endl;
    }

    return node;
}

void ItemBST::insertItem(const Item& item) {
    if (item.itemID == "") {
        std::cout << "Insert failed. Item ID cannot be empty." << std::endl;
        return;
    }

    if (item.stockQuantity < 0) {
        std::cout << "Insert failed. Stock quantity cannot be negative." << std::endl;
        return;
    }

    root = insert(root, item);
}

// Recursively searches by itemID using the BST ordering.
ItemBST::Node* ItemBST::search(Node* node, const std::string& itemID) {
    if (node == nullptr || node->data.itemID == itemID) {
        return node;
    }

    if (itemID < node->data.itemID) {
        return search(node->left, itemID);
    }

    return search(node->right, itemID);
}

bool ItemBST::searchItembyID(const std::string& itemID, Item& result) {
    if (itemID == "") {
        return false;
    }

    Node* foundNode = search(root, itemID);

    if (foundNode == nullptr) {
        return false;
    }

    result = foundNode->data;
    return true;
}

// Name search cannot use the BST ordering, so it checks every node inorder.
// Partial matches are accepted, and the comparison is case-insensitive.
void ItemBST::searchByName(Node* node, const std::string& itemName) {
    if (node == nullptr) {
        return;
    }

    searchByName(node->left, itemName);

    if (toLowercase(node->data.itemName).find(toLowercase(itemName)) != std::string::npos) {
        displayItem(node->data);
        nameSearchFound = true;
    }

    searchByName(node->right, itemName);
}

void ItemBST::searchItembyName(const std::string& keyword) {
    if (isEmpty()) {
        std::cout << "Inventory is empty." << std::endl;
        return;
    }

    if (keyword == "") {
        std::cout << "Search failed. Item name cannot be empty." << std::endl;
        return;
    }

    nameSearchFound = false;
    std::cout << "\n=== Search Results for \"" << keyword << "\" ===" << std::endl;
    displaySeparator();
    searchByName(root, keyword);

    if (!nameSearchFound) {
        std::cout << "No item found matching name: " << keyword << std::endl;
    }
}

bool ItemBST::updateStock(const std::string& itemID, int newQuantity) {
    if (itemID == "" || newQuantity < 0) {
        return false;
    }

    Node* foundNode = search(root, itemID);

    if (foundNode == nullptr) {
        return false;
    }

    foundNode->data.stockQuantity = newQuantity;
    return true;
}

// Returns the smallest itemID in the given subtree.
// Used as the inorder successor during two-child deletion.
ItemBST::Node* ItemBST::findMin(Node* node) {
    if (node == nullptr || node->left == nullptr) {
        return node;
    }

    return findMin(node->left);
}

// Recursively deletes a node and reconnects the BST correctly.
ItemBST::Node* ItemBST::deleteNode(Node* node, const std::string& itemID) {
    if (node == nullptr) {
        return nullptr;
    }

    if (itemID < node->data.itemID) {
        node->left = deleteNode(node->left, itemID);
    } else if (itemID > node->data.itemID) {
        node->right = deleteNode(node->right, itemID);
    } else {
        // Case 1: leaf node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Case 2: one child
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }

        if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: two children
        // Replace with inorder successor from the right subtree.
        Node* successor = findMin(node->right);
        node->data = successor->data;
        node->right = deleteNode(node->right, successor->data.itemID);
    }

    return node;
}

void ItemBST::deleteItem(const std::string& itemID) {
    if (itemID == "") {
        std::cout << "Delete failed. Item ID cannot be empty." << std::endl;
        return;
    }

    if (search(root, itemID) == nullptr) {
        std::cout << "Delete failed. Item not found: " << itemID << std::endl;
        return;
    }

    root = deleteNode(root, itemID);
    std::cout << "Delete successful. Removed item: " << itemID << std::endl;
}

// Inorder traversal displays items sorted by itemID.
void ItemBST::inOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    inOrder(node->left);
    displayItem(node->data);
    inOrder(node->right);
}

void ItemBST::displayAllItems() {
    if (isEmpty()) {
        std::cout << "\n=== Inventory Items ===" << std::endl;
        std::cout << "Inventory is empty. No items are currently stored." << std::endl;
        return;
    }

    std::cout << "\n=== Inventory Items ===" << std::endl;
    std::cout << "Total Items: " << getTotalItems() << std::endl;
    displaySeparator();
    inOrder(root);
}

void ItemBST::displayLowStock(Node* node, int threshold) {
    if (node == nullptr) {
        return;
    }

    displayLowStock(node->left, threshold);

    if (node->data.stockQuantity <= threshold) {
        displayItem(node->data);
        lowStockFound = true;
    }

    displayLowStock(node->right, threshold);
}

void ItemBST::displayLowStockItems(int threshold) {
    if (isEmpty()) {
        std::cout << "\n=== Low Stock Alert ===" << std::endl;
        std::cout << "Inventory is empty. No low stock check can be performed." << std::endl;
        return;
    }

    if (threshold < 0) {
        std::cout << "\n=== Low Stock Alert ===" << std::endl;
        std::cout << "Invalid threshold. Threshold cannot be negative." << std::endl;
        return;
    }

    lowStockFound = false;
    std::cout << "\n=== Low Stock Alert ===" << std::endl;
    std::cout << "Showing items with stock at or below threshold: "
              << threshold << std::endl;
    displaySeparator();
    displayLowStock(root, threshold);

    if (!lowStockFound) {
        std::cout << "No low stock items found at or below threshold: "
                  << threshold << std::endl;
    }
}

int ItemBST::countNodes(Node* node) {
    if (node == nullptr) {
        return 0;
    }

    return 1 + countNodes(node->left) + countNodes(node->right);
}

int ItemBST::getTotalItems() {
    return countNodes(root);
}

bool ItemBST::isEmpty() {
    return root == nullptr;
}

void ItemBST::destroyTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}
