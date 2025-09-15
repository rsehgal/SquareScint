# ALSO HERE IN THIS FILE WE R DEFINING A FUNCTION 
# HERE WE ARE JUST TAKING THE TIME DIFF OF 0_2 AND
# FITTING GAUSSIAN THEN PLOTS WILL GET DISPLAYED

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

# Gaussian function for fitting
def gaussian(x, amp, mu, sigma):
    return amp * np.exp(-(x - mu) ** 2 / (2 * sigma ** 2))

# ----------------------- TimeDiff_0_2 Histogram ----------------------

def plot_timediff_0_2(df_grouped_energy_list, colors):
    plt.figure(figsize=(10, 6))
    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        if "TimeDiff_0_2" in df_grouped_energy.columns:
            time_diff_0_2 = df_grouped_energy["TimeDiff_0_2"].dropna().values / 1000.0
            if len(time_diff_0_2) > 0:
                plt.hist(time_diff_0_2, bins=100, histtype='step', alpha=0.7,
                         label=f'File {file_idx + 1} - TimeDiff_0_2',
                         density=True, linestyle='--')

    plt.xlabel("Time Difference (timestamp units / 1000)", fontsize =12)
    plt.ylabel("Normalized Counts", fontsize=12)
    plt.title("TimeDiff_0_2 Histograms (No Fit)",fontsize =12)
    plt.xticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.yticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.legend(fontsize=12)
    plt.grid(True)
    plt.tight_layout()
    plt.xlim(-10, 10)
    #plt.show()

# ++++++++++++++++++++++++++++++++ Gaussian Fit for TimeDiff_0_2 +++++++++++++++++++++++++++++++
def plot_timediff_0_2_fit(df_grouped_energy_list, colors):
    plt.figure(figsize=(10, 6))
    y_mean_list = [0] * len(df_grouped_energy_list)
    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        if "TimeDiff_0_2" in df_grouped_energy.columns:
            time_diff_0_2 = df_grouped_energy["TimeDiff_0_2"].dropna().values / 1000.0
            if len(time_diff_0_2) > 0:
                counts, bins, _ = plt.hist(time_diff_0_2, bins=100, histtype='step',
                                           alpha=0.6, label=f'File {file_idx + 1}',
                                           color=colors[file_idx % len(colors)], density=True)
                bin_centers = 0.5 * (bins[:-1] + bins[1:])
                p0 = [np.max(counts), np.mean(time_diff_0_2), np.std(time_diff_0_2)]
                try:
                    popt, _ = curve_fit(gaussian, bin_centers, counts, p0=p0)
                    x_fit = np.linspace(bin_centers[0], bin_centers[-1], 1000)
                    y_fit = gaussian(x_fit, *popt)
                    plt.plot(x_fit, y_fit, linestyle='--', color=colors[file_idx % len(colors)],
                             label=f'Fit File {file_idx + 1}: μ={popt[1]:.2f}, σ={popt[2]:.2f}')
                    y_mean_list[file_idx] = float(popt[1])
                except RuntimeError:
                    print(f"⚠️ Gaussian fit failed for TimeDiff_0_2 in File {file_idx + 1}")

    plt.xlabel("Time Difference (timestamp units / 1000)",fontsize=12)
    plt.ylabel("Normalized Counts",fontsize =12)
    plt.xticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.yticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.title("TimeDiff_0_2 with Gaussian Fit", fontsize =12)
    plt.legend(fontsize=12)
    plt.grid(True)
    plt.tight_layout()
    plt.xlim(-10, 10)
    #plt.show()

    # Final means printout
    print(f"\n{'TimeDiff_0_2 Means':>20}")
    for y in y_mean_list:
        print(f"{y:>20.4f}")

    #return y_mean_list, log_q0_q2_mean_list