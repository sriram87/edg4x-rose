#!/usr/bin/python

import os
from subprocess import call
import random
import subprocess
import re
import sys
import math

def main(argv):
  print "numLayers numDependencies varsPerLayer freshVars useSSA Time NumLines"
  for numLayers in range(2, 10) :
    for numDependencies in [1, 3, 10] :
      for varsPerLayer in [10, 30] :
        for freshVars in [0, 1] :
          for useSSA in [0] :
            os.system("~/Compilers/rose/projects/fuse/src/genArithStagedNetwork.py arithNetwork.C "+str(numLayers)+" "+str(numDependencies)+" "+str(varsPerLayer)+" "+str(freshVars)+" "+str(useSSA))
            proc = subprocess.Popen(["wc", "-l", "arithNetwork.C"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            numLines = -1
            for line in iter(proc.stdout.readline, b''):
              line = line.rstrip('\n')
              fields = line.split(" ")
              numLines = int(fields[0])

            proc = subprocess.Popen(["./fuse", "arithNetwork.C"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            for line in iter(proc.stdout.readline, b''):
              if line.find("ConstPropAnal Elapsed") >=0 :
                line = line.rstrip('\n')
                fields = line.split("=")
                print str(numLayers)+" "+str(numDependencies)+" "+str(varsPerLayer)+" "+str(freshVars)+" "+str(useSSA)+" "+fields[1].rstrip("s")+" "+str(numLines)
            proc.communicate()

if __name__ == "__main__":
  sys.argv.pop(0)
  main(sys.argv)
