#!/bin/bash

FILENAME=$1

echo "    sweepRooting: \"$FILENAME\""

size=$(stat -c%s ${FILENAME})
if [ "$size" -lt 900 ]; then
    exit 1
fi


RESULT=`root -l -b -q sweeproot_macro.C+'("'$FILENAME'")' | grep "result=" | sed s/result=//g`

if [ "$RESULT" == "0" ]; then 
    exit 0
else
    exit 1
fi
