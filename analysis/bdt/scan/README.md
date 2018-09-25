## Submitting to condor

### Generating scan list
Batch submission starts by generating a list of command line
options for `runner.py`. This is done through the `permute.py`
script. Edit in any new parameters at the top, along with a 
list of available options to scan over. Specify `NMAX` which
is the max number of random permutations to spit out. Next,
take note that some combinations of options don't make sense,
so ignore them where we say `ignore some combinations that don't
make sense`. This script prints out command line flags to stdout
so invoke with `python permute.py >> allargs.txt`. Alternatively,
you can just insert arguments here by hand.

### Submitting scan list
Submission happens in `condor.py`. At the top, specify a 
tag for this batch of submissions. Specify the hadoop output directory
and specify the input file, which should live on hadoop.
If you didn't name your scan list `allargs.txt`, then change it
in this file.

Run this script as `python condor.py`.
If all goes well, you'll see one submission per line in
your scan list. Running it again will ignore scans that exist
on hadoop (done), or scans that are on condor (running)

### Analyzing outputs
Relevant script is `analyze.py`. After copying the pickle
outputs into a folder locally (for speed reasons), use
the commented code block at the top of the script to combine
them into a single pickle file. Afterwards, you can comment it
out to avoid re-combining them every time you run the script.
