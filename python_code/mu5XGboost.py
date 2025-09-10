import uproot
import pandas as pd
import os
import joblib
import time
from xgboost import XGBRegressor
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
import seaborn as sns
from tqdm import tqdm

# Configuration
folder_paths = [
    '/home/harshita/shared/SquareScint_Harshita/x0 data file (copy)',
    '/home/harshita/shared/SquareScint_Harshita/x5 data file',
    '/home/harshita/shared/SquareScint_Harshita/x10 data file',
    '/home/harshita/shared/SquareScint_Harshita/x20 data file',
    '/home/harshita/shared/SquareScint_Harshita/x-10 data file',
    '/home/harshita/shared/SquareScint_Harshita/x-20 data file'
]

tree_name = 'Data_F'
features = ['Channel', 'Timestamp']
model_path = './saved_model/model.pkl'
scaler_path = './saved_model/scaler.pkl'


def extract_label_from_folder(folder_path):
    folder_name = os.path.basename(folder_path)
    for part in folder_name.split():
        if part.startswith('x'):
            try:
                return int(part.replace('x', ''))
            except ValueError:
                pass
    raise ValueError(f"Cannot extract label from folder: {folder_path}")


def fast_event_grouping(df, threshold=100):
    df = df.sort_values('Timestamp')
    df['event_id'] = (df['Timestamp'].diff().gt(threshold)).cumsum()
    return df


def process_event_time_differences(df, threshold=100):
    """
    Vectorized event grouping and pivot-based time difference calculation.
    """
    df['Timestamp'] = df['Timestamp'].astype(float)  # ✅ Prevent overflow
    df = fast_event_grouping(df, threshold)
    
    pivot = df.pivot(index='event_id', columns='Channel', values='Timestamp')
    pivot = pivot.dropna(subset=[0, 1, 2, 3])  # Ensure all channels exist

    pivot['time_diff_0_2'] = pivot[2] - pivot[0]
    pivot['time_diff_1_3'] = pivot[3] - pivot[1]

    return pivot[['time_diff_0_2', 'time_diff_1_3']].reset_index()


def load_data_from_multiple_folders(folder_paths):
    all_data = []
    for folder_path in tqdm(folder_paths, desc="📁 Loading folders"):
        if not os.path.exists(folder_path):
            print(f"[WARNING] Folder does not exist: {folder_path}")
            continue

        try:
            label = extract_label_from_folder(folder_path)
        except ValueError as e:
            print(f"[ERROR] {e}")
            continue

        for file in tqdm(os.listdir(folder_path), desc=os.path.basename(folder_path), leave=False):
            if file.endswith('.root'):
                file_path = os.path.join(folder_path, file)
                try:
                    with uproot.open(file_path) as f:
                        tree = f[tree_name]
                        df_raw = tree.arrays(features, library='pd')
                        features_df = process_event_time_differences(df_raw)
                        if not features_df.empty:
                            features_df['label'] = label
                            all_data.append(features_df)
                except Exception as e:
                    print(f"[ERROR] Failed to read {file_path}: {e}")
    if not all_data:
        raise ValueError("No valid data loaded.")
    return pd.concat(all_data, ignore_index=True)


def main():
    start_time = time.time()
    print("🔄 Loading data...")
    data = load_data_from_multiple_folders(folder_paths)
    print(f"✅ Data loaded: {data.shape} in {time.time() - start_time:.2f}s")

    data.dropna(inplace=True)

    X = data[['time_diff_0_2', 'time_diff_1_3']]
    y = data['label']

    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.2, random_state=42)

    print("🚀 Training XGBoost Regressor...")
    model = XGBRegressor(n_estimators=100, random_state=42)
    model.fit(X_train, y_train)

    y_pred = model.predict(X_test)
    print(f"📉 MSE: {mean_squared_error(y_test, y_pred):.4f}")
    print(f"📈 R² Score: {r2_score(y_test, y_pred):.4f}")

    os.makedirs(os.path.dirname(model_path), exist_ok=True)
    joblib.dump(model, model_path)
    joblib.dump(scaler, scaler_path)
    print(f"✅ Model & Scaler saved.")


def predict_on_new_file(file_path):
    print("📦 Loading model and scaler...")
    model = joblib.load(model_path)
    scaler = joblib.load(scaler_path)

    with uproot.open(file_path) as f:
        tree = f[tree_name]
        df_raw = tree.arrays(features, library='pd')

    features_df = process_event_time_differences(df_raw)
    if features_df.empty:
        raise RuntimeError("❌ No valid events found.")

    X_new = scaler.transform(features_df[['time_diff_0_2', 'time_diff_1_3']])
    features_df['predicted_label'] = model.predict(X_new)
    return features_df


def plot_prediction_histogram(df, column='predicted_label'):
    plt.figure(figsize=(8, 5))
    sns.kdeplot(df[column], fill=True, color='skyblue')
    plt.title(f'Density Plot of {column}')
    plt.xlabel(column)
    plt.ylabel('Density')
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.show()


# === RUNNING EXAMPLE ===
if __name__ == "__main__":
    # Step 1: Uncomment to retrain
    #main()

    # Step 2: Predict on new test file
    test_file = '/home/harshita/shared/SquareScint_Harshita/x10 data file/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_18_Aug_Ground_10_-10_30min.root'

    t0 = time.time()
    df = predict_on_new_file(test_file)
    print(f"✅ Prediction completed in {time.time() - t0:.2f}s")

    df.to_csv('predictions_on_test_file.csv', index=False)
    print("✅ Predictions saved to 'predictions_on_test_file.csv'")
    print("📊 Mean prediction:", df['predicted_label'].mean())

    plot_prediction_histogram(df)
