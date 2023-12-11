## Overview
This project implements a menu-driven console program for managing hash and overflow tables. It provides functionality for creating tables, searching for records by ID, deleting records, and printing records based on their status (active, deleted, unprocessed).

## Features
- **Create Hash and Overflow Tables**: Initialize tables with specified capacities.
- **Search for Records**: Locate a record using its unique ID.
- **Delete Records**: Remove records from the tables.
- **Print Records**: Display active, deleted, or unprocessed records.

## Getting Started
### Prerequisites
- C++ Compiler (GCC, Clang, MSVC, etc.)
- Basic knowledge of C++ and data structures

### Installation
1. Clone the repository to your local machine:
   ``` git clone [github.com/danieldotwav/Hashing]
2. Compile the source files. For example, using GCC:
   ``` g++ -o hash_table source.cpp HashTable.cpp
3. Run the compiled program:
   ``` ./hash_table

## Usage
After running the program, follow the on-screen menu to interact with the hash table system. The menu includes options for creating tables, searching, deleting, and printing records.

## How It Works
### Data Structure
- **Hash Table**: A primary table for storing records.
- **Overflow Table**: An auxiliary table used when the hash table is full.

### Operations
- **Insertion**: Records are added to the hash table or overflow table based on availability.
- **Search**: The program searches the hash table first, then the overflow table if necessary.
- **Deletion**: Marks a record as deleted without physically removing it from the table.
- **Printing**: Displays records based on their status.

## Contributing
Contributions to the project are welcome. Please ensure to follow the project's coding standards and submit a pull request for review.
