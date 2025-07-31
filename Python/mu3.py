import uproot
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
from collections import Counter

# Open ROOT file and tree
file = uproot.open("DataF_SqrSci_Cs137_10min_Th_19_20_22_22_990v_975v_975v_1025v_31July.root")
print(file.keys())

tree = file["Data_F"]
print("Total entries:", tree.num_entries)

# Load only needed columns
data = tree.arrays(["Timestamp", "Channel", "Energy"], library="pd")

# Sort data by timestamp
data_sorted = data.sort_values("Timestamp").reset_index(drop=True)

# Grouping events based on timestamp difference ≤ 20000
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
            # Check if valid group before appending
            if len(current_group) == 4:
                channels = [r.Channel for r in current_group]
                if len(set(channels)) == 4:
                    grouped_lists.append(current_group)

            # Start a new group
            current_group = [row]
            group_start_time = ts

# Final group check
if len(current_group) == 4:
    channels = [r.Channel for r in current_group]
    if len(set(channels)) == 4:
        grouped_lists.append(current_group)

print(f"\n✅ Valid groups with 4 unique channels: {len(grouped_lists)}")

# Extract energy values by channel
E0, E1, E2, E3 = [], [], [], []
E = [E0, E1, E2, E3]

for group in grouped_lists:
    for item in group:
        E[item.Channel].append(item.Energy)

# Plot histograms
plt.hist(E[0], bins=500, histtype='step', edgecolor='black', align='left', label='Ch 0')
plt.hist(E[1], bins=500, histtype='step', edgecolor='red', align='left', label='Ch 1')
plt.hist(E[2], bins=500, histtype='step', edgecolor='blue', align='left', label='Ch 2')
plt.hist(E[3], bins=500, histtype='step', edgecolor='green', align='left', label='Ch 3')

plt.yscale('log')
plt.xlabel("Energy")
plt.ylabel("Counts")
plt.title("Energy Spectra by Channel")
plt.legend()
plt.show()

# Optional: Group size visualization
group_sizes = [len(group) for group in grouped_lists]
frequency = Counter(group_sizes)
print("\nGroup size frequencies:", frequency)

plt.hist(group_sizes, bins=range(0, max(group_sizes) + 2), edgecolor='black', align='left')
plt.xlabel("Group Size")
plt.ylabel("Frequency")
plt.title("Histogram of Group Sizes")
plt.show()
