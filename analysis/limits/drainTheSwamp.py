#!/usr/bin/env python

import os

def main():
    os.system("ps ux | grep combine | awk '{print $2}' | xargs kill -9")
    os.system("ps ux | grep roostats | awk '{print $2}' | xargs kill -9")
    os.system("rm -f roostats*root")

if __name__ == "__main__":
    # This is obviously meant to be a bash script,
    # but losers use bash and winners use python.
    # Hint: Python and presidency both start with the same
    # letter. 

    main()
    
