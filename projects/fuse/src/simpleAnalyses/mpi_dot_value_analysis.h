#ifndef _MPI_DOT_VALUE_ANALYSIS_H
#define _MPI_DOT_VALUE_ANALYSIS_H

/*****************************************
 * Author: Sriram Aananthakrishnan, 2016 *
 *****************************************/

#include "compose.h"
#include "abstract_object_map.h"
#include "mpi.h"

namespace fuse {  
  class MPIDotValueObject;
  typedef boost::shared_ptr<MPIDotValueObject> MPIDotValueObjectPtr;
  /*!
   *! MPIDotValueObject represents an interpretation of MPI buffers by MPIDotValueAnalysis
   *! This interpretation assigns a special value to buffers of MPI Comm operations
   *! The interpretation is a string value used to generate a dot graph
   *! The value assigned by this object may equals every other value as we dont know the actual set
   *! We only distinguish the set of values based on the callsite
   *! String is used to denote such sets
   *! Empty string is empty value and the string "unknown" denotes Top
   *! Two values v1 \join v2 is unknown
   *! We will use unknown to denote values for which this analysis has no idea about.
   *! For instance, this analysis has no idea about ranks or size.
   *! Such values are assigned unknown
   *! When implementing Expr2Val, if the value mapped is unknown the query is forwarded to a prior analysis.
   */
  class MPIDotValueObject : public FiniteLattice, public ValueObject {
    std::string dotvalue;
  public:
    MPIDotValueObject(PartEdgePtr pedge);
    MPIDotValueObject(std::string dotvalue, PartEdgePtr pedge);
    MPIDotValueObject(ValueObjectPtr v, PartEdgePtr pedge);
    MPIDotValueObject(const MPIDotValueObject& that);

    std::string get_dot_value() const;

    virtual void initialize();
    virtual Lattice* copy() const;
    virtual bool meetUpdate(Lattice* that);
    virtual bool operator==(Lattice* that);
    virtual bool setToFull();
    virtual bool setToEmpty();
    virtual bool isFullLat();
    virtual bool isEmptyLat();
    virtual bool setMLValueToFull (MemLocObjectPtr ml);

    // Value Object Methods
    virtual bool mayEqualV (ValueObjectPtr o, PartEdgePtr pedge); 
    virtual bool mustEqualV (ValueObjectPtr o, PartEdgePtr pedge);
    virtual bool equalSetV (ValueObjectPtr o, PartEdgePtr pedge); 
    virtual bool subSetV (ValueObjectPtr o, PartEdgePtr pedge);
    virtual bool meetUpdateV (ValueObjectPtr that, PartEdgePtr pedge);
    virtual bool isFullV(PartEdgePtr pedge);
    virtual bool isEmptyV(PartEdgePtr pedge);
    
    virtual bool isConcrete();
    virtual SgType* getConcreteType();
    virtual std::set<boost::shared_ptr< SgValueExp> > getConcreteValue();
    virtual ValueObjectPtr copyV () const;
    std::string str(std::string indent="") const;
  };

  /*********************
   * MPISendOpCallSite *
   *********************/
  class MPISendOpCallSite {
    const Function mpif_;
    SgFunctionCallExp* sgn;
  public:
    MPISendOpCallSite(const Function& mpif_, SgFunctionCallExp* sgn);
    //! Return syntactic expression corresponding to send buffer
    SgNode* getSendBuffer();
    bool isSendOp();
  };

  /*********************
   * MPIRecvOpCallSite *
   *********************/
  class MPIRecvOpCallSite {
    const Function mpif_;
    SgFunctionCallExp* sgn;
  public:
    MPIRecvOpCallSite(const Function& mpif_, SgFunctionCallExp* sgn);
    SgNode* getRecvBuffer();
    bool isRecvOp();
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

    string getRecvMPIDotValue(PartPtr part);
    bool isRecvOpATSNode(PartPtr part);
    bool isMPIOpATSNode(PartPtr part);
    void generateDot();
    void generateDotFile();
  };
}; // end namespace fuse
#endif
