import uproot
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# ------------------ SETUP SELECTION ------------------
available_setups = ["square_slab", "1st_bar", "2nd_bar"]

user_input = input(f"Enter desired setup(s) separated by commas (options: {', '.join(available_setups)}): ").lower()
selected_setups = [setup.strip() for setup in user_input.split(',') if setup.strip() in available_setups]

if not selected_setups:
    raise ValueError(f"No valid setups provided. Please choose from: {available_setups}")

print(f"\n✅ Selected setups: {selected_setups}")

# ------------------ SETUP TO CHANNELS ------------------
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

allowed_channels = get_channels_from_config(selected_setups)
print(f"✅ Selected channels: {allowed_channels}")

# ------------------ LOAD ROOT FILES FUNCTION ------------------
def load_multiple_root_files(file_paths, allowed_channels):
    all_dataframes = []
    for file_path in file_paths:
        print(f"📂 Loading: {file_path}")
        try:
            file = uproot.open(file_path)
            if "Data_F" not in file:
                print(f"⚠️ 'Data_F' tree not found in {file_path}")
                continue
            tree = file["Data_F"]
            df = tree.arrays(["Timestamp", "Channel", "Energy"], library="pd")
            df = df[df['Channel'].isin(allowed_channels)]
            all_dataframes.append(df)
        except Exception as e:
            print(f"❌ Error reading {file_path}: {e}")
            continue
    if not all_dataframes:
        raise ValueError("❌ No valid ROOT files or data found.")
    combined_df = pd.concat(all_dataframes, ignore_index=True)
    print(f"✅ Combined entries: {len(combined_df)}")
    return combined_df, all_dataframes

# ------------------ USER FILE INPUT ------------------
file_input = input("\nEnter full paths to ROOT files separated by commas:\n").strip()
root_files = [fp.strip() for fp in file_input.split(',') if fp.strip()]
data, all_dataframes = load_multiple_root_files(root_files, allowed_channels)

# ------------------ GROUPING EVENTS ------------------
data_sorted = data.sort_values("Timestamp").reset_index(drop=True)
grouped_lists = []
current_group = []
group_start_time = None
required_channel_count = len(set(allowed_channels))

for row in data_sorted.itertuples(index=False):
    ts = row.Timestamp
    if group_start_time is None:
        group_start_time = ts
        current_group.append(row)
    elif ts - group_start_time <= 24000:
        current_group.append(row)
    else:
        channels = [r.Channel for r in current_group]
        if len(current_group) == len(set(channels)) and len(current_group) == required_channel_count:
            grouped_lists.append(current_group)
        current_group = [row]
        group_start_time = ts

print(f"\n✅ Valid groups with {required_channel_count} unique channels: {len(grouped_lists)}")

# ------------------ LOG DETECTED CHANNELS ------------------
all_channels = sorted(data['Channel'].unique())
with open("mu3.log", "w") as log_file:
    log_file.write(f"Detected channels: {all_channels}\n")
print(f"📝 Detected channels: {all_channels}")

# ------------------ ENERGY / TIME STORAGE ------------------
E = {ch: [] for ch in allowed_channels}
T = {ch: [] for ch in allowed_channels}

for group in grouped_lists:
    for item in group:
        E[item.Channel].append(item.Energy)
        T[item.Channel].append(item.Timestamp)

print("\nCounts per selected channel:")
for ch in allowed_channels:
    print(f"Channel {ch}: {len(E[ch])}")

# ------------------ DATAFRAME CONSTRUCTION ------------------
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
df_grouped_energy = df_grouped_energy[sorted(df_grouped_energy.columns, key=lambda x: int(x.split('_')[1]))]
df_time = df_time[sorted(df_time.columns, key=lambda x: int(x.split('_')[2]))]

# ------------------ ADD RATIOS & TIME DIFFERENCES ------------------
def add_ratio_and_time_diff(ch1, ch2):
    if set([ch1, ch2]).issubset(unique_channels):
        df_grouped_energy[f'Ratio_{ch1}_{ch2}'] = df_grouped_energy[f'Ch_{ch1}'] / df_grouped_energy[f'Ch_{ch2}']
        df_grouped_energy[f'TimeDiff_{ch1}_{ch2}'] = df_time[f'Time_Ch_{ch1}'] - df_time[f'Time_Ch_{ch2}']

add_ratio_and_time_diff(0, 2)
add_ratio_and_time_diff(1, 3)
add_ratio_and_time_diff(4, 5)
add_ratio_and_time_diff(6, 7)

df_grouped_energy.replace([np.inf, -np.inf], np.nan, inplace=True)
df_grouped_energy.dropna(how='any', inplace=True)

for col in df_grouped_energy.columns:
    if "Ratio" in col or "TimeDiff" in col:
        print(f"\n{col} - first 5 values:")
        print(df_grouped_energy[col].head())

# ------------------ SAVE TO CSV ------------------
df_grouped_energy.to_csv("superset_ortho_data_6ch.csv", index=False)
print("\n✅ Saved clean grouped energy data to 'superset_ortho_data_6ch.csv'")

# ------------------ PLOT ENERGY HISTOGRAMS ------------------
plt.figure(figsize=(10, 5))
colors = ['blue', 'green', 'red', 'orange', 'black', 'purple', 'brown', 'pink']

for idx, ch in enumerate(allowed_channels):
    energy_values = E.get(ch, [])
    if energy_values:
        plt.hist(energy_values, bins=100, histtype='step', alpha=0.7, label=f'Channel {ch}', color=colors[idx % len(colors)])
    else:
        print(f"⚠️ No energy data available for channel {ch}")

plt.xlabel("Energy (keV or ADC units)")
plt.ylabel("Counts")
plt.legend()
plt.tight_layout()
plt.yscale("log")
plt.show()

# ------------------ ENERGY RATIOS ------------------
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

# ------------------ TIME DIFFERENCES + FIT ------------------
def gaussian(x, A, mu, sigma):
    return A * np.exp(-(x - mu) ** 2 / (2 * sigma ** 2))

plt.figure(figsize=(10, 6))
colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink', 'cyan']
color_idx = 0

time_diff_columns = [col for col in df_grouped_energy.columns if col.startswith("TimeDiff_")]

for col in time_diff_columns:
    data = df_grouped_energy[col].values
    if len(data) < 10:
        continue

    counts, bin_edges, _ = plt.hist(data, bins=100, histtype='step', alpha=0.7,
                                    label=f'{col} Data', color=colors[color_idx % len(colors)])

    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2

    try:
        popt, _ = curve_fit(gaussian, bin_centers, counts,
                            p0=[counts.max(), np.mean(data), np.std(data)])
        x_fit = np.linspace(bin_edges[0], bin_edges[-1], 1000)
        plt.plot(x_fit, gaussian(x_fit, *popt), linestyle='--',
                 linewidth=2, color=colors[color_idx % len(colors)])

        A_fit, mu_fit, sigma_fit = popt
        print(f"Gaussian fit for {col}: mu = {mu_fit:.2f}, sigma = {sigma_fit:.2f}")

    except RuntimeError:
        print(f"⚠️ Gaussian fit failed for {col}")

    color_idx += 1

plt.xlabel("Time Difference (timestamp units)")
plt.ylabel("Counts")
plt.title("Time Difference Histograms with Gaussian Fits")
plt.legend()
plt.xlim(-20000, 20000)
plt.tight_layout()
plt.show()




