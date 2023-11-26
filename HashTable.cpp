// HashTable.cpp
#include "HashTable.h"

// Constructor
HashTable::HashTable(int size) : capacity(size) {
    table = new Node * [capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

// Destructor
HashTable::~HashTable() {
    for (int i = 0; i < capacity; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] table;
}

// Search for a record in the hash table
StudentRecord* HashTable::searchRecord(const std::string& id) {
    int index = hashFunction(id, capacity);
    Node* current = table[index];
    StudentRecord* result = nullptr;

    while (current != nullptr && result == nullptr) {
        if (current->data.getId() == id) {
            result = &current->data;
        }
        else {
            current = current->next;
        }
    }

    return result;
}

// Insert a record into the hash table
void HashTable::insertRecord(const StudentRecord& record) {
    int index = hashFunction(record.getId(), capacity);
    Node* newNode = new Node(record);
    newNode->next = table[index];
    if (table[index] != nullptr) {
        table[index]->prev = newNode;
    }
    table[index] = newNode;
}

// Delete a record from the hash table
void HashTable::deleteRecord(const std::string& id) {
    int index = hashFunction(id, capacity);
    Node* current = table[index];
    bool found = false;

    while (current != nullptr && !found) {
        if (current->data.getId() == id) {
            found = true;

            if (current->prev != nullptr) {
                current->prev->next = current->next;
            }
            else { // Node is the first in the list
                table[index] = current->next;
            }

            if (current->next != nullptr) {
                current->next->prev = current->prev;
            }

            delete current;
        }
        else {
            current = current->next;
        }
    }
}

// Convert the student ID to an integer value
int HashTable::convertToInteger(const std::string& id) {
    unsigned long value = 0;
    for (char ch : id) {
        if (isdigit(ch)) {
            value = value * 10 + (ch - '0');
        }
    }
    return value;
}

// Hash function using a combination of extraction and mid-square methods
int hashFunction(const std::string& studentId, int tableSize) {
    std::string extracted = studentId.substr(2); // Extraction
    std::string part1 = extracted.substr(0, 3);
    std::string part2 = extracted.substr(3, 4);

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