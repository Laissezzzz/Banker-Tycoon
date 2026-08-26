#pragma once

#include <string>

class Client {
private:
    int id = 0;
    std::string name            = "Blank";
    std::string surname         = "Blank";
    std::string birthDate       = "1900-01-01";
    std::string maritalStatus   = "single";
    int childrenCount           = 0;

    int invalidID               = 0;
public:
    // Constructors
    Client() = default;
    Client(int ID, const std::string& n, const std::string& s, const std::string& bd, const std::string& ms, int cc)
        : id(ID), name(n), surname(s), birthDate(bd), maritalStatus(ms), childrenCount(cc) {}

    // Getter functions
    bool isValid() const {return id != invalidID;}
    int getId() const {return id;}
    const std::string& getName() const {return name;}
    const std::string& getSurname() const {return surname;}
    const std::string& getBirthDate() const {return birthDate;}
    const std::string& getMaritalStatus() const {return maritalStatus;}
    int getChildrenCount() const {return childrenCount;}
};