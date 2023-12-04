/* Override the global 'new' operator to test DMA failure after the specified allocation count */
/*
#include <new>
#include <cstddef>
#include <iostream>

static int allocationCount = 0;
static int failOnCount = 10000; // Set this to the allocation count on which you want to fail

void* operator new(std::size_t size) throw(std::bad_alloc) {
    if (++allocationCount >= failOnCount) {
        throw std::bad_alloc();

    }
    return malloc(size);
}

/*
void operator delete(void* memory) throw() {
    free(memory);
}
*/


/* End of 'new' operator override */

#include "HashTable.h"
#include <fstream>
using namespace std;

// Wrapper function for getting valid table size, creating hash/overflow tables, and filling tables from input file
void CreateAndPopulateTables(HashTable*& table);
void setTableSizes(int& hashTableSize, int& overflowTableSize);
void initializeTables(HashTable*& table, int hashTableSize, int overflowTableSize);
void populateTables(HashTable* table, int hashTableSize, int overflowTableSize);

StudentRecord* getRecordID(HashTable* table);
void searchRecord(HashTable* table, stringstream& header);
void deleteRecord(HashTable* table, stringstream& header);
void printActiveRecords(HashTable* table, stringstream& header);
void printDeletedRecords(HashTable* table, stringstream& header);
void printUnprocessedRecords(HashTable* table, stringstream& header);
void purgeInputErrors(string errorMessage);

int main() {
    cout << "Welcome to the Student Record System\n";

    stringstream ssHeader;
    ssHeader << left << setw(ID_WIDTH) << "ID" << setw(NAME_WIDTH) << "Last Name" 
        << setw(NAME_WIDTH) << "First Name" << setw(UNITS_WIDTH) << "Units" 
        << setw(LOCATION_WIDTH) << "Location" << endl;

	HashTable* table = nullptr;
    MenuOption menuOption = CREATE_TABLES; // Default State
    int userInput{ 0 };

    do {
        cout << "\nSelect one of the following:\n"
            "1. Create hash and overflow tables\n"
            "2. Search for a record by ID\n"
            "3. Delete a record\n"
            "4. Print all active records\n"
            "5. Print all deleted records\n"
            "6. Print unprocessed records\n"
            "7. Exit\n"
            "Selection: ";
        cin >> userInput;

        switch (userInput) {
            case CREATE_TABLES:
                CreateAndPopulateTables(table);
                break;
            case SEARCH_RECORD:
                searchRecord(table, ssHeader);
                break;
            case DELETE_RECORD:
                deleteRecord(table, ssHeader);
                break;
            case PRINT_ACTIVE_RECORDS:
                printActiveRecords(table, ssHeader);
                break;
            case PRINT_DELETED_RECORDS:
                printDeletedRecords(table, ssHeader);
                break;
            case PRINT_UNPROCESSED_RECORDS:
                printUnprocessedRecords(table, ssHeader);
                break;
            case EXIT:
                cout << "\nHave A Great Day!\n";
                menuOption = EXIT;
                break;
            default:
                purgeInputErrors("\nError: Invalid Menu Selection\n");
                menuOption = ERROR_MENU; // Resetting to default state
        }
    } while (menuOption != EXIT);
    
    if (table != nullptr) {
        delete table;
    }
    
    cout << "Terminating Program\n";
	return 0;
}

void CreateAndPopulateTables(HashTable*& table) {
    cout << "\n--Generate Hash & Overflow Tables--";
    int hashTableSize{ 0 };
    int overflowTableSize{ 0 };

    // DETERMINE HASH/OVERFLOW TABLE SIZE
    setTableSizes(hashTableSize, overflowTableSize);
    
    try {
        // ALLOCATE MEMORY FOR NEW HASHTABLE
        initializeTables(table, hashTableSize, overflowTableSize);

        // FILL THE HASHTABLE/OVERFLOW TABLE
        if (table->hashTableIsInitialized() && table->overflowTableIsInitialized()) {
            // First Check For Existing Tables
            populateTables(table, hashTableSize, overflowTableSize);
            cout << "\n\nHash/Overflow Tables *Populated* With Student Records Successfully\n";
        }
    }
    catch (bad_alloc& ex) {
        cout << "\nError: DMA Failure. Unable to *Populate* HashTable/OverflowTable With Student Records. Please select a smaller HashTable size or try again later\n";
        
        if (table != nullptr) {
            table->emptyTables();
            table = nullptr;
        }
    }
}

void setTableSizes(int& hashTableSize, int& overflowTableSize) {
    HashTableSize size = INVALID_HASH_SIZE;
    int userInput{ 0 };

    while (size == INVALID_HASH_SIZE) {
        cout << "\nPlease enter the size of the hash table (10, 100, 1000): ";
        cin >> userInput;

        switch (userInput) {
        case TEN:
        case ONE_HUNDRED:
        case ONE_THOUSAND:
            hashTableSize = static_cast<HashTableSize>(userInput);
            size = static_cast<HashTableSize>(userInput);
            break;
        default:
            purgeInputErrors("\nError: Invalid Hash Table Size\n");
            size = INVALID_HASH_SIZE;
        }
    }

    overflowTableSize = OVERFLOW_TABLE_SIZE_MODIFIER * hashTableSize;
    if ((OVERFLOW_TABLE_SIZE_MODIFIER * hashTableSize) != overflowTableSize) {
        overflowTableSize++;
    }

    cout << "\nCreating Hash Table of Size " << userInput << "...\n"
        << "Creating Overflow Table of Size " << overflowTableSize << "...\n";
}

void initializeTables(HashTable*& table, int hashTableSize, int overflowTableSize) {
    if (table != nullptr) {
        delete table; // Delete existing hashTable if any
        table = nullptr;
    }

    table = new HashTable(hashTableSize, overflowTableSize);

    // If the object cannot be created, its table members are cleaned up in the constructor
    // So we check if either table is uninitialized to determine if the object was created
    if (table != nullptr && table->hashTableIsInitialized() && table->overflowTableIsInitialized()) {
        cout << "\nHash/Overflow Tables *Initialized* Successfully\n";
    }
    else {
        delete table;
        table = nullptr;
        cout << "Error: DMA Failure. Unable to *Initialize* Hash/Overflow Tables. Please Try Again Later\n";
    }
}

void populateTables(HashTable* table, int hashTableSize, int overflowTableSize) {
    ifstream studentRecordsFile("StudentRecords.txt");
    ofstream unprocessedRecordsFile("UnprocessedRecords.txt");

    if (!studentRecordsFile) {
        cout << "\nError: Input File Not Found. Terminating Program\n";
        exit(EXIT_FAILURE);
    }
    if (studentRecordsFile.peek() == EOF) {
        cout << "\nError: Input File Empty. Unable to Process Data. Terminating Program";
        studentRecordsFile.close();
        exit(EXIT_FAILURE);
    }

    // Handle failure to open output file
    bool canWriteToUnprocessedFile = true;
    if (!unprocessedRecordsFile) {
        cout << "\nWarning: Unable to Generate/Modify UnprocessedRecords.txt File\n"
            "Any records that do not fit in the Hash/Overflow tables will be printed to the console\n";
        canWriteToUnprocessedFile = false;
    }

    string tempID, tempLastName, tempFirstName;
    int tempUnits;

    int processedRecordsCount = 0, unprocessedRecordsCount = 0, allOtherRecordsCount = 0;
    stringstream ssAllOtherRecords("\nRecords Not in Hash/Overflow Tables or Unprocessed Records File:\n");

    // FILL HASH/OVERFLOW TABLES
    int index = 0;
    while (studentRecordsFile >> tempID >> tempLastName >> tempFirstName >> tempUnits) {
        StudentRecord record(tempID, tempLastName, tempFirstName, tempUnits);
        table->insertRecord(record);

        // HANDLE UNPROCESSED RECORDS
        if (record.getRecordLocation() == "Unprocessed") {
            if (canWriteToUnprocessedFile) {
                unprocessedRecordsFile << tempID << " " << tempLastName << " " 
                    << tempFirstName << " " << tempUnits << "\n";

                unprocessedRecordsCount++;
            }
            else {
                ssAllOtherRecords << tempID << " " << tempLastName << " " 
                    << tempFirstName << " " << tempUnits << "\n";

                allOtherRecordsCount++;
            }
        }
        else {
            processedRecordsCount++;
        }
    }

    cout << "\n- Successfully Stored " << table->getHashTableCurrentSize() 
        << " Records in HashTable\n";
    cout << "- Successfully Stored " << table->getOverflowTableCurrentSize() 
        << " Records in OverflowTable\n";
    cout << "- Successfully Stored " << unprocessedRecordsCount << " Records in UnprocessedRecords.txt";

    unprocessedRecordsFile.close();
    studentRecordsFile.close();
}

StudentRecord* getRecordID(HashTable* table) {
    StudentRecord* result = nullptr;
    int input = 0;

    cout << "\nPlease enter the ID of the record you would like to search for: ";
    cin >> input;

    if (cin.fail()) {
        purgeInputErrors("\nError: Invalid ID\n");
    }
    else if (to_string(input).length() == ID_TOTAL_LENGTH) {
        string id = to_string(input);
        result = table->searchRecord(id);
    }
    return result;
}

void searchRecord(HashTable* table, stringstream& header) {
    if (table != nullptr) {
        StudentRecord* record = getRecordID(table);
        if (record != nullptr) {
            cout << "\n-Record Found-\n\n" << header.str() << record->toString();
		}
		else {
			cout << "\n-Record Not Found-\n";
        }
    }
    else {
		cout << "\nUninitialized Hash Table: Cannot Search For Record\n";
	}
}

void deleteRecord(HashTable* table, stringstream& header) {
    if (table != nullptr) {
        StudentRecord* record = getRecordID(table);
        if (record != nullptr) {
            cout << "\n-Record Deleted Successfully-\n\n" << header.str() << record->toString();
            record->setDeletedStatus(true);
            table->incrementNumDeletedRecords();
		}
        else {
			cout << "\n-Record Not Found-\n";
		}
	}
    else {
		cout << "\nUninitialized Hash Table: Cannot Delete Record\n";
	}
}

void printActiveRecords(HashTable* table, stringstream& header) {
    if (table != nullptr) {
        cout << "\n--Active Records--\n\n" << header.str();
        table->printActiveRecords();
    }
    else {
        cout << "\nUninitialized Hash Table: Cannot Print Active Records\n";
    }
}

void printDeletedRecords(HashTable* table, stringstream& header) {
    if (table != nullptr) {
        if (table->getNumDeletedRecords() > 0) {
            cout << "\n--Deleted Records--\n\n" << header.str();
            table->printDeletedRecords();
        }
        else {
            cout << "\n-No Deleted Records Found-\n";
        }
    }
    else {
        cout << "\nUninitialized Hash Table: Cannot Print Deleted Records\n";
    }
}

void printUnprocessedRecords(HashTable* table, stringstream& header) {
    if (table != nullptr) {
		cout << "\n--Unprocessed Records--\n\n";
        fstream unprocessedRecordsFile("UnprocessedRecords.txt");

        if (!unprocessedRecordsFile) {
            cout << "\nError: UnprocesedRecords.txt File Not Found. Cannot Print Records\n";
        }
        else {
            if (unprocessedRecordsFile.peek() == EOF) {
                cout << "\nUnprocessed Records File Empty. Cannot Print Records";
            }
            else {
                string tempID, tempLastName, tempFirstName;
                int tempUnits, unprocessedRecordsCount = 0;
                cout << header.str();

                while (unprocessedRecordsFile >> tempID >> tempLastName >> tempFirstName >> tempUnits) {
                    cout << left << setw(ID_WIDTH) << tempID << setw(NAME_WIDTH) << tempLastName
                        << setw(NAME_WIDTH) << tempFirstName << right << setw(UNITS_COL1) << tempUnits
                        << setw(UNITS_COL2) << "" << left << setw(LOCATION_WIDTH) << "Unprocessed" << endl;
                    unprocessedRecordsCount++;
                }
                cout << "\nSuccessfully Printed " << unprocessedRecordsCount << " Records\n";
            }
            unprocessedRecordsFile.close();
        }
	}
    else {
		cout << "\nUninitialized Hash Table: Cannot Print Unprocessed Records\n";
    }
}

void purgeInputErrors(string errorMessage) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << errorMessage;
}