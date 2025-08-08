import uproot
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from collections import Counter

# ----------------- CONFIG VIA INPUT -----------------
available_setups = ["square_slab", "1st_bar", "2nd_bar"]

user_input = input(f"Enter desired setup(s) separated by commas (options: {', '.join(available_setups)}): ").lower()

# Process the input
selected_setups = [setup.strip() for setup in user_input.split(',') if setup.strip() in available_setups]

if not selected_setups:
    raise ValueError(f"No valid setups provided. Please choose from: {available_setups}")

print(f"\n✅ Selected setups: {selected_setups}")

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
allowed_channels = get_channels_from_config(selected_setups)
print(f"\n✅ Selected setups: {selected_setups}")
print(f"✅ Selected channels: {allowed_channels}")
'''
# ----------- NEW: Add multiple ROOT files -----------
root_files = [
    "/home/harshita/shared/SquareScint_Harshita/DataF_Muon_super_setup_orthogonal_16cm_btw_twobars_6aug.root",
    "/home/harshita/shared/07_Aug_Na22/DataF_Na22_CFD_th_8_8_9_9_Slab_7Aug_0_0.root",
    "/home/harshita/shared/Cs137_5pos_7aug/DataF_Cs137_Slab_LED_7Aug_0_0.root"
   
]

all_dataframes = []

for file_path in root_files:
    print(f"📂 Loading: {file_path}")
    file = uproot.open(file_path)

    if "Data_F" not in file:
        print(f"⚠️ 'Data_F' tree not found in {file_path}")
        continue

    tree = file["Data_F"]
    print(f"📈 Entries in '{file_path}': {tree.num_entries}")

    df = tree.arrays(["Timestamp", "Channel", "Energy"], library="pd")
    df = df[df['Channel'].isin(allowed_channels)]
    all_dataframes.append(df)

# Combine all DataFrames into one
if not all_dataframes:
    raise ValueError("❌ No valid ROOT files or data found.")

data = pd.concat(all_dataframes, ignore_index=True)
print(f"✅ Combined entries from all ROOT files: {len(data)}")

'''
# Load ROOT file
file = uproot.open("/data/Aug08_CFD/DataF_Muon_CFD_th_8_8_9_9_Slab_8Aug_0_0.root")
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

# --- CHANGED HERE ---
# Energy and timestamp storage: use allowed_channels, not all_channels
E = {ch: [] for ch in allowed_channels}
T = {ch: [] for ch in allowed_channels}

for group_idx, group in enumerate(grouped_lists):
    for item in group:
        E[item.Channel].append(item.Energy)
        T[item.Channel].append(item.Timestamp)

# Print data count per channel
print("\nCounts per selected channel:")
for ch in allowed_channels:
    print(f"Channel {ch}: {len(E[ch])}")


unique_channels = sorted({item.Channel for group in grouped_lists for item in group})
csv_rows = []
time_rows = []

for group in grouped_lists:
    row = {f"Ch_{ch}": np.nan for ch in unique_channels}
    time_row = {f"Time_Ch_{ch}": np.nan for ch in unique_channels}
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

# Save CSV
df_grouped_energy.to_csv("superset_ortho_data_6ch.csv", index=False)
print("\n✅ Saved clean grouped energy data to 'superset_ortho_data_6ch.csv'")



# Plot energy histograms
plt.figure(figsize=(10, 5))
colors = ['blue', 'green', 'red', 'orange', 'black', 'purple', 'brown', 'pink']

for idx, ch in enumerate(allowed_channels):
    energy_values = E.get(ch, [])
    if len(energy_values) > 0:
        plt.hist(energy_values, bins=100, histtype='step', alpha=0.7,
                 label=f'Channel {ch}', color=colors[idx % len(colors)])
    else:
        print(f"⚠️ No energy data available for channel {ch}")

plt.xlabel("Energy (keV or ADC units)")
plt.ylabel("Counts")
plt.legend()
plt.tight_layout()
plt.yscale("log")
plt.show()

# ✅ Plot all available energy ratio histograms on the same figure
plt.figure(figsize=(10, 5))
for col in df_grouped_energy.columns:
    if col.startswith("Ratio_"):
        plt.hist(df_grouped_energy[col], bins=500, histtype='step', alpha=0.7, label=col)

plt.title("Energy Ratio Histograms")
plt.xlabel("Energy Ratio")
plt.ylabel("Counts")
plt.legend()
plt.tight_layout()
plt.yscale("log")
plt.show()

# -------------------------------------------------------------
# -------------------- Plot Time Difference Histograms with Gaussian Fit --------------------
all_time_diff_data = np.concatenate([
    df_grouped_energy[col].values
    for col in df_grouped_energy.columns
    if col.startswith("TimeDiff_") and len(df_grouped_energy[col]) > 10
])

print(f"\nTotal combined time difference data points: {len(all_time_diff_data)}")


plt.figure(figsize=(10, 6))
colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink', 'cyan']
color_idx = 0

time_diff_columns = [col for col in df_grouped_energy.columns if col.startswith("TimeDiff_") and len(df_grouped_energy[col]) > 10]

for col in time_diff_columns:
    data = df_grouped_energy[col].values

    # Histogram for this TimeDiff column
    counts, bin_edges, _ = plt.hist(data, bins=100, histtype='step', alpha=0.7,
                                   label=f'{col} Data', color=colors[color_idx % len(colors)])

    # Bin centers for fitting
    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2

    # Initial guesses for Gaussian fit
    A_guess = counts.max()
    mu_guess = np.mean(data)
    sigma_guess = np.std(data)

    try:
        popt, _ = curve_fit(gaussian, bin_centers, counts, p0=[A_guess, mu_guess, sigma_guess])
        A_fit, mu_fit, sigma_fit = popt
        print(f"Gaussian fit for {col}: mu = {mu_fit:.4f}, sigma = {sigma_fit:.4f}")

        # Plot Gaussian fit curve
        x_fit = np.linspace(bin_edges[0], bin_edges[-1], 1000)
        plt.plot(x_fit, gaussian(x_fit, *popt), color=colors[color_idx % len(colors)], linestyle='--', linewidth=2)

    except RuntimeError:
        print(f"Gaussian fit failed for {col}")

    color_idx += 1

plt.xlabel("Time Difference (timestamp units)")
plt.ylabel("Counts")
plt.title("Time Difference Histograms with Individual Gaussian Fits")
plt.legend()
plt.xlim (-20000, 20000)
plt.tight_layout()
plt.show()


# ---------------------- Final Combined Time Difference Plot with One Gaussian Fit per Source ----------------------
# -------------------- Final Combined Time Difference Plot --------------------
# One histogram per source, but only one combined Gaussian fit
'''
# Separate time difference arrays per ROOT file
time_diffs_muon = []
time_diffs_na22 = []
time_diffs_cs137 = []

# Categorize time differences by file index
file_time_diff_map = {
    0: time_diffs_muon,
    1: time_diffs_na22,
    2: time_diffs_cs137
}

# Track which file each row came from
file_row_counts = [len(df) for df in all_dataframes]
file_indices = np.repeat(range(len(file_row_counts)), file_row_counts)

# Assign each row in df_grouped_energy a file ID
df_grouped_energy['file_id'] = file_indices[:len(df_grouped_energy)]

# Populate time difference arrays
for idx, row in df_grouped_energy.iterrows():
    file_id = int(row['file_id'])
    for col in df_grouped_energy.columns:
        if col.startswith("TimeDiff_") and not pd.isna(row[col]):
            file_time_diff_map[file_id].append(row[col])

# Convert to NumPy arrays
time_diffs_muon = np.array(time_diffs_muon)
time_diffs_na22 = np.array(time_diffs_na22)
time_diffs_cs137 = np.array(time_diffs_cs137)

# Combine all time differences for single Gaussian fit
combined_time_diffs = np.concatenate([time_diffs_muon, time_diffs_na22, time_diffs_cs137])

# Plot histograms
plt.figure(figsize=(10, 6))
bins = np.linspace(-20000, 20000, 100)

plt.hist(time_diffs_muon, bins=bins, alpha=0.6, label='Muon', histtype='step', color='blue')
plt.hist(time_diffs_na22, bins=bins, alpha=0.6, label='Na-22', histtype='step', color='green')
plt.hist(time_diffs_cs137, bins=bins, alpha=0.6, label='Cs-137', histtype='step', color='red')

# Fit Gaussian to combined data
counts, bin_edges = np.histogram(combined_time_diffs, bins=bins)
bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
p0 = [counts.max(), np.mean(combined_time_diffs), np.std(combined_time_diffs)]

try:
    popt, _ = curve_fit(gaussian, bin_centers, counts, p0=p0)
    A_fit, mu_fit, sigma_fit = popt

    # Plot Gaussian fit
    x_fit = np.linspace(bin_edges[0], bin_edges[-1], 1000)
    plt.plot(x_fit, gaussian(x_fit, *popt), 'k--', linewidth=2,
             label=f'Combined Gaussian Fit\nμ = {mu_fit:.1f}, σ = {sigma_fit:.1f}')
except RuntimeError:
    print("⚠️ Gaussian fit failed.")

# Finalize plot
plt.xlabel("Time Difference (timestamp units)")
plt.ylabel("Counts")
plt.title("Time Difference Histograms (Muon, Na-22, Cs-137) with One Combined Gaussian Fit")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.xlim(-20000, 20000)
plt.show()
''' 

