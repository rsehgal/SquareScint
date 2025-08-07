import uproot
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from collections import Counter

def gaussian(x, A, mu, sigma):
    return A * np.exp(-(x - mu) ** 2 / (2 * sigma ** 2))

# Load ROOT file
file = uproot.open("/home/harshita/shared/SquareScint_Harshita/DataF_Muon_super_setup_orthogonal_16cm_btw_twobars_6aug.root")
print(file.keys())

tree = file["Data_F"]
print("Total entries:", tree.num_entries)

# Load selected columns
data = tree.arrays(["Timestamp", "Channel", "Energy"], library="pd")

allowed_channels = [0, 1, 2, 3]
data = data[data['Channel'].isin(allowed_channels)]

# Sort by timestamp
data_sorted = data.sort_values("Timestamp").reset_index(drop=True)

# Grouping events
grouped_lists = []
current_group = []
group_start_time = None

for row in data_sorted.itertuples(index=False):
    ts = row.Timestamp
    if group_start_time is None:
        group_start_time = ts
        current_group.append(row)
    else:
        if ts - group_start_time <= 20000:
            current_group.append(row)
        else:
            channels = [r.Channel for r in current_group]
            if len(current_group) == len(set(channels)) and len(current_group) == 4:
                grouped_lists.append(current_group)
            current_group = [row]
            group_start_time = ts

print(f"\n✅ Valid groups with 4 unique channels: {len(grouped_lists)}")

# Detect channels
all_channels = sorted(data['Channel'].unique())
clean_channels = [int(ch) for ch in all_channels]
log_text = f"\nDetected channels: {clean_channels}"
print(log_text)

with open("mu3.log", "w") as log_file:
    log_file.write(log_text + "\n")

# Energy and timestamp storage
E = {ch: [] for ch in all_channels}
T = {ch: [] for ch in all_channels}

for group_idx, group in enumerate(grouped_lists):
    for item in group:
        # Collect energy and timestamp per channel (if you still want to keep this for analysis)
        E[item.Channel].append(item.Energy)
        T[item.Channel].append(item.Timestamp)



'''
for group in grouped_lists:
    for item in group:
        E[item.Channel].append(item.Energy)
        T[item.Channel].append(item.Timestamp)
'''
print(len(E[0]), " : ", len(E[1]), " : ", len(E[2]), " : ", len(E[3]))# " : ", #len(E[4]), " : ", len(E[5]))


unique_channels = sorted({item.Channel for group in grouped_lists for item in group})

csv_rows = []
for group in grouped_lists:
    row = {f"Ch_{ch}": np.nan for ch in unique_channels}
    for item in group:
        row[f"Ch_{item.Channel}"] = item.Energy
    csv_rows.append(row)

df_grouped_energy = pd.DataFrame(csv_rows)
df_grouped_energy = df_grouped_energy[sorted(df_grouped_energy.columns, key=lambda x: int(x.split('_')[1]))]

df_grouped_energy.to_csv("text_ortho6_data.csv", index=False)
print("\n✅ Saved clean grouped energy data to 'text_ortho6_data.csv'")


plt.figure(figsize=(10, 5))

# Plot filled histograms with custom colors and transparency
plt.hist(E[0], bins=500,histtype ='step', alpha=0.7, label='Channel 0', color='blue')
plt.hist(E[1], bins=500,histtype ='step', alpha=0.7, label='Channel 1', color='green')
plt.hist(E[2], bins=500,histtype ='step', alpha=0.7, label='Channel 2', color='red')
plt.hist(E[3], bins=500,histtype ='step', alpha=0.7, label='Channel 3', color='orange')

plt.yscale('log')
plt.xlabel("Energy")
plt.ylabel("Counts")
plt.title("Energy Spectra by Channel")
plt.legend()
plt.tight_layout()
plt.show()