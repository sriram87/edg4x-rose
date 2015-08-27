#ifndef _MPI_COMM_ANALYSIS_H
#define _MPI_COMM_ANALYSIS_H

/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

#include "compose.h"

namespace fuse {

  /********************
   * MPICommValueKind *
   ********************/
  class MPICommValueKind;
  typedef boost::shared_ptr<MPICommValueKind> MPICommValueKindPtr;
  class MPICommValueKind {
  public:
    enum ValueKind{concrete, unknown};
  protected:
    ValueKind kind;
  public:
    MPICommValueKind(ValueKind kind);
    ValueKind getKindType() const;
    bool mayEqualK(MPICommValueKindPtr that);
    bool mustEqualK(MPICommValueKindPtr that);
    bool meetUpdateK(MPICommValueKindPtr that);
    bool equalSetK(MPICommValueKindPtr that);
    bool subSetK(MPICommValueKindPtr that);
    bool isEmptyK();
    bool isFullK();

    virtual MPICommValueKindPtr copyK()=0;
    virtual bool mayEqualVK(MPICommValueVKindPtr thatVK)=0;
    virtual bool mustEqualVK(MPICommValueVKindPtr thatVK)=0;
    virtual bool meetUpdateVK(MPICommValueVKindPtr thatVK)=0;
    virtual bool equalSetVK(MPICommValueVKindPtr thatVK)=0;
    virtual bool subSetVK(MPICommValueVKindPtr thatVK)=0;
  };

  /****************************
   * MPICommValueConcreteKind *
   ****************************/
  class MPICommValueConcreteKind : public MPICommValueKind {
    SgType* valueType;
    typedef boost::shared_ptr<SgValueExp> ConcreteValue;
    std::set<ConcreteValue> concreteValues;   
  public:
    MPICommValueConcreteKind(SgType* valueType, std::set<ConcreteValue> concreteValues);
    MPICommValueConcreteKind(const MPICommValueConcreteKind& that);
    MPICommValueKindPtr copyK();
    bool mayEqualVK(MPICommValueVKindPtr thatVK);
    bool mustEqualVK(MPICommValueVKindPtr thatVK);
    bool meetUpdateVK(MPICommValueVKindPtr thatVK);
    bool equalSetVK(MPICommValueVKindPtr thatVK);
    bool subSetVK(MPICommValueVKindPtr thatVK);
  };

  typedef boost::shared_ptr<MPICommValueConcreteKind> MPICommValueConcreteKindPtr;

  /***************************
   * MPICommValueUnknownKind *
   ***************************/
  class MPICommValueUnknownKind : public MPICommValueKind {
  public:
    MPICommValueUnknownKind();
    MPICommValueUnknownKind(const MPICommValueUnknownKind& that);
    MPICommValueKindPtr copyK();
    bool mayEqualVK(MPICommValueVKindPtr thatVK);
    bool mustEqualVK(MPICommValueVKindPtr thatVK);
    bool meetUpdateVK(MPICommValueVKindPtr thatVK);
    bool equalSetVK(MPICommValueVKindPtr thatVK);
    bool subSetVK(MPICommValueVKindPtr thatVK);
  };

  typedef boost::shared_ptr<MPICommValueUnknownKind> MPICommValueUnknownKindPtr;

  /**********************
   * MPICommValueObject *
   **********************/
  //! Lattice value associated with MPI buffer
  //! Dataflow states are set of all constants
  //! Set is lifted to form a flat lattice with \bot and \top element
  class MPICommValueObject : public FiniteLattice, public ValueObject {
    MPICommValueKindPtr kind;
  public:
    MPICommValueObject(PartEdgePtr pedge, const ValueObjectPtr that);
    MPICommValueObject(const MPICommValueObject& that);

    MPICommValueKindPtr getKind() const;
    
    // Lattice interface
    void initialize();
    Lattice* copy() const;
    bool meetUpdate(Lattice* thatL);
    bool operator==(Lattice* thatL);
    bool setToFull();
    bool setToEmpty();
    bool setMLValueToFull(MemLocObjectPtr ml);
    bool isFullLat();
    bool isEmptyLat();

    // ValueObject interface
    bool mayEqualV(ValueObjectPtr vo, PartEdgePtr pedge);
    bool mustEqualV(ValueObjectPtr vo, PartEdgePtr pedge);
    bool equalSetV(ValueObjectPtr vo, PartEdgePtr pedge);
    bool subSetV(ValueObjectPtr vo, PartEdgePtr pedge);
    bool meetUpdateV(ValueObjectPtr vo, PartEdgePtr pedge);
    bool isEmptyV();
    bool isFullV();
    bool isConcrete();
    SgType* getConcreteType();
    std::set<boost::shared_ptr<SgValueExp> > getConcreteValue();
    ValueObjectPtr copyV();
    
    // printable
    std::string str(std::string indent="") const;
  };

  // typedef MPICommValueObject<ComposedAnalysis*, false> UnionAnalMPICommValueObject;
  // typedef boost::shared_ptr<MPICommValueObject<ComposedAnalysis*, false> > UnionAnalMPICommValueObjectPtr;

  /********************
   * MPICommOpCallExp *
   ********************/
  struct MPICommOp {
    enum OpType {SEND, 
                 RECV,
                 INIT,
                 FINALIZE,
                 NOOP};
  };

  //! Class to process arguments of MPICommOp call expression
  class MPICommOpCallExp {
    Function mpifunc;
    SgExprListExp* argList;
    MPICommOp::OpType optype;
        
  public:
    MPICommOpCallExp(const Function& func, SgExprListExp* arglist);
    MPICommOpCallExp(const MPICommOpCallExp& that);
    SgExpression* getCommOpBufferExpr();
    SgExpression* getCommOpDestExpr();
    SgExpression* getCommOpTagExpr();
    bool isMPICommOp();
  };

  /**************************
   * MPICommOpCallParamList *
   **************************/
  //! Class to process arguments of MPICommOp SgFunctionParameterList
  class MPICommOpCallParamList {
    Function mpifunc;
    const SgInitializedNamePtrList& argList;
    MPICommOp::OpType optype;
  public:
    MPICommOpCallParamList(const Function& func, const SgInitializedNamePtrList& argList);
    MPICommOpCallParamList(const MPICommOpCallParamList& that);
    SgInitializedName* getCommOpBuffer();
    SgPointerDerefExp* getCommOpBufferDerefExpr();
    bool isMPICommOp();
  };

  /**************************
   * MPICommAnalysisTranfer *
   **************************/

  class MPICommAnalysis;

  class MPICommAnalysisTransfer : public DFTransferVisitor {
    // Protected member variable names are part, cn, nodeState, dfInfo
    MPICommAnalysis* analysis;
  public:
    MPICommAnalysisTransfer(PartPtr part,
                            CFGNode cfgn,
                            NodeState& state,
                            std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo,
                            MPICommAnalysis* analysis);
    void visit(SgFunctionParameterList* sgn);
    void visit(SgFunctionCallExp* sgn);
    void visit(SgPointerDerefExp* sgn);
    void visit(SgNode* sgn);
    bool finish();

    //! Get the function from the parameter list
    Function getFunction(SgFunctionParameterList* sgn);
    Function getFunction(SgFunctionCallExp* sgn);
    Function getFunction(SgNode* sgn);
    //! Check if this function is a MPI call 
    bool isMPIFuncCall(const Function& func) const;
    bool isMPICommOpFuncCall(const Function& func) const;
  };

  /*******************
   * MPICommAnalysis *
   *******************/

  class MPICommAnalysis : public FWDataflow {
  public:
    MPICommAnalysis();

    ComposedAnalysisPtr copy() { return boost::make_shared<MPICommAnalysis>(); }

    // Initializes the state of analysis lattices at the given function, part and edge into our out of the part
    // by setting initLattices to refer to freshly-allocated Lattice objects.
    void genInitLattice(PartPtr part, PartEdgePtr pedge, 
                        std::vector<Lattice*>& initLattices);

    bool transfer(PartPtr part, CFGNode cn, NodeState& state, 
                  std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo) { assert(0); }

    boost::shared_ptr<DFTransferVisitor> getTransferVisitor(PartPtr part, CFGNode cn, NodeState& state, 
                                                            std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);
   
    bool implementsExpr2MemRegion() { return false; }
    bool implementsExpr2MemLoc() { return false; }
    bool implementsExpr2Val() { return true; }

    ValueObjectPtr Expr2Val(SgNode* sgn, PartEdgePtr pedge);

    bool implementsATSGraph() { return false; }

    // pretty print for the object
    std::string str(std::string indent="") const;
  };

}; // end namespace fuse

#endif
