#ifndef _VARIABLEIDUTILS_H
#define _VARIABLEIDUTILS_H

/*************************************************************
 * Copyright: (C) 2013 by Sriram Aananthakrishnan            *
 * Author   : Sriram Aananthakrishnan                        *
 * email    : aananthakris1@llnl.gov                         *
 *************************************************************/

#include "VariableIdMapping.h"
#include <set>

using namespace CodeThorn;

// NOTE: should change across other files
// if we change the underlying set container
typedef std::set<VariableId> VariableIdSet;

// some utility functions for sets of VariableId
// 

class VariableIdSetPrettyPrint
{
public:
  std::string static str(VariableIdSet& vset, VariableIdMapping& vidm);
  std::string static str(VariableIdSet& vset);
};

// utility function to union two VariableIdSet into rset
void set_union(const VariableIdSet& set1, const VariableIdSet& set2, VariableIdSet& rset);


#endif