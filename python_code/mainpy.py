from refactored_m4 import *

#list of files to be processed

#def paramEqu(coeffs,filename):
   # outy = []
    #Read filename and get delt event by event
    #allow_channels=get_channels_from_config(["square_slab","1st_bar","2nd_bar"])
    #E,df_grouped_energy=ProcessData(filename,allowed_channels)
    #deltL = df_grouped_energy["TimeDiff_0_2"]/1000.
    #print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@   RAMAN   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
    #print(deltL)
    #for delt in deltL:
     #   outy.append(coeffs[0]+coeffs[1]*delt+coeffs[2]*(delt**2)+coeffs[3]*(delt**3))
    #plt.hist(outy,histtype="step", color="red")
    #plt.show()



file_list=[
    "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_0_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_0_30min.root",
    "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_10_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_10_30min.root",
    "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_20_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_20_30min.root",
    "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_+5_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_+5_30min.root",
     "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_+15_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_+15_30min.root",
     "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-5_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-5_30min.root",
     "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-10_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-10_30min.root",
     "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-15_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-15_30min.root",
     "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-20_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-20_30min.root"
    ]
allowed_channels_list=[
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]), 
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]), 
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]), 
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),
    get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),  

    ]

print(type(allowed_channels_list))



#allowed_channels_list=[get_channels_from_config(["square_slab"])]
#file_list = [
 #"/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-10_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-10_30min.root"
#]

#allowed_channels_list = [
 #   get_channels_from_config(["square_slab", "1st bar", "2nd bar"]),
#]

#print(type(allowed_channels_list))

E_list=[]
df_grouped_energy_list = []

plt.figure(figsize=(10, 5))
colors = ['blue', 'green', 'red', 'orange', 'black', 'purple', 'brown', 'pink']

counter=0
for filename in file_list:
    print("==== Processig file : ",filename)
    E,df_grouped_energy=ProcessData(filename,allowed_channels_list[counter])
    E_list.append(E)
    df_grouped_energy_list.append(df_grouped_energy)
    counter += 1

counter=0



# Optional: plot histogram

#=======================ploting energy histogram=======================

for i, E in enumerate(E_list):
    allowed_channels = allowed_channels_list[i]
    plt.figure(figsize=(10,6))
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
 #   plt.show()

#================energy ratio plot for all dataset=================

# ========== Energy Ratio Histogram Plots for All Datasets ==========

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
plt.yscale("log")  # Optional: remove for linear scale
#plt.show()




# ========= Energy Histogram Plottingall the dataset combined========= #



plt.figure(figsize=(12, 6))
colors = ['blue', 'green', 'red', 'orange', 'purple', 'cyan', 'magenta', 'brown']

for file_idx, E in enumerate(E_list):
    allowed_channels = allowed_channels_list[file_idx]
    for ch_idx, ch in enumerate(allowed_channels):
        if ch in E and len(E[ch]) > 0:
            color_idx = (file_idx * len(allowed_channels) + ch_idx) % len(colors)
            plt.hist(E[ch], bins=500, histtype='step', alpha=0.8,
                     label=f'File {file_idx+1} - {ch}', color=colors[color_idx])

plt.xlabel("Energy (ADC units or keV)")
plt.ylabel("Counts")
plt.yscale("log")  # Log scale for better visibility
plt.legend(fontsize='small', ncol=2)
plt.title("Energy Histograms for Square Slab, 1st Bar, 2nd Bar (dataset)")
plt.grid(True)
plt.tight_layout()
#plt.show()




#===========time diff =====

plt.figure(figsize=(10, 6))
colors = ['red', 'blue', 'green', 'orange', 'purple', 'cyan', 'magenta', 'brown']

for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
    if "TimeDiff_0_2" in df_grouped_energy.columns:
        time_diff_data = df_grouped_energy["TimeDiff_0_2"].dropna().values / 1000.0
        if len(time_diff_data) > 0:
            plt.hist(time_diff_data, bins=100, histtype='step', alpha=0.7,
                     label=f'File {file_idx+1} - TimeDiff_0_2',
                     color=colors[file_idx % len(colors)], density=True)

plt.xlabel("Time Difference (timestamp units / 1000)")
plt.ylabel("Normalized Counts")
plt.title("TimeDiff_0_2 Histograms (No Fit, Scaled)")
plt.legend(fontsize='small', ncol=2)
plt.grid(True)
plt.tight_layout()
plt.xlim(-10, 10)
plt.yscale("log")  # Remove if you want linear scale
#plt.show()

#---------------------gaussian fit-----------------
plt.figure(figsize=(10, 6))
colors = ['red', 'blue', 'green', 'orange', 'purple', 'cyan', 'magenta', 'brown']

for file_idx, df_grouped_energy in enumerate(df_grouped_energy_list):
    if "TimeDiff_0_2" in df_grouped_energy.columns:
        # Get cleaned and scaled time difference data
        time_diff_data = df_grouped_energy["TimeDiff_0_2"].dropna().values / 1000.0
        
        if len(time_diff_data) > 0:
            # Plot histogram (normalized)
            counts, bins, _ = plt.hist(
                time_diff_data, bins=100, histtype='step', alpha=0.6,
                label=f'File {file_idx+1} - Data',
                color=colors[file_idx % len(colors)],
                density=True
            )

            # Prepare data for Gaussian fit
            bin_centers = 0.5 * (bins[:-1] + bins[1:])
            A_guess = np.max(counts)
            mu_guess = np.mean(time_diff_data)
            sigma_guess = np.std(time_diff_data)
            p0 = [A_guess, mu_guess, sigma_guess]

            try:
                # Perform Gaussian fit
                popt, _ = curve_fit(gaussian, bin_centers, counts, p0=p0)
                A_fit, mu_fit, sigma_fit = popt

                # Generate smooth curve for fit
                x_fit = np.linspace(bin_centers[0], bin_centers[-1], 1000)
                y_fit = gaussian(x_fit, *popt)

                # Plot Gaussian fit
                plt.plot(
                    x_fit, y_fit, linestyle='--',
                    color=colors[file_idx % len(colors)],
                    label=f'Fit {file_idx+1}: μ={mu_fit:.2f}, σ={sigma_fit:.2f}'
                )
            except RuntimeError:
                print(f"⚠️ Gaussian fit failed for File {file_idx+1}")

# Final plot settings
plt.xlabel("Time Difference (timestamp units / 1000)")
plt.ylabel("Normalized Counts")
plt.title("TimeDiff_0_2 Histograms with Gaussian Fit")
plt.legend(fontsize='small', ncol=2)
plt.grid(True)
plt.tight_layout()
plt.xlim(-10, 10)
#plt.yscale("log")  # Remove this if linear scale is preferred
#plt.show()

#----------------------------3rd order ------------------------

# === Step 1: Hardcoded values ===
# Replace with your actual fitted means from the TimeDiff_0_2 Gaussian fits
mean_values = [-3.20,-2.54,-2.21,-1.17,0.25,0.66,1.65,2.24,2.66]  # X-axis
positions = [-20, -15, -10, -5, 0, 5, 10, 15, 20]                # Y-axis

# === Step 2: Fit a 3rd order polynomial ===
coeffs = np.polyfit(mean_values, positions, 3)
poly_fit = np.poly1d(coeffs)

# === Step 3: Display polynomial coefficients ===
print("===================================================")
print("3rd Order Polynomial Coefficients:")
print(f"a (const): {coeffs[0]}")
print(f"b (x¹): {coeffs[1]}")
print(f"c (x²): {coeffs[2]}")
print(f"d (x³): {coeffs[3]}")
print("=====================================================")




# === Step 4: Plot ===
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


# Specify the new file path
separate_file = "/data/WaveAnalysis/Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-15_30min/FILTERED/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_-15_30min.root"  

# Specify allowed channels
separate_allowed_channels = get_channels_from_config(["square_slab", "1st_bar", "2nd_bar"])

# Process the file to get energy and grouped DataFrame
E_sep, df_grouped_sep = ProcessData(separate_file, separate_allowed_channels)

# === Predict positions from the TimeDiff_0_2 of the separate file ===

if "TimeDiff_0_2" in df_grouped_sep.columns:
    delt_values = df_grouped_sep["TimeDiff_0_2"].dropna().values / 1000.0
    
    # Apply 3rd order polynomial to each time diff to get predicted positions
    predicted_positions = poly_fit(delt_values)
    
    print("=============================================")
    print(f"Predicted positions from TimeDiff_0_2 for file: {separate_file}")
    print("=============================================")
    
    for idx, pos in enumerate(predicted_positions):
        print(f"Event {idx+1}: Position = {pos:.3f} mm")
    
    # Optional: Plot histogram of predicted positions
    plt.figure(figsize=(10, 6))
    plt.hist(predicted_positions, bins=100, histtype='step', color='red')
    plt.xlabel("Predicted Position (mm)")
    plt.ylabel("Counts")
    plt.title(f"Predicted Positions from TimeDiff_0_2 ({separate_file})")
    plt.grid(True)
    print(np.mean(predicted_positions))
    print(np.std(predicted_positions))
    plt.tight_layout()
    plt.show()
else:

    print(f"⚠️ 'TimeDiff_0_2' not found in file: {separate_file}")


