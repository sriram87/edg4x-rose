#ifndef _ATS_TO_DOT_ANALYSIS_H
#define _ATS_TO_DOT_ANALYSIS_H

/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

#include "compose.h"

class AtsToDotLattice : public FiniteLattice {
public:
  AtsToDotLattice();
  void initialize();
  Lattice* copy() const;
  void copy(Lattice* that);
  bool meetUpdate(Lattice* that);
  bool operator==(Lattice* that);
  bool setToFull();
  bool setToEmpty();
  bool isFullLat();
  bool isEmptyLat();
  std::string str(std::string indent="") const;
};

//! Dataflow analysis to generate dot representation of an ATS.
//! Implemented as a forward dataflow analysis.
//! FWDataflow allows this analysis to be composed tightly with other analyses
//! Analysis produces a stringstream emitting dot graph for the ATS
class AtsToDotAnalysis : public FWDataflow {
  ostringstream dotstream;    
public:
  AtsToDotAnalysis() { }
  virtual void initAnalysis(std::set<PartPtr>& startingParts);

  ComposedAnalysisPtr copy() { return boost::make_shared<AtsToDotAnalysis>(); }

  // Initializes the state of analysis lattices at the given ATS node
  void genInitLattice(PartPtr part, PartEdgePtr pedge, 
                      std::vector<Lattice*>& initLattices);

  bool transfer(PartPtr part, CFGNode cn, NodeState& state, 
                std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);
   
  bool implementsExpr2MemRegion() { return false; }
  bool implementsExpr2MemLoc() { return false; }
  bool implementsExpr2Val() { return false; }
  bool implementsATSGraph() { return false; }

  // pretty print for the object
  std::string str(std::string indent="") const;
};

#endif
