#!/usr/bin/env bash

for model in t1tttt t6ttww t5qqqqvv t5qqqqvvdm20 t5qqqqww t5qqqqwwdm20 t6tthzbrh t6tthzbrb t6tthzbrz t5tttt t5ttcc t1ttbb; do
    for sigstr in "" "--sig"; do
        echo python plotSMS.py -m $model $sigstr
        python plotSMS.py -m $model $sigstr
    done
done
