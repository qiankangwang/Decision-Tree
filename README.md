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

**https://qiankangwang.github.io/Decision-Tree/**

The demo visualizes a decision tree trained on the Kaggle Titanic dataset (714 passengers with complete `pclass`/`sex`/`age`/`fare`, depth 5, 17 leaves, 82.9% training accuracy). You can click nodes to expand/collapse, pan and zoom, or use the **Predict Survival** panel to walk through the tree interactively.

> The web demo uses a scikit-learn tree because it splits on four features and produces a richer structure to explore. The from-scratch C++ implementation currently splits only on **Sex** and **Pclass**, so its tree is intentionally shallow — it is the learning exercise, not the showcase.

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

The `index.html` demo is fully self-contained — D3.js is loaded from a CDN and the tree structure is embedded directly as a JSON object, so it runs by simply opening the file in a browser.

To regenerate the embedded tree from the Kaggle dataset:

```bash
pip install pandas scikit-learn
python scripts/generate_tree.py   # writes tree_data.json
```

Then replace the `treeData` object in `index.html` with the contents of `tree_data.json`.

The C++ program also exports its own (shallower) tree to `tree_cpp.json` on each run, which can be embedded the same way if you want the demo to reflect the from-scratch implementation instead.

## How It Works

1. **Data Loading** — Reads passenger records (survival, class, sex, age, etc.) from a processed dataset.
2. **Splitting** — Evaluates Gini impurity for candidate features and selects the optimal split.
3. **Recursion** — Builds left and right subtrees until reaching a pure node, minimum size, or max depth.
4. **Prediction** — Traverses the tree based on feature values to classify survival outcome.
5. **Export** — Recursively serializes the tree to JSON for the web demo.

## Project Materials

- [Project Report & Slides (PDF)](./decision-tree-report.pdf)
