#pragma once

#include <string>

class Player {
private:
    std::string name = "Mr. Banker";
    std::size_t maximumCharNumber = 10;
    std::size_t minimumCharNumber = 1;

public:
    // Getter function
    const std::string& getName() const { return name; }

    // Setter function
    bool setName(const std::string& newName) {
        if (!isNameValid(newName)) {
            return false;
        }

        name = newName;
        return true;
    }

    // Validator function
    bool isNameValid(const std::string& newName) const {
        return newName.size() >= minimumCharNumber && newName.size() <= maximumCharNumber;
    }
};