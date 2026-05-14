# Decision Tree from Scratch

A from-scratch C++ implementation of a decision tree classifier, built to understand the core mechanics of tree-based machine learning algorithms.

## Overview

This project implements a decision tree using the **Gini impurity** criterion for splitting, trained on the classic **Titanic dataset**. It covers the full pipeline from data ingestion to recursive tree construction and prediction.

## Features

- Recursive tree construction with configurable max depth and minimum node size
- Gini impurity-based splitting for continuous (**Age**) and categorical (**Sex**, **Pclass**) features
- Preorder tree traversal for visualization
- Accuracy evaluation on training data
- **JSON export** of the trained tree for external visualization
- **Interactive web visualization** of the trained decision tree

## Tech Stack

- C++ (standard library)
- Python + scikit-learn (for data conversion and reference training)
- D3.js (for the interactive web demo)

## Project Structure

```
.
├── include/
│   └── DecisionTree.h          # Data structures and function declarations
├── src/
│   ├── main.cpp                # Entry point and evaluation
│   ├── Model.cpp               # Core algorithm: tree building, splitting, prediction
│   ├── Gini.cpp                # Gini impurity calculations
│   ├── ReadPassenger.cpp       # Data loading and preprocessing
│   └── ExportTree.cpp          # Export trained tree to JSON for the demo
├── scripts/
│   ├── convert_kaggle.py       # Convert Kaggle CSV to C++ input format
│   └── generate_tree.py        # Train with sklearn and export tree JSON
├── index.html                  # Interactive D3.js visualization demo
├── Makefile
└── README.md
```

## Live Demo

An interactive visualization is automatically deployed via GitHub Actions:

**https://xiaole5211314.github.io/Decision-Tree/**

The demo shows the **actual tree trained by the C++ algorithm** on the Kaggle Titanic dataset. You can click nodes to expand/collapse, pan and zoom, or use the **Predict Survival** panel to walk through the tree interactively.

## Build & Run (C++)

```bash
make
./decision_tree processed_train.txt
```

The program trains on the provided dataset, prints the tree in preorder, reports accuracy, and exports the tree structure to `tree_cpp.json`.

### Training on the Full Kaggle Dataset

1. Download `train.csv` from [Kaggle Titanic](https://www.kaggle.com/competitions/titanic/data)
2. Convert it to the format expected by the C++ program:

```bash
python scripts/convert_kaggle.py train.csv processed_train.txt
```

3. Build and run:

```bash
make
./decision_tree processed_train.txt
```

The C++ tree (trained on 712 passengers) achieves **77.9%** accuracy on the training set.

## Visualization Setup

The `index.html` demo is self-contained and reads the tree structure directly from an embedded JSON object. To regenerate the JSON from the C++ tree after training:

```bash
# After running ./decision_tree, tree_cpp.json is produced automatically.
# Then update the demo by replacing the treeData object in index.html.
```

To train a reference tree with sklearn (for comparison):

```bash
pip install pandas scikit-learn
python scripts/generate_tree.py
```

## How It Works

1. **Data Loading** — Reads passenger records (survival, class, sex, age, etc.) from a processed dataset.
2. **Splitting** — Evaluates Gini impurity for candidate features and selects the optimal split.
3. **Recursion** — Builds left and right subtrees until reaching a pure node, minimum size, or max depth.
4. **Prediction** — Traverses the tree based on feature values to classify survival outcome.
5. **Export** — Recursively serializes the tree to JSON for the web demo.

## Project Materials

- [Project Report & Slides (PDF)](./Decison%20%20tree%20tree%20structure%20in%20machine%20lerning.pdf)
