from tqdm import tqdm
import os
import sys
from glob import glob
import numpy as np
import pickle

# fnames = glob("histories/*pkl")
# total = 0
# d_total = {}
# for fname in tqdm(fnames):
#     num = int(fname.rsplit("_",1)[-1].split(".",1)[0])
#     try:
#         data = pickle.load(open(fname,"r"))
#     except:
#         continue
#     total += 1
#     d_total[num] = data
# print "Found {} total pickle files".format(total)
# pickle.dump(d_total, open("total_info.pkl","w"))


all_data = pickle.load(open("total_info.pkl","r"))
print len(all_data.keys())

def get_avg_auc(x,which="test"):
    return x["{}-auc-mean".format(which)][-10:].mean()
    # return x["{}-auc-mean".format(which)][-1]

print get_avg_auc(all_data[all_data.keys()[0]])

temp = sorted(all_data.items(), key=lambda y: -get_avg_auc(y[1]))

# for num,d in temp[:5]:
#     print num, get_avg_auc(d,"train"), get_avg_auc(d,"test"), d["time_per_tree"], d["params"]

points = np.array([ [get_avg_auc(x,"train"),get_avg_auc(x,"test"),x["params"]["eta"]] for num,x in all_data.items() ])
print points

# for k,v in all_data.items():
for k,v in temp:
    train_auc, test_auc, params = get_avg_auc(v,"train"), get_avg_auc(v,"test"), v["params"]
    ntrees,time_tree = len(v["test-auc-mean"]), v["time_per_tree"]
    if train_auc > 0.8955: continue
    if test_auc < 0.8929: continue
    # if test_auc < 0.8935: continue
    print "{},{:.4f},{:.4f},{:.4f},{},{:.1f}".format(k,test_auc,train_auc,train_auc-test_auc,ntrees,time_tree),params

import sys
sys.path.insert(0,'/home/users/namin/.local/lib/python2.7/site-packages/')
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
fig, ax = plt.subplots()

# 12:17:45 PM | namin@uaf-10 | ~/2018/fourtop/all/FTAnalysis/analysis/bdt > cat log_bo_v4.txt | grep "train-auc"
to_parse = """
[502]	train-auc:0.902945+0.000277264	test-auc:0.894863+0.000330881
[144]	train-auc:0.901776+2.62619e-05	test-auc:0.896826+0.000210218
[754]	train-auc:0.902236+0.000103482	test-auc:0.897302+0.000142424
[237]	train-auc:0.905066+0.0001659	test-auc:0.896859+0.000136434
[345]	train-auc:0.901973+0.000135728	test-auc:0.896796+0.000204634
[1930]	train-auc:0.900717+3.88941e-05	test-auc:0.896708+0.000155047
[578]	train-auc:0.903337+5.69402e-05	test-auc:0.896556+0.000126989
[618]	train-auc:0.903293+6.69477e-05	test-auc:0.897338+0.000190842
[391]	train-auc:0.907392+0.000218189	test-auc:0.895898+0.000234916
[188]	train-auc:0.902572+7.26477e-05	test-auc:0.896964+0.000183563
[328]	train-auc:0.903221+0.00010563	test-auc:0.897342+0.000200009
[420]	train-auc:0.904826+4.05802e-05	test-auc:0.897264+0.000240205
[96]	train-auc:0.903534+0.000158892	test-auc:0.896757+0.000186615
[721]	train-auc:0.903661+8.98203e-05	test-auc:0.897268+0.000205784
[458]	train-auc:0.900145+4.74993e-05	test-auc:0.896293+0.000154496
[678]	train-auc:0.905687+6.23659e-05	test-auc:0.89734+0.000163327
[1157]	train-auc:0.901988+6.08661e-05	test-auc:0.897434+0.000204863
[1004]	train-auc:0.902155+5.1415e-05	test-auc:0.897355+0.000211327
[947]	train-auc:0.903976+0.000122928	test-auc:0.897211+0.0001325
[572]	train-auc:0.904604+8.7946e-05	test-auc:0.897266+0.000177232
[911]	train-auc:0.904201+3.94675e-05	test-auc:0.897353+0.000180889
[1069]	train-auc:0.90373+4.67707e-05	test-auc:0.897424+0.000151591
[344]	train-auc:0.90369+0.000147801	test-auc:0.897117+0.000227952
[397]	train-auc:0.902872+9.32577e-05	test-auc:0.897265+0.000256257
[1088]	train-auc:0.902784+5.33497e-05	test-auc:0.897363+0.000141367
[771]	train-auc:0.905346+9.7103e-05	test-auc:0.897435+0.000136881
[741]	train-auc:0.905476+2.20497e-05	test-auc:0.897406+0.000157877
"""

more_points = []
for line in to_parse.splitlines():
    parts = line.split()
    if not parts: continue
    train_auc = float(parts[1].split(":")[1].split("+")[0])
    test_auc = float(parts[2].split(":")[1].split("+")[0])
    more_points.append([train_auc,test_auc])
more_points = np.array(more_points)
print more_points
print more_points.shape

# train bkg
# ax.hist(preds_bkg_train, weights=weights_raw_train[y_train==0], bins=bins,histtype="stepfilled",alpha=0.45, density=density, label="bkg, train",color="C0")
# for i in [3,4,5,6]:
# for i in [ 0.5,1,1.5,2,3,5 ]:
# for i in [0.08,0.1,0.15,0.2,0.3,0.4]:
#     ax.scatter(points[:,0][points[:,2] == i], points[:,1][points[:,2] == i], s=3.0, alpha=0.5, label="depth = {}".format(i))
# points = points[points[:,1] > 0.893]
ax.scatter(points[:,0], points[:,1], s=10.0, alpha=0.8, label="naive condor")
ax.scatter(more_points[:,0], more_points[:,1], s=25.0, alpha=0.8, label="bayesian opt")
# ax.set_ylim([0.9999*points[:,1].min(),1.0001*points[:,1].max()])
ax.plot([points[:,0].min(),points[:,0].max()],[points[:,0].min(),points[:,0].max()],label="train=test")
ax.set_xlabel("train auc")
ax.set_ylabel("test auc")


ax.legend()

# ax.set_ylim([0.,ax.get_ylim()[1]])
fig.savefig("plot.png")
os.system("which ic && ic plot.png")

# d_args = {}

# best_mudmll = 999.
# best_args = None
# ranked_args = []
# for tag,data in all_data.items():
#     # print tag, data
#     # print data.keys()
#     mudmll = data["mu_deltamlls"]
#     sigdmll = data["sig_deltamlls"]
#     args = data["args"]
#     del args["input_file"]
#     del args["nepochs_max"]
#     del args["output_size"]

#     absmudmll = abs(float(mudmll))
#     if not (absmudmll < 10): continue
#     if (sigdmll-5.77513) < 1e-4: continue

#     # if absmudmll < best_mudmll:
#     #     best_mudmll = absmudmll
#     #     best_args = args

#     ranked_args.append( [absmudmll, sigdmll, args] )

#     # print "HERE"
#     # print mudmll, args
#     # print "HERE"

#     for argpair in args.items():
#         if argpair[0] in ["tag","input_file","nepochs_max"]: continue
#         if argpair not in d_args: d_args[argpair] = 0 
#         d_args[argpair] += 1./absmudmll

#     # break
# for item in sorted(d_args.items(),key=lambda x: x[1], reverse=True):
#     print "{:20s} {:20s} {}".format(str(item[0][0]),str(item[0][1]),item[1])
# # print d_args

# def args_to_str(args):
#     return " ".join(["--{} {}".format(k,v) for k,v in args.items()])

# for mudmll,sigdmll,args in sorted(ranked_args)[:50]:
#     print mudmll, sigdmll,args_to_str(args)
# # print "best >>>"
# # print best_mudmll
# # print args_to_str(best_args)
# # print "<<< best"
