#include "sage3basic.h"
#include "const_prop_analysis.h"

#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/make_shared.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/casts.hpp>
#include "sageInterface.h"

using namespace std;
using namespace sight;
using namespace SageInterface;

#include <cwchar>

#define constantPropagationAnalysisDebugLevel 1

// Define type conversions for lambda operators that are not supported by Boost::Lambda
namespace boost { 
namespace lambda {
  
template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, char> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, char, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, short> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, short, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, int, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, long> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, unsigned char> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned char, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, unsigned short> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned short, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, unsigned int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned int, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, unsigned long> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, float> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, float, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, double> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, double, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, long double> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long double, long long int> {
  typedef long long type;
};

//////////////////////////////////////


template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, char> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, char, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, short> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, short, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, int, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, long> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, unsigned long long int> {
  typedef long long type;
};


template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, unsigned char> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned char, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, unsigned short> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned short, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, unsigned int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned int, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, unsigned long> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, float> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, float, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, double> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, double, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, long double> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long double, unsigned long long int> {
  typedef long long type;
};

//////////////////////////////////////



template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, char> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, char, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, short> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, short, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, int, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, long> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, unsigned char> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned char, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, unsigned short> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned short, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, unsigned int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned int, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, unsigned long> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long long int, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, unsigned long long int> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, unsigned long long int, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, float> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, float, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, double> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, double, wchar_t> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, wchar_t, long double> {
  typedef long long type;
};

template<class Act> 
struct plain_return_type_2<arithmetic_action<Act>, long double, wchar_t> {
  typedef long long type;
};
}
}

namespace fuse {

// ************************
// **** CPValueLattice *****
// ************************

CPValueLatticePtr NULLCPValueLattice;
CPValueKindPtr NULLCPValueKind;

CPValueLattice::CPValueLattice(PartEdgePtr pedge) :
  Lattice(pedge), FiniteLattice(pedge)
{
  kind = boost::make_shared<CPUninitializedKind>();
}

CPValueLattice::CPValueLattice(CPValueKindPtr kind, PartEdgePtr pedge) :
  Lattice(pedge), FiniteLattice(pedge), kind(kind)
{
}

CPValueLattice::CPValueLattice(const CPValueLattice & that) :
  Lattice(that.latPEdge), FiniteLattice(that.latPEdge)
{
  this->kind = that.kind->copyAOType();
}

CPValueKindPtr CPValueLattice::getKind() const {
  return kind;
}

bool CPValueLattice::setKind(CPValueKindPtr kind) {
  bool modified = (this->kind->getKind() == kind->getKind() &&
                   this->kind->equalSetAO(kind));
  this->kind = kind;
  return modified;
}

void
CPValueLattice::initialize()
{
  // Use the default constructor (implemented above).
  // So nothing to do here.
}

// returns a copy of this lattice
Lattice*
CPValueLattice::copy() const
{
  return new CPValueLattice(*this);
}

// Returns a shared pointer to a newly-allocated copy of this CPValueLatice
CPValueLatticePtr CPValueLattice::copyCPLat() const
{ return boost::make_shared<CPValueLattice>(*this); }


// Overwrites the state of "this" Lattice with "that" Lattice
void
CPValueLattice::copy(Lattice* X)
{
  Lattice::copy(X);
  CPValueLattice* that = dynamic_cast<CPValueLattice*>(X);
  assert(that);

  this->kind = that->kind;
}


bool
CPValueLattice::operator==(Lattice* X) /*const*/
{
  // Implementation of equality operator.
  CPValueLattice* that = dynamic_cast<CPValueLattice*>(X);
  assert(that);
  return (this->kind->getKind() == that->kind->getKind() &&
          this->kind->equalSetAO(that->kind));
}

// Computes the meet of this and that and saves the result in this
// Returns true if this causes this to change and false otherwise
bool 
CPValueLattice::meetUpdate(Lattice* X)
{
  CPValueLattice* that = dynamic_cast<CPValueLattice*>(X);
  assert(that);
  return meetUpdate(that);
}

bool
CPValueLattice::meetUpdate(CPValueLattice* that)
{
  pair<bool, CPValueKindPtr> ret = kind->meetUpdateAO(that->kind);
  // Update kind
  kind = ret.second;
  
  // Return whether kind was modified
  return ret.first;
  
/*  if(this->kind->getKind()==CPValueKind::uninitialized && )
  if (this->level == emptySet) {
    if(that->level == emptySet) {
      // leave it and return false
      return false;
    } else if (that->level == constantValue) {
      this->level = constantValue;
      this->value = that->value;
      return true;
    } else if (that->level == fullSet) {
      this->level = fullSet;
      return true;
    }
    assert(0); // We should never get here
  } else if (this->level == constantValue) {
    if(that->level == emptySet) {
      return false;
    } else if (that->level == constantValue) {
      if (this->value == that->value) {
        return false;
      } else {
        this->level = fullSet;
        return true;
      }
    } else if (that->level == fullSet) {
      this->level = fullSet;
      return true;
    }
    assert(0); // We should never get here
  } else if (this->level == fullSet) { 
    // Already at the fullSet. Cannot go up further.
    return false;
  }

  // Make up a return value for now.
  return false;*/
}

// Computes the intersection of this and that and saves the result in this
// Returns true if this causes this to change and false otherwise
bool
CPValueLattice::intersectUpdate(Lattice* X)
{
  CPValueLattice* that = dynamic_cast<CPValueLattice*>(X);
  assert(that);
  return intersectUpdate(that);
}

bool
CPValueLattice::intersectUpdate(CPValueLattice* that)
{
  pair<bool, CPValueKindPtr> ret = kind->intersectUpdateAO(that->kind);
  // Update kind
  kind = ret.second;

  // Return whether kind was modified
  return ret.first;
}

// Set this Lattice object to represent the set of all possible execution prefixes.
// Return true if this causes the object to change and false otherwise.
bool
CPValueLattice::setToFull()
{
  if(kind->getKind()!=CPValueKind::unknown) {
    kind = boost::make_shared<CPUnknownKind>();
    return true;
  } else
    return false;
}

// Set this Lattice object to represent the of no execution prefixes (empty set)
// Return true if this causes the object to change and false otherwise.
bool
CPValueLattice::setToEmpty()
{
  if(kind->getKind()!=CPValueKind::uninitialized) {
    kind = boost::make_shared<CPUninitializedKind>();
    return true;
  } else
    return false;
}

// Set all the information associated Lattice object with this MemLocObjectPtr to full.
// Return true if this causes the object to change and false otherwise.
bool
CPValueLattice::setMLValueToFull(MemLocObjectPtr ml)
{
  // Do nothing since this object does not contain information about MemLocObjects
  return false;
}

// Returns whether this lattice denotes the set of all possible execution prefixes.
bool
CPValueLattice::isFull()
{ return kind->isFullAO(getPartEdge()); }

// Returns whether this lattice denotes the empty set.
bool
CPValueLattice::isEmpty()
{ return kind->isEmptyAO(getPartEdge()); }

string
CPValueLattice::str(string indent) const
{
  return kind->str(indent);
}

// Applies the given unary or binary operation to this and the given CPValueKind
// Returns:
//    - if this CPValueKind can be updated to incorporate the result of the addition, 
//       return a freshly-allocated CPValueKind that holds the result.
//    - if the two objects could not be merged and therefore that must be placed after 
//       this in the parent CPValueLattice's list, return that.
CPValueLatticePtr CPValueLattice::op(SgUnaryOp* op) {
  SIGHT_VERB_DECL(scope, (txt()<<"CPValueLattice::op(SgUnaryOp "<<SgNode2Str(op)<<")", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  SIGHT_VERB(dbg << "this="<<str()<<endl, 1, constantPropagationAnalysisDebugLevel)
  return boost::make_shared<CPValueLattice>(kind->op(op), getPartEdge());
}

CPValueLatticePtr CPValueLattice::op(SgBinaryOp* op, CPValueLatticePtr that) {
  SIGHT_VERB_DECL(scope, (txt()<<"CPValueLattice::op(SgBinaryOp "<<SgNode2Str(op)<<")", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  SIGHT_VERB_IF(1, constantPropagationAnalysisDebugLevel)
    dbg << "this="<<str()<<endl;
    dbg << "that="<<(that? that->str(): "NULL")<<endl;
  SIGHT_VERB_FI()
  if(that) return boost::make_shared<CPValueLattice>(kind->op(op, that->kind), getPartEdge());
  else     return boost::make_shared<CPValueLattice>(kind->op(op, NULLCPValueKind), getPartEdge());
}

// Returns a freshly-allocated CPValueObject that communicates the information from this
// Lattice to other analyses
CPValueObjectPtr CPValueLattice::createValueObject()
{ return boost::make_shared<CPValueObject>(shared_from_this()); }

// ************************
// **** CPValueObject *****
// ************************

CPValueObjectPtr NULLCPValueObject;

CPValueObject::CPValueObject(CPValueLatticePtr ground) : ValueObject(NULL), ground(ground)
{
}

bool CPValueObject::mayEqualAO(ValueObjectPtr o, PartEdgePtr pedge)
{
  CPValueObjectPtr that = boost::dynamic_pointer_cast<CPValueObject>(o);
  assert(that);
  return ground->getKind()->mustEqualAO(that->ground->getKind());
}

bool CPValueObject::mustEqualAO(ValueObjectPtr o, PartEdgePtr pedge)
{
  CPValueObjectPtr that = boost::dynamic_pointer_cast<CPValueObject>(o);
  assert(that);
  return ground->getKind()->mustEqualAO(that->ground->getKind());
}

// Returns whether the two abstract objects denote the same set of concrete objects
bool CPValueObject::equalSetAO(ValueObjectPtr o, PartEdgePtr pedge)
{
  CPValueObjectPtr that = boost::dynamic_pointer_cast<CPValueObject>(o);
  assert(that);
  return ground->getKind()->equalSetAO(that->ground->getKind());
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
bool CPValueObject::subSetAO(ValueObjectPtr o, PartEdgePtr pedge)
{
  CPValueObjectPtr that = boost::dynamic_pointer_cast<CPValueObject>(o);
  assert(that);
  return ground->getKind()->subSetAO(that->ground->getKind());
}

// Computes the meet of this and that and saves the result in this.
// Returns true if this causes this to change and false otherwise.
bool CPValueObject::meetUpdateAO(ValueObjectPtr o, PartEdgePtr pedge)
{
  CPValueObjectPtr that = boost::dynamic_pointer_cast<CPValueObject>(o);
  assert(that);
  // Note: this does not change the original lattice since the base of a CPValueObject
  //       always a copy of the original lattice
  return ground->meetUpdate(that->ground.get());
}

// Computes the intersection of this and that and saves the result in this.
// Returns true if this causes this to change and false otherwise.
bool CPValueObject::intersectUpdateAO(ValueObjectPtr o, PartEdgePtr pedge)
{
  CPValueObjectPtr that = boost::dynamic_pointer_cast<CPValueObject>(o);
  assert(that);
  // Note: this does not change the original lattice since the base of a CPValueObject
  //       always a copy of the original lattice
  return ground->intersectUpdate(that->ground.get());
}

// Returns whether this lattice denotes the set of all possible execution prefixes.
bool
CPValueObject::isFullAO(PartEdgePtr pedge)
{ return ground->getKind()->isFullAO(pedge); }

// Returns whether this lattice denotes the empty set.
bool
CPValueObject::isEmptyAO(PartEdgePtr pedge)
{ return ground->getKind()->isEmptyAO(pedge); }


// Allocates a copy of this object and returns a pointer to it
ValueObjectPtr CPValueObject::copyAOType() const
{
  return boost::make_shared<CPValueObject>(*this);
}

// Returns true if this ValueObject corresponds to a concrete value that is statically-known
bool CPValueObject::isConcrete()
{ return ground->getKind()->isConcrete(); }

// Returns the number of concrete values in this set
int CPValueObject::concreteSetSize()
{ return ground->getKind()->concreteSetSize(); }

// Returns the type of the concrete value (if there is one)
SgType* CPValueObject::getConcreteType()
{
  assert(ground->getKind()->asConcreteKind());
  return ground->getKind()->asConcreteKind()->getVal()->get_type();
}

// Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
// the normal ROSE mechanisms to decode it
set<boost::shared_ptr<SgValueExp> > CPValueObject::getConcreteValue()
{
  assert(ground->getKind()->asConcreteKind());
  set<boost::shared_ptr<SgValueExp> > concreteVals;
  concreteVals.insert(ground->getKind()->asConcreteKind()->getVal());
  return concreteVals;
}

// Returns a key that uniquely identifies this particular AbstractObject in the 
// set hierarchy.
const AbstractionHierarchy::hierKeyPtr& CPValueObject::getHierKey() const {
  if(!isHierKeyCached) {
    ((CPValueObject*)this)->cachedHierKey = boost::make_shared<AOSHierKey>(((CPValueObject*)this)->shared_from_this());
    
    // The all object gets an empty key since it contains all the object types
    if(ground->getKind()->getKind()==CPValueKind::unknown) { }
    else {
      ((CPValueObject*)this)->cachedHierKey->add(boost::make_shared<CPValueKind::comparableKind>(ground->getKind()->getKind()));
      ground->getKind()->addHierSubKey(((CPValueObject*)this)->cachedHierKey);

      //dbg << "computed"<<endl;
    }
    ((CPValueObject*)this)->isHierKeyCached = true;
  }
  //dbg << "((CPValueObject*)this)->cachedHierKey="<<((CPValueObject*)this)->cachedHierKey<<endl;
  return cachedHierKey;
}

string
CPValueObject::str(string indent) const
{
  return ground->str(indent);
}

string
CPValueObject::strp(PartEdgePtr pedge, string indent) const
{
  return ground->str(indent);
}


// ******************************
// **** CPUninitializedKind *****
// ******************************

// Applies the given unary or binary operation to this and the given CPValueKind
// Returns:
//    - if this CPValueKind can be updated to incorporate the result of the addition,
//       return a freshly-allocated CPValueKind that holds the result.
//    - if the two objects could not be merged and therefore that must be placed after
//       this in the parent CPValueObject's list, return that.
CPValueKindPtr CPUninitializedKind::op(SgUnaryOp* op) {
  // Uninitialized denotes the empty set, so any operation applied to it results in the empty set
  return copyAOType();
}

CPValueKindPtr CPUninitializedKind::op(SgBinaryOp* op, CPValueKindPtr that) {
  // Uninitialized denotes the empty set, so any operation that involves it results in the empty set
  return copyAOType();
}

// Returns whether this and that CPValueKinds are may/must equal to each other
bool CPUninitializedKind::mayEqualAO(CPValueKindPtr that)
{
  // Uninitialized denotes the empty set, which does not overlap with any other set
  return false;
}

bool CPUninitializedKind::mustEqualAO(CPValueKindPtr that) {
  // Uninitialized denotes the empty set, which may only be equal to another empty set
  return that->getKind() == CPValueKind::uninitialized;
}

// Returns whether the two CPValueKinds denote the same set of concrete values
bool CPUninitializedKind::equalSetAO(CPValueKindPtr that) {
  // Uninitialized denotes the empty set, which may only be equal to another empty set
  return (that->getKind() == CPValueKind::uninitialized);
}

// Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given CPValueKind
bool CPUninitializedKind::subSetAO(CPValueKindPtr that) {
  // Uninitialized denotes the empty set, which is a subset of every other set
  return true;
}

// Computes the meet of this and that and returns the resulting kind
pair<bool, CPValueKindPtr> CPUninitializedKind::meetUpdateAO(CPValueKindPtr that)
{
  bool modified = that->getKind() != CPValueKind::uninitialized;
  // Uninitialized MEET * => *
  return make_pair(modified, that->copyAOType());
}

// Computes the intersection of this and that and returns the resulting kind
std::pair<bool, CPValueKindPtr> CPUninitializedKind::intersectUpdateAO(CPValueKindPtr that) {
  bool modified = false;
  // Uninitialized INTERSECT * => Uninitialized
  return make_pair(modified, copyAOType());
}

// Returns true if this ValueObject corresponds to a concrete value that is statically-known
bool CPUninitializedKind::isConcrete()
{ return false; }

// Returns the number of concrete values in this set
int CPUninitializedKind::concreteSetSize()
{ return -1; }

// Returns the type of the concrete value (if there is one)
SgType* CPUninitializedKind::getConcreteType()
{ return NULL; }

// Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
// the normal ROSE mechanisms to decode it
std::set<boost::shared_ptr<SgValueExp> > CPUninitializedKind::getConcreteValue()
{ return std::set<boost::shared_ptr<SgValueExp> >(); }

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool CPUninitializedKind::isFullAO(PartEdgePtr pedge) { return false; }
// Returns whether this AbstractObject denotes the empty set.
bool CPUninitializedKind::isEmptyAO(PartEdgePtr pedge) { return true; }

std::string CPUninitializedKind::str(std::string indent) const
{ return "[CPUninitializedKind]"; }

// ******************************
// **** CPConcreteKind *****
// ******************************

// Creates a CPConcreteKind from the given value. This function is overloaded with different argument types
// and for each type it creates a CPConcreteKind with a different SgValueExp.
CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(bool val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(char val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildCharVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(short val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildShortVal(val))); }
  
CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(int val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildIntVal(val))); }
  
CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(long val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongIntVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(long long val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongLongIntVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(unsigned char val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedCharVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(unsigned short val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedShortVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(unsigned int val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedIntVal(val))); }
  
CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(unsigned long val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedLongVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(unsigned long long val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedLongLongIntVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(wchar_t val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildWcharVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(float val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildFloatVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(double val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildDoubleVal(val))); }

CPValueKindPtr CPConcreteKind::createCPValueKindFromVal(long double val)
{ return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongDoubleVal(val))); }


// Applies the given operation functor to the expression in this ConcreteKind and returns the resulting CPKind
template<class DoOpType>
	CPValueKindPtr CPConcreteKind::doUnaryOp(DoOpType& doOp) {
	  switch(exp.get()->variantT()) {
	    case V_SgBoolValExp:             
	    case V_SgCharVal:                
	    case V_SgShortVal:                
	    case V_SgIntVal:                 
	    case V_SgLongIntVal:             
	    case V_SgLongLongIntVal:         
	    case V_SgUnsignedCharVal:        
	    case V_SgUnsignedIntVal:         
	    case V_SgUnsignedLongVal:        
	    case V_SgUnsignedLongLongIntVal: 
	    case V_SgUnsignedShortVal:       
	    case V_SgUpcMythread:            
	    case V_SgUpcThreads:             
	    case V_SgWcharVal:               
	    case V_SgEnumVal:
	      return doUnaryIntegralOp(doOp);
	    
	    case V_SgDoubleVal: case V_SgFloatVal: case V_SgLongDoubleVal: case V_SgComplexVal:
	      return doUnaryFloatOp(doOp);
	    
	    default:
	      // We've enumerated all cases so we should never get here
	      assert(0);
	  }  
	}

	// Applies the given operation functor to the expression in this ConcreteKind, whoch is assumed to be an integral type 
	// and returns the resulting CPKind
	template<class DoOpType>
	CPValueKindPtr CPConcreteKind::doUnaryIntegralOp(DoOpType& doOp) {
	  switch(exp.get()->variantT()) {
	/*    case V_SgBoolValExp:             return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp            (doOp(isSgBoolValExp            (exp.get())->get_value()))));
	    case V_SgCharVal:                return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildCharVal               (doOp(isSgCharVal               (exp.get())->get_value()))));
	    case V_SgIntVal:                 return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildIntVal                (doOp(isSgIntVal                (exp.get())->get_value()))));
	    case V_SgLongIntVal:             return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongIntVal            (doOp(isSgLongIntVal            (exp.get())->get_value()))));
	    case V_SgLongLongIntVal:         return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongLongIntVal        (doOp(isSgLongLongIntVal        (exp.get())->get_value()))));
	    case V_SgUnsignedCharVal:        return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedCharVal       (doOp(isSgUnsignedCharVal       (exp.get())->get_value()))));
	    case V_SgUnsignedIntVal:         return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedIntVal        (doOp(isSgUnsignedIntVal        (exp.get())->get_value()))));
	    case V_SgUnsignedLongVal:        return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedLongVal       (doOp(isSgUnsignedLongVal       (exp.get())->get_value()))));
	    case V_SgUnsignedLongLongIntVal: return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedLongLongIntVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value()))));
	    case V_SgUnsignedShortVal:       return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildUnsignedShortVal      (doOp(isSgUnsignedShortVal      (exp.get())->get_value()))));
	    case V_SgUpcMythread:            return boost::make_shared<CPUnknownKind>();
	    case V_SgUpcThreads:             return boost::make_shared<CPUnknownKind>();
	    case V_SgWcharVal:               return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildWcharVal              (doOp(isSgWcharVal              (exp.get())->get_value()))));*/
	    case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgBoolValExp            (exp.get())->get_value()));
	    case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgCharVal               (exp.get())->get_value()));
	    case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgShortVal               (exp.get())->get_value()));
	    case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgIntVal                (exp.get())->get_value()));
	    case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgLongIntVal            (exp.get())->get_value()));
	    case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgLongLongIntVal        (exp.get())->get_value()));
	    case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedCharVal       (exp.get())->get_value()));
	    case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedIntVal        (exp.get())->get_value()));
	    case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongVal       (exp.get())->get_value()));
	    case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value()));
	    case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedShortVal      (exp.get())->get_value()));
	    case V_SgUpcMythread:            return boost::make_shared<CPUnknownKind>();
	    case V_SgUpcThreads:             return boost::make_shared<CPUnknownKind>();
	    case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgWcharVal              (exp.get())->get_value()));

	    
	    case V_SgEnumVal:    
	      //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnusmVal               (exp.get())->get_value()-1)));
	      // NOTE: Need to write code to get the value of the Enum Val and create a new one
	      assert(0);
	    default:
	      // We've enumerated all cases so we should never get here
	      assert(0);
	  }  
	}

	// Applies the given operation functor to the expression in this ConcreteKind, which is assumed to a floating point type 
	// and returns the resulting CPKind
	template<class DoOpType>
	CPValueKindPtr CPConcreteKind::doUnaryFloatOp(DoOpType& doOp) {
	  switch(exp.get()->variantT()) {
	    case V_SgComplexVal:             return boost::make_shared<CPUnknownKind>();                        
	    /*case V_SgDoubleVal:              return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildDoubleVal             (doOp(isSgDoubleVal             (exp.get())->get_value()))));
	    case V_SgFloatVal:               return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildFloatVal              (doOp(isSgFloatVal              (exp.get())->get_value()))));
	    case V_SgLongDoubleVal:          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongDoubleVal         (doOp(isSgLongDoubleVal         (exp.get())->get_value()))));*/
	    case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgDoubleVal             (exp.get())->get_value()));
	    case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgFloatVal              (exp.get())->get_value()));
	    case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgLongDoubleVal         (exp.get())->get_value()));
	    
	    default:
	      // We've enumerated all cases so we should never get here
	      assert(0);
	  }  
}

// Applies the given unary or binary operation to this and the given CPValueKind
// Returns:
//    - if this CPValueKind can be updated to incorporate the result of the addition,
//       return a freshly-allocated CPValueKind that holds the result.
//    - if the two objects could not be merged and therefore that must be placed after
//       this in the parent CPValueObject's list, return that.
CPValueKindPtr CPConcreteKind::op(SgUnaryOp* op) {
  if(isSgBitComplementOp(op)) {
    if(isSgBoolValExp(exp.get())) 
      switch(exp.get()->variantT()) {
      case V_SgBoolValExp: case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
      case V_SgFloatVal: case V_SgDoubleVal: case V_SgLongDoubleVal:
        return boost::make_shared<CPUnknownKind>();
      default: return doUnaryIntegralOp(~ boost::lambda::_1);
    }
  } else if(isSgCastExp(op)) {
    SgType* t=isSgCastExp(op)->get_type();
    // Unwrap any typedefs that alias the base type of t
    while(SgTypedefType* tt=isSgTypedefType(t))
      t = tt->get_base_type();

    switch(t->variantT()) {
      case V_SgTypeBool:               return doUnaryOp(boost::lambda::ll_static_cast<bool>              (boost::lambda::_1)); 
      case V_SgTypeComplex:            assert(0);             
      case V_SgTypeFloat:              return doUnaryOp(boost::lambda::ll_static_cast<float>             (boost::lambda::_1));
      case V_SgTypeDouble:             return doUnaryOp(boost::lambda::ll_static_cast<double>            (boost::lambda::_1));
      case V_SgTypeLongDouble:         return doUnaryOp(boost::lambda::ll_static_cast<long double>       (boost::lambda::_1));
      case V_SgTypeChar:               return doUnaryOp(boost::lambda::ll_static_cast<char>              (boost::lambda::_1)); 
      case V_SgTypeInt:                return doUnaryOp(boost::lambda::ll_static_cast<int>               (boost::lambda::_1));
      case V_SgTypeLong:               return doUnaryOp(boost::lambda::ll_static_cast<long>              (boost::lambda::_1));
      case V_SgTypeLongLong:           return doUnaryOp(boost::lambda::ll_static_cast<long long>         (boost::lambda::_1));
      case V_SgTypeShort:              return doUnaryOp(boost::lambda::ll_static_cast<short>             (boost::lambda::_1));
      case V_SgTypeUnsignedChar:       return doUnaryOp(boost::lambda::ll_static_cast<unsigned char>     (boost::lambda::_1));
      case V_SgTypeUnsignedInt:        return doUnaryOp(boost::lambda::ll_static_cast<unsigned int>      (boost::lambda::_1));
      case V_SgTypeUnsignedLong:       return doUnaryOp(boost::lambda::ll_static_cast<unsigned long>     (boost::lambda::_1));
      case V_SgTypeUnsignedLongLong:   return doUnaryOp(boost::lambda::ll_static_cast<unsigned long long>(boost::lambda::_1));
      case V_SgTypeUnsignedShort:      return doUnaryOp(boost::lambda::ll_static_cast<unsigned short>    (boost::lambda::_1));
      case V_SgTypeSignedChar:         return doUnaryOp(boost::lambda::ll_static_cast<char>              (boost::lambda::_1));
      case V_SgTypeSignedInt:          return doUnaryOp(boost::lambda::ll_static_cast<int>               (boost::lambda::_1));
      case V_SgTypeSignedLong:         return doUnaryOp(boost::lambda::ll_static_cast<long>              (boost::lambda::_1));      
      case V_SgTypeSignedLongLong:     return doUnaryOp(boost::lambda::ll_static_cast<long long>         (boost::lambda::_1));
      case V_SgTypeSignedShort:        return doUnaryOp(boost::lambda::ll_static_cast<short>             (boost::lambda::_1));
      case V_SgTypeWchar:              return doUnaryOp(boost::lambda::ll_static_cast<wchar_t>           (boost::lambda::_1));
        
      case V_SgArrayType: case V_SgFunctionType: case V_SgJavaWildcardType: case V_SgModifierType: 
      case V_SgNamedType: case V_SgPointerType: case V_SgQualifiedNameType: case V_SgReferenceType: 
      case V_SgTemplateType: case V_SgTypeCAFTeam: case V_SgTypeCrayPointer: case V_SgTypeDefault: 
      case V_SgTypeEllipse: case V_SgTypeGlobalVoid: case V_SgTypeImaginary: case V_SgTypeLabel: 
      case V_SgTypeVoid:
      default:
        cerr << "ERROR: unknown cast expression type. unwrapped type="<<SgNode2Str(t)<<" full type="<<SgNode2Str(isSgCastExp(op)->get_type())<<", op="<<SgNode2Str(op)<<endl;
        assert(0);
    }
  } else if(isSgConjugateOp(op)) {
    // TO DO
    /*if(isSgComplexVal(exp.get())) 
      return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildComplexVal(
               isSgComplexVal(exp.get())->get_real_value(),
              -isSgComplexVal(exp.get())->get_imaginary_value())));
    else
      return boost::make_shared<CPUnknownKind>();*/
  } else if(isSgExpressionRoot(op)) {
    // What is this?
  } else if(isSgImagPartOp(op)) {
    if(isSgComplexVal(exp.get())) 
      return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(isSgComplexVal(exp.get())->get_imaginary_value()));
    else
      return boost::make_shared<CPUnknownKind>();
  } else if(isSgRealPartOp(op)) {
    if(isSgComplexVal(exp.get())) 
      return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(isSgComplexVal(exp.get())->get_real_value()));
    else
      return boost::make_shared<CPUnknownKind>();
  } else if(isSgMinusMinusOp(op)) {
    switch(exp.get()->variantT()) {
      case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: return boost::make_shared<CPUnknownKind>();
      default: return doUnaryOp(boost::lambda::_1 - 1);
    }
  } else if(isSgPlusPlusOp(op)) {
    switch(exp.get()->variantT()) {
      case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: return boost::make_shared<CPUnknownKind>();
      default: return doUnaryOp(boost::lambda::_1 + 1);
    }
  } else if(isSgMinusOp(op)) {
    switch(exp.get()->variantT()) {
      case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: return boost::make_shared<CPUnknownKind>();
      default: return doUnaryOp(0 - boost::lambda::_1);
    }
  } else if(isSgNotOp(op)) {
    switch(exp.get()->variantT()) {
      case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: return boost::make_shared<CPUnknownKind>();
      default: return doUnaryOp(!boost::lambda::_1);
    }
     
  } else if(isSgThrowOp(op)) {
    return boost::make_shared<CPUnknownKind>();
    // TODO: control flow effects
  
  } else if(isSgUnaryAddOp(op)) {
    // What is this?
    
  } else if(isSgUserDefinedUnaryOp(op)) {
    // What is this?
  
  } else if(isSgAddressOfOp(op)) {
    // TODO
    return boost::make_shared<CPUnknownKind>();
    
    
  } else if(isSgPointerDerefExp(op)) {
    // This should be handled inside CPMemLocObjects
    assert(0);
  }
  
  // We've enumerated all cases so we should never get here
  cerr << "ERROR: no support for "<<SgNode2Str(op)<<"!";
  assert(0);
}

// Returns whether a given AST node that represents a constant is interpretable as a signed integer and
// sets val to be the numeric value of that integer
bool IsSignedConstInt(SgExpression* exp, long long &val)
{
  switch(exp->variantT())
  {
    case V_SgBoolValExp:             val = isSgBoolValExp(exp)->get_value();             return true;
    case V_SgCharVal:                val = isSgCharVal(exp)->get_value();                return true;
    case V_SgWcharVal:               val = isSgWcharVal(exp)->get_valueUL();             return true;
    case V_SgIntVal:                 val = isSgIntVal(exp)->get_value();                 return true;
    case V_SgLongIntVal:             val = isSgLongIntVal(exp)->get_value();             return true;
    case V_SgLongLongIntVal:         val = isSgLongLongIntVal(exp)->get_value();         return true;
    case V_SgShortVal:               val = isSgShortVal(exp)->get_value();               return true;
    default: return false;
  }
}

// Returns whether a given AST node that represents a constant is interpretable as an unsigned integer and
// sets val to be the numeric value of that integer
bool IsUnsignedConstInt(SgExpression* exp, unsigned long long &val)
{
  switch(exp->variantT())
  {
    case V_SgBoolValExp:             val = isSgBoolValExp(exp)->get_value();             return true;
    case V_SgUnsignedCharVal:        val = isSgUnsignedCharVal(exp)->get_value();        return true;
    case V_SgUnsignedIntVal:         val = isSgUnsignedIntVal(exp)->get_value();         return true;
    case V_SgUnsignedLongVal:        val = isSgUnsignedLongVal(exp)->get_value();        return true;
    case V_SgUnsignedLongLongIntVal: val = isSgUnsignedLongLongIntVal(exp)->get_value(); return true;
    case V_SgUnsignedShortVal:       val = isSgUnsignedShortVal(exp)->get_value();       return true;
    default: return false;
  }
}

// Returns whether a given AST node that represents a constant is interpretable as a float and
// sets val to be the numeric value of that float
bool IsConstFloat(SgExpression* exp, double &val)
{
  switch(exp->variantT())
  {
    case V_SgBoolValExp:             val = isSgBoolValExp(exp)->get_value();             return true;
    case V_SgCharVal:                val = isSgCharVal(exp)->get_value();                return true;
    case V_SgWcharVal:               val = isSgWcharVal(exp)->get_valueUL();             return true;
    case V_SgIntVal:                 val = isSgIntVal(exp)->get_value();                 return true;
    case V_SgLongIntVal:             val = isSgLongIntVal(exp)->get_value();             return true;
    case V_SgLongLongIntVal:         val = isSgLongLongIntVal(exp)->get_value();         return true;
    case V_SgShortVal:               val = isSgShortVal(exp)->get_value();               return true;
    case V_SgUnsignedIntVal:         val = isSgUnsignedIntVal(exp)->get_value();         return true;
    case V_SgUnsignedLongVal:        val = isSgUnsignedLongVal(exp)->get_value();        return true;
    case V_SgUnsignedLongLongIntVal: val = isSgUnsignedLongLongIntVal(exp)->get_value(); return true;
    case V_SgUnsignedShortVal:       val = isSgUnsignedShortVal(exp)->get_value();       return true;
    case V_SgDoubleVal:              val = isSgDoubleVal(exp)->get_value();              return true;
    case V_SgFloatVal:               val = isSgFloatVal(exp)->get_value();               return true;
    default: return false;
  }
}

/*
template<class DoOpType, class DoOpRetType>
DoOpRetType CPConcreteKind::bindDoOpArgs1(DoOpType doOp) {
  switch(exp.get()->variantT()) {
    case V_SgBoolValExp:             return boost::bind(doOp, isSgBoolValExp            (exp.get())->get_value(), _1);
    case V_SgCharVal:                return boost::bind(doOp, isSgCharVal               (exp.get())->get_value(), _1);
    case V_SgIntVal:                 return boost::bind(doOp, isSgIntVal                (exp.get())->get_value(), _1);
    case V_SgLongIntVal:             return boost::bind(doOp, isSgLongIntVal            (exp.get())->get_value(), _1);
    case V_SgLongLongIntVal:         return boost::bind(doOp, isSgLongLongIntVal        (exp.get())->get_value(), _1);
    case V_SgUnsignedCharVal:        return boost::bind(doOp, isSgUnsignedCharVal       (exp.get())->get_value(), _1);
    case V_SgUnsignedIntVal:         return boost::bind(doOp, isSgUnsignedIntVal        (exp.get())->get_value(), _1);
    case V_SgUnsignedLongVal:        return boost::bind(doOp, isSgUnsignedLongVal       (exp.get())->get_value(), _1);
    case V_SgUnsignedLongLongIntVal: return boost::bind(doOp, isSgUnsignedLongLongIntVal(exp.get())->get_value(), _1);
    case V_SgUnsignedShortVal:       return boost::bind(doOp, isSgUnsignedShortVal      (exp.get())->get_value(), _1);
    case V_SgUpcMythread:  case V_SgUpcThreads: 
      assert(0);
    
    case V_SgWcharVal:               return boost::bind(doOp, isSgWcharVal              (exp.get())->get_value(), _1);
    case V_SgEnumVal:
      assert(0);
    case V_SgDoubleVal:              return boost::bind(doOp, isSgDoubleVal             (exp.get())->get_value(), _1);
    case V_SgFloatVal:               return boost::bind(doOp, isSgFloatVal              (exp.get())->get_value(), _1);
    case V_SgLongDoubleVal:          return boost::bind(doOp, isSgLongDoubleVal         (exp.get())->get_value(), _1);
    case V_SgComplexVal:             
      assert(0);
    
    default:
      // We've enumerated all cases so we should never get here
      assert(0);
  }
}

template<class DoOpType, class DoOpRetType>
DoOpRetType CPConcreteKind::bindDoOpArgs2(DoOpType doOp) {
  switch(exp.get()->variantT()) {
    case V_SgBoolValExp:             return boost::bind(doOp, isSgBoolValExp            (exp.get())->get_value());
    case V_SgCharVal:                return boost::bind(doOp, isSgCharVal               (exp.get())->get_value());
    case V_SgIntVal:                 return boost::bind(doOp, isSgIntVal                (exp.get())->get_value());
    case V_SgLongIntVal:             return boost::bind(doOp, isSgLongIntVal            (exp.get())->get_value());
    case V_SgLongLongIntVal:         return boost::bind(doOp, isSgLongLongIntVal        (exp.get())->get_value());
    case V_SgUnsignedCharVal:        return boost::bind(doOp, isSgUnsignedCharVal       (exp.get())->get_value());
    case V_SgUnsignedIntVal:         return boost::bind(doOp, isSgUnsignedIntVal        (exp.get())->get_value());
    case V_SgUnsignedLongVal:        return boost::bind(doOp, isSgUnsignedLongVal       (exp.get())->get_value());
    case V_SgUnsignedLongLongIntVal: return boost::bind(doOp, isSgUnsignedLongLongIntVal(exp.get())->get_value());
    case V_SgUnsignedShortVal:       return boost::bind(doOp, isSgUnsignedShortVal      (exp.get())->get_value());
    case V_SgUpcMythread:  case V_SgUpcThreads: 
      assert(0);
    
    case V_SgWcharVal:               return boost::bind(doOp, isSgWcharVal              (exp.get())->get_value());
    case V_SgEnumVal:                
      assert(0);
    case V_SgDoubleVal:              return boost::bind(doOp, isSgDoubleVal             (exp.get())->get_value());
    case V_SgFloatVal:               return boost::bind(doOp, isSgFloatVal              (exp.get())->get_value());
    case V_SgLongDoubleVal:          return boost::bind(doOp, isSgLongDoubleVal         (exp.get())->get_value());
    case V_SgComplexVal:             
      assert(0);
    
    default:
      // We've enumerated all cases so we should never get here
      assert(0);
  }
}*/

// Returns whether v denotes an integral (as opposed to floating point) value
bool isIntegralVal(SgValueExp* v) {
  switch(v->variantT()) {
    case V_SgBoolValExp:             
    case V_SgCharVal:                
    case V_SgShortVal:               
    case V_SgIntVal:                 
    case V_SgLongIntVal:             
    case V_SgLongLongIntVal:         
    case V_SgUnsignedCharVal:        
    case V_SgUnsignedShortVal:       
    case V_SgUnsignedIntVal:         
    case V_SgUnsignedLongVal:        
    case V_SgUnsignedLongLongIntVal: 
    case V_SgWcharVal:               
    case V_SgUpcMythread: case V_SgUpcThreads: 
    case V_SgEnumVal:    
      return true;

    case V_SgFloatVal:               
    case V_SgDoubleVal:              
    case V_SgLongDoubleVal:          
    case V_SgComplexVal: 
      return false;

    default:
      // We've enumerated all cases so we should never get here
      assert(0);
  }
}

// Returns whether v denotes an floating point (as opposed to integral) value
bool isFloatVal(SgValueExp* v)
{ return !isIntegralVal(v); }

//! Calculate the number of dimensions of an array type
size_t getArrayDimCount(SgArrayType* t, SgPntrArrRefExp* ref)
{
  ROSE_ASSERT(t);
  SgExpression * indexExp =  t->get_index();

//strip off THREADS for UPC array with a dimension like dim*THREADS
  if (isUpcArrayWithThreads(t))
  {
    SgMultiplyOp* multiply = isSgMultiplyOp(indexExp);
    ROSE_ASSERT(multiply);
    indexExp = multiply->get_lhs_operand();
  }

  /*cout << "t="<<SgNode2Str(t)<<endl;
  cout << "indexExp="<<(indexExp? SgNode2Str(indexExp): "NULL")<<endl;*/

  // If the dimension is not specified in the type, (e.g. array[])
  if (indexExp == NULL || isSgNullExpression(indexExp)) {
    // Look at the array's declaration to see if its size was specified via an SgAggregateInitializer
    if(SgVarRefExp* vref=isSgVarRefExp(ref->get_lhs_operand())) {
      if(SgAggregateInitializer* init = isSgAggregateInitializer(vref->get_symbol()->get_declaration()->get_initializer())) {
        return init->get_initializers()->get_expressions().size();
      } else {
        cerr << "ERROR: cannot count the number of elements in array "<<SgNode2Str(ref->get_lhs_operand())<<" since SgAggregateInitializer could not be found!"<<endl;
        assert(0);
      }
    } else {
      cerr << "ERROR: cannot count the number of elements in array reference "<<SgNode2Str(ref->get_lhs_operand())<<" since it is not a SgVarRefExp!"<<endl;
      assert(0);
    }
  } else {
    //Take advantage of the fact that the value expression is always SgUnsignedLongVal in AST
    SgUnsignedLongVal * valExp = isSgUnsignedLongVal(indexExp);
    SgIntVal * valExpInt = isSgIntVal(indexExp);
    ROSE_ASSERT(valExp || valExpInt); // TODO: return -1 is better ?
    if (valExp)
      return valExp->get_value();
    else
      return valExpInt->get_value();
  }

  assert(0);
}

//! Calculate the number of elements of an array type
size_t getArrayElementCount_GB(SgArrayType* t, SgPntrArrRefExp* ref)
{
  ROSE_ASSERT(t);

  size_t result = getArrayDimCount(t, ref);

  // consider multi dimensional case
  SgArrayType* arraybase = isSgArrayType(t->get_base_type());
  if (arraybase)
    result = result * getArrayElementCount_GB(arraybase, isSgPntrArrRefExp(ref->get_rhs_operand()));

  return result;
} // getArrayElementCount()


// Returns the offset of the given SgPntrArrRefExp relative to the starting point of its parent expression,
// which may be a SgVarRefExp, SgDotExp, SgPntrArrRefExp or other expressions
long long getPntrArrRefOffset(SgPntrArrRefExp* ref, CPConcreteKindPtr that) {
  //scope s(txt()<<"getPntrArrRefOffset("<<SgNode2Str(ref));
  /*cout << "----------------------------------"<<endl;
  cout << "getPntrArrRefOffset("<<SgNode2Str(ref)<<endl;
  cout << "rhs="<<SgNode2Str(ref->get_rhs_operand())<<endl;
  cout << "lhs="<<SgNode2Str(ref->get_lhs_operand())<<endl;
  if(isSgVarRefExp(ref->get_lhs_operand())) {
    cout << "decl="<<SgNode2Str(isSgVarRefExp(ref->get_lhs_operand())->get_symbol()->get_declaration())<<endl;
    cout << "initializer="<<SgNode2Str(isSgVarRefExp(ref->get_lhs_operand())->get_symbol()->get_declaration()->get_initializer())<<endl;
  }
  cout << "lhs type="<<SgNode2Str(ref->get_lhs_operand()->get_type())<<endl;*/

  unsigned long long subArraySize;
  if(SgArrayType* arrType = isSgArrayType(ref->get_lhs_operand()->get_type())) {
    /*cout << "   index="<<SgNode2Str(arrType->get_index())<<endl;
    cout << "   dim="<<SgNode2Str(arrType->get_dim_info())<<"="<<arrType->get_dim_info()<<endl;
    cout << "   rank="<<arrType->get_rank()<<endl;
    cout << "   eltCount="<<getArrayElementCount_GB(arrType, ref)<<endl;
    cout << " that="<<that->str()<<endl;*/

    // Compute the number of entries in the array of the current sub-level in the SgArrayType by
    // dividing the number of total entries in the current SgArrayType by the number of sub-arrays
    // in the next dimension.
    /*assert(isSgValueExp(arrType->get_index()));
    long long sTypeIdx;
    unsigned long long usTypeIdx;
    if(IsSignedConstInt(isSgValueExp(arrType->get_index()), sTypeIdx))
      subArraySize = getArrayElementCount_GB(arrType, ref) / sTypeIdx;
    else if(IsUnsignedConstInt(isSgValueExp(arrType->get_index()), usTypeIdx))
      subArraySize = getArrayElementCount_GB(arrType, ref) / usTypeIdx;
    else
      // The index in the array's type must be an integer of some sort
      assert(0);*/
    subArraySize = getArrayElementCount_GB(arrType, ref) / getArrayDimCount(arrType, ref);
  } else if(SgPointerType* paType = isSgPointerType(ref->get_lhs_operand()->get_type())) {
    subArraySize = 1;
  } else
    assert(0);

  // Given the number of entries in the next level's sub-array, compute the offset of
  // the next array index (that value), which is a multiple of the next level's sub-array
  long long sRefIdx;
  unsigned long long usRefIdx;
  if(IsSignedConstInt(that->getVal().get(), sRefIdx)) 
    return subArraySize * sRefIdx;
  else if(IsUnsignedConstInt(that->getVal().get(), usRefIdx)) 
    return subArraySize * usRefIdx;
  else
    // The index in the array's reference expression must be an integer of some sort
    assert(0);
}

// Applies the given binary operation functor to the expression in this and that ConcreteKinds, assuming they're
// both integral types and returns the resulting CPKind
template<class DoOpType>
CPValueKindPtr CPConcreteKind::doBinaryOpIntInt(DoOpType& doOp, CPConcreteKindPtr that) {
  long long sVal1, sVal2;
  unsigned long long usVal1, usVal2;
  if(IsSignedConstInt(exp.get(), sVal1)) {
    if(IsSignedConstInt(that->getVal().get(), sVal2))
      return createCPValueKindFromVal(doOp(sVal1, sVal2));
    else if(IsUnsignedConstInt(that->getVal().get(), usVal2))
      return createCPValueKindFromVal(doOp(sVal1, (long long)usVal2));
  } else if(IsUnsignedConstInt(exp.get(), usVal1)) {
    if(IsSignedConstInt(that->getVal().get(), sVal2))
      return createCPValueKindFromVal(doOp((long long)usVal1, sVal2));
    else if(IsUnsignedConstInt(that->getVal().get(), usVal2))
      return createCPValueKindFromVal(doOp(usVal1, usVal2));
  }
  assert(0);
  /*switch(exp.get()->variantT()) {
    case V_SgBoolValExp:              
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
    
    case V_SgCharVal:
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }

    case V_SgShortVal:
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgIntVal:
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }

    case V_SgLongIntVal:             
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgLongLongIntVal:              
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedCharVal:        
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedShortVal:                    
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedIntVal:                            
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedLongVal:                     
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedLongLongIntVal:              
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgWcharVal:            
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUpcMythread: case V_SgUpcThreads:
      return boost::make_shared<CPUnknownKind>();
    
    case V_SgEnumVal:    
      //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
      // NOTE: Need to write code to get the value of the Enum Val and create a new one
      assert(0);
    default:
      // We've enumerated all cases so we should never get here
      assert(0);
  }  */
}

// Applies the given binary operation functor to the expression in this and that ConcreteKinds, assuming at least one
// is not an integral type and returns the resulting CPKind
template<class DoOpType>
CPValueKindPtr CPConcreteKind::doBinaryOp(DoOpType& doOp, CPConcreteKindPtr that) {
  long long sVal1, sVal2;
  unsigned long long usVal1, usVal2;
  
  if(IsSignedConstInt(exp.get(), sVal1)) {
    if(IsSignedConstInt(that->getVal().get(), sVal2))
      return createCPValueKindFromVal(doOp(sVal1, sVal2));
    else if(IsUnsignedConstInt(that->getVal().get(), usVal2))
      return createCPValueKindFromVal(doOp(sVal1, (long long)usVal2));
  } else if(IsUnsignedConstInt(exp.get(), usVal1)) {
    if(IsSignedConstInt(that->getVal().get(), sVal2))
      return createCPValueKindFromVal(doOp((long long)usVal1, sVal2));
    else if(IsUnsignedConstInt(that->getVal().get(), usVal2))
      return createCPValueKindFromVal(doOp(usVal1, usVal2));
  }
  // If the values are not integers, lets see if they're floating point
  double fVal1, fVal2;
  if(IsConstFloat(exp.get(), fVal1) && IsConstFloat(that->getVal().get(), fVal2))
    return createCPValueKindFromVal(doOp(fVal1,fVal2));

  assert(0);
  /*
  switch(exp.get()->variantT()) {
    case V_SgBoolValExp:              
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgBoolValExp(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
    
    case V_SgCharVal:
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgCharVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }

    case V_SgShortVal:
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgShortVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgIntVal:
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgIntVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }

    case V_SgLongIntVal:             
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgLongIntVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgLongLongIntVal:              
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgLongLongIntVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedCharVal:        
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgUnsignedCharVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedShortVal:                    
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgUnsignedShortVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedIntVal:                            
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgUnsignedIntVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedLongVal:                     
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgUnsignedLongVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgUnsignedLongLongIntVal:              
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgUnsignedLongLongIntVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgWcharVal:            
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgWcharVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
    
    case V_SgFloatVal:            
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgFloatVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
    
    case V_SgDoubleVal:            
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgDoubleVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
    
    case V_SgLongDoubleVal:            
      switch(that->exp.get()->variantT()) {
        case V_SgBoolValExp:             return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgBoolValExp            (that->exp.get())->get_value()));
        case V_SgCharVal:                return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgCharVal               (that->exp.get())->get_value()));
        case V_SgShortVal:               return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgShortVal              (that->exp.get())->get_value()));
        case V_SgIntVal:                 return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgIntVal                (that->exp.get())->get_value()));
        case V_SgLongIntVal:             return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgLongIntVal            (that->exp.get())->get_value()));
        case V_SgLongLongIntVal:         return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgLongLongIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedCharVal:        return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgUnsignedCharVal       (that->exp.get())->get_value()));
        case V_SgUnsignedShortVal:       return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgUnsignedShortVal      (that->exp.get())->get_value()));
        case V_SgUnsignedIntVal:         return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgUnsignedIntVal        (that->exp.get())->get_value()));
        case V_SgUnsignedLongVal:        return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgUnsignedLongVal       (that->exp.get())->get_value()));
        case V_SgUnsignedLongLongIntVal: return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgUnsignedLongLongIntVal(that->exp.get())->get_value()));
        case V_SgWcharVal:               return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgWcharVal              (that->exp.get())->get_value()));
        case V_SgFloatVal:               return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgFloatVal              (that->exp.get())->get_value()));
        case V_SgDoubleVal:              return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgDoubleVal             (that->exp.get())->get_value()));
        case V_SgLongDoubleVal:          return createCPValueKindFromVal(doOp(isSgLongDoubleVal(exp.get())->get_value(), isSgLongDoubleVal         (that->exp.get())->get_value()));       
        case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads: 
          return boost::make_shared<CPUnknownKind>();
        case V_SgEnumVal:    
          //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
          // NOTE: Need to write code to get the value of the Enum Val and create a new one
          assert(0);
        default:
          // We've enumerated all cases so we should never get here
          assert(0);
      }
      
    case V_SgComplexVal: case V_SgUpcMythread: case V_SgUpcThreads:
      return boost::make_shared<CPUnknownKind>();
    
    case V_SgEnumVal:    
      //return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildEnumVal               (isSgEnumVal               (exp.get())->get_value()-1)));
      // NOTE: Need to write code to get the value of the Enum Val and create a new one
      assert(0);
    default:
      // We've enumerated all cases so we should never get here
      assert(0);
  }  */
}


// Applies the given unary or binary operation to this and the given CPValueKind
// Returns:
//    - if this CPValueKind can be updated to incorporate the result of the addition, 
//       return a freshly-allocated CPValueKind that holds the result.
//    - if the two objects could not be merged and therefore that must be placed after 
//       this in the parent CPValueObject's list, return that.
CPValueKindPtr CPConcreteKind::op(SgBinaryOp* op, CPValueKindPtr that) {
  assert(that);
  
  /*scope("CPConcreteKind::op(SgBinaryOp*)", scope::medium, attrGE("constantPropagationAnalysisDebugLevel", 2));
  if(constantPropagationAnalysisDebugLevel()>=2) {
    dbg << "this="<<str()<<endl;
    dbg << "that="<<that->str()<<endl;
    dbg << "(that->getKind() == CPValueKind::uninitialized)="<<(that->getKind() == CPValueKind::uninitialized)<<endl;
    dbg << "(that->getKind() == CPValueKind::unknown)="<<(that->getKind() == CPValueKind::unknown)<<endl;
  }*/
  
  if(that->getKind() == CPValueKind::uninitialized) {
    if(isSgAndOp(op)) {
      // True && uninitialized => uninitialized
      if(isConstantTrue(getVal().get())) return that;
      // False && uninitialized => False
      else       return boost::make_shared<CPUninitializedKind>();
    
    } else if(isSgOrOp(op)) {
      // True || uninitialized => True
      if(isConstantTrue(getVal().get())) return copyAOType();
      // False | uninitialized => uninitialized
      else       return boost::make_shared<CPUninitializedKind>();
    
    // * op uninitialized => *
    } else
      return copyAOType();
  }
  
  // * op unknown => unknown
  if(that->getKind() == CPValueKind::unknown)
    return boost::make_shared<CPUnknownKind>();
  
  if(that->getKind() == CPValueKind::concrete) {
    CPConcreteKindPtr thatConcrete = that->asConcreteKind();
 

    // ----- Arithmetic -----
    if(isSgAddOp(op) || isSgPlusAssignOp(op)) {
      long long sVal1, sVal2;
      unsigned long long usVal1, usVal2;
      if(IsSignedConstInt(exp.get(), sVal1)) {
        if(IsSignedConstInt(thatConcrete->getVal().get(), sVal2))
          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongLongIntVal(sVal1+sVal2)));
        else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal2))
          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongLongIntVal(sVal1+usVal2)));
      } else if(IsUnsignedConstInt(exp.get(), usVal1)) {
        if(IsSignedConstInt(thatConcrete->getVal().get(), sVal2))
          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongLongIntVal(usVal1+sVal2)));
        else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal2))
          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongLongIntVal(usVal1+usVal2)));
      }
      // If the values are not integers, lets see if they're floating point
      double fVal1, fVal2;
      if(IsConstFloat(exp.get(), fVal1) && IsConstFloat(thatConcrete->getVal().get(), fVal2))
        return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildDoubleVal(fVal1+fVal2)));
      // If not floating point, give up
      return boost::make_shared<CPUnknownKind>();
    
    } else if(isSgSubtractOp(op) || isSgMinusAssignOp(op)) {
      //return createCPValueKindFromVal(thatConcrete->bindDoOpArgs2(bindDoOpArgs1(boost::lambda::_1 - boost::lambda::_2)));
      return doBinaryOp(boost::lambda::_1 - boost::lambda::_2, thatConcrete);
    } else if(isSgMultiplyOp(op) || isSgMultAssignOp(op)) {
      return doBinaryOp(boost::lambda::_1 * boost::lambda::_2, thatConcrete);
    } else if(isSgDivideOp(op) || isSgDivAssignOp(op)) {
      return doBinaryOp(boost::lambda::_1 / boost::lambda::_2, thatConcrete);
    } else if(isSgIntegerDivideOp(op) || isSgIntegerDivideAssignOp(op)) {
      // TODO
      
    } else if(isSgModOp(op) || isSgModAssignOp(op)) {
      assert(isIntegralVal(exp.get()) && isIntegralVal(thatConcrete->exp.get()));
      return doBinaryOpIntInt(boost::lambda::_1 % boost::lambda::_2, thatConcrete);
    } else if(isSgExponentiationOp(op) || isSgExponentiationAssignOp(op)) {
      // TO DO
    // ----- Logical -----
    } else if(isSgAndOp(op) || isSgAndAssignOp(op)) {
      return doBinaryOp(boost::lambda::_1 && boost::lambda::_2, thatConcrete);
    } else if(isSgOrOp(op) || isSgIorAssignOp(op)) {
      return doBinaryOp(boost::lambda::_1 || boost::lambda::_2, thatConcrete);
    // ----- Bitwise -----
    } else if(isSgBitAndOp(op)) {
      assert(isIntegralVal(exp.get()) && isIntegralVal(thatConcrete->exp.get()));
      return doBinaryOpIntInt(boost::lambda::_1 & boost::lambda::_2, thatConcrete);
    } else if(isSgBitOrOp(op)) {
      assert(isIntegralVal(exp.get()) && isIntegralVal(thatConcrete->exp.get()));
      return doBinaryOpIntInt(boost::lambda::_1 | boost::lambda::_2, thatConcrete);
    } else if(isSgBitXorOp(op) || isSgXorAssignOp(op)) {
      assert(isIntegralVal(exp.get()) && isIntegralVal(thatConcrete->exp.get()));
      return doBinaryOpIntInt(boost::lambda::_1 ^ boost::lambda::_2, thatConcrete);
    } else if(isSgLshiftOp(op) || isSgLshiftAssignOp(op)) {
      assert(isIntegralVal(exp.get()) && isIntegralVal(thatConcrete->exp.get()));
      return doBinaryOpIntInt(boost::lambda::_1 << boost::lambda::_2, thatConcrete);
    } else if(isSgRshiftOp(op) || isSgRshiftAssignOp(op)) {
      assert(isIntegralVal(exp.get()) && isIntegralVal(thatConcrete->exp.get()));
      return doBinaryOpIntInt(boost::lambda::_1 >> boost::lambda::_2, thatConcrete);
    } else if(isSgJavaUnsignedRshiftOp(op) || isSgJavaUnsignedRshiftAssignOp(op)) {
      // TODO (Java))
    
    // ----- Comparison -----
    } else if(isSgEqualityOp(op)) {
      long long sVal1, sVal2;
      unsigned long long usVal1, usVal2;
      if(IsSignedConstInt(exp.get(), sVal1)) {
        if(IsSignedConstInt(thatConcrete->getVal().get(), sVal2))
          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(sVal1==sVal2)));
        else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal2))
          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(sVal1==(long long)usVal2)));
      } else if(IsUnsignedConstInt(exp.get(), usVal1)) {
        if(IsSignedConstInt(thatConcrete->getVal().get(), sVal2))
          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp((long long)usVal1==sVal2)));
        else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal2))
          return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(usVal1==usVal2)));
      }
      // If the values are not integers, lets see if they're floating point
      double fVal1, fVal2;
      if(IsConstFloat(exp.get(), fVal1) && IsConstFloat(thatConcrete->getVal().get(), fVal2))
        return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(fVal1==fVal2)));
      // If not floating point, give up
      return boost::make_shared<CPUnknownKind>();
    } else if(isSgNotEqualOp(op)) {
      return doBinaryOp(boost::lambda::_1 != boost::lambda::_2, thatConcrete);
    } else if(isSgLessOrEqualOp(op)) {
      return doBinaryOp(boost::lambda::_1 <= boost::lambda::_2, thatConcrete);
    } else if(isSgLessThanOp(op)) {
      return doBinaryOp(boost::lambda::_1 <  boost::lambda::_2, thatConcrete);
    } else if(isSgGreaterOrEqualOp(op)) {
      return doBinaryOp(boost::lambda::_1 >= boost::lambda::_2, thatConcrete);
    } else if(isSgGreaterThanOp(op)) {
      return doBinaryOp(boost::lambda::_1 >  boost::lambda::_2, thatConcrete);
    } else if(isSgIsOp(op)) {
      // TODO
    
    } else if(isSgIsNotOp(op)) {
      // TODO
      
    // ----- Memory References -----
    } else if(isSgDotExp(op)) {
      // This should be handled inside CPMemLocObjects
      assert(0);
    } else if(isSgArrowExp(op)) {
      // This should be handled inside CPMemLocObjects
      assert(0);
    } else if(isSgPntrArrRefExp(op)) {
      scope s("SgPntrArrRefExp");
      assert(thatConcrete);
      // Get the offset of the SgPntrArrRefExp relative to the starting point of its parent expression
      long long nextOffset = getPntrArrRefOffset(isSgPntrArrRefExp(op), thatConcrete);
      return doBinaryOp(boost::lambda::_1 + boost::lambda::_2, 
                        boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildLongLongIntVal(nextOffset))));
      
    } else if(isSgDotStarOp(op)) {
      // This should be handled inside CPMemLocObjects
      assert(0);
    } else if(isSgPointerAssignOp(op)) {
      // TODO (Fortran)
      
    // ----- Miscelaneous ----
    } else if(isSgAssignOp(op)) {
      // This should be handled inside the ConstantPropagationAnalysis transfer function
      assert(0);
    } else if(isSgCommaOpExp(op)) {
      // This should be handled inside the control flow functionality
      assert(0);
    
    } else if(isSgConcatenationOp(op)) {
      // TODO
    } else if(isSgMembershipOp(op)) {
      // TODO
    } else if(isSgNonMembershipOp(op)) {
      // TODO
    } else if(isSgUserDefinedBinaryOp(op)) {
      // TODO
    }
  } else if(that->getKind() == CPValueKind::offsetList) {
    CPConcreteKindPtr thatOffset = that->asConcreteKind();
    
    // Implement the operations that are defined for pointers by having the offset list kind deal with them
    if(isSgAddOp(op) || isSgSubtractOp(op)) return thatOffset->op(op, shared_from_this());
    // Other options are undefined. 
    // Technically this means that we can return an uninitialized kind but to be safe we'll return unknown
    else 
      return boost::make_shared<CPUnknownKind>();
  }
  
  // We've enumerated all cases so we should never get here
  cerr << "ERROR: no support for "<<SgNode2Str(op)<<"!";
  assert(0);
}

// Returns whether the two given SgValueExps denote the same numeric value. 
// If unknown, return unknownVal.
bool CPConcreteKind::equalVals(SgValueExp* val1, SgValueExp* val2, bool unknownVal) {
  long long sVal1, sVal2;
  unsigned long long usVal1, usVal2;
  
  // First check if both concrete values are integers of some type
  if(IsSignedConstInt(val1, sVal1)) {
    if(IsSignedConstInt(val2, sVal2))
      return sVal1==sVal2;
    else if(IsUnsignedConstInt(val2, usVal2))
      return sVal1==(long long)usVal2;
  } else if(IsUnsignedConstInt(val1, usVal1)) {
    if(IsSignedConstInt(val2, sVal2))
      return (long long)usVal1==sVal2;
    else if(IsUnsignedConstInt(val2, usVal2))
      return usVal1==usVal2;
  }
  // If the values are not integers, lets see if they're floating point
  double fVal1, fVal2;
  if(IsConstFloat(val1, fVal1) && IsConstFloat(val2, fVal2))
    return fVal1==fVal2;

  // Otherwise, return unknownVal since we don't handle the other cases
  return unknownVal;
}

// Returns whether the SgValueExps denoted by val1 < the value denoted by val2.
// If unknown, return unknownVal.
bool CPConcreteKind::lessThanVals(SgValueExp* val1, SgValueExp* val2, bool unknownVal) {
  long long sVal1, sVal2;
  unsigned long long usVal1, usVal2;
  
  // First check if both concrete values are integers of some type
  if(IsSignedConstInt(val1, sVal1)) {
    if(IsSignedConstInt(val2, sVal2))
      return sVal1<sVal2;
    else if(IsUnsignedConstInt(val2, usVal2))
      return sVal1<(long long)usVal2;
  } else if(IsUnsignedConstInt(val1, usVal1)) {
    if(IsSignedConstInt(val2, sVal2))
      return (long long)usVal1<sVal2;
    else if(IsUnsignedConstInt(val2, usVal2))
      return usVal1<usVal2;
  }
  // If the values are not integers, lets see if they're floating point
  double fVal1, fVal2;
  if(IsConstFloat(val1, fVal1) && IsConstFloat(val2, fVal2))
    return fVal1<fVal2;

  // Otherwise, return unknownVal since we don't handle the other cases
  return unknownVal;
}

// Returns whether this and that CPValueKinds are may/must equal to each other
bool CPConcreteKind::mayEqualAO(CPValueKindPtr that)
{
  // If that is not concrete, use its implementation
  if(that->getKind() != CPValueKind::concrete) return that->mayEqualAO(shared_from_this());
  CPConcreteKindPtr thatConcrete = that->asConcreteKind();
  
  return equalVals(exp.get(), thatConcrete->getVal().get(),
                   /*unknownVal, default to mayEquals*/true);
}

bool CPConcreteKind::mustEqualAO(CPValueKindPtr that) {
  // If that is not concrete, use its implementation
  if(that->getKind() != CPValueKind::concrete) return that->mustEqualAO(shared_from_this());
  CPConcreteKindPtr thatConcrete = that->asConcreteKind();
  
  return equalVals(exp.get(), thatConcrete->getVal().get(),
                   /*unknownVal, default to mustEquals*/false);
}

// Returns whether the two CPValueKinds denote the same set of concrete values
bool CPConcreteKind::equalSetAO(CPValueKindPtr that) {
  // The logic here is the same as mustEquals
  return mustEqualAO(that);
}

// Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given CPValueKind
bool CPConcreteKind::subSetAO(CPValueKindPtr that) {
  // The logic here is the same as mustEquals
  return mustEqualAO(that);
}

// Computes the meet of this and that and returns the resulting kind
pair<bool, CPValueKindPtr> CPConcreteKind::meetUpdateAO(CPValueKindPtr that)
{
  //scope s("CPConcreteKind::meetUpdateV");
  // Concrete MEET Uninitialized => Concrete
  if(that->getKind() == CPValueKind::uninitialized) {
    //dbg << "that=>uninitialized"<<endl;
    return make_pair(true, copyAOType());
  }
  
  // Concrete MEET Unknown => Unknown
  // Concrete MEET Offset  => Unknown
  if(that->getKind() == CPValueKind::offsetList ||
     that->getKind() == CPValueKind::unknown) {
    //dbg << "that=>offsetlist or unknown"<<endl;
    return make_pair(true, boost::make_shared<CPUnknownKind>());
  }
  
  // That is definitely concrete
  
  // If this and that denote the same concrete value
  if(mustEqualAO(that)) {
    //dbg << "must equal, not modified"<<endl;
    return make_pair(false, copyAOType());
  }
  // If the concrete values differ
  else {
    //dbg << "different, modified"<<endl;
    return make_pair(true, boost::make_shared<CPUnknownKind>());
  }
}

// Computes the intersection of this and that and returns the resulting kind
pair<bool, CPValueKindPtr> CPConcreteKind::intersectUpdateAO(CPValueKindPtr that)
{
  /*scope s("CPConcreteKind::intersectUpdateV");
  dbg << "this="<<str()<<endl;
  dbg << "that="<<that->str()<<endl;*/
  // Concrete Intersection Unknown => Concrete
  if(that->getKind() == CPValueKind::unknown) {
    //dbg << "that=>uninitialized"<<endl;
    return make_pair(true, copyAOType());
  }

  // Concrete INTERSECT Uninitialized => Uninitialized
  // Concrete INTERSECT Offset  => Uninitialized
  if(that->getKind() == CPValueKind::offsetList ||
     that->getKind() == CPValueKind::uninitialized) {
    //dbg << "that=>offsetlist or unknown"<<endl;
    return make_pair(true, boost::make_shared<CPUninitializedKind>());
  }

  // That is definitely concrete

  // If this and that denote the same concrete value
  if(mustEqualAO(that)) {
    //dbg << "must equal, not modified"<<endl;
    return make_pair(false, copyAOType());
  }
  // If the concrete values differ
  else {
    //dbg << "different, modified"<<endl;
    return make_pair(true, boost::make_shared<CPUninitializedKind>());
  }
}

// Returns true if this ValueObject corresponds to a concrete value that is statically-known
bool CPConcreteKind::isConcrete()
{ return true; }

// Returns the number of concrete values in this set
int CPConcreteKind::concreteSetSize()
{ return 1; }

// Returns the type of the concrete value (if there is one)
SgType* CPConcreteKind::getConcreteType()
{ return exp->get_type(); }

// Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
// the normal ROSE mechanisms to decode it
std::set<boost::shared_ptr<SgValueExp> > CPConcreteKind::getConcreteValue() { 
  std::set<boost::shared_ptr<SgValueExp> > vals;
  vals.insert(exp);
  return vals;
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool CPConcreteKind::isFullAO(PartEdgePtr pedge) { return false; }
// Returns whether this AbstractObject denotes the empty set.
bool CPConcreteKind::isEmptyAO(PartEdgePtr pedge) { return false; }

std::string CPConcreteKind::str(std::string indent) const
{ return txt()<<"[CPConcreteKind: val="<<(exp? SgNode2Str(exp.get()): "NULL")<<"]"; }


// ********************************
// **** CPOffsetListKind *****
// ********************************

SgTypeLongLong *CPOffsetListKind::type = NULL;

// Applies the given unary or binary operation to this and the given CPValueKind
// Returns:
//    - if this CPValueKind can be updated to incorporate the result of the addition,
//       return a freshly-allocated CPValueKind that holds the result.
//    - if the two objects could not be merged and therefore that must be placed after
//       this in the parent CPValueObject's list, return that.
CPValueKindPtr CPOffsetListKind::op(SgUnaryOp* op) {
  // The only operations defiend on offset kinds are binary + and -
  return boost::make_shared<CPUnknownKind>();
}

CPValueKindPtr CPOffsetListKind::op(SgBinaryOp* op, CPValueKindPtr that) {
  // * op uninitialized => *
  if(that && that->getKind() == CPValueKind::uninitialized)
    return copyAOType();
  
  // * op unknown => unknown
  if(that && that->getKind() == CPValueKind::unknown)
    return boost::make_shared<CPUnknownKind>();
  
  // If that is a concrete value or that was not provided because it is not needed (e.g. dot expression)
  if(!that || that->getKind() == CPValueKind::concrete) {
    CPConcreteKindPtr thatConcrete; if(that) thatConcrete = that->asConcreteKind();
    assert(offsetL.size()>0);
 
    // The arithmetic only operations defined on offset kinds are binary + and -
    switch(op->variantT()) {
      case V_SgAddOp:
      {
        assert(thatConcrete);
        // The offset list of the newly-generated offset kind
        list<intWrap> newOffsetL = offsetL;

        // If the last value in the offset list is a concrete offset
        if(offsetL.back().getType()==intWrap::offsetT) {
          long long sVal;
          unsigned long long usVal;
          long long res;
          if(IsSignedConstInt(thatConcrete->getVal().get(), sVal))
            res = offsetL.back().get() + sVal;
          else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal))
            res = offsetL.back().get() + usVal;
          else
            return boost::make_shared<CPUnknownKind>();

          newOffsetL.back().set(res);
        // If the last value in the offset is a rank, append the concrete offset onto the list
        } else if(offsetL.back().getType()==intWrap::rankT) {
          long long sVal;
          unsigned long long usVal;
          if(IsSignedConstInt(thatConcrete->getVal().get(), sVal))
            offsetL.push_back(intWrap(sVal, intWrap::offsetT));
          else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal))
            offsetL.push_back(intWrap((long long)usVal, intWrap::offsetT));
          else
            return boost::make_shared<CPUnknownKind>();
        }

        return boost::make_shared<CPOffsetListKind>(newOffsetL);
      }
      
      case V_SgSubtractOp :
      {
        assert(thatConcrete);
        // The offset list of the newly-generated offset kind
        list<intWrap> newOffsetL = offsetL;

        // If the last value in the offset list is a concrete offset
        if(offsetL.back().getType()==intWrap::offsetT) {
          long long sVal;
          unsigned long long usVal;
          long long res;
          if(IsSignedConstInt(thatConcrete->getVal().get(), sVal))
            res = offsetL.back().get() - sVal;
          else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal))
            res = offsetL.back().get() - usVal;
          else
            return boost::make_shared<CPUnknownKind>();

          newOffsetL.back().set(res);
        // If the last value in the offset is a rank, append the concrete offset onto the list
        } else if(offsetL.back().getType()==intWrap::rankT) {
          long long sVal;
          unsigned long long usVal;
          if(IsSignedConstInt(thatConcrete->getVal().get(), sVal))
            offsetL.push_back(intWrap(sVal, intWrap::offsetT));
          else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal))
            offsetL.push_back(intWrap((long long)usVal, intWrap::offsetT));
          else
            return boost::make_shared<CPUnknownKind>();
        }

        return boost::make_shared<CPOffsetListKind>(newOffsetL);
      }
       
      // ----- Comparison -----
      case V_SgEqualityOp:
      case V_SgNotEqualOp:
      case V_SgLessOrEqualOp:
      case V_SgLessThanOp:
      case V_SgGreaterOrEqualOp:
      case V_SgGreaterThanOp:
      {
        assert(thatConcrete);
        // If this offset is just a single concrete value
        if(offsetL.size()==1 && offsetL.back().getType()==intWrap::offsetT) {
          long long sVal;
          unsigned long long usVal;
          switch(op->variantT()) {
            case V_SgEqualityOp:
              if(IsSignedConstInt(thatConcrete->getVal().get(), sVal)) {
                if(sVal==offsetL.back().get()) 
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal)) {
                if((long long)usVal==offsetL.back().get())
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              }
              break;
              
            case V_SgNotEqualOp:
              if(IsSignedConstInt(thatConcrete->getVal().get(), sVal)) {
                if(sVal!=offsetL.back().get()) 
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal)) {
                if((long long)usVal!=offsetL.back().get())
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              }
              break;
            
            case V_SgLessOrEqualOp:
              if(IsSignedConstInt(thatConcrete->getVal().get(), sVal)) {
                if(sVal<=offsetL.back().get()) 
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal)) {
                if((long long)usVal<=offsetL.back().get())
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              }
              break;
            
            case V_SgLessThanOp:
              if(IsSignedConstInt(thatConcrete->getVal().get(), sVal)) {
                if(sVal<offsetL.back().get()) 
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal)) {
                if((long long)usVal<offsetL.back().get())
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              }
              break;
              
            case V_SgGreaterOrEqualOp:
              if(IsSignedConstInt(thatConcrete->getVal().get(), sVal)) {
                if(sVal>=offsetL.back().get()) 
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal)) {
                if((long long)usVal>=offsetL.back().get())
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              }
              break;
            
            case V_SgGreaterThanOp:
              if(IsSignedConstInt(thatConcrete->getVal().get(), sVal)) {
                if(sVal>offsetL.back().get()) 
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), usVal)) {
                if((long long)usVal>offsetL.back().get())
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(true)));
                else
                  return boost::make_shared<CPConcreteKind>(boost::shared_ptr<SgValueExp>(SageBuilder::buildBoolValExp(false)));
              }
              break;
              
            default:
              break;
          }
        }
        // If we don't have enough information to compare the two values
        return boost::make_shared<CPUnknownKind>();
      }
      
      // ----- Memory References -----
      case V_SgDotExp:
      {
        SIGHT_VERB_DECL(scope, ("V_SgDotExp", scope::medium), 1, constantPropagationAnalysisDebugLevel)
        // Find the index of the DotExp's RHS within the class of its LHS
        SgClassType *type;
        if((type = isSgClassType(isSgDotExp(op)->get_lhs_operand()->get_type()))==NULL) {
          if(isSgReferenceType(isSgDotExp(op)->get_lhs_operand()->get_type()))
            type = isSgClassType(isSgReferenceType(isSgDotExp(op)->get_lhs_operand()->get_type())->get_base_type());
        }
/*        cout << "op="<<SgNode2Str(op)<<endl;
        cout << "isSgDotExp(op)->get_lhs_operand()="<<SgNode2Str(isSgDotExp(op)->get_lhs_operand())<<endl;
        cout << "type="<<SgNode2Str(isSgDotExp(op)->get_lhs_operand()->get_type())<<endl;
        if(isSgReferenceType(isSgDotExp(op)->get_lhs_operand()->get_type())) {
          cout << "isSgReferenceType(type)->get_base_type()="<<SgNode2Str(isSgReferenceType(isSgDotExp(op)->get_lhs_operand()->get_type())->get_base_type())<<endl;
        }*/
        assert(type);
        SgClassDeclaration* decl = isSgClassDeclaration(type->get_declaration()->get_definingDeclaration());
        assert(decl);
        
        /*dbg << "  type="<<SgNode2Str(type)<<endl;
        dbg << "  decl="<<SgNode2Str(type->get_declaration())<<", classdecl="<<isSgClassDeclaration(type->get_declaration()->get_definingDeclaration())<<endl;
        dbg << "  def="<<SgNode2Str(isSgClassDeclaration(type->get_declaration()->get_definingDeclaration())->get_definition())<<endl;*/
        SgClassDefinition* def = decl->get_definition();
        assert(def);

        const SgDeclarationStatementPtrList& members = def->get_members();
        long long memberIdx=0;
        SgInitializedName* rhsVarDecl=NULL;
        SgMemberFunctionDeclaration* rhsFuncDecl=NULL;
        if(isSgVarRefExp(isSgDotExp(op)->get_rhs_operand()))
          rhsVarDecl = isSgVarRefExp(isSgDotExp(op)->get_rhs_operand())->get_symbol()->get_declaration();
        else if(isSgMemberFunctionRefExp(isSgDotExp(op)->get_rhs_operand()))
          rhsFuncDecl = isSgMemberFunctionDeclaration(isSgMemberFunctionRefExp(isSgDotExp(op)->get_rhs_operand())->get_symbol()->get_declaration()->get_definingDeclaration());
        //dbg << "rhsDecl="<<SgNode2Str(rhsDecl)<<endl;
        assert(rhsVarDecl || rhsFuncDecl);

        for(SgDeclarationStatementPtrList::const_iterator m=members.begin(); m!=members.end(); m++) {
          //scope s2(txt()<<memberIdx<<":    member ="<<SgNode2Str(*m));
          if(isSgVariableDeclaration(*m)) {
            const SgInitializedNamePtrList& decls = isSgVariableDeclaration(*m)->get_variables();
            for(SgInitializedNamePtrList::const_iterator d=decls.begin(); d!=decls.end(); d++) {
              //dbg << "        decl "<<memberIdx<<"="<<SgNode2Str(*d)<<" type="<<SgNode2Str((*d)->get_type())<<endl;
              if(*d == rhsVarDecl) { 
                //dbg << "    memberIdx="<<memberIdx<<endl;
                
                // Return a new offset kind that extends the current one by appending a memberIdx rank
                list<intWrap> newOffsetL = offsetL;
                // Remove the trailing element in newOffsetL if it is a concrete constant 0 since such
                // a concrete offset has no meaning
                if(newOffsetL.back().getType()==intWrap::offsetT && newOffsetL.back().get()==0) newOffsetL.pop_back();
                newOffsetL.push_back(rank(memberIdx));
                return boost::make_shared<CPOffsetListKind>(newOffsetL);
              }
              memberIdx++;
            }
          } else if(isSgMemberFunctionDeclaration(*m)) {
            if(rhsFuncDecl == isSgMemberFunctionDeclaration(*m)->get_definingDeclaration()) {
              //dbg << "    memberIdx="<<memberIdx<<endl;

              // Return a new offset kind that extends the current one by appending a memberIdx rank
              list<intWrap> newOffsetL = offsetL;
              // Remove the trailing element in newOffsetL if it is a concrete constant 0 since such
              // a concrete offset has no meaning
              if(newOffsetL.back().getType()==intWrap::offsetT && newOffsetL.back().get()==0) newOffsetL.pop_back();
              newOffsetL.push_back(rank(memberIdx));
              return boost::make_shared<CPOffsetListKind>(newOffsetL);
            }
            memberIdx++;
          }
          
          /*else if(isSgClassDeclaration(*m)) {
            memberIdx += getNumClassMembers(isSgClassDeclaration(isSgClassDeclaration(*m)->get_definingDeclaration())->get_definition());
          }*/
        }
        assert(0); // There must be at least one match
      }
      
      case V_SgArrowExp: 
        // Arrow expressions introduce aliasing, so we don't know their referent 
        return boost::make_shared<CPUnknownKind>();
    
      case V_SgPntrArrRefExp:
      {
        SIGHT_VERB_DECL(scope, ("V_SgPntrArrRefExp", scope::medium), 1, constantPropagationAnalysisDebugLevel)
        assert(thatConcrete);
        
        SgPntrArrRefExp* ref = isSgPntrArrRefExp(op);
        
        // Get the offset of the SgPntrArrRefExp relative to the starting point of its parent expression
        long long nextOffset = getPntrArrRefOffset(ref, thatConcrete);
        list<intWrap> newOffsetL = offsetL;
        
        // If the last element in the current offset list is a concrete value, add nextOffset to it
        if(offsetL.back().getType()==intWrap::offsetT)
          newOffsetL.back().set(newOffsetL.back().get() + nextOffset);
        // If the last element is a rank, append nextOffset
        else
          newOffsetL.push_back(offset(nextOffset));
          
        return boost::make_shared<CPOffsetListKind>(newOffsetL);
      }
      
      case V_SgDotStarOp:
        // TODO
        return boost::make_shared<CPUnknownKind>();
        
      case V_SgPointerAssignOp:
        // TODO (Fortran)
        return boost::make_shared<CPUnknownKind>();
    
      // ----- Miscelaneous ----
      case V_SgAssignOp:
        // This should be handled inside the ConstantPropagationAnalysis transfer function
        assert(0);

      case V_SgCommaOpExp:
        // This should be handled inside the control flow functionality
        assert(0);
        
      default:
        return boost::make_shared<CPUnknownKind>();
    }
  } else if(that->getKind() == CPValueKind::offsetList) {
    // This is not yet implemented so this assert will notify us when we need to do the heavy lifting
    assert(0);
  }
  
  // We've enumerated all cases so we should never get here
  cerr << "ERROR: no support for "<<SgNode2Str(op)<<"!";
  assert(0);
}

// Returns whether this and that CPValueKinds are may/must equal to each other
bool CPOffsetListKind::mayEqualAO(CPValueKindPtr that)
{
  // If that unknown or uninitialized, use its implementation
  if(that->getKind() == CPValueKind::uninitialized || that->getKind() == CPValueKind::unknown) 
    return that->mayEqualAO(shared_from_this());

  if(that->getKind() == CPValueKind::concrete) {
    CPConcreteKindPtr thatConcrete = that->asConcreteKind();
    
    // If this object denotes a concrete value
    if(offsetL.size()==1 && offsetL.begin()->getType()==intWrap::offsetT) {
      long long thatSV;
      unsigned long long thatUSV;
      // If that object is a concrete integer (as opposed to some other concrete value), use == comparison
      if(IsSignedConstInt(thatConcrete->getVal().get(), thatSV)) {
        return offsetL.begin()->get() == thatSV;
      } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), thatUSV)) {
        return offsetL.begin()->get() == (long long)thatUSV;
      // If that object is not an integer, they may not be equal
      } else
        return false;
    // If this object denotes some combination of ranks and concrete values
    } else
      // Conservatively answer that they're may-equal
      return true;
  }
  
  if(that->getKind() == CPValueKind::offsetList) {
    CPOffsetListKindPtr thatOffset = that->asOffsetListKind();
    
    list<intWrap>::iterator itThis = offsetL.begin();
    list<intWrap>::iterator itThat = thatOffset->offsetL.begin();
    for(; itThis!=offsetL.end() && itThat!=thatOffset->offsetL.end(); itThis++, itThat++) {
      // Check if the current entry in both lists is identical. If not, they're not may-equal
      if(itThis->getType() != itThat->getType()) return false;
      if(itThis->get()     != itThat->get())     return false;
    }
    // We've reached the end of one or both of the offset lists
    
    // If we've reached the end of both offset lists, then the objects denote the same offset
    if(itThis==offsetL.end() && itThat==thatOffset->offsetL.end()) return true;
    
    // We've reached the end of only one offset list, meaning that the other object refers to some
    // deeper offset within the parent memory region. The objects may denote the same offset only if
    // the un-finished object's remaining offsetL elements all denote a 0 offset or 0 rank
    // !!!GB: I'm not sure about the 0 rank since there may be additional padding!!!
    if(itThis==offsetL.end()) 
      for(; itThat!=thatOffset->offsetL.end(); itThat++) { if(itThat->get()!=0) return false; }
    if(itThat==thatOffset->offsetL.end())
      for(; itThis!=offsetL.end(); itThis++) { if(itThis->get()!=0) return false; }
    
    // All the offsets are 0, so the two objects are may-equal
    return true;
  }
  
  // We've covered all the cases
  assert(0);
}

bool CPOffsetListKind::mustEqualAO(CPValueKindPtr that) {
  // If that unknown or uninitialized, use its implementation
  if(that->getKind() == CPValueKind::uninitialized || that->getKind() == CPValueKind::unknown) 
    return that->mayEqualAO(shared_from_this());

  if(that->getKind() == CPValueKind::concrete) {
    CPConcreteKindPtr thatConcrete = that->asConcreteKind();
    
    // If this object denotes a concrete value
    if(offsetL.size()==1 && offsetL.begin()->getType()==intWrap::offsetT) {
      long long thatSV;
      unsigned long long thatUSV;
      // If that object is a concrete integer (as opposed to some other concrete value), use == comparison
      if(IsSignedConstInt(thatConcrete->getVal().get(), thatSV)) {
        return offsetL.begin()->get() == thatSV;
      } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), thatUSV)) {
        return offsetL.begin()->get() == (long long)thatUSV;
      // If that object is not an integer, they may not be equal
      } else
        return false;
    // If this object denotes some combination of ranks and concrete values
    } else
      // Conservatively answer that they're not must-equal
      return false;
  }
  
  if(that->getKind() == CPValueKind::offsetList) {
    CPOffsetListKindPtr thatOffset = that->asOffsetListKind();
    
    list<intWrap>::iterator itThis = offsetL.begin();
    list<intWrap>::iterator itThat = thatOffset->offsetL.begin();
    for(; itThis!=offsetL.end() && itThat!=thatOffset->offsetL.end(); itThis++, itThat++) {
      // Check if the current entry in both lists is identical. If not, they're not may-equal
      if(itThis->getType() != itThat->getType()) return false;
      if(itThis->get()     != itThat->get())     return false;
    }
    // We've reached the end of one or both of the offset lists
    
    // If we've reached the end of both offset lists, then the objects denote the same offset
    if(itThis==offsetL.end() && itThat==thatOffset->offsetL.end()) return true;
    
    // We've reached the end of only one offset list, meaning that the other object refers to some
    // deeper offset within the parent memory region. The objects may denote the same offset only if
    // the un-finished object's remaining offsetL element is a single concrete value 0.
    if(itThis==offsetL.end() && itThat->getType()!=intWrap::offsetT && itThat->get()==0) return true;
    if(itThat==thatOffset->offsetL.end() && itThis->getType()!=intWrap::offsetT && itThis->get()==0) return true;
    
    // The trailing offset is not a concrete 0, so the objects are not must-equal
    return false;
  }
  
  // We've covered all the cases
  assert(0);
}

// Returns whether the two CPValueKinds denote the same set of concrete values
bool CPOffsetListKind::equalSetAO(CPValueKindPtr that) {
  // The logic here is the same as mustEquals
  return mustEqualAO(that);
}

// Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given CPValueKind
bool CPOffsetListKind::subSetAO(CPValueKindPtr that) {
  // The logic here is the same as mustEquals
  return mustEqualAO(that);
}

// Computes the meet of this and that and returns the resulting kind
pair<bool, CPValueKindPtr> CPOffsetListKind::meetUpdateAO(CPValueKindPtr that)
{
  // OffsetList MEET Uninitialized => OffsetList
  if(that->getKind() == CPValueKind::uninitialized) 
    return make_pair(true, copyAOType());
  
  // OffsetList MEET Unknown => Unknown
  if(that->getKind() == CPValueKind::unknown)
    return make_pair(true, boost::make_shared<CPUnknownKind>());

  // OffsetList MEET Concrete
  if(that->getKind() == CPValueKind::concrete) {
    CPConcreteKindPtr thatConcrete = that->asConcreteKind();
    
    // If this offset is just a single concrete value
    if(offsetL.size()==1 && offsetL.back().getType()==CPOffsetListKind::intWrap::offsetT) {
      // If both objects denote the same concrete value
      long long thatSV;
      unsigned long long thatUSV;
      if(IsSignedConstInt(thatConcrete->getVal().get(), thatSV)) {
        if(offsetL.back().get()==thatSV) return make_pair(false, copyAOType());
      } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), thatUSV)) {
        if(offsetL.back().get()==(long long)thatUSV) return make_pair(false, copyAOType());
      }
    }
    // If the two objects do not denote the same concrete value
    return make_pair(true, boost::make_shared<CPUnknownKind>());
  }
  
  // OffsetList MEET OffsetList
  if(that->getKind() == CPValueKind::offsetList) {
    CPOffsetListKindPtr thatOffset = that->asOffsetListKind();
    // Compare the two offset lists directly 
    // !!! (NOTE: this comparison doesn't take types into account when ranks are compared)
    if(offsetL.size() == thatOffset->offsetL.size()) {
      list<intWrap>::const_iterator thisI = offsetL.begin();
      list<intWrap>::const_iterator thatI = thatOffset->offsetL.begin();
      for(; thisI!=offsetL.end(); thisI++, thatI++) {
        // If the two offsetList objects are not identical
        if(*thisI != *thatI) return make_pair(true, boost::make_shared<CPUnknownKind>());
      }
      // If we reached this point the two offset lists must be identical
      return make_pair(false, copyAOType());
    // The two objects have lists of different sizes, so they're not identical
    } else
      return make_pair(true, boost::make_shared<CPUnknownKind>());
  }
  
  // We've covered all the cases
  assert(0);
}

// Computes the intersection of this and that and returns the resulting kind
pair<bool, CPValueKindPtr> CPOffsetListKind::intersectUpdateAO(CPValueKindPtr that)
{
  // OffsetList INTERSECT Unknown => OffsetList
  if(that->getKind() == CPValueKind::unknown)
    return make_pair(true, copyAOType());

  // OffsetList INTERSECT Uninitialized => Uninitialized
  if(that->getKind() == CPValueKind::uninitialized)
    return make_pair(true, boost::make_shared<CPUninitializedKind>());

  // OffsetList INTERSECT Concrete
  if(that->getKind() == CPValueKind::concrete) {
    CPConcreteKindPtr thatConcrete = that->asConcreteKind();

    // If this offset is just a single concrete value
    if(offsetL.size()==1 && offsetL.back().getType()==CPOffsetListKind::intWrap::offsetT) {
      // If both objects denote the same concrete value
      long long thatSV;
      unsigned long long thatUSV;
      if(IsSignedConstInt(thatConcrete->getVal().get(), thatSV)) {
        if(offsetL.back().get()==thatSV) return make_pair(false, copyAOType());
      } else if(IsUnsignedConstInt(thatConcrete->getVal().get(), thatUSV)) {
        if(offsetL.back().get()==(long long)thatUSV) return make_pair(false, copyAOType());
      }
    }
    // If the two objects do not denote the same concrete value, their intersection is empty
    return make_pair(true, boost::make_shared<CPUninitializedKind>());
  }

  // OffsetList INTERSECT OffsetList
  if(that->getKind() == CPValueKind::offsetList) {
    CPOffsetListKindPtr thatOffset = that->asOffsetListKind();
    // Compare the two offset lists directly
    // !!! (NOTE: this comparison doesn't take types into account when ranks are compared)
    if(offsetL.size() == thatOffset->offsetL.size()) {
      list<intWrap>::const_iterator thisI = offsetL.begin();
      list<intWrap>::const_iterator thatI = thatOffset->offsetL.begin();
      for(; thisI!=offsetL.end(); thisI++, thatI++) {
        // If the two offsetList objects are not identical, their intersection is empty
        if(*thisI != *thatI) return make_pair(true, boost::make_shared<CPUninitializedKind>());
      }
      // If we reached this point the two offset lists must be identical
      return make_pair(false, copyAOType());
    // The two objects have lists of different sizes, so they're not identical and their intersection is empty
    } else
      return make_pair(true, boost::make_shared<CPUninitializedKind>());
  }

  // We've covered all the cases
  assert(0);
}


// Returns true if this ValueObject corresponds to a concrete value that is statically-known
bool CPOffsetListKind::isConcrete()
{ return offsetL.size()==1 && offsetL.back().getType()==CPOffsetListKind::intWrap::offsetT; }

// Returns the number of concrete values in this set
int CPOffsetListKind::concreteSetSize()
{ return (isConcrete()? 1: -1); }

// Returns the type of the concrete value (if there is one)
SgType* CPOffsetListKind::getConcreteType()
// !!! NOTE: this may be a memory leak
{ return type; }

// Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
// the normal ROSE mechanisms to decode it
std::set<boost::shared_ptr<SgValueExp> > CPOffsetListKind::getConcreteValue() { 
  assert(isConcrete());
  std::set<boost::shared_ptr<SgValueExp> > vals;
  vals.insert(boost::shared_ptr<SgLongLongIntVal>(SageBuilder::buildLongLongIntVal(offsetL.back().get())));
  return vals;
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool CPOffsetListKind::isFullAO(PartEdgePtr pedge) { return false; }
// Returns whether this AbstractObject denotes the empty set.
bool CPOffsetListKind::isEmptyAO(PartEdgePtr pedge) { return false; }

std::string CPOffsetListKind::str(std::string indent) const { 
  ostringstream oss; 
  
  oss <<"[CPOffsetListKind: offsetL=";
  for(list<intWrap>::const_iterator o=offsetL.begin(); o!=offsetL.end(); o++) {
    if(o!=offsetL.begin()) oss << ",";
    oss << o->get();
  }
  oss << "]";

  return oss.str();
}

// *****************************
// **** CPUnknownKind *****
// *****************************

// Applies the given unary or binary operation to this and the given CPValueKind
// Returns:
//    - if this CPValueKind can be updated to incorporate the result of the addition,
//       return a freshly-allocated CPValueKind that holds the result.
//    - if the two objects could not be merged and therefore that must be placed after
//       this in the parent CPValueObject's list, return that.
CPValueKindPtr CPUnknownKind::op(SgUnaryOp* op) {
  // Uninitialized denotes the full value set, so any operation applied to it results in the full set
  return copyAOType();
}

CPValueKindPtr CPUnknownKind::op(SgBinaryOp* op, CPValueKindPtr that) {
  // Uninitialized denotes the full value set, so any operation that involves it results in the full set
  return copyAOType();
}

// Returns whether this and that CPValueKinds are may/must equal to each other
bool CPUnknownKind::mayEqualAO(CPValueKindPtr that) {
  // Unknown denotes the full set, which overlaps with every other set
  return true;
}

bool CPUnknownKind::mustEqualAO(CPValueKindPtr that) {
  // Unknown denotes the full set, which has unbounded size and therefore is not must-equal to any set
  return false;
}

// Returns whether the two CPValueKinds denote the same set of concrete values
bool CPUnknownKind::equalSetAO(CPValueKindPtr that) {
  // Unknown denotes the full set, which may only be equal to another full set
  return that->getKind() == CPValueKind::unknown;
}

// Returns whether this CPValueKind denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given CPValueKind
bool CPUnknownKind::subSetAO(CPValueKindPtr that) {
  // Unknown  denotes the full set, which is a subset of another full set
  return that->getKind() == CPValueKind::unknown;
}

// Computes the meet of this and that and returns the resulting kind
pair<bool, CPValueKindPtr> CPUnknownKind::meetUpdateAO(CPValueKindPtr that)
{
  bool modified = false;
  // Unknown MEET * => Unknown
  return make_pair(modified, copyAOType());
}

// Computes the intersection of this and that and returns the resulting kind
std::pair<bool, CPValueKindPtr> CPUnknownKind::intersectUpdateAO(CPValueKindPtr that) {
  bool modified = that->getKind() != CPValueKind::unknown;
  // Unknown INTERSECT * => *
  return make_pair(modified, that->copyAOType());
}

// Returns true if this ValueObject corresponds to a concrete value that is statically-known
bool CPUnknownKind::isConcrete()
{ return false; }

// Returns the number of concrete values in this set
int CPUnknownKind::concreteSetSize()
{ return -1; }

// Returns the type of the concrete value (if there is one)
SgType* CPUnknownKind::getConcreteType()
{ return NULL; }

// Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
// the normal ROSE mechanisms to decode it
std::set<boost::shared_ptr<SgValueExp> > CPUnknownKind::getConcreteValue()
{ return std::set<boost::shared_ptr<SgValueExp> >(); }

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool CPUnknownKind::isFullAO(PartEdgePtr pedge) { return true; }
// Returns whether this AbstractObject denotes the empty set.
bool CPUnknownKind::isEmptyAO(PartEdgePtr pedge) { return false; }

std::string CPUnknownKind::str(std::string indent) const
{ return "[CPUnknownKind]"; }


// *************************
// **** CPMemLocObject *****
// *************************

// returns a copy of this lattice
/*Lattice* CPMemLocObject::copy() const {
  return new CPMemLocObject(*this);
}*/

/*bool
CPMemLocObject::operator==(Lattice* X)
{
  // Implementation of equality operator.
  CPMemLocObject* that = dynamic_cast<CPMemLocObject*>(X);
  assert(that);
  return (this->getRegion() == that->getRegion() &&
          this->getIndex()  == that->getIndex());
}*/

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
/*Lattice* CPMemLocObject::remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge) {
  if(isCPFull || isCPEmpty) return dynamic_cast<CPMemLocObject*>(copyAOTypePtr());
  
  CPMemLocObject* ret=NULL;
  for(std::set<MLMapping>::const_iterator m=ml2ml.begin(); m!=ml2ml.end(); m++) {
    if(m->from->mayEqual(MemLocObject::shared_from_this(), fromPEdge, analysis->getComposer(), analysis)) {
      if(ret==NULL) {
        ret = dynamic_cast<CPMemLocObject*>(m->to->copyMLPtr());
        assert(ret);
      } else
        ret->MemLocObject::meetUpdate(m->to, fromPEdge, analysis->getComposer(), analysis);
    }
  }
  assert(ret);
  return ret;
}*/

// Returns whether this object may/must be equal to o within the given Part p
// These methods are called by composers and should not be called by analyses.
bool CPMemLocObject::mayEqualAO(MemLocObjectPtr o, PartEdgePtr pedge) {
  CPMemLocObjectPtr that = boost::dynamic_pointer_cast<CPMemLocObject>(o);
  assert(that);
  
  /*scope s("CPMemLocObject::mayEqualML");
  dbg << "this="<<str()<<endl;
  dbg << "that="<<that->str()<<endl;*/
  
  // If either object denotes the set of all memlocs, they're may-equal
  if(isCPFull || that->isCPFull)
    return true;
  
  // If either object denotes the empty set, they're not may-equal
  if(isCPEmpty || that->isCPEmpty)
    return false;
  
  // Before we call into the parent class, make sure that it can understand the state of 
  // this MemLocObject (a full or empty CPMemLocObject has NULL region and index)
  assert(!isCPFull && !that->isCPFull && !isCPEmpty && !that->isCPEmpty);
  
  // Compare the region and index directly. We call mayEqualMR here because CP does not implement
  // MemRegions, meaning that we need to thread the logic through the composer to the real implementer.
  // However, values are implemented in CP, meaning that we can call mayEqualV directly
  // dbg << "region->mayEqual()=" << region->mayEqual(that->getRegion(), pedge, analysis->getComposer(), analysis) << endl;
  // dbg << "index->mayEqual()=" << ((!index && !that->index) || index->mayEqualAO(that->getIndex(), pedge)) << endl;
  return region && region->mayEqual(that->getRegion(), pedge, analysis->getComposer(), analysis) && 
         ((!index && !that->index) || index->mayEqualAO(that->getIndex(), pedge));
}

bool CPMemLocObject::mustEqualAO(MemLocObjectPtr o, PartEdgePtr pedge) {
  CPMemLocObjectPtr that = boost::dynamic_pointer_cast<CPMemLocObject>(o);
  assert(that);
  
  /*scope s("CPMemLocObject::mustEqualML");
  dbg << "this="<<str()<<endl;
  dbg << "that="<<that->str()<<endl;*/
  
  // If either object denotes the set of all memlocs, they're not must-equal since they denote unbounded sets
  if(isCPFull || that->isCPFull)
    return false;
  
  // If either object denotes the empty set, they're not must-equal, since must equality occurs only for sets of size 1
  if(isCPEmpty || that->isCPEmpty)
    return false;
  
  // Before we call into the parent class, make sure that it can understand the state of 
  // this MemLocObject (a full or empty CPMemLocObject has NULL region and index)
  assert(!isCPFull && !that->isCPFull && !isCPEmpty && !that->isCPEmpty);
  
  // Compare the region and index directly. We call mustEqualMR here because CP does not implement
  // MemRegions, meaning that we need to thread the logic through the composer to the real implementer.
  // However, values are implemented in CP, meaning that we can call mustEqualV directly
  return region && region->mustEqual(that->getRegion(), pedge, analysis->getComposer(), analysis) && 
         ((!index && !that->index) || index->mustEqualAO(that->getIndex(), pedge));
}

// Returns whether the two abstract objects denote the same set of concrete objects
// These methods are called by composers and should not be called by analyses.
bool CPMemLocObject::equalSetAO(MemLocObjectPtr o, PartEdgePtr pedge) {
  CPMemLocObjectPtr that = boost::dynamic_pointer_cast<CPMemLocObject>(o);
  assert(that);
  
  // If both objects denote the set of all memlocs, they're the same set
  if(isCPFull && that->isCPFull)
    return true;
  
  // If only one of the objects denotes the set of all memlocs, they're not the same set
  if(isCPFull || that->isCPFull)
    return false;
  
  // If both objects denotes the empty set, they're the same set
  if(isCPEmpty && that->isCPEmpty)
    return false;
  
  // If only one of the objects denotes the empty set, they're not the same set
  if(isCPEmpty && that->isCPEmpty)
    return false;

  // Before we call into the parent class, make sure that it can understand the state of 
  // this MemLocObject (a full or empty CPMemLocObject has NULL region and index)
  assert(!isCPFull && !that->isCPFull && !isCPEmpty && !that->isCPEmpty);
  
  // Compare the region and index directly. We call equalSetMR here because CP does not implement
  // MemRegions, meaning that we need to thread the logic through the composer to the real implementer.
  // However, values are implemented in CP, meaning that we can call equalSetV directly
  return region && region->equalSet(that->getRegion(), pedge, analysis->getComposer(), analysis) && 
         ((!index && !that->index) || index->equalSetAO(that->getIndex(), pedge));
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
// These methods are called by composers and should not be called by analyses.
bool CPMemLocObject::subSetAO(MemLocObjectPtr o, PartEdgePtr pedge) {
  CPMemLocObjectPtr that = boost::dynamic_pointer_cast<CPMemLocObject>(o);
  assert(that);
  
  // If that object denotes the set of all memlocs, this is a subset of that
  if(that->isCPFull)
    return true;
  
  // If this denotes the set of all memlocs but that does not, this is not a subset of that
  if(isCPFull)
    return false;
  
  // If this denotes the empty set, it is a subset of that
  if(isCPEmpty)
    return true;
  
  // If that denotes the empty set but this does not, this is not a subset of that
  if(that->isCPEmpty)
    return false;
  
  // Before we call into the parent class, make sure that it can understand the state of 
  // this MemLocObject (a full or empty CPMemLocObject has NULL region and index)
  assert(!isCPFull && !that->isCPFull && !isCPEmpty && !that->isCPEmpty);
  
  // Compare the region and index directly. We call subSetMR here because CP does not implement
  // MemRegions, meaning that we need to thread the logic through the composer to the real implementer.
  // However, values are implemented in CP, meaning that we can call subSetV directly
  return region && region->subSet(that->getRegion(), pedge, analysis->getComposer(), analysis) && 
         ((!index && !that->index) || index->subSetAO(that->getIndex(), pedge));
}

// Returns true if this object is live at the given part and false otherwise
bool CPMemLocObject::isLiveAO(PartEdgePtr pedge) {
  if(isCPFull || isCPEmpty) return true;
  
/*  // For now all CPMemLocs are live but in the future we may restrict this only to the CFGNode
  // of the Base expression.
  return true;*/

  // Forward the query to the parent analysis
  //return MemLocObject::isLiveAO(pedge);
  return MemLocObject::isLive(pedge, analysis->getComposer(), analysis);
}

// Set this Lattice object to represent the set of all possible execution prefixes.
// Return true if this causes the object to change and false otherwise.
bool CPMemLocObject::setToFull() {
  // NOTE: AbstractObjects do not support this method but we may want to make it a universal requirement.
  if(isCPFull) return false;
  
  isCPFull  = true;
  isCPEmpty = false;
  region    = NULLMemRegionObject;
  index     = NULLValueObject;
  return true;
}

// Set this Lattice object to represent the of no execution prefixes (empty set).
// Return true if this causes the object to change and false otherwise.
bool CPMemLocObject::setToEmpty() {
  // NOTE: AbstractObjects do not support this method but we may want to make it a universal requirement.
  if(isCPEmpty) return false;
  
  isCPFull  = false;
  isCPEmpty = true;
  region    = NULLMemRegionObject;
  index     = NULLValueObject;
  return true;
}

/*
// Returns whether this lattice denotes the set of all possible execution prefixes.
bool CPMemLocObject::isFull() {
  return isFullAO(getPartEdge());
}

// Returns whether this lattice denotes the empty set.
bool CPMemLocObject::isEmpty() {
  return isEmptyAO(getPartEdge());
}*/

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool CPMemLocObject::isFullAO(PartEdgePtr pedge) {
  if(isCPFull) return true;
  
  // Compare the region and index directly. We call isFullMR here because CP does not implement
  // MemRegions, meaning that we need to thread the logic through the composer to the real implementer.
  // However, values are implemented in CP, meaning that we can call isFullV directly
  return region->isFull(pedge, analysis->getComposer(), analysis) && 
         (!index || index->isFullAO(pedge));
  
}

// Returns whether this AbstractObject denotes the empty set.
bool CPMemLocObject::isEmptyAO(PartEdgePtr pedge) {
  if(isCPEmpty) return true;
  
  // Compare the region and index directly. We call isEmptyMR here because CP does not implement
  // MemRegions, meaning that we need to thread the logic through the composer to the real implementer.
  // However, values are implemented in CP, meaning that we can call isEmptyV directly
  return region->isEmpty(pedge, analysis->getComposer(), analysis) && 
         (!index || index->isEmptyAO(pedge));
}

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
// The part of this object is to be used for AbstractObject comparisons.
/*bool CPMemLocObject::meetUpdate(Lattice* that_arg)
{
  CPMemLocObject* that = dynamic_cast<CPMemLocObject*>(that_arg);
  assert(that);
  return meetUpdate(that, getPartEdge(), analysis->getComposer(), analysis);
}*/

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool CPMemLocObject::meetUpdateAO(MemLocObjectPtr that_arg, PartEdgePtr pedge) {
  CPMemLocObjectPtr that = boost::dynamic_pointer_cast<CPMemLocObject>(that_arg);
  assert(that);
/*  return meetUpdate(that.get(), pedge, analysis->getComposer(), analysis);
}


// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool CPMemLocObject::meetUpdate(CPMemLocObject* that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis)
{*/
  /*  scope S("CPMemLocObject::meetUpdate");
  dbg << "this="<<str()<<endl;
  dbg << "that="<<that->str()<<endl;*/
  // The regions must be identical
  //assert(getRegion()->equalSet(that->getRegion(), pedge, comp, analysis));

  // If this object is full, don't bother
  if(isCPFull)
    return false;
  // If it is empty, jut copy that over this
  else if(isCPEmpty) {
    if(that->isCPEmpty) return false;

    isCPFull  = that->isCPFull;
    isCPEmpty = that->isCPEmpty;
    region    = that->region;
    index     = that->index;
    return true;
  } else {
    // Meet the regions
    bool modified = getRegion()->meetUpdate(that->getRegion(), pedge, analysis->getComposer(), analysis);

    // Meet the indexes, using a direct call to CPValueObject::meetUpdate since we know that both indexes are
    // CPValueObjects.
    modified = boost::dynamic_pointer_cast<CPValueObject>(getIndex())->meetUpdateAO(
                               boost::dynamic_pointer_cast<CPValueObject>(that->getIndex()), pedge) || modified;

    return modified;
  }
}

/*
// Computes the intersection of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
// The part of this object is to be used for AbstractObject comparisons.
bool CPMemLocObject::intersectUpdate(Lattice* that_arg)
{
  CPMemLocObject* that = dynamic_cast<CPMemLocObject*>(that_arg);
  assert(that);
  return meetUpdate(that, getPartEdge(), analysis->getComposer(), analysis);
}
*/

// Computes the intersection of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool CPMemLocObject::intersectUpdateAO(MemLocObjectPtr that_arg, PartEdgePtr pedge) {
  CPMemLocObjectPtr that = boost::dynamic_pointer_cast<CPMemLocObject>(that_arg);
  assert(that);
/*  return intersectUpdate(that.get(), pedge, analysis->getComposer(), analysis);
}

// Computes the intersection of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool CPMemLocObject::intersectUpdate(CPMemLocObject* that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis)
{*/
  /*  scope S("CPMemLocObject::meetUpdate");
  dbg << "this="<<str()<<endl;
  dbg << "that="<<that->str()<<endl;*/
  // The regions must be identical
  //assert(getRegion()->equalSet(that->getRegion(), pedge, comp, analysis));

  // If this object is empty, don't bother
  if(isCPEmpty)
    return false;
  // If it is full, just copy that over this
  else if(isCPFull) {
    if(that->isCPFull) return false;

    isCPFull  = that->isCPFull;
    isCPEmpty = that->isCPEmpty;
    region    = that->region;
    index     = that->index;
    return true;
  } else {
    // Intersect the regions
    bool modified = getRegion()->intersectUpdate(that->getRegion(), pedge, analysis->getComposer(), analysis);

    // Intersect the indexes, using a direct call to CPValueObject::meetUpdate since we know that both indexes are
    // CPValueObjects.
    modified = boost::dynamic_pointer_cast<CPValueObject>(getIndex())->intersectUpdateAO(
                               boost::dynamic_pointer_cast<CPValueObject>(that->getIndex()), pedge) || modified;

    return modified;
  }
}

// Allocates a copy of this object and returns a pointer to it
MemLocObjectPtr CPMemLocObject::copyAOType() const {
  return boost::make_shared<CPMemLocObject>(*this);
}
  
  // Allocates a copy of this object and returns a regular pointer to it
/*MemLocObject* CPMemLocObject::copyAOTypePtr() const {
  return new CPMemLocObject(*this);
}*/

std::string CPMemLocObject::str(std::string indent) const { // pretty print for the object
  if(isCPFull)       return "[CPMemLocObject: Full]";
  else if(isCPEmpty) return "[CPMemLocObject: Empty]";
  
  ostringstream oss; 
  //oss << "[CPMemLocObject: region="<<(region?region->str(indent+"    "):"NULL");
  oss << "<table><tr><td colspan=\"2\">CPMemLocObject</td></tr>";
  oss << "<tr><td>region:</td><td>"<<(region?region->str():"NULL")<<"</td><tr>";
  if(index) {
    //oss <<", "<<endl<< indent << "             index="<<index->str(indent+"    ");
    oss << "<tr><td>index:</td><td>"<<index->str()<<"</td></tr>";
  }
  //oss <<"]";
  oss << "</table>"<<endl;
  
  return oss.str();
  return "";
}

// **********************************************************************
//            ConstantPropagationAnalysisTransfer
// **********************************************************************

// Transfer function for logical short-circuit operations: && and ||
/*void ConstantPropagationAnalysisTransfer::transferShortCircuitLogical(SgBinaryOp *sgn)
{
  CPValueObjectPtr arg1Lat, arg2Lat, resLat;
  arg1Lat = getLatticeOperand(sgn, sgn->get_lhs_operand());
  resLat  = getLattice(sgn);
  
  // If the first operand of the short-circuit operation has a known value and it is sufficient to determine
  // the operation's outcome
  if(arg1Lat->getLevel() == CPValueObject::constantValue &&
     ((isSgAndOp(sgn) && !arg1Lat->getValue()) ||
      (isSgOrOp(sgn)  &&  arg1Lat->getValue()))) {
    resLat->setValue(arg1Lat->getValue());
  // Otherwise, if the second value needs to be read to determine the operation's outcome
  } else {
    // Case 1: arg1's value was known but not sufficient
    if(arg1Lat->getLevel() == CPValueObject::constantValue) {
      arg2Lat = getLatticeOperand(sgn, sgn->get_rhs_operand());
      if(arg2Lat->getLevel() == CPValueObject::constantValue) {
        resLat->setValue(arg2Lat->getValue());
      // If arg2's value is uninitialized, then any value is valid for the result. Use uninitialized
      } else if(arg2Lat->getLevel() == CPValueObject::emptySet) {
        resLat->setToEmpty();
      // If arg2 may have multiple values, then the result may have multiple values
      } else if(arg2Lat->getLevel() == CPValueObject::fullSet) {
        resLat->setToFull();
      } else assert(0);
    // Else if, arg1's value is uninitialized, then anly value is valid for the result. Use arg2's v
    } else if(arg1Lat->getLevel() == CPValueObject::emptySet) {
      resLat->setToEmpty();
    // If arg1 may have multiple values, then the result may have multiple values
    } else if(arg1Lat->getLevel() == CPValueObject::fullSet) {
      resLat->setToFull();
    } else assert(0);
  }
}*/

void ConstantPropagationAnalysisTransfer::visit(SgVarRefExp *vref) { 
}

void ConstantPropagationAnalysisTransfer::visit(SgDotExp *dot) {
  SIGHT_VERB_DECL(scope, ("ConstantPropagationAnalysisTransfer::visit(SgDotExp *dot)", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  
  /*assert(dfInfo[NULLPartEdge].size()==2);
  
  CodeLocObjectPtr cl = boost::make_shared<CodeLocObject>(getPart(), getCFGNode());
  dbg << "cl="<<cl->str();
  
  AbstractObjectMap* cl2ml = dynamic_cast<AbstractObjectMap*>(dfInfo[NULLPartEdge][1]);
  assert(cl2ml);*/
  
  /*MemLocObjectPtr ml = boost::dynamic_pointer_cast<MemLocObject>(cl2ml->get(cl));//composer->Expr2MemLocSelf(dot->get_lhs_operand(), part->inEdgeFromAny(), analysis);
  assert(ml);
  CPMemLocObjectPtr core = boost::dynamic_pointer_cast<CPMemLocObject>(ml);
  assert(core);*/
  /*
  scope s2("visit");
  dbg << "ml="<<(ml?ml->str():"NULL")<<endl;
  UnionMemLocObjectPtr coreUnion = boost::dynamic_pointer_cast<UnionMemLocObject>(ml);
  dbg << "coreUnion="<<(coreUnion?coreUnion->str():"NULL")<<endl;
  
  const list<MemLocObjectPtr>& coreMLs = coreUnion->getMemLocs();
  CPMemLocObjectPtr core;
  if(coreMLs.size()==1)
    core = boost::dynamic_pointer_cast<CPMemLocObject>(*(coreMLs.begin()));
  else {
    for(list<MemLocObjectPtr>::const_iterator ml=coreMLs.begin(); ml!=coreMLs.end(); ml++) {
      CPMemLocObjectPtr cpML = boost::dynamic_pointer_cast<CPMemLocObject>(*ml);
      if(ml==coreMLs.begin())
        core = boost::dynamic_pointer_cast<CPMemLocObject>(cpML->copyAOType());
      else
        core->meetUpdate(cpML.get(), part->inEdgeFromAny(), analysis->getComposer(), analysis);
    }
  }*/
  /*list<PartEdgePtr> opPartEdges = part->inEdgeFromAny()->getOperandPartEdge(dot, dot->get_lhs_operand());
  CPMemLocObjectPtr core;
  if(opPartEdges.size()==1) {
    PartPtr operandPart = (*opPartEdges.begin())->source();
    dbg << "operandPart="<<operandPart->str()<<endl;
    
    if(operandPart->CFGNodes().size()==1) {
      CFGNode cn = *operandPart->CFGNodes().begin();
      dbg << "cn="<<CFGNode2Str(cn)<<endl;
      
      CodeLocObjectPtr lhsCL = boost::make_shared<CodeLocObject>(operandPart, cn);
      dbg << "lhsCL="<<lhsCL->str()<<endl;
      
      core = boost::dynamic_pointer_cast<CPMemLocObject>(cl2ml->get(lhsCL));
      dbg << "core="<<(core? core->str():"NULL")<<endl;
      assert(core);
    } else assert(0);
  } else assert(0);*/
  
  //MemLocObjectPtr ml = composer->OperandExpr2MemLoc(dot, dot->get_lhs_operand(), part->inEdgeFromAny(), analysis);
  MemLocObjectPtr ml = analysis->OperandExpr2MemLocUse(dot, dot->get_lhs_operand(), part->inEdgeFromAny());
  CombinedMemLocObjectPtr mlUnion = boost::dynamic_pointer_cast<CombinedMemLocObject>(ml);
  assert(mlUnion);
  const std::list<MemLocObjectPtr>& mlVals = mlUnion->getMemLocs();
  assert(mlVals.size()==1);
  CPMemLocObjectPtr core = boost::dynamic_pointer_cast<CPMemLocObject>(*mlVals.begin());
  assert(core);
  
  // Compute the offset into the region of the core MemLoc that results from the dot expression
  CPValueLatticePtr offset = core->getCPIndex()->ground->op(dot, NULLCPValueLattice);
  SIGHT_VERB(dbg << "offset="<<(offset? offset->str(): "NULL")<<endl, 1, constantPropagationAnalysisDebugLevel)
  
  // Update this node's MemLoc to use the same region as core but with the new offset
  
  /*if(dfInfo[NULLPartEdge][1]) delete dfInfo[NULLPartEdge][1];
  dfInfo[NULLPartEdge][1] = new CPMemLocObject(core->getRegion(), offset, dot, part->inEdgeFromAny(), analysis);
  dbg << "dfInfo[1]="<<dfInfo[NULLPartEdge][1]->str()<<endl;*/
  CPMemLocObjectPtr dotML = boost::make_shared<CPMemLocObject>(core->getRegion(), offset->createValueObject(), dot, part->inEdgeFromAny(), analysis);
  SIGHT_VERB(dbg << "dotML="<<(dotML? dotML->str(): "NULL")<<endl, 1, constantPropagationAnalysisDebugLevel)
  //cl2ml->insert(cl, dotML);
  nodeState.addFact(analysis, 0, new CPMemLocObjectNodeFact(dotML));
}

void ConstantPropagationAnalysisTransfer::visit(SgPntrArrRefExp *paRef) { 
  SIGHT_VERB_DECL(scope, ("ConstantPropagationAnalysisTransfer::visit(SgPntrArrRefExp *paRef)", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  
  /*assert(dfInfo[NULLPartEdge].size()==2);
  
  CodeLocObjectPtr cl = boost::make_shared<CodeLocObject>(getPart(), getCFGNode());
  dbg << "cl="<<cl->str();
  
  AbstractObjectMap* cl2ml = dynamic_cast<AbstractObjectMap*>(dfInfo[NULLPartEdge][1]);
  assert(cl2ml);*/
  
  // In expression array[i], the memory location denoted by "array"
  /*list<PartEdgePtr> opPartEdges = part->inEdgeFromAny()->getOperandPartEdge(paRef, paRef->get_lhs_operand());
  CPMemLocObjectPtr core;
  if(opPartEdges.size()==1) {
    PartPtr operandPart = (*opPartEdges.begin())->source();
    dbg << "operandPart="<<operandPart->str()<<endl;
    
    if(operandPart->CFGNodes().size()==1) {
      CFGNode cn = *operandPart->CFGNodes().begin();
      dbg << "cn="<<CFGNode2Str(cn)<<endl;
      
      CodeLocObjectPtr lhsCL = boost::make_shared<CodeLocObject>(operandPart, cn);
      dbg << "lhsCL="<<lhsCL->str()<<endl;
      
      core = boost::dynamic_pointer_cast<CPMemLocObject>(cl2ml->get(lhsCL));
      dbg << "core="<<(core? core->str():"NULL")<<endl;
      assert(core);
    } else assert(0);
  } else assert(0);*/
  //MemLocObjectPtr ml = composer->OperandExpr2MemLoc(paRef, paRef->get_lhs_operand(), part->inEdgeFromAny(), analysis);
  MemLocObjectPtr ml = analysis->OperandExpr2MemLocUse(paRef, paRef->get_lhs_operand(), part->inEdgeFromAny());
  SIGHT_VERB(dbg << "ml="<<(ml? ml->str(): "NULL")<<endl, 1, constantPropagationAnalysisDebugLevel)
  CombinedMemLocObjectPtr mlUnion = boost::dynamic_pointer_cast<CombinedMemLocObject>(ml);
  assert(mlUnion);
  const std::list<MemLocObjectPtr>& mlVals = mlUnion->getMemLocs();
  assert(mlVals.size()==1);
  CPMemLocObjectPtr core = boost::dynamic_pointer_cast<CPMemLocObject>(*mlVals.begin());
  assert(core);
  SIGHT_VERB(dbg << "core="<<(core? core->str(): "NULL")<<endl, 1, constantPropagationAnalysisDebugLevel)

  // In expression array[i], the value location denoted by "i"
  ValueObjectPtr val = composer->OperandExpr2Val(paRef, paRef->get_rhs_operand(), part->inEdgeFromAny(), analysis);
  SIGHT_VERB(dbg << "val="<<val->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
  CombinedValueObjectPtr indexUnion = boost::dynamic_pointer_cast<CombinedValueObject>(val);
  assert(indexUnion);
  const std::list<ValueObjectPtr>& indexVals = indexUnion->getValues();
  assert(indexVals.size()==1);
  CPValueObjectPtr index = boost::dynamic_pointer_cast<CPValueObject>(*indexVals.begin());
  assert(index);
  SIGHT_VERB(dbg << "index="<<index->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
  
  // Compute the offset into the region of the core MemLoc that results from the arraypntr reference expression
  CPValueLatticePtr offset = core->getCPIndex()->ground->op(paRef, index->ground);
  SIGHT_VERB(dbg << "offset="<<offset->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
  
  CPMemLocObjectPtr paRefML = boost::make_shared<CPMemLocObject>(core->getRegion(), offset->createValueObject(), paRef, part->inEdgeFromAny(), analysis);
  SIGHT_VERB(dbg << "paRefML="<<(paRefML? paRefML->str(): "NULL")<<endl, 1, constantPropagationAnalysisDebugLevel)
  //cl2ml->insert(cl, paRefML);
  nodeState.addFact(analysis, 0, new CPMemLocObjectNodeFact(paRefML));
}

void ConstantPropagationAnalysisTransfer::visit(SgBinaryOp *sgn) { 
  SIGHT_VERB_DECL(scope, ("ConstantPropagationAnalysisTransfer::visit(SgBinaryOp)", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  
  // Only bother to consider operators with short-circuiting a the end of the operator so that
  // all of its operands precede the operator
  if((isSgAndOp(sgn) || isSgOrOp(sgn)) && cn.getIndex()!=2) return;
  
  CPValueLatticePtr arg1Lat, arg2Lat;//, resLat_tmp;
  getLattices(sgn, arg1Lat, arg2Lat);//, resLat_tmp);
  CPValueLatticePtr resLat = arg1Lat->op(sgn, arg2Lat);

//prodLat->setToEmpty();
 //dbg << "after op modified="<<modified<<endl;
  setLattice(sgn, resLat);
  if(isSgCompoundAssignOp(sgn))
    setLatticeOperand(sgn, sgn->get_lhs_operand(), resLat->copyCPLat());
  //dbg << "after setLattice modified="<<modified<<endl;
}

// Unary ops that update the operand
void ConstantPropagationAnalysisTransfer::visit(SgMinusMinusOp *sgn) { 
  SIGHT_VERB_DECL(scope, ("ConstantPropagationAnalysisTransfer::visit(SgMinusMinusOp)", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  
  CPValueLatticePtr arg1Lat;//, arg2Lat;//, resLat_tmp;
  getLattices(sgn, arg1Lat);//, arg2Lat);//, resLat_tmp);
  CPValueLatticePtr resLat = arg1Lat->op(sgn);
//prodLat->setToEmpty();
  setLattice(sgn, resLat);
  setLatticeOperand(sgn, sgn->get_operand(), resLat);
}

void ConstantPropagationAnalysisTransfer::visit(SgPlusPlusOp *sgn) { 
  SIGHT_VERB_DECL(scope, ("ConstantPropagationAnalysisTransfer::visit(SgPlusPlusOp)", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  
  CPValueLatticePtr arg1Lat;//, arg2Lat;//, resLat_tmp;
  getLattices(sgn, arg1Lat);//, arg2Lat);//, resLat_tmp);
  CPValueLatticePtr resLat = arg1Lat->op(sgn);
//prodLat->setToEmpty();
  setLattice(sgn, resLat);
  setLatticeOperand(sgn, sgn->get_operand(), resLat);
}

// Unary ops that do not update the operand
// void ConstantPropagationAnalysisTransfer::visit(SgUnaryOp *sgn) { 
//   scope s("ConstantPropagationAnalysisTransfer::visit(SgUnaryOp)", scope::medium, attrGE("constantPropagationAnalysisDebugLevel", 1));
  
//   CPValueLatticePtr arg1Lat;//, arg2Lat;//, resLat_tmp;
//   getLattices(sgn, arg1Lat);//, arg2Lat);//, resLat_tmp);
//   CPValueLatticePtr resLat = arg1Lat->op(sgn);
// //prodLat->setToEmpty();
//   setLattice(sgn, resLat);
// }

// Unary ops that do not update the operand
void ConstantPropagationAnalysisTransfer::visit(SgCastExp *sgn) { 
  SIGHT_VERB_DECL(scope, ("ConstantPropagationAnalysisTransfer::visit(SgCastExp)", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  
  CPValueLatticePtr arg1Lat;//, arg2Lat;//, resLat_tmp;
  getLattices(sgn, arg1Lat);//, arg2Lat);//, resLat_tmp);
  CPValueLatticePtr resLat = arg1Lat->op(sgn);
//prodLat->setToEmpty();
  setLattice(sgn, resLat);
}

// Unary ops that do not update the operand
void ConstantPropagationAnalysisTransfer::visit(SgMinusOp *sgn) { 
  SIGHT_VERB_DECL(scope, ("ConstantPropagationAnalysisTransfer::visit(SgMinusOp)", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  
  CPValueLatticePtr arg1Lat;//, arg2Lat;//, resLat_tmp;
  getLattices(sgn, arg1Lat);//, arg2Lat);//, resLat_tmp);
  CPValueLatticePtr resLat = arg1Lat->op(sgn);
//prodLat->setToEmpty();
  setLattice(sgn, resLat);
}

// Unary ops that do not update the operand
void ConstantPropagationAnalysisTransfer::visit(SgNotOp *sgn) { 
  SIGHT_VERB_DECL(scope, ("ConstantPropagationAnalysisTransfer::visit(SgNotOp)", scope::medium), 1, constantPropagationAnalysisDebugLevel);
  
  CPValueLatticePtr arg1Lat;//, arg2Lat;//, resLat_tmp;
  getLattices(sgn, arg1Lat);//, arg2Lat);//, resLat_tmp);
  CPValueLatticePtr resLat = arg1Lat->op(sgn);
//prodLat->setToEmpty();
  setLattice(sgn, resLat);
}

void ConstantPropagationAnalysisTransfer::visit(SgValueExp *val) {
  SIGHT_VERB(scope reg("ConstantPropagationAnalysisTransfer::visit(SgValExp)", scope::low), 1, constantPropagationAnalysisDebugLevel)
  assert(val);
  
//prodLat->setToEmpty();
  SgTreeCopy copyHelp;
  boost::shared_ptr<SgValueExp> valCopy((SgValueExp*)(val->copy(copyHelp)));
  setLattice(val, boost::make_shared<CPValueLattice>(boost::make_shared<CPConcreteKind>(valCopy), part->inEdgeFromAny()));
}


ConstantPropagationAnalysisTransfer::ConstantPropagationAnalysisTransfer(
          PartPtr part, PartPtr supersetPart, CFGNode cn, NodeState& state, 
          map<PartEdgePtr, vector<Lattice*> >& dfInfo, 
          Composer* composer, ConstantPropagationAnalysis* analysis)
   : VariableStateTransfer<CPValueLattice, ConstantPropagationAnalysis>
                       (state, dfInfo, boost::make_shared<CPValueLattice>(part->inEdgeFromAny()),
                        composer, analysis, part, supersetPart, cn, 
                        constantPropagationAnalysisDebugLevel, "constantPropagationAnalysisDebugLevel")
{
}




// **********************************************************************
//             ConstantPropagationAnalysis
// **********************************************************************

// GB: Is this needed for boost shared-pointers?
ConstantPropagationAnalysis::ConstantPropagationAnalysis(bool useSSA) : FWDataflow(/*trackBase2RefinedPartEdgeMapping*/ false, useSSA)
{
}

// Creates a basic CPMemLocObject for the given SgNode. This object does not take into
// account any constant propagation and will be used as a seed from which to propagate 
// more precise information.
CPMemLocObjectPtr ConstantPropagationAnalysis::createBasicCPML(SgNode* n, PartEdgePtr pedge) {
  //scope s("ConstantPropagationAnalysis::createBasicCPML");
  MemRegionObjectPtr curMR = composer->Expr2MemRegion(n, pedge, this);
  //dbg << "curMR="<<curMR->str()<<endl;
    
  // If this expression denotes the starting point of a memory region, create a MemLocObject
  // that is explicitly at the start of mr
  CPMemLocObjectPtr ml;
  if(isSgVarRefExp(n) || isSgInitializedName(n))
    return boost::make_shared<CPMemLocObject>(
                                curMR, boost::make_shared<CPValueLattice>(
                                           boost::make_shared<CPOffsetListKind>(CPOffsetListKind::offset(0)),
                                           pedge)->createValueObject(),
                                n, pedge, this);
  // Otherwise, create one that refers to an unknown offset within mr
  else
    return boost::make_shared<CPMemLocObject>(
                                curMR, boost::make_shared<CPValueLattice>(
            // !!! Should create ServerImplKind here!!!
                                           boost::make_shared<CPOffsetListKind>(CPOffsetListKind::offset(0)),
                                           pedge)->createValueObject(),
                                       // boost::make_shared<CPValueObject>(boost::make_shared<CPUninitializedKind>(), pedge), 
                                n, pedge, this);
  
}

// Initializes the state of analysis lattices at the given function, part and edge into our out of the part
// by setting initLattices to refer to freshly-allocated Lattice objects.
void ConstantPropagationAnalysis::genInitLattice(PartPtr part, PartEdgePtr pedge, PartPtr supersetPart,
                                                 vector<Lattice*>& initLattices)
{
  SIGHT_VERB_DECL(scope, (txt()<<"ConstantPropagationAnalysis::genInitLattice(part="<<part->str()<<")", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  AbstractObjectMap* ml2val = new AbstractObjectMap(boost::make_shared<CPValueLattice>(pedge),
                                                    pedge,
                                                    getComposer(), this);
  /*dbg << "ConstantPropagationAnalysis::initializeState, analysis="<<returning l="<<l<<" n=<"<<escape(p.getNode()->unparseToString())<<" | "<<p.getNode()->class_name()<<" | "<<p.getIndex()<<">\n";
  dbg << "    l="<<l->str("    ")<<endl;*/
  initLattices.push_back(ml2val);
  
  //initLattices.push_back(new CPMemLocObject(NULLMemRegionObject, NULLCPValueObject, NULL, pedge, this));
  
  /*
  // The second lattice represents the memory location denoted by the current CFGNode
  assert(part->CFGNodes().size()==1);
  CFGNode n = *(part->CFGNodes().begin());
  
  MemRegionObjectPtr mr = composer->Expr2MemRegion(n.getNode(), part->inEdgeFromAny(), this);
  CPMemLocObject* ml;
  scope s("ConstantPropagationAnalysis::genInitLattice()", scope::medium, attrGE("constantPropagationAnalysisDebugLevel", 1));
  
  // If this expression denotes the starting point of a memory region, create a MemLocObject
  // that is explicitly at the start of mr
  if(isSgVarRefExp(n.getNode()))
    ml = new CPMemLocObject(mr, boost::make_shared<CPValueObject>(
                                         boost::make_shared<CPConcreteKind>(
                                                  boost::shared_ptr<SgValueExp>(SageBuilder::buildIntVal(0))), pedge), 
                            n.getNode(), pedge, this);
  // Otherwise, create one that refers to an unknown offset within mr
  else
    ml = new CPMemLocObject(mr, boost::make_shared<CPValueObject>(boost::make_shared<CPUninitializedKind>(), pedge), 
                            n.getNode(), pedge, this);
  
  dbg << "new MemLoc post:"<<ml->str()<<endl;
  initLattices.push_back(ml);*/
  
  /*AbstractObjectMap* cl2ml = new AbstractObjectMap(boost::make_shared<CPMemLocObject>(false, true, (SgNode*)NULL, pedge, this),
                                                   pedge,
                                                   getComposer(), this);
  const set<CFGNode>& nodes = part->CFGNodes();
  for(set<CFGNode>::const_iterator n=nodes.begin(); n!=nodes.end(); n++) {
    CodeLocObjectPtr curCL = boost::make_shared<CodeLocObject>(part, *n);
    cl2ml->insert(curCL, ConstantPropagationAnalysis::createBasicCPML(n->getNode(), part->inEdgeFromAny()));
  }
  
  dbg << "cl2ml:"<<cl2ml->str()<<endl;
  initLattices.push_back(cl2ml);*/
}
  
bool
ConstantPropagationAnalysis::transfer(PartPtr p, CFGNode cn, NodeState& state, 
                                      map<PartEdgePtr, vector<Lattice*> >& dfInfo)
{
  assert(0); 
  return false;
}

boost::shared_ptr<DFTransferVisitor>
ConstantPropagationAnalysis::getTransferVisitor(PartPtr part, PartPtr supersetPart, CFGNode cn, NodeState& state,
                                                map<PartEdgePtr, vector<Lattice*> >& dfInfo)
{
  // Why is the boost shared pointer used here?
  ConstantPropagationAnalysisTransfer* t = new ConstantPropagationAnalysisTransfer(part, supersetPart, cn, state, dfInfo, getComposer(), this);
  return boost::shared_ptr<DFTransferVisitor>(t);
}

ValueObjectPtr ConstantPropagationAnalysis::Expr2Val(SgNode* n, PartEdgePtr pedge)
{
  SIGHT_VERB_DECL(scope, (txt()<<"ConstantPropagationAnalysis::Expr2Val(n="<<SgNode2Str(n)<<", pedge="<<pedge->str()<<")", scope::medium), 1, constantPropagationAnalysisDebugLevel)
  
  //MemLocObjectPtr ml = getComposer()->Expr2MemLoc(n, pedge, this);
  MemLocObjectPtr ml = Expr2MemLocUse(n, pedge);
  SIGHT_VERB(dbg << "ml="<<(ml? ml->str(): "NULL")<<endl, 1, constantPropagationAnalysisDebugLevel)
  
  // If pedge doesn't have wildcards
  dbg << "source="<<pedge->source()->str()<<endl;
  dbg << "target="<<pedge->target()->str()<<endl;
  if(pedge->source() && pedge->target()) {
    // Get the NodeState at the source of this edge
    NodeState* state = NodeState::getNodeState(this, (useSSA? NULLPart: pedge->source()));
    SIGHT_VERB(dbg << "state="<<state->str(this)<<endl, 1, constantPropagationAnalysisDebugLevel)
    
    // Get the value map at the current edge
    AbstractObjectMap* cpMap =
              useSSA? dynamic_cast<AbstractObjectMap*>(state->getLatticeAbove(this, NULLPartEdge, 0)) :
                      dynamic_cast<AbstractObjectMap*>(state->getLatticeBelow(this, pedge->getSupersetPartEdge(), 0));
    if(cpMap == NULL) {
      Lattice* l = useSSA? state->getLatticeBelow(this, NULLPartEdge, 0) :
                           state->getLatticeBelow(this, pedge->getSupersetPartEdge(),0);
      SIGHT_VERB(dbg << "pedge->getSupersetPartEdge()="<<pedge->getSupersetPartEdge()->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
      SIGHT_VERB(dbg << "l="<<(l?l->str():"NULL")<<endl, 1, constantPropagationAnalysisDebugLevel)
    }
    assert(cpMap);
    
    // We currently can only handle requests for the SgNode that corresponds to the current Part
    set<CFGNode> nodes = pedge->source()->CFGNodes();
    assert(nodes.size()==1);
//    assert(nodes.begin()->getNode() == n);
    
    // Get the MemLoc at the source part
    SIGHT_VERB_IF(2, constantPropagationAnalysisDebugLevel)
      indent ind;
      dbg << "cpMap Below="<<cpMap<<"="<<cpMap->str()<<endl;
      dbg << "nodeState = "<<state->str()<<endl;
    SIGHT_VERB_FI()

    // Return the lattice associated with n's expression
    CPValueLatticePtr val = boost::dynamic_pointer_cast<CPValueLattice>(cpMap->get(ml));
    assert(val);
    SIGHT_VERB(dbg << "val="<<val->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
    
    return val->copyCPLat()->createValueObject();
  // If the target of this edge is a wildcard
  } else if(pedge->source()) {
    // Get the NodeState at the source of this edge
    NodeState* state = NodeState::getNodeState(this, pedge->source());
    //dbg << "state="<<state->str(this)<<endl;
        
    map<PartEdgePtr, vector<Lattice*> >& e2lats = state->getLatticeBelowAllMod(this);
    assert(e2lats.size()>=1);
    CPValueLatticePtr mergedLat;
    for(map<PartEdgePtr, vector<Lattice*> >::iterator lats=e2lats.begin(); lats!=e2lats.end(); lats++) {
      PartEdgePtr supersetEdge = lats->first;
      assert(supersetEdge->source() == pedge->getSupersetPartEdge()->source());
      SIGHT_VERB_DECL(scope, (txt()<<"edge "<<lats->first.get()->str(), scope::medium), 1, constantPropagationAnalysisDebugLevel)
      
      // Get the value map at the current edge
      AbstractObjectMap* cpMap = dynamic_cast<AbstractObjectMap*>(state->getLatticeBelow(this, lats->first, 0));
      assert(cpMap);
      
      //MemLocObjectPtr p = composer->Expr2MemLoc(n, pedge, this);
      // We currently can only handle requests for the SgNode that corresponds to the current Part
      set<CFGNode> nodes = pedge->source()->CFGNodes();
      assert(nodes.size()==1);
      assert(nodes.begin()->getNode() == n);
      
      SIGHT_VERB_IF(2, constantPropagationAnalysisDebugLevel)
        indent ind;
        dbg << "cpMap="<<cpMap<<"="<<cpMap->str()<<endl;
      SIGHT_VERB_FI() 
      
      CPValueLatticePtr val = boost::dynamic_pointer_cast<CPValueLattice> (boost::dynamic_pointer_cast<ValueObject>(cpMap->get(ml)));
      SIGHT_VERB(dbg << "val="<<val->str()<<endl, 1, constantPropagationAnalysisDebugLevel)

      if(lats==e2lats.begin())
        mergedLat = val->copyCPLat();
      else 
        mergedLat->meetUpdate(val.get());
      
      SIGHT_VERB(dbg << "mergedLat="<<mergedLat->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
    }
    return mergedLat->createValueObject();
  
  // If the source of this edge is a wildcard
  } else if(pedge->target()) {
    // Get the NodeState at the target of this edge
    NodeState* state = NodeState::getNodeState(this, pedge->target());
    SIGHT_VERB(dbg << "state="<<state->str()<<endl, 2, constantPropagationAnalysisDebugLevel)
    
    // Get the value map at the NULL edge, which denotes the meet over all incoming edges
    AbstractObjectMap* cpMap = dynamic_cast<AbstractObjectMap*>(state->getLatticeAbove(this, pedge->getSupersetPartEdge(), 0));
    assert(cpMap);
    
    SIGHT_VERB_IF(2, constantPropagationAnalysisDebugLevel)
      indent ind;
      dbg << "cpMap="<<cpMap<<"="<<cpMap->str()<<endl;
    SIGHT_VERB_FI()

    // Return the lattice associated with n's expression since that is likely to be more precise
    CPValueLatticePtr val = boost::dynamic_pointer_cast<CPValueLattice>(cpMap->get(ml));
    assert(val);
    SIGHT_VERB(dbg << "val="<<val->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
    
    return val->copyCPLat()->createValueObject();
  }
  assert(0);
}

MemLocObjectPtr ConstantPropagationAnalysis::Expr2MemLoc(SgNode* n, PartEdgePtr pedge) {
  SIGHT_VERB_DECL(scope, (txt()<<"ConstantPropagationAnalysis::Expr2MemLoc(n="<<SgNode2Str(n)<<", pedge="<<pedge->str()<<")", scope::medium), 1, constantPropagationAnalysisDebugLevel)
 
  // SgInitializedNames denote entities that are lexically known and thus do not require
  // any special handling by ConstantPropagation Analysis
  //if(isSgInitializedName(n) || isSgVarRefExp(n)) {
  if(!isSgDotExp(n) && !isSgPntrArrRefExp(n)) {
    SIGHT_VERB(dbg << "Creating basic CPML"<<endl, 1, constantPropagationAnalysisDebugLevel)
    MemLocObjectPtr ret = createBasicCPML(n, pedge);
    SIGHT_VERB(dbg << "ret = "<<ret->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
    return ret;
  }
  
  // NOTE: this is a temporary hack where we assume the appropriate index for the CFGNode
  //       that represents SgNode n. In the future we should change Expr2* to accept CFGNodes
  CFGNode cn;
       if(isSgBinaryOp(n) || 
          isSgUnaryOp(n))     cn = CFGNode(n, 2);
  else if(isSgValueExp(n))    cn = CFGNode(n, 1);
  else                        cn = CFGNode(n, 0);
  
  // Confirm that n corresponds to the source part
  if(pedge->source()) {
    assert(pedge->source()->CFGNodes().size()==1);
    //assert(pedge->source()->CFGNodes().begin()->getNode() == n);
  } else if(pedge->target()) {
    assert(pedge->target()->CFGNodes().size()==1);
    assert(pedge->target()->CFGNodes().begin()->getNode() == n);
  }
  
  if(pedge->source()) {
    SIGHT_VERB_DECL(scope, (txt()<<"Source: "<<pedge->source()->str(), scope::medium), 2, constantPropagationAnalysisDebugLevel)
    NodeState* state = NodeState::getNodeState(this, pedge->source());
    SIGHT_VERB(dbg << "state="<<state->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
  }
  
  if(pedge->target()) {
    SIGHT_VERB_DECL(scope, (txt()<<"target: "<<pedge->target()->str(), scope::medium), 2, constantPropagationAnalysisDebugLevel)
    NodeState* state = NodeState::getNodeState(this, pedge->target());
    SIGHT_VERB(dbg << "state="<<state->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
  }
  
  // If pedge doesn't have wildcards
  if(pedge->source() && pedge->target()) {
    /*CodeLocObjectPtr cl = boost::make_shared<CodeLocObject>(pedge->source(), cn);
    SIGHT_VERB(dbg << "cl="<<cl->str()<<endl, 1, constantPropagationAnalysisDebugLevel)*/
    
    // Get the NodeState at the source of this edge
    NodeState* state = NodeState::getNodeState(this, pedge->source());
    SIGHT_VERB(dbg << "state="<<state->str()<<endl, 3, constantPropagationAnalysisDebugLevel)
    
    /* // Get the memory location at the current edge
    AbstractObjectMap* cl2ml = dynamic_cast<AbstractObjectMap*>(state->getLatticeBelow(this, pedge->getSupersetPartEdge(), 1));
    assert(cl2ml);
    
    // Get the memory location at the current edge
    CPMemLocObjectPtr ml = boost::dynamic_pointer_cast<CPMemLocObject>(cl2ml->get(cl));*/
    NodeFact* mlFact = state->getFact(this, 0);
    CPMemLocObjectNodeFact* cpmlFact = dynamic_cast<CPMemLocObjectNodeFact*>(mlFact);
    assert(cpmlFact);
    CPMemLocObjectPtr ml = cpmlFact->ml;
    assert(ml);
    SIGHT_VERB(dbg << "ml="<<ml->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
    
    return ml->copyAOType();
  // If the target of this edge is a wildcard
  } else if(pedge->source()) {
    // Get the NodeState at the source of this edge
    NodeState* state = NodeState::getNodeState(this, pedge->source());
    SIGHT_VERB(dbg << "state="<<state->str()<<endl, 2, constantPropagationAnalysisDebugLevel)
    
    map<PartEdgePtr, vector<Lattice*> >& e2lats = state->getLatticeBelowAllMod(this);
    assert(e2lats.size()>=1);
    CPMemLocObjectPtr mergedML;
    for(map<PartEdgePtr, vector<Lattice*> >::iterator lats=e2lats.begin(); lats!=e2lats.end(); lats++) {
      SIGHT_VERB_DECL(scope, (txt()<<"edge "<<lats->first.get()->str(), scope::medium), 1, constantPropagationAnalysisDebugLevel)
      PartEdgePtr supersetEdge = lats->first;
      assert(supersetEdge.get()->source() == pedge->getSupersetPartEdge()->source());
      
      // NOTE: for now we're assuming that the CFGNode index is 0 but this will need to be corrected
      /*CodeLocObjectPtr cl = boost::make_shared<CodeLocObject>(pedge->source(), cn);
      SIGHT_VERB(dbg << "cl="<<cl->str(), 1, constantPropagationAnalysisDebugLevel)*/
      
      // Get the memory location at the current edge
      /*AbstractObjectMap* cl2ml = dynamic_cast<AbstractObjectMap*>(state->getLatticeBelow(this, supersetEdge, 1));
      assert(cl2ml);
    
      // Get the memory location at the current edge
      CPMemLocObjectPtr ml = boost::dynamic_pointer_cast<CPMemLocObject>(cl2ml->get(cl));*/
      NodeFact* mlFact = state->getFact(this, 0);
      CPMemLocObjectNodeFact* cpmlFact = dynamic_cast<CPMemLocObjectNodeFact*>(mlFact);
      assert(cpmlFact);
      CPMemLocObjectPtr ml = cpmlFact->ml;
      assert(ml);
      SIGHT_VERB(dbg << "ml="<<ml->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
      
      if(lats==e2lats.begin())
        mergedML = boost::dynamic_pointer_cast<CPMemLocObject>(ml->copyAOType());
      else
        mergedML->meetUpdate((MemLocObjectPtr)ml, supersetEdge, getComposer(), this);
      
      SIGHT_VERB(dbg << "mergedML="<<mergedML->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
    }
    return mergedML;
  // If the source of this edge is a wildcard
  } else if(pedge->target()) {
    // Get the NodeState at the target of this edge
    NodeState* state = NodeState::getNodeState(this, pedge->target());
    SIGHT_VERB(dbg << "state="<<state->str()<<endl, 3, constantPropagationAnalysisDebugLevel)
    
    // NOTE: for now we're assuming that the CFGNode index is 0 but this will need to be corrected
    /*CodeLocObjectPtr cl = boost::make_shared<CodeLocObject>(pedge->target(), cn);
    SIGHT_VERB(dbg << "cl="<<cl->str(), 1, constantPropagationAnalysisDebugLevel)*/
    
    // Get the memory location at the current edge
    /*AbstractObjectMap* cl2ml = dynamic_cast<AbstractObjectMap*>(state->getLatticeAbove(this, NULLPartEdge, 1));
    assert(cl2ml);
    
    // Get the memory location at the current edge
    CPMemLocObjectPtr ml = boost::dynamic_pointer_cast<CPMemLocObject>(cl2ml->get(cl));*/
    NodeFact* mlFact = state->getFact(this, 0);
    CPMemLocObjectNodeFact* cpmlFact = dynamic_cast<CPMemLocObjectNodeFact*>(mlFact);
    assert(cpmlFact);
    CPMemLocObjectPtr ml = cpmlFact->ml;
    assert(ml);
    SIGHT_VERB(dbg << "ml="<<ml->str()<<endl, 1, constantPropagationAnalysisDebugLevel)
    
    return ml->copyAOType();
  }
  
  // If pedge doesn't have wildcards
  /*if(pedge->source() && pedge->target()) {
    // Confirm that n corresponds to the source part
    assert(pedge->source()->CFGNodes().size()==1);
    assert(pedge->source()->CFGNodes().begin()->getNode() == n);
    
    // Get the NodeState at the source of this edge
    NodeState* state = NodeState::getNodeState(this, pedge->source());
    
    // Get the memory location at the current edge
    CPMemLocObject* ml = dynamic_cast<CPMemLocObject*>(state->getLatticeBelow(this, pedge->getSupersetPartEdge(), 1));
    if(ml==NULL) { Lattice* l = state->getLatticeBelow(this, pedge->getSupersetPartEdge(), 1);dbg << "ml="<<(l? l->str(): "NULL")<<endl; }
    assert(ml);
    
    if(constantPropagationAnalysisDebugLevel()>=1) dbg << "ml="<<ml->str()<<endl;
    
    return ml->copyAOType();
  // If the target of this edge is a wildcard
  } else if(pedge->source()) {
    // Get the NodeState at the source of this edge
    NodeState* state = NodeState::getNodeState(this, pedge->source());
    
    map<PartEdgePtr, vector<Lattice*> >& e2lats = state->getLatticeBelowAllMod(this);
    assert(e2lats.size()>=1);
    CPMemLocObjectPtr mergedML;
    for(map<PartEdgePtr, vector<Lattice*> >::iterator lats=e2lats.begin(); lats!=e2lats.end(); lats++) {
      scope sEdge(txt()<<"edge "<<lats->first.get()->str(), scope::medium, attrGE("constantPropagationAnalysisDebugLevel", 1));
      PartEdge* edgePtr = lats->first.get();
      assert(edgePtr->source() == pedge.get()->source());
      
      // Confirm that n corresponds to the source part
      assert(pedge->source()->CFGNodes().size()==1);
      assert(pedge->source()->CFGNodes().begin()->getNode() == n);
      
      // Get the value map at the current edge
      CPMemLocObject* ml = dynamic_cast<CPMemLocObject*>(state->getLatticeBelow(this, lats->first, 1));
      if(ml==NULL) { Lattice* l = state->getLatticeBelow(this, pedge, 1);dbg << "ml="<<(l? l->str(): "NULL")<<endl; }
      assert(ml);
      
      if(constantPropagationAnalysisDebugLevel()>=1) dbg << "ml="<<ml->str()<<endl;
      
      if(lats==e2lats.begin())
        mergedML = boost::dynamic_pointer_cast<CPMemLocObject>(ml->copyAOType());
      else
        mergedML->meetUpdate(ml, lats->first, getComposer(), this);
      
      if(constantPropagationAnalysisDebugLevel()>=1) dbg << "mergedML="<<mergedML->str()<<endl;
    }
    return mergedML;
  
  // If the source of this edge is a wildcard
  } else if(pedge->target()) {
    // Get the NodeState at the target of this edge
    NodeState* state = NodeState::getNodeState(this, pedge->target());
    if(constantPropagationAnalysisDebugLevel()>=2) dbg << "state="<<state->str()<<endl;
    
    // Get the value map at the current edge
    CPMemLocObject* ml = dynamic_cast<CPMemLocObject*>(state->getLatticeAbove(this, NULLPartEdge, 1));
    if(ml==NULL) { Lattice* l = state->getLatticeAbove(this, NULLPartEdge, 1);dbg << "ml="<<(l? l->str(): "NULL")<<endl; }
    assert(ml);
    
    if(constantPropagationAnalysisDebugLevel()>=1) dbg << "ml="<<ml->str()<<endl;
    
    return ml->copyAOType();
  }*/
  assert(0);
}

}; // namespace fuse;
