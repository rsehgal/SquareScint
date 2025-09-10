'''
import pandas as pd
import uproot
import awkward as ak

# Load CSV
df = pd.read_csv("/home/harshita/shared/text.csv")

# Select and rename the columns
df = df[['Ch_0', 'Ch_1', 'Ch_2', 'Ch_3']]#'Ch_4', 'Ch_5']]
df.columns = ['Q0', 'Q1', 'Q2', 'Q3'] #'Q4', 'Q5']  # Rename to desired branch names

# Convert to awkward array
#ak_array = ak.from_pandas(df)


ak_array = ak.Array({col: df[col].values for col in df.columns})


# Save to ROOT file with one tree named 'Events'
with uproot.recreate("/home/harshita/shared/text1_xdata.root") as f:
    f["Events"] = ak_array  # 'Events' is the tree name

print("✅ Saved ROOT file with tree 'Events' and branches: Q0–Q5")

import uproot
import awkward as ak
import pandas as pd

root_path = "/home/harshita/shared/SquareScint_Harshita/Data/merged_outputt_.root"
tree_name = "treeML"
csv_path = "/home/harshita/shared/SquareScint_Harshita/Data/output_merged_.csv"

with uproot.open(root_path) as file:
    tree = file[ ]
    # Load jagged arrays as awkward arrays
    arrays = tree.arrays(library="ak")

# Convert awkward arrays to pandas DataFrame after flattening or converting jagged arrays to lists
# For example, convert each jagged array branch to lists:
data_dict = {}
for key, array in arrays.items():
    # Convert jagged array to list of lists or list of values
    data_dict[key] = ak.to_list(array)

df = pd.DataFrame(data_dict)

df.to_csv(csv_path, index=False)
print(f"Saved to {csv_path}")
'''





