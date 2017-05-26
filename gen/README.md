## MC Generation
This directory contains configurations for the generation of rare SM background processes
for the measurement of SM 4-top production.

Right now we plan to generate the following processes at LO with no extra partons:
   * ttww
   * tttw
   * tttj
   * ttwz
   * ttzz
   * ttzh
   * tthh

For the production of ttww, we explicitly exclude diagrams containing a higgs, i.e. p p > t t~ w+ w- / h.

These processes will be normalized to NLO cross sections which are available for all except tttj at present:
   * sigma(ttww) ~ 10 fb
   * sigma(tttw) ~ 1 fb
   * sigma(tttj) ~ 0.4 fb
   * sigma(ttwz) ~ 3.9 fb
   * sigma(ttzz) ~ 2 fb
   * sigma(ttzh) ~ 1.5 fb
   * sigma(tthh) ~ 0.8 fb

It should be sufficient to generate 100k events of each.

These processes are all produced with no extra partons, so we will rely on pythia for additional radiation.
As a result, we'll also need a pythia ttbar sample to determine an appropriate ISR njet reweighting.
Included in this directory is a pythia fragment to generate a ttbar (>=1L) sample.
