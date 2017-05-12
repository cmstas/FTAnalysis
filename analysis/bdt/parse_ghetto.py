import json
import pprint
import numpy as np
from tqdm import tqdm

with open("model.json", "r") as fhin:
    js = json.loads(fhin.read())

def get_leaf(entry, depth=0):
    if "leaf" in entry: 
        return entry["leaf"]
    splitvar = entry["split"]
    splitval = entry["split_condition"]
    yesnode = [c for c in entry["children"] if c["nodeid"] == entry["yes"]][0]
    nonode = [c for c in entry["children"] if c["nodeid"] == entry["no"]][0]
    return "({} < {} ? {} : {})".format(splitvar, splitval, get_leaf(yesnode, depth=depth+1), get_leaf(nonode, depth=depth+1))

# print js
# for j in js:
#     print  j

# # 0.00678105
# vals = {
# "f0": 5.0, "f1": 1.0, "f2": 79.2425, "f3": 81.8827, "f4": 298.478,
# "f5": 46.3206, "f6": 60.5358, "f7": 2.0, "f8": 298.478, "f9": 150.899,
# "f10": 3.02887, "f11": 72.4613, "f12": 71.4765, "f13": 62.3212,
# "f14": 63.6879, "f15": 0.0, "f16": 1.46461, "f17": 0.771269,
# }

import variables
nfeatures = len(variables.cols)
colnames = ["float f"+str(ic) for ic in range(nfeatures)]
print "// float pred = get_prediction({});".format(",".join(["(float)tree_{}".format(c) for c in variables.cols]))
print "float get_prediction({}) {{".format(",".join(colnames))
print "  float w = 0.;"
for j in js:
    print "  w += {};".format(get_leaf(j))
print "  return 1.0/(1.0+exp(-1.0*w));"
print "}"


