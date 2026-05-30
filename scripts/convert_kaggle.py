import pandas as pd
import sys


def convert_kaggle_to_simple(input_csv: str, output_txt: str):
    """Convert Kaggle Titanic train.csv to the format expected by the C++ project."""
    df = pd.read_csv(input_csv)

    # Normalize column names (Kaggle uses Title Case, some sources use lowercase)
    df.columns = [c.lower() for c in df.columns]

    # Select and rename columns to match C++ struct order:
    # survived, pclass, sex, age, sibsp, parch, fare, embarked
    df = df[["survived", "pclass", "sex", "age", "sibsp", "parch", "fare", "embarked"]].copy()

    # Encode categorical fields BEFORE dropping NaNs, so any value that is not in
    # the mapping (or is already missing) becomes NaN and gets dropped below.
    # Lowercase both sex and embarked first to be robust to casing.
    df["sex"] = df["sex"].str.lower().map({"male": 0, "female": 1})
    df["embarked"] = df["embarked"].str.lower().map({"s": 0, "c": 1, "q": 2})

    # Drop rows with missing values (including unmapped categoricals)
    df = df.dropna()

    # Integer columns must be written without a trailing '.0' — pandas promotes
    # int columns to float64 once any NaN is present, and the C++ reader parses
    # these with `iss >> int`, which fails on '1.0'. Cast them back to int.
    # age and fare stay float (the C++ reader parses them with `iss >> double`).
    int_cols = ["survived", "pclass", "sex", "sibsp", "parch", "embarked"]
    df[int_cols] = df[int_cols].astype(int)

    # Write as space-separated values (no header)
    df.to_csv(output_txt, sep=" ", header=False, index=False)

    print(f"Converted {len(df)} rows from {input_csv} to {output_txt}")


if __name__ == "__main__":
    in_file = sys.argv[1] if len(sys.argv) > 1 else "train.csv"
    out_file = sys.argv[2] if len(sys.argv) > 2 else "processed_train.txt"
    convert_kaggle_to_simple(in_file, out_file)
