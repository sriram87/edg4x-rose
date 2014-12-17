#ifndef CONSTANT_PROPAGATION_ANALYSIS_H
#define CONSTANT_PROPAGATION_ANALYSIS_H

#include "compose.h"
namespace fuse
{
  class CPValueLattice;
  class CPValueObject;
};
#include "VariableStateTransfer.h"
#include "abstract_object_map.h"
#include <boost/enable_shared_from_this.hpp>

namespace fuse
{
/***************************************
 ***** ConstantPropagationAnalysis *****
 ***************************************/

class ConstantPropagationAnalysis;
  
// This is a forward dataflow analysis that implements a simple abstraction of values 
// that consists of the universal set, a single constant value and an empty set. It 
// maintains a map of memory locations to these value abstractions.

class CPValueLattice;
typedef boost::shared_ptr<CPValueLattice> CPValueLatticePtr;
extern CPValueLatticePtr NULLCPValueLattice;

class CPValueObject;
typedef boost::shared_ptr<CPValueObject> CPValueObjectPtr;
extern CPValueObjectPtr NULLCPValueObject;

class CPValueKind;
typedef boost::shared_ptr<CPValueKind> CPValueKindPtr;
extern CPValueKindPtr NULLCPValueKind;

class CPUninitializedKind;
typedef boost::shared_ptr<CPUninitializedKind> CPUninitializedKindPtr;

class CPConcreteKind;
typedef boost::shared_ptr<CPConcreteKind> CPConcreteKindPtr;

class CPOffsetListKind;
typedef boost::shared_ptr<CPOffsetListKind> CPOffsetListKindPtr;

class CPUnknownKind;
typedef boost::shared_ptr<CPUnknownKind> CPUnknownKindPtr;

class CPValueKind: public sight::printable, public boost::enable_shared_from_this<CPValueKind> {
  public:
  
  // The different kinds of CPValueObjects
  typedef enum {uninitialized, // The value has not been initialized (analyses may set this value to anything they want)
                concrete,   // The exact value is known and can be described with an SgValueExp
                offsetList, // The value denotes the offset of some class/struct member from a pointer the member's 
                            // host object. Since the compiler can add arbitrary padding, such values are lists 
                            // of concrete values and members' ranks within their host objects.
                
                // !!! TODO
                serverImpl, // Thin wrapper for a server-implemented ValueObject
                unknown     // The exact value is not known
     } valueKind;
  valueKind kind;

  static std::string valueKind2Str(valueKind kind)
  { return (kind==uninitialized?"uninitialized":(kind==concrete?"concrete":(kind==offsetList?"offsetList":(kind==serverImpl?"serverImpl":(kind==unknown?"unknown":"???"))))); }
  
  class comparableKind: public comparable {
    public:
    valueKind kind;
    comparableKind(valueKind kind): kind(kind) {}
    bool equal(const comparable& that_arg) const {
      //try {
        const comparableKind& that = dynamic_cast<const comparableKind&>(that_arg);
        return kind == that.kind;
      //} catch (std::bad_cast bc) {
      //  ROSE_ASSERT(0);
      //}
    }
    bool less(const comparable& that_arg) const {
      //try{
        const comparableKind& that = dynamic_cast<const comparableKind&>(that_arg);
        return kind < that.kind;
      //} catch (std::bad_cast bc) {
      //  ROSE_ASSERT(0);
      //}
    }
    std::string str(std::string indent="") const { return valueKind2Str(kind); }
  }; // class comparableKind
  typedef boost::shared_ptr<comparableKind> comparableKindPtr;

  CPValueKind(valueKind kind): kind(kind)
  {}

  valueKind getKind() const { return kind; }
  
  CPUninitializedKindPtr asUninitializedKind()
  { return boost::static_pointer_cast<CPUninitializedKind>(shared_from_this()); }
  CPConcreteKindPtr asConcreteKind()
  { return boost::static_pointer_cast<CPConcreteKind>(shared_from_this()); }
  CPOffsetListKindPtr asOffsetListKind()
  { return boost::static_pointer_cast<CPOffsetListKind>(shared_from_this()); }
  CPUnknownKindPtr asUnknownKind()
  { return boost::static_pointer_cast<CPUnknownKind>(shared_from_this()); }
  
  // Applies the given unary or binary operation to this and the given CPValueKind
  // Returns:
  //    - if this CPValueKind can be updated to incorporate the result of the addition, 
  //       return a freshly-allocated CPValueKind that holds the result.
  //    - if the two objects could not be merged and therefore that must be placed after 
  //       this in the parent CPValueObject's list, return that.
  virtual CPValueKindPtr op(SgUnaryOp* op)=0;
  virtual CPValueKindPtr op(SgBinaryOp* op, CPValueKindPtr that)=0;
  
  // Returns whether this and that CPValueKinds are may/must equal to each other
  virtual bool mayEqualAO(CPValueKindPtr that)=0;
  virtual bool mustEqualAO(CPValueKindPtr that)=0;
  
  // Returns whether the two CPValueKinds denote the same set of concrete values
  virtual bool equalSetAO(CPValueKindPtr that)=0;
  
  // Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given CPValueKind
  virtual bool subSetAO(CPValueKindPtr that)=0;
  
  // Computes the meet of this and that and returns the resulting kind
  virtual std::pair<bool, CPValueKindPtr> meetUpdateAO(CPValueKindPtr that)=0;
  
  // Computes the intersection of this and that and returns the resulting kind
  virtual std::pair<bool, CPValueKindPtr> intersectUpdateAO(CPValueKindPtr that)=0;

  // Returns true if this ValueObject corresponds to a concrete value that is statically-known
  virtual bool isConcrete()=0;
  // Returns the number of concrete values in this set
  virtual int concreteSetSize()=0;
  // Returns the type of the concrete value (if there is one)
  virtual SgType* getConcreteType()=0;
  // Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
  // the normal ROSE mechanisms to decode it
  virtual std::set<boost::shared_ptr<SgValueExp> > getConcreteValue()=0;
  
  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  virtual bool isFullAO(PartEdgePtr pedge)=0;
  // Returns whether this AbstractObject denotes the empty set.
  virtual bool isEmptyAO(PartEdgePtr pedge)=0;
  
  // Returns a copy of this CPConcreteKind
  virtual CPValueKindPtr copyAOType() const=0;
  
  // Appends to the given hierarchical key the additional information that uniquely 
  // identifies this type's set
  virtual void addHierSubKey(const AbstractionHierarchy::hierKeyPtr& key)=0;
}; // CPValueKind

// CPValueObject form a set hierarchy and thus implement the AbstractionHierarchy
// interface. The hierarchy is:
// empty key: unknown
// non-empty:
//   uninitalized: special value to denote the empty set
//   concrete:
//     SgValueExp
//   offsetList:
//     rankT
//       long
//     offsetT
//       long
// The keys the differentiate MemRegionObjects are:
// std::list<comparableKind, comparableSgNode>


// CPValueLattice are used to identify both raw values and the locations of class fields within
// a class. Since there may be arbitrary amounts of padding between such fields, we can only determine
// the order of their offsets within the class object, not the actual concrete values of these offsets.
// Since the application may use these offsets to find additional memory regions (e.g. a.b.c, (&a.b)+1 or
// array[1].a[2].b[3]), we represent such derived offsets as sequences of StxValueObjects, the sum of
// which corresponds to the actual offset.
// Thus,
// a.b.c: region(a), rank(b in a) + rank(c in b)
// (&a.b)+1: region(a), rank(b in a) + concrete(1)
// array[1].a[2].b[3]: region(array), concrete(1*sizeof(elt of array)) + rank(a in array elt) +
//                                    concrete(2*sizeof elt of array[1]) + rank(b in array[1] elt) +
//                                    concrete(3*sizeof elt of array[1].a[2])
// We maintain such sums of StxValueObjects as a linked list, with each object in the chain pointing
// to the next one. However, not all combinations need to be represented explicitly:
// concrete + concrete: concrete object that denotes the sum of the arguments
// concrete + rank, rank + concrete: maintained as a list
// rank + rank: unknown
// * + unknown, unknown + *: a single unknown value object
/*  // pointer(memregion, *1) + *2, *2 + pointer(memregion, *1):
//          resulting object is pointer(memregion, *1+*2), with the + following the above rules */
class CPValueLattice : public FiniteLattice, public boost::enable_shared_from_this<CPValueLattice> {
  CPValueKindPtr kind;

  friend class CPValueObject;

  public:

  // Do we need a default constructor?
  CPValueLattice(PartEdgePtr pedge);
  CPValueLattice(CPValueKindPtr kind, PartEdgePtr pedge);
  CPValueLattice(const CPValueLattice & X);

  // Access functions.
  CPValueKindPtr getKind() const;
  // Sets this object's kind to the given kind, returning true if this causes the CPValueObject to change
  bool setKind(CPValueKindPtr kind);

  void initialize();

  // Returns a copy of this lattice
  Lattice* copy() const;
  
  // Returns a shared pointer to a newly-allocated copy of this CPValueLatice
  CPValueLatticePtr copyCPLat() const;

  // overwrites the state of "this" Lattice with "that" Lattice
  void copy(Lattice* that);
  
  bool operator==(Lattice* that) /*const*/;
  
  // Computes the meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  bool meetUpdate(Lattice* that);
  bool meetUpdate(CPValueLattice* that);
  
  // Computes the intersection of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  bool intersectUpdate(Lattice* that);
  bool intersectUpdate(CPValueLattice* that);

  // Set this Lattice object to represent the set of all possible execution prefixes.
  // Return true if this causes the object to change and false otherwise.
  bool setToFull();
  
  // Set this Lattice object to represent the of no execution prefixes (empty set)
  // Return true if this causes the object to change and false otherwise.
  bool setToEmpty();
  
  // Set all the information associated Lattice object with this MemLocObjectPtr to full.
  // Return true if this causes the object to change and false otherwise.
  bool setMLValueToFull(MemLocObjectPtr ml);
  
  // Returns whether this lattice denotes the set of all possible execution prefixes.
  bool isFull();
  // Returns whether this lattice denotes the empty set.
  bool isEmpty();
  
  // pretty print for the object
  std::string str(std::string indent="") const;
  
  // Applies the given unary or binary operation to this and the given CPValueKind
  // Returns:
  //    - if this CPValueKind can be updated to incorporate the result of the addition, 
  //       return a freshly-allocated CPValueKind that holds the result.
  //    - if the two objects could not be merged and therefore that must be placed after 
  //       this in the parent CPValueObject's list, return that.
  CPValueLatticePtr op(SgUnaryOp* op);
  CPValueLatticePtr op(SgBinaryOp* op, CPValueLatticePtr that);

  // Returns a freshly-allocated CPValueObject that communicates the information from this
  // Lattice to other analyses
  CPValueObjectPtr createValueObject();
}; // class CPValueLattice


// CPValueObjects are ValueObjects that are defined by CPValueLattices
class CPValueObject : public ValueObject {
  public:
  // The lattice that grounds the definition of this ValueObject.
  CPValueLatticePtr ground;

  CPValueObject(CPValueLatticePtr ground);

  // Wrapper for shared_from_this that returns an instance of this class rather than its parent
  CPValueObjectPtr shared_from_this() { return boost::static_pointer_cast<CPValueObject>(ValueObject::shared_from_this()); }

  bool mayEqualAO(ValueObjectPtr o, PartEdgePtr pedge);
  bool mustEqualAO(ValueObjectPtr o, PartEdgePtr pedge);
  
  // Returns whether the two abstract objects denote the same set of concrete objects
  bool equalSetAO(ValueObjectPtr o, PartEdgePtr pedge);
  
  // Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given abstract object.
  bool subSetAO(ValueObjectPtr o, PartEdgePtr pedge);
  
  // Computes the meet of this and that and returns the resulting kind
  bool meetUpdateAO(ValueObjectPtr that, PartEdgePtr pedge);
  
  // Computes the intersect of this and that and returns the resulting kind
  bool intersectUpdateAO(ValueObjectPtr that, PartEdgePtr pedge);

  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  bool isFullAO(PartEdgePtr pedge);
  // Returns whether this AbstractObject denotes the empty set.
  bool isEmptyAO(PartEdgePtr pedge);
  
  // Allocates a copy of this object and returns a pointer to it
  ValueObjectPtr copyAOType() const;

  // Returns true if this ValueObject corresponds to a concrete value that is statically-known
  bool isConcrete();
  // Returns the number of concrete values in this set
  int concreteSetSize();
  // Returns the type of the concrete value (if there is one)
  SgType* getConcreteType();
  // Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
  // the normal ROSE mechanisms to decode it
  set<boost::shared_ptr<SgValueExp> > getConcreteValue();
  
  // Returns whether all instances of this class form a hierarchy. Every instance of the same
  // class created by the same analysis must return the same value from this method!
  bool isHierarchy() const { return true; }
  
  // Returns a key that uniquely identifies this particular AbstractObject in the 
  // set hierarchy.
  const hierKeyPtr& getHierKey() const;

  // pretty print for the object
  std::string str(std::string indent="") const;
  std::string strp(PartEdgePtr pedge, std::string indent="") const;
}; // class CPValueObject

class CPUninitializedKind : public CPValueKind {
  public:
  CPUninitializedKind() : CPValueKind(uninitialized)
  {}

  // Applies the given unary or binary operation to this and the given CPValueKind
  // Returns:
  //    - if this CPValueKind can be updated to incorporate the result of the addition, 
  //       return a freshly-allocated CPValueKind that holds the result.
  //    - if the two objects could not be merged and therefore that must be placed after 
  //       this in the parent CPValueObject's list, return that.
  CPValueKindPtr op(SgUnaryOp* op);
  CPValueKindPtr op(SgBinaryOp* op, CPValueKindPtr that);
  
  // Returns whether this and that CPValueKinds are may/must equal to each other
  bool mayEqualAO(CPValueKindPtr that);
  bool mustEqualAO(CPValueKindPtr that);
  
  // Returns whether the two CPValueKinds denote the same set of concrete values
  bool equalSetAO(CPValueKindPtr that);
  
  // Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given CPValueKind
  bool subSetAO(CPValueKindPtr that);
  
  // Computes the meet of this and that and returns the resulting kind
  std::pair<bool, CPValueKindPtr> meetUpdateAO(CPValueKindPtr that);
  
  // Computes the intersection of this and that and returns the resulting kind
  std::pair<bool, CPValueKindPtr> intersectUpdateAO(CPValueKindPtr that);

  // Returns true if this ValueObject corresponds to a concrete value that is statically-known
  bool isConcrete();
  // Returns the number of concrete values in this set
  int concreteSetSize();
  // Returns the type of the concrete value (if there is one)
  SgType* getConcreteType();
  // Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
  // the normal ROSE mechanisms to decode it
  std::set<boost::shared_ptr<SgValueExp> > getConcreteValue();
  
  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  bool isFullAO(PartEdgePtr pedge);
  // Returns whether this AbstractObject denotes the empty set.
  bool isEmptyAO(PartEdgePtr pedge);
  
  // Returns a copy of this CPUninitializedKind
  CPValueKindPtr copyAOType() const { return boost::make_shared<CPUninitializedKind>(); }
  
  std::string str(std::string indent="") const;
  
  // Appends to the given hierarchical key the additional information that uniquely 
  // identifies this type's set
  void addHierSubKey(const AbstractionHierarchy::hierKeyPtr& key)
  // Don't add anything else since the hier-level key already separates the empty
  // set from all the other sets
  { }
}; // CPUninitializedKind

class CPConcreteKind : public CPValueKind {
  // The expression that denotes the known value of this object
  boost::shared_ptr<SgValueExp> exp;

  public:
  CPConcreteKind(boost::shared_ptr<SgValueExp> exp) : CPValueKind(concrete), exp(exp)
  {}

  boost::shared_ptr<SgValueExp> getVal() const { return exp; }
  
  // Applies the given operation functor to the expression in this ConcreteKind and returns the resulting CPKind
  template<class DoOpType>
  CPValueKindPtr doUnaryOp(DoOpType& doOp);
  
  // Applies the given operation functor to the expression in this ConcreteKind, whoch is assumed to be an integral type 
  // and returns the resulting CPKind
  template<class DoOpType>
  CPValueKindPtr doUnaryIntegralOp(DoOpType& doOp);
  
  // Applies the given operation functor to the expression in this ConcreteKind, which is assumed to a floating point type 
  // and returns the resulting CPKind
  template<class DoOpType>
  CPValueKindPtr doUnaryFloatOp(DoOpType& doOp);
  
  template<class DoOpType, class DoOpRetType>
  DoOpRetType bindDoOpArgs1(DoOpType doOp);
  
  template<class DoOpType, class DoOpRetType>
  DoOpRetType bindDoOpArgs2(DoOpType doOp);
  
  // Creates a CPConcreteKind from the given value. This function is overloaded with different argument types
  // and for each type it creates a CPConcreteKind with a different SgValueExp.
  CPValueKindPtr createCPValueKindFromVal(bool val);
  CPValueKindPtr createCPValueKindFromVal(char val);
  CPValueKindPtr createCPValueKindFromVal(short val);
  CPValueKindPtr createCPValueKindFromVal(int val);
  CPValueKindPtr createCPValueKindFromVal(long val);
  CPValueKindPtr createCPValueKindFromVal(long long val);
  CPValueKindPtr createCPValueKindFromVal(unsigned char val);
  CPValueKindPtr createCPValueKindFromVal(unsigned short val);
  CPValueKindPtr createCPValueKindFromVal(unsigned int val);
  CPValueKindPtr createCPValueKindFromVal(unsigned long val);
  CPValueKindPtr createCPValueKindFromVal(unsigned long long val);
  CPValueKindPtr createCPValueKindFromVal(wchar_t val);
  CPValueKindPtr createCPValueKindFromVal(float val);
  CPValueKindPtr createCPValueKindFromVal(double val);
  CPValueKindPtr createCPValueKindFromVal(long double val);
  
  // Applies the given binary operation functor to the expression in this and that ConcreteKinds, assuming they're
  // both integral types and returns the resulting CPKind
  template<class DoOpType>
  CPValueKindPtr doBinaryOpIntInt(DoOpType& doOp, CPConcreteKindPtr that);
  
  // Applies the given binary operation functor to the expression in this and that ConcreteKinds, assuming at least one
  // is not an integral type and returns the resulting CPKind
  template<class DoOpType>
  CPValueKindPtr doBinaryOp(DoOpType& doOp, CPConcreteKindPtr that);
  
  // Applies the given unary or binary operation to this and the given CPValueKind
  // Returns:
  //    - if this CPValueKind can be updated to incorporate the result of the addition, 
  //       return a freshly-allocated CPValueKind that holds the result.
  //    - if the two objects could not be merged and therefore that must be placed after 
  //       this in the parent CPValueObject's list, return that.
  CPValueKindPtr op(SgUnaryOp* op);
  CPValueKindPtr op(SgBinaryOp* op, CPValueKindPtr that);
  
  // Returns whether the two given SgValueExps denote the same numeric value. 
  // If unknown, return unknownVal.
  static bool equalVals(SgValueExp* val1, SgValueExp* val2, bool unknownVal);
  
  // Returns whether the SgValueExps denoted by val1 < the value denoted by val2.
  // If unknown, return unknownVal.
  static bool lessThanVals(SgValueExp* val1, SgValueExp* val2, bool unknownVal);
  
  // Returns whether this and that CPValueKinds are may/must equal to each other
  bool mayEqualAO(CPValueKindPtr that);
  bool mustEqualAO(CPValueKindPtr that);
  
  // Returns whether the two CPValueKinds denote the same set of concrete values
  bool equalSetAO(CPValueKindPtr that);
  
  // Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given CPValueKind
  bool subSetAO(CPValueKindPtr that);
  
  // Computes the meet of this and that and returns the resulting kind
  std::pair<bool, CPValueKindPtr> meetUpdateAO(CPValueKindPtr that);
  
  // Computes the intersection of this and that and returns the resulting kind
  std::pair<bool, CPValueKindPtr> intersectUpdateAO(CPValueKindPtr that);

  // Returns true if this ValueObject corresponds to a concrete value that is statically-known
  bool isConcrete();
  // Returns the number of concrete values in this set
  int concreteSetSize();
  // Returns the type of the concrete value (if there is one)
  SgType* getConcreteType();
  // Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
  // the normal ROSE mechanisms to decode it
  std::set<boost::shared_ptr<SgValueExp> > getConcreteValue();
  
  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  bool isFullAO(PartEdgePtr pedge);
  // Returns whether this AbstractObject denotes the empty set.
  bool isEmptyAO(PartEdgePtr pedge);
  
  // Returns a copy of this CPConcreteKind
  CPValueKindPtr copyAOType() const { return boost::make_shared<CPConcreteKind>(getVal()); }
  
  std::string str(std::string indent="") const;
  
  // Generic wrapper for comparing SgNode*'s that implements the comparable interface
  class comparableSgValueExp : public comparable {
    protected:
    SgValueExp *val;
    public:
    comparableSgValueExp(SgValueExp* val): val(val) {}
    bool equal(const comparable& that_arg) const {
      //try{
        const comparableSgValueExp& that = dynamic_cast<const comparableSgValueExp&>(that_arg);
        return equalVals(val, that.val, val==that.val);
      //} catch (std::bad_cast bc) {
      //  ROSE_ASSERT(0);
      //}
    }
    bool less(const comparable& that_arg) const {
      //try{
        const comparableSgValueExp& that = dynamic_cast<const comparableSgValueExp&>(that_arg);
        return equalVals(val, that.val, val<that.val);
      //} catch (std::bad_cast bc) {
      //  ROSE_ASSERT(0);
      //}
    }
    std::string str(std::string indent="") const { return SgNode2Str(val); }
  };
  typedef boost::shared_ptr<comparableSgNode> comparableSgNodePtr;
  
  // Appends to the given hierarchical key the additional information that uniquely 
  // identifies this type's set
  void addHierSubKey(const AbstractionHierarchy::hierKeyPtr& key)
  { key->add(boost::make_shared<comparableSgValueExp>(exp.get())); }

}; // CPConcreteKind

class CPOffsetListKind : public CPValueKind {
  class intWrap: public comparable {
    public:
    typedef enum {rankT, offsetT} type;
    
    private:
    type t;
    
    long long v;
    public:
    intWrap(long long v, type t): t(t), v(v){}
    intWrap(const intWrap& that): t(that.t), v(that.v){}
    
    bool operator==(const intWrap& that) const { return t==that.t && v==that.v; }
    bool operator!=(const intWrap& that) const { return !(*this == that); }
    
    long long operator=(long long v) { return (this->v = v);}
    
    long long get() const { return v; }
    void set(unsigned long long v) { this->v = v; }
    
    type getType() const { return t; }
    
    bool equal(const comparable& that_arg) const {
      //try {
        const intWrap& that = dynamic_cast<const intWrap&>(that_arg);
        return t == that.t && v == that.v;
      //} catch (std::bad_cast bc) {
      //  ROSE_ASSERT(0);
      //}
    }
    bool less(const comparable& that_arg) const {
      //try{
        const intWrap& that = dynamic_cast<const intWrap&>(that_arg);
        return (t  < that.t) || 
               (t == that.t && v < that.v);
      //} catch (std::bad_cast bc) {
      //  ROSE_ASSERT(0);
      //}
    }
    std::string str(std::string indent="") const { return sight::txt()<<"[type="<<(t==rankT?"rankT":(t==offsetT?"offsetT":"NULL"))<<", v="<<v<<"]"; }
  }; // class intWrap
  typedef boost::shared_ptr<intWrap> intWrapPtr;
  
  static SgTypeLongLong* type;
  
  public:
  
  // Represents the rank of a field within a class/struct (full offset is not known)
  class rank : public intWrap {
    public:
    rank(long long v): intWrap(v, intWrap::rankT) {}
  };
  
  // Represents a known offset from the address of some memory region or an internal class field
  class offset : public intWrap {
    public:
    offset(long long v): intWrap(v, intWrap::offsetT) {}
  };
  
  std::list<intWrap> offsetL;
  
  public:
  CPOffsetListKind(const rank& r) : CPValueKind(offsetList) {
    if(type==NULL) type = SageBuilder::buildLongLongType();
    offsetL.push_back(r);
  }
  
  CPOffsetListKind(const offset& o) : CPValueKind(offsetList) {
    if(type==NULL) type = SageBuilder::buildLongLongType();
    offsetL.push_back(o);
  }
  
  CPOffsetListKind(const std::list<intWrap>& offsetL) : CPValueKind(offsetList), offsetL(offsetL) { 
    if(type==NULL) type = SageBuilder::buildLongLongType();
  }
  
  // Applies the given unary or binary operation to this and the given CPValueKind
  // Returns:
  //    - if this CPValueKind can be updated to incorporate the result of the addition, 
  //       return a freshly-allocated CPValueKind that holds the result.
  //    - if the two objects could not be merged and therefore that must be placed after 
  //       this in the parent CPValueObject's list, return that.
  CPValueKindPtr op(SgUnaryOp* op);
  CPValueKindPtr op(SgBinaryOp* op, CPValueKindPtr that);
  
  // Returns whether this and that CPValueKinds are may/must equal to each other
  bool mayEqualAO(CPValueKindPtr that);
  bool mustEqualAO(CPValueKindPtr that);
  
  // Returns whether the two CPValueKinds denote the same set of concrete values
  bool equalSetAO(CPValueKindPtr that);
  
  // Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given CPValueKind
  bool subSetAO(CPValueKindPtr that);
  
  // Computes the meet of this and that and returns the resulting kind
  std::pair<bool, CPValueKindPtr> meetUpdateAO(CPValueKindPtr that);
  
  // Computes the intersection of this and that and returns the resulting kind
  std::pair<bool, CPValueKindPtr> intersectUpdateAO(CPValueKindPtr that);

  // Returns true if this ValueObject corresponds to a concrete value that is statically-known
  bool isConcrete();
  // Returns the number of concrete values in this set
  int concreteSetSize();
  // Returns the type of the concrete value (if there is one)
  SgType* getConcreteType();
  // Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
  // the normal ROSE mechanisms to decode it
  std::set<boost::shared_ptr<SgValueExp> > getConcreteValue();
  
  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  bool isFullAO(PartEdgePtr pedge);
  // Returns whether this AbstractObject denotes the empty set.
  bool isEmptyAO(PartEdgePtr pedge);
  
  // Returns a copy of this CPOffsetListKind
  CPValueKindPtr copyAOType() const { return boost::make_shared<CPOffsetListKind>(offsetL); }
  
  std::string str(std::string indent="") const;
  
  // Appends to the given hierarchical key the additional information that uniquely 
  // identifies this type's set
  void addHierSubKey(const AbstractionHierarchy::hierKeyPtr& key) {
    // Add all the intWrap objects on the offset list to the key
    for(std::list<intWrap>::iterator o=offsetL.begin(); o!=offsetL.end(); o++)
      key->add(boost::make_shared<intWrap>(*o));
  }
}; // CPOffsetListKind

class CPUnknownKind : public CPValueKind {
  public:
  CPUnknownKind() : CPValueKind(unknown)
  {}

  // Applies the given unary or binary operation to this and the given CPValueKind
  // Returns:
  //    - if this CPValueKind can be updated to incorporate the result of the addition, 
  //       return a freshly-allocated CPValueKind that holds the result.
  //    - if the two objects could not be merged and therefore that must be placed after 
  //       this in the parent CPValueObject's list, return that.
  CPValueKindPtr op(SgUnaryOp* op);
  CPValueKindPtr op(SgBinaryOp* op, CPValueKindPtr that);
  
  // Returns whether this and that CPValueKinds are may/must equal to each other
  bool mayEqualAO(CPValueKindPtr that);
  bool mustEqualAO(CPValueKindPtr that);
  
  // Returns whether the two CPValueKinds denote the same set of concrete values
  bool equalSetAO(CPValueKindPtr that);
  
  // Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given CPValueKind
  bool subSetAO(CPValueKindPtr that);
  
  // Computes the meet of this and that and returns the resulting kind
  std::pair<bool, CPValueKindPtr> meetUpdateAO(CPValueKindPtr that);
  
  // Computes the intersection of this and that and returns the resulting kind
  std::pair<bool, CPValueKindPtr> intersectUpdateAO(CPValueKindPtr that);

  // Returns true if this ValueObject corresponds to a concrete value that is statically-known
  bool isConcrete();
  // Returns the number of concrete values in this set
  int concreteSetSize();
  // Returns the type of the concrete value (if there is one)
  SgType* getConcreteType();
  // Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
  // the normal ROSE mechanisms to decode it
  std::set<boost::shared_ptr<SgValueExp> > getConcreteValue();
  
  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  bool isFullAO(PartEdgePtr pedge);
  // Returns whether this AbstractObject denotes the empty set.
  bool isEmptyAO(PartEdgePtr pedge);
  
  // Returns a copy of this CPUnknownKind
  CPValueKindPtr copyAOType() const { return boost::make_shared<CPUnknownKind>(); }
  
  std::string str(std::string indent="") const;
  
  // Appends to the given hierarchical key the additional information that uniquely 
  // identifies this type's set
  void addHierSubKey(const AbstractionHierarchy::hierKeyPtr& key)
  // Don't add anything else since the full set is denoted by the empty key
  { }
}; // CPUnknownKind

class CPMemLocObject;
typedef boost::shared_ptr<CPMemLocObject> CPMemLocObjectPtr;

class CPMemLocObject: public virtual MemLocObject/*, public FiniteLattice*/
{
  ConstantPropagationAnalysis* analysis;
  // Records whether this object is full or empty
  bool isCPFull;
  bool isCPEmpty;
  
  public:
  CPMemLocObject(bool isCPFull, bool isCPEmpty, SgNode* base, PartEdgePtr pedge, ConstantPropagationAnalysis* analysis) : 
    MemLocObject(base),
    analysis(analysis),
    isCPFull(isCPFull), isCPEmpty(isCPEmpty)
  { }
  
  CPMemLocObject(MemRegionObjectPtr region, CPValueObjectPtr index, SgNode* base, PartEdgePtr pedge, ConstantPropagationAnalysis* analysis) : 
    MemLocObject(region, index, base),
    analysis(analysis),
    isCPFull(false), isCPEmpty(false)
  { }
  
  CPMemLocObject(const CPMemLocObject& that) : 
    MemLocObject(that), 
    analysis(that.analysis),
    isCPFull(false), isCPEmpty(false)
  { }
  
  CPValueObjectPtr getCPIndex() const {
    return boost::dynamic_pointer_cast<CPValueObject>(getIndex());
  }
  
  // returns a copy of this lattice
  //Lattice* copy() const;
  
  // Initializes this Lattice to its default state, if it is not already initialized
  //void initialize() {}

  
  //bool operator==(Lattice*);
  
  // Called by analyses to transfer this lattice's contents from across function scopes from a caller function 
  //    to a callee's scope and vice versa. If this this lattice maintains any information on the basis of 
  //    individual MemLocObjects these mappings must be converted, with MemLocObjects that are keys of the ml2ml 
  //    replaced with their corresponding values. If a given key of ml2ml does not appear in the lattice, it must
  //    be added to the lattice and assigned a default initial value. In many cases (e.g. over-approximate sets 
  //    of MemLocObjects) this may not require any actual insertions. If the value of a given ml2ml mapping is 
  //    NULL (empty boost::shared_ptr), any information for MemLocObjects that must-equal to the key should be 
  //    deleted.
  // Since the function is called for the scope change across some Part, it needs to account for the fact that
  //    the keys in ml2ml are in scope on one side of Part, while the values on the other side. Specifically, it is
  //    guaranteed that the keys are in scope at fromPEdge while the values are in scope at the edge returned 
  //    by getPartEdge().
  //Lattice* remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge);
  
  // Adds information about the MemLocObjects in newL to this Lattice, overwriting any information previously 
  //    maintained in this lattice about them.
  // Returns true if the Lattice state is modified and false otherwise.
  //bool replaceML(Lattice* newL) { return false; }
  
  // Returns whether this object may/must be equal to o within the given Part p
  // These methods are called by composers and should not be called by analyses.
  bool mayEqualAO(MemLocObjectPtr o, PartEdgePtr pedge);
  bool mustEqualAO(MemLocObjectPtr o, PartEdgePtr pedge);
  
  // Returns whether the two abstract objects denote the same set of concrete objects
  // These methods are called by composers and should not be called by analyses.
  bool equalSetAO(MemLocObjectPtr o, PartEdgePtr pedge);
  
  // Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given abstract object.
  // These methods are called by composers and should not be called by analyses.
  bool subSetAO(MemLocObjectPtr o, PartEdgePtr pedge);
  
  // Returns true if this object is live at the given part and false otherwise
  bool isLiveAO(PartEdgePtr pedge);
  
  // Set this Lattice object to represent the set of all possible execution prefixes.
  // Return true if this causes the object to change and false otherwise.
  bool setToFull();
  // Set this Lattice object to represent the of no execution prefixes (empty set).
  // Return true if this causes the object to change and false otherwise.
  bool setToEmpty();
  
  // Set all the value information that this Lattice object associates with this MemLocObjectPtr to full.
  // Return true if this causes the object to change and false otherwise.
  //bool setMLValueToFull(MemLocObjectPtr ml) { return false; }
  /*
  // Returns whether this lattice denotes the set of all possible execution prefixes.
  bool isFull();
  // Returns whether this lattice denotes the empty set.
  bool isEmpty();*/
  
  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  bool isFullAO(PartEdgePtr pedge);
  // Returns whether this AbstractObject denotes the empty set.
  bool isEmptyAO(PartEdgePtr pedge);
  
  // Computes the meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  bool meetUpdateAO(MemLocObjectPtr that, PartEdgePtr pedge);
  
/*  // Computes the meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  // The part of this object is to be used for AbstractObject comparisons.
  bool meetUpdate(Lattice* that);
  
  // Computes the meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  bool meetUpdate(CPMemLocObject* that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);*/

  // Computes the meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  bool intersectUpdateAO(MemLocObjectPtr that, PartEdgePtr pedge);
/*
  // Computes intersection meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  // The part of this object is to be used for AbstractObject comparisons.
  bool intersectUpdate(Lattice* that);

  // Computes the meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  bool intersectUpdate(CPMemLocObject* that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);*/

  // Allocates a copy of this object and returns a pointer to it
  MemLocObjectPtr copyAOType() const;
  
  // Allocates a copy of this object and returns a regular pointer to it
  //MemLocObject* copyAOTypePtr() const;
  
  std::string str(std::string indent="") const; // pretty print for the object
}; // CPMemLocObject
  
class CPMemLocObjectNodeFact: public NodeFact
{
  public:
  CPMemLocObjectPtr ml;
  CPMemLocObjectNodeFact(CPMemLocObjectPtr ml): ml(ml) {}
  
  // returns a copy of this node fact
  NodeFact* copy() const { return new CPMemLocObjectNodeFact(ml); }
  std::string str(std::string indent="") const { return ml->str(); }
};

class ConstantPropagationAnalysis : virtual public FWDataflow
{
  protected:
  //static std::map<varID, Lattice*> constVars;
  //AbstractObjectMap constVars;
   
  public:
  ConstantPropagationAnalysis(bool useSSA);
  
  // Returns a shared pointer to a freshly-allocated copy of this ComposedAnalysis object
  ComposedAnalysisPtr copy() { return boost::make_shared<ConstantPropagationAnalysis>(useSSA); }

  // Creates a basic CPMemLocObject for the given SgNode. This object does not take into
  // account any constant propagation and will be used as a seed from which to propagate 
  // more precise information.
  CPMemLocObjectPtr createBasicCPML(SgNode* sgn, PartEdgePtr pedge);
  
  // Initializes the state of analysis lattices at the given function, part and edge into our out of the part
  // by setting initLattices to refer to freshly-allocated Lattice objects.
  void genInitLattice(PartPtr part, PartEdgePtr pedge, 
                      std::vector<Lattice*>& initLattices);
  
  bool transfer(PartPtr part, CFGNode cn, NodeState& state, std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);
  
  boost::shared_ptr<DFTransferVisitor> getTransferVisitor(PartPtr part, CFGNode cn, 
                                              NodeState& state, std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);
  
  boost::shared_ptr<ValueObject> Expr2Val(SgNode* n, PartEdgePtr pedge);
  bool implementsExpr2Val() { return true; }
  implTightness Expr2ValTightness() { return ComposedAnalysis::tight; }
  
  boost::shared_ptr<MemLocObject> Expr2MemLoc(SgNode* n, PartEdgePtr pedge);
  bool implementsExpr2MemLoc() { return true; }
  implTightness Expr2MemLocTightness() { return ComposedAnalysis::tight; }
  
  // pretty print for the object
  std::string str(std::string indent="") const
  { return "ConstPropAnal"; }
  
  friend class ConstantPropagationAnalysisTransfer;
}; // class ConstantPropagationAnalysis

class ConstantPropagationAnalysisTransfer : public VariableStateTransfer<CPValueLattice, ConstantPropagationAnalysis>
{
  private:
  
  // Transfer function for logical short-circuit operations: && and ||
  //void transferShortCircuitLogical(SgBinaryOp *sgn);

  public:
  //  void visit(SgNode *);
  // Values
  void visit(SgVarRefExp *vref);
  void visit(SgDotExp *dot);
  void visit(SgPntrArrRefExp *paRef);
  void visit(SgBinaryOp *sgn);
  
  // Unary ops that update the operand
  void visit(SgMinusMinusOp *sgn);
  void visit(SgPlusPlusOp *sgn);
  // Unary ops that do not update the operand
  // void visit(SgUnaryOp *sgn);
  void visit(SgCastExp* sgn);
  void visit(SgMinusOp* sgn);
  void visit(SgNotOp* sgn);
  
  void visit(SgValueExp *val);
  
  ConstantPropagationAnalysisTransfer(PartPtr part, CFGNode cn, NodeState& state, 
                                      std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo, 
                                      Composer* composer, ConstantPropagationAnalysis* analysis);
};

}; //namespace fuse

#endif
