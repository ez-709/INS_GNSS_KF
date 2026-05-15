import os
import pandas as pd

DATA_DIR = os.path.join(os.path.dirname(__file__), '..', 'data')

def load(name):
    return pd.read_csv(os.path.join(DATA_DIR, name + '.csv'), on_bad_lines='skip')