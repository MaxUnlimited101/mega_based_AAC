import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from sklearn.linear_model import LinearRegression
import os
import glob

# Load all CSV files from the results folder
results_folder = 'results'
csv_files = glob.glob(os.path.join(results_folder, '*.csv'))

# Check if there are any CSV files
if not csv_files:
    print(f"No CSV files found in {results_folder} folder")
    exit(1)

print(f"Found {len(csv_files)} CSV file(s):")
for file in csv_files:
    print(f"  - {file}")

# Read and combine all CSV files into one dataframe
dataframes = []
for file in csv_files:
    df_temp = pd.read_csv(file)
    dataframes.append(df_temp)

# Combine all dataframes
df = pd.concat(dataframes, ignore_index=True)

print(f"\nCombined dataframe shape: {df.shape}")
print(f"Columns: {df.columns.tolist()}")
print(f"First few rows:\n{df.head()}")

# Extract x and y values
x = df['exact'].values.reshape(-1, 1)
y = df['approx'].values

# Create and fit the linear regression model
model = LinearRegression()
model.fit(x, y)

# Generate predictions for the regression line
x_line = np.linspace(x.min(), x.max(), 100).reshape(-1, 1)
y_pred = model.predict(x_line)

# Create the plot
plt.figure(figsize=(10, 6))
plt.scatter(x, y, color='blue', alpha=0.6, s=50, label='Data points')
plt.plot(x_line, y_pred, color='red', linewidth=2, label=f'Regression line: y = {model.coef_[0]:.2f}x + {model.intercept_:.2f}')

# Add labels and title
plt.xlabel('Exact', fontsize=12)
plt.ylabel('Approx', fontsize=12)
plt.title('Exact vs Approx with Regression Line', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)

# Display R² score
r_squared = model.score(x, y)
plt.text(0.05, 0.95, f'R² = {r_squared:.4f}', 
         transform=plt.gca().transAxes, 
         verticalalignment='top',
         bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

# Save and show the plot
plt.tight_layout()
plt.savefig('regression_plot.png', dpi=300)
plt.show()

print(f"\nRegression equation: y = {model.coef_[0]:.2f}x + {model.intercept_:.2f}")
print(f"R² score: {r_squared:.4f}")
print(f"Total data points: {len(x)}")