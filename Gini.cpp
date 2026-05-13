#include "DecisionTree.h"

double calculateGiniForSex(std::vector<Passenger>& passengers) {
    double countFemale = 0, countMale = 0;
    double survivedFemale = 0, survivedMale = 0;

    for (const auto& p : passengers) {
        if (p.sex == 1) {
            countFemale++;
            if (p.survived)
                survivedFemale++;
        } else {
            countMale++;
            if (p.survived)
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

double calculateGiniForPclass(std::vector<Passenger>& passengers) {
    double count[3] = { 0, 0, 0 };
    double survivedCount[3] = { 0, 0, 0 };

    for (const auto& p : passengers) {
        if (p.pclass >= 1 && p.pclass <= 3) {
            int index = p.pclass - 1;
            count[index]++;
            if (p.survived) {
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

double findBestSplitForPclass(std::vector<Passenger>& passengers) {
    int counts[3] = { 0, 0, 0 };
    int survivedCounts[3] = { 0, 0, 0 };

    for (const auto& p : passengers) {
        if (p.pclass >= 1 && p.pclass <= 3) {
            int index = p.pclass - 1;
            counts[index]++;
            if (p.survived) {
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

void selectionSort(std::vector<Passenger>& passengers) {
    for (size_t i = 0; i < passengers.size() - 1; i++) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < passengers.size(); j++) {
            if (passengers[j].age < passengers[minIndex].age) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(passengers[i], passengers[minIndex]);
        }
    }
}

double calculateGini(int counts[2], double total) {
    if (total == 0) {
        return 0;
    }

    double sum = 0;
    for (int i = 0; i < 2; ++i) {
        double probability = counts[i] / total;
        sum += probability * probability;
    }

    return 1 - sum;
}

double findBestSplit(std::vector<Passenger>& passengers) {
    std::vector<Passenger> sortedPassengers = passengers;
    selectionSort(sortedPassengers);

    double bestGini = 1.0;
    double bestSplit = -1.0;
    size_t total = sortedPassengers.size();

    for (size_t i = 1; i < total; i++) {
        if (sortedPassengers[i].age != sortedPassengers[i - 1].age) {
            double currentSplit = (sortedPassengers[i].age + sortedPassengers[i - 1].age) / 2.0;

            int leftCounts[2] = { 0, 0 };
            int rightCounts[2] = { 0, 0 };

            for (size_t j = 0; j < i; j++) {
                leftCounts[sortedPassengers[j].survived]++;
            }
            for (size_t j = i; j < total; j++) {
                rightCounts[sortedPassengers[j].survived]++;
            }

            double leftGini = calculateGini(leftCounts, static_cast<double>(i));
            double rightGini = calculateGini(rightCounts, static_cast<double>(total - i));

            double weightedGini = (static_cast<double>(i) * leftGini + static_cast<double>(total - i) * rightGini) / static_cast<double>(total);
            if (weightedGini < bestGini) {
                bestGini = weightedGini;
                bestSplit = currentSplit;
            }
        }
    }

    return bestSplit;
}

double calculateGiniAtSplit(std::vector<Passenger>& passengers, double splitAge) {
    int leftCounts[2] = { 0, 0 };
    int rightCounts[2] = { 0, 0 };

    int leftTotal = 0, rightTotal = 0;

    for (const auto& p : passengers) {
        if (p.age <= splitAge) {
            leftCounts[p.survived]++;
            leftTotal++;
        } else {
            rightCounts[p.survived]++;
            rightTotal++;
        }
    }

    double leftGini = calculateGini(leftCounts, leftTotal);
    double rightGini = calculateGini(rightCounts, rightTotal);

    double total = leftTotal + rightTotal;
    double totalGini = (leftTotal * leftGini + rightTotal * rightGini) / total;

    return totalGini;
}
