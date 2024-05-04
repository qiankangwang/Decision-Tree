#include "DecisionTree.h"

double calculateGiniForSex(vector<Passenger>& passengers) {
    double countFemale = 0, countMale = 0;
    double survivedFemale = 0, survivedMale = 0;

    for (int i = 0; i < passengers.size(); ++i) {
        if (passengers[i].sex == 1) {  // Assuming 1 represents females
            countFemale++;
            if (passengers[i].survived) 
                survivedFemale++;
        }
        else {
            countMale++;
            if (passengers[i].survived) 
                survivedMale++;
        }
    }

    double pSurvivedFemale = 0, pSurvivedMale = 0;

    if (countFemale > 0) {
        pSurvivedFemale = survivedFemale / countFemale;
    }
    if (countMale > 0) {
        pSurvivedMale = survivedMale / countMale;
    }

    double giniFemale = 1 - (pSurvivedFemale * pSurvivedFemale) - (1 - pSurvivedFemale) * (1 - pSurvivedFemale);
    double giniMale = 1 - (pSurvivedMale * pSurvivedMale) - (1 - pSurvivedMale) * (1 - pSurvivedMale);

    double total = countFemale + countMale;
    double totalGini = 0.0;

    if (total > 0) { 
        totalGini = countFemale / total * giniFemale + countMale / total * giniMale;
    }

    return totalGini;
}

double calculateGiniForPclass(vector<Passenger>& passengers) {
    double count[3] = { 0, 0, 0 }; // Array to count the number of passengers in each category
    double survivedCount[3] = { 0, 0, 0 }; // Number of survivors in each category

    for (int i = 0; i < passengers.size(); ++i) {
        if (passengers[i].pclass >= 1 && passengers[i].pclass <= 3) {
            int index = passengers[i].pclass - 1;
            count[index]++;
            if (passengers[i].survived) {
                survivedCount[index]++;
            }
        }
    }

    double total = passengers.size();
    double totalGini = 0.0;

    for (int i = 0; i < 3; i++) {
        if (count[i] > 0) {
            double pSurvived = survivedCount[i] / count[i];
            double gini = 1 - (pSurvived * pSurvived) - ((1 - pSurvived) * (1 - pSurvived));
            totalGini += (count[i] / total) * gini;
        }
    }

    return totalGini;
}

double findBestSplitForPclass(vector<Passenger>& passengers) {
    int counts[3] = { 0, 0, 0 };
    int survivedCounts[3] = { 0, 0, 0 }; 

    for (int i = 0; i < passengers.size(); ++i) {
        if (passengers[i].pclass >= 1 && passengers[i].pclass <= 3) {
            int index = passengers[i].pclass - 1;
            counts[index]++;
            if (passengers[i].survived) {
                survivedCounts[index]++;
            }
        }
    }

    double bestGini = 1.0;
    double bestSplit = -1;
    double total = passengers.size();

    for (int i = 0; i < 3; i++) {
        if (counts[i] > 0) {
            double pSurvived = survivedCounts[i] / counts[i];
            double gini = 1 - (pSurvived * pSurvived) - (1 - pSurvived) * (1 - pSurvived);
            double weightedGini = (counts[i] / total) * gini;
            if (weightedGini < bestGini) {
                bestGini = weightedGini;
                bestSplit = i + 1;
            }
        }
    }

    return bestSplit;
}

void selectionSort(vector<Passenger>& passengers) {
    for (int i = 0; i < passengers.size() - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < passengers.size(); j++) {
            if (passengers[j].age < passengers[minIndex].age) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            Passenger temp = passengers[i];
            passengers[i] = passengers[minIndex];
            passengers[minIndex] = temp;
        }
    }
}

double calculateGini(int counts[2], double total) {
    if (total == 0) {
        return 0;
    }

    double sum = 0;

    // We are assuming counts is an array with two elements
    for (int i = 0; i < 2; ++i) {  // Since we know there are only two categories
        double probability = counts[i] / total;
        sum += probability * probability;
    }

    return 1 - sum;
}


double findBestSplit(vector<Passenger>& passengers) {
    vector<Passenger> sortedPassengers = passengers;
    selectionSort(sortedPassengers);

    double bestGini = 1.0;
    double bestSplit = -1.0;
    int total = sortedPassengers.size();

    for (int i = 1; i < total; i++) {
        if (sortedPassengers[i].age != sortedPassengers[i - 1].age) {
            double currentSplit = (sortedPassengers[i].age + sortedPassengers[i - 1].age) / 2.0;

            int leftCounts[2] = { 0, 0 };  // [0] for not survived, [1] for survived
            int rightCounts[2] = { 0, 0 }; // [0] for not survived, [1] for survived

            for (int j = 0; j < i; j++) {
                leftCounts[sortedPassengers[j].survived]++;
            }
            for (int j = i; j < total; j++) {
                rightCounts[sortedPassengers[j].survived]++;
            }

            double leftGini = calculateGini(leftCounts, i);
            double rightGini = calculateGini(rightCounts, total - i);

            double weightedGini = (i * leftGini + (total - i) * rightGini) / total;
            if (weightedGini < bestGini) {
                bestGini = weightedGini;
                bestSplit = currentSplit;
            }
        }
    }

    return bestSplit;
}

double calculateGiniAtSplit(vector<Passenger>& passengers, double splitAge) {
    int leftCounts[2] = { 0, 0 };
    int rightCounts[2] = { 0, 0 };

    int leftTotal = 0, rightTotal = 0;

    for (int i = 0; i < passengers.size(); i++) {
        if (passengers[i].age <= splitAge) {
            leftCounts[passengers[i].survived]++;
            leftTotal++;
        }
        else {
            rightCounts[passengers[i].survived]++;
            rightTotal++;
        }
    }

    // Calculate Gini for each side
    double leftGini = calculateGini(leftCounts, leftTotal);
    double rightGini = calculateGini(rightCounts, rightTotal);

    // Calculate weighted Gini
    double total = leftTotal + rightTotal;
    double totalGini = (leftTotal * leftGini + rightTotal * rightGini) / total;

    return totalGini;
}