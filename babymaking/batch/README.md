## Making babies
<img src="http://i.imgur.com/6BFuXOh.png" width="500">

### So you want to develop the babymaker?
```bash
# Copy over CORE and stuff
./copy_files.sh
# Edit the babymaker source files or whatever
# Compile
./make -j10
# Test? Or don't, actually. We don't make mistakes.
./test_babymaker.sh
# Make a tar file to ship to the worker nodes
./make_tar.sh
```

### So you want to make some babies?
I know this guy -- great guy -- the best. He uses condor to deliver his babies instead of storks.
This is what he does
```bash
# Edit four_top.py with samples you want and increment tag near the bottom
# Edit ducks.py to build instructions with appropriate samples
# Open up a screen before proceeding
# Get environment
cd NtupleTools/AutoTwopler/
# edit `campaign` in `params.py` to be "FT_babymaker"
. setup.sh
cd ../../
# Submit
python ducks.py
```
