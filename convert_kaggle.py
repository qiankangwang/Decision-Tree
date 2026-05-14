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

    # Drop rows with missing values
    df = df.dropna()

    # Encode categorical fields
    df["sex"] = df["sex"].map({"male": 0, "female": 1})
    df["embarked"] = df["embarked"].str.lower().map({"s": 0, "c": 1, "q": 2})

    # Write as space-separated values (no header)
    df.to_csv(output_txt, sep=" ", header=False, index=False)

    print(f"Converted {len(df)} rows from {input_csv} to {output_txt}")


if __name__ == "__main__":
    in_file = sys.argv[1] if len(sys.argv) > 1 else "train.csv"
    out_file = sys.argv[2] if len(sys.argv) > 2 else "processed_train.txt"
    convert_kaggle_to_simple(in_file, out_file)
