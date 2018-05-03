import json

data = json.load(open("impacts.json","r"))

new_params = []
for param in data["params"]:
    name = param["name"]
    if "_stat_" in name or "_statshape_"in name:
        continue
    new_params.append(param)
data["params"] = new_params

json.dump(data, open("impacts_nostat.json","w"))
    # print key
