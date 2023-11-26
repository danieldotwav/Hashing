#ifndef STUDENTRECORD_H
#define STUDENTRECORD_H

#include <iostream>
#include <sstream>

class StudentRecord {
private:
    std::string id;
    std::string firstName;
    std::string lastName;
    int units;

public:
    StudentRecord(std::string id, std::string firstName, std::string lastName, int units)
        : id(id), firstName(firstName), lastName(lastName), units(units) {}

    std::string getId() const { return id; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    int getUnits() const { return units; }

    std::string toString() const {
        std::stringstream ss;
        ss << "ID: " << id << ", Name: " << firstName << " " << lastName << ", Units: " << units;
        return ss.str();
    }
};

#endif // STUDENTRECORD_H