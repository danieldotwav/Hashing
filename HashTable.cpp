// HashTable.cpp
#include "HashTable.h"

// Default Constructor
HashTable::HashTable() 
    : hashTableCapacity(0), overflowTableCapacity(0), hashTable(nullptr), overflowTable(nullptr), hashTableCurrentSize(0), overflowTableCurrentSize(0), numDeletedRecords(0) {}

// Parameterized Constructor
HashTable::HashTable(int HTSize, int OTSize)
    : hashTableCapacity(HTSize), overflowTableCapacity(OTSize), hashTableCurrentSize(0), overflowTableCurrentSize(0), numDeletedRecords(0) {
    
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
    // pass copy object hashTable and overflowTable
    try {
        copyHashTable(other, hashTable, overflowTable);
    }
    catch (std::bad_alloc& e) {
        std::cerr << "\nError: Unable to Dynamically Allocate Memory. Returning Original Object\n";
        
        // Cleanup logic
        if (hashTable != nullptr) {
            for (int i = 0; i < other.hashTableCapacity; ++i) {
                if (hashTable[i] != nullptr) {
                    delete hashTable[i];
                }
            }
            delete[] hashTable;
        }
        if (overflowTable != nullptr) {
            for (int i = 0; i < other.overflowTableCapacity; ++i) {
                if (overflowTable[i] != nullptr) {
                    delete overflowTable[i];
                }
            }
            delete[] overflowTable;
        }
    }
}

// Copy Assignment Operator
HashTable& HashTable::operator=(const HashTable& other) {
    // use temp pointers to dynamically allocate
    // if it works  th to
    
    if (this != &other) {
        StudentRecord** tempHashTable;
        StudentRecord** tempOverflowTable;

        try {
            //copyHashTable(to, th, other);
            copyHashTable(other, tempHashTable, tempOverflowTable);
            // delete (HT, OT);
            emptyTables();
            // hashTable=th; over=to;
            hashTable = tempHashTable;
            overflowTable = tempOverflowTable;
        }
        catch (std::bad_alloc& e) {
            std::cerr << "\nError: Unable to Dynamically Allocate Memory. Returning Original Object\n";
        }
    }
    return *this;
}

// Destructor
HashTable::~HashTable() {
    // first check if hashtable and overflow table are nullptr
    // write a function that deletes all dynamically allocated memory in hashtable/overflow table 
    emptyTables();
}

// use in copy assignment operator after successfull copy
void HashTable::emptyTables() {
    if (hashTable != nullptr) {
        for (int i = 0; i < hashTableCapacity; ++i) {
            if (hashTable[i] != nullptr) {
                delete hashTable[i];
            }
        }
        delete[] hashTable;
        hashTable = nullptr;
    }
    
    if (overflowTable != nullptr) {
        for (int i = 0; i < overflowTableCurrentSize; ++i) {
            if (overflowTable[i] != nullptr) {
                delete overflowTable[i];
            }
        }
        delete[] overflowTable;
        overflowTable = nullptr;
    }
}

// in the assignemnt operator the th and to are the temporary pointers created using the left objects memebers
// dont need the temporary pointers in the 
// dont delete in the copy hast table function
void HashTable::copyHashTable(const HashTable& other, StudentRecord**& tempHashTable, StudentRecord**& tempOverflowTable) {
    try {
        // Allocate new memory for the tables
        tempHashTable = new StudentRecord * [other.hashTableCapacity];
        tempOverflowTable = new StudentRecord * [other.overflowTableCapacity];

        // Initialize and copy hash table records
        for (int i = 0; i < other.hashTableCapacity; ++i) {
            if (other.hashTable[i] != nullptr) {
                tempHashTable[i] = new StudentRecord(*(other.hashTable[i])); // Deep copy of the StudentRecord
            }
            else {
                tempHashTable[i] = nullptr;
            }
        }

        // Initialize and copy overflow table records
        for (int i = 0; i < other.overflowTableCapacity; ++i) {
            if (other.overflowTable[i] != nullptr) {
                tempOverflowTable[i] = new StudentRecord(*(other.overflowTable[i])); // Deep copy of the StudentRecord
            }
            else {
                tempOverflowTable[i] = nullptr;
            }
        }

        // Copy the capacities and current sizes
        hashTableCapacity = other.hashTableCapacity;
        overflowTableCapacity = other.overflowTableCapacity;
        hashTableCurrentSize = other.hashTableCurrentSize;
        overflowTableCurrentSize = other.overflowTableCurrentSize;
        numDeletedRecords = other.numDeletedRecords;
    }
    catch (std::bad_alloc& e) {
		std::cerr << "Error: Freeing up memory for temporary hash tables\n";
        
        if (tempHashTable != nullptr) {
            for (int i = 0; i < other.hashTableCapacity; ++i) {
                if (tempHashTable[i] != nullptr) {
                    delete tempHashTable[i];
                }
            }
            delete[] tempHashTable;
            tempHashTable = nullptr;
        }

        if (tempOverflowTable != nullptr) {
            for (int i = 0; i < other.overflowTableCurrentSize; ++i) {
                if (tempOverflowTable[i] != nullptr) {
                    delete tempOverflowTable[i];
                }
            }
            delete[] tempOverflowTable;
            tempOverflowTable = nullptr;
        }
	}
}

// Search for a record in the hash table
StudentRecord* HashTable::searchRecord(const std::string& id) {
    int index = hashFunction(id, hashTableCapacity);
    bool isFound = false;

    //StudentRecord* current = hashTable[index];
    StudentRecord* result = nullptr;
    int iterations = 0;
    
    // Search HashTable First
    while ((hashTable[index] != nullptr && hashTable[index]->getID() != id) && iterations < hashTableCapacity) {
        index++;
        iterations++;

        if (index == hashTableCapacity) {
            index = 0;
        }
    }
    if (hashTable[index] != nullptr) {
        if (hashTable[index]->getID() == id && hashTable[index]->getDeletedStatus() == false) {
            result = hashTable[index];
        }
        else if (iterations == hashTableCapacity) {
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
    }
  
    return result;
}

// Insert a record into the hash table
void HashTable::insertRecord(StudentRecord& record) {
    int index = hashFunction(record.getID(), hashTableCapacity);
    StudentRecord* insertRecord = new StudentRecord(record);

    if (hashTableIsFull()) {
        if (overflowTableIsFull()) {
            insertRecord->setRecordLocation("Unprocessed");
        }
        else {
            insertRecord->setRecordLocation("Overflow");
            overflowTable[overflowTableCurrentSize] = insertRecord;
            overflowTableCurrentSize++;
        }
    }
    else {
        while (hashTable[index] != nullptr) {
            index++;

            if (index == hashTableCapacity) {
                index = 0; // loop around
            }
        }

        insertRecord->setRecordLocation("Hash");
        hashTable[index] = insertRecord;
        hashTableCurrentSize++;
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
