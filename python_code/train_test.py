import uproot
import pandas as pd
import joblib
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.multioutput import MultiOutputRegressor
from sklearn.metrics import mean_squared_error
from xgboost import XGBRegressor
import matplotlib.pyplot as plt

# === Step 1: Path to ROOT file ===
file_path = "/home/harshita/shared/SquareScint_Harshita/MuonData/merged_dataMu.root"

# === Step 2: Open ROOT file and TTree ===
tree = uproot.open(file_path)["outml"]

# === Step 3: Define features and target ===
features = [ 'q0', 'q1', 'q2', 'q3']
targets = ['x', 'y']  # multi-output

# === Step 4: Load data into DataFrame ===
df = tree.arrays(features + targets, library="pd").dropna()

X = df[features].values
y = df[targets].values  # Multi-output target

# === Step 5: Scale features ===
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# === Step 6: Train/test split ===
X_train, X_test, y_train, y_test = train_test_split(
    X_scaled, y, test_size=0.2, random_state=42
)

# === Step 7: Define and train model ===
model = MultiOutputRegressor(
    XGBRegressor(objective='reg:squarederror', n_estimators=100, random_state=42)
)

model.fit(X_train, y_train)

# === Step 8: Predict and evaluate ===
y_pred = model.predict(X_test)

# Print MSE for each output
for i, target_name in enumerate(targets):
    mse = mean_squared_error(y_test[:, i], y_pred[:, i])
    print(f"Test MSE for '{target_name}': {mse:.4f}")

print("✅ Model trained.")

# === Step 9: Save model and scaler ===
joblib.dump(model, "xgboost_model.joblib")
joblib.dump(scaler, "scaler.joblib")
print("✅ Saved model and scaler.")

pred_df = pd.DataFrame(y_pred, columns=["x_pred", "y_pred"])
pred_df.to_csv("predictions.csv", index=False)
print("✅ Saved the predicion.")



# === Step 10: Plot histograms of predicted values ===
plt.figure(figsize=(8, 6))

colors = ['red', 'blue']

for i, target_name in enumerate(targets):
    #plt.hist(y_pred[:, i], bins=100, histtype='step', 
             #color=colors[i], alpha=0.7, label=f"Predicted '{target_name}'")
    if i == 0:
        plt.hist(y_test[:, i], bins=100, histtype='step', color=colors[i], alpha=0.7, label=f"test '{target_name}'")
        plt.hist(y_pred[:, i], bins=100, histtype='step', color=colors[i], alpha=0.7, label=f"Predicted '{target_name}'")
plt.title("Histogram of Predicted Values for x and y")
plt.xlabel("Value")
plt.ylabel("Frequency")
plt.legend()
plt.grid(True)
plt.show()


#======= diff between true and traget values =========
plt.figure(figsize=(8, 6))

colors = ['blue', 'green']

for i, target_name in enumerate(targets):
    residuals = y_test[:, i] - y_pred[:, i]
    plt.hist(residuals, bins=100, histtype='step', 
             color=colors[i], alpha=0.7, 
             label=f"Residuals for '{target_name}'")

plt.title("Histogram of Residuals (True - Predicted) for x and y")
plt.xlabel("Residual")
plt.ylabel("Frequency")
#plt.axvline(0, color='black', linestyle='--')  # Zero residual line
plt.legend()
plt.grid(True)
plt.show()

'''
for i, target_name in enumerate(targets):
    plt.hist2d(y_pred[:, 0], y_pred[:, 1], bins=50, cmap='viridis')
    plt.colorbar(label='Counts')
    plt.xlabel("x position")
    plt.ylabel("y position")
    plt.title("2D Histogram of Predicted Positions (x, y)")
    plt.grid(True)
    plt.axis('equal')
    plt.show()
    '''


