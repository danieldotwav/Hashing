#include "HashTable.h"
#include "StudentRecord.h"
#include <fstream>
using namespace std;

enum MenuOption {
    CREATE_TABLES = 1,
    SEARCH_RECORD,
    DELETE_RECORD,
    PRINT_ACTIVE_RECORDS,
    PRINT_DELETED_RECORDS,
    PRINT_UNPROCESSED_RECORDS,
    EXIT,
    ERROR_MENU
};

enum HashTableSize {
    TEN = 10,
    ONE_HUNDRED = 100,
    ONE_THOUSAND = 1000,
    INVALID_HASH_SIZE = -999
};


void createTables(HashTable*& hashTable);

void purgeInputErrors(string errorMessage);

int main() {
    cout << "Welcome to the Student Record System\n";

	HashTable* hashTable = nullptr;
    MenuOption menuOption = CREATE_TABLES; // default state
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
                createTables(hashTable);
                
                break;
            case SEARCH_RECORD:
                // Logic to search for a record by ID
                break;
            case DELETE_RECORD:
                // Logic to delete a record
                break;
            case PRINT_ACTIVE_RECORDS:
                // Logic to print all active records
                break;
            case PRINT_DELETED_RECORDS:
                // Logic to print all deleted records
                break;
            case PRINT_UNPROCESSED_RECORDS:
                // Logic to print unprocessed records
                break;
            case EXIT:
                cout << "Thank You, Have A Great Day\n";
                delete hashTable; // Ensure proper memory management
                break;
            default:
                purgeInputErrors("\nError: Invalid Menu Selection\n");
                menuOption = ERROR_MENU; // Resetting to default state
        }
    } while (menuOption != EXIT);

    cout << "Terminating Program\n";
	return 0;
}

void createTables(HashTable*& hashTable) {
    cout << "\n--Generate Hash & Overflow Tables--";
    HashTableSize hashTableSize = INVALID_HASH_SIZE;
    int userInput{ 0 };

    while (hashTableSize == INVALID_HASH_SIZE) {
        cout << "\nPlease enter the size of the hash table (10, 100, 1000): ";
        cin >> userInput;

        switch (userInput) {
            case TEN:
            case ONE_HUNDRED:
            case ONE_THOUSAND:
                hashTableSize = static_cast<HashTableSize>(userInput);
                cout << "Creating Hash Table of Size " << userInput << endl;
                break;
            default:
                purgeInputErrors("\nError: Invalid Hash Table Size\n");
                hashTableSize = INVALID_HASH_SIZE;
        }
    } 
    
    try {
        if (hashTable != nullptr) {
            delete hashTable; // Delete existing hashTable if any
            hashTable = nullptr;
        }
        hashTable = new HashTable(hashTableSize);
        cout << "Hash and overflow tables created successfully.\n";
    }
    catch (bad_alloc& ex) {
		cout << "Error: Unable to allocate memory for hash table. Please try again later\n";
	}
}

void purgeInputErrors(string errorMessage) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << errorMessage;
}