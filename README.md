# Decision Tree from Scratch

A from-scratch C++ implementation of a decision tree classifier, built to understand the core mechanics of tree-based machine learning algorithms.

## Overview

This project implements a decision tree using the **Gini impurity** criterion for splitting, trained on the classic **Titanic dataset**. It covers the full pipeline from data ingestion to recursive tree construction and prediction.

## Features

- Recursive tree construction with configurable max depth and minimum node size
- Gini impurity-based splitting for continuous (**Age**) and categorical (**Sex**, **Pclass**) features
- Preorder tree traversal for visualization
- Accuracy evaluation on training data
- **Interactive web visualization** of the trained decision tree

## Tech Stack

- C++ (standard library)
- Python + scikit-learn (for visualization data generation)
- D3.js (for the interactive web demo)

## File Structure

| File | Description |
|------|-------------|
| `DecisionTree.h` | Header with data structures and function declarations |
| `Model.cpp` | Core algorithm: tree building, splitting, prediction |
| `Gini.cpp` | Gini impurity calculations |
| `ReadPassenger.cpp` | Data loading and preprocessing |
| `main.cpp` | Entry point and evaluation |
| `index.html` | Interactive D3.js visualization demo |
| `generate_tree.py` | Python script to train on Kaggle data and export tree JSON |

## Live Demo

An interactive visualization of the trained decision tree is automatically deployed via GitHub Actions:

**https://xiaole5211314.github.io/Decision-Tree/**

The demo lets you:
- Click nodes to expand / collapse the tree
- Pan and zoom to explore the full structure
- Use the **Predict Survival** panel to walk through the tree interactively
- See real sample counts at every node (trained on 714 passengers from the Kaggle Titanic dataset)

## How It Works

1. **Data Loading** — Reads passenger records (survival, class, sex, age, etc.) from a processed dataset.
2. **Splitting** — Evaluates Gini impurity for candidate features and selects the optimal split.
3. **Recursion** — Builds left and right subtrees until reaching a pure node, minimum size, or max depth.
4. **Prediction** — Traverses the tree based on feature values to classify survival outcome.

## Build & Run (C++)

```bash
g++ -std=c++17 main.cpp Model.cpp Gini.cpp ReadPassenger.cpp -o decision_tree
./decision_tree
```

The program trains on `processed_train_simple.txt` and prints the tree structure in preorder along with training accuracy.

## Visualization Setup

To regenerate the demo tree from the full Kaggle dataset:

1. Download `train.csv` from [Kaggle Titanic](https://www.kaggle.com/competitions/titanic/data)
2. Place it in the repo root and rename to `titanic.csv`
3. Run the Python script:

```bash
pip install pandas scikit-learn
python generate_tree.py
```

This trains a decision tree with `max_depth=5` and `min_samples_leaf=20`, then exports `tree_data.json` which is embedded into `index.html`.

## Project Materials

- [Project Report & Slides (PDF)](./Decison%20%20tree%20tree%20structure%20in%20machine%20lerning.pdf)
