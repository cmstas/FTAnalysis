from itertools import izip, product
import random
import pickle
import os
import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
import numpy as np
import uproot
import xgboost as xgb
import time
from bayes_opt import BayesianOptimization

def prepare_data(fname_in, fname_out):
    f = uproot.open(fname_in)
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
    weights = arrs["weight"]*1.0
    # normalize weights to have abs avg of 1 for sig/bkg separately
    avg_weight_bkg = np.abs(weights[y_data == 0]).mean()
    avg_weight_sig = np.abs(weights[y_data == 1]).mean()
    weights[y_data == 0] *= 1.0/avg_weight_bkg
    weights[y_data == 1] *= 1.0/avg_weight_sig
    # clip weight for large weight events (mainly xg and fakes)
    weights[np.abs(weights) > 2.] *= 2.0/(weights[np.abs(weights) > 2.])
    # keep = np.abs(weights) < 5.0
    # stype = stype[keep]
    # y_data = y_data[keep]
    # x_data = x_data[keep]
    # weights = weights[keep]
    np.savez_compressed(fname_out,
                    x_data=np.array(x_data,dtype=np.float32),
                    y_data=np.array(y_data,dtype=np.float32),
                    weights=weights,
                    )
    print x_data.shape

def load_data(fname):
    data = np.load(fname)
    x_data = data["x_data"]
    y_data = data["y_data"]
    weights = data["weights"]
    dtrain = xgb.DMatrix( x_data, label=y_data, weight=np.abs(weights))
    return dtrain

def xgb_evaluate(params,xgtrain,nfold=6,num_rounds=2000,early_stop=30,seed=42):
    t0 = time.time()
    sumw_pos = np.abs(xgtrain.get_weight()[xgtrain.get_label()==1]).sum()
    sumw_neg = np.abs(xgtrain.get_weight()[xgtrain.get_label()==0]).sum()
    params["scale_pos_weight"] = sumw_neg/sumw_pos
    cv_result = xgb.cv(params, xgtrain, num_boost_round=num_rounds, nfold=nfold,
                       seed=seed,
                       callbacks=[xgb.callback.early_stop(early_stop)],as_pandas=False,verbose_eval=1)
    for k in cv_result.keys():
        cv_result[k] = np.array(cv_result[k])
    cv_result["time_per_tree"] = (time.time() - t0)/len(cv_result["train-auc-mean"])
    return cv_result
    # return cv_result['test-auc-mean'][-1]

def plot_history(fname,ndrop=3):
    info = pickle.load(open(fname,"r"))

    from matplottery.plotter import set_defaults
    set_defaults()
    
    import matplotlib as mpl
    mpl.use('Agg')
    import matplotlib.pyplot as plt
    fig, ax = plt.subplots()


    xs = 1.0*np.array(range(1,len(info["train-auc-mean"])+1))[ndrop:]
    train_mu = info["train-auc-mean"][ndrop:]
    test_mu = info["test-auc-mean"][ndrop:]
    train_sig = info["train-auc-std"][ndrop:]
    test_sig = info["test-auc-std"][ndrop:]

    ax.errorbar(xs,(1-train_mu),marker="o", markersize=3, label="1 - train AUC")
    ax.fill_between(xs, (1-train_mu-train_sig), (1-train_mu+train_sig), alpha=0.5, edgecolor='#1B2ACC', facecolor='#089FFF')
    ax.errorbar(xs,(1-test_mu) ,marker="o", markersize=3, label="1 - test AUC")
    ax.fill_between(xs, (1-test_mu-test_sig), (1-test_mu+test_sig), alpha=0.5, edgecolor='#CC4F1B', facecolor='#FF9848')

    axr = ax.twinx()
    axr.errorbar(xs,(train_mu-test_mu) ,marker="o", markersize=3, label="train-test",color="C3")

    ax.legend(loc="upper left")
    axr.legend(loc="upper center")

    ax.set_ylabel("1-AUC")
    ax.set_xlabel("ntrees")

    fig.set_tight_layout(True)

    fig.savefig("plot.png")
    os.system("which ic && ic plot.png")


if __name__ == '__main__':

    # prepare_data(fname_in="../make_inputs/output_total.root",fname_out="out.npz")
    # xgtrain = load_data("out.npz")

    # params = {
    #         'eval_metric': "auc",
    #         'nthread': 1,
    #         'silent': 1,
    #         # 'eta': (0.05,0.5),
    #         # 'max_depth': (2, 7),
    #         # 'min_child_weight': (0.5, 20),
    #         # 'colsample_bytree': (0.1, 1),
    #         # 'max_depth': (3, 8),
    #         # 'subsample': (0.4, 1),
    #         # 'gamma': (0, 10),
    #         # 'alpha': (0, 10),
    #         # 'lambda': (1, 4),
    # }
    # extra_params = dict(
    #         nfold=4,
    #         num_rounds=10,
    #         )
    # info = xgb_evaluate(params,xgtrain,**extra_params)
    # pickle.dump(info, open("history.pkl","w"))
    # print info["time_per_tree"]

    # plot_history("history.pkl",ndrop=10)

    pgrid = {
            # "eta": [0.08,0.1,0.15,0.2,0.3,0.4],
            # "max_depth": [3,4,5,6],
            # "min_child_weight": [0.5,1,1.5,2.5,5,10,15],
            # "colsample_bytree": [0.2,0.5,1],
            # "subsample": [ 0.5,0.6,0.8,0.9,1 ],
            # "gamma": [ 0,1,2,4,8 ],
            # "alpha": [ 0,1,2,4,8 ],
            # "lamb": [ 0.5,1,1.5,2,3,5 ],
            "eta": [0.04,0.06,0.08,0.09],
            "max_depth": [4,5,6],
            "min_child_weight": [1,1.5,2.5,5],
            "colsample_bytree": [0.5,1],
            "subsample": [ 0.6,0.8,0.9 ],
            "gamma": [ 0.1,0.5,1,2 ],
            "alpha": [ 1,2,4,8 ],
            "lamb": [ 1,1.5,2,3 ],
            }
    def my_product(dicts):
        return (dict(izip(dicts, x)) for x in product(*dicts.values()))
    products = list(my_product(pgrid))
    random.shuffle(products)

    all_args = []
    NMAX = 500
    for product in products[:NMAX]:
        # ignore some combinations that don't make sense
        args = []
        for k,v in product.items():
            if type(v) == bool and v == False: continue
            if type(v) == bool and v == True:
                args.append("--{}".format(k,v))
            else:
                args.append("--{} {}".format(k,v))
        all_args.append(args)

    for args in all_args:
        print " | ".join(args)

