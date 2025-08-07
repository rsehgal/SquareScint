import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from xgboost import XGBRegressor
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
import joblib

# Load and clean column names
data = pd.read_csv('/home/harshita/shared/SquareScint_Harshita/angula_data/angular_ortho_data_6ch.csv')
data.columns = data.columns.str.strip().str.upper()
print("Columns found in CSV:", data.columns.tolist())

product = data['CH_4'] * data['CH_5']
valid_mask = (product >= 0) & (~product.isna())
data = data[valid_mask].copy() 

# Calculate derived target = (Q4 * Q5)^0.5
data['TARGET'] = np.sqrt(data['CH_4'] * data['CH_5'])

# Features and target
X = data[['CH_0', 'CH_1', 'CH_2', 'CH_3']]
y = data['TARGET']

scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# 🔹 Save the scaler
scaler_path = "/home/harshita/shared/SquareScint_Harshita/angula_data/scaler_ortho6.pkl"
joblib.dump(scaler, scaler_path)
print(f"Scaler saved to: {scaler_path}")

# Train-test split
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# Train XGBoost regressor
model = XGBRegressor(objective='reg:squarederror', random_state=42)
model.fit(X_train, y_train)

# Predict on test data
y_pred = model.predict(X_test)

# Evaluation metrics
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)

print(f"Test Mean Squared Error (MSE): {mse:.4f}")
print(f"Test R-squared (R2): {r2:.4f}")

# Save predictions
output_df = pd.DataFrame({
    'Q0': X_test['CH_0'],
    'Q1': X_test['CH_1'],
    'Q2': X_test['CH_2'],
    'Q3': X_test['CH_3'],
    'True_Target': y_test,
    'Predicted_Target': y_pred
})
output_df.to_csv('prediction_angular_data.csv', index=False)
print("Predictions saved to 'prediction_angular_data.csv'")

# Print true and predicted values
print("\nSample of True vs Predicted Target Values:")
print(output_df[['True_Target', 'Predicted_Target']])  # adjust number of rows if needed

# Calculate the difference (error)
output_df['Error'] = output_df['True_Target'] - output_df['Predicted_Target']

model.save_model("xgb_model1x_ortho6_data.json")  # You can also use .model extension
print("Trained model saved to 'xgb_model1x_ortho6_data.json'")
'''
# Plot histogram of True and Predicted target values
plt.figure(figsize=(10, 6))
plt.hist(output_df['True_Target'], bins=200, alpha=0.6, label='True Target', color='green', histtype='step')
plt.hist(output_df['Predicted_Target'], bins=200, alpha=0.6, label='Predicted Target', color='purple', histtype='step')
plt.yscale('log')
plt.show()
'''

# 3. Histogram of true vs predicted target values
plt.figure(figsize=(10, 6))
plt.hist(y_test, bins=200, histtype='step', alpha=0.7, label='True Target', color='blue')
plt.hist(y_pred, bins=200, histtype='step', alpha=0.7, label='Predicted Target', color='orange')
plt.yscale('log')
plt.show()

# 1. Histogram of error
plt.figure(figsize=(10, 6))
plt.hist(output_df['Error'], bins=200,range= (-200,200), histtype='step', color='red', alpha=0.6)
plt.axvline(0, color='black', linestyle='--', linewidth=1)
#plt.yscale('log')
plt.show()
'''
# 2. Optional: Line plot of error across samples
plt.figure(figsize=(10, 5)) 
plt.plot(output_df['Error'].values, marker='o', linestyle='-', color='crimson', alpha=0.7)
plt.axhline(0, color='black', linestyle='--')
plt.show()
'''

'''
# 4. Scatter plot: True vs Predicted values
plt.figure(figsize=(8, 6))
plt.scatter(y_test, y_pred, alpha=0.6, edgecolor='k')
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'r--')
plt.show()
'''

