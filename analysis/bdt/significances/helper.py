import sys
import os
import commands
import tempfile
import numpy as np

def fix_spacing(ss):
    if type(ss) in [str]:
        ss = ss.split()
    return "".join(["{:<9s}".format(x) for x in ss])

def get_significance(proc_yields,proc_errors={},
        do_mcstats=True,
        print_card=False,
        delete_card=True,
        ):
    if not proc_errors:
        do_mcstats = False
    nbins = len(proc_yields["tttt"])

    ibins = range(1,nbins+1)
    proc_names = ["tttt"] + [x for x in sorted(proc_yields.keys()) if x != "tttt"]
    nproc = len(proc_names)

    bin_str = " ".join([" ".join(["bin{}".format(i)]*nproc) for i in ibins])
    proc1_str = (" ".join(proc_names) + " ") * nbins
    proc2_str = " ".join([" ".join(map(str,range(nproc))) for _ in ibins])
    rate_list = sum(map(list,zip(*[proc_yields[proc] for proc in proc_names])),[])
    rate_str = " ".join(["{:.2f}".format(e) for e in rate_list])

    buff = ""
    buff += "imax {}\n".format(nbins)
    buff += "jmax {}\n".format(nproc-1)
    buff += "kmax {}\n".format(nproc*nbins if do_mcstats else 0)
    buff += "------------\n"
    buff += "bin " + " ".join(["bin{}".format(i) for i in ibins]) + "\n"
    buff += "observation " + " ".join(["0" for i in ibins]) + "\n"
    buff += "------------\n"
    buff += "bin                      {}\n".format(fix_spacing(bin_str))
    buff += "process                  {}\n".format(fix_spacing(proc1_str))
    buff += "process                  {}\n".format(fix_spacing(proc2_str))
    buff += "rate                     {}\n".format(fix_spacing(rate_str))
    buff += "------------\n"
    if do_mcstats:
        for ibin in ibins:
            for iproc,proc in enumerate(proc_names):
                name = "stat_{}_bin{}".format(proc,ibin)
                buff += "{:<18s} lnN   ".format(name)
                logn = proc_errors[proc][ibin-1]/proc_yields[proc][ibin-1]+1
                if not np.isfinite(logn):
                    print ibin,proc,proc_errors[proc][ibin-1],proc_yields[proc][ibin-1], logn
                    logn = 1.0
                islot = (ibin-1)*nproc+iproc
                slots = sum([["-"]*nproc for i in ibins],[])
                slots[islot] = "{:.2f}".format(logn)
                buff += fix_spacing(slots)
                buff += "\n"
    if print_card:
        print buff

    temp = tempfile.NamedTemporaryFile(delete=delete_card)
    with open(temp.name, "w") as f:
        f.write(buff)
    # stat, out = commands.getstatusoutput("combine -M Significance --significance -t -1 --expectSignal=1 {} -S 0".format(temp.name))
    stat, out = commands.getstatusoutput("combine -M Significance --significance -t -1 --expectSignal=1 {}".format(temp.name))
    sig = -1.
    # try:
    if True:
        for line in out.splitlines():
            if line.startswith("Significance:"):
                sig = float(line.split(":")[-1].strip())
    if sig < 0:
        print "[!] Error with combine:"
        print out
    # except:
    #     pass
    return sig

if __name__ == "__main__":

    nbins = 15
    np.random.seed(42)
    proc_yields = {
            "tttt": np.abs(np.random.normal(1,1,nbins)),
            "tth": np.abs(np.random.normal(3,0.1,nbins)),
            "ttw": np.abs(np.random.normal(2,0.1,nbins)),
            "ttz": np.abs(np.random.normal(1,0.1,nbins)),
            }
    print get_significance(proc_yields) # should be 2.11517
    proc_errors = {
            "tttt": 0.1*np.abs(proc_yields["tttt"]),
            "tth": 0.15*np.abs(proc_yields["tth"]),
            "ttw": 0.25*np.abs(proc_yields["ttw"]),
            "ttz": 0.35*np.abs(proc_yields["ttz"]),
            }
    print get_significance_with_mcstats(proc_yields,proc_errors) # should be 2.03076
