// OverflowTable.cpp
#include "OverflowTable.h"

// Constructor
OverflowTable::OverflowTable(int initialSize) : capacity(initialSize), count(0) {
    overflowRecords = new StudentRecord * [capacity];
    // Initialize all elements to nullptr
    for (int i = 0; i < capacity; ++i) {
        overflowRecords[i] = nullptr;
    }
}

// Destructor
OverflowTable::~OverflowTable() {
    for (int i = 0; i < count; ++i) {
        delete overflowRecords[i];
    }
    delete[] overflowRecords;
}

// Resize method for dynamic array expansion
void OverflowTable::resize() {
    int newCapacity = capacity * 2;
    StudentRecord** newTable = new StudentRecord * [newCapacity];
    // Initialize all elements to nullptr
    for (int i = 0; i < newCapacity; ++i) {
        newTable[i] = nullptr;
    }
    // Copy existing records
    for (int i = 0; i < count; ++i) {
        newTable[i] = overflowRecords[i];
    }
    delete[] overflowRecords;
    overflowRecords = newTable;
    capacity = newCapacity;
}

// Insert a record into the overflow table
void OverflowTable::insertRecord(const StudentRecord& record) {
    if (count >= capacity) {
        resize();
    }
    overflowRecords[count++] = new StudentRecord(record);
}

// Search for a record in the overflow table
StudentRecord* OverflowTable::searchRecord(const std::string& id) {
    StudentRecord* foundRecord = nullptr;
    for (int i = 0; i < count && foundRecord == nullptr; ++i) {
        if (overflowRecords[i]->getId() == id) {
            foundRecord = overflowRecords[i];
        }
    }
    return foundRecord;
}

// Delete a record from the overflow table
void OverflowTable::deleteRecord(const std::string& id) {
    int foundIndex = -1;
    for (int i = 0; i < count && foundIndex == -1; ++i) {
        if (overflowRecords[i]->getId() == id) {
            foundIndex = i;
        }
    }

    if (foundIndex != -1) {
        delete overflowRecords[foundIndex];
        for (int i = foundIndex; i < count - 1; ++i) {
            overflowRecords[i] = overflowRecords[i + 1];
        }
        count--;
    }
}