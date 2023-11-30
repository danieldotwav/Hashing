// HashTable.cpp
#include "HashTable.h"

// Default Constructor
HashTable::HashTable() 
    : hashTableCapacity(0), overflowTableCapacity(0), hashTable(nullptr), overflowTable(nullptr), hashTableCurrentSize(0), overflowTableCurrentSize(0) {}

// Parameterized Constructor
HashTable::HashTable(int HTSize, int OTSize)
    : hashTableCapacity(HTSize), overflowTableCapacity(OTSize), hashTableCurrentSize(0), overflowTableCurrentSize(0) {
    
    hashTable = new StudentRecord * [hashTableCapacity];
    for (int i = 0; i < hashTableCapacity; ++i) {
        hashTable[i] = nullptr;
    }

    overflowTable = new StudentRecord * [overflowTableCapacity];
    for (int i = 0; i < overflowTableCapacity; ++i) {
        overflowTable[i] = nullptr;
    }
}

// Copy Constructor
HashTable::HashTable(const HashTable& other) {
    copyHashTable(other);
}

// Copy Assignment Operator
HashTable& HashTable::operator=(const HashTable& other) {
    if (this != &other) {
        copyHashTable(other);
    }
    return *this;
}

// Destructor
HashTable::~HashTable() {
    for (int i = 0; i < hashTableCapacity; ++i) {
        delete hashTable[i];
    }
    delete[] hashTable;

    for (int i = 0; i < overflowTableCurrentSize; ++i) {
        delete overflowTable[i];
    }
    delete[] overflowTable;
}

// Search for a record in the hash table
StudentRecord* HashTable::searchRecord(const std::string& id) {
    int index = hashFunction(id, hashTableCapacity);
    bool isFound = false;

    StudentRecord* current = hashTable[index];
    StudentRecord* result = nullptr;

    if (hashTable[index] != nullptr && hashTable[index]->getID() == id) {
        // Search HashTable First
        if (hashTable[index]->getDeletedStatus() == false) {
            result = hashTable[index];
        }
    }
    else {
        // Search OverflowTable Next
        for (int i = 0; i < overflowTableCurrentSize && !isFound; i++) {
            if (overflowTable[i]->getID() == id) {
                isFound = true;

                if (overflowTable[i]->getDeletedStatus() == false) {
                    result = overflowTable[i];
                }
            }
        }
    }
    return result;
}

// Insert a record into the hash table
void HashTable::insertRecord(StudentRecord& record) {
    int index = hashFunction(record.getID(), hashTableCapacity);
    StudentRecord* insertRecord = new StudentRecord(record);
    
    if (hashTable[index] == nullptr) {
        insertRecord->setRecordLocation("Hash");
        hashTable[index] = insertRecord;

        hashTableCurrentSize++;
    }
    else {
        if (overflowTableCurrentSize < overflowTableCapacity) {
            insertRecord->setRecordLocation("Overflow");
            overflowTable[overflowTableCurrentSize] = insertRecord;

            overflowTableCurrentSize++;
        }
        else {
            insertRecord->setRecordLocation("Unprocessed");
        }
    }

    record.setRecordLocation(insertRecord->getRecordLocation());
}

// Delete a record from the hash table
void HashTable::deleteRecord(const std::string& id) {
    int index = hashFunction(id, hashTableCapacity);

    bool isDeleted = false;

    // Search in the hash table
    if (hashTable[index] != nullptr && hashTable[index]->getID() == id) {
        if (!hashTable[index]->getDeletedStatus()) {
            hashTable[index]->setDeletedStatus(true);
            std::cout << "\nRecord Deleted Successfully\n";
            isDeleted = true;
        }
        else {
            std::cout << "\nRecord Already Marked as Deleted\n";
            isDeleted = true;
        }
    }

    // Search in the overflow table, if not found/deleted in the hash table
    for (int i = 0; i < overflowTableCurrentSize && !isDeleted; i++) {
        if (overflowTable[i] != nullptr && overflowTable[i]->getID() == id) {
            if (!overflowTable[i]->getDeletedStatus()) {
                overflowTable[i]->setDeletedStatus(true);
                std::cout << "\nRecord Deleted Successfully\n";
                isDeleted = true;
            }
            else {
                std::cout << "\nRecord Already Marked as Deleted\n";
                isDeleted = true;
            }
        }
    }

    // If the record was not found or already marked as deleted
    if (!isDeleted) {
        std::cout << "\nRecord Not Found or Already Deleted\n";
    }
}

void HashTable::copyHashTable(const HashTable& other) {
    // Delete the current tables
    for (int i = 0; i < hashTableCapacity; ++i) {
        delete hashTable[i];
    }
    delete[] hashTable;

    for (int i = 0; i < overflowTableCapacity; ++i) {
        delete overflowTable[i];
    }
    delete[] overflowTable;

    // Copy the capacities and current sizes
    hashTableCapacity = other.hashTableCapacity;
    overflowTableCapacity = other.overflowTableCapacity;
    hashTableCurrentSize = other.hashTableCurrentSize;
    overflowTableCurrentSize = other.overflowTableCurrentSize;

    // Allocate new memory for the tables
    hashTable = new StudentRecord * [hashTableCapacity];
    overflowTable = new StudentRecord * [overflowTableCapacity];

    // Initialize and copy hash table records
    for (int i = 0; i < hashTableCapacity; ++i) {
        if (other.hashTable[i] != nullptr) {
            hashTable[i] = new StudentRecord(*(other.hashTable[i])); // Deep copy of the StudentRecord
        }
        else {
            hashTable[i] = nullptr;
        }
    }

    // Initialize and copy overflow table records
    for (int i = 0; i < overflowTableCapacity; ++i) {
        if (other.overflowTable[i] != nullptr) {
            overflowTable[i] = new StudentRecord(*(other.overflowTable[i])); // Deep copy of the StudentRecord
        }
        else {
            overflowTable[i] = nullptr;
        }
    }
}

void HashTable::printActiveRecords() const {
    // Print Active Records in Hash Table
    for (int i = 0; i < hashTableCapacity; ++i) {
        if (hashTable[i] != nullptr && !hashTable[i]->getDeletedStatus()) {
			std::cout << hashTable[i]->toString();
		}
	}

    // Print Active Records in Overflow Table
    for (int i = 0; i < overflowTableCurrentSize; ++i) {
        if (overflowTable[i] != nullptr && !overflowTable[i]->getDeletedStatus()) {
            std::cout << overflowTable[i]->toString();
        }
    }
}

void HashTable::printDeletedRecords() const {
    // Print Records Marked For Deletion in Hash Table
    for (int i = 0; i < hashTableCapacity; ++i) {
        if (hashTable[i] != nullptr && hashTable[i]->getDeletedStatus()) {
			std::cout << hashTable[i]->toString();
		}
	}

    // Print Records Marked For Deletion in Overflow Table
    for (int i = 0; i < overflowTableCurrentSize; ++i) {
        if (overflowTable[i] != nullptr && overflowTable[i]->getDeletedStatus()) {
            std::cout << overflowTable[i]->toString();
        }
    }
}

// Hash function using a combination of extraction and mid-square methods
int HashTable::hashFunction(const std::string& studentId, int tableSize) {
    std::string extracted = studentId.substr(ID_STARTING_POS);
    std::string part1 = extracted.substr(0, ID_PART1_LENGTH);
    std::string part2 = extracted.substr(ID_PART1_LENGTH, ID_PART2_LENGTH);

    // Boundary Folding
    std::reverse(part1.begin(), part1.end());
    std::reverse(part2.begin(), part2.end());
    int folded = std::stoi(part1) + std::stoi(part2);

    // Mid-Squared
    long long squared = static_cast<long long>(folded) * folded;
    std::string squaredStr = std::to_string(squared);
    int midIndex = squaredStr.length() / 2;
    int numDigits = std::to_string(tableSize).length();
    std::string midDigits = squaredStr.substr(midIndex - numDigits / 2, numDigits);
    int hashIndex = std::stoi(midDigits) % tableSize;

    return hashIndex;
}
