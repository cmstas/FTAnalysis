import random
import argparse
import pickle
import os
import sys
import numpy as np
import xgboost as xgb
import time
from pprint import pprint

def load_data(fname):
    data = np.load(fname)
    x_data = data["x_data"]
    y_data = data["y_data"]
    weights = data["weights"]
    dtrain = xgb.DMatrix( x_data, label=y_data, weight=np.abs(weights))
    return dtrain

def xgb_evaluate(params,xgtrain,nfold=4,num_rounds=2000,early_stop=20,seed=42):
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


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument("tag", help="run tag for bookkeeping")
    parser.add_argument("--eta", help="eta", default=0.3, type=float)
    parser.add_argument("--max_depth", help="max_depth", default=6, type=int)
    parser.add_argument("--min_child_weight", help="min_child_weight", default=1, type=float)
    parser.add_argument("--colsample_bytree", help="colsample_bytree", default=1, type=float)
    parser.add_argument("--subsample", help="subsample", default=1, type=float)
    parser.add_argument("--gamma", help="gamma", default=0, type=float)
    parser.add_argument("--alpha", help="alpha", default=0, type=float)
    parser.add_argument("--lamb", help="lamb", default=1, type=float)

    args = parser.parse_args()

    xgtrain = load_data("out.npz")

    params = {
            'eval_metric': "auc",
            'nthread': 1,
            'silent': 1,
    }
    params.update({
            "eta": args.eta,
            "max_depth": args.max_depth,
            "min_child_weight": args.min_child_weight,
            "colsample_bytree": args.colsample_bytree,
            "subsample": args.subsample,
            "gamma": args.gamma,
            "alpha": args.alpha,
            "lambda": args.lamb,
            })
    pprint(params)
    extra_params = dict(
            # nfold=4,
            # num_rounds=10,
            )

    info = xgb_evaluate(params,xgtrain,**extra_params)
    info["params"] = params
    pickle.dump(info, open("history_{}.pkl".format(args.tag),"w"))


