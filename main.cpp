#include "DecisionTree.h"

int main() {
    string filename = "F:\processed_train_simple.txt";
    vector<Passenger> passengers = readPassengers(filename);

    printPassengers(passengers);

    double bestSplit = findBestSplit(passengers);
    cout << endl << "Gini impurity for Best split Age: " << bestSplit << endl;

    double giniImpurity = calculateGiniAtSplit(passengers, bestSplit);
    cout << "Gini Impurity at age " << bestSplit << " is " << giniImpurity << endl;

    // Calculate Gini impurity for sex split
    double giniSex = calculateGiniForSex(passengers);
    cout << "Gini impurity for sex: " << giniSex << endl;

    // Calculate Gini impurity for passenger class
    double giniPclass = calculateGiniForPclass(passengers);
    cout << "Gini impurity for passenger class: " << giniPclass << endl;

    double bestSplitPclass = findBestSplitForPclass(passengers);
        cout << "Gini impurity for Best split Age: " << bestSplitPclass << endl;

    set<string> usedFeatures;
    TreeNode* root = buildDecisionTree(passengers, 0, usedFeatures);
    printTreePreorder(root);
    cout << endl;
    double accuracy = calculateAccuracy(passengers, root);
    cout << "Accuracy of the decision tree: " << accuracy << "%" << endl;


    return 0;
}