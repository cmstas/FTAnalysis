import re
import numpy as np
import uproot
from matplottery import Hist1D


# basedir = "v3.26_feb22_small_v1/"
basedir = "v3.26_feb27_allsigs_v1/"

bg_procs = ["ttw","ttz","tth","xg","ww","wz","rares","fakes","flips"]
regions = ["srhh","srhl","srll","srml","srlm"]
years = [2016,2017,2018]

def sum_dicts(ds):
    d0 = ds[0]
    for d in ds[1:]:
        for k,v in d.items():
            d0[k] += v
    return d0

def get_yields_for_proc(proc):
    d_yields = {}
    for region in regions:
        for year in years:
            fname = "{basedir}/{proc}_histos_{region}_{year}.root".format(basedir=basedir,proc=proc,region=region,year=year)
            f = uproot.open(fname)
            hc = Hist1D(f.get("sr",None)).counts
            if region not in d_yields:
                d_yields[region] = hc
            else:
                d_yields[region] += hc
    return d_yields

def get_highest_sb(sig_yields,bg_yields,num=10):
    srnames = []
    vals = []
    for k in sig_yields:
        s = sig_yields[k]
        b = bg_yields[k]
        ind = np.arange(1,len(s)+1)
        srs = [k.replace("sr","").upper()+str(i) for i in ind]
        srnames.extend(srs)
        sb = np.nan_to_num(s/(s+b)**0.5)
        vals.append(sb)
    srnames = np.array(srnames)
    vals = np.concatenate(vals)
    # print srnames
    # print vals
    return list(srnames[np.argsort(vals)[::-1]][:num])

if __name__ == "__main__":


    sigs = """
fs_t1qqqql_m1600_m1
fs_t1qqqql_m2400_m1
rpv_t1tbs_m1700
rpv_t1tbs_m1200
fs_t1tttt_m1550_m1150
fs_t1tttt_m1550_m1100
fs_t1tttt_m1300_m1050
fs_t1tttt_m1300_m1075
fs_t1tttt_m1400_m400
fs_t1tttt_m2000_m100
fs_t1tttt_m1800_m100
fs_t1tttt_m1800_m1000
fs_t1tttt_m1600_m600
fs_t6ttww_m1000_m600
fs_t6ttww_m900_m400
fs_t6ttww_m800_m400
fs_t5qqqqvv_m1400_m1
fs_t5qqqqvv_m900_m600
fs_t5qqqqww_m1400_m1
fs_t5qqqqww_m900_m600
fs_t5qqqqvvdm20_m1400_m1
fs_t5qqqqvvdm20_m900_m600
fs_t5qqqqwwdm20_m1400_m1
fs_t5qqqqwwdm20_m900_m600
fs_t6tthzbrz_m850_m625
fs_t6tthzbrb_m850_m625
fs_t6tthzbrh_m850_m625
    """.strip().splitlines()

    bg_yields = sum_dicts(map(get_yields_for_proc,bg_procs))
    info = []
    for sig in sigs:
        sig_yields = get_yields_for_proc(sig)
        print sig, get_highest_sb(sig_yields,bg_yields)
        info.append([sig, get_highest_sb(sig_yields,bg_yields)])



    def nicer(x):
        x = x.replace("rpv_","").replace("fs_","")
        x = x.replace("qqqql","qqqqL")
        x = x.replace("t1","T1")
        x = x.replace("t5","T5")
        x = x.replace("t6","T6")
        x = x.replace("ww","WW")
        x = x.replace("vv","VV")
        x = x.replace("hz","HZ")
        x = x.replace("dm20"," (dM20)")
        return x

    # info = [
    #         ["fs_t1qqqql_m2000_m1", ['HH62', 'LM11', 'HH59', 'HH53', 'HH51', 'HH52', 'HH50', 'HL43', 'HH61', 'HL42']],
    #         ["rpv_t1tbs_m1600", ['HH62', 'HH50', 'HH59', 'HH52', 'HL42', 'HL39', 'LM11', 'HH61', 'HH56', 'HH58']],
    #         ["fs_t1tttt_m1400_m400", ['ML21', 'HH53', 'HH52', 'HH51', 'HH50', 'HL39', 'ML20', 'HH45', 'ML19', 'HH44']],
    #         ["fs_t1tttt_m2000_m100", ['HH53', 'HH52', 'ML21', 'HL39', 'HH49', 'HH51', 'ML20', 'HH50', 'HH62', 'HH48']],
    #         ["fs_t1tttt_m1800_m100", ['HH53', 'ML21', 'HH52', 'HL39', 'HH51', 'HH49', 'HH50', 'ML20', 'HH62', 'ML19']],
    #         ["fs_t1tttt_m1800_m1000", ['ML21', 'HH53', 'HH52', 'HH51', 'HH50', 'HL39', 'HH45', 'HH44', 'ML20', 'HL38']],
    #         ["fs_t1tttt_m1600_m600", ['ML21', 'HH53', 'HH52', 'HH51', 'HH50', 'HL39', 'ML20', 'HH49', 'ML19', 'ML44']],
    #         ["fs_t6ttww_m1000_m600", ['ML21', 'HH53', 'HH51', 'HH50', 'HH52', 'ML20', 'HH49', 'HH34', 'HH47', 'HH22']],
    #         ["fs_t6ttww_m900_m400", ['ML21', 'HH51', 'HH50', 'HH53', 'ML20', 'HH52', 'HH34', 'HH22', 'ML19', 'HH49']],
    #         ["fs_t6ttww_m800_m400", ['ML21', 'HH51', 'HH50', 'HH34', 'ML20', 'HH22', 'HH53', 'HH47', 'HH52', 'ML19']],
    #         ["fs_t5qqqqvv_m1400_m1", ['ML44', 'HH53', 'HH52', 'HH51', 'HH49', 'ML42', 'HH50', 'HH47', 'HH48', 'HH10']],
    #         ["fs_t5qqqqvv_m900_m600", ['ML25', 'HH3', 'HH10', 'ML44', 'HH4', 'ML29', 'HH50', 'HH51', 'HH7', 'ML42']],
    #         ["fs_t5qqqqww_m1400_m1", ['HH53', 'HH52', 'HH49', 'HH51', 'HH50', 'HH47', 'HH48', 'HH10', 'HH46', 'HH62']],
    #         ["fs_t5qqqqww_m900_m600", ['HH3', 'HH10', 'HH4', 'HH7', 'HH50', 'HH51', 'HH47', 'HH1', 'HH22', 'HH2']],
    #         ["fs_t5qqqqvvdm20_m1400_m1", ['HH59', 'HH53', 'HH52', 'HH62', 'HH51', 'HH50', 'HH56', 'HH49', 'HL39', 'HH47']],
    #         ["fs_t5qqqqvvdm20_m900_m600", ['LL2', 'LL1', 'LL4', 'HL39', 'HL37', 'HL35', 'HL34', 'HL7', 'LL6', 'HL6']],
    #         ["fs_t5qqqqwwdm20_m1400_m1", ['HH59', 'HH53', 'HH52', 'HH51', 'HH62', 'HH50', 'HH56', 'HH49', 'HL39', 'HH47']],
    #         ["fs_t5qqqqwwdm20_m900_m600", ['LL2', 'LL4', 'HL39', 'LL1', 'HL37', 'HL35', 'HL7', 'LL6', 'HL10', 'HH52']],
    #         ]

    for proc, bins in info:
        model = nicer(proc.split("_m",1)[0])
        masses = map(int,re.findall(r"_m([0-9]+)", proc))
        if any(x in proc for x in ["t1qqqql","rpv"]):
            masses = masses[:1]
        smass = ", ".join(map(lambda x:str(x).rjust(4),masses))
        print "{:<17s} {:<15s} {}".format(model,smass,"  ".join(map(lambda x:x.ljust(4), bins)))
