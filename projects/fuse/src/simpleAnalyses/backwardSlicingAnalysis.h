#ifndef _BACKWARDSLICINGANALYSIS_H
#define _BACKWARDSLICINGANALYSIS_H

/*****************************************
 * author: Sriram Aananthakrishnan, 2013 *
 *****************************************/

#include "compose.h"
#include "abstract_object_set.h"
#include <utility>

namespace fuse {

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
  //! Slicing algorithm determines relevant variables and relevant statements
  //! relevant variabes are tracked using two sets relevantMLRef, relevantMLVal of the BSL
  //! relevantMLRef denotes the memory reference of the ML
  //! relevantMLVal denotes the symoblic value associated with the ML
  //! same ML is used to denote reference and symbolic value
  //! assignment semantics kills symbolic value and reference is maintained
  //! relevantAS : set of abstract states (paritions) that are in the slice
  //! relevantAS membership is determined by 
  //! 1. semantics of the expression (relevant to slice)
  //! 2. contents of relevantMLRef and relevantMLVal
  class BackwardSlicingLattice : public FiniteLattice
  {
    AbstractObjectSet* relevantMLRef;
    AbstractObjectSet* relevantMLVal;
    //! flag is used to denote if the set is full or not
    std::pair<bool, std::set<PartPtr> > relevantAS;    

  public:
  BackwardSlicingLattice(PartEdgePtr pedge,
                         AbstractObjectSet& _relevantMLRef,
                         AbstractObjectSet& _relevantMLVal,
                         std::pair<bool, std::set<PartPtr> >& _relevantAS)
    : Lattice(pedge), 
      FiniteLattice(pedge) {
        relevantMLRef = new AbstractObjectSet(_relevantMLRef);
        relevantMLVal = new AbstractObjectSet(_relevantMLVal);
        relevantAS = _relevantAS;
    }

    bool insertRelMLRef(MemLocObjectPtr ml);
    bool insertRelMLVal(MemLocObjectPtr ml);
    bool insertRelAS(std::set<PartPtr>& partPtrSet);
    bool insertRelAS(PartPtr part);

    bool removeRelMLVal(MemLocObjectPtr ml);

    bool containsML(AbstractObjectSet* relevantML, MemLocObjectPtr ml);
    bool containsRelMLVal(MemLocObjectPtr ml);
    bool containsRelMLRef(MemLocObjectPtr ml);
    bool containsAS(PartPtr part);

    // lattice operators
    void initialize();
    Lattice* copy() const;
    void copy(Lattice* that);
    bool meetUpdate(Lattice* that);
    bool operator==(Lattice* that);
    bool setToFull();
    bool setToEmpty();
    bool setMLValueToFull(MemLocObjectPtr ml);
    bool isFullLat() {
      return isFull();
    }
    bool isEmptyLat() {
      return isEmpty();
    }
    bool isFull();
    bool isEmpty();
    std::string str(std::set<PartPtr>& partPtrSet);
    std::string strp(PartEdgePtr pedge, std::string indent="");
    std::string str(std::string indent="");
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
    void initBSLForPart(SliceCriterionsList::SliceCriterion sc,
                        PartPtr part,
                        BackwardSlicingLattice* bsl);

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
    void visit(SgVariableDeclaration* sgn);
    void visit(SgInitializedName* sgn);
    
    // should not reach here
    void visit(SgNode* sgn) {
      // NOTE: assert(false) here
    }

    /*****************************
     * DFTransferVisitor Methods *
     *****************************/
    bool finish();
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
    std::set<PartPtr> relAS;

    // data needed for transfer functions
    Composer* composer;
    PartPtr part;
    BackwardSlicingAnalysis* bsa;
    BackwardSlicingLattice* bsl;
  public:
  BackwardSlicingExprTransfer(BackwardSlicingTransfer& _bst)
    : bst(_bst), 
      modified(false),
      defML(bst.getPartPtr()->inEdgeFromAny(), 
            bst.getComposer(), 
            bst.getBSA(), AbstractObjectSet::may),
      refML(bst.getPartPtr()->inEdgeFromAny(), 
            bst.getComposer(), 
            bst.getBSA(), AbstractObjectSet::may),
      composer(bst.getComposer()),
      part(bst.getPartPtr()),
      bsa(bst.getBSA()),
      bsl(bst.getBSL()) { }
    
    void visit(SgNode* sgn) {
      // this class explicity should handle only expressions
      dbg << "Unhandled Expression " << sgn->class_name() << std::endl;
      ROSE_ASSERT(false);
    }

    template <typename T> void binaryExprTransfer(SgBinaryOp* sgn, T transferFunctor);
        
    // expression with modifying semantics
    void visit(SgAssignOp* sgn);    
    void visit(SgCompoundAssignOp* sgn);

    // binary op with no-modification semantics
    // NOTE: these expressions may have side-effects
    // side-effects are handled when handling sub-expressions
    void visit(SgAddOp* sgn);
    void visit(SgAndOp* sgn);
    void visit(SgArrowExp* sgn);
    void visit(SgArrowStarOp* sgn);
    void visit(SgBitAndOp* sgn);
    void visit(SgBitOrOp* sgn);
    void visit(SgBitXorOp* sgn);
    void visit(SgCommaOpExp* sgn);
    void visit(SgDivideOp* sgn);
    void visit(SgDotExp* sgn);
    void visit(SgDotStarOp* sgn);
    void visit(SgEqualityOp* sgn);
    void visit(SgGreaterOrEqualOp* sgn);
    void visit(SgGreaterThanOp* sgn);
    void visit(SgLessOrEqualOp* sgn);
    void visit(SgLessThanOp* sgn);
    void visit(SgLshiftOp* sgn);
    void visit(SgModOp* sgn);
    void visit(SgMultiplyOp* sgn);
    void visit(SgNotEqualOp* sgn);
    void visit(SgOrOp* sgn);
    void visit(SgPntrArrRefExp* sgn);
    void visit(SgRshiftOp* sgn);
    void visit(SgSubtractOp* sgn);
    
    void visit(SgVarRefExp* sgn);
    void visit(SgValueExp* sgn);

    void visit(SgInitializedName* sgn);
    void visit(SgAssignInitializer* sgn);

    // implementation of transfer functions
    void transferAssignment(SgNode* bexp, SgNode* lexp, SgNode* rexp);
    void transferBinaryOpNoMod(SgNode* bexp, SgNode* lexp, SgNode* rexp);

    // helper methods
    std::list<PartPtr> getOperandPartPtr(SgNode* anchor, SgNode* operand);
    void updateBSLState();
    bool isStateModified() {
      return modified;
    }
  };

}; // end namespace

#endif