import os
import numpy as np

BASE_DIR = os.path.join(os.path.dirname(__file__), '..')
DATA_DIR = os.path.join(BASE_DIR, 'data')

IMU_PATH  = os.path.join(DATA_DIR, 'imu.csv')
GNSS_PATH = os.path.join(DATA_DIR, 'gnss.csv')
KF_PATH   = os.path.join(DATA_DIR, 'estimation.csv')

def load_csv(path):
    data = np.genfromtxt(path, delimiter=',', names=True, invalid_raise=False)
    return {name: data[name] for name in data.dtype.names}

def load_imu():
    return load_csv(IMU_PATH)

def load_gnss():
    return load_csv(GNSS_PATH)

def load_kf():
    return load_csv(KF_PATH)