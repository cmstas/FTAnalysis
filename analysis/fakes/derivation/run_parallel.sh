#!/usr/bin/env bash

# for fname in `ls .parallel*C`; do
# for fname in `ls runners/parallel*C`; do
for fname in `ls runners/parallel*_iso.C`; do
# for fname in `ls runners/parallel*2018*{dy,wjets,ttjets}.C`; do
# for fname in `ls .parallel*qcd*mu*C`; do
# for fname in `ls .parallel*ttjets{,_iso}.C`; do
    logname=$(echo $fname | sed 's/\.C/.log/')
    echo "$fname --> $logname"
    root -b -q -l ${fname} >& ${logname} &
    sleep 1m
done
