from refactored_m4 import *
import matplotlib.pyplot as plt
import numpy as np
import os
from scipy.optimize import curve_fit

# Gaussian function for fitting
def gaussian(x, amp, mu, sigma):
    return amp * np.exp(-(x - mu) ** 2 / (2 * sigma ** 2))

# Initialize global lists for means
x_mean_list = []  # For TimeDiff_1_3 means
y_mean_list = []  # For TimeDiff_0_2 means
log_q0_q2_mean_list = []

def collect_root_files(inputs):
    """
    Given a list of inputs (folders or root file paths),
    returns list of all .root files found.
    """
    root_files = []
    for entry in inputs:
        if os.path.isdir(entry):
            for fname in os.listdir(entry):
                if fname.endswith(".root"):
                    root_files.append(os.path.join(entry, fname))
        elif os.path.isfile(entry) and entry.endswith(".root"):
            root_files.append(entry)
        else:
            raise ValueError(f"Input '{entry}' is neither a folder nor a .root file")
    if not root_files:
        raise FileNotFoundError("❌ No .root files found in the specified inputs")
    return root_files

def run_analysis():
    # List your inputs here (folders or individual root files)
    inputs = [
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_0_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_+5_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_10_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_+15_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_20_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-5_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-10_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-15_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-20_30min.root"
    ]

    # Collect all root files from the inputs
    file_list = collect_root_files(inputs)
    print(f"Found {len(file_list)} root files from {len(inputs)} inputs.")

    allowed_channels_list = [
        get_channels_from_config(["square_slab", "1st_bar", "2nd_bar"]) for _ in file_list
    ]

    E_list = []
    df_grouped_energy_list = []

    for counter, filename in enumerate(file_list):
        print("==== Processing file:", filename)
        E, df_grouped_energy, x_mean_local, y_mean_local = ProcessData(filename, allowed_channels_list[counter])
        E_list.append(E)
        df_grouped_energy_list.append(df_grouped_energy)

        x_mean_list.append(x_mean_local)
        y_mean_list.append(y_mean_local)

    # Plot Energy Ratio Histograms
    plt.figure(figsize=(12, 6))
    colors = ['red', 'green', 'blue', 'brown', 'purple', 'cyan', 'yellow', 'black']
    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        ratio_cols = [col for col in df_grouped_energy.columns if col.startswith("Ratio_")]
        for r_idx, col in enumerate(ratio_cols):
            data = df_grouped_energy[col].dropna()
            if len(data) > 0:
                color = colors[(file_idx * len(ratio_cols) + r_idx) % len(colors)]
                plt.hist(data, bins=100, histtype='step', alpha=0.7,
                         label=f'File {file_idx + 1} - {col}',
                         color=color, density=True)
    plt.xlabel("Energy Ratio")
    plt.ylabel("Normalized Counts")
    plt.title("Energy Ratio Histograms (All Files)")
    plt.legend(fontsize='small', ncol=2)
    plt.grid(True)
    plt.tight_layout()
    plt.yscale("log")
    # plt.show()

    # log(Q0/Q2) Histogram with Gaussian fit
    plt.figure(figsize=(12, 6))
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

    plt.xlabel("log(Energy Ratio)")
    plt.ylabel("Normalized Counts")
    plt.title("log(Ch_0/Ch_2) Energy Ratio Histograms with Gaussian Fit")
    plt.grid(True)
    plt.legend(fontsize='small', ncol=2)
    plt.tight_layout()
    # plt.show()

    # Combined Energy Histograms
    plt.figure(figsize=(12, 6))
    for file_idx, E in enumerate(E_list):
        allowed_channels = allowed_channels_list[file_idx]
        for ch in allowed_channels:
            if ch in E and len(E[ch]) > 0:
                plt.hist(E[ch], bins=500, histtype='step', alpha=0.8,
                         label=f'File {file_idx + 1} - {ch}')
    plt.xlabel("Energy (ADC units or keV)")
    plt.ylabel("Counts")
    plt.yscale("log")
    plt.legend(fontsize='small', ncol=2)
    plt.title("Energy Histograms for Square Slab, 1st Bar, 2nd Bar")
    plt.grid(True)
    plt.tight_layout()
    # plt.show()

    # TimeDiff_0_2 Histogram (no fit)
    plt.figure(figsize=(10, 6))
    for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
        if "TimeDiff_0_2" in df_grouped_energy.columns:
            time_diff_0_2 = df_grouped_energy["TimeDiff_0_2"].dropna().values / 1000.0
            if len(time_diff_0_2) > 0:
                plt.hist(time_diff_0_2, bins=100, histtype='step', alpha=0.7,
                         label=f'File {file_idx + 1} - TimeDiff_0_2',
                         density=True, linestyle='--')

    plt.xlabel("Time Difference (timestamp units / 1000)")
    plt.ylabel("Normalized Counts")
    plt.title("TimeDiff_0_2 Histograms (No Fit)")
    plt.legend(fontsize='small', ncol=2)
    plt.grid(True)
    plt.tight_layout()
    plt.xlim(-10, 10)
    # plt.show()

    # Gaussian Fit for TimeDiff_0_2
    plt.figure(figsize=(10, 6))
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

    plt.xlabel("Time Difference (timestamp units / 1000)")
    plt.ylabel("Normalized Counts")
    plt.title("TimeDiff_0_2 with Gaussian Fit")
    plt.legend(fontsize='small', ncol=2)
    plt.grid(True)
    plt.tight_layout()
    plt.xlim(-10, 10)
    # plt.show()

    # Final means printout
    print(f"\n{'TimeDiff_0_2 Means':>20}")
    for y in y_mean_list:
        print(f"{y:>20.4f}")

    return x_mean_list, y_mean_list, log_q0_q2_mean_list


if __name__ == "__main__":
    x_mean_list, y_mean_list, log_q0_q2_mean_list = run_analysis()

    # === Hardcoded means ===
    mean_values_time_diff = [-3.20, -2.54, -2.21, -1.17, -0.25, 0.66, 1.65, 2.24, 2.66]
    mean_values_log_q0_q2 = [0.637, 0.474, 0.370, 0.115, -0.079, -0.266, -0.504, -0.657, -0.743]

    # Positions for calibration (mm)
    positions = [-20, -15, -10, -5, 0, 5, 10, 15, 20]

    # === Polynomial fit for TimeDiff_0_2 ===
    coeffs_time_diff = np.polyfit(mean_values_time_diff, positions, 3)
    poly_fit_time_diff = np.poly1d(coeffs_time_diff)

    print("\n================== 3rd Order Polynomial Coefficients (TimeDiff_0_2) ==================")
    print(f"a (x^3): {coeffs_time_diff[0]}")
    print(f"b (x^2): {coeffs_time_diff[1]}")
    print(f"c (x^1): {coeffs_time_diff[2]}")
    print(f"d (const): {coeffs_time_diff[3]}")
    print("========================================================================")

    # === Polynomial fit for log(Ch_0 / Ch_2) ===
    coeffs_log = np.polyfit(mean_values_log_q0_q2, positions, 3)
    poly_fit_log = np.poly1d(coeffs_log)

    print("\n================== 3rd Order Polynomial Coefficients (log(Ch_0/Ch_2)) ==================")
    print(f"a (x^3): {coeffs_log[0]}")
    print(f"b (x^2): {coeffs_log[1]}")
    print(f"c (x^1): {coeffs_log[2]}")
    print(f"d (const): {coeffs_log[3]}")
    print("========================================================================")

    # === Plot polynomial fit for TimeDiff_0_2 separately ===
    x_vals_time_diff = np.linspace(min(mean_values_time_diff), max(mean_values_time_diff), 50)
    y_vals_time_diff = poly_fit_time_diff(x_vals_time_diff)

    plt.figure(figsize=(5, 5))
    plt.scatter(mean_values_time_diff, positions, color='blue', label=r"$\langle \Delta T_{0 2} \rangle$")
    plt.plot(x_vals_time_diff, y_vals_time_diff, 'b--', label='3rd Order Polynomial Fit (TimeDiff_0_2)')
    plt.xlabel(r"$\langle \Delta T_{0 2} \rangle$", fontsize=15, fontweight='bold', fontname='Times New Roman')
    plt.ylabel("Position (cm)", fontsize =15)
    #plt.title("3rd Order Polynomial Fit: Position vs. TimeDiff_0_2")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    #plt.show()

    # === Plot polynomial fit for log(Ch_0 / Ch_2) separately ===
    x_vals_log = np.linspace(min(mean_values_log_q0_q2), max(mean_values_log_q0_q2), 50)
    y_vals_log = poly_fit_log(x_vals_log)

    plt.figure(figsize=(5, 5))
    plt.scatter(mean_values_log_q0_q2, positions, color='green', label=r" $\langle\ln\left(\frac{Q_0}{Q_2}\right)\rangle $")
    plt.plot(x_vals_log, y_vals_log, 'g--', label='3rd Order Polynomial Fit (log ratio)')
    plt.xlabel(r" $\langle\ln\left(\frac{Q_0}{Q_2}\right)\rangle $" , fontsize=15, fontweight='bold', fontname='Times New Roman')

    plt.ylabel("Position (cm)", fontsize =15)
    #plt.title("3rd Order Polynomial Fit: Position vs. (Ch_0 / Ch_2)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    #plt.show()

    # === Predict positions for separate file ===
    separate_file = "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_0_30min.root"

    # Process separately
    allowed_channels_sep = get_channels_from_config(["square_slab", "1st_bar", "2nd_bar"])
    E_sep, df_grouped_energy_sep, x_mean_sep, y_mean_sep = ProcessData(separate_file, allowed_channels_sep)

    # Predict position using polynomial fits
    predicted_pos_time_diff = float(poly_fit_time_diff(y_mean_sep))

    log_ratio_mean = np.mean(np.log(df_grouped_energy_sep["Ch_0"] / df_grouped_energy_sep["Ch_2"]))
    predicted_pos_log = float(poly_fit_log(log_ratio_mean))

    print(f"\nPredicted position using TimeDiff_0_2 mean: {predicted_pos_time_diff:.2f} mm")
    print(f"Predicted position using log(Ch_0/Ch_2) mean: {predicted_pos_log:.2f} mm")


    # Predict from log(Ch_0/Ch_2)
    if ("Ch_0" in df_grouped_energy_sep.columns) and ("Ch_2" in  df_grouped_energy_sep.columns):
        q0_sep =  df_grouped_energy_sep["Ch_0"].dropna()
        q2_sep =  df_grouped_energy_sep["Ch_2"].dropna()
        valid_mask_sep = (q0_sep > 0) & (q2_sep > 0)
        log_vals_sep = np.log(q0_sep[valid_mask_sep] / q2_sep[valid_mask_sep])

        predicted_positions_log = poly_fit_log(log_vals_sep)

        print("=============================================")
        print(f"Predicted positions from log(Ch_0/Ch_2) for file: {separate_file}")
        print("=============================================")
        for idx, pos in enumerate(predicted_positions_log[:20]):
            print(f"Event {idx + 1}: Position = {pos:.3f} mm")

        plt.figure(figsize=(10, 6))
        plt.hist(predicted_positions_log, bins=100, histtype='step', color='green')
        plt.xlabel("Predicted Position (mm)")
        plt.ylabel("Counts")
        plt.title(f"Predicted Positions from log(Ch_0 / Ch_2) ({separate_file})")
        plt.grid(True)
        plt.tight_layout()
        plt.show()

        print(f"Mean predicted position (log ratio): {np.mean(predicted_positions_log):.3f} mm")
        print(f"Std dev predicted position (log ratio): {np.std(predicted_positions_log):.3f} mm")

    else:
        print(f"⚠️ 'Ch_0' or 'Ch_2' not found in file: {separate_file}")
