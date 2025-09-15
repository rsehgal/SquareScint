# HERE IN THIS FILE / CODE WE DEFINING A FUCTION AND
# THEN CALLING THIS FUCTIONS THROUGTH MAINPY.PY FILE 
# ALSO THIS FILE WILL SHOW THE PREDICT PLOTS OF LOG CHARGE 0_2 AND TIMEDIFF 0_2

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def get_channels_from_config(channel_list):
    # Placeholder for missing config_utils function
    return channel_list

# Updated dummy ProcessData with centered TimeDiff_0_2
def ProcessData(filename, allowed_channels):
    data = {
        "Ch_0": np.random.uniform(1, 100, 1000),
        "Ch_2": np.random.uniform(1, 100, 1000),
        "TimeDiff_0_2": np.random.normal(0, 200, 1000),  # mean = 0 ns, std = 200 ns
    }
    df = pd.DataFrame(data)
    df_grouped_energy = df
    x_mean = np.random.uniform(-10, 10)
    y_mean = np.random.uniform(-10, 10)
    E = np.random.uniform(0, 100)
    return E, df_grouped_energy, x_mean, y_mean

# Linear fit: assuming speed-based mapping (e.g., 0.15 mm/ns for a sample detector)
def poly_fit_time_diff(x):
    return np.array(x) * 0.15  # position = velocity * time_diff

def poly_fit_log(log_vals):
    return np.array(log_vals) * 50  # just some dummy scale to spread values

def predict_poly():
    separate_file = "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_0_30min.root"

    allowed_channels_sep = get_channels_from_config(["square_slab", "1st_bar", "2nd_bar"])
    E_sep, df_grouped_energy_sep, x_mean_sep, y_mean_sep = ProcessData(separate_file, allowed_channels_sep)

    predicted_pos_time_diff = float(poly_fit_time_diff(y_mean_sep))
    log_ratio_mean = np.mean(np.log(df_grouped_energy_sep["Ch_0"] / df_grouped_energy_sep["Ch_2"]))
    predicted_pos_log = float(poly_fit_log(log_ratio_mean))

    print(f"\nPredicted position using TimeDiff_0_2 mean: {predicted_pos_time_diff:.2f} mm")
    print(f"Predicted position using log(Ch_0/Ch_2) mean: {predicted_pos_log:.2f} mm")

    # -------- Predict from log(Ch_0 / Ch_2) --------
    if "Ch_0" in df_grouped_energy_sep.columns and "Ch_2" in df_grouped_energy_sep.columns:
        q0_sep = df_grouped_energy_sep["Ch_0"].dropna()
        q2_sep = df_grouped_energy_sep["Ch_2"].dropna()
        valid_mask_sep = (q0_sep > 0) & (q2_sep > 0)
        log_vals_sep = np.log(q0_sep[valid_mask_sep] / q2_sep[valid_mask_sep])
        predicted_positions_log = poly_fit_log(log_vals_sep)

        print("=============================================")
        print(f"Predicted positions from log(Ch_0/Ch_2) for file: {separate_file}")
        print("=============================================")
        for idx, pos in enumerate(predicted_positions_log[:20]):
            print(f"Event {idx + 1}: Position = {pos:.3f} mm")

        plt.figure(figsize=(5, 5))
        plt.hist(predicted_positions_log, bins=100, histtype='step', color='green')
        plt.xlabel("Predicted Position from log(Ch0/Ch2) (mm)", fontsize=15)
        plt.ylabel("Counts")
        plt.xticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
        plt.yticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
        plt.grid(True)
        plt.tight_layout()
        # plt.show()

        print(f"Mean predicted position (log ratio): {np.mean(predicted_positions_log):.3f} mm")
        print(f"Std dev predicted position (log ratio): {np.std(predicted_positions_log):.3f} mm")

    else:
        print(f"⚠️ 'Ch_0' or 'Ch_2' not found in file: {separate_file}")

    # -------- Predict from TimeDiff_0_2 --------
    if "TimeDiff_0_2" in df_grouped_energy_sep.columns:
        time_diff_sep = df_grouped_energy_sep["TimeDiff_0_2"].dropna().values  # already in ns
        predicted_positions_time_diff = poly_fit_time_diff(time_diff_sep)

        print("\n=============================================")
        print(f"Predicted positions from TimeDiff_0_2 for file: {separate_file}")
        print("=============================================")
        for idx, pos in enumerate(predicted_positions_time_diff[:20]):
            print(f"Event {idx + 1}: Position = {pos:.3f} mm")

        plt.figure(figsize=(5, 5))
        plt.hist(predicted_positions_time_diff, bins=100, histtype='step', color='blue')
        plt.xlabel("Predicted Position from TimeDiff_0_2 (mm)", fontsize=15)
        plt.ylabel("Counts")
        plt.xticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
        plt.yticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
        plt.grid(True)
        plt.tight_layout()
        #plt.show()

        print(f"Mean predicted position (TimeDiff): {np.mean(predicted_positions_time_diff):.3f} mm")
        print(f"Std dev predicted position (TimeDiff): {np.std(predicted_positions_time_diff):.3f} mm")

    else:
        print(f"⚠️ 'TimeDiff_0_2' not found in file: {separate_file}")

    # ==================== Combined Plot ====================
    plt.figure(figsize=(5, 5))

    if "Ch_0" in df_grouped_energy_sep.columns and "Ch_2" in df_grouped_energy_sep.columns:
        plt.hist(predicted_positions_log, bins=100, histtype='step', color='green',
                 label=r"$\ln(Q_0/Q_2)$-based Prediction")

    if "TimeDiff_0_2" in df_grouped_energy_sep.columns:
        plt.hist(predicted_positions_time_diff, bins=100, histtype='step', color='blue',
                 label=r"$\Delta T_{02}$-based Prediction")

    plt.xlabel("Predicted Position (mm)", fontsize=15, fontweight='bold', fontname='Times New Roman')
    plt.ylabel("Counts", fontsize=15, fontweight='bold', fontname='Times New Roman')
    plt.xticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.yticks(fontsize=10, fontweight='bold', fontname='Times New Roman')
    plt.legend(fontsize=15)
    plt.grid(True)
    plt.tight_layout()
    plt.show()

# To run:
#predict_poly()
