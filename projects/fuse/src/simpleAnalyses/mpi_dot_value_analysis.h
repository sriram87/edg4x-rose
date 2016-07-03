#ifndef _MPI_DOT_VALUE_ANALYSIS_H
#define _MPI_DOT_VALUE_ANALYSIS_H

/*****************************************
 * Author: Sriram Aananthakrishnan, 2016 *
 *****************************************/

#include "compose.h"
#include "abstract_object_map.h"
#include "mpi.h"

namespace fuse {

  /*******************
   * MPIDotValueKind *
   *******************/
  class MPIDotValueKind;
  typedef boost::shared_ptr<MPIDotValueKind> MPIDotValueKindPtr;
  //! Basic lattice type
  class MPIDotValueKind {
    // enum for avoiding downcasting
    // empty -> bottom, string -> set of strings, unknown -> top
  public:
    typedef enum ktype {empty, stringT, unknown} KType;
  private:
    KType ktype_;
  public:    
    MPIDotValueKind(KType ktype_);
    MPIDotValueKind(const MPIDotValueKind& that);
    KType getType() const;
    virtual bool mayEqualK(MPIDotValueKindPtr thatK)=0;
    virtual bool mustEqualK(MPIDotValueKindPtr thatK)=0;
    virtual bool equalSetK(MPIDotValueKindPtr thatK)=0;
    virtual bool subSetK(MPIDotValueKindPtr thatK)=0;
    virtual bool isEmptyK()=0;
    virtual bool isFullK()=0;
    virtual MPIDotValueKindPtr copyK()=0;
    virtual std::string str(std::string indent="")=0;
  };

  /**************************
   * MPIDotValueDefaultKind *
   **************************/
  class MPIDotValueDefaultKind : public MPIDotValueKind {
  public:
    MPIDotValueDefaultKind();
    MPIDotValueDefaultKind(const MPIDotValueDefaultKind& that);
    bool mayEqualK(MPIDotValueKindPtr thatK);
    bool mustEqualK(MPIDotValueKindPtr thatK);
    bool equalSetK(MPIDotValueKindPtr thatK);
    bool subSetK(MPIDotValueKindPtr thatK);
    bool isEmptyK();
    bool isFullK();
    MPIDotValueKindPtr copyK();
    std::string str(std::string indent="");
  };

  /**************************
   * MPIDotValueUnknownKind *
   **************************/
  class MPIDotValueUnknownKind : public MPIDotValueKind {
  public:
    MPIDotValueUnknownKind();
    MPIDotValueUnknownKind(const MPIDotValueUnknownKind& that);
    bool mayEqualK(MPIDotValueKindPtr thatK);
    bool mustEqualK(MPIDotValueKindPtr thatK);
    bool equalSetK(MPIDotValueKindPtr thatK);
    bool subSetK(MPIDotValueKindPtr thatK);
    bool isEmptyK();
    bool isFullK();
    MPIDotValueKindPtr copyK();
    std::string str(std::string indent="");
  };

  /*************************
   * MPIDotValueStringKind *
   *************************/
  class MPIDotValueStringKind;
  typedef boost::shared_ptr<MPIDotValueStringKind> MPIDotValueStringKindPtr;
  class MPIDotValueStringKind : public MPIDotValueKind {
    std::set<std::string> dotvalues;
  public:
    MPIDotValueStringKind(std::set<std::string> dotvalues);
    MPIDotValueStringKind(std::string dotvalue);
    MPIDotValueStringKind(const MPIDotValueStringKind& that);
    std::set<std::string> get_dot_values() const;
    bool compareK(MPIDotValueKindPtr thatK);
    bool mayEqualK(MPIDotValueKindPtr thatK);
    bool mustEqualK(MPIDotValueKindPtr thatK);
    bool equalSetK(MPIDotValueKindPtr thatK);
    bool subSetK(MPIDotValueKindPtr thatK);
    bool isEmptyK();
    bool isFullK();
    bool mergeStringKind(MPIDotValueStringKindPtr thatK);
    MPIDotValueKindPtr copyK();
    std::string str(std::string indent="");

    bool compareMayK(MPIDotValueStringKindPtr thatK);
    bool compareEqualK(MPIDotValueStringKindPtr thatK);
    bool compareSubK(MPIDotValueStringKindPtr thatK);
  };

  /*********************
   * MPIDotValueObject *
   *********************/
  /*!
   *! MPIDotValueObject represents an interpretation of MPI buffers by MPIDotValueAnalysis
   *! This interpretation assigns a special value to buffers of MPI Comm operations
   *! The interpretation is a set of string values used to generate a dot graph
   *! The value assigned by this object may equals every other value as we dont know the actual set
   *! We only distinguish the set of values based on the callsite
   *! String is used to denote such sets
   *! Two values v1 \join v2 is unknown
   *! We will use unknown to denote values for which this analysis has no idea about.
   *! For instance, this analysis has no idea about ranks or size.
   *! Such values are assigned unknown
   *! When implementing Expr2Val, if the value mapped is unknown the query is forwarded to a prior analysis.
   */
  class MPIDotValueObject;
  typedef boost::shared_ptr<MPIDotValueObject> MPIDotValueObjectPtr;
  class MPIDotValueObject : public FiniteLattice, public ValueObject {
    MPIDotValueKindPtr kind;
  public:
    MPIDotValueObject(PartEdgePtr pedge);
    MPIDotValueObject(std::string dotvalue, PartEdgePtr pedge);
    MPIDotValueObject(ValueObjectPtr vo, PartEdgePtr pedge);
    MPIDotValueObject(const MPIDotValueObject& that);

    MPIDotValueKindPtr getKind() const;
    void initialize();
    Lattice* copy() const;
    bool operator==(Lattice* that);
    bool meetUpdate(Lattice* that);
    bool setToFull();
    bool setToEmpty();
    bool isFullLat();
    bool isEmptyLat();
    bool setMLValueToFull (MemLocObjectPtr ml);

    // Value Object Methods
    bool mayEqualV(ValueObjectPtr o, PartEdgePtr pedge); 
    bool mustEqualV(ValueObjectPtr o, PartEdgePtr pedge);
    bool equalSetV(ValueObjectPtr o, PartEdgePtr pedge); 
    bool subSetV(ValueObjectPtr o, PartEdgePtr pedge);
    bool meetUpdateV(ValueObjectPtr that, PartEdgePtr pedge);
    bool isFullV(PartEdgePtr pedge);
    bool isEmptyV(PartEdgePtr pedge);
    
    bool isConcrete();
    SgType* getConcreteType();
    std::set<boost::shared_ptr< SgValueExp> > getConcreteValue();
    ValueObjectPtr copyV () const;
    std::string str(std::string indent="") const;
  };

  class MPIDotValueAnalysis;
  /**********************
   * MDVTransferVisitor *
   **********************/
  class MDVTransferVisitor : public DFTransferVisitor {
    MPIDotValueAnalysis* analysis;
    bool modified;
    AbstractObjectMap* aMapState;
  public:
    MDVTransferVisitor(PartPtr part,
                       CFGNode cn,
                       NodeState& state,
                       std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo,
                       MPIDotValueAnalysis* analysis);

    bool isMPISendOp(Function& mpif_);
    bool isMPIRecvOp(Function& mpif_);

    void havocUnknownValues(SgFunctionCallExp* sgn);
    bool transferSendOp(Function& mpif_, SgFunctionCallExp* sgn);
    bool transferRecvOp(Function& mpif_, SgFunctionCallExp* sgn);
    bool transferOutBcastOp(Function&, SgFunctionCallExp*);
    bool transferInBcastOp(Function&, SgFunctionCallExp*);
    bool transferOutReduceOp(Function&, SgFunctionCallExp*);
    bool transferInReduceOp(Function&, SgFunctionCallExp*);

    void visit(SgNullExpression* sgn);

    // Unary Op
    void visit(SgAddressOfOp* sgn);
    void visit(SgBitComplementOp* sgn);
    void visit(SgCastExp* sgn);
    void visit(SgMinusMinusOp* sgn);
    void visit(SgMinusOp* sgn);
    void visit(SgNotOp* sgn);
    void visit(SgPlusPlusOp* sgn);
    void visit(SgPointerDerefExp* sgn);
    void visit(SgUnaryAddOp* sgn);
    void visit(SgUnaryOp* sgn);

    // Binary Op
    void visit(SgAddOp* sgn);
    void visit(SgAndOp* sgn);
    void visit(SgArrowExp* sgn);
    void visit(SgArrowStarOp* sgn);
    void visit(SgAssignOp* sgn);
    void visit(SgBitAndOp* sgn);
    void visit(SgBitOrOp* sgn);
    void visit(SgBitXorOp* sgn);
    void visit(SgCommaOpExp* sgn);
    void visit(SgCompoundAssignOp* sgn);
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
    void visit(SgSubtractOp* sgn);    
    void visit(SgBinaryOp* sgn);

    void visit(SgFunctionCallExp* sgn);
    void visit(SgConditionalExp* sgn);
    void visit(SgDeleteExp* sgn);
    void visit(SgExprListExp* sgn);
    void visit(SgFunctionRefExp* sgn);
    void visit(SgInitializer* sgn);
    void visit(SgNewExp* sgn);
    void visit(SgSizeOfOp* sgn);
    void visit(SgValueExp* sgn);
    void visit(SgVarRefExp* sgn);
    void visit(SgExpression* sgn);
    void visit(SgInitializedName* sgn);
    bool finish();
  };

  //! Dataflow analysis to generate dot representation of an ATS.
  //! Implemented as a forward dataflow analysis.
  //! FWDataflow allows this analysis to be composed tightly with other analyses
  //! Analysis produces a stringstream emitting dot graph for the ATS
  class MPIDotValueAnalysis : public FWDataflow {
    int contextid;
    std::map<PartContextPtr, std::string> context2DotStrMap;
  public:
    MPIDotValueAnalysis() : contextid(0) { }
    virtual void initAnalysis(std::set<PartPtr>& startingParts);

    ComposedAnalysisPtr copy() { return boost::make_shared<MPIDotValueAnalysis>(); }

    // Initializes the state of analysis lattices at the given ATS node
    void genInitLattice(PartPtr part, PartEdgePtr pedge, 
                        std::vector<Lattice*>& initLattices);

    bool transfer(PartPtr part, CFGNode cn, NodeState& state, 
                  std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);

    boost::shared_ptr<DFTransferVisitor>
    getTransferVisitor(PartPtr part, 
                       CFGNode cfgn, 
                       NodeState& state, 
                       std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);
   
    bool implementsExpr2MemRegion() { return false; }
    bool implementsExpr2MemLoc() { return false; }
    bool implementsExpr2Val() { return true; }
    bool implementsATSGraph() { return false; }

    ValueObjectPtr Expr2Val(SgNode* sgn, PartEdgePtr pedge);

    // Auxillary functions for dotid creation
    void createContext2dotid(PartContextPtr context);
    std::string context2dotid(PartContextPtr context) const;
    std::string part2dotid(PartPtr part) const;

    // pretty print for the object
    std::string str(std::string indent="") const;
  };

  class MPIDotGraphGenerator {
    std::ostringstream nodess;
    std::ostringstream edgess;
    MPIDotValueAnalysis* analysis;
  public:
    MPIDotGraphGenerator(MPIDotValueAnalysis* analysis);
    std::string cfgn2label(CFGNode cfgn);
    std::string part2label(PartPtr part);
    std::string part2dot(PartPtr part);
    std::string partedge2dot(PartEdgePtr pedge);
    std::string commedge2dot(string sdotvalue, PartPtr target);
    std::string commedges2dot(std::set<std::string> sdotvalues, PartPtr target);
    std::set<std::string> getRecvMPIDotValue(PartPtr part);
    std::set<std::string> getBcastMPIDotValue(PartPtr part);
    std::set<std::string> getReduceMPIDotValue(PartPtr part);

    std::string recvcommedge2dot(PartPtr part);
    std::string bcastcommedge2dot(PartPtr part);
    std::string reducecommedge2dot(PartPtr part);
    bool isRecvOpATSNode(PartPtr part);
    bool isMPIOpATSNode(PartPtr part);
    bool isReduceOpATSNode(PartPtr part);
    bool isMPICommOpATSNode(PartPtr part);
    bool isBcastOpATSNode(PartPtr part);
    void generateDot();
    void generateDotFile();
  };
}; // end namespace fuse
#endif
