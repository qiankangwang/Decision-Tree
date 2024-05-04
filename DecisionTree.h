#ifndef DECISIONTREE_H
#define DECISIONTREE_H
#include <iostream>
#include <vector>
#include <set>
using namespace std;

struct Passenger {
    int survived;
    int pclass;
    int sex;
    double age;
    int sibsp;
    int parch;
    double fare;
    int embarked;
};

struct TreeNode {
    string feature;
    double splitValue;
    int category;
    TreeNode* leftChild;
    TreeNode* rightChild;

    TreeNode(string f, double split, int cat = -1)
        : feature(f), splitValue(split), category(cat), leftChild(nullptr), rightChild(nullptr) {}

    TreeNode(int cat)
        : category(cat), leftChild(nullptr), rightChild(nullptr) {}
};

vector<Passenger> readPassengers(string& filename);
void printPassengers(vector<Passenger>& passengers);

void selectionSort(vector<Passenger>& passengers);
double findBestSplit(vector<Passenger>& passengers);
double calculateGiniAtSplit(vector<Passenger>& passengers, double splitAge);
double calculateGiniForSex(vector<Passenger>& passengers);
double calculateGiniForPclass(vector<Passenger>& passengers);
double findBestSplitForPclass(vector<Passenger>& passengers);


TreeNode* buildDecisionTree(vector<Passenger>& passengers, int depth, set<string>& usedFeatures);
void printTreePreorder(TreeNode* node);
double calculateAccuracy(vector<Passenger>& passengers, TreeNode* root);

#endif