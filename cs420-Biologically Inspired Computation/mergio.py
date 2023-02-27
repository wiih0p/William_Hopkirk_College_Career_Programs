import pandas as pd
import glob
import os

path = '*.csv'

# Use glob to get all the CSV files in the path
all_files = glob.glob(path)

# Combine all CSV files into a single DataFrame
df = pd.concat((pd.read_csv(f) for f in all_files))

# Export the combined DataFrame to a CSV file
df.to_csv('all.csv', index=False)