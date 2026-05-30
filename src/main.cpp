#include "DecisionTree.h"
#include <cstring>
#include <random>
#include <algorithm>

int main(int argc, char* argv[]) {
    std::string filename = "processed_train_simple.txt";
    bool verbose = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else {
            filename = argv[i];
        }
    }

    std::vector<Passenger> passengers = readPassengers(filename);
    if (passengers.empty()) {
        return 1;
    }

    if (verbose) {
        printPassengers(passengers);
    }

    double bestSplit = findBestSplit(passengers);
    std::cout << std::endl << "Gini impurity for best split Age: " << bestSplit << std::endl;

    double giniImpurity = calculateGiniAtSplit(passengers, bestSplit);
    std::cout << "Gini impurity at age " << bestSplit << " is " << giniImpurity << std::endl;

    double giniSex = calculateGiniForSex(passengers);
    std::cout << "Gini impurity for sex: " << giniSex << std::endl;

    double giniPclass = calculateGiniForPclass(passengers);
    std::cout << "Gini impurity for passenger class: " << giniPclass << std::endl;

    double giniPclassSplit;
    double bestSplitPclass = findBestSplitForPclass(passengers, giniPclassSplit);
    std::cout << "Gini impurity for best split Pclass: " << bestSplitPclass << std::endl;

    // Deterministic train/test split (fixed seed for reproducibility): ~80% train, ~20% test.
    std::vector<Passenger> shuffled = passengers;
    std::mt19937 rng(42);
    std::shuffle(shuffled.begin(), shuffled.end(), rng);

    size_t testSize = shuffled.size() / 5;          // ~20%
    size_t trainSize = shuffled.size() - testSize;  // ~80%

    std::vector<Passenger> trainSet(shuffled.begin(), shuffled.begin() + trainSize);
    std::vector<Passenger> testSet(shuffled.begin() + trainSize, shuffled.end());

    std::set<std::string> usedFeatures;
    auto root = buildDecisionTree(trainSet, 0, usedFeatures);

    printTreePreorder(root.get());
    std::cout << std::endl;

    double trainAccuracy = calculateAccuracy(trainSet, root.get());
    double testAccuracy = testSet.empty() ? 0.0 : calculateAccuracy(testSet, root.get());
    std::cout << "Train accuracy: " << trainAccuracy << "%" << std::endl;
    std::cout << "Test accuracy: " << testAccuracy << "%" << std::endl;

    exportTreeToJson(root.get(), "tree_cpp.json");

    return 0;
}
