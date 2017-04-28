#!/usr/bin/env bash

#Make and fill CORE directory
mkdir -p CORE 
cp ../../common/CORE/*.{h,cc} CORE/

#And data directory inside CORE
mkdir -p CORE/data
cp ../../common/CORE/data/*.xml CORE/data/

#And tools directory inside CORE
mkdir -p CORE/Tools
cp -r ../../common/CORE/Tools CORE/

cp myCORE/* CORE/
