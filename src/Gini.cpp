#include "DecisionTree.h"
#include <algorithm>

double calculateGiniForSex(const std::vector<Passenger>& passengers) {
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

double calculateGiniForPclass(const std::vector<Passenger>& passengers) {
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

double findBestSplitForPclass(const std::vector<Passenger>& passengers) {
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
            double pSurvived = static_cast<double>(survivedCounts[i]) / counts[i];
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

static double calculateGini(const int counts[2], double total) {
    if (total == 0) {
        return 0;
    }
    double p0 = counts[0] / total;
    double p1 = counts[1] / total;
    return 1 - (p0 * p0 + p1 * p1);
}

double findBestSplit(const std::vector<Passenger>& passengers) {
    std::vector<Passenger> sorted = passengers;
    std::sort(sorted.begin(), sorted.end(),
              [](const Passenger& a, const Passenger& b) { return a.age < b.age; });

    const size_t total = sorted.size();
    if (total < 2) return -1.0;

    int totalCounts[2] = { 0, 0 };
    for (const auto& p : sorted) totalCounts[p.survived]++;

    double bestGini = 1.0;
    double bestSplit = -1.0;
    int leftCounts[2] = { 0, 0 };

    for (size_t i = 1; i < total; i++) {
        leftCounts[sorted[i - 1].survived]++;
        if (sorted[i].age == sorted[i - 1].age) continue;

        int rightCounts[2] = {
            totalCounts[0] - leftCounts[0],
            totalCounts[1] - leftCounts[1],
        };
        double currentSplit = (sorted[i].age + sorted[i - 1].age) / 2.0;
        double leftGini = calculateGini(leftCounts, static_cast<double>(i));
        double rightGini = calculateGini(rightCounts, static_cast<double>(total - i));
        double weightedGini = (i * leftGini + (total - i) * rightGini) / total;
        if (weightedGini < bestGini) {
            bestGini = weightedGini;
            bestSplit = currentSplit;
        }
    }

    return bestSplit;
}

double calculateGiniAtSplit(const std::vector<Passenger>& passengers, double splitAge) {
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
    if (total == 0) return 0.0;
    double totalGini = (leftTotal * leftGini + rightTotal * rightGini) / total;

    return totalGini;
}
