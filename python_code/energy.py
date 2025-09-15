# ALSO HERE IN THIS FILE WE R DEFINING A FUNCTION 
# WE ARE SIMPLY JUST PLOTS HISTOGRAM FOR ENGERY, RATIO OF ENERGY 
# THEN LOG ENERGY OF 0_2 THEN FITTED IT 

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

# Gaussian function for fitting
def gaussian(x, amp, mu, sigma):
    return amp * np.exp(-(x - mu) ** 2 / (2 * sigma ** 2))

def plot_energy_all_histograms(E_list, allowed_channels_list, colors):
    plt.figure(figsize=(10, 6))
    for file_idx, E in enumerate(E_list):
        allowed_channels = allowed_channels_list[file_idx]
        for ch in allowed_channels:
            if ch in E and len(E[ch]) > 0:
                plt.hist(E[ch], bins=500, histtype='step', alpha=0.8,
                         label=f'File {file_idx + 1} - {ch}',
                         color=colors[file_idx % len(colors)])
    plt.xlabel("Energy (ADC units or keV)", fontsize=12)
    plt.ylabel("Counts", fontsize=12)
    plt.yscale("log")
    plt.xticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.yticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.legend(fontsize=12)
    plt.title("Energy Histograms for Square Slab, 1st Bar, 2nd Bar", fontsize=12)
    plt.grid(True)
    plt.tight_layout()
   #plt.show()


def plot_energy_ratio_histograms(df_grouped_energy_list, colors):
    plt.figure(figsize=(10, 6))
    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        ratio_cols = [col for col in df_grouped_energy.columns if col.startswith("Ratio_")]
        for r_idx, col in enumerate(ratio_cols):
            data = df_grouped_energy[col].dropna()
            if len(data) > 0:
                color = colors[(file_idx * len(ratio_cols) + r_idx) % len(colors)]
                plt.hist(data, bins=100, histtype='step', alpha=0.7,
                         label=f'File {file_idx + 1} - {col}',
                         color=color, density=True)
    plt.xlabel("Energy Ratio", fontsize=12)
    plt.ylabel("Counts", fontsize=12)
    plt.xticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.yticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.title("Energy Ratio Histograms (All Files)", fontsize=12)
    plt.legend(fontsize='small', ncol=2)
    plt.grid(True)
    plt.yscale("log")
    plt.tight_layout()
   # plt.show()


def plot_log_q0_q2_histograms(df_grouped_energy_list, colors, log_q0_q2_mean_list):
    plt.figure(figsize=(10, 6))
    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        if "Ch_0" in df_grouped_energy.columns and "Ch_2" in df_grouped_energy.columns:
            q0 = df_grouped_energy["Ch_0"]
            q2 = df_grouped_energy["Ch_2"]
            valid_mask = (q0 > 0) & (q2 > 0)
            log_q0_q2 = np.log(q0[valid_mask] / q2[valid_mask])
            if len(log_q0_q2) == 0:
                continue

            counts, bins, _ = plt.hist(log_q0_q2, bins=100, histtype='step',
                                      alpha=0.6,
                                      label=f'File {file_idx + 1}',
                                      color=colors[file_idx % len(colors)],
                                      density=True)
            bin_centers = 0.5 * (bins[:-1] + bins[1:])
            p0 = [np.max(counts), np.mean(log_q0_q2), np.std(log_q0_q2)]

            try:
                popt, _ = curve_fit(gaussian, bin_centers, counts, p0=p0)
                x_fit = np.linspace(bin_centers[0], bin_centers[-1], 1000)
                y_fit = gaussian(x_fit, *popt)
                plt.plot(x_fit, y_fit, linestyle='--', color=colors[file_idx % len(colors)],
                         label=f'Fit File {file_idx + 1}: μ={popt[1]:.3f}, σ={popt[2]:.3f}')
                log_q0_q2_mean_list.append(float(popt[1]))
            except RuntimeError:
                print(f"⚠️ Gaussian fit failed for log(Ch_0/Ch_2) in File {file_idx + 1}")

    plt.xlabel("log(Energy Ratio)", fontsize=12)
    plt.ylabel("Normalized Counts", fontsize=12)
    plt.title("log(Ch_0/Ch_2) Energy Ratio Histograms with Gaussian Fit", fontsize=12)
    plt.xticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.yticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.grid(True)
    plt.legend(fontsize=12)
    plt.tight_layout()
   # plt.show()
