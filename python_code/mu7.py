import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, r2_score
import matplotlib.pyplot as plt
from xgboost import XGBRegressor
import joblib

# Load your trained model
model = XGBRegressor()
model.load_model("/home/harshita/shared/SquareScint_Harshita/angula_data/xgb_model1x_ortho6_data.json")  # path to your saved model

# Load and clean new dataset
data = pd.read_csv('/home/harshita/shared/SquareScint_Harshita/angula_data/text_ortho6_data.csv')
data.columns = data.columns.str.strip().str.upper()
print("Columns found in CSV:", data.columns.tolist())

required_columns = ['CH_0', 'CH_1', 'CH_2', 'CH_3']
if not all(col in data.columns for col in required_columns):
    raise ValueError(f"Missing one or more required feature columns: {required_columns}")

# Extract features
X = data[required_columns]

# 🔹 Load the scaler used during training (if available)
scaler = joblib.load('/home/harshita/shared/SquareScint_Harshita/angula_data/scaler_ortho6.pkl')  # must match training time

# 🔹 Normalize the features
X_scaled = scaler.transform(X)

# 🔹 Predict using the normalized data
predictions = model.predict(X_scaled)

# 🔹 Save predictions
output_df = pd.DataFrame(X_scaled, columns=required_columns)
output_df['Predicted_Target'] = predictions
output_df.to_csv('predictions_ortho6nndata.csv', index=False)
print("✅ Predictions saved to 'predictions_ortho6nndata.csv'")

'''
# Predict using the model
predictions = model.predict(X)

# Save predictions
output_df = X.copy()
output_df['Predicted_Target'] = predictions
output_df.to_csv('/home/harshita/shared/new_predictions.csv', index=False)
print("✅ Predictions saved to 'predictions_only.csv'")
'''

# Calculate the difference (error)


mybins = np.linspace(0,10000,500)
print(mybins)
# 3. Histogram of true vs predicted target values
plt.figure(figsize=(10, 6))
plt.hist(predictions, bins=mybins,histtype='step', alpha=0.7, label='True Target', color='blue')
plt.hist(X['CH_0'], bins=mybins,histtype='step', alpha=0.7, label='True Target', color='green')
plt.hist(X['CH_1'], bins=mybins,histtype='step', alpha=0.7, label='True Target', color='red')
plt.hist(X['CH_2'], bins=mybins,histtype='step', alpha=0.7, label='True Target', color='yellow')
plt.hist(X['CH_3'], bins=mybins,histtype='step', alpha=0.7, label='True Target', color='black')
#plt.hist(predictions, bins=200,range=(-500,500), histtype='step', alpha=0.7, label='True Target', color='brown')
#plt.hist(y_pred, bins=200, range=(-200,200), histtype='step', alpha=0.7, label='Predicted Target', color='grey')
plt.yscale('log')
plt.legend()
plt.tight_layout()
plt.show()
'''
# 1. Histogram of error
plt.figure(figsize=(10, 6))
plt.hist(output_df['Error'], bins=200,range= (-200,200), histtype='step', color='red', alpha=0.6)
plt.axvline(0, color='black', linestyle='--', linewidth=1)
#plt.yscale('log')
plt.show()
'''