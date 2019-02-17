
import os
import sys

import numpy as np

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')

import uproot

from sklearn.model_selection import train_test_split

from tqdm import tqdm

import xgboost as xgb
from utils import write_json, json_to_cfunc

from matplottery.utils import Hist1D
import pickle

print xgb.__file__

# f = uproot.open("make_inputs/output_total_Sep24.root")
# f = uproot.open("make_inputs/output_total.root")
f = uproot.open("make_inputs/output_run2.root")
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

# keep = arrs["class"] == 3
keep = (arrs["class"] == 3) | (arrs["stype"] > 0.)
stype = stype[keep]
y_data = y_data[keep]
x_data = x_data[keep]
weights = weights[keep]
weights_raw = weights_raw[keep]

avg_weight_bkg = np.abs(weights[y_data == 0]).mean()
avg_weight_sig = np.abs(weights[y_data == 1]).mean()
print avg_weight_bkg, avg_weight_sig
print 1.0/avg_weight_bkg, 1.0/avg_weight_sig
weights[y_data == 0] *= 1.0/avg_weight_bkg
weights[y_data == 1] *= 1.0/avg_weight_sig
# weights = arrs["weight"]*100.
# weights[y_data == 1] *= 10.

print "avg weights"
for s in np.unique(stype):
    print s, (stype==s).sum(), np.abs(weights[stype == s]).mean()

# clip weight for large weight events (mainly xg and fakes)
weights[stype == 4] *= 0.3
weights[np.abs(weights) > 10.] *= 10.0/(weights[np.abs(weights) > 10.])
# keep = (np.abs(weights) < 5.0) & (stype != 5)
# stype = stype[keep]
# y_data = y_data[keep]
# x_data = x_data[keep]
# weights = weights[keep]
# weights_raw = weights_raw[keep]
print weights

print "avg weights"
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
# num_round = 750
param = {}
param['objective'] = 'binary:logistic'

# # bst_orig.pkl
# param['eta'] = 0.04
# param['max_depth'] = 4
# param['silent'] = 1
# param['nthread'] = 15
# param['eval_metric'] = "auc"
# param['subsample'] = 0.6
# param['alpha'] = 4.0
# param['gamma'] = 2.0
# param['lambda'] = 1.5
# param['min_child_weight'] = 5.0
# param['colsample_bytree'] = 0.5

# 2337,0.8964,0.8993,0.0028,1547,3.5 {'colsample_bytree': 1.0, 'silent': 1, 'eval_metric': 'auc', 'scale_pos_weight': 1.9180059, 'nthread': 1, 'min_child_weight': 1.0, 'subsample': 0.6, 'eta': 0.04, 'alpha': 8.0, 'max_depth': 4, 'gamma': 2.0, 'lambda': 1.0}
num_round = 500
param['eta'] = 0.07
# param['max_depth'] = 4
param['max_depth'] = 5
param['silent'] = 1
param['nthread'] = 15
param['eval_metric'] = "auc"
param['subsample'] = 0.6
param['alpha'] = 8.0
param['gamma'] = 2.0
param['lambda'] = 1.0
param['min_child_weight'] = 1.0
param['colsample_bytree'] = 1.0

# param['eta'] = 0.03
# param['max_depth'] = 5
# param['silent'] = 1
# param['nthread'] = 15
# param['eval_metric'] = "auc"
# param['subsample'] = 0.86
# param['alpha'] = 9.45
# param['gamma'] = 4.3
# param['lambda'] = 3.7
# param['min_child_weight'] = 1.4
# param['colsample_bytree'] = 0.94

sumw_pos = np.abs(dtrain.get_weight()[dtrain.get_label()==1]).sum()
sumw_neg = np.abs(dtrain.get_weight()[dtrain.get_label()==0]).sum()
param["scale_pos_weight"] = sumw_neg/sumw_pos
# pklname = "bst_ssos.pkl" # bdt_earlystoppingnominal.h <-- 3.3\sigma with k=11.5,17 bins
# pklname = "bst_run2_ssos_v2.pkl"
pklname = "bst_run2_v3.pkl"
# pklname = "bst_ssos_depth4.pkl"
# pklname = "bst_ss.pkl"
if not os.path.exists(pklname):
    os.nice(10)
    bst = xgb.train( param.items(), dtrain, num_round, evallist, early_stopping_rounds=20 )
    pickle.dump(bst,open(pklname,"wb"))
    write_json("model.json", bst, feature_names)
    json_to_cfunc("model.json", fname_out="func.h")
else:
    print "[!] Found pickle file...loading"
    bst = pickle.load(open(pklname,"rb"))

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

# # XXX TRANSFORM by unstretching peaks at 0 and 1 with inverse sigmoid
# vmin = min(preds_bkg_train.min(),preds_bkg_test.min())
# vmax = max(preds_sig_train.max(),preds_sig_test.max())
# def inv_sigmoid(x,tau):
#     return (1.0/tau)*np.log(x/(1-x))
# def trans(x,tau=6.):
#     y = inv_sigmoid(x,tau)
#     ymin = inv_sigmoid(vmin,tau)
#     ymax = inv_sigmoid(vmax,tau)
#     y = (y-ymin)/(ymax-ymin)
#     return y
# preds_bkg_train = trans(preds_bkg_train)
# preds_sig_train = trans(preds_sig_train)
# preds_bkg_test = trans(preds_bkg_test)
# preds_sig_test = trans(preds_sig_test)
# bins = np.linspace(0,1,50)
# print preds_bkg_train.min(),preds_bkg_train.max()
# print preds_sig_train.min(),preds_sig_train.max()
# print preds_bkg_test.min(),preds_bkg_test.max()
# print preds_sig_test.min(),preds_sig_test.max()

from scipy import stats
pks_bkg = stats.ks_2samp(preds_bkg_train,preds_bkg_test)[1]
pks_sig = stats.ks_2samp(preds_sig_train,preds_sig_test)[1]


def get_bkg_weights(train=True):
    nbtagsbins = [0.5,1.5,2.5,3.5,99.5]
    njetsbins = [1.5,2.5,3.5,4.5,5.5,99.5]
    if train:
        is_signal = y_train==1
        nbtags = x_train[:,0]
        njets = x_train[:,1]
    else:
        is_signal = y_test==1
        nbtags = x_test[:,0]
        njets = x_test[:,1]
    extra_weights = np.ones(len(is_signal))
    # counts_sig, _,_ = np.histogram2d(nbtags[ is_signal],njets[ is_signal],bins=[nbtagsbins,njetsbins])
    # counts_bkg, _,_ = np.histogram2d(nbtags[~is_signal],njets[~is_signal],bins=[nbtagsbins,njetsbins])
    # nbtagsidxs = np.digitize(nbtags[~is_signal],nbtagsbins)-1
    # njetsidxs = np.digitize(njets[~is_signal],njetsbins)-1
    # extra_weights[~is_signal] = (1.0*counts_sig[nbtagsidxs,njetsidxs]/counts_bkg[nbtagsidxs,njetsidxs])
    return extra_weights[~is_signal]

train_bkg_weights = get_bkg_weights(train=True)
test_bkg_weights = get_bkg_weights(train=False)

# train bkg
counts_train_bkg,_,_ = ax.hist(preds_bkg_train, weights=train_bkg_weights*weights_raw_train[y_train==0], bins=bins,histtype="stepfilled",alpha=0.45, density=density, label="bkg, train",color="C0")
h_train_bkg = Hist1D(preds_bkg_train, weights=train_bkg_weights*weights_raw_train[y_train==0], bins=bins)
h_train_bkg *= float(counts_train_bkg.sum()/h_train_bkg.get_integral())
# ax.errorbar( h_train_bkg.get_bin_centers(), h_train_bkg.counts, yerr=h_train_bkg.errors, marker="o", linestyle="", markersize=2, linewidth=1.5, color="k",)

# train sig
counts_train_sig,_,_ = ax.hist(preds_sig_train, weights=weights_raw_train[y_train==1], bins=bins,histtype="stepfilled",alpha=0.45, density=density, label="sig, train",color="C3")
h_train_sig = Hist1D(preds_sig_train, weights=weights_raw_train[y_train==1], bins=bins)
h_train_sig *= float(counts_train_sig.sum()/h_train_sig.get_integral())
# ax.errorbar( h_train_sig.get_bin_centers(), h_train_sig.counts, yerr=h_train_sig.errors, marker="o", linestyle="", markersize=2, linewidth=1.5, color="k",)

# test bkg error bars
counts_test_bkg,_,_ = ax.hist(preds_bkg_test, weights=test_bkg_weights*weights_raw_test[y_test==0], bins=bins,histtype="step",alpha=1.0, density=density, label="bkg, test (KS prob = {:.2f})".format(pks_bkg),color="C0", lw=1.5, linestyle="solid")
h_test_bkg = Hist1D(preds_bkg_test, weights=test_bkg_weights*weights_raw_test[y_test==0], bins=bins)
h_test_bkg *= float(counts_test_bkg.sum()/h_test_bkg.get_integral())
ax.errorbar( h_test_bkg.get_bin_centers(), h_test_bkg.counts, yerr=h_test_bkg.errors, marker="o", linestyle="", markersize=2, linewidth=1.5, color="C0",)

# test sig error bars
counts_test_sig,_,_= ax.hist(preds_sig_test, weights=weights_raw_test[y_test==1], bins=bins,histtype="step",alpha=1.0, density=density, label="sig, test (KS prob = {:.2f})".format(pks_sig),color="C3", lw=1.5, linestyle="solid")
h_test_sig = Hist1D(preds_sig_test, weights=weights_raw_test[y_test==1], bins=bins)
h_test_sig *= float(counts_test_sig.sum()/h_test_sig.get_integral())
ax.errorbar( h_test_sig.get_bin_centers(), h_test_sig.counts, yerr=h_test_sig.errors, marker="o", linestyle="", markersize=2, linewidth=1.5, color="C3",)

# print h_train_bkg
# print h_test_bkg
# print h_train_sig
# print h_test_sig
# import ROOT as r
# def to_root(h, name):
#     edges = h._edges
#     counts = h._counts
#     errors = h._errors
#     h1 = r.TH1F(name,"",len(edges)-1,edges)
#     for i,(count,error) in enumerate(zip(counts,errors)):
#         h1.SetBinContent(i+1,count)
#         h1.SetBinError(i+1,error)
#     return h1
# h_train1 = to_root(h_train_bkg, "h_train1")
# h_train2 = to_root(h_train_sig, "h_train2")
# h_test1 = to_root(h_test_bkg, "h_test1")
# h_test2 = to_root(h_test_sig, "h_test2")
# print h_train1.KolmogorovTest(h_train2)
# print h_test1.KolmogorovTest(h_test2)




ax.legend()

ax.set_ylim([0.,ax.get_ylim()[1]])
fig.set_tight_layout(True)
fig.savefig("disc_run2.pdf")
os.system("which ic && ic disc_run2.pdf")
