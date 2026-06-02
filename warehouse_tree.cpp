#include "warehouse_tree.hpp"

#include <iostream>

const int MAX_WAREHOUSE_PATH_DEPTH = 50;

WarehouseTree::WarehouseTree() {
    root = createNode("ENTRANCE", "Main Entrance", "Entrance", nullptr);
}

WarehouseTree::~WarehouseTree() {
    destroyRecursive(root);
    root = nullptr;
}

WarehouseNode* WarehouseTree::createNode(const std::string& locationId,
                                         const std::string& locationName,
                                         const std::string& locationType,
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
                                              const std::string& locationId) {
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
        std::cout << "  ";
    }

    std::cout << current->locationId << " - " << current->locationName
              << " (" << current->locationType << ")" << std::endl;

    displayRecursive(current->firstChild, level + 1);
    displayRecursive(current->nextSibling, level);
}

void WarehouseTree::traversalRecursive(WarehouseNode* current) {
    if (current == nullptr) {
        return;
    }

    std::cout << current->locationId << " [" << current->locationType << "]"
              << std::endl;

    traversalRecursive(current->firstChild);
    traversalRecursive(current->nextSibling);
}

void WarehouseTree::addLocation(const std::string& parentId,
                                const std::string& locationId,
                                const std::string& locationName,
                                const std::string& locationType) {
    if (locationExists(locationId)) {
        std::cout << "[Warehouse Layout] Location " << locationId
                  << " already exists. Duplicate not added." << std::endl;
        return;
    }

    WarehouseNode* parent = searchLocation(parentId);
    if (parent == nullptr) {
        std::cout << "[Warehouse Layout] Parent location " << parentId
                  << " was not found. Cannot add " << locationId << "."
                  << std::endl;
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

WarehouseNode* WarehouseTree::searchLocation(const std::string& locationId) {
    return searchRecursive(root, locationId);
}

bool WarehouseTree::locationExists(const std::string& locationId) {
    return searchLocation(locationId) != nullptr;
}

void WarehouseTree::displayLayout() {
    std::cout << "\n=== Warehouse Layout ===" << std::endl;
    displayRecursive(root, 0);
}

void WarehouseTree::displayTraversal() {
    std::cout << "\n=== Warehouse Traversal (Pre-order) ===" << std::endl;
    traversalRecursive(root);
}

void WarehouseTree::displayPathFromEntrance(const std::string& destinationId) {
    WarehouseNode* destination = searchLocation(destinationId);
    if (destination == nullptr) {
        std::cout << "[Warehouse Layout] Route cannot be generated. Location "
                  << destinationId << " does not exist." << std::endl;
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
        std::cout << "[Warehouse Layout] Path is deeper than the supported "
                  << MAX_WAREHOUSE_PATH_DEPTH << " locations." << std::endl;
        return;
    }

    std::cout << "\n=== Path Generated for Robot Navigation ===" << std::endl;
    for (int i = count - 1; i >= 0; i--) {
        std::cout << path[i]->locationId;
        if (i > 0) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
    std::cout << "Distance from ENTRANCE: " << (count - 1)
              << " step(s)" << std::endl;
}

int WarehouseTree::calculateDistanceFromEntrance(const std::string& destinationId) {
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
