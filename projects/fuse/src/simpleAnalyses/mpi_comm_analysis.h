#ifndef _MPI_COMM_ANALYSIS_H
#define _MPI_COMM_ANALYSIS_H

/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

#include "compose.h"
#include "abstract_object_map.h"

namespace fuse {

  /********************
   * MPICommValueKind *
   ********************/
  class MPICommValueKind;
  typedef boost::shared_ptr<MPICommValueKind> MPICommValueKindPtr;

  class MPICommValueKind : public sight::printable {
  public:
    enum ValueKind{bottom, concrete, unknown};
  protected:
    ValueKind kind;
  public:
    MPICommValueKind(ValueKind kind);
    ValueKind getKindType() const;

    virtual bool mayEqualK(MPICommValueKindPtr that)=0;
    virtual bool mustEqualK(MPICommValueKindPtr that)=0;
    virtual bool equalSetK(MPICommValueKindPtr that)=0;
    virtual bool subSetK(MPICommValueKindPtr that)=0;
    bool isEmptyK();
    bool isFullK();
    virtual MPICommValueKindPtr copyK()=0;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;

    virtual std::string str(std::string indent="") const=0;
  };

  /***************************
   * MPICommValueDefaultKind *
   ***************************/
  class MPICommValueDefaultKind : public MPICommValueKind {
  public:
    MPICommValueDefaultKind();
    MPICommValueDefaultKind(const MPICommValueDefaultKind& that);
    MPICommValueKindPtr copyK();
    bool mayEqualK(MPICommValueKindPtr thatK);
    bool mustEqualK(MPICommValueKindPtr thatK);    
    bool equalSetK(MPICommValueKindPtr thatK);
    bool subSetK(MPICommValueKindPtr thatK);
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;
    std::string str(std::string indent="") const;
  };


  /*****************
   * ConcreteValue *
   *****************/
  typedef boost::shared_ptr<SgValueExp> SgValueExpPtr;
  typedef std::set<SgValueExpPtr> SgValueExpPtrSet;
  
  class ConcreteValue;
  typedef CompSharedPtr<ConcreteValue> ConcreteValuePtr;

  class ConcreteValue {
  public:
    ConcreteValue();
    ConcreteValue(const ConcreteValue& that);
    virtual bool operator<(const ConcreteValuePtr& that) const=0;
    virtual bool operator==(const ConcreteValuePtr& that) const=0;
    virtual bool operator!=(const ConcreteValuePtr& that) const=0;

    virtual SgValueExpPtr getConcreteValue() const=0;
    virtual SgType* getConcreteType() const=0;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;

    virtual std::string str(std::string indent="") const=0;
  };

  /************************
   * IntegerConcreteValue *
   ************************/
  class IntegerConcreteValue : public ConcreteValue {
    boost::shared_ptr<SgIntVal> value;
  public:
    IntegerConcreteValue(int value);
    IntegerConcreteValue(const IntegerConcreteValue& that);
    ConcreteValue* copy() const;

    bool operator<(const ConcreteValuePtr& that) const;
    bool operator==(const ConcreteValuePtr& that) const;
    bool operator!=(const ConcreteValuePtr& that) const;

    SgValueExpPtr getConcreteValue() const;
    SgType* getConcreteType() const;
    int get_value() const;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;
    std::string str(std::string indent="") const;
  };
  typedef CompSharedPtr<IntegerConcreteValue> IntegerConcreteValuePtr;

  /****************************
   * LongIntConcreteValue *
   ****************************/
  class LongIntConcreteValue : public ConcreteValue {
    boost::shared_ptr<SgLongIntVal> value;
  public:
    LongIntConcreteValue(long value);
    LongIntConcreteValue(const LongIntConcreteValue& that);
    ConcreteValue* copy() const;

    bool operator<(const ConcreteValuePtr& that) const;
    bool operator==(const ConcreteValuePtr& that) const;
    bool operator!=(const ConcreteValuePtr& that) const;

    SgValueExpPtr getConcreteValue() const;
    SgType* getConcreteType() const;
    long get_value() const;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;
    std::string str(std::string indent="") const;
  };
  typedef CompSharedPtr<LongIntConcreteValue> LongIntConcreteValuePtr;

  /****************************
   * LongLongIntConcreteValue *
   ****************************/
  class LongLongIntConcreteValue : public ConcreteValue {
    boost::shared_ptr<SgLongLongIntVal> value;
  public:
    LongLongIntConcreteValue(long value);
    LongLongIntConcreteValue(const LongLongIntConcreteValue& that);
    ConcreteValue* copy() const;

    bool operator<(const ConcreteValuePtr& that) const;
    bool operator==(const ConcreteValuePtr& that) const;
    bool operator!=(const ConcreteValuePtr& that) const;

    SgValueExpPtr getConcreteValue() const;
    SgType* getConcreteType() const;
    long long get_value() const;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;
    std::string str(std::string indent="") const;
  };
  typedef CompSharedPtr<LongLongIntConcreteValue> LongLongIntConcreteValuePtr;

  /***********************
   * StringConcreteValue *
   ***********************/
  class StringConcreteValue : public ConcreteValue {
    boost::shared_ptr<SgStringVal> value;
  public:
    StringConcreteValue(std::string string);
    StringConcreteValue(const StringConcreteValue& that);
    ConcreteValue* copy() const;

    bool operator<(const ConcreteValuePtr& that) const;
    bool operator==(const ConcreteValuePtr& that) const;
    bool operator!=(const ConcreteValuePtr& that) const;

    SgValueExpPtr getConcreteValue() const;
    SgType* getConcreteType() const;
    std::string get_value() const;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;
    std::string str(std::string indent="") const;
  };
  typedef CompSharedPtr<StringConcreteValue> StringConcreteValuePtr;

  /****************************
   * MPICommValueConcreteKind *
   ****************************/
  class MPICommValueConcreteKind;
  typedef boost::shared_ptr<MPICommValueConcreteKind> MPICommValueConcreteKindPtr;

  //! Represents the concrete set of values
  //! Stores stl set of ConcreteValue objects
  class MPICommValueConcreteKind : public MPICommValueKind {
    std::set<ConcreteValuePtr> concreteValues;   
  public:
    MPICommValueConcreteKind(const SgValueExpPtrSet& concreteValues);
    MPICommValueConcreteKind(const std::set<ConcreteValuePtr>& concreteValues);
    MPICommValueConcreteKind(const MPICommValueConcreteKind& that);
    MPICommValueKindPtr copyK();

    SgType* getConcreteType() const;
    SgValueExpPtrSet getConcreteValue() const;
    std::set<ConcreteValuePtr> getConcreteValuePtrSet() const;

    bool mayEqualK(MPICommValueKindPtr thatK);
    bool mustEqualK(MPICommValueKindPtr thatK);
    bool equalSetK(MPICommValueKindPtr thatK);
    bool subSetK(MPICommValueKindPtr thatK);

    bool set_subset(const std::set<ConcreteValuePtr>& setone, const std::set<ConcreteValuePtr>& settwo) const;
    bool set_intersect(const std::set<ConcreteValuePtr>& setone, const std::set<ConcreteValuePtr>& settwo) const;
    bool set_equal(const std::set<ConcreteValuePtr>& setone, const std::set<ConcreteValuePtr>& settwo) const;
    bool set_union(std::set<ConcreteValuePtr>& setone, const std::set<ConcreteValuePtr>& settwo);

    bool unionConcreteValues(MPICommValueConcreteKindPtr thatCK);
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;

    std::string str(std::string indent="") const;
    ~MPICommValueConcreteKind();
  };

  /***************************
   * MPICommValueUnknownKind *
   ***************************/
  class MPICommValueUnknownKind : public MPICommValueKind {
  public:
    MPICommValueUnknownKind();
    MPICommValueUnknownKind(const MPICommValueUnknownKind& that);
    MPICommValueKindPtr copyK();
    bool mayEqualK(MPICommValueKindPtr thatK);
    bool mustEqualK(MPICommValueKindPtr thatK);    
    bool equalSetK(MPICommValueKindPtr thatK);
    bool subSetK(MPICommValueKindPtr thatK);
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;
    std::string str(std::string indent="") const;
  };
  typedef boost::shared_ptr<MPICommValueUnknownKind> MPICommValueUnknownKindPtr;

  /******************
   * Helper Methods *
   ******************/
  MPICommValueKindPtr createMPICommValueKind(ValueObjectPtr vo, PartEdgePtr pedge);

  /**********************
   * MPICommValueObject *
   **********************/
  //! Lattice value associated with MPI buffer
  //! Dataflow states are set of all constants
  //! Set is lifted to form a flat lattice with \bot and \top element

  class MPICommValueObject : public FiniteLattice, public ValueObject {
    MPICommValueKindPtr kind;
  public:
    MPICommValueObject(PartEdgePtr pedge);
    MPICommValueObject(MPICommValueKindPtr kind, PartEdgePtr pedge);
    MPICommValueObject(ValueObjectPtr that, PartEdgePtr pedge);
    MPICommValueObject(const MPICommValueObject& that);

    MPICommValueKindPtr getKind() const;
    void setKind(MPICommValueKindPtr thatK);
    
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
    bool isEmptyV(PartEdgePtr pedge);
    bool isFullV(PartEdgePtr pedge);
    bool isConcrete();
    SgType* getConcreteType();
    std::set<boost::shared_ptr<SgValueExp> > getConcreteValue();
    ValueObjectPtr copyV() const;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
    friend class boost::serialization::access;

    // printable
    std::string str(std::string indent="") const;
  };

  typedef boost::shared_ptr<MPICommValueObject> MPICommValueObjectPtr;

  /**************************
   * MPICommAnalysisTranfer *
   **************************/

  class MPICommAnalysis;

  class MPICommAnalysisTransfer : public DFTransferVisitor {
    // Protected member variable names are part, cn, nodeState, dfInfo
    MPICommAnalysis* analysis;
    AbstractObjectMap* latticeMap;
    bool modified;
  public:
    MPICommAnalysisTransfer(PartPtr part,
                            CFGNode cfgn,
                            NodeState& state,
                            std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo,
                            MPICommAnalysis* analysis);
    //! Get the function from the parameter list
    Function getFunction(SgFunctionParameterList* sgn);
    Function getFunction(SgFunctionCallExp* sgn);
    Function getFunction(SgNode* sgn);

    //! Check if this function is a MPI call 
    bool isMPISendOp(const Function& func) const;
    bool isMPIRecvOp(const Function& func) const;

    std::string serialize(MPICommValueObjectPtr mvo);
    MPICommValueObjectPtr deserialize(std::string data);

    void visit(SgPointerDerefExp* sgn);
    /*!
     * The sender sends the dataflow state by querying for ValueObject,
     * serializing the ValueObject and sending it through MPI runtime.
     */
    void transferMPISendOp(SgPointerDerefExp* sgn, Function mpif_);
    /*!
     * Each analysis is also constantly probing the incoming channel.
     * When a message is present on the channel, it receives the message,
     * deserialize the value object and updates the MemLoc->ValueObject mapping
     * at the corresponding call site of the receiver.
     */
    void transferMPIRecvOp(SgPointerDerefExp* sgn, Function mpif_);

    void transferMPIBarrier();

    void transferMPIBcastOp(SgPointerDerefExp* sgn, Function mpif_);
    
    list<PartEdgePtr> outGoingEdgesMPICallExp(PartPtr part, string funcname);

    void visit(SgFunctionParameterList* sgn);
    void visit(SgFunctionCallExp* sgn);
    void visit(SgNode* sgn);
    bool finish();
  };

  /*******************
   * MPICommAnalysis *
   *******************/

  /*! 
   * Distributed Termination:
   * Each analysis iterates towards its local fixpoint.
   * When analyses communicate, the dataflow state is merged at the receiver.
   * The join causes new worklist tems to be added to the fixpoint iteration.
   * Each analysis therefore are operating on different points on the worklist.
   * The whole system terminates when every analysis reaches its own fixpoint.
   * To detect this we employ a distributed termination detection scheme.
   * The algorithm is similar to weight throwing scheme.
   * The system has a controlloing agent which is the root process 0.
   * Every process is initially in the idle state.
   * They become active on the reception of a message from the controlling agent.
   * Each active process maintains a counter.
   * Every time a message is sent to a single process the counter is decreased by 1.
   * Every time a message is sent to multiple processes the counter is decreasd by m.
   * 'm' is the number of processes the message was sent to.
   * Consequently, when the controlling agent initiates other processes it decrements its counter by N-1.
   * Every time a message is received, the counter is increased by 1.
   * Similarly, if a message is received from multiple processes the counter is increased by m.
   * When the worklist is empty all non-controlling agents go to idle state.
   * They remain in this state until they get woken up by a new message from an active process.
   * When a process goes to idle state, it sends the the value of the counter to the controlling agent
   * The controlling agent adds the value received to its own counter.
   * The entire system has terminated if the controlling agent's counter counts back to 0.
   * When the controlling agent determines termination, it sends a termination signal for all process to exit.
   *
   * MPICommAnalysis is distributed approach to analyze MPI programs.
   * An instance of MPICommAnalysis is associated with each process.
   * Global fixpoint is a product of local fixpoints.
   * MPICommAnalysis is responsible for carrying out MPI semantics.
   * It implements transfer functions for MPI operations.
   * MPICommAnalysis talks to other MPICommAnalysis using MPI runtime.
   * 
   */

  /*!
   * A simple pair container to store the received value object.
   * ml: ML is the memloc corresponding to the buffer
   * value: value is the ValueObject received through MPI_Recv
   */
  class RecvMLVal : public sight::printable {
    MemLocObjectPtr ml;
    MPICommValueObjectPtr value;
  public:
    RecvMLVal();
    RecvMLVal(MemLocObjectPtr ml, MPICommValueObjectPtr val);
    MPICommValueObjectPtr getValueObject() const;
    MemLocObjectPtr getMemLocObject() const;
    std::string str(std::string indent="") const;
  };
  typedef boost::shared_ptr<RecvMLVal> RecvMLValPtr;


  class MPICommAnalysis : public FWDataflow {
    
    //! Analysis store the received objects on the edge that immediately follows
    std::map<PartEdgePtr, RecvMLValPtr> recvMLValMap;
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

    std::list<PartEdgePtr> matchingPartEdges(PartEdgePtr pedge) const;
    bool insertRecvMLVal(PartEdgePtr pedge, MemLocObjectPtr ml, MPICommValueObjectPtr mvalue);
    std::list<RecvMLValPtr> getRecvMLVal(PartEdgePtr pedge) const;
    MPICommValueObjectPtr mergeMayEqualMLVal(MemLocObjectPtr ml, PartEdgePtr pedge, std::list<RecvMLValPtr>& rmlvals);
    
    std::string stringifyRecvMLValMap() const;
       
    bool implementsATSGraph() { return false; }

    void runMPIAnalysis();

    // pretty print for the object
    std::string str(std::string indent="") const;
  };

}; // end namespace fuse

#endif
