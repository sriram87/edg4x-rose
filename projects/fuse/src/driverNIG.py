#!/usr/bin/python

import os
from subprocess import call
import random
import subprocess
import re
import sys
import math

def main(argv):
  print "funcDepth ifPerFuncDepth globalVars constInitVars Time"
  for constInitVars in [1,0] :
    for funcDepth in range(4) :
      for ifPerFuncDepth in range(4) :
        for globalVars in [0,1] :
          os.system("/g/g15/bronevet/Compilers/rose/projects/fuse/src/nestedIfGen.py nestedIf.C "+str(funcDepth)+" "+str(ifPerFuncDepth)+" "+str(globalVars)+" "+str(constInitVars))
          proc = subprocess.Popen(["./fuse", "nestedIf.C"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
          for line in iter(proc.stdout.readline, b''):
            if line.find("TightComposer Elapsed") >=0 :
              line = line.rstrip('\n')
              fields = line.split("=")
              print str(funcDepth)+" "+str(ifPerFuncDepth)+" "+str(globalVars)+" "+str(constInitVars)+" "+fields[1]
          proc.communicate()

if __name__ == "__main__":
  sys.argv.pop(0)
  main(sys.argv)
