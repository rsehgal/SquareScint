import uproot
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
from collections import Counter

file = uproot.open("data.root")
print(file.keys())

tree = file["Data_F"]
entries = 50000
print("Total entries : ",tree.num_entries)

# Load and sort data
data = tree.arrays(library="pd")
data_sorted = data.sort_values(by="Timestamp", ascending=True).reset_index(drop=True)

# Grouping by timestamp difference <= 20000
grouped_lists = []
current_group = []
group_start_time = None
rows_processed = 0

for index, row in data_sorted.iterrows():
    #if rows_processed >= entries:
    #if index >= entries:
        #break

    ts = row["Timestamp"]

    if group_start_time is None:
        group_start_time = ts 
        current_group.append(row)
    else:
        if ts - group_start_time <= 20000:
            current_group.append(row)
        else:
            grouped_lists.append(current_group)
            current_group = [row]
            group_start_time = ts

   # rows_processed += 1
             # Start new group
            current_group = [row]
            group_start_time = ts

# Final group check
if len(current_group) == 4:
    channels = [r["Channel"] for r in current_group]
    if len(set(channels)) == 4:
        grouped_lists.append(current_group)

# Count how many valid groups we have
print(f"\nValid groups with 4 unique channels: {len(grouped_lists)}")

'''
# 🔍 Print each group with channel info
print("\n--- Valid Groups with 4 Unique Channels ---\n")
for i, group in enumerate(grouped_lists):
    print(f"Group {i+1}:")
    for row in group:
        print(f"  Channel: {row['Channel']}, Timestamp: {row['Timestamp']}, Energy: {row.get('Energy', 'N/A')}")
    print("-" * 40)

'''
# ✅ Extract energy values by channel
E0, E1, E2, E3 = [], [], [], []
E=[E0, E1,E2,E3]

for group in grouped_lists:
    #print("===========================")
    for item in group:
        #print(item)
        E[item["Channel"]].append(item["Energy"])

        '''
        if item["Channel"] == 0:
            E0.append(item["Energy"])
            E1.append(ITEM["Energy"])
            E1.append(ITEM["Energy"])
            '''

plt.hist(E[0], bins=500, edgecolor='black', align='left')
plt.hist(E[1], bins=500, edgecolor='red', align='left')
plt.hist(E[2], bins=500, edgecolor='blue', align='left')
plt.hist(E[3], bins=500, edgecolor='green', align='left')
plt.yscale('log')
plt.show()

'''
for group in grouped_lists:
    # Map channel to energy
    ch_energy_map = {row["Channel"]: row.get("Energy", None) for row in group}

    # Append energy values to respective lists
    if 0 in ch_energy_map: E0.append(ch_energy_map[0])
    if 1 in ch_energy_map: E1.append(ch_energy_map[1])
    if 2 in ch_energy_map: E2.append(ch_energy_map[2])
    if 3 in ch_energy_map: E3.append(ch_energy_map[3])

# 🔍 Print energy values by channel
print("\n--- Energy Lists by Channel ---")
print("E0:", E0)
print("E1:", E1)
print("E2:", E2)
print("E3:", E3)

'''

'''
# Optional: visualize the timestamps of those valid groups
group_sizes = [len(group) for group in grouped_lists]  # should all be 4

plt.hist(group_sizes, bins=range(0, 6), edgecolor='black', align='left')
plt.xlabel("Group Size (Only Unique 4-Channel Groups)")
plt.ylabel("Frequency")
plt.title("Valid 4-Channel Groups Histogram")
plt.show()
'''
'''
#print("Num of Groups : " , len(grouped_lists))
#if current_group:
#    grouped_lists.append(current_group)

group_sizes = [len(group) for group in grouped_lists]
frequency = Counter(group_sizes)
print(frequency)

#print(group_sizes)


totalEntries=0
#for group in grouped_lists:
    
plt.hist(group_sizes, bins=range(0, max(group_sizes)+2), edgecolor='black', align='left')
plt.show()
'''

