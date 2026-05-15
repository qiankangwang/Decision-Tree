#include "DecisionTree.h"

const int MIN_SIZE = 2;
const int MAX_DEPTH = 5;

bool isPure(const std::vector<Passenger>& passengers) {
    if (passengers.empty()) {
        return true;
    }

    int firstSurvivalStatus = passengers[0].survived;
    for (size_t i = 1; i < passengers.size(); ++i) {
        if (passengers[i].survived != firstSurvivalStatus) {
            return false;
        }
    }
    return true;
}

int findMostCommonCategory(const std::vector<Passenger>& passengers) {
    if (passengers.empty()) {
        return -1;
    }

    int countSurvived = 0;
    int countNotSurvived = 0;

    for (const auto& p : passengers) {
        if (p.survived) {
            countSurvived++;
        } else {
            countNotSurvived++;
        }
    }

    return (countSurvived > countNotSurvived) ? 1 : 0;
}

void partitionPassengers(const std::vector<Passenger>& passengers,
                         const std::string& bestFeature,
                         double bestSplit,
                         std::vector<Passenger>& leftSubSet,
                         std::vector<Passenger>& rightSubSet) {
    for (const auto& passenger : passengers) {
        if (bestFeature == "Sex" && passenger.sex <= bestSplit) {
            leftSubSet.push_back(passenger);
        } else if (bestFeature == "Pclass" && passenger.pclass == bestSplit) {
            leftSubSet.push_back(passenger);
        } else {
            rightSubSet.push_back(passenger);
        }
    }
}

std::unique_ptr<TreeNode> buildDecisionTree(const std::vector<Passenger>& passengers,
                                            int depth,
                                            std::set<std::string>& usedFeatures) {
    if (passengers.empty() || depth > MAX_DEPTH) {
        return nullptr;
    }

    if (passengers.size() < static_cast<size_t>(MIN_SIZE) || isPure(passengers)) {
        int commonCategory = findMostCommonCategory(passengers);
        return std::make_unique<TreeNode>(commonCategory);
    }

    std::string bestFeature;
    double bestSplit = -1;
    double minGini = 999999999.99;
    bool improved = false;

    if (usedFeatures.find("Sex") == usedFeatures.end()) {
        double giniSex = calculateGiniForSex(passengers);
        if (giniSex < minGini) {
            minGini = giniSex;
            bestFeature = "Sex";
            bestSplit = 0.5;
            improved = true;
        }
    }

    if (usedFeatures.find("Pclass") == usedFeatures.end()) {
        double giniPclass = calculateGiniForPclass(passengers);
        if (giniPclass < minGini) {
            minGini = giniPclass;
            bestFeature = "Pclass";
            bestSplit = findBestSplitForPclass(passengers);
            improved = true;
        }
    }

    if (!improved) {
        int commonCategory = findMostCommonCategory(passengers);
        return std::make_unique<TreeNode>(commonCategory);
    }

    usedFeatures.insert(bestFeature);

    std::vector<Passenger> leftSubSet, rightSubSet;
    partitionPassengers(passengers, bestFeature, bestSplit, leftSubSet, rightSubSet);

    auto node = std::make_unique<TreeNode>(bestFeature, bestSplit);
    node->leftChild = buildDecisionTree(leftSubSet, depth + 1, usedFeatures);
    node->rightChild = buildDecisionTree(rightSubSet, depth + 1, usedFeatures);

    return node;
}

void printTreePreorder(const TreeNode* node) {
    if (node == nullptr) return;
    if (node->category != -1) {
        std::cout << "Category: " << node->category << " ";
    } else {
        std::cout << node->feature << ": " << node->splitValue << " ";
    }
    printTreePreorder(node->leftChild.get());
    printTreePreorder(node->rightChild.get());
}

int predict(const TreeNode* node, const Passenger& passenger) {
    if (node == nullptr) {
        return -1;
    }
    if (!node->leftChild && !node->rightChild) {
        return node->category;
    }

    if (node->feature == "Sex") {
        if (passenger.sex <= node->splitValue) {
            return predict(node->leftChild.get(), passenger);
        } else {
            return predict(node->rightChild.get(), passenger);
        }
    } else if (node->feature == "Pclass") {
        if (passenger.pclass == static_cast<int>(node->splitValue)) {
            return predict(node->leftChild.get(), passenger);
        } else {
            return predict(node->rightChild.get(), passenger);
        }
    } else if (node->feature == "Age") {
        if (passenger.age <= node->splitValue) {
            return predict(node->leftChild.get(), passenger);
        } else {
            return predict(node->rightChild.get(), passenger);
        }
    } else {
        return -1;
    }
}

double calculateAccuracy(const std::vector<Passenger>& passengers, const TreeNode* root) {
    if (root == nullptr || passengers.empty()) {
        return 0.0;
    }

    double correctPredictions = 0;
    for (const auto& passenger : passengers) {
        int predicted = predict(root, passenger);
        if (predicted == passenger.survived) {
            correctPredictions++;
        }
    }

    return (correctPredictions / passengers.size()) * 100.0;
}
