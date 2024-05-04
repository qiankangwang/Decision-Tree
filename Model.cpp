#include "DecisionTree.h"
const int MIN_SIZE = 2;
const int MAX_DEPTH = 5;

bool isPure(vector<Passenger>& passengers) {
    if (passengers.empty()) {
        return true;
    }

    int firstSurvivalStatus = passengers[0].survived;
    for (int i = 0; i < passengers.size(); ++i) {
        if (passengers[i].survived != firstSurvivalStatus) {
            return false;
        }
    }
    return true;
}

int findMostCommonCategory(vector<Passenger>& passengers) {
    if (passengers.empty()) {
        return -1;
    }

    int countSurvived = 0;
    int countNotSurvived = 0;

    for (int i = 0; i < passengers.size(); i++) {
        if (passengers[i].survived) {
            countSurvived++;
        }
        else {
            countNotSurvived++;
        }
    }

    if (countSurvived > countNotSurvived) {
        return 1;
    }
    else {
        return 0;
    }
}

void partitionPassengers(vector<Passenger>& passengers, string& bestFeature, double bestSplit, vector<Passenger>& leftSubSet, vector<Passenger>& rightSubSet) {
    for (int i = 0; i < passengers.size(); ++i) {
        Passenger& passenger = passengers[i];
        if (bestFeature == "Sex" && passenger.sex <= bestSplit) {
            leftSubSet.push_back(passenger);
        }
        else if (bestFeature == "Pclass" && passenger.pclass == bestSplit) {
            leftSubSet.push_back(passenger);
        }
        else {
            rightSubSet.push_back(passenger);
        }
    }
}


TreeNode* buildDecisionTree(vector<Passenger>& passengers, int depth, set<string>& usedFeatures) {
    if (passengers.empty() || depth > MAX_DEPTH) {
        return nullptr;
    }

    if (passengers.size() < MIN_SIZE || isPure(passengers)) {
        int commonCategory = findMostCommonCategory(passengers);
        return new TreeNode(commonCategory);
    }

    string bestFeature;
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
        return new TreeNode(commonCategory);
    }

    usedFeatures.insert(bestFeature);

    vector<Passenger> leftSubSet, rightSubSet;
    partitionPassengers(passengers, bestFeature, bestSplit, leftSubSet, rightSubSet);

    TreeNode* node = new TreeNode(bestFeature, bestSplit);
    node->leftChild = buildDecisionTree(leftSubSet, depth + 1, usedFeatures);
    node->rightChild = buildDecisionTree(rightSubSet, depth + 1, usedFeatures);

    return node;
}


void printTreePreorder(TreeNode* node) {
    if (node != nullptr) {
        if (node->category != -1) {
            cout << "Category: " << node->category << " ";
        }
        else {
            cout << node->feature << ": " << node->splitValue << " ";
        }
        printTreePreorder(node->leftChild);
        printTreePreorder(node->rightChild);
    }
}

int predict(TreeNode* node, Passenger& passenger) {
    // Base case: If the node has no children, return its category.
    if (node->leftChild == nullptr && node->rightChild == nullptr) {
        return node->category;
    }

    // Recursive case: Check the feature and decide the next node based on the feature's value.
    if (node->feature == "Sex") {
        if (passenger.sex <= node->splitValue) {
            return predict(node->leftChild, passenger);
        }
        else {
            return predict(node->rightChild, passenger);
        }
    }
    else if (node->feature == "Pclass") {
        if (passenger.pclass == int(node->splitValue)) {
            return predict(node->leftChild, passenger);
        }
        else {
            return predict(node->rightChild, passenger);
        }
    }
    else if (node->feature == "Age") {
        if (passenger.age <= node->splitValue) {
            return predict(node->leftChild, passenger);
        }
        else {
            return predict(node->rightChild, passenger);
        }
    }
    else {
        return -1;  // Return an invalid category or handle error appropriately
    }
}



double calculateAccuracy(vector<Passenger>& passengers, TreeNode* root) {
    double correctPredictions = 0;
    for (int i = 0; i < passengers.size(); ++i) {
        int predicted = predict(root, passengers[i]);
        if (predicted == passengers[i].survived) {
            correctPredictions++;
        }
    }
    double Accuracy = correctPredictions / passengers.size() * 100.0;

    return Accuracy;  // Convert to percentage
}