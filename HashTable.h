#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "StudentRecord.h"
#include "Node.h"

class HashTable {
private:
    Node** table;
    int capacity;

public:
    HashTable(int size);
    ~HashTable();
    void insertRecord(const StudentRecord& record);
    StudentRecord* searchRecord(const std::string& id);
    void deleteRecord(const std::string& id);

    int convertToInteger(const std::string& id);
    int hashFunction(const std::string& id, int tableSize);
};

#endif // HASHTABLE_H