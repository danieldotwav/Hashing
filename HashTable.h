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

    StudentRecord* searchRecord(const std::string& id);
    void insertRecord(StudentRecord& record);
    void deleteRecord(const std::string& id);
    void copyHashTable(const HashTable& other, StudentRecord**& tempHashTable, StudentRecord**& tempOverflowTable);


    int getHashTableCurrentSize() const { return hashTableCurrentSize; }
    int getOverflowTableCurrentSize() const { return overflowTableCurrentSize; }
    int getNumDeletedRecords() const { return numDeletedRecords; }
    void incrementNumDeletedRecords() { ++numDeletedRecords; }

    bool hashTableIsFull() { return hashTableCurrentSize == hashTableCapacity; }
    bool overflowTableIsFull() { return overflowTableCurrentSize == overflowTableCapacity; }

    void printActiveRecords() const;
    void printDeletedRecords() const;
    void emptyTables();

    int hashFunction(const std::string& id, int tableSize);
};

#endif // HASHTABLE_H