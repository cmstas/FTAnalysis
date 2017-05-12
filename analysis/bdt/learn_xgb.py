#!/usr/bin/env python
import numpy as np
from tqdm import tqdm
# from sklearn.model_selection import train_test_split
# from sklearn import metrics
import sys
import os
from variables import cols

import xgboost as xgb

### Load files from images.py

xdata = np.load("dump_xdata.npa")
ydata = np.load("dump_ydata.npa")
weightSRs = np.load("dump_extra.npa")

# xdata = np.load("dumpsr_xdata.npa")
# ydata = np.load("dumpsr_ydata.npa")
# weightSRs = np.load("dumpsr_extra.npa")

# from read import d_name_map
# good = (ydata!=d_name_map["xg"]) \
#       *(ydata!=d_name_map["flips"]) \
#       *(ydata!=d_name_map["qqww"]) \
#       *(ydata!=d_name_map["rares"]) \
#       *(ydata!=d_name_map["fakes_mc"]) \
# print len(good), np.sum(good)
# print good
# print xdata
# xdata = xdata[good]
# ydata = ydata[good]
# weightSRs = weightSRs[good]

# sys.exit()

weights = weightSRs[:,0]
SRs = weightSRs[:,1]

# print xdata
# print ydata
# print SRs


# abs the weights, turns out xgboost already does this anyways
# weights = np.abs(weights)

# 1 is tttt
ydata[ydata < 6.5] = 0
ydata[ydata > 6.5] = 1

# print weights[ydata == 1]
# sys.exit()

### Splits and stuff
np.random.seed(42)
# np.random.shuffle(xdata)
# np.random.shuffle(ydata)
# np.random.shuffle(weights)
# np.random.shuffle(SRs)
randomize = np.arange(len(xdata))
np.random.shuffle(randomize)
xdata = xdata[randomize]
ydata = ydata[randomize]
weights = weights[randomize]
SRs = SRs[randomize]

def train_test_split(mat, test_size=0.5):
    cutoff = int(test_size*len(mat))
    return mat[cutoff:], mat[:cutoff]

X_train, X_test = train_test_split(xdata)
y_train, y_test = train_test_split(ydata)
weights_train, weights_test = train_test_split(weights)
SRs_train, SRs_test = train_test_split(SRs)

### Train.
dtrain = xgb.DMatrix( X_train, label=y_train, weight=np.abs(weights_train))
dtest = xgb.DMatrix( X_test, label=y_test, weight=np.abs(weights_test))
# dtrain = xgb.DMatrix( X_train, label=y_train) #, weight=weights_train)
# dtest = xgb.DMatrix( X_test, label=y_test) #, weight=weights_test)
evallist  = [(dtest,'eval'), (dtrain,'train')]
num_round = 200
# num_round = 500
param = {}
param['objective'] = 'binary:logistic'
param['eta'] = 0.09
param['max_depth'] = 5
param['silent'] = 1
param['nthread'] = 1
param['eval_metric'] = "auc"
param['subsample'] = 0.9
param['colsample_bytree'] = 0.5
bst = xgb.train( param.items(), dtrain, num_round, evallist, early_stopping_rounds=100 )
y_pred = bst.predict(dtest)

# np.array(X_test, dtype=np.float32).dump("X_test.npa")
# print y_pred[:10]
print " ".join(map(str,X_test[0]))
for rank,name in sorted(zip(map(bst.get_fscore().get, bst.feature_names),cols), reverse=True):
    print "{:4s} {:15s}".format(str(rank),str(name))
print 

with open("model.json", "w") as fhout:
    fhout.write("[\n"+",\n".join(bst.get_dump(dump_format="json"))+"\n]")
# for thing in bst.get_dump(dump_format="json"):
#     print thing

