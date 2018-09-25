
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

    # if (proc == "tttt")  tree_stype = 0;
    # if (proc == "ttw")   tree_stype = 1;
    # if (proc == "ttz")   tree_stype = 2;
    # if (proc == "tth")   tree_stype = 3;
    # if (proc == "fakes") tree_stype = 4;
    # if (proc == "flips") tree_stype = 5;
    # if (proc == "xg")    tree_stype = 6;
    # if (proc == "ttvv")  tree_stype = 7;
    # if (proc == "rares") tree_stype = 8;

stype = arrs["stype"]

# stype[stype >= 1] = 1
x_data = np.column_stack([arrs[name] for name in feature_names])

# throw away flips
keep = (stype != 5)
stype = stype[keep]
x_data = x_data[keep]
stype[stype >= 5] -= 1

y_data = stype

weights = np.ones(len(y_data))
for ic in np.unique(stype):
    weights[stype==ic] /= (stype==ic).sum()
weights /= weights.mean()

for ic in np.unique(stype):
    print ic,weights[stype==ic][:3]
print weights

print x_data.shape

        # weights_raw_train, weights_raw_test \
x_train, x_test, \
        y_train, y_test, \
        weights_train, weights_test, \
        = train_test_split(
                x_data,
                y_data,
                weights,
                # weights_raw,
                test_size=0.30, random_state=42,
                )


dtrain = xgb.DMatrix( x_train, label=y_train, weight=np.abs(weights_train))
dtest = xgb.DMatrix( x_test, label=y_test, weight=np.abs(weights_test))
# dtrain = xgb.DMatrix( x_train, label=y_train)
# dtest = xgb.DMatrix( x_test, label=y_test)
evallist  = [(dtrain,'train'), (dtest,'eval')]
# 2131,0.8930,0.8954,0.0024,2626,3.5 {'colsample_bytree': 0.5, 'silent': 1, 'eval_metric': 'auc', 'scale_pos_weight': 1.262414, 'nthread': 1, 'min_child_weight': 5.0, 'subsample': 0.6, 'eta': 0.04, 'alpha': 4.0, 'max_depth': 5, 'gamma': 2.0, 'lambda': 1.5}
num_round = 400
param = {}
param['objective'] = 'multi:softprob'
# param['objective'] = 'multi:softmax'
param['eta'] = 0.1
param['max_depth'] = 5
param['silent'] = 1
param['nthread'] = 10
param['eval_metric'] = "mlogloss" # https://github.com/dmlc/xgboost/issues/1143 NOTE
param['subsample'] = 0.6
param['num_class'] = len(np.unique(dtrain.get_label()))
param['class_labels'] = np.unique(dtrain.get_label()) # xgb doesn't use this. this is for us. see NOTE below
# print param
if not os.path.exists("bst_mc.pkl"):
    bst = xgb.train( param.items(), dtrain, num_round, evallist, early_stopping_rounds=20 )
    pickle.dump(bst,open("bst_mc.pkl","wb"))
    write_json("model_multiclass.json", bst, feature_names, class_labels=param["class_labels"])
    json_to_cfunc("model_multiclass.json", fname_out="func_mc.h")
else:
    print "[!] Found pickle file...loading"
    bst = pickle.load(open("bst_mc.pkl","rb"))


y_pred_train = bst.predict(dtrain)
y_pred = bst.predict(dtest)
# turn softprobs into softmax
y_pred = np.argmax(y_pred,axis=1)

# according to
# https://github.com/dmlc/xgboost/issues/1746 NOTE
# there will be Nclasses*Ntrees trees in the dump, there will be Nclasses trees for iteration 0, then Nclasses
# trees for iteration 1, ... up to total number of boosts/iterations/rounds
# the order of these classes matches np.unique(trainlabels) apparently



# import ROOT as r
# x0 = x_train[:1].tolist()[0]
# xgb_preds = bst.predict(xgb.DMatrix(x_train[:1]))[0]
# r.gROOT.ProcessLine(".L func_mc.h")
# cpp_preds = np.array(list(r.get_prediction(*x0)))
# print "XGB, CPP:"
# print xgb_preds.tolist(), xgb_preds.sum()
# print cpp_preds.tolist(), cpp_preds.sum()
# print (xgb_preds-cpp_preds).tolist()



# cv_result = xgb.cv(param, dtrain, num_round, nfold=5, seed=42, callbacks=[xgb.callback.early_stop(20)],as_pandas=False)
# print cv_result
# print dir(cv_result)
# y_pred = cv_result.predict(dtest)
# print dir(bst)
# print dir(xgb)

# write_json("model.json", bst, feature_names)

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
fig, ax = plt.subplots()

# from matplottery.plotter import set_defaults
# set_defaults()

def get_class_name(c):
    # if c == 0: return "tttt"
    # if c == 1: return "ttw"
    # if c == 2: return "ttz"
    # if c == 3: return "tth"
    # if c == 4: return "fakes"
    # if c == 5: return "flips"
    # if c == 6: return "xg"
    # if c == 7: return "ttvv"
    # if c == 8: return "rares"
    if c == 0: return "tttt"
    if c == 1: return "ttw"
    if c == 2: return "ttz"
    if c == 3: return "tth"
    if c == 4: return "fakes"
    if c == 5: return "xg"
    if c == 6: return "ttvv"
    if c == 7: return "rares"
    return "UNKNOWN"


from sklearn.metrics import confusion_matrix
import itertools
cm = confusion_matrix(y_test, y_pred)


normalize = True
classes = param["class_labels"]
print classes
if normalize:
    cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
plt.imshow(cm, interpolation='nearest', cmap="Blues")
plt.title("blah")
plt.colorbar()
tick_marks = np.arange(len(classes))
plt.xticks(tick_marks, map(get_class_name,classes), rotation=0)
plt.yticks(tick_marks, map(get_class_name,classes))

fmt = '.2f' if normalize else 'd'
thresh = 0.6 * cm.max()
for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
    plt.text(j, i, format(cm[i, j], fmt),
             horizontalalignment="center",
             verticalalignment="center",
             color="white" if cm[i, j] > thresh else "black")

plt.ylabel('True label')
plt.xlabel('Predicted label')

fig.set_tight_layout(True)

fig.savefig("mat.png")
os.system("which ic && ic mat.png")
