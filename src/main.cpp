#include "DecisionTree.h"
#include <cstring>

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

    double bestSplitPclass = findBestSplitForPclass(passengers);
    std::cout << "Gini impurity for best split Pclass: " << bestSplitPclass << std::endl;

    std::set<std::string> usedFeatures;
    auto root = buildDecisionTree(passengers, 0, usedFeatures);

    printTreePreorder(root.get());
    std::cout << std::endl;

    double accuracy = calculateAccuracy(passengers, root.get());
    std::cout << "Accuracy of the decision tree: " << accuracy << "%" << std::endl;

    exportTreeToJson(root.get(), "tree_cpp.json");

    return 0;
}
