#include "DecisionTree.h"
#include <fstream>
#include <sstream>

vector<Passenger> readPassengers(string& filename) {
    vector<Passenger> passengers;
    ifstream file(filename);
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::istringstream iss(line);
            Passenger passenger;
            iss >> passenger.survived >> passenger.pclass >> passenger.sex
                >> passenger.age >> passenger.sibsp >> passenger.parch
                >> passenger.fare >> passenger.embarked;
            passengers.push_back(passenger);
        }
        file.close();
    }
    else {
        cerr << "Unable to open file: " << filename << endl;
    }
    return passengers;
}

void printPassengers(vector<Passenger>& passengers) {
    for (int i = 0; i < passengers.size(); ++i) {
        cout << "Survived: " << passengers[i].survived
            << ", Class: " << passengers[i].pclass
            << ", Sex: " << passengers[i].sex
            << ", Age: " << passengers[i].age
            << ", Siblings/Spouses: " << passengers[i].sibsp
            << ", Parents/Children: " << passengers[i].parch
            << ", Fare: " << passengers[i].fare
            << ", Embarked: " << passengers[i].embarked << endl;
    }
}