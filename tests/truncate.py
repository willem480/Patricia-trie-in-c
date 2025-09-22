import pandas as pd
df = pd.read_csv('tests/dataset_full.csv', index_col=0)
df = df.head(500000)
df.to_csv('tests/dataset_test.csv', index=False)

