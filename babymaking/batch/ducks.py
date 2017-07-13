import sys
sys.path.insert(0,"NtupleTools/AutoTwopler")
import params as p
import four_top as ft

# make instructions
instructions = []
# types = ["data"]
# types = ["backgrounds"]
types = ["backgrounds","data"]
for typ in types:
    for ds in ft.d_ds2name[typ].keys():
        instructions.append({"executable": ft.executable, "package": ft.package, "analysis": "FT", "dataset": ds, "baby_tag": ft.tag, "type": "BABY", "extra": ""})

p.dataset_to_shortname = ft.dataset_to_shortname
p.dashboard_name = ft.dashboard_name
p.sweepRoot_scripts = ft.sweepRoot_scripts
p.merging_scripts = ft.merging_scripts
p.baby_merged_dir = ft.baby_merged_dir
p.exit_when_done = True
p.open_datasets = False

import run
run.main(instructions=instructions, params=p)
