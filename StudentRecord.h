#ifndef STUDENTRECORD_H
#define STUDENTRECORD_H

#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <sstream>

class StudentRecord {
private:
    std::string id;
    std::string firstName;
    std::string lastName;
    int units;

    bool isDeleted;
    std::string recordLocation;

public:
    StudentRecord() : id(""), firstName(""), lastName(""), units(0), isDeleted(false), recordLocation("") {}

    StudentRecord(std::string id, std::string lastName, std::string firstName, int units)
        : id(id), firstName(firstName), lastName(lastName), units(units), isDeleted(false) {}

    std::string getID() const { return id; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    int getUnits() const { return units; }
    bool getDeletedStatus() const { return isDeleted; }
    std::string getRecordLocation() const { return recordLocation; }

    void setID(std::string n_id) { id = n_id; }
    void setFirstName(std::string n_firstName) { firstName = n_firstName; }
    void setLastName(std::string n_lastName) { lastName = n_lastName; }
    void setUnits(int n_units) { units = n_units; }
    void setDeletedStatus(bool n_isDeleted) { isDeleted = n_isDeleted; }
    void setRecordLocation(std::string n_location) { recordLocation = n_location; }

    std::string toString() const {
        std::stringstream ss;
        ss << std::left << std::setw(ID_WIDTH) << getID() << std::setw(NAME_WIDTH) 
            << getLastName() << std::setw(NAME_WIDTH) << getFirstName() << std::right << std::setw(UNITS_COL1) << getUnits() << std::setw(UNITS_COL2) << "" << std::left << std::setw(LOCATION_WIDTH) << getRecordLocation() << std::endl;
        return ss.str();
    }
};

#endif // STUDENTRECORD_H