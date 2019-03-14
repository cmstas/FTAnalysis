#!/usr/bin/env sh

mtarfile package.tar.xz --excludepatterns "*data/benchmarks/*"  "*.xlsx"  --xz --extrapaths $CMSSW_BASE/bin/slc6_amd64_gcc630/combine
