#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <memory>

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
    std::string feature;
    double splitValue;
    int category;
    std::unique_ptr<TreeNode> leftChild;
    std::unique_ptr<TreeNode> rightChild;

    TreeNode(std::string f, double split, int cat = -1)
        : feature(std::move(f)), splitValue(split), category(cat) {}

    explicit TreeNode(int cat)
        : category(cat) {}
};

std::vector<Passenger> readPassengers(const std::string& filename);
void printPassengers(const std::vector<Passenger>& passengers);

double findBestSplit(const std::vector<Passenger>& passengers);
double calculateGiniAtSplit(const std::vector<Passenger>& passengers, double splitAge);
double calculateGiniForSex(const std::vector<Passenger>& passengers);
double calculateGiniForPclass(const std::vector<Passenger>& passengers);
double findBestSplitForPclass(const std::vector<Passenger>& passengers);

std::unique_ptr<TreeNode> buildDecisionTree(const std::vector<Passenger>& passengers, int depth, std::set<std::string>& usedFeatures);
void printTreePreorder(const TreeNode* node);
double calculateAccuracy(const std::vector<Passenger>& passengers, const TreeNode* root);
void exportTreeToJson(const TreeNode* root, const std::string& filename);

#endif
