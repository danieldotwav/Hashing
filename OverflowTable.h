// OverflowTable.h
#ifndef OVERFLOWTABLE_H
#define OVERFLOWTABLE_H

#include "StudentRecord.h"

class OverflowTable {
private:
    StudentRecord** overflowRecords;
    int capacity;
    int count;
    void resize();

public:
    OverflowTable(int initialSize = 10);
    ~OverflowTable();
    void insertRecord(const StudentRecord& record);
    StudentRecord* searchRecord(const std::string& id);
    void deleteRecord(const std::string& id);
};

#endif // OVERFLOWTABLE_H