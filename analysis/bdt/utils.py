import ast
import json
import numpy as np

def write_json(fname,bst,feature_names, class_labels=[]):
    buff = "[\n"
    trees = bst.get_dump()
    ntrees = len(trees)
    for itree,tree in enumerate(trees):
        prev_depth = 0
        depth = 0
        for line in tree.splitlines():
            depth = line.count("\t")
            nascending = prev_depth - depth
            (depth == prev_depth-1)
            # print ascending, depth, prev_depth
            prev_depth = depth
            parts = line.strip().split()
            padding = "    "*depth
            for iasc in range(nascending):
                buff += "{padding}]}},\n".format(padding="    "*(depth-iasc+1))
            if len(parts) == 1:  # leaf
                nodeid = int(parts[0].split(":")[0])
                leaf = float(parts[0].split("=")[-1])
                # print "leaf: ",depth,nodeid,val
                buff += """{padding}{{ "nodeid": {nodeid}, "leaf": {leaf} }},\n""".format(
                        padding=padding,
                        nodeid=nodeid,
                        leaf=leaf,
                        )
            else:
                nodeid = int(parts[0].split(":")[0])
                split, split_condition = parts[0].split(":")[1].replace("[f","").replace("]","").split("<")
                split = feature_names[int(split)]
                split_condition = float(split_condition)
                yes, no, missing = map(lambda x:int(x.split("=")[-1]), parts[1].split(","))
                # print "branch: ",depth,nodeid,split,split_condition,yes,no
                buff += """{padding}{{ "nodeid": {nodeid}, "depth": {depth}, "split": "{split}", "split_condition": {split_condition}, "yes": {yes}, "no": {no}, "missing": {missing}, "children": [\n""".format(
                        padding=padding,
                        nodeid=nodeid,
                        depth=depth,
                        split=split,
                        split_condition=split_condition,
                        yes=yes,
                        no=no,
                        missing=missing,
                        )
        for i in range(depth):
            padding = "    "*(max(depth-1,0))
            if i == 0:
                buff += "{padding}]}}".format(padding=padding)
            else:
                buff += "\n{padding}]}}".format(padding=padding)
            depth -= 1
        if itree != len(trees)-1:
            buff += ",\n"
    buff += "\n]"
    # print buff
    to_dump = {
            "trees": list(ast.literal_eval(buff)),
            "feature_names": feature_names,
            "class_labels": map(int,np.array(class_labels).tolist()), # numpy array not json serializable
            }
    with open(fname, "w") as fhout:
        json.dump(to_dump,fhout,indent=2)


def json_to_cfunc(fname_in,fname_out=None):
    with open(fname_in, "r") as fhin:
        data = json.loads(fhin.read())
        trees = data["trees"]
        feature_names = data["feature_names"]
        class_labels = data["class_labels"]
    def get_leaf(entry, depth=0):
        if "leaf" in entry: 
            return entry["leaf"]
        splitvar = entry["split"]
        splitval = entry["split_condition"]
        yesnode = [c for c in entry["children"] if c["nodeid"] == entry["yes"]][0]
        nonode = [c for c in entry["children"] if c["nodeid"] == entry["no"]][0]
        return "({} < {} ? {} : {})".format(splitvar, splitval, get_leaf(yesnode, depth=depth+1), get_leaf(nonode, depth=depth+1))
    buff = ""
    multi = False
    if len(class_labels) > 0:
        multi = True
        ntrees_per_class = len(trees) // len(class_labels)
        nclasses = len(class_labels)
    if multi:
        buff += "std::vector<float> get_prediction({}) {{\n".format(",".join(map(lambda x: "float {}".format(x), feature_names)))
        for ic in class_labels:
            buff += "  float w_{} = 0.;\n".format(ic)
        for itree,j in enumerate(trees):
            iclass = int(class_labels[itree % nclasses])
            buff += "  w_{} += {};\n".format(iclass,get_leaf(j))
        buff += "  float w_sum = {};\n".format("+".join("exp(w_{})".format(ic) for ic in class_labels))
        for ic in class_labels:
            buff += "  w_{0} = exp(w_{0})/w_sum;\n".format(ic)
        buff += "  return {{ {} }};\n".format(",".join("w_{}".format(ic) for ic in class_labels))
    else:
        buff += "float get_prediction({}) {{\n".format(",".join(map(lambda x: "float {}".format(x), feature_names)))
        buff += "  float w = 0.;\n"
        for itree,j in enumerate(trees):
            buff += "  w += {};\n".format(get_leaf(j))
        buff += "  return 1.0/(1.0+exp(-1.0*w));\n"
    buff += "}"
    if fname_out:
        with open(fname_out, "w") as fhout:
            fhout.write(buff)
    else:
        return buff
