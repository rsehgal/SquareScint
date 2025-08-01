import uproot
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
from collections import Counter

# Open ROOT file and tree
file = uproot.open("/home/barc/Harshita/DataF_Muon_Cente_Bar_3600s_01Aug.root")
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
            channels =[r.Channel for r in current_group]
            if len(current_group) == len(set(channels)) and len(current_group) == 6:
                #channels = [r.Channel for r in current_group]
                #if len(set(channels)) == 4:
                grouped_lists.append(current_group)

            # Start a new group
            current_group = [row]
            group_start_time = ts

# Final group check
'''
    channels = [r.Channel for r in current_group]
    if len(current_group) == len(set(channels)):
        grouped_lists.append(current_group)

        '''

print(f"\n✅ Valid groups with 4 unique channels: {len(grouped_lists)}")

all_channels = sorted(data['Channel'].unique())
#print(f"\n dectected channels:{all_channels}")
clean_channels =[int(ch) for ch in all_channels]
log_text = f"\n dectected channels:{clean_channels}"

print(f"\n{log_text}")

with open("mu3.log", "w") as log_file:
    log_file.write(log_text+"\n")

# Extract energy values by channel
E = {ch:[] for ch in all_channels}


for group in grouped_lists:
    for item in group:
        E[item.Channel].append(item.Energy)

#index=0
print(len(E[0])," : ",len(E[1])," : ",len(E[2])," : ",len(E[3])," : ",len(E[4])," : ",len(E[5]))
'''
plt.hist(E[4], bins=500, histtype='step', edgecolor='blue', align='left', label='Ch 2')
plt.hist(E[5], bins=500, histtype='step', edgecolor='green', align='left', label='Ch 3')
e4=np.array(E[4])
e5=np.array(E[5])
Egeom = np.sqrt(e4*e5)
plt.hist(Egeom, bins=500, histtype='step', edgecolor='red', align='left', label='Ch 3')
'''


colormap = plt.cm.nipy_spectral  # Continuous colormap

for idx, ch in enumerate(all_channels):
    color = colormap(idx / len(all_channels))  # Map index to color
    plt.hist(
        E[ch],
        bins=500,
        histtype='step',
        edgecolor=color,
        align='left',
        label=f'Ch {ch}'
    )
plt.yscale('log')
plt.xlabel("Energy")
plt.ylabel("Counts")
plt.title("Energy Spectra by Channel")
plt.legend()
plt.tight_layout()
plt.show()
