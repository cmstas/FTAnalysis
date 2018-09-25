
import os
import sys

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import numpy as np
import uproot

from sklearn.model_selection import train_test_split

from tqdm import tqdm

import xgboost as xgb
from utils import write_json, json_to_cfunc

from matplottery.utils import Hist1D
import pickle

print xgb.__file__

# f = uproot.open("make_inputs/output_total_Sep24.root")
f = uproot.open("make_inputs/output_total.root")
# f = uproot.open("make_inputs/output_total_missm1.root")
t = f["t"]
arrs = t.arrays(t.keys())

feature_names = [
        "nbtags",
        "njets",
        "met",
        "ptl2",
        "nlb40",
        "ntb40",
        "nleps",
        "htb",
        "q1",
        "ptj1",
        "ptj6",
        "ptj7",
        "ml1j1",
        "dphil1l2",
        "maxmjoverpt",
        "ptl1",
        "detal1l2",
        "ptj8",
        "ptl3",
        ]

stype = arrs["stype"]
y_data = (stype == 0.)
x_data = np.column_stack([arrs[name] for name in feature_names])
weights_raw = arrs["weight"]


weights = arrs["weight"]*1.0

avg_weight_bkg = np.abs(weights[y_data == 0]).mean()
avg_weight_sig = np.abs(weights[y_data == 1]).mean()
print avg_weight_bkg, avg_weight_sig
print 1.0/avg_weight_bkg, 1.0/avg_weight_sig
weights[y_data == 0] *= 1.0/avg_weight_bkg
weights[y_data == 1] *= 1.0/avg_weight_sig
# weights = arrs["weight"]*100.
# weights[y_data == 1] *= 10.

# clip weight for large weight events (mainly xg and fakes)
weights[np.abs(weights) > 2.] *= 2.0/(weights[np.abs(weights) > 2.])
# keep = (np.abs(weights) < 5.0) & (stype != 5)
# stype = stype[keep]
# y_data = y_data[keep]
# x_data = x_data[keep]
# weights = weights[keep]
# weights_raw = weights_raw[keep]
print weights

for s in np.unique(stype):
    print s, (stype==s).sum(), np.abs(weights[stype == s]).mean()

print x_data.shape

print weights
print y_data
print x_data

x_train, x_test, \
        y_train, y_test, \
        weights_train, weights_test, \
        weights_raw_train, weights_raw_test \
        = train_test_split(
                x_data,
                y_data,
                weights,
                weights_raw,
                test_size=0.25, random_state=42,
                )


dtrain = xgb.DMatrix( x_train, label=y_train, weight=np.abs(weights_train)) #, missing=-1.0)
dtest = xgb.DMatrix( x_test, label=y_test, weight=np.abs(weights_test)) #, missing=-1.0)
evallist  = [(dtrain,'train'), (dtest,'eval')]
# 2131,0.8930,0.8954,0.0024,2626,3.5 {'colsample_bytree': 0.5, 'silent': 1, 'eval_metric': 'auc', 'scale_pos_weight': 1.262414, 'nthread': 1, 'min_child_weight': 5.0, 'subsample': 0.6, 'eta': 0.04, 'alpha': 4.0, 'max_depth': 5, 'gamma': 2.0, 'lambda': 1.5}
num_round = 1500
param = {}
param['objective'] = 'binary:logistic'

param['eta'] = 0.04
param['max_depth'] = 5
param['silent'] = 1
param['nthread'] = 15
param['eval_metric'] = "auc"
param['subsample'] = 0.6
param['alpha'] = 4.0
param['gamma'] = 2.0
param['lambda'] = 1.5
param['min_child_weight'] = 5.0
param['colsample_bytree'] = 0.5

# param['eta'] = 0.05
# param['max_depth'] = 5
# param['silent'] = 1
# param['nthread'] = 15
# param['eval_metric'] = "auc"
# param['subsample'] = 0.8
# param['alpha'] = 6.9
# param['gamma'] = 9.7
# param['lambda'] = 1.5
# param['min_child_weight'] = 6.6
# param['colsample_bytree'] = 0.8

sumw_pos = np.abs(dtrain.get_weight()[dtrain.get_label()==1]).sum()
sumw_neg = np.abs(dtrain.get_weight()[dtrain.get_label()==0]).sum()
param["scale_pos_weight"] = sumw_neg/sumw_pos
if not os.path.exists("bst.pkl"):
    bst = xgb.train( param.items(), dtrain, num_round, evallist, early_stopping_rounds=20 )
    pickle.dump(bst,open("bst.pkl","wb"))
    write_json("model.json", bst, feature_names)
    json_to_cfunc("model.json", fname_out="func.h")
else:
    print "[!] Found pickle file...loading"
    bst = pickle.load(open("bst.pkl","rb"))

pickle.dump(bst,open("bst.pkl","wb"))
write_json("model.json", bst, feature_names)

y_pred_train = bst.predict(dtrain)
y_pred = bst.predict(dtest)

# cv_result = xgb.cv(param, dtrain, num_round, nfold=5, seed=42, callbacks=[xgb.callback.early_stop(20)],as_pandas=False)
# print cv_result
# print dir(cv_result)
# y_pred = cv_result.predict(dtest)
# print dir(bst)
# print dir(xgb)

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
fig, ax = plt.subplots()
preds_bkg_test = y_pred[y_test==0]
preds_sig_test = y_pred[y_test==1]
preds_bkg_train = y_pred_train[y_train==0]
preds_sig_train = y_pred_train[y_train==1]

# # print stype[weights*50 > 1.5]
# # print np.histogram(stype[weights*50 > 1.5])
# print stype[weights < 0.0001]
# print np.histogram(stype[weights < 0.0001])
# print weights[weights < 0.0001]

# ax.hist(np.abs(weights)[y_data==0], bins=np.linspace(0.0,10.1,100),histtype="stepfilled",alpha=0.5, density=False, label="weights bkg",color="C0")
# ax.hist(np.abs(weights)[y_data==1], bins=np.linspace(0.0,10.1,100),histtype="stepfilled",alpha=0.5, density=False, label="weights sig",color="C3")
# ax.set_yscale("log",nonposy="clip")
# # ax.set_xscale("log",nonposx="clip")

density = True
bins = np.linspace(0.0,1,50)

from scipy import stats
pks_bkg = stats.ks_2samp(preds_bkg_train,preds_bkg_test)[1]
pks_sig = stats.ks_2samp(preds_sig_train,preds_sig_test)[1]


# train bkg
ax.hist(preds_bkg_train, weights=weights_raw_train[y_train==0], bins=bins,histtype="stepfilled",alpha=0.45, density=density, label="bkg, train",color="C0")

# train sig
ax.hist(preds_sig_train, weights=weights_raw_train[y_train==1], bins=bins,histtype="stepfilled",alpha=0.45, density=density, label="sig, train",color="C3")

# test bkg error bars
counts_bkg,_,_ = ax.hist(preds_bkg_test, weights=weights_raw_test[y_test==0], bins=bins,histtype="step",alpha=1.0, density=density, label="bkg, test (KS prob = {:.2f})".format(pks_bkg),color="C0", lw=1.5, linestyle="solid")
h = Hist1D(preds_bkg_test, weights=weights_raw_test[y_test==0], bins=bins)
h *= float(counts_bkg.sum()/h.get_integral())
ax.errorbar( h.get_bin_centers(), h.counts, yerr=h.errors, marker="o", linestyle="", markersize=2, linewidth=1.5, color="C0",)

# test sig error bars
counts_sig,_,_= ax.hist(preds_sig_test, weights=weights_raw_test[y_test==1], bins=bins,histtype="step",alpha=1.0, density=density, label="sig, test (KS prob = {:.2f})".format(pks_sig),color="C3", lw=1.5, linestyle="solid")
h = Hist1D(preds_sig_test, weights=weights_raw_test[y_test==1], bins=bins)
h *= float(counts_sig.sum()/h.get_integral())
ax.errorbar( h.get_bin_centers(), h.counts, yerr=h.errors, marker="o", linestyle="", markersize=2, linewidth=1.5, color="C3",)

print counts_bkg.sum()
print counts_sig.sum()



ax.legend()

ax.set_ylim([0.,ax.get_ylim()[1]])
fig.savefig("disc.png")
os.system("which ic && ic disc.png")
