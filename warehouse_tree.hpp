#ifndef WAREHOUSE_TREE_HPP
#define WAREHOUSE_TREE_HPP

#include <string>

// MODULE 5 - WAREHOUSE LAYOUT AND NAVIGATION
//
// The warehouse is naturally hierarchical:
// Entrance -> Zones -> Aisles -> Shelves.
// A tree models that hierarchy directly, while parent pointers make it easy
// to rebuild a robot route from any shelf back to the entrance.

struct WarehouseNode {
    std::string locationId;
    std::string locationName;
    std::string locationType;

    WarehouseNode* parent;
    WarehouseNode* firstChild;
    WarehouseNode* nextSibling;
};

class WarehouseTree {
private:
    WarehouseNode* root;

    WarehouseNode* createNode(const std::string& locationId,
                              const std::string& locationName,
                              const std::string& locationType,
                              WarehouseNode* parent);
    WarehouseNode* searchRecursive(WarehouseNode* current,
                                   const std::string& locationId);
    void displayRecursive(WarehouseNode* current, int level);
    void traversalRecursive(WarehouseNode* current);
    void destroyRecursive(WarehouseNode* current);

public:
    WarehouseTree();
    ~WarehouseTree();

    void addLocation(const std::string& parentId,
                     const std::string& locationId,
                     const std::string& locationName,
                     const std::string& locationType);
    WarehouseNode* searchLocation(const std::string& locationId);
    bool locationExists(const std::string& locationId);
    void displayLayout();
    void displayTraversal();
    void displayPathFromEntrance(const std::string& destinationId);
    int calculateDistanceFromEntrance(const std::string& destinationId);
};

#endif
