#include "DecisionTree.h"
#include <fstream>
#include <sstream>

std::vector<Passenger> readPassengers(const std::string& filename) {
    std::vector<Passenger> passengers;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        std::cerr << "Usage: ./decision_tree <data_file.txt>" << std::endl;
        return passengers;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Passenger passenger;
        iss >> passenger.survived >> passenger.pclass >> passenger.sex
            >> passenger.age >> passenger.sibsp >> passenger.parch
            >> passenger.fare >> passenger.embarked;

        if (iss.fail()) {
            std::cerr << "Warning: Failed to parse line: " << line << std::endl;
            continue;
        }

        passengers.push_back(passenger);
    }

    file.close();
    std::cout << "Loaded " << passengers.size() << " passengers from " << filename << std::endl;

    return passengers;
}

void printPassengers(const std::vector<Passenger>& passengers) {
    for (const auto& p : passengers) {
        std::cout << "Survived: " << p.survived
                  << ", Class: " << p.pclass
                  << ", Sex: " << p.sex
                  << ", Age: " << p.age
                  << ", Siblings/Spouses: " << p.sibsp
                  << ", Parents/Children: " << p.parch
                  << ", Fare: " << p.fare
                  << ", Embarked: " << p.embarked << std::endl;
    }
}
