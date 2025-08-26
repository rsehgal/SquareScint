from refactored_m4 import *
import matplotlib.pyplot as plt
import numpy as np
import glob
import os
from scipy.optimize import curve_fit

# Gaussian function for fitting
def gaussian(x, amp, mu, sigma):
    return amp * np.exp(-(x - mu) ** 2 / (2 * sigma ** 2))

# Initialize global lists for means
x_mean_list = []  # For TimeDiff_1_3 means
y_mean_list = []  # For TimeDiff_0_2 means

def run_analysis():
    parent_directories = [
        "/home/harshita/shared/SquareScint_Harshita/x0 data file (copy)",
        "/home/harshita/shared/SquareScint_Harshita/x5 data file",
        "/home/harshita/shared/SquareScint_Harshita/x10 data file",
        "/home/harshita/shared/SquareScint_Harshita/x20 data file",
        "/home/harshita/shared/SquareScint_Harshita/x-10 data file",
        "/home/harshita/shared/SquareScint_Harshita/x-20 data file",
    ]

    file_list = []
    for directory in parent_directories:
        files = glob.glob(os.path.join(directory, "**", "*.root"), recursive=True)
        file_list.extend(files)

    file_list = sorted(file_list)

    # Allowed channels for all files
    allowed_channels_list = [
        get_channels_from_config(["square_slab", "1st_bar", "2nd_bar"]) for _ in file_list
    ]

    print(type(allowed_channels_list))

    E_list = []
    df_grouped_energy_list = []

    plt.figure(figsize=(10, 5))
    colors = ['blue', 'green', 'red', 'orange', 'black', 'purple', 'brown', 'pink']

    for counter, filename in enumerate(file_list):
        print("==== Processing file:", filename)
        E, df_grouped_energy, x_mean_local, y_mean_local = ProcessData(filename, allowed_channels_list[counter])
        E_list.append(E)
        df_grouped_energy_list.append(df_grouped_energy)

    # Plot Energy Histograms per dataset and channel
    for i, E in enumerate(E_list):
        allowed_channels = allowed_channels_list[i]
        plt.figure(figsize=(10, 6))
        for idx, ch in enumerate(allowed_channels):
            if ch in E and len(E[ch]) > 0:
                plt.hist(E[ch], bins=500, histtype='step', alpha=0.7,
                         label=f'Channel {ch}', color=colors[idx % len(colors)])
        plt.xlabel("Energy (keV or ADC units)")
        plt.ylabel("Counts")
        plt.legend()
        plt.yscale("log")
        plt.title(f"Energy Histogram for Dataset {i+1}")
        plt.tight_layout()

    # Energy Ratio Histograms for all datasets combined
    plt.figure(figsize=(12, 6))
    colors = ['red', 'blue', 'green', 'orange', 'purple', 'cyan', 'magenta', 'brown']

    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        ratio_cols = [col for col in df_grouped_energy.columns if col.startswith("Ratio_")]
        for r_idx, col in enumerate(ratio_cols):
            data = df_grouped_energy[col].dropna()
            if len(data) > 0:
                color = colors[(file_idx * len(ratio_cols) + r_idx) % len(colors)]
                plt.hist(data, bins=100, histtype='step', alpha=0.7,
                         label=f'File {file_idx+1} - {col}',
                         color=color, density=True)

    plt.xlabel("Energy Ratio")
    plt.ylabel("Normalized Counts")
    plt.title("Energy Ratio Histograms (All Files)")
    plt.legend(fontsize='small', ncol=2)
    plt.grid(True)
    plt.tight_layout()
    plt.yscale("log")

    # Combined Energy Histograms for all datasets
    plt.figure(figsize=(12, 6))

    for file_idx, E in enumerate(E_list):
        allowed_channels = allowed_channels_list[file_idx]
        for ch_idx, ch in enumerate(allowed_channels):
            if ch in E and len(E[ch]) > 0:
                color_idx = (file_idx * len(allowed_channels) + ch_idx) % len(colors)
                plt.hist(E[ch], bins=500, histtype='step', alpha=0.8,
                         label=f'File {file_idx+1} - {ch}', color=colors[color_idx])

    plt.xlabel("Energy (ADC units or keV)")
    plt.ylabel("Counts")
    plt.yscale("log")
    plt.legend(fontsize='small', ncol=2)
    plt.title("Energy Histograms for Square Slab, 1st Bar, 2nd Bar (dataset)")
    plt.grid(True)
    plt.tight_layout()

    # Time difference histograms (TimeDiff_0_2 and TimeDiff_1_3)
    plt.figure(figsize=(10, 6))

    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        # TimeDiff_0_2
        if "TimeDiff_0_2" in df_grouped_energy.columns:
            time_diff_0_2 = df_grouped_energy["TimeDiff_0_2"].dropna().values / 1000.0
            if len(time_diff_0_2) > 0:
                plt.hist(time_diff_0_2, bins=100, histtype='step', alpha=0.7,
                         label=f'File {file_idx+1} - TimeDiff_0_2',
                         color=colors[file_idx % len(colors)], density=True, linestyle='-')

        # TimeDiff_1_3
        if "TimeDiff_1_3" in df_grouped_energy.columns:
            time_diff_1_3 = df_grouped_energy["TimeDiff_1_3"].dropna().values / 1000.0
            if len(time_diff_1_3) > 0:
                plt.hist(time_diff_1_3, bins=100, histtype='step', alpha=0.7,
                         label=f'File {file_idx+1} - TimeDiff_1_3',
                         color=colors[file_idx % len(colors)], density=True, linestyle='--')

    plt.xlabel("Time Difference (timestamp units / 1000)")
    plt.ylabel("Normalized Counts")
    plt.title("TimeDiff_0_2 and TimeDiff_1_3 Histograms (No Fit, Scaled)")
    plt.legend(fontsize='small', ncol=2)
    plt.grid(True)
    plt.tight_layout()
    plt.xlim(-10, 10)

    # Gaussian fitting for time differences
    plt.figure(figsize=(10, 6))

    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        color = colors[file_idx % len(colors)]

        # Fit TimeDiff_0_2
        if "TimeDiff_0_2" in df_grouped_energy.columns:
            time_diff_0_2 = df_grouped_energy["TimeDiff_0_2"].dropna().values / 1000.0
            if len(time_diff_0_2) > 0:
                counts, bins, _ = plt.hist(time_diff_0_2, bins=100, histtype='step',
                                           alpha=0.6, label=f'File {file_idx+1} - TimeDiff_0_2',
                                           color=color, density=True, linestyle='-')
                bin_centers = 0.5 * (bins[:-1] + bins[1:])
                p0 = [np.max(counts), np.mean(time_diff_0_2), np.std(time_diff_0_2)]
                try:
                    popt, _ = curve_fit(gaussian, bin_centers, counts, p0=p0)
                    x_fit = np.linspace(bin_centers[0], bin_centers[-1], 1000)
                    y_fit = gaussian(x_fit, *popt)
                    plt.plot(x_fit, y_fit, linestyle='--', color=color,
                             label=f'Fit 0_2 F{file_idx+1}: μ={popt[1]:.2f}, σ={popt[2]:.2f}')
                    y_mean_list.append(float(popt[1]))
                except RuntimeError:
                    print(f"⚠️ Gaussian fit failed for TimeDiff_0_2 in File {file_idx+1}")
                    y_mean_list.append(np.nan)

        # Fit TimeDiff_1_3
        if "TimeDiff_1_3" in df_grouped_energy.columns:
            time_diff_1_3 = df_grouped_energy["TimeDiff_1_3"].dropna().values / 1000.0
            if len(time_diff_1_3) > 0:
                counts, bins, _ = plt.hist(time_diff_1_3, bins=100, histtype='step',
                                           alpha=0.6, label=f'File {file_idx+1} - TimeDiff_1_3',
                                           color=color, density=True, linestyle=':')
                bin_centers = 0.5 * (bins[:-1] + bins[1:])
                p0 = [np.max(counts), np.mean(time_diff_1_3), np.std(time_diff_1_3)]
                try:
                    popt, _ = curve_fit(gaussian, bin_centers, counts, p0=p0)
                    x_fit = np.linspace(bin_centers[0], bin_centers[-1], 1000)
                    y_fit = gaussian(x_fit, *popt)
                    plt.plot(x_fit, y_fit, linestyle='-.', color=color,
                             label=f'Fit 1_3 F{file_idx+1}: μ={popt[1]:.2f}, σ={popt[2]:.2f}')
                    x_mean_list.append(float(popt[1]))
                except RuntimeError:
                    print(f"⚠️ Gaussian fit failed for TimeDiff_1_3 in File {file_idx+1}")
                    x_mean_list.append(np.nan)

    plt.xlabel("Time Difference (timestamp units / 1000)")
    plt.ylabel("Normalized Counts")
    plt.title("TimeDiff_0_2 and TimeDiff_1_3 Histograms with Gaussian Fit")
    plt.legend(fontsize='small', ncol=2)
    plt.grid(True)
    plt.tight_layout()
    plt.xlim(-10, 10)
    plt.show()

    # Print results vertically aligned
    print(f"\n{'TimeDiff_0_2':>20} {'TimeDiff_1_3':>20}")
    for y, x in zip(y_mean_list, x_mean_list):
        print(f"{y:>20.4f} {x:>20.4f}")

    return x_mean_list, y_mean_list


# Uncomment and use this part after calling run_analysis()
'''


# Polynomial fitting example using predefined data
mean_values = [-0.13, -0.12, -0.15, -0.12, -0.15, -0.10, -0.11, -0.10, -0.17]
positions = [-20, -15, -10, -5, 0, 5, 10, 15, 20]

# Fit a 3rd order polynomial (position as a function of mean_values)
coeffs = np.polyfit(mean_values, positions, 3)
poly_fit = np.poly1d(coeffs)

print("===================================================")
print("3rd Order Polynomial Coefficients:")
print(f"a (x^3): {coeffs[0]}")
print(f"b (x^2): {coeffs[1]}")
print(f"c (x^1): {coeffs[2]}")
print(f"d (const): {coeffs[3]}")
print("=====================================================")

# Plot polynomial fit
x_vals = np.linspace(min(mean_values), max(mean_values), 500)
y_vals = poly_fit(x_vals)

plt.figure(figsize=(10, 6))
plt.scatter(mean_values, positions, color='blue', label='Data Points')
plt.plot(x_vals, y_vals, 'r--', label='3rd Order Polynomial Fit')
plt.grid(True)
plt.xlabel("Mean of TimeDiff_0_2 (μ)")
plt.ylabel("Position (mm)")
plt.title("3rd Order Polynomial Fit: Position vs. Mean TimeDiff")
plt.legend()
plt.tight_layout()
plt.show()


# Example for prediction using separate file data
separate_file = "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-15_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-15_30min.root"
separate_allowed_channels = get_channels_from_config(["square_slab", "1st_bar", "2nd_bar"])

E_sep, df_grouped_sep = ProcessData(separate_file, separate_allowed_channels)

if "TimeDiff_0_2" in df_grouped_sep.columns:
    delt_values = df_grouped_sep["TimeDiff_0_2"].dropna().values / 1000.0
    predicted_positions = poly_fit(delt_values)

    print("=============================================")
    print(f"Predicted positions from TimeDiff_0_2 for file: {separate_file}")
    print("=============================================")

    for idx, pos in enumerate(predicted_positions):
        print(f"Event {idx + 1}: Position = {pos:.3f} mm")

    plt.figure(figsize=(10, 6))
    plt.hist(predicted_positions, bins=100, histtype='step', color='red')
    plt.xlabel("Predicted Position (mm)")
    plt.ylabel("Counts")
    plt.title(f"Predicted Positions from TimeDiff_0_2 ({separate_file})")
    plt.grid(True)
    plt.tight_layout()
    plt.show()

    print(f"Mean predicted position: {np.mean(predicted_positions):.3f} mm")
    print(f"Std dev of predicted position: {np.std(predicted_positions):.3f} mm")

else:
    print(f"⚠️ 'TimeDiff_0_2' not found in file: {separate_file}")
'''
