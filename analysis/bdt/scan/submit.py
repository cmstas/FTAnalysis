from pprint import pprint

from metis.Utils import condor_q, get_hist, condor_submit

import math, sys, os
from tqdm import tqdm

if __name__ == "__main__":

    submission_tag = "v1"
    outputdir = "/hadoop/cms/store/user/namin/ft_bdt_scan/{}/".format(submission_tag)
    input_file = "/home/users/namin/2018/fourtop/all/FTAnalysis/analysis/bdt/scan/out.npz"
    scanlist = "allargs.txt"

    # os.system("mkdir -p {}".format(outputdir))

    my_jobs = condor_q(selection_pairs=[["BDTtag",submission_tag]],extra_columns=["job_num"])
    already_running = [int(job["job_num"]) for job in my_jobs]

    all_args = []
    with open(scanlist,"r") as fhin:
        for line in fhin:
            parts = line.strip().split("|")
            if len(parts) > 0:
                all_args.append(parts)
    v_sub_arguments = []
    v_sub_selpairs = []
    for job_num, pyargs in tqdm(enumerate(all_args)):
        # if job_num < 2000: continue
        # if job_num < 1409: continue
        if job_num in already_running: continue # don't resubmit if already running
        job_tag = "{}_{}".format(submission_tag,job_num)
        # FIXME rewrite this as a single glob into a set, and then check the set so we don't spam I/O
        if os.path.exists("{}/{}/history.pkl".format(outputdir, job_tag)): continue # don't resubmit if already finished
        arguments = [ outputdir, job_tag ] + pyargs
        v_sub_arguments.append(arguments)
        v_sub_selpairs.append([["BDTtag",submission_tag],["job_num",job_num]])
        # if job_num > 10:
        #     print "FIXME"
        #     break
    params = {
            "executable": "executable.sh",
            "arguments": v_sub_arguments,
            "inputfiles": ["runner.py",input_file],
            "logdir": "logs/",
            "selection_pairs": v_sub_selpairs,
            # "sites": "T2_US_UCSD,UAF",
            # "sites": "T2_US_UCSD",
            "sites": "T2_US_UCSD,T2_US_Nebraska,T2_US_MIT,T2_US_Caltech",
            "multiple": True,
            }
    succeeded, cid = condor_submit(**params)
    if succeeded:
        print "Submitted {} jobs to {}.".format(len(v_sub_arguments), cid)
    else:
        print "ERROR submitting {} jobs.".format(len(v_sub_arguments))


    all_statuses = [job["JobStatus"] for job in my_jobs]
    print "______ STATUS ______"
    if all_statuses:
        print get_hist(all_statuses)
