# ALSO HERE IN THIS FILE WE R DEFINING A FUNCTION 
# THEN HERE WE R APPLY 3RD ORDER POLYNOMIAL TO GET THE POSITION 
# HAVE TO MENTION THE MEAN VALUES HARDCORE (EDIT THE CODE ) 
# THEN THE POSITION PLOTS WILL GET DIPLAYED (LOG CHARGE 0_2 AND TIME DIFF 0_2)

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

# Gaussian function for fitting
def gaussian(x, amp, mu, sigma):
    return amp * np.exp(-(x - mu) ** 2 / (2 * sigma ** 2))

def plot_polyfit_both(x_mean_list, y_mean_list, log_q0_q2_mean_list, colors):
   # x_mean_list, y_mean_list, log_q0_q2_mean_list = run_analysis()
    
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
   # bold_font = fm.FontProperties(weight='bold')
    plt.scatter(mean_values_time_diff, positions, color='blue', label="Data Point")
    plt.plot(x_vals_time_diff, y_vals_time_diff,'r' , label='$3^{rd}$ Order \n Poly.fit')
    plt.xlabel(r"$\langle \Delta T_{0 2} \rangle$", fontsize=25, fontweight='bold', fontname='Times New Roman')
    plt.ylabel("Position (cm)", fontsize =25)
    #plt.title("3rd Order Polynomial Fit: Position vs. TimeDiff_0_2")
    #plt.xlim(-20, 20)
    plt.xticks(np.arange(-3,4,2),fontsize=20, fontname='Times New Roman')
    plt.yticks(fontsize=20, fontname='Times New Roman')
   # plt.ylim(-20, 20)
    #plt.yticks(fontsize=25, fontname='Times New Roman')
    plt.legend(fontsize=20,framealpha =0.2, loc='upper left')
    plt.grid(True)
    plt.tight_layout()
    #plt.show()

# === Plot polynomial fit for log(Ch_0 / Ch_2) separately ===
    x_vals_log = np.linspace(min(mean_values_log_q0_q2), max(mean_values_log_q0_q2), 50)
    y_vals_log = poly_fit_log(x_vals_log)
     
    plt.figure(figsize=(5, 5))
    #bold_font = fm.FontProperties(weight='bold')
    plt.scatter(mean_values_log_q0_q2, positions, color='green', label="Data Point")
    plt.plot(x_vals_log, y_vals_log,'r',label='$3^{rd}$ Order\n Poly.fit')
    plt.xlabel(r" $\langle\ln\left(\frac{Q_0}{Q_2}\right)\rangle $" , fontsize=25, fontname='Times New Roman')

    plt.ylabel("Position (cm)", fontsize =25)
    #plt.title("3rd Order Polynomial Fit: Position vs. (Ch_0 / Ch_2)")
    #plt.xlim(-20, 20)
    plt.xticks( np.arange(-0.8,0.8,0.4),fontsize=20, fontname='Times New Roman')
    plt.yticks(fontsize=20, fontname='Times New Roman')
    #plt.ylim(-20, 20)
    #plt.yticks(fontsize=30, fontname='Times New Roman')
    plt.legend(fontsize=20, framealpha =0.2, loc='upper right')
    plt.grid(True)
    plt.tight_layout()
   # plt.show()
