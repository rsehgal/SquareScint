from refactored_m4 import *
#list of files to be processed
#file_list=["/data/07_Aug_Na22_Cs_Muon/DataF_Cs137_CFD_th_8_8_9_9_Slab_7Aug_-0,0.root",
#"/data/07_Aug_Na22_Cs_Muon/DataF_Na22_CFD_th_8_8_9_9_Slab_7Aug_0_0.root","/data/07_Aug_Na22_Cs_Muon/DataF_Muon_CFD_th_8_8_9_9_Slab_7Aug_0_0.root"]
#allowed_channels_list=[get_channels_from_config(["square_slab"]),get_channels_from_config(["square_slab"]),get_channels_from_config(["square_slab","1st_bar","2nd_bar"])]
file_list=["/data/Aug08_CFD/DataF_Muon_CFD_6_75_th_8_8_9_9_Slab_8Aug_0_0.root","/data/Aug08_CFD/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_8Aug_10_0.root",
"/data/Aug08_CFD/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_8Aug_0_10.root","/data/Aug08_CFD/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_8Aug_0_20.root","/data/Aug08_CFD/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_8Aug_20_0.root"]
allowed_channels_list=[get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),
get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),get_channels_from_config(["square_slab","1st_bar","2nd_bar"]),get_channels_from_config(["square_slab","1st_bar","2nd_bar"])]

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
plt.figure(figsize=(10, 6))
colors = ['red', 'blue', 'green', 'orange', 'purple']
color_idx = 0

for df_grouped_energy in df_grouped_energy_list:
    print("=========================")
    time_diff_columns = [col for col in df_grouped_energy.columns if col.startswith("TimeDiff_") and len(df_grouped_energy[col]) > 10]
    print(time_diff_columns)
    counter += 1
    counts, bin_edges, _ = plt.hist(df_grouped_energy["TimeDiff_0_2"].values, bins=100, histtype='step', alpha=0.7,
                                label=f'{"muon_00", "muon_10_0", "muon_0_10"} Data', color=colors[counter % len(colors)],density=True)

    bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2

    # Fit Gaussian
    data = df_grouped_energy["TimeDiff_0_2"].dropna().values.astype(float)
    A_guess = counts.max()
    mu_guess = np.mean(data)
    sigma_guess = np.std(data)

    try:
        popt, _ = curve_fit(gaussian, bin_centers, counts, p0=[A_guess, mu_guess, sigma_guess])
        A_fit, mu_fit, sigma_fit = popt

        print(f"✅ Fit {counter+1}: mu = {mu_fit:.4f}, sigma = {sigma_fit:.4f}")

        # Plot the fitted Gaussian
        x_fit = np.linspace(bin_edges[0], bin_edges[-1], 1000)
        y_fit = gaussian(x_fit, *popt)
        plt.plot(x_fit, y_fit, '--', color=colors[counter % len(colors)],
                 label=f'Gaussian Fit {counter+1}\nμ={mu_fit:.4f}, σ={sigma_fit:.4f}')

    except RuntimeError:
        print(f"⚠️ Gaussian fit failed for dataset {counter+1}")

    counter += 1

plt.xlabel("Time Difference (timestamp units)")
plt.ylabel("Normalized Counts")
plt.title("Time Difference Histograms with Gaussian Fits")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.xlim(-20000, 20000)
plt.show()