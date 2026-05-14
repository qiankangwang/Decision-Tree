import json
import sys
import pandas as pd
from sklearn.tree import DecisionTreeClassifier, _tree

CSV_PATH = "titanic.csv"

# Feature mapping for display
FEATURE_NAMES = {
    "pclass": "Passenger Class",
    "sex": "Sex",
    "age": "Age",
    "fare": "Fare",
}

EDGE_LABELS = {
    "sex": {True: "Female", False: "Male"},
}


def build_tree_json(tree, feature_names, node_id=0):
    """Recursively convert sklearn tree to JSON for D3 visualization."""
    tree_ = tree.tree_
    feature = tree_.feature[node_id]
    threshold = tree_.threshold[node_id]
    value = tree_.value[node_id][0]
    samples = int(tree_.n_node_samples[node_id])
    # sklearn stores proportions in value, multiply by n_samples to get counts
    survived = int(round(value[1] * samples))
    perished = int(round(value[0] * samples))

    if feature == _tree.TREE_UNDEFINED:
        # Leaf node
        pred = int(tree_.value[node_id].argmax())
        name = "Survived" if pred == 1 else "Perished"
        desc = f"{survived} survived, {perished} perished (n={samples})"
        return {
            "name": name,
            "type": "survived" if pred == 1 else "perished",
            "desc": desc,
            "samples": samples,
            "survived": survived,
            "perished": perished,
        }

    fname = feature_names[feature]
    display_name = FEATURE_NAMES.get(fname, fname)

    # For categorical-like splits (sex, pclass), show nicer labels
    # sklearn left child = feature <= threshold, right child = feature > threshold
    if fname == "sex":
        # sex: 0=male, 1=female; threshold ~0.5 → left=male, right=female
        question = "Sex?"
        left_label = "Male"
        right_label = "Female"
    elif fname == "pclass":
        question = f"Class <= {int(threshold)}?"
        left_label = f"Class <= {int(threshold)}"
        right_label = f"Class > {int(threshold)}"
    elif fname == "age":
        question = f"Age <= {threshold:.1f}?"
        left_label = f"Age <= {threshold:.1f}"
        right_label = f"Age > {threshold:.1f}"
    elif fname == "fare":
        question = f"Fare <= {threshold:.2f}?"
        left_label = f"Fare <= {threshold:.2f}"
        right_label = f"Fare > {threshold:.2f}"
    else:
        question = f"{display_name} <= {threshold:.2f}?"
        left_label = f"<= {threshold:.2f}"
        right_label = f"> {threshold:.2f}"

    left_child = tree_.children_left[node_id]
    right_child = tree_.children_right[node_id]

    node = {
        "name": question,
        "desc": f"Split on {display_name}. Samples: {samples}, Survived: {survived}, Perished: {perished}",
        "samples": samples,
        "survived": survived,
        "perished": perished,
        "children": [
            {
                **build_tree_json(tree, feature_names, left_child),
                "edgeLabel": left_label,
            },
            {
                **build_tree_json(tree, feature_names, right_child),
                "edgeLabel": right_label,
            },
        ],
    }
    return node


def main():
    # Load data (Kaggle Titanic format compatible)
    try:
        df = pd.read_csv(CSV_PATH)
    except FileNotFoundError:
        print(f"Error: {CSV_PATH} not found.")
        print("Download the Kaggle Titanic train.csv and save it as titanic.csv")
        sys.exit(1)

    # Select columns matching the project + fare for richer tree
    # The C++ project uses: survived, pclass, sex, age, sibsp, parch, fare, embarked
    # We keep pclass, sex, age, fare for the demo tree
    df = df[["survived", "pclass", "sex", "age", "fare"]].dropna()
    df["sex"] = df["sex"].map({"male": 0, "female": 1})

    X = df[["pclass", "sex", "age", "fare"]]
    y = df["survived"]

    # Train a decision tree with depth comparable to the C++ project
    clf = DecisionTreeClassifier(
        max_depth=5,
        min_samples_leaf=20,
        random_state=42,
    )
    clf.fit(X, y)

    # Export tree structure
    tree_json = build_tree_json(clf, ["pclass", "sex", "age", "fare"])
    tree_json["name"] = "Would this passenger\nsurvive the Titanic?"
    tree_json["type"] = "root"
    tree_json["edgeLabel"] = "Start"

    # Print tree stats
    accuracy = clf.score(X, y) * 100
    print(f"Tree trained on {len(df)} passengers")
    print(f"Training accuracy: {accuracy:.1f}%")
    print(f"Tree depth: {clf.get_depth()}")
    print(f"Number of leaves: {clf.get_n_leaves()}")

    # Save JSON
    with open("tree_data.json", "w", encoding="utf-8") as f:
        json.dump(tree_json, f, indent=2, ensure_ascii=True)

    print("Saved tree structure to tree_data.json")
    print("\n--- Tree Rules ---")
    from sklearn.tree import export_text
    print(export_text(clf, feature_names=["pclass", "sex", "age", "fare"]))


if __name__ == "__main__":
    main()
