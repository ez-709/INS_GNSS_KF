import os
import webbrowser
import pandas as pd
from plots import plot_imu, plot_kf, plot_map

DATA_DIR  = os.path.join(os.path.dirname(__file__), '..', 'data')

imu  = pd.read_csv(os.path.join(DATA_DIR, 'imu.csv'),  on_bad_lines='skip')
gnss = pd.read_csv(os.path.join(DATA_DIR, 'gnss.csv'), on_bad_lines='skip')
bins = pd.read_csv(os.path.join(DATA_DIR, 'bins.csv'), on_bad_lines='skip')
kf   = pd.read_csv(os.path.join(DATA_DIR, 'kf.csv'),   on_bad_lines='skip')

plot_imu(imu)
plot_kf(kf)

path = plot_map(bins, gnss)
webbrowser.open("file://" + path)