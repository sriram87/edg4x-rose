#ifndef _BACKWARDSLICINGANALYSIS_H
#define _BACKWARDSLICINGANALYSIS_H

/*****************************************
 * author: Sriram Aananthakrishnan, 2013 *
 *****************************************/

#include "compose.h"
#include "abstract_object_set.h"

namespace fuse {
  /*******************
   * SliceCriterions *
   *******************/
  //! class to specify the criterion for slicing
  //! criterions are specified using pair<SgStatement*, list<SgVarRefExp> > 
  class SliceCriterions
  {
  public:
    typedef std::pair<SgStatement*, std::set<SgVarRefExp*> > SlicingCriterion;
  private:
    std::vector<SlicingCriterion> sliceCriterions;
  public:
    SliceCriterions() { }
    void addSlicingCriterion(SlicingCriterion sc) {
      sliceCriterions.push_back(sc);
    }
    std::vector<SlicingCriterion> getSlicingCriterions() {
      return sliceCriterions;
    }

  private:
    // internal helper method to build slicing criterion
    SlicingCriterion buildSlicingCriterionFromStmt(SgStatement* stmt);

  public:
    //! interface method to build and add slicing criterion from a SgStatement
    void addSlicingCriterionFromStmt(SgStatement* stmt) {
      SlicingCriterion sc = buildSlicingCriterionFromStmt(stmt);
      addSlicingCriterion(sc);
    }

    //! stringfy the constraints
    std::string str();
  };

  
  /**************************
   * BackwardSlicingLattice *
   **************************/
  //! BackwardSlicingLattice consists of two sets
  //! relevantMLSet - tracks direct/indirect MemLocs 
  //! relevant to the slice
  //! relevantCFGNSet - tracks direct/indirect statements
  //! and other CFGNodes that are relevant to slice   
  class BackwardSlicingLattice : public FiniteLattice
  {
    AbstractObjectSet* relevantMLSet;
    // should be using PartPtr/PartEdgePtr instead ?
    std::set<SgNode*> relevantCFGNSet;
    bool relevantCFGNSetIsFull;

  public:
  BackwardSlicingLattice(PartEdgePtr pedge,
                         AbstractObjectSet _relevantMLSet,
                         std::set<SgNode*> _relevantCFGNSet)
    : Lattice(pedge), 
      FiniteLattice(pedge), 
      relevantCFGNSet(_relevantCFGNSet),
      relevantCFGNSetIsFull(false) { 
      relevantMLSet = new AbstractObjectSet(_relevantMLSet);
    }
    void initialize();
    Lattice* copy() const;
    void copy(Lattice* that);
    bool meetUpdate(Lattice* that);
    bool operator==(Lattice* that);
    bool setToFull();
    bool setToEmpty();
    bool setMLValueToFull(MemLocObjectPtr ml);
    bool isFull();
    bool isEmpty();
    std::string strp(PartEdgePtr pedge, std::string indent);
    std::string str(std::string indent);
  };

  /***************************
   * BackwardSlicingAnalysis *
   ***************************/
  class BackwardSlicingAnalysis : public BWDataflow
  {
    SliceCriterions& sc;
  public:
    BackwardSlicingAnalysis(SliceCriterions& _sc) : sc(_sc) { }
    // intra-procedural analysis initializes the edges of
    // starting parts by calling this function
    // bw: pedge : part->outEdgeToAny
    void genInitLattice(PartPtr part, 
                        PartEdgePtr pedge,
                        std::vector<Lattice*>& initLattices);

    // visitor pattern invoked by
    // ComposedAnalysis::runAnalysis on each part
    boost::shared_ptr<DFTransferVisitor> 
      getTransferVisitor(PartPtr part,
                         CFGNode cfgn,
                         NodeState& state,
                         std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);

    // never executed
    // should be deprecated
    bool transfer(PartPtr part,
                  CFGNode cfgn,
                  NodeState& state,
                  std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo) {
      ROSE_ASSERT(false);
      return 0;
    }

    // Returns a shared pointer to a freshly-allocated copy of this ComposedAnalysis object
    ComposedAnalysisPtr copy() { 
      return boost::make_shared<BackwardSlicingAnalysis>(*this); 
    }

    std::string str(std::string indent="") {
      return "BackwardSlicingAnalysis";
    }
  };

  class BackwardSlicingTransfer : public DFTransferVisitor
  {
    PartPtr part;
    CFGNode cfgn;
    NodeState& state;
    std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo;
  public:
    BackwardSlicingTransfer(PartPtr _part,
                            CFGNode _cfgn,
                            NodeState& _state,
                            std::map<PartEdgePtr, std::vector<Lattice*> >& _dfInfo) 
      : DFTransferVisitor(_part, _cfgn, _state, _dfInfo),
      part(_part),
      cfgn(_cfgn),
      state(_state),
      dfInfo(_dfInfo) { }
    void visit(SgAssignOp* sgn);
    void visit(SgNode* sgn);

    bool finish() {
      return false;
    }
  };

}; // end namespace

#endif
