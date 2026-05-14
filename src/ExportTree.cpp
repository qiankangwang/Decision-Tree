#include "DecisionTree.h"
#include <fstream>
#include <iomanip>

static void exportNodeToJson(const TreeNode* node, std::ofstream& out, int indentLevel, const std::string& edgeLabel) {
    std::string indent(indentLevel * 2, ' ');
    out << indent << "{\n";

    if (!edgeLabel.empty()) {
        out << indent << "  \"edgeLabel\": \"" << edgeLabel << "\",\n";
    }

    std::string name;
    if (node->category != -1) {
        name = (node->category == 1) ? "Survived" : "Perished";
        out << indent << "  \"name\": \"" << name << "\",\n";
        out << indent << "  \"type\": \"" << (node->category == 1 ? "survived" : "perished") << "\"\n";
    } else {
        if (node->feature == "Sex") {
            name = "Sex = female?";
        } else if (node->feature == "Pclass") {
            name = "Class = " + std::to_string(static_cast<int>(node->splitValue)) + "?";
        } else if (node->feature == "Age") {
            name = "Age <= " + std::to_string(node->splitValue) + "?";
        } else {
            name = node->feature + " <= " + std::to_string(node->splitValue) + "?";
        }
        out << indent << "  \"name\": \"" << name << "\"";

        if (node->leftChild || node->rightChild) {
            out << ",\n" << indent << "  \"children\": [\n";

            std::string leftLabel, rightLabel;
            if (node->feature == "Sex") {
                leftLabel = "No (Male)";
                rightLabel = "Yes (Female)";
            } else if (node->feature == "Pclass") {
                leftLabel = "Class = " + std::to_string(static_cast<int>(node->splitValue));
                rightLabel = "Class != " + std::to_string(static_cast<int>(node->splitValue));
            } else if (node->feature == "Age") {
                leftLabel = "Age <= " + std::to_string(node->splitValue);
                rightLabel = "Age > " + std::to_string(node->splitValue);
            } else {
                leftLabel = "<= " + std::to_string(node->splitValue);
                rightLabel = "> " + std::to_string(node->splitValue);
            }

            if (node->leftChild) {
                exportNodeToJson(node->leftChild.get(), out, indentLevel + 2, leftLabel);
            }
            if (node->rightChild) {
                if (node->leftChild) {
                    out << ",\n";
                }
                exportNodeToJson(node->rightChild.get(), out, indentLevel + 2, rightLabel);
            }
            out << "\n" << indent << "  ]\n";
        } else {
            out << "\n";
        }
    }

    out << indent << "}";
}

void exportTreeToJson(const TreeNode* root, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Unable to open " << filename << std::endl;
        return;
    }

    out << "{\n";
    out << "  \"name\": \"Would this passenger\\nsurvive the Titanic?\",\n";
    out << "  \"type\": \"root\",\n";
    out << "  \"children\": [\n";
    exportNodeToJson(root, out, 2, "Start");
    out << "\n  ]\n";
    out << "}\n";

    out.close();
    std::cout << "Tree exported to " << filename << std::endl;
}
