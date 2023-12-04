#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "StudentRecord.h"
#include "Constants.h"

class HashTable {
private:
    StudentRecord** hashTable = nullptr;
    StudentRecord** overflowTable = nullptr;

    int hashTableCapacity;
    int overflowTableCapacity;

    int hashTableCurrentSize;
    int overflowTableCurrentSize;
    int numDeletedRecords;

public:
    HashTable();
    HashTable(int HTSize, int OTSize);
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
    ~HashTable();
    void emptyTables();
    void cleanupPartiallyFilledTables(StudentRecord**& tempHashTable, StudentRecord**& tempOverflowTable, int hashTableCapacity, int overflowTableCapacity);

    StudentRecord* searchRecord(const std::string& id);
    void insertRecord(StudentRecord& record);
    void deleteRecord(const std::string& id);
    void copyHashTable(const HashTable& other, StudentRecord**& tempHashTable, StudentRecord**& tempOverflowTable);

    int getHashTableCurrentSize() const { return hashTableCurrentSize; }
    int getOverflowTableCurrentSize() const { return overflowTableCurrentSize; }
    int getHashTableCapacity() const { return hashTableCapacity; }
    int getOverflowTableCapacity() const { return overflowTableCapacity; }
    int getNumDeletedRecords() const { return numDeletedRecords; }

    void incrementNumDeletedRecords() { ++numDeletedRecords; }
    void setHashTableCapacity(int capacity) { hashTableCapacity = capacity; }
    void setOverflowTableCapacity(int capacity) { overflowTableCapacity = capacity; }

    bool hashTableIsFull() { return hashTableCurrentSize == hashTableCapacity; }
    bool overflowTableIsFull() { return overflowTableCurrentSize == overflowTableCapacity; }
    bool hashTableIsInitialized() const { return (hashTable != nullptr); }
    bool overflowTableIsInitialized() const { return (overflowTable != nullptr); }

    void printActiveRecords() const;
    void printDeletedRecords() const;

    int hashFunction(const std::string& id, int tableSize);
};

#endif // HASHTABLE_H