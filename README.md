# Decision Tree from Scratch

A from-scratch C++ implementation of a decision tree classifier, built to understand the core mechanics of tree-based machine learning algorithms.

## Overview

This project implements a decision tree using the **Gini impurity** criterion for splitting, trained on the classic **Titanic dataset**. It covers the full pipeline from data ingestion to recursive tree construction and prediction.

## Features

- Recursive tree construction with configurable max depth and minimum node size
- Gini impurity-based splitting for continuous (**Age**) and categorical (**Sex**, **Pclass**) features
- Preorder tree traversal for visualization
- Accuracy evaluation on training data

## Tech Stack

- C++ (standard library)

## File Structure

| File | Description |
|------|-------------|
| `DecisionTree.h` | Header with data structures and function declarations |
| `Model.cpp` | Core algorithm: tree building, splitting, prediction |
| `Gini.cpp` | Gini impurity calculations |
| `ReadPassenger.cpp` | Data loading and preprocessing |
| `main.cpp` | Entry point and evaluation |

## How It Works

1. **Data Loading** — Reads passenger records (survival, class, sex, age, etc.) from a processed dataset.
2. **Splitting** — Evaluates Gini impurity for candidate features and selects the optimal split.
3. **Recursion** — Builds left and right subtrees until reaching a pure node, minimum size, or max depth.
4. **Prediction** — Traverses the tree based on feature values to classify survival outcome.

## Project Materials

- [Project Report & Slides (PDF)](./Decison%20%20tree%20tree%20structure%20in%20machine%20lerning.pdf)
