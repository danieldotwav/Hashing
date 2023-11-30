#ifndef CONSTANTS_H
#define CONSTANTS_H

enum HashTableSize {
    TEN = 10,
    ONE_HUNDRED = 100,
    ONE_THOUSAND = 1000,
    INVALID_HASH_SIZE = -999
};

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

const double OVERFLOW_TABLE_SIZE_MODIFIER = 0.25;
const int ID_STARTING_POS = 2; // Characters from ID to be excluded
const int ID_PART1_LENGTH = 3;
const int ID_PART2_LENGTH = 4;
const int ID_TOTAL_LENGTH = ID_STARTING_POS + ID_PART1_LENGTH + ID_PART2_LENGTH;

const int ID_WIDTH = 14;
const int NAME_WIDTH = 15;
const int UNITS_WIDTH = 10;
const int LOCATION_WIDTH = 10;
const int UNITS_COL1 = 5;
const int UNITS_COL2 = UNITS_WIDTH - UNITS_COL1;

#endif