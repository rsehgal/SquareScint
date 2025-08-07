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

allowed_channels = [0, 1, 2, 3,4,5]
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
            if len(current_group) == len(set(channels)) and len(current_group) == 6:
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




print(len(E[0]), " : ", len(E[1]), " : ", len(E[2]), " : ", len(E[3]), " : ", len(E[4]), " : ", len(E[5]))


unique_channels = sorted({item.Channel for group in grouped_lists for item in group})

csv_rows = []
#time_rows =[]
for group in grouped_lists:
    row = {f"Ch_{ch}": np.nan for ch in unique_channels}
    time_row = {f"Time_Ch_{ch}": np.nan for ch in unique_channels}
    for item in group:
        row[f"Ch_{item.Channel}"] = item.Energy
        time_row[f"Time_Ch_{item.Channel}"] = item.Timestamp

    csv_rows.append(row)
    #time_rows.append(time_row)
    '''

df_grouped_energy = pd.DataFrame(csv_rows)
df_time = pd.DataFrame(time_rows)
df_grouped_energy = df_grouped_energy[sorted(df_grouped_energy.columns, key=lambda x: int(x.split('_')[1]))]
df_time = df_time[sorted(df_time.columns, key=lambda x: int(x.split('_')[2]))]

df_grouped_energy['Ratio_0_1'] = df_grouped_energy['Ch_0'] / df_grouped_energy['Ch_1']
df_grouped_energy['Ratio_2_3'] = df_grouped_energy['Ch_2'] / df_grouped_energy['Ch_3']
df_grouped_energy['Ratio_4_5'] = df_grouped_energy['Ch_4'] / df_grouped_energy['Ch_5']

# ✅ Compute time differences
df_grouped_energy['TimeDiff_0_1'] = df_time['Time_Ch_0'] - df_time['Time_Ch_1']
df_grouped_energy['TimeDiff_2_3'] = df_time['Time_Ch_2'] - df_time['Time_Ch_3']
df_grouped_energy['TimeDiff_4_5'] = df_time['Time_Ch_4'] - df_time['Time_Ch_5']

# ✅ Clean NaNs/Infs
df_grouped_energy.replace([np.inf, -np.inf], np.nan, inplace=True)
df_grouped_energy.dropna(subset=['Ratio_0_1', 'Ratio_2_3', 'Ratio_4_5', 'TimeDiff_0_1', 'TimeDiff_2_3', 'TimeDiff_4_5'], inplace=True)

print(df_grouped_energy[['Ratio_0_1', 'Ratio_2_3', 'Ratio_4_5', 'TimeDiff_0_1', 'TimeDiff_2_3', 'TimeDiff_4_5']].describe())



print("Energy Ratio CH_0 / CH_1:")
print(df_grouped_energy['Ratio_0_1'].head())

print("\nEnergy Ratio CH_2 / CH_3:")
print(df_grouped_energy['Ratio_2_3'].head())

# Print first few entries of time differences
print("\nTime Difference CH_0 - CH_1:")
print(df_grouped_energy['TimeDiff_0_1'].head())

print("\nTime Difference CH_2 - CH_3:")
print(df_grouped_energy['TimeDiff_2_3'].head())
'''

#df_grouped_energy.to_csv
df_grouped_energy = pd.DataFrame(csv_rows)
df_grouped_energy.to_csv("superset_ortho_data_6ch.csv", index=False)
print("\n✅ Saved clean grouped energy data to 'superset_ortho_data_6ch.csv'")


plt.figure(figsize=(10, 5))

# Plot filled histograms with custom colors and transparency
plt.hist(E[0], bins=500,histtype ='step', alpha=0.7, label='Channel 0', color='blue')
plt.hist(E[1], bins=500,histtype ='step', alpha=0.7, label='Channel 1', color='green')
plt.hist(E[2], bins=500,histtype ='step', alpha=0.7, label='Channel 2', color='red')
plt.hist(E[3], bins=500,histtype ='step', alpha=0.7, label='Channel 3', color='orange')
plt.hist(E[4], bins=500,histtype ='step', alpha=0.7, label='Channel 2', color='black')
plt.hist(E[5], bins=500,histtype ='step', alpha=0.7, label='Channel 3', color='purple')

#plt.yscale('log')
plt.xlabel("Energy (keV or ADC units)")  # Adjust unit if known
plt.ylabel("Counts")
plt.legend()
plt.tight_layout()
plt.show()



'''
colormap = plt.cm.nipy_spectral
for idx, ch in enumerate(all_channels):
    color = colormap(idx / len(all_channels))
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
'''
'''
# ✅ Plot energy ratio histograms
plt.figure(figsize=(10, 5))
plt.hist(df_grouped_energy['Ratio_0_1'],histtype ='step', bins=100, alpha=0.7, label='CH_0 / CH_1', color='blue')
plt.hist(df_grouped_energy['Ratio_2_3'],histtype ='step', bins=100, alpha=0.7, label='CH_2 / CH_3', color='green')
plt.hist(df_grouped_energy['Ratio_4_5'],histtype ='step', bins=100, alpha=0.7, label='CH_4 / CH_5', color='black')
plt.xlabel("Energy Ratio")
plt.ylabel("Counts")
#plt.yscale('log')
plt.show()

# ✅ Plot time difference histograms
plt.figure(figsize=(10, 5))
plt.hist(df_grouped_energy['TimeDiff_0_1'],histtype ='step', bins=100, alpha=0.7, label='Time CH_0 - CH_1', color='orange')
plt.hist(df_grouped_energy['TimeDiff_2_3'],histtype ='step', bins=100, alpha=0.7, label='Time CH_2 - CH_3', color='purple')
plt.hist(df_grouped_energy['TimeDiff_4_5'],histtype ='step', bins=100, alpha=0.7, label='Time CH_4 - CH_5', color='red')
plt.xlabel("Time Difference (timestamp units)")
plt.ylabel("Counts")
#plt.yscale('log')
plt.show()
'''