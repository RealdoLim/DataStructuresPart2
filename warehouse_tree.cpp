#include "warehouse_tree.hpp"

#include <iostream>

using namespace std;

const int MAX_WAREHOUSE_PATH_DEPTH = 50;

WarehouseTree::WarehouseTree() {
    root = createNode("ENTRANCE", "Main Entrance", "Entrance", nullptr);
}

WarehouseTree::~WarehouseTree() {
    destroyRecursive(root);
    root = nullptr;
}

WarehouseNode* WarehouseTree::createNode(const string& locationId,
                                         const string& locationName,
                                         const string& locationType,
                                         WarehouseNode* parent) {
    WarehouseNode* node = new WarehouseNode;
    node->locationId = locationId;
    node->locationName = locationName;
    node->locationType = locationType;
    node->parent = parent;
    node->firstChild = nullptr;
    node->nextSibling = nullptr;
    return node;
}

void WarehouseTree::destroyRecursive(WarehouseNode* current) {
    if (current == nullptr) {
        return;
    }

    destroyRecursive(current->firstChild);
    destroyRecursive(current->nextSibling);
    delete current;
}

WarehouseNode* WarehouseTree::searchRecursive(WarehouseNode* current,
                                              const string& locationId) {
    if (current == nullptr) {
        return nullptr;
    }

    if (current->locationId == locationId) {
        return current;
    }

    WarehouseNode* foundInChild = searchRecursive(current->firstChild, locationId);
    if (foundInChild != nullptr) {
        return foundInChild;
    }

    return searchRecursive(current->nextSibling, locationId);
}

void WarehouseTree::displayRecursive(WarehouseNode* current, int level) {
    if (current == nullptr) {
        return;
    }

    for (int i = 0; i < level; i++) {
        cout << "  ";
    }

    cout << current->locationId << " - " << current->locationName
         << " (" << current->locationType << ")" << endl;

    displayRecursive(current->firstChild, level + 1);
    displayRecursive(current->nextSibling, level);
}

void WarehouseTree::traversalRecursive(WarehouseNode* current) {
    if (current == nullptr) {
        return;
    }

    cout << current->locationId << " [" << current->locationType << "]"
         << endl;

    traversalRecursive(current->firstChild);
    traversalRecursive(current->nextSibling);
}

void WarehouseTree::addLocation(const string& parentId,
                                const string& locationId,
                                const string& locationName,
                                const string& locationType) {
    if (locationExists(locationId)) {
        cout << "[Warehouse Layout] Location " << locationId
             << " already exists. Duplicate not added." << endl;
        return;
    }

    WarehouseNode* parent = searchLocation(parentId);
    if (parent == nullptr) {
        cout << "[Warehouse Layout] Parent location " << parentId
             << " was not found. Cannot add " << locationId << "."
             << endl;
        return;
    }

    WarehouseNode* newNode = createNode(locationId, locationName, locationType, parent);

    if (parent->firstChild == nullptr) {
        parent->firstChild = newNode;
        return;
    }

    WarehouseNode* sibling = parent->firstChild;
    while (sibling->nextSibling != nullptr) {
        sibling = sibling->nextSibling;
    }
    sibling->nextSibling = newNode;
}

WarehouseNode* WarehouseTree::searchLocation(const string& locationId) {
    return searchRecursive(root, locationId);
}

bool WarehouseTree::locationExists(const string& locationId) {
    return searchLocation(locationId) != nullptr;
}

void WarehouseTree::displayLayout() {
    cout << "\n=== Warehouse Layout ===" << endl;
    displayRecursive(root, 0);
}

void WarehouseTree::displayTraversal() {
    cout << "\n=== Warehouse Traversal (Pre-order) ===" << endl;
    traversalRecursive(root);
}

void WarehouseTree::displayPathFromEntrance(const string& destinationId) {
    WarehouseNode* destination = searchLocation(destinationId);
    if (destination == nullptr) {
        cout << "[Warehouse Layout] Route cannot be generated. Location "
             << destinationId << " does not exist." << endl;
        return;
    }

    // No STL stack/vector is used. Parent pointers are followed upward into a
    // fixed array, then printed backward to produce Entrance -> ... -> Shelf.
    WarehouseNode* path[MAX_WAREHOUSE_PATH_DEPTH];
    int count = 0;
    WarehouseNode* current = destination;

    while (current != nullptr && count < MAX_WAREHOUSE_PATH_DEPTH) {
        path[count] = current;
        count++;
        current = current->parent;
    }

    if (current != nullptr) {
        cout << "[Warehouse Layout] Path is deeper than the supported "
             << MAX_WAREHOUSE_PATH_DEPTH << " locations." << endl;
        return;
    }

    cout << "\n=== Path Generated for Robot Navigation ===" << endl;
    for (int i = count - 1; i >= 0; i--) {
        cout << path[i]->locationId;
        if (i > 0) {
            cout << " -> ";
        }
    }
    cout << endl;
    cout << "Distance from ENTRANCE: " << (count - 1)
         << " step(s)" << endl;
}

int WarehouseTree::calculateDistanceFromEntrance(const string& destinationId) {
    WarehouseNode* destination = searchLocation(destinationId);
    if (destination == nullptr) {
        return -1;
    }

    int distance = 0;
    WarehouseNode* current = destination;
    while (current->parent != nullptr) {
        distance++;
        current = current->parent;
    }
    return distance;
}
