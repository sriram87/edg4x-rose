#!/usr/bin/python

import os
from subprocess import call
import random
import subprocess
import re
import sys
import math

varCntr=0
def genVarID(idx, freshVars) :
  global varCntr
  
  if(freshVars) :
    varCntr += 1
    return varCntr
  else: 
    return idx

def varName(varID, layer) :
  if(layer%2==0) : return "varEven"+str(varID)
  else :           return "varOdd"+str(varID)

def main(argv):
  if(len(argv)!=6) :
    sys.exit("Usage: nestedIfGen.py outFile numLayers numDependencies varsPerLayer freshVars useSSA")

  outFile         = argv[0]
  numLayers       = int(argv[1])
  numDependencies = int(argv[2])
  varsPerLayer    = int(argv[3])
  freshVars       = int(argv[4])
  useSSA          = int(argv[5])

  assertCntr=0

  with open(outFile, "w") as out:
    #out.write("#include <assert.h>\n")
    if useSSA : out.write("#pragma fuse lc(SSA:cp)\n")
    else :      out.write("#pragma fuse lc(cp)\n")
    
    out.write("int main() {\n")
    
    # We'll maintain the values of the variables in the current and last layers
    vals = []
    lastVals = []
    
    # The IDs of the variables in the current layer
    varIDs = [-1] * varsPerLayer
    lastVarIDs = []
    
    # Initialize the values of the first layer
    for i in range(varsPerLayer) : vals.append(random.randrange(0, 100))
    
    # Generate the IDs of the variables in the first layer
    for i in range(varsPerLayer) : varIDs[i] = genVarID(i, freshVars)
    
    # Generate the variable assignments in the first layer
    for i in range(varsPerLayer) :
      out.write("    int "+varName(varIDs[i], 0)+"="+str(vals[i])+";\n")
    
    # Generate the subsequent layers
    for layer in range(1, numLayers) :
      lastVals = vals
      vals = [0] * varsPerLayer

      lastVarIDs = varIDs
      varIDs = [-1] * varsPerLayer

      # Generate the IDs of the variables in this layer
      for i in range(varsPerLayer) : varIDs[i] = genVarID(i, freshVars)
      
      # Generate the variable assignments in this layer
      for i in range(varsPerLayer) :
        # The value of this variable
        val = 0

        # Generate the dependencies of this variable on the variables from the prior layer
        out.write("    ")
        if(layer==1 or freshVars) : out.write("int ")
        out.write(varName(varIDs[i], layer)+"=")
        for j in range(numDependencies) :
          depIdx = random.randrange(0, varsPerLayer)
          val += lastVals[depIdx]
          if(j>0) : out.write("+")
          out.write(varName(lastVarIDs[depIdx], layer-1));
        out.write(";\n")
        vals[i] = val
        out.write("    void CompDebugAssert"+str(assertCntr)+"(bool);\n")
        out.write("    CompDebugAssert"+str(assertCntr)+"("+varName(varIDs[i], layer)+"=="+str(val)+");\n")
        assertCntr += 1
        #out.write("    assert("+varName(varIDs[i], layer)+"=="+str(val)+");\n")
      
    out.write("    return 0;\n")
    out.write("}\n")

if __name__ == "__main__":
  sys.argv.pop(0)
  main(sys.argv)


