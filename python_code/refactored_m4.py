import uproot
import os
import glob
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from collections import Counter



# Load ROOT file
def ProcessData(filename,allowed_channels):
    
    root_files = []  # initialize here

    if os.path.isdir(filename):
        root_files = glob.glob(os.path.join(filename, "*.root"))
        if not root_files:
            raise FileNotFoundError(f"❌ No ROOT files found in directory: {filename}")
        print(f"📁 Directory detected. Using file: {root_files[0]}")
        filename = root_files[0]


    file_ext = os.path.splitext(filename)[-1].lower()
    print("===========================================================")
    print("ALLOWED CHANNELS : ",allowed_channels)
    print("===========================================================")
    #file = uproot.open("/home/harshita/shared/07_Aug_Na22/DataF_Na22_CFD_th_8_8_9_9_Slab_7Aug_0_0.root")
    file = uproot.open(filename)
    print(file.keys())

    tree = file["Data_F"]
    print("Total entries:", tree.num_entries)

    # Load selected columns
    data = tree.arrays(["Timestamp", "Channel", "Energy"], library="pd")
    data = data[data['Channel'].isin(allowed_channels)]

    # Sort by timestamp
    data_sorted = data.sort_values("Timestamp").reset_index(drop=True)

    # Grouping events
    grouped_lists = []
    current_group = []
    group_start_time = None
    required_channel_count = len(set(allowed_channels))

    for row in data_sorted.itertuples(index=False):
        ts = row.Timestamp
        if group_start_time is None:
            group_start_time = ts
            current_group.append(row)
        else:
            if ts - group_start_time <= 24000:
                current_group.append(row)
            else:
                channels = [r.Channel for r in current_group]
                if len(current_group) == len(set(channels)) and len(current_group) == required_channel_count:
                    grouped_lists.append(current_group)
                current_group = [row]
                group_start_time = ts

    print(f"\n✅ Valid groups with {required_channel_count} unique channels: {len(grouped_lists)}")

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
            E[item.Channel].append(item.Energy)
            T[item.Channel].append(item.Timestamp)

    # Print data count per channel
    print("\nCounts per selected channel:")
    for ch in all_channels:
        print(f"Channel {ch}: {len(E[ch])}")

    # Build grouped DataFrame
    unique_channels = sorted({item.Channel for group in grouped_lists for item in group})
    csv_rows = []
    time_rows = []


    for group in grouped_lists:
        row = {f"Ch_{ch}": np.nan for ch in unique_channels}
        time_row = {f"Time_Ch_{ch}": np.nan for ch in unique_channels}
        timestamps=[]
        for item in group:
            row[f"Ch_{item.Channel}"] = item.Energy
            time_row[f"Time_Ch_{item.Channel}"] = item.Timestamp



        csv_rows.append(row)
        time_rows.append(time_row)

    df_grouped_energy = pd.DataFrame(csv_rows)
    df_time = pd.DataFrame(time_rows)


    print("df_time columns:", df_time.columns.tolist())


    # Sort columns by channel number
    df_grouped_energy = df_grouped_energy[sorted(df_grouped_energy.columns, key=lambda x: int(x.split('_')[1]))]
    df_time = df_time[sorted(df_time.columns, key=lambda x: int(x.split('_')[2]))]



    # Add ratios and time differences for valid combinations
    if set([0, 2]).issubset(unique_channels):
        df_grouped_energy['Ratio_0_2'] = df_grouped_energy['Ch_0'] / df_grouped_energy['Ch_2']
        df_grouped_energy['TimeDiff_0_2'] = df_time['Time_Ch_0'] - df_time['Time_Ch_2']

    if set([1, 3]).issubset(unique_channels):
        df_grouped_energy['Ratio_1_3'] = df_grouped_energy['Ch_1'] / df_grouped_energy['Ch_3']
        df_grouped_energy['TimeDiff_1_3'] = df_time['Time_Ch_1'] - df_time['Time_Ch_3']

    if set([4, 5]).issubset(unique_channels):
        df_grouped_energy['Ratio_4_5'] = df_grouped_energy['Ch_4'] / df_grouped_energy['Ch_5']
        df_grouped_energy['TimeDiff_4_5'] = df_time['Time_Ch_4'] - df_time['Time_Ch_5']

    if set([6, 7]).issubset(unique_channels):
        df_grouped_energy['Ratio_6_7'] = df_grouped_energy['Ch_6'] / df_grouped_energy['Ch_7']
        df_grouped_energy['TimeDiff_6_7'] = df_time['Time_Ch_6'] - df_time['Time_Ch_7']

    # Clean data
    df_grouped_energy.replace([np.inf, -np.inf], np.nan, inplace=True)
    df_grouped_energy.dropna(how='any', inplace=True)

       # Print ratios and time diffs if present
    for col in df_grouped_energy.columns:
        if "Ratio" in col or "TimeDiff" in col:
            print(f"\n{col} - first 5 values:")
            print(df_grouped_energy[col].head())

            required_cols = ['Ch_0', 'Ch_1', 'Ch_2', 'Ch_3']
    if all(col in df_grouped_energy.columns for col in required_cols):
        lnQ0 = np.log(df_grouped_energy['Ch_0'])
        lnQ1 = np.log(df_grouped_energy['Ch_1'])
        lnQ2 = np.log(df_grouped_energy['Ch_2'])
        lnQ3 = np.log(df_grouped_energy['Ch_3'])
        
        numerator = lnQ0**2 + lnQ2**2 - lnQ1**2 - lnQ3**2
        denominator = 2 * np.log((df_grouped_energy['Ch_0'] * df_grouped_energy['Ch_2']) / (df_grouped_energy['Ch_1'] * df_grouped_energy['Ch_3']))

    with np.errstate(divide='ignore', invalid='ignore'):
         Q4 = np.exp(numerator / denominator)
    return E, df_grouped_energy  # Now correctly indented inside the function



# ----------------- CONFIG VIA INPUT -----------------
available_setups = ["square_slab", "1st_bar", "2nd_bar"]


# ------------------------------------------

def gaussian(x, A, mu, sigma):
    return A * np.exp(-(x - mu) ** 2 / (2 * sigma ** 2))



# Mapping of detector setups to channels
detector_configurations = {
    "square_slab": [0, 1, 2, 3],
    "1st_bar": [4, 5],
    "2nd_bar": [6, 7]
}

def get_channels_from_config(config_list):
    selected_channels = set()
    for config in config_list:
        if config in detector_configurations:
            selected_channels.update(detector_configurations[config])
        else:
            print(f"⚠️ Unknown configuration: {config}")
    return sorted(selected_channels)

# Get allowed channels dynamically
#allowed_channels = get_channels_from_config(selected_setups)
#print(f"\n✅ Selected setups: {selected_setups}")
#print(f"✅ Selected channels: {allowed_channels}")

