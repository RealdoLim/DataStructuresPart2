#ifndef WAREHOUSE_TREE_HPP
#define WAREHOUSE_TREE_HPP

#include <string>

using namespace std;

// MODULE 5 - WAREHOUSE LAYOUT AND NAVIGATION
//
// The warehouse is naturally hierarchical:
// Entrance -> Zones -> Aisles -> Shelves.
// A tree models that hierarchy directly, while parent pointers make it easy
// to rebuild a robot route from any shelf back to the entrance.

struct WarehouseNode {
    string locationId;
    string locationName;
    string locationType;

    WarehouseNode* parent;
    WarehouseNode* firstChild;
    WarehouseNode* nextSibling;
};

class WarehouseTree {
private:
    WarehouseNode* root;

    WarehouseNode* createNode(const string& locationId,
                              const string& locationName,
                              const string& locationType,
                              WarehouseNode* parent);
    WarehouseNode* searchRecursive(WarehouseNode* current,
                                   const string& locationId);
    void displayRecursive(WarehouseNode* current, int level);
    void traversalRecursive(WarehouseNode* current);
    void destroyRecursive(WarehouseNode* current);

public:
    WarehouseTree();
    ~WarehouseTree();

    void addLocation(const string& parentId,
                     const string& locationId,
                     const string& locationName,
                     const string& locationType);
    WarehouseNode* searchLocation(const string& locationId);
    bool locationExists(const string& locationId);
    void displayLayout();
    void displayTraversal();
    void displayPathFromEntrance(const string& destinationId);
    int calculateDistanceFromEntrance(const string& destinationId);
};

#endif
