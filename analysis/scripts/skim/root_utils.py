from ROOT import *
from tqdm import tqdm

def copy_events(fname_in, fname_out="unique.root", treename="t"):


    d_re = set()


    ch = TChain(treename)
    ch.Add(fname_in)
    n_entries = ch.GetEntries()

    newFile = TFile(fname_out,"RECREATE") 
    ch_new = ch.CloneTree(0) 

    num_fill = 0
    for i in tqdm(range(n_entries)):
        ch.GetEntry(i)
        evt = ch.event
        run = ch.run

        if (int(run), int(evt)) in d_re: continue

        d_re.add( (int(run),int(evt)) )

        ch_new.Fill() 
        num_fill += 1
 
    ch_new.GetCurrentFile().Write() 
    ch_new.GetCurrentFile().Close()

    return num_fill


if __name__ == "__main__":


    # f = TFile("skim.root")
    print copy_events("skim.root")

