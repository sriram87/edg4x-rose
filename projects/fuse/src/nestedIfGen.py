#!/usr/bin/python

import os
from subprocess import call
import random
import subprocess
import re
import sys
import math

funcCntr = 0
varCntr = 0
assertCntr = 0

def appendPathTest(pathTest, condTest) :
  if(pathTest=="") : return condTest
  else : return pathTest + " && " + condTest

def genFunc(fDepth, funcDepth, ifDepth, ifPerFuncDepth, globalVars, constInitVars, out, indent, pathTest) :
  global funcCntr, varCntr, assertCntr
  res = ""
  #print indent+"genFunc("+str(fDepth)+", "+str(funcDepth)+", "+str(ifDepth)+", "+str(ifPerFuncDepth)+", "+str(globalVars)+")"
  if(fDepth == funcDepth) :
    return {"funcName": "", "resStr": res, "resGlobalDecls": ""}
  else :
    funcName = "func"+str(funcCntr)
    #out.write(indent+"void "+funcName+"(bool pathTest) {\n")
    funcCntr+=1
    retIf = genIf(fDepth, funcDepth, 0, ifPerFuncDepth, globalVars, constInitVars, out, indent+"  ", pathTest)
    res += retIf["resDeeperFuncs"]
    res += indent+"static void "+funcName+"(bool pathTest) {\n"
    res += retIf["resCurFunc"]

    #out.write(indent+"}\n")
    res += indent+"}\n"

    
    return {"funcName": funcName, "resStr": res, "resGlobalDecls": retIf["resGlobalDecls"]}
    

def genIf(fDepth, funcDepth, ifDepth, ifPerFuncDepth, globalVars, constInitVars, out, indent, pathTest) :
  global funcCntr, varCntr, assertCntr
  #print indent+"genIf("+str(fDepth)+", "+str(funcDepth)+", "+str(ifDepth)+", "+str(ifPerFuncDepth)+", "+str(globalVars)+")"
  resCurFunc = ""
  resDeeperFuncs = ""
  resGlobalDecls = ""
  if(ifDepth == ifPerFuncDepth) :
    retF = genFunc(fDepth+1, funcDepth, 0, ifPerFuncDepth, globalVars, constInitVars, out, indent+"  ", "")
    resDeeperFuncs += retF["resStr"]
    resGlobalDecls += retF["resGlobalDecls"]
    #if(funcName): out.write(indent+funcName+"("+pathTest+");\n")
    if(retF["funcName"]): resCurFunc += indent+retF["funcName"]+"("+pathTest+");\n"
    else :
      #out.write("void CompDebugAssert"+str(assertCntr)+"(bool);\n")
      #out.write("CompDebugAssert"+str(assertCntr)+"(pathTest && "+pathTest+");\n")
      resCurFunc += indent+"void CompDebugAssert"+str(assertCntr)+"(bool);\n"
      resCurFunc += indent+"CompDebugAssert"+str(assertCntr)+"(pathTest && "+pathTest+");\n"
      assertCntr += 1
  else :
    #out.write(indent+"int var"+str(varCntr)+"="+str(varCntr)+";\n")
    if constInitVars :
      if globalVars: resGlobalDecls += indent+"int var"+str(varCntr)+"="+str(varCntr)+";\n"
      else :         resCurFunc    += indent+"int var"+str(varCntr)+"="+str(varCntr)+";\n"
    else:
      if globalVars: resGlobalDecls += indent+"int var"+str(varCntr)+";\n"
      else :         resCurFunc    += indent+"int var"+str(varCntr)+";\n"
    condTest = "var"+str(varCntr)+"=="+str(varCntr)
    #out.write(indent+"if("+condTest+") {\n")
    resCurFunc += indent+"if("+condTest+") {\n"
    varCntr+=1
    
    retIf1 = genIf(fDepth, funcDepth, ifDepth+1, ifPerFuncDepth, globalVars, constInitVars, out, indent+"  ", appendPathTest(pathTest, condTest));
    resCurFunc += retIf1["resCurFunc"]
    resDeeperFuncs += retIf1["resDeeperFuncs"]
    resGlobalDecls += retIf1["resGlobalDecls"]

    #out.write(indent+"} else {\n")
    resCurFunc += indent+"} else {\n"
    retIf2 = genIf(fDepth, funcDepth, ifDepth+1, ifPerFuncDepth, globalVars, constInitVars, out, indent+"  ", appendPathTest(pathTest, condTest));
    resCurFunc += retIf2["resCurFunc"]
    resDeeperFuncs += retIf2["resDeeperFuncs"]
    resGlobalDecls += retIf2["resGlobalDecls"]
    
    #out.write(indent+"}\n")
    resCurFunc += indent+"}\n"

  return {"resCurFunc": resCurFunc, "resDeeperFuncs": resDeeperFuncs, "resGlobalDecls": resGlobalDecls}

def main(argv):
  if(len(argv)!=5) : 
    sys.exit("Usage: nestedIfGen.py outFile funcDepth ifPerFuncDepth globalVars constInitVars")
  
  outFile        = argv[0]
  funcDepth      = int(argv[1])
  ifPerFuncDepth = int(argv[2])
  globalVars     = int(argv[3])
  constInitVars  = int(argv[4])
  
  with open(outFile, "w") as out:
    out.write("#pragma fuse lc(t(dp,cp))\n")
    retF = genFunc(0, funcDepth, 0, ifPerFuncDepth, globalVars, constInitVars, out, "", "")
    
    out.write(retF["resGlobalDecls"]);
    out.write(retF["resStr"]);

    out.write("int main() {\n")
    out.write("    "+retF["funcName"]+"(true);\n")
    out.write("    return 0;\n")
    out.write("}\n")

if __name__ == "__main__":
  sys.argv.pop(0)
  main(sys.argv)
