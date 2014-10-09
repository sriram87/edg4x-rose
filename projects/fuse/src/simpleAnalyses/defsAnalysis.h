#pragma once
#include "sage3basic.h"
#include "compose.h"
#include "VirtualCFGIterator.h"
#include <boost/make_shared.hpp>
#include "sight_verbosity.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "sageInterface.h"
using namespace SageInterface;

#include "midend/abstractLayer/VariableIdMapping.h"
#include "midend/abstractLayer/RDAstAttributeInterface.h"
#include "midend/abstractLayer/AstAnnotator.h"
#include "midend/abstractLayer/Labeler.h"

namespace fuse {

template<typename mapFunctor>
void mapPath(mapFunctor& f, CFGNode start, CFGNode end) {
  CFGIterator cur = CFGIterator::begin(start);
  while(1){
    f(*cur);
    if(*cur==end) return;
    cur++;
    if(*cur==end) return;
  }
}

class DefsAnalysis {
  Composer* composer;
  checkDataflowInfoPass* cdip;
  VariableIdMapping& vIDMap;
  // The set of definitions to be filled during the course of the analysis
  VariableIdMapping::VariableIdSet& defs;
  public:
  DefsAnalysis(Composer* composer, checkDataflowInfoPass* cdip, VariableIdMapping& vIDMap, VariableIdMapping::VariableIdSet& defs) : 
      composer(composer), cdip(cdip), vIDMap(vIDMap), defs(defs) {}
 
  void addDef(SgExpression* expr, PartEdgePtr pedge);
  
  void operator()(const CFGNode& n);

  void runAnalysis(SgNode* target);
}; // class DefsAnalysis

class FuseRDAstAttribute : public RDAstAttributeInterface {
  Composer* composer;
  checkDataflowInfoPass* cdip;
  VariableIdMapping& vIDMap;
  SgNode* target;
  
  // The set of variables defined at the labels this attribute it attached to
  VariableIdMapping::VariableIdSet defs;
  
  // Maps labels in the AST to a reference to the set of defs at the label.
  static std::map<Label, VariableIdMapping::VariableIdSet> label2defsMap;
  // Alternate representation that keeps the same data a set of pairs
  static std::set<std::pair<Label, VariableId> > label2defsSet;
  
  // All the variables that were defined
  static std::set<VariableId> allDefs;
  
  // Maps each variableId to all of its definition labels
  static std::map<VariableId, LabelSet> def2labels;
  
  // All the labels that were annotated
  static LabelSet allLabelsSet;
  
  public:
  FuseRDAstAttribute(Composer* composer, checkDataflowInfoPass* cdip, VariableIdMapping& vIDMap, Label label, Labeler& labeler);
  ~FuseRDAstAttribute();
  
  public:
  
  bool isBottomElement() { assert(0); }
  
  VariableIdSet allVariableIds() {
    return allDefs;
  }
  
  LabelSet allLabels() {
    return allLabelsSet;
  }
  
  LabelSet definitionsOfVariableId(VariableId varId) {
    return def2labels[varId];
  }
  
  VariableIdSet variableIdsOfDefinition(Label def) {
    return label2defsMap[def];
  }
  
  iterator begin() { return label2defsSet.begin(); }
  iterator end() { return label2defsSet.end(); }
  
  string toString();
  
  static void placeLabeler(Composer* composer, checkDataflowInfoPass* cdip, VariableIdMapping& vIDMap, Labeler& labeler);
}; // class FuseRDAstAttribute

}; // namespace fuse