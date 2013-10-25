#ifndef _BACKWARDSLICINGANALYSIS_H
#define _BACKWARDSLICINGANALYSIS_H

/*****************************************
 * author: Sriram Aananthakrishnan, 2013 *
 *****************************************/

#include "compose.h"
#include "abstract_object_set.h"

namespace fuse {

  extern int backwardSlicingDebugLevel;

  /*******************
   * SliceCriterions *
   *******************/
  //! class to specify the criterion for slicing
  //! criterions are specified using pair<SgStatement*, list<SgVarRefExp> > 
  class SliceCriterionsList
  {
  public:
    typedef std::pair<SgStatement*, std::set<SgVarRefExp*> > SliceCriterion;;
  private:
    std::vector<SliceCriterion> sliceCriterions;
  public:
    SliceCriterionsList() { }
    void addSliceCriterion(SliceCriterion sc) {
      sliceCriterions.push_back(sc);
    }
    std::vector<SliceCriterion> getSliceCriterions() {
      return sliceCriterions;
    }

  private:
    // internal helper method to build slicing criterion
    SliceCriterion buildSliceCriterionFromStmt(SgStatement* stmt);

  public:
    //! interface method to build and add slicing criterion from a SgStatement
    void addSliceCriterionFromStmt(SgStatement* stmt) {
      SliceCriterion sc = buildSliceCriterionFromStmt(stmt);
      addSliceCriterion(sc);
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

    // insert ML into relevantMLSet
    // helper method for transfer functions
    bool insertRelevantML(MemLocObjectPtr mlp) {
      return relevantMLSet->insert(boost::dynamic_pointer_cast<AbstractObject>(mlp));
    }

    // insert CFGN into relevantCFGNSet
    // helper method for transfer functions
    bool insertRelevantCFGN(CFGNode cfgn) {
      typedef std::set<SgNode*>::iterator CFGNIterator;
      std::pair<CFGNIterator, bool> rv = relevantCFGNSet.insert(cfgn.getNode());
      return rv.second;
    }
   
    AbstractObjectSet& getRelevantMLSet() {
      return *relevantMLSet;
    }

    std::set<SgNode*>& getRelevantCFGNSet() {
      return relevantCFGNSet;
    }

    // lattice operators
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
    std::string relevantCFGNSetToStr(std::set<SgNode*>& relevantCFGNSet);
    std::string strp(PartEdgePtr pedge, std::string indent);
    std::string str(std::string indent);
  };

  /***************************
   * BackwardSlicingAnalysis *
   ***************************/
  class BackwardSlicingAnalysis : public BWDataflow
  {
    SliceCriterionsList& sc;
  public:
    BackwardSlicingAnalysis(SliceCriterionsList& _sc) : sc(_sc) { }

    // intra-procedural analysis initializes the edges of
    // starting parts by calling this function
    // bw: pedge : part->outEdgeToAny
    void genInitLattice(PartPtr part, 
                        PartEdgePtr pedge,
                        std::vector<Lattice*>& initLattices);

    // helper function for genInitLattice
    void createSliceCriterionForPart(SliceCriterionsList::SliceCriterion sc,
                                     PartPtr part,
                                     AbstractObjectSet& relevantMLSet,
                                     std::set<SgNode*>& relevantCFGNSet);

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

  // dataflow transfer visitor class for statements
  // transfer functions are applied on statements first in
  // the backward direction that subsequently calls transfer
  // functions for expressions of BackwardSlicingExprTransfer
  class BackwardSlicingTransfer : public DFTransferVisitor
  {
    PartPtr part;
    CFGNode cfgn;
    NodeState& state;
    std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo;
    Composer* composer;
    BackwardSlicingAnalysis* bsa;      
    bool modified;
    BackwardSlicingLattice* bsl;
  public:
    BackwardSlicingTransfer(PartPtr _part,
                            CFGNode _cfgn,
                            NodeState& _state,
                            std::map<PartEdgePtr, std::vector<Lattice*> >& _dfInfo,
                            Composer* _composer,
                            BackwardSlicingAnalysis* _bsa) 
      : DFTransferVisitor(_part, _cfgn, _state, _dfInfo),
      part(_part),
      cfgn(_cfgn),
      state(_state),
      dfInfo(_dfInfo),
      composer(_composer),
      bsa(_bsa),
      modified(false) {
      // set up the lattice from dfinfo
      setBSLFromDfInfo();
    }

    /********************************************************
     * Access methods for BackwardSlicingExpressionTransfer *
     ********************************************************/
    void setBSLFromDfInfo();

    BackwardSlicingLattice* getBSL() {
      ROSE_ASSERT(bsl);
      return bsl;
    }

    PartPtr getPartPtr() {
      return part;
    }

    Composer* getComposer() {
      return composer;
    }

    BackwardSlicingAnalysis* getBSA() {
      return bsa;
    }

    CFGNode getCFGN() {
      return cfgn;
    }

    /**********************
     * Transfer Functions *
     **********************/

    void visit(SgExpression* sgn); 
    
    // should not reach here
    void visit(SgNode* sgn) {
      // NOTE: assert(false) here
    }

    /*****************************
     * DFTransferVisitor Methods *
     *****************************/
    bool finish() {
      return modified;
    }
  };

  // transfer functions for expressions
  class BackwardSlicingExprTransfer : public ROSE_VisitorPatternDefaultBase
  {
    // handle to the transfer functions class
    BackwardSlicingTransfer& bst;
    // flag to indicate if the transfer functions modified state
    bool modified;

    // keep track ml defined by this expression
    AbstractObjectSet defML;
    AbstractObjectSet refML;
  public:
  BackwardSlicingExprTransfer(BackwardSlicingTransfer& _bst)
    : bst(_bst), 
      modified(false),
      defML(bst.getPartPtr()->inEdgeFromAny(), 
            bst.getComposer(), 
            bst.getBSA(), AbstractObjectSet::may),
      refML(bst.getPartPtr()->inEdgeFromAny(), 
            bst.getComposer(), 
            bst.getBSA(), AbstractObjectSet::may) { }
    
    void visit(SgNode* sgn) {
      // this class explicity should handle only expressions
      std::cerr << "Unhandled Expression " << sgn->class_name() << std::endl;
      ROSE_ASSERT(false);
    }

    template <typename T> void binaryExprTransfer(SgBinaryOp* sgn, T transferFunctor);
        
    void visit(SgAssignOp* sgn);
    void visit(SgAddOp* sgn);
    
    void visit(SgVarRefExp* sgn) {
    }

    void visit(SgValueExp* sgn) {
    }

    // implementation of transfer functions
    void transferAssignment(MemLocObjectPtr mlExp, MemLocObjectPtr mlLHS, MemLocObjectPtr mlRHS, 
                            AbstractObjectSet& relevantMLSetIN);
    void transferBinaryOpNoMod(MemLocObjectPtr mlExp, MemLocObjectPtr mlLHS, MemLocObjectPtr mlRHS, 
                               AbstractObjectSet& relevantMLSetIN);

    void updateRelevantMLSet();
    void updateRelevantCFGNSet();
    bool isStateModified() {
      return modified;
    }
  };

}; // end namespace

#endif
