# -*- coding: UTF-8 -*-
import ROOT as r
import random

# get distribution of nhadronic taus from four top events
# use MC since it's faster than writing out the binomial equation terms
h_nhadtau = r.TH1F("nhadtau","nhadtau",5,-0.5,4.5)
for _ in range(100000):
    nlep = 0
    nhadtau = 0
    for iw in range(4):
        if random.random() < 0.11:  # BR(W->τ)
            if random.random() < 0.65:  # BR(τ->jj)
                nhadtau += 1
            nlep += 1
    h_nhadtau.Fill(nhadtau)

h_nhadtau.Scale(1./h_nhadtau.Integral())
print list(h_nhadtau)[1:-1]



