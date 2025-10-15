import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
import glob
import os

# Load all CSV files from results folder
results_folder = 'results'
csv_files = glob.glob(os.path.join(results_folder, '*.csv'))

if not csv_files:
    print(f"No CSV files found in {results_folder} folder")
    exit(1)

print(f"Found {len(csv_files)} CSV file(s):")
for file in csv_files:
    print(f"  - {file}")

# Read and combine all CSV files
dataframes = [pd.read_csv(file) for file in csv_files]
df = pd.concat(dataframes, ignore_index=True)

print(f"\nLoaded {len(df)} data points")
print(f"Columns: {df.columns.tolist()}")

# Calculate errors
df['absolute_error'] = np.abs(df['exact'] - df['approx'])

# Avoid division by zero - only calculate relative error where exact != 0
df['relative_error'] = np.where(
    df['exact'] != 0,
    df['absolute_error'] / np.abs(df['exact']),
    np.nan
)
df['percentage_error'] = df['relative_error'] * 100

# Filter out invalid values for relative error analysis
df_valid_rel = df[np.isfinite(df['relative_error'])].copy()

# Statistical bounds
mean_error = df['absolute_error'].mean()
std_error = df['absolute_error'].std()
max_error = df['absolute_error'].max()
min_error = df['absolute_error'].min()

# 95% confidence interval upper bound
confidence = 0.95
n = len(df)
margin = stats.t.ppf((1 + confidence) / 2, n - 1) * (std_error / np.sqrt(n))
upper_bound_95 = mean_error + margin

# Percentile-based bounds
percentile_95 = np.percentile(df['absolute_error'], 95)
percentile_99 = np.percentile(df['absolute_error'], 99)
percentile_999 = np.percentile(df['absolute_error'], 99.9)

# 3-sigma rule (99.7% of data in normal distribution)
three_sigma_bound = mean_error + 3 * std_error

print("\n" + "="*60)
print("ABSOLUTE ERROR ANALYSIS")
print("="*60)
print(f"Mean absolute error:        {mean_error:.6f}")
print(f"Median absolute error:      {np.median(df['absolute_error']):.6f}")
print(f"Std deviation:              {std_error:.6f}")
print(f"Minimum observed error:     {min_error:.6f}")
print(f"Maximum observed error:     {max_error:.6f}")

print("\n" + "="*60)
print("STATISTICAL UPPER BOUNDS")
print("="*60)
print(f"95% confidence upper bound: {upper_bound_95:.6f}")
print(f"95th percentile:            {percentile_95:.6f}")
print(f"99th percentile:            {percentile_99:.6f}")
print(f"99.9th percentile:          {percentile_999:.6f}")
print(f"Mean + 3σ (99.7% rule):     {three_sigma_bound:.6f}")
print(f"Maximum observed:           {max_error:.6f}")

# Relative error bounds
mean_rel_error = df_valid_rel['relative_error'].mean()
max_rel_error = df_valid_rel['relative_error'].max()
percentile_999_rel = np.percentile(df_valid_rel['relative_error'], 99.9)

print("\n" + "="*60)
print("RELATIVE ERROR BOUNDS")
print("="*60)
print(f"Valid samples for relative error: {len(df_valid_rel)} / {len(df)}")
print(f"Mean relative error:        {mean_rel_error:.6f} ({mean_rel_error*100:.2f}%)")
print(f"Maximum relative error:     {max_rel_error:.6f} ({max_rel_error*100:.2f}%)")
print(f"99.9th percentile relative: {percentile_999_rel:.6f} ({percentile_999_rel*100:.2f}%)")

# Recommended upper bound
recommended_bound = percentile_999
print("\n" + "="*60)
print("RECOMMENDED UPPER BOUND")
print("="*60)
print(f"Recommended bound (99.9th percentile): {recommended_bound:.6f}")
print(f"This means 99.9% of approximations have error ≤ {recommended_bound:.6f}")
print("="*60)

# Visualization
fig, axes = plt.subplots(2, 2, figsize=(14, 10))

# Plot 1: Error distribution histogram
axes[0, 0].hist(df['absolute_error'], bins=50, edgecolor='black', alpha=0.7, color='skyblue')
axes[0, 0].axvline(mean_error, color='r', linestyle='--', linewidth=2, label=f'Mean: {mean_error:.6f}')
axes[0, 0].axvline(percentile_999, color='g', linestyle='--', linewidth=2, label=f'99.9%: {percentile_999:.6f}')
axes[0, 0].axvline(max_error, color='orange', linestyle='--', linewidth=2, label=f'Max: {max_error:.6f}')
axes[0, 0].set_xlabel('Absolute Error', fontsize=11)
axes[0, 0].set_ylabel('Frequency', fontsize=11)
axes[0, 0].set_title('Absolute Error Distribution', fontsize=12, fontweight='bold')
axes[0, 0].legend()
axes[0, 0].grid(True, alpha=0.3)

# Plot 2: Error vs exact value
axes[0, 1].scatter(df['exact'], df['absolute_error'], alpha=0.5, s=20, color='blue')
axes[0, 1].axhline(percentile_999, color='r', linestyle='--', linewidth=2, label=f'99.9% bound: {percentile_999:.6f}')
axes[0, 1].axhline(mean_error, color='orange', linestyle='--', linewidth=2, label=f'Mean: {mean_error:.6f}')
axes[0, 1].set_xlabel('Exact Value', fontsize=11)
axes[0, 1].set_ylabel('Absolute Error', fontsize=11)
axes[0, 1].set_title('Error vs Exact Value', fontsize=12, fontweight='bold')
axes[0, 1].legend()
axes[0, 1].grid(True, alpha=0.3)

# Plot 3: Relative error distribution
axes[1, 0].hist(df_valid_rel['percentage_error'], bins=50, edgecolor='black', alpha=0.7, color='lightcoral')
axes[1, 0].axvline(np.mean(df_valid_rel['percentage_error']), color='r', linestyle='--', linewidth=2,
                   label=f'Mean: {np.mean(df_valid_rel["percentage_error"]):.2f}%')
axes[1, 0].axvline(percentile_999_rel * 100, color='g', linestyle='--', linewidth=2,
                   label=f'99.9%: {percentile_999_rel*100:.2f}%')
axes[1, 0].set_xlabel('Percentage Error (%)', fontsize=11)
axes[1, 0].set_ylabel('Frequency', fontsize=11)
axes[1, 0].set_title('Relative Error Distribution', fontsize=12, fontweight='bold')
axes[1, 0].legend()
axes[1, 0].grid(True, alpha=0.3)

# Plot 4: Q-Q plot (check if errors are normally distributed)
stats.probplot(df['absolute_error'], dist="norm", plot=axes[1, 1])
axes[1, 1].set_title('Q-Q Plot (Normal Distribution Check)', fontsize=12, fontweight='bold')
axes[1, 1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('error_analysis.png', dpi=300)
print(f"\nVisualization saved as 'error_analysis.png'")

# Cumulative distribution plot
plt.figure(figsize=(10, 6))
sorted_errors = np.sort(df['absolute_error'])
cumulative = np.arange(1, len(sorted_errors) + 1) / len(sorted_errors) * 100

plt.plot(sorted_errors, cumulative, linewidth=2)
plt.axhline(95, color='r', linestyle='--', label='95%')
plt.axhline(99, color='orange', linestyle='--', label='99%')
plt.axhline(99.9, color='g', linestyle='--', label='99.9%')
plt.axvline(percentile_95, color='r', linestyle=':', alpha=0.5)
plt.axvline(percentile_99, color='orange', linestyle=':', alpha=0.5)
plt.axvline(percentile_999, color='g', linestyle=':', alpha=0.5)
plt.xlabel('Absolute Error', fontsize=12)
plt.ylabel('Cumulative Percentage (%)', fontsize=12)
plt.title('Cumulative Error Distribution', fontsize=14, fontweight='bold')
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('cumulative_error.png', dpi=300)
print(f"Cumulative distribution saved as 'cumulative_error.png'")

plt.show()

print(f"\nAnalysis complete! Total data points: {len(df)}")
