import os
import numpy as np
import pandas as pd

DATA_DIR = os.path.join(os.path.dirname(__file__), '..', 'data')

def load(name):
    return pd.read_csv(os.path.join(DATA_DIR, name + '.csv'), on_bad_lines='skip')

def read_csv(fname):
    raw = np.genfromtxt(fname, delimiter=',', skip_header=1, invalid_raise=False)
    raw = raw[~np.isnan(raw).any(axis=1)]
    t   = raw[:, 0] - raw[0, 0]
    gyr = raw[:, 1:4]
    acc = raw[:, 4:7]
    mag = raw[:, 7:10]
    return t, gyr, acc, mag