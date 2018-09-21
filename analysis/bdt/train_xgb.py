
import os
import sys

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import numpy as np
import uproot

from sklearn.model_selection import train_test_split

from tqdm import tqdm

import xgboost as xgb
print xgb.__file__


f = uproot.open("make_inputs/output_total.root")
t = f["t"]
arrs = t.arrays(t.keys())

weights = arrs["weight"]
y_data = (arrs["stype"] == 0.)
x_data = np.c_[
        arrs["detal1l2"],
        arrs["dphil1l2"],
        arrs["htb"],
        arrs["maxmjoverpt"],
        arrs["met"],
        arrs["ml1j1"],
        arrs["nbtags"],
        arrs["njets"],
        arrs["nlb40"],
        arrs["nleps"],
        arrs["ntb40"],
        arrs["ptj1"],
        arrs["ptj6"],
        arrs["ptj7"],
        arrs["ptj8"],
        arrs["ptl1"],
        arrs["ptl2"],
        arrs["ptl3"],
        arrs["q1"],
        ]

print weights
print y_data
print x_data

x_train, x_test, \
        y_train, y_test, \
        weights_train, weights_test \
        = train_test_split(
                x_data,
                y_data,
                weights,
                test_size=0.33, random_state=42
                )


dtrain = xgb.DMatrix( x_train, label=y_train, weight=np.abs(weights_train))
dtest = xgb.DMatrix( x_test, label=y_test, weight=np.abs(weights_test))
evallist  = [(dtrain,'train'), (dtest,'eval')]
num_round = 50
param = {}
param['objective'] = 'binary:logistic'
param['eta'] = 0.09
param['max_depth'] = 5
param['silent'] = 1
param['nthread'] = 10
param['eval_metric'] = "auc"
param['subsample'] = 0.9
param['colsample_bytree'] = 0.5
bst = xgb.train( param.items(), dtrain, num_round, evallist, early_stopping_rounds=20 )
y_pred = bst.predict(dtest)
print y_pred

print y_pred[y_test == 1]
print y_pred[y_test == 0]

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
fig, ax = plt.subplots()
ax.hist(y_pred[y_test==0], weights=weights_test[y_test==0], bins=np.linspace(-0.1,1.1,30),histtype="stepfilled",alpha=0.5, normed=True, label="bkg",color="C0")
ax.hist(y_pred[y_test==1], weights=weights_test[y_test==1], bins=np.linspace(-0.1,1.1,30),histtype="stepfilled",alpha=0.5, normed=True, label="sig",color="C3")
ax.legend()
fig.savefig("ptratios.png")
os.system("which ic && ic ptratios.png")
