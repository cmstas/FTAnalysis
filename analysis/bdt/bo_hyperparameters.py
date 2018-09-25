
import os
import sys
import numpy as np
# pip install --user uproot
import uproot
import xgboost as xgb
# pip install --user bayesian-optimization
from bayes_opt import BayesianOptimization

def xgb_evaluate(eta,
                 min_child_weight,
                 colsample_bytree,
                 max_depth,
                 subsample,
                 gamma,
                 lamb,
                 alpha):

    params['min_child_weight'] = int(min_child_weight)
    params['colsample_bytree'] = max(min(colsample_bytree, 1), 0)
    params['max_depth'] = int(max_depth)
    params['subsample'] = max(min(subsample, 1), 0)
    params['gamma'] = max(gamma, 0)
    params['alpha'] = max(alpha, 0)
    params['lambda'] = max(lamb, 0)
    params['eta'] = max(eta, 0)

    sumw_pos = np.abs(xgtrain.get_weight()[xgtrain.get_label()==1]).sum()
    sumw_neg = np.abs(xgtrain.get_weight()[xgtrain.get_label()==0]).sum()
    params["scale_pos_weight"] = sumw_neg/sumw_pos

    cv_result = xgb.cv(params, xgtrain, num_boost_round=num_rounds, nfold=4,
             seed=random_state,
             callbacks=[xgb.callback.early_stop(20)])

    return cv_result['test-auc-mean'].values[-1]


def prepare_data():
    f = uproot.open("make_inputs/output_total.root")
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
    dtrain = xgb.DMatrix( x_data, label=y_data, weight=np.abs(weights))
    return dtrain

if __name__ == '__main__':
    xgtrain = prepare_data()

    num_rounds = 2000
    random_state = 42
    num_iter = 40
    init_points = 7
    params = {
            'objective': 'binary:logistic',
            'eta': 0.1,
            'silent': 1,
            'eval_metric': 'auc',
            'verbose_eval': True,
            'seed': random_state,
            }

    xgbBO = BayesianOptimization(xgb_evaluate, {
                                                'eta': (0.025,0.2),
                                                'min_child_weight': (1, 10),
                                                'colsample_bytree': (0.1, 1),
                                                'max_depth': (5.2, 6.5),
                                                'subsample': (0.5, 0.9),
                                                'lamb': (1, 4),
                                                'gamma': (0, 10),
                                                'alpha': (0, 10),
                                                })

    xgbBO.maximize(init_points=init_points, n_iter=num_iter)
    xgbBO.points_to_csv("data_bo.csv")

