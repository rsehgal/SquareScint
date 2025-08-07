import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, r2_score
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.callbacks import EarlyStopping

# Load and clean column names
data = pd.read_csv('/home/harshita/shared/angular_1data.csv')
data.columns = data.columns.str.strip().str.upper()
print("Columns found in CSV:", data.columns.tolist())

product = data['CH_4'] * data['CH_5']
valid_mask = (product >= 0) & (~product.isna())
data = data[valid_mask].copy()

# Calculate derived target = (Q4 * Q5)^0.5
data['TARGET'] = np.sqrt(data['CH_4'] * data['CH_5'])

# Features and target
X = data[['CH_0', 'CH_1', 'CH_2', 'CH_3']].values
y = data['TARGET'].values

# Train-test split
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# Build a simple Neural Network model
model = Sequential([
    Dense(4, activation='relu', input_shape=(X_train.shape[1],)),
    Dense(4, activation='relu'),
    Dense(1)  # Output layer for regression (linear activation by default)
])

model.compile(optimizer='adam', loss='mse', metrics=['mse'])

# Optional early stopping to prevent overfitting
early_stop = EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True)

# Train model
history = model.fit(
    X_train, y_train,
    validation_split=0.2,
    epochs=100,
    batch_size=32,
    callbacks=[early_stop],
    verbose=1
)

# Predict on test data
y_pred = model.predict(X_test).flatten()

# Evaluation metrics
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)

print(f"Test Mean Squared Error (MSE): {mse:.4f}")
print(f"Test R-squared (R2): {r2:.4f}")

# Save predictions
output_df = pd.DataFrame({
    'Q0': X_test[:, 0],
    'Q1': X_test[:, 1],
    'Q2': X_test[:, 2],
    'Q3': X_test[:, 3],
    'True_Target': y_test,
    'Predicted_Target': y_pred
})
output_df.to_csv('prediction_nn.csv', index=False)
print("Predictions saved to 'prediction_nn.csv'")


# Optional: Save the model
model.save('neural_net_model.h5')
print("Neural network model saved to 'neural_net_model.h5'")

#Print true and predicted values
print("\nSample of True vs Predicted Target Values:")
print(output_df[['True_Target', 'Predicted_Target']])  # adjust number of rows if needed

# Calculate the difference (error)
output_df['Error'] = output_df['True_Target'] - output_df['Predicted_Target']

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


