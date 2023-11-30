#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "StudentRecord.h"
#include "Constants.h"

class HashTable {
private:
    StudentRecord** hashTable;
    StudentRecord** overflowTable;

    int hashTableCapacity;
    int overflowTableCapacity;

    int hashTableCurrentSize;
    int overflowTableCurrentSize;

public:
    HashTable();
    HashTable(int HTSize, int OTSize);
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
    ~HashTable();

    StudentRecord* searchRecord(const std::string& id);
    void insertRecord(StudentRecord& record);
    void deleteRecord(const std::string& id);
    void copyHashTable(const HashTable& other);
    int getHashTableCurrentSize() const { return hashTableCurrentSize; }
    int getOverflowTableCurrentSize() const { return overflowTableCurrentSize; }

    void printActiveRecords() const;
    void printDeletedRecords() const;

    int hashFunction(const std::string& id, int tableSize);
};

#endif // HASHTABLE_H