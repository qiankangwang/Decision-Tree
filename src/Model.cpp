#include "DecisionTree.h"
#include <limits>

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

    // A 50/50 tie is classified as 0 (perished).
    return (countSurvived > countNotSurvived) ? 1 : 0;
}

void partitionPassengers(const std::vector<Passenger>& passengers,
                         const std::string& bestFeature,
                         double bestSplit,
                         std::vector<Passenger>& leftSubSet,
                         std::vector<Passenger>& rightSubSet) {
    for (const auto& passenger : passengers) {
        bool goLeft;
        if (bestFeature == "Sex") {
            goLeft = passenger.sex <= bestSplit;
        } else if (bestFeature == "Pclass") {
            goLeft = passenger.pclass == static_cast<int>(bestSplit);
        } else if (bestFeature == "Age") {
            goLeft = passenger.age <= bestSplit;
        } else {
            goLeft = false;
        }

        if (goLeft) {
            leftSubSet.push_back(passenger);
        } else {
            rightSubSet.push_back(passenger);
        }
    }
}

std::unique_ptr<TreeNode> buildDecisionTree(const std::vector<Passenger>& passengers,
                                            int depth,
                                            std::set<std::string> usedFeatures) {
    if (passengers.empty()) {
        return nullptr;
    }

    if (depth > MAX_DEPTH) {
        return std::make_unique<TreeNode>(findMostCommonCategory(passengers));
    }

    if (passengers.size() < static_cast<size_t>(MIN_SIZE) || isPure(passengers)) {
        int commonCategory = findMostCommonCategory(passengers);
        return std::make_unique<TreeNode>(commonCategory);
    }

    std::string bestFeature;
    double bestSplit = -1;
    double minGini = std::numeric_limits<double>::max();
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
        double giniPclass;
        double pclassSplit = findBestSplitForPclass(passengers, giniPclass);
        if (pclassSplit != -1.0 && giniPclass < minGini) {
            minGini = giniPclass;
            bestFeature = "Pclass";
            bestSplit = pclassSplit;
            improved = true;
        }
    }

    if (usedFeatures.find("Age") == usedFeatures.end()) {
        double splitAge = findBestSplit(passengers);
        if (splitAge != -1.0) {
            double giniAge = calculateGiniAtSplit(passengers, splitAge);
            if (giniAge < minGini) {
                minGini = giniAge;
                bestFeature = "Age";
                bestSplit = splitAge;
                improved = true;
            }
        }
    }

    // Information-gain stop: only split if the best candidate strictly reduces impurity.
    double parentGini = nodeGini(passengers);
    if (!improved || minGini >= parentGini - 1e-12) {
        return std::make_unique<TreeNode>(findMostCommonCategory(passengers));
    }

    usedFeatures.insert(bestFeature);

    std::vector<Passenger> leftSubSet, rightSubSet;
    partitionPassengers(passengers, bestFeature, bestSplit, leftSubSet, rightSubSet);

    // Degenerate split (everything went one way): make a majority leaf so no child is null.
    if (leftSubSet.empty() || rightSubSet.empty()) {
        return std::make_unique<TreeNode>(findMostCommonCategory(passengers));
    }

    auto node = std::make_unique<TreeNode>(bestFeature, bestSplit);
    node->leftChild = buildDecisionTree(leftSubSet, depth + 1, usedFeatures);
    node->rightChild = buildDecisionTree(rightSubSet, depth + 1, usedFeatures);

    return node;
}

void printTreePreorder(const TreeNode* node) {
    if (node == nullptr) return;
    if (node->isLeaf()) {
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
