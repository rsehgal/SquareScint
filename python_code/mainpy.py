 # here in this file / code 
 # we are reading all the files and also calling all the functions defined 
 # JUST RUN THIS FILE AND ALL THE FUCTIONS WE GET CALLED AND ALL THE PLOTS WILL GET DISPLAYED

from refactored_m4 import *  # your data processing code and ProcessData etc.
import os

def collect_root_files(inputs):
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
    # Import plots here to avoid circular import problems
    from energy import plot_energy_all_histograms, plot_energy_ratio_histograms, plot_log_q0_q2_histograms
    from timediff import plot_timediff_0_2, plot_timediff_0_2_fit
    from poly_fit import plot_polyfit_both
    from predict_plot import predict_poly
    
    inputs = [
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_0_30min.root",
       "/home/harshita/shared/SquareScint_Harshita/x0 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_0_+5_30min.root",
       # add the rest of your input files...
    ]

    file_list = collect_root_files(inputs)
    print(f"Found {len(file_list)} root files from {len(inputs)} inputs.")

    allowed_channels_list = [
        get_channels_from_config(["square_slab", "1st_bar", "2nd_bar"]) for _ in file_list
    ]

    E_list = []
    df_grouped_energy_list = []

    x_mean_list = []  # (optional if you use them)
    y_mean_list = []  # (optional)
    log_q0_q2_mean_list = []

    for counter, filename in enumerate(file_list):
        print(f"==== Processing file: {filename}")
        E, df_grouped_energy, x_mean_local, y_mean_local = ProcessData(filename, allowed_channels_list[counter])
        E_list.append(E)
        df_grouped_energy_list.append(df_grouped_energy)
        x_mean_list.append(x_mean_local)
        y_mean_list.append(y_mean_local)

    colors = ['red', 'green', 'blue', 'brown', 'purple', 'cyan', 'yellow', 'black']

    # Call the plotting functions with all required data passed explicitly
    plot_energy_all_histograms(E_list, allowed_channels_list, colors)
    plot_energy_ratio_histograms(df_grouped_energy_list, colors)
    plot_log_q0_q2_histograms(df_grouped_energy_list, colors, log_q0_q2_mean_list)
    plot_timediff_0_2(df_grouped_energy_list, colors)
    plot_timediff_0_2_fit(df_grouped_energy_list, colors)
    plot_polyfit_both(x_mean_list, y_mean_list, log_q0_q2_mean_list, colors)
    predict_poly ()


    # You can also print or return summary stats if you want
    print("log_q0_q2_mean_list:", log_q0_q2_mean_list)

if __name__ == "__main__":
    run_analysis()


