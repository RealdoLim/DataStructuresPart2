#ifndef ITEM_BST_HPP
#define ITEM_BST_HPP

#include <iostream>
#include <string>
#include "shared_types.hpp"

class ItemBST {
    private:
    struct Node{
        Item data;
        Node* left;
        Node* right;

        Node(const Item& item){
            data = item;
            left = nullptr;
            right = nullptr;
        }
    };
    Node* root;

    // Helper functions

    Node* insert(Node* node, const Item& item);
    Node* deleteNode(Node* node, const std::string& itemID);
    Node* search(Node* node, const std::string& itemID);
    Node* findMin(Node* node);
    Node* inOrder(Node* node);
    void searchByName(Node* node, const std::string& itemName);
    void destroyTree(Node* node);
    int countNodes(Node* node);
    void displayLowStock(Node* node, int threshold);

    public:
    // Constructor and Destructor
    ItemBST();
    ~ItemBST();

    // Core operations
    void insertItem(const Item& item);
    void deleteItem(const std::string& itemID);
    bool searchItembyID(const std::string& itemID, Item& result);
    void searchItembyName(const std::string& keyword);
    bool updateStock(const std::string& itemID, int newQuantity);

    // Display and utility functions
    void displayAllItems();
    void displayLowStockItems(int threshold);
    int getTotalItems();
    bool isEmpty();
};

#endif // ITEM_BST_HPP