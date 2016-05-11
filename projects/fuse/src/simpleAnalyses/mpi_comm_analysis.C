/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

#include "mpi.h"
#include "sage3basic.h"
#include "mpi_comm_analysis.h"
#include "serialization_impl.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "serialization_exports.h"

using namespace std;
using namespace sight;

namespace fuse {

  DEBUG_LEVEL(mpiCommAnalysisDebugLevel, 0);

  /********************
   * MPICommValueKind *
   ********************/
  MPICommValueKind::MPICommValueKind(ValueKind kind) : kind(kind) { }
  
  MPICommValueKind::ValueKind MPICommValueKind::getKindType() const {
    return kind;
  }

  bool MPICommValueKind::isEmptyK() {
    return (kind == bottom);
  }

  bool MPICommValueKind::isFullK() {
    return (kind == unknown);
  }

  /***************************
   * MPICommValueDefaultKind *
   ***************************/
  MPICommValueDefaultKind::MPICommValueDefaultKind() : MPICommValueKind(MPICommValueKind::bottom) {}
  MPICommValueDefaultKind::MPICommValueDefaultKind(const MPICommValueDefaultKind& that) : MPICommValueKind(that) { }

  MPICommValueKindPtr MPICommValueDefaultKind::copyK() {
    return boost::make_shared<MPICommValueDefaultKind>(*this);
  }

  bool MPICommValueDefaultKind::mayEqualK(MPICommValueKindPtr thatK) {
    return thatK->getKindType() == MPICommValueKind::bottom;
  }

  bool MPICommValueDefaultKind::mustEqualK(MPICommValueKindPtr thatK) {
    return false;
  }

  bool MPICommValueDefaultKind::equalSetK(MPICommValueKindPtr thatK) {
    return thatK->getKindType() == MPICommValueKind::bottom;
  }

  bool MPICommValueDefaultKind::subSetK(MPICommValueKindPtr thatK) {
    return thatK->getKindType() == MPICommValueKind::bottom;
  }

  string MPICommValueDefaultKind::str(string indent) const {
    return "default";
  }

  /*****************
   * ConcreteValue *
   *****************/
  ConcreteValue::ConcreteValue() { }
  ConcreteValue::ConcreteValue(const ConcreteValue& that) { }

  /************************
   * IntegerConcreteValue *
   ************************/
  IntegerConcreteValue::IntegerConcreteValue(int val)
    : ConcreteValue() {
    value = boost::shared_ptr<SgIntVal>(SageBuilder::buildIntVal(val));
  }

  IntegerConcreteValue::IntegerConcreteValue(const IntegerConcreteValue& that)
    : ConcreteValue(that),
      value(that.value) { }

  int IntegerConcreteValue::get_value() const {
    return value->get_value();
  }

  bool IntegerConcreteValue::operator<(const ConcreteValuePtr& that) const {
    IntegerConcreteValuePtr thatV = dynamicConstPtrCast<IntegerConcreteValue>(that);
    assert(thatV);
    return value->get_value() < thatV->get_value();
  }

  bool IntegerConcreteValue::operator==(const ConcreteValuePtr& that) const {
    IntegerConcreteValuePtr thatV = dynamicConstPtrCast<IntegerConcreteValue>(that);
    assert(thatV);
    return value->get_value() == thatV->get_value();
  }

  bool IntegerConcreteValue::operator!=(const ConcreteValuePtr& that) const {
    IntegerConcreteValuePtr thatV = dynamicConstPtrCast<IntegerConcreteValue>(that);
    assert(thatV);
    return value->get_value() != thatV->get_value();
  }

  SgType* IntegerConcreteValue::getConcreteType() const {
    return value->get_type();
  }

  SgValueExpPtr IntegerConcreteValue::getConcreteValue() const {
    return boost::dynamic_pointer_cast<SgValueExp>(value);
  }

  ConcreteValue* IntegerConcreteValue::copy() const {
    return new IntegerConcreteValue(*this);
  }

  string IntegerConcreteValue::str(string indent) const {
    ostringstream oss;
    oss << value->get_value();
    return oss.str();
  }

  /************************
   * LongIntConcreteValue *
   ************************/
  LongIntConcreteValue::LongIntConcreteValue(long val)
    : ConcreteValue() {
    value = boost::shared_ptr<SgLongIntVal>(SageBuilder::buildLongIntVal(val));
  }

  LongIntConcreteValue::LongIntConcreteValue(const LongIntConcreteValue& that)
    : ConcreteValue(that),
      value(that.value) { }

  long LongIntConcreteValue::get_value() const {
    return value->get_value();
  }

  bool LongIntConcreteValue::operator<(const ConcreteValuePtr& that) const {
    LongIntConcreteValuePtr thatV = dynamicConstPtrCast<LongIntConcreteValue>(that);
    assert(thatV);
    return value->get_value() < thatV->get_value();
  }

  bool LongIntConcreteValue::operator==(const ConcreteValuePtr& that) const {
    LongIntConcreteValuePtr thatV = dynamicConstPtrCast<LongIntConcreteValue>(that);
    assert(thatV);
    return value->get_value() == thatV->get_value();
  }

  bool LongIntConcreteValue::operator!=(const ConcreteValuePtr& that) const {
    LongIntConcreteValuePtr thatV = dynamicConstPtrCast<LongIntConcreteValue>(that);
    assert(thatV);
    return value->get_value() != thatV->get_value();
  }

  SgType* LongIntConcreteValue::getConcreteType() const {
    return value->get_type();
  }

  SgValueExpPtr LongIntConcreteValue::getConcreteValue() const {
    return boost::dynamic_pointer_cast<SgValueExp>(value);
  }

  ConcreteValue* LongIntConcreteValue::copy() const {
    return new LongIntConcreteValue(*this);
  }

  string LongIntConcreteValue::str(string indent) const {
    ostringstream oss;
    oss << value->get_value();
    return oss.str();
  }

  /****************************
   * LongLongIntConcreteValue *
   ****************************/
  LongLongIntConcreteValue::LongLongIntConcreteValue(long val)
    : ConcreteValue() {
    value = boost::shared_ptr<SgLongLongIntVal>(SageBuilder::buildLongLongIntVal(val));
  }

  LongLongIntConcreteValue::LongLongIntConcreteValue(const LongLongIntConcreteValue& that)
    : ConcreteValue(that),
      value(that.value) { }

  long long LongLongIntConcreteValue::get_value() const {
    return value->get_value();
  }

  bool LongLongIntConcreteValue::operator<(const ConcreteValuePtr& that) const {
    LongLongIntConcreteValuePtr thatV = dynamicConstPtrCast<LongLongIntConcreteValue>(that);
    assert(thatV);
    return value->get_value() < thatV->get_value();
  }

  bool LongLongIntConcreteValue::operator==(const ConcreteValuePtr& that) const {
    LongLongIntConcreteValuePtr thatV = dynamicConstPtrCast<LongLongIntConcreteValue>(that);
    assert(thatV);
    return value->get_value() == thatV->get_value();
  }

  bool LongLongIntConcreteValue::operator!=(const ConcreteValuePtr& that) const {
    LongLongIntConcreteValuePtr thatV = dynamicConstPtrCast<LongLongIntConcreteValue>(that);
    assert(thatV);
    return value->get_value() != thatV->get_value();
  }

  SgType* LongLongIntConcreteValue::getConcreteType() const {
    return value->get_type();
  }

  SgValueExpPtr LongLongIntConcreteValue::getConcreteValue() const {
    return boost::dynamic_pointer_cast<SgValueExp>(value);
  }

  ConcreteValue* LongLongIntConcreteValue::copy() const {
    return new LongLongIntConcreteValue(*this);
  }

  string LongLongIntConcreteValue::str(string indent) const {
    ostringstream oss;
    oss << value->get_value();
    return oss.str();
  }

  /***********************
   * StringConcreteValue *
   ***********************/
  StringConcreteValue::StringConcreteValue(string val)
    : ConcreteValue() {
    value = boost::shared_ptr<SgStringVal>(SageBuilder::buildStringVal(val));
  }

  StringConcreteValue::StringConcreteValue(const StringConcreteValue& that)
    : ConcreteValue(that),
      value(that.value) { }
  
  string StringConcreteValue::get_value() const {
    return value->get_value();
  }

  bool StringConcreteValue::operator<(const ConcreteValuePtr& that) const {
    StringConcreteValuePtr thatV = dynamicConstPtrCast<StringConcreteValue>(that);
    assert(thatV);
    return (value->get_value().compare(thatV->get_value()) < 0);
  }

  bool StringConcreteValue::operator==(const ConcreteValuePtr& that) const {
    StringConcreteValuePtr thatV = dynamicConstPtrCast<StringConcreteValue>(that);
    assert(thatV);
    return (value->get_value().compare(thatV->get_value()) == 0);
  }

  bool StringConcreteValue::operator!=(const ConcreteValuePtr& that) const {
    StringConcreteValuePtr thatV = dynamicConstPtrCast<StringConcreteValue>(that);
    assert(thatV);
    return (value->get_value().compare(thatV->get_value()) > 0);
  }

  SgType* StringConcreteValue::getConcreteType() const {
    return value->get_type();
  }

  SgValueExpPtr StringConcreteValue::getConcreteValue() const {
    return boost::dynamic_pointer_cast<SgValueExp>(value);
  }

  ConcreteValue* StringConcreteValue::copy() const {
    return new StringConcreteValue(*this);
  }

  string StringConcreteValue::str(string indent) const {
    ostringstream oss;
    oss << value->get_value();
    return oss.str();
  }
    
  /*********************
   * Utility Functions *
   *********************/
  ConcreteValuePtr makeConcreteValue(const SgValueExpPtr sgvalue) {
    switch(sgvalue->variantT()) {
    case V_SgIntVal: 
      {
        boost::shared_ptr<SgIntVal> ivalue = boost::dynamic_pointer_cast<SgIntVal>(sgvalue);
        return boost::make_shared<IntegerConcreteValue>(ivalue->get_value());
      }
    case V_SgLongIntVal:
      {
        boost::shared_ptr<SgLongIntVal> ivalue = boost::dynamic_pointer_cast<SgLongIntVal>(sgvalue);
        return boost::make_shared<LongIntConcreteValue>(ivalue->get_value());
      }
    case V_SgLongLongIntVal:
      {
        boost::shared_ptr<SgLongLongIntVal> ivalue = boost::dynamic_pointer_cast<SgLongLongIntVal>(sgvalue);
        return boost::make_shared<LongLongIntConcreteValue>(ivalue->get_value());
      }
    case V_SgStringVal:
      {
        boost::shared_ptr<SgStringVal> svalue = boost::dynamic_pointer_cast<SgStringVal>(sgvalue);
        return boost::make_shared<StringConcreteValue>(svalue->get_value());
      }      
    default:
      dbg << "sgvalue=" << SgNode2Str(sgvalue.get()) << endl;
      assert(false);
    }
  }
  
  /****************************
   * MPICommValueConcreteKind *
   ****************************/
  MPICommValueConcreteKind::MPICommValueConcreteKind(const SgValueExpPtrSet& valueExpSet) :
    MPICommValueKind(MPICommValueKind::concrete) {
    SgValueExpPtrSet::const_iterator c = valueExpSet.begin();
    for( ; c != valueExpSet.end(); ++c) {
      ConcreteValuePtr cval = makeConcreteValue(*c);
      concreteValues.insert(cval);
    }
  }

  // do we need a deep copy here?
  MPICommValueConcreteKind::MPICommValueConcreteKind(const MPICommValueConcreteKind& that) :
    MPICommValueKind(that), concreteValues(that.concreteValues) {
  }

  MPICommValueConcreteKind::MPICommValueConcreteKind(const std::set<ConcreteValuePtr>& thatConcreteValues) :
    MPICommValueKind(MPICommValueKind::concrete), concreteValues(thatConcreteValues) {
  }

  MPICommValueConcreteKind::~MPICommValueConcreteKind() {
  }

  MPICommValueKindPtr MPICommValueConcreteKind::copyK() {
    return boost::make_shared<MPICommValueConcreteKind>(*this);
  }

  SgType* MPICommValueConcreteKind::getConcreteType() const {
    assert(concreteValues.size() > 0);
    set<ConcreteValuePtr>::iterator c = concreteValues.begin();
    SgType* type = (*c)->getConcreteType(); ++c;
    for( ; c != concreteValues.end(); ++c) {
      assert(type == (*c)->getConcreteType());
    }
    return type;
  }

  SgValueExpPtrSet MPICommValueConcreteKind::getConcreteValue() const {
    SgValueExpPtrSet cvalSet;
    set<ConcreteValuePtr>::const_iterator cIt = concreteValues.begin();
    for( ; cIt != concreteValues.end(); ++cIt) {
      ConcreteValuePtr cval = *cIt;
      SgValueExpPtr sgValExp = cval->getConcreteValue();
      cvalSet.insert(sgValExp);
    }
    return cvalSet;
  }

  set<ConcreteValuePtr> MPICommValueConcreteKind::getConcreteValuePtrSet() const {
    return concreteValues;
  }

  bool MPICommValueConcreteKind::set_intersect(const set<ConcreteValuePtr>& setone, 
                                               const set<ConcreteValuePtr>& settwo) const {
    set<ConcreteValuePtr>::const_iterator oi = setone.begin(), ti = settwo.begin();
    while(oi != setone.end() && ti != settwo.end()) {
      if(*oi < *ti) ++oi;
      else if(*ti < *oi) ++ti;
      // we found one element that is common in between them
      else  return true;
    }
    return false;
  }

  bool MPICommValueConcreteKind::set_equal(const set<ConcreteValuePtr>& setone,
                                           const set<ConcreteValuePtr>& settwo) const {
    set<ConcreteValuePtr>::const_iterator oi = setone.begin(), ti = settwo.begin();
    for( ; oi != setone.end() && ti != settwo.end(); ++oi, ++ti) {
      if(*oi != *ti) return false;
    }
    return true;
  }


  bool MPICommValueConcreteKind::set_subset(const set<ConcreteValuePtr>& setone,
                                            const set<ConcreteValuePtr>& settwo) const {
    set<ConcreteValuePtr>::const_iterator oi = setone.begin(), ti = settwo.begin();

    while(ti != settwo.end()) {
      if(*ti < *oi) ++ti;
      else if(*oi == *ti) {
        ++ti; ++oi;
      }
      // element not found
      else return false;
    }
    // Left over elements not found
    if(oi != setone.end()) return false;
    return true;
  }

  bool MPICommValueConcreteKind::set_union(set<ConcreteValuePtr>& setone,
                                           const set<ConcreteValuePtr>& settwo) {
    set<ConcreteValuePtr>::const_iterator oi = setone.begin(), ti = settwo.begin();
    bool modified = false;
    if(oi == setone.end()) {
      setone.insert(settwo.begin(), settwo.end());
      modified = true;
    }

    while(oi != setone.end() && ti != settwo.end()) {
      if(*ti < *oi) {
        oi = setone.insert(oi, *ti);
        ++oi; ++ti;
        modified = modified || true;
      }
      else if(*oi == *ti) {
        ++oi; ++ti;
      }
      // *oi < *ti
      else {
        ++oi;
      }
    }
    if(ti != settwo.end()) {
      setone.insert(ti, settwo.end());
      modified = modified || true;
    }
    return modified;
  }

  bool MPICommValueConcreteKind::mayEqualK(MPICommValueKindPtr thatK) {
    if(thatK->getKindType() == MPICommValueKind::bottom) return false;
    else if(thatK->getKindType() == MPICommValueKind::unknown) return true;
    else {
      // both are concrete
      MPICommValueConcreteKindPtr thatCK = boost::dynamic_pointer_cast<MPICommValueConcreteKind>(thatK);
      assert(thatCK);
      set<ConcreteValuePtr> thatCKSet = thatCK->getConcreteValuePtrSet();
      return set_intersect(concreteValues, thatCKSet);
    }    
  }

  bool MPICommValueConcreteKind::mustEqualK(MPICommValueKindPtr thatK) {
    if(thatK->getKindType() == MPICommValueKind::unknown ||
       thatK->getKindType() == MPICommValueKind::bottom) return false;
    MPICommValueConcreteKindPtr thatCK = boost::dynamic_pointer_cast<MPICommValueConcreteKind>(thatK);
    assert(thatCK);
    set<ConcreteValuePtr> thatCKSet = thatCK->getConcreteValuePtrSet();
    if(concreteValues.size() != 1 || thatCKSet.size() != 1) return false;
    return set_equal(concreteValues, thatCKSet);
  }
  
  bool MPICommValueConcreteKind::equalSetK(MPICommValueKindPtr thatK) {
    if(thatK->getKindType() == MPICommValueKind::unknown ||
       thatK->getKindType() == MPICommValueKind::bottom) return false;
    MPICommValueConcreteKindPtr thatCK = boost::dynamic_pointer_cast<MPICommValueConcreteKind>(thatK);
    assert(thatCK);
    return set_equal(concreteValues, thatCK->getConcreteValuePtrSet());
  }
  
  bool MPICommValueConcreteKind::subSetK(MPICommValueKindPtr thatK) {
    if(thatK->getKindType() == MPICommValueKind::unknown) return true;
    else if(thatK->getKindType() == MPICommValueKind::bottom) return false;
    else {
      MPICommValueConcreteKindPtr thatCK = boost::dynamic_pointer_cast<MPICommValueConcreteKind>(thatK);
      assert(thatCK);
      return set_subset(concreteValues, thatCK->getConcreteValuePtrSet());
    }
  }

  bool MPICommValueConcreteKind::unionConcreteValues(MPICommValueConcreteKindPtr thatCK) {
    return set_union(concreteValues, thatCK->getConcreteValuePtrSet());
  }

  string MPICommValueConcreteKind::str(string indent) const {
    ostringstream oss;
    oss << "concrete: [";
    set<ConcreteValuePtr>::const_iterator cit = concreteValues.begin();
    while(cit != concreteValues.end()) {
      oss << (*cit)->str();
      oss << ", cval.addr=" << (*cit).get();
      ++cit;
      if(cit != concreteValues.end()) {
        oss << ", ";
      }   
    }
    oss << "]";
    return oss.str();
  }


  /***************************
   * MPICommValueUnknownKind *
   ***************************/
  MPICommValueUnknownKind::MPICommValueUnknownKind() : MPICommValueKind(MPICommValueKind::unknown) { }
  MPICommValueUnknownKind::MPICommValueUnknownKind(const MPICommValueUnknownKind& that) : MPICommValueKind(that) { }

  MPICommValueKindPtr MPICommValueUnknownKind::copyK() {
    return boost::make_shared<MPICommValueUnknownKind>(*this);
  }

  bool MPICommValueUnknownKind::mayEqualK(MPICommValueKindPtr thatK) {
    return true;
  }

  bool MPICommValueUnknownKind::mustEqualK(MPICommValueKindPtr thatK) {
    return false;
  }

  bool MPICommValueUnknownKind::equalSetK(MPICommValueKindPtr thatK) {
    if(thatK->getKindType() != MPICommValueKind::unknown) return false;
    else return true;
  }

  bool MPICommValueUnknownKind::subSetK(MPICommValueKindPtr thatK) {
    if(thatK->getKindType() != MPICommValueKind::unknown) return false;
    else return true;
  }

  string MPICommValueUnknownKind::str(string indent) const {
    return "unknown";
  }

  /******************
   * Helper Methods *
   ******************/

  MPICommValueKindPtr createMPICommValueKind(ValueObjectPtr vo, PartEdgePtr pedge) {
    if(vo->isConcrete()) {
      SgValueExpPtrSet cvalues = vo->getConcreteValue();
      return boost::make_shared<MPICommValueConcreteKind>(cvalues);
    }
    else if(vo->isEmptyV(pedge)) {
      return boost::make_shared<MPICommValueDefaultKind>();
    }
    else if(vo->isFullV(pedge)) {
      return boost::make_shared<MPICommValueUnknownKind>();
    }
  }

  /**********************
   * MPICommValueObject *
   **********************/
  MPICommValueObject::MPICommValueObject(PartEdgePtr pedge) 
    : Lattice(pedge),
      FiniteLattice(pedge),
      ValueObject(NULL) {
    kind = boost::make_shared<MPICommValueDefaultKind>();
  }

  MPICommValueObject::MPICommValueObject(MPICommValueKindPtr thatK, PartEdgePtr pedge)
    : Lattice(pedge),
      FiniteLattice(pedge),
      ValueObject(0),
      kind(thatK) {

  }

  MPICommValueObject::MPICommValueObject(ValueObjectPtr that, PartEdgePtr pedge)
    : Lattice(pedge),
      FiniteLattice(pedge),
      ValueObject(0) {
    if(that->isEmptyV(pedge)) {
      kind = boost::make_shared<MPICommValueDefaultKind>();
    }
    else if(that->isFullV(pedge)) {
      kind = boost::make_shared<MPICommValueUnknownKind>();
    }
    else if(that->isConcrete()) {
      kind = boost::make_shared<MPICommValueConcreteKind>(that->getConcreteValue());
    }
  }
      
  MPICommValueObject::MPICommValueObject(const MPICommValueObject& that)
    : Lattice(that),
      FiniteLattice(that),
      ValueObject(that)
  {
    kind = that.kind->copyK();
  }

  MPICommValueKindPtr MPICommValueObject::getKind() const {
    return kind;
  }

  void MPICommValueObject::setKind(MPICommValueKindPtr thatK) {
    kind = thatK;
  }

  void MPICommValueObject::initialize() {
    assert(0);
  }

  Lattice* MPICommValueObject::copy() const {
    return (Lattice*) new MPICommValueObject(*this);
  }
   
  bool MPICommValueObject::meetUpdate(Lattice* thatL) {
    scope reg(txt() << "MPICommValueObject::meetUpdate(thisL=" << str() << "thatL=" << thatL->str() << ")",
              scope::medium,
              attrGE("mpiCommAnalysisDebugLevel", 3));
    MPICommValueObject* thatV = dynamic_cast<MPICommValueObject*>(thatL); assert(thatV);
    MPICommValueKindPtr thatK = thatV->getKind(); assert(thatK);
    
    // if this is unknown or that is bottom nothing to join
    if(kind->getKindType() == MPICommValueKind::unknown ||
       thatK->getKindType() == MPICommValueKind::bottom) {
      return false;
    }
    // if this is bottom copy thatK into this
    else if(kind->getKindType() == MPICommValueKind::bottom) {
      kind = thatK->copyK();
      if(mpiCommAnalysisDebugLevel() >=3) {
        dbg << "After join thisL=" << str() << endl;
      }
      return true;
    }
    // if that is unknown and this is not -> this is unknown now
    else if(thatK->getKindType() == MPICommValueKind::unknown) {
      kind = boost::make_shared<MPICommValueUnknownKind>();
      return true;
    }
    // both are concrete values    
    else {
      MPICommValueConcreteKindPtr thisCK = boost::dynamic_pointer_cast<MPICommValueConcreteKind>(kind);
      MPICommValueConcreteKindPtr thatCK = boost::dynamic_pointer_cast<MPICommValueConcreteKind>(thatV->getKind());
      assert(thisCK && thatCK);
      // if types are different
      if(thisCK->getConcreteType()->variantT() != thatCK->getConcreteType()->variantT()) {
        kind = boost::make_shared<MPICommValueUnknownKind>();
        return true;
      }
      return thisCK->unionConcreteValues(thatCK);
    }    
  }

  bool MPICommValueObject::operator==(Lattice* thatL) {
    MPICommValueObject* thatV = dynamic_cast<MPICommValueObject*>(thatL);
    assert(thatV);
    return kind->equalSetK(thatV->getKind());
  }

  bool MPICommValueObject::setToFull() {
    if(kind->getKindType() == MPICommValueKind::unknown) return false;
    kind = boost::make_shared<MPICommValueUnknownKind>();
    return true;
  }

  bool MPICommValueObject::setToEmpty()  {
    return false;
  }

  bool MPICommValueObject::setMLValueToFull(MemLocObjectPtr ml) {
    assert(false);
  }

  bool MPICommValueObject::isFullLat() {
    return kind->isFullK();
  }

  bool MPICommValueObject::isEmptyLat() {
    return kind->isEmptyK();
  }

  bool MPICommValueObject::mayEqualV(ValueObjectPtr vo, PartEdgePtr pedge) {
    MPICommValueObjectPtr thatV = boost::dynamic_pointer_cast<MPICommValueObject>(vo);
    assert(thatV);
    return kind->mayEqualK(thatV->getKind());
  }

  bool MPICommValueObject::mustEqualV(ValueObjectPtr vo, PartEdgePtr pedge) {
    MPICommValueObjectPtr thatV = boost::dynamic_pointer_cast<MPICommValueObject>(vo);
    assert(thatV);
    return kind->mustEqualK(thatV->getKind());
  }

  bool MPICommValueObject::equalSetV(ValueObjectPtr vo, PartEdgePtr pedge) {
    MPICommValueObjectPtr thatV = boost::dynamic_pointer_cast<MPICommValueObject>(vo);
    assert(thatV);
  typedef boost::shared_ptr<MPICommValueUnknownKind> MPICommValueUnknownKindPtr;    return kind->equalSetK(thatV->getKind());
  }

  bool MPICommValueObject::subSetV(ValueObjectPtr vo, PartEdgePtr pedge) {
    MPICommValueObjectPtr thatV = boost::dynamic_pointer_cast<MPICommValueObject>(vo);
    assert(thatV);
    return kind->subSetK(thatV->getKind());
  }

  bool MPICommValueObject::meetUpdateV(ValueObjectPtr vo, PartEdgePtr pedge) {
    MPICommValueObjectPtr thatVO = boost::dynamic_pointer_cast<MPICommValueObject>(vo);    
    Lattice* thatL = static_cast<Lattice*>(thatVO.get());
    assert(thatL);
    return meetUpdate(thatL);
  }

  bool MPICommValueObject::isEmptyV(PartEdgePtr pedge) {
    return kind->isEmptyK();
  }

  bool MPICommValueObject::isFullV(PartEdgePtr pedge) {
    return kind->isFullK();
  }

  bool MPICommValueObject::isConcrete() {
    return kind->getKindType() == MPICommValueKind::concrete;
  }

  SgType* MPICommValueObject::getConcreteType() {
    assert(kind->getKindType() == MPICommValueKind::concrete);
    MPICommValueConcreteKindPtr concreteK = boost::dynamic_pointer_cast<MPICommValueConcreteKind>(kind);
    assert(concreteK);
    return concreteK->getConcreteType();
  }

  std::set<boost::shared_ptr<SgValueExp> > MPICommValueObject::getConcreteValue() {
    assert(kind->getKindType() == MPICommValueKind::concrete);
    MPICommValueConcreteKindPtr concreteK = boost::dynamic_pointer_cast<MPICommValueConcreteKind>(kind);
    assert(concreteK);
    return concreteK->getConcreteValue();
  }

  ValueObjectPtr MPICommValueObject::copyV() const {
    return boost::make_shared<MPICommValueObject>(*this);
  }

  string MPICommValueObject::str(string indent) const  {
    ostringstream oss;
    oss << "[MPICommValueObject: " << kind->str();
    oss << ", kind.addr=" << kind.get() << ", kind.use_count=" << kind.use_count();
    oss << "]";
    return oss.str();
  }

  /*************
   * MPICommOp *
   *************/
  MPICommOp::MPICommOp(const Function& func) 
    : mpifunc(func) {
    string name = mpifunc.get_name().getString();
    if(name.compare("MPI_Send") == 0) optype = MPICommOp::SEND;
    else if(name.compare("MPI_Recv") == 0) optype = MPICommOp::RECV;
    else assert(0);    
  }

  MPICommOp::MPICommOp(const MPICommOp& that)
    : mpifunc(that.mpifunc), optype(that.optype) {
  }

  SgPointerDerefExp* MPICommOp::getCommOpBufferDerefExpr() const {
    SgPointerDerefExp* buffDerefExpr;
    if(isMPICommSendOp() || isMPICommRecvOp()) {
      SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
      SgInitializedNamePtrList& arglist = *arglist_p;
      SgVariableSymbol* buffSymbol = isSgVariableSymbol(arglist[0]->search_for_symbol_from_symbol_table());
      assert(buffSymbol);
      SgVarRefExp* buffVarRefExpr = SageBuilder::buildVarRefExp(buffSymbol);
      buffDerefExpr = SageBuilder::buildUnaryExpression<SgPointerDerefExp>(buffVarRefExpr);
      assert(buffDerefExpr);
    }
    else assert(false);
    return buffDerefExpr;
  }

  SgInitializedName* MPICommOp::getCommOpTarget() const {
    SgInitializedName* target;
    if(isMPICommSendOp() || isMPICommRecvOp()) {
      SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
      SgInitializedNamePtrList& arglist = *arglist_p;
      target = arglist[3];
    }
    else assert(false);
    assert(target);
    return target;
  }

  SgInitializedName* MPICommOp::getCommOpTag() const {
    SgInitializedName* tag;
    if(isMPICommSendOp() || isMPICommRecvOp()) {
      SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
      SgInitializedNamePtrList& arglist = *arglist_p;
      tag = arglist[4];
    }
    else assert(false);
    assert(tag);
    return tag;
  }

  SgInitializedName* MPICommOp::getCommOpComm() const {
    SgInitializedName* comm;
    if(isMPICommSendOp() || isMPICommRecvOp()) {
      SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
      SgInitializedNamePtrList& arglist = *arglist_p;
      comm = arglist[5];
    }
    else assert(false);
    assert(comm);
    return comm;
  }

  bool MPICommOp::isMPICommSendOp() const {
    return optype == MPICommOp::SEND;
  }

  bool MPICommOp::isMPICommRecvOp() const {
    return optype == MPICommOp::RECV;
  }

  /**************************
   * MPICommAnalysisTranfer *
   **************************/
  MPICommAnalysisTransfer::MPICommAnalysisTransfer(PartPtr part,
                                                   CFGNode cfgn,
                                                   NodeState& state,
                                                   std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo,
                                                   MPICommAnalysis* _analysis)
    : DFTransferVisitor(part, cfgn, state, dfInfo), analysis(_analysis), modified(false) {
    // assert(dfInfo.size()==1);
    // PartEdgePtr wildCardPartEdge = part->inEdgeFromAny();
    // assert(dfInfo[wildCardPartEdge][0]);
    // Lattice *l = dfInfo[wildCardPartEdge][0];
    // latticeMap = (dynamic_cast<AbstractObjectMap*>(l));
    // assert(latticeMap);
  }

  Function MPICommAnalysisTransfer::getFunction(SgFunctionParameterList* sgn) {
    SgFunctionDeclaration* decl = isSgFunctionDeclaration(sgn->get_parent());
    ROSE_ASSERT(decl);
    Function func(decl);
    return func;
  }

  Function MPICommAnalysisTransfer::getFunction(SgFunctionCallExp* sgn) {
    SgFunctionDeclaration* decl = isSgFunctionDeclaration(sgn->getAssociatedFunctionDeclaration());
    ROSE_ASSERT(decl);
    Function func(decl);
    return func;
  }

  Function MPICommAnalysisTransfer::getFunction(SgNode* sgn) {
    SgFunctionDefinition* defn = SageInterface::getEnclosingFunctionDefinition(sgn);
    ROSE_ASSERT(defn);
    Function func(defn);
    return func;
  }

  bool MPICommAnalysisTransfer::isMPIFuncCall(const Function& func) const {
    if(func.get_name().getString().find("MPI_") != string::npos) return true;
    return false;
  }

  bool MPICommAnalysisTransfer::isMPISendOp(const Function& func) const {
    string name = func.get_name().getString();
    if(name.compare("MPI_Send") == 0) return true;
    return false;
  }

  bool MPICommAnalysisTransfer::isMPIRecvOp(const Function& func) const {
     string name = func.get_name().getString();
     if(name.compare("MPI_Recv") == 0) return true;
     return false;
  }
 
  bool MPICommAnalysisTransfer::isMPICommOpFuncCall(const Function& func) const {
    return isMPISendOp(func) || isMPIRecvOp(func);    
  }

  MPICommAnalysisTransfer::ValueObject2Int::ValueObject2Int(Composer* composer, PartEdgePtr pedge, ComposedAnalysis* analysis)
    : composer(composer), pedge(pedge), analysis(analysis) { }

  int MPICommAnalysisTransfer::ValueObject2Int::operator()(SgInitializedName* sgn) {
    scope reg("MPICommAnalysisTransfer::ValueObject2Int", scope::low, 
          attrGE("mpiCommAnalysisDebugLevel", 2));
    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << "sgn=" << SgNode2Str(sgn) << endl;
      dbg << "pedge=" << pedge->str() << endl;
    }

    ValueObjectPtr vo = composer->Expr2Val(sgn, pedge, analysis);

    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << vo->str() << endl;
    }
    assert(vo->isConcrete());

    SgValueExpPtrSet cvalues = vo->getConcreteValue();
    if(mpiCommAnalysisDebugLevel() >= 2) {
      SgValueExpPtrSet::iterator it = cvalues.begin();
      for(int i = 0; it != cvalues.end(); ++it, ++i) {
        dbg << "cvalues[" << i << "]=" << SgNode2Str(it->get()) << endl;
      }
    }
    assert(cvalues.size() == 1);
    SgValueExpPtr sgval = *cvalues.begin();

    switch(sgval.get()->variantT()) {      
      case V_SgIntVal: {
        return isSgIntVal(sgval.get())->get_value();
      }
      case V_SgLongIntVal: {
        return isSgLongIntVal(sgval.get())->get_value();
      }
      case V_SgLongLongIntVal: {
        return isSgLongLongIntVal(sgval.get())->get_value();
      }
      // TODO: Fill out cases for other int types
      default: {
        dbg << "unhandled type=" << SgNode2Str(sgval.get()) << endl;
        assert(0);      
      } // end default
    } // end switch case
  }

  string MPICommAnalysisTransfer::serialize(MPICommValueObjectPtr mvo) {
    stringstream ss;
    boost::archive::text_oarchive oa(ss);
    oa << mvo;
    return ss.str();
  }

  MPICommValueObjectPtr MPICommAnalysisTransfer::deserialize(string data) {
    MPICommValueObjectPtr mvo;
    stringstream ss(data);
    boost::archive::text_iarchive ia(ss);
    ia >> mvo;
    assert(mvo);
    return mvo;
  }


  void MPICommAnalysisTransfer::visit(SgFunctionCallExp* sgn) {
    if(Part::isIncomingFuncCall(cn)) {
      Function call = getFunction(sgn);
      string name = call.get_name().getString();
      if(name.compare("MPI_Comm_rank") == 0 ||
         name.compare("MPI_Comm_size") == 0) {
        assert(sgn->getAttribute("fuse:UnknownSideEffectsAttribute"));
        Composer* composer = analysis->getComposer();
        modified = composer->HavocFuncSideEffects(sgn, analysis, part->inEdgeFromAny(), dfInfo);
      }
    }
    modified = dynamic_cast<BoolAndLattice*>(dfInfo[part->inEdgeFromAny()][0])->set(true);
  }
  
  void MPICommAnalysisTransfer::visit(SgFunctionParameterList* sgn) {
    Function call = getFunction(sgn);
    string name = call.get_name().getString();

    if(name.compare("MPI_Barrier") == 0) {
      transferMPIBarrier();
    }
  }

  void MPICommAnalysisTransfer::transferMPIBarrier() {
    MPI_Barrier(MPI_COMM_WORLD);
  }
  
  void MPICommAnalysisTransfer::transferMPISendOp(SgPointerDerefExp* sgn, const MPICommOp& commop) {
    scope reg("MPICommAnalysisTransfer::transferMPISendOp", scope::low, 
          attrGE("mpiCommAnalysisDebugLevel", 2));
    assert(commop.isMPICommSendOp());

    Composer* composer = analysis->getComposer();
    MemLocObjectPtr bml = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);

    ValueObjectPtr  bvo = composer->Expr2Val(sgn, part->inEdgeFromAny(), analysis);
    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << "buffer value=" << bvo->str() << endl;      
    }
    // Build MPICommValueObject based on this ValueObject
    MPICommValueKindPtr kind = createMPICommValueKind(bvo, part->inEdgeFromAny());
    MPICommValueObjectPtr bmvo = boost::make_shared<MPICommValueObject>(kind, part->inEdgeFromAny());               
    // modified = latticeMap->insert(bml, bmvo) || modified;

    // Generic functor to get value object given an sage expression
    // Forwards the query to the compoer
    ValueObject2Int vo2int(composer, part->inEdgeFromAny(), analysis);

    SgInitializedName* targetExpr = commop.getCommOpTarget();
    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << "targetExpr=" << SgNode2Str(targetExpr) << endl;
    }
    int target = vo2int(targetExpr);

    SgInitializedName* tag_sgn = commop.getCommOpTag();
    int tag = vo2int(tag_sgn);

    SgInitializedName* comm_sgn = commop.getCommOpComm();
    MPI_Comm comm = (MPI_Comm) vo2int(comm_sgn);
    //TODO: verify we are communicating in MPI_COMM_WORLD
    assert(comm == MPI_COMM_WORLD);

    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << "target=" << target << ", tag= " << tag << ", MPI_Comm= " << comm << endl;
    }

    string sdata = serialize(bmvo);
    const char* sdata_p = sdata.c_str();

    // Issue the MPI_Send operation to runtime from analysis
    MPI_Send(sdata_p, sdata.size(), MPI_CHAR, target, tag, comm);    
  }

   void MPICommAnalysisTransfer::transferMPIRecvOp(SgPointerDerefExp* sgn, const MPICommOp& commop) {
    scope reg("MPICommAnalysisTransfer::transferMPIRecvOp", scope::medium, 
          attrGE("mpiCommAnalysisDebugLevel", 2));
    assert(commop.isMPICommRecvOp());

    Composer* composer = analysis->getComposer();
    ValueObject2Int vo2int(composer, part->inEdgeFromAny(), analysis);

    SgInitializedName* source_sgn = commop.getCommOpTarget();
    int source = vo2int(source_sgn);

    SgInitializedName* tag_sgn = commop.getCommOpTag();
    int tag = vo2int(tag_sgn);

    SgInitializedName* comm_sgn = commop.getCommOpComm();
    MPI_Comm comm = (MPI_Comm) vo2int(comm_sgn);
    //TODO: verify we are communicating in MPI_COMM_WORLD
    assert(comm == MPI_COMM_WORLD);
    
    // space of 1000 char to receive dataflow info
    char* sdata_p = new char[1000];

    // Issue the MPI_Recv operation to runtime from analysis
    MPI_Status status;
    MPI_Recv(sdata_p, 1000, MPI_CHAR, source, tag, comm, &status);

    // deserialize
    string sdata(sdata_p);
    MPICommValueObjectPtr mvo = deserialize(sdata);

    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << "Recv VO:" << mvo->str() << endl;
    }

    // Find the outgoing edges of MPI call expression part
    list<PartEdgePtr> callExpEdges = outGoingEdgesMPICallExp(part, "MPI_Recv");
    
    // Update the RecvValueObjectMap corresponding to each edge in callExpEdges list
    MemLocObjectPtr buffML = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    list<PartEdgePtr>::iterator ce = callExpEdges.begin();
    
    for( ; ce != callExpEdges.end(); ++ce) {
      analysis->insertRecvMLVal(*ce, buffML, mvo);
    }

    if(mpiCommAnalysisDebugLevel() >= 3) {
      dbg << analysis->stringifyRecvMLValMap() << endl;
    }
    
    // modified = latticeMap->insert(buffML, mvo) || modified;
  }

  void MPICommAnalysisTransfer::visit(SgPointerDerefExp* sgn) {
    Function func = getFunction(sgn);
    if(isMPISendOp(func)) {
      MPICommOp sendop(func);
      transferMPISendOp(sgn, sendop);
    }
    else if(isMPIRecvOp(func)) {
      MPICommOp recvop(func);
      transferMPIRecvOp(sgn, recvop);
    }
    modified = dynamic_cast<BoolAndLattice*>(dfInfo[part->inEdgeFromAny()][0])->set(true);
  }
 
  void MPICommAnalysisTransfer::visit(SgNode* sgn) {
    // identity transfer
    modified = dynamic_cast<BoolAndLattice*>(dfInfo[part->inEdgeFromAny()][0])->set(true);
  }

  bool mpiFuncCallExpFilter(string funcname, CFGNode cfgn) {
    SgNode* sgn = cfgn.getNode();
    if(!isSgFunctionCallExp(sgn)) return false;
    SgFunctionCallExp* funcall = isSgFunctionCallExp(sgn);
    Function func(funcall);
    string mpifuncname = func.get_name().getString();
    return (mpifuncname.compare(funcname) == 0);
  }

  list<PartEdgePtr> MPICommAnalysisTransfer::outGoingEdgesMPICallExp(PartPtr srcPart, string funcname) {
    scope reg("MPICommAnalysisTransfer::outGoingEdgesMPICallExp", scope::medium, attrGE("mpiCommAnalysisDebugLevel", 3));
    list<PartEdgePtr> pedges;
    // Find the first edge outgoing from ATS part corresponding to MPI function call expression
    fw_dataflowGraphEdgeIterator<PartEdgePtr, PartPtr> fe;
    fe.addStart(part);
    PartContextPtr context = srcPart->getPartContext();
    assert(context);

    if(mpiCommAnalysisDebugLevel() >=3) {
      dbg << "srcPart=" << srcPart->str() << endl;
      dbg << "funcname=" << funcname << endl;
      dbg << "context=" << context->str() << endl;
    }

    fe.pushAllDescendants();
    fe++;
    while(fe != fw_dataflowGraphEdgeIterator<PartEdgePtr, PartPtr>::end()) {
      PartPtr part = fe.getPart();
      PartEdgePtr pe = *fe;

      // boost::function<bool (CFGNode)> filter = boost::bind(mpiFuncCallExpFilter, funcname, _1);
      // PartPtr source = pe->source();
      // if(source && source->mapCFGNodeALL<bool>(filter)) {        
      //   pedges.push_back(pe);
      // }
      if(mpiCommAnalysisDebugLevel() >= 3) dbg << "Edge match: " << pe->str() << endl;
      pedges.push_back(pe);

      if(part->getPartContext() == context) {
        fe.pushAllDescendants();
      }
      fe++;
    }

    assert(pedges.size() > 0);
    return pedges;
  }

  bool MPICommAnalysisTransfer::finish() {
    return modified;
  }

  /*************
   * RecvMLVal *
   *************/
  RecvMLVal::RecvMLVal() { }

  RecvMLVal::RecvMLVal(MemLocObjectPtr ml, MPICommValueObjectPtr mval) : ml(ml), value(mval) { }

  MemLocObjectPtr RecvMLVal::getMemLocObject() const {
    return ml;
  }

  MPICommValueObjectPtr RecvMLVal::getValueObject() const {
    return value;
  }

  string RecvMLVal::str(string indent) const {
    ostringstream oss;
    oss << "ML=" << ml->str() << ",";
    oss << "Val=" << value->str() << endl;
    return oss.str();
  }

  /*******************
   * MPICommAnalysis *
   *******************/
  MPICommAnalysis::MPICommAnalysis() {
  }

  // void MPICommAnalysis::initAnalysis(set<PartPtr>& startingParts) {
  //   assert(0);
  // }

  void MPICommAnalysis::genInitLattice(PartPtr part, PartEdgePtr pedge, 
                                       std::vector<Lattice*>& initLattices) {
    // AbstractObjectMap* aomap = new AbstractObjectMap(boost::make_shared<MPICommValueObject>(pedge), 
    //                                                  pedge, 
    //                                                  getComposer(), 
    //                                                  this);
    // initLattices.push_back(aomap);
    initLattices.push_back(new BoolAndLattice(0, pedge));
  }

  boost::shared_ptr<DFTransferVisitor> MPICommAnalysis::getTransferVisitor(PartPtr part, 
                                                                           CFGNode cn, 
                                                                           NodeState& state, 
                                                                           map<PartEdgePtr, vector<Lattice*> >& dfInfo) {
    return boost::shared_ptr<MPICommAnalysisTransfer>(new MPICommAnalysisTransfer(part, cn, state, dfInfo, this));
  }

  /** 
   * Inserting into stl map with PartEdgePtr as key.
   * Working with the assumption that all keys are full defined.
   * Each PartEdgePtr has well defined source and target PartPtr.
   * Each key PartEdgePtr do not contain any wildcard edges.
   * @param pedge key.
   * @param ml, mval MemLocObject and ValueObject corresponding the Recv buffer.
   * ValueObject mval is first copied and only copied objects are inserted into the map.
   * This is to ensure proper usage of shared_ptrs.
   * Analysis is free to modify the ValueObjectPtr however they want and their changes
   * shouldn't impact the ValueObject stored in the map.
   * Two cases: tuple <pedge, ml, mval> (i) already exists(ii) doesnt exist in the map.
   * If the tuple already exists join the values using meetUpdateV to create a new ValueObject.
   * Remove the old mapping and insert the tuple.
   * If this is the first insertion insert the tuple into the map.
   */
  bool MPICommAnalysis::insertRecvMLVal(PartEdgePtr pedge, MemLocObjectPtr ml, MPICommValueObjectPtr mval) {
    scope reg(txt() << "MPICommAnalysis::insertRecvMLVal",
              scope::medium,
              attrGE("mpiCommAnalysisDebugLevel", 2));
    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << "pedge=" << pedge->str() << endl;
      dbg << "ml=" << ml->str() << endl;
      dbg << "mval=" << mval->str() << endl;
    }

    // First copy the ValueObject
    MPICommValueObjectPtr mvalc = boost::dynamic_pointer_cast<MPICommValueObject>(mval->copyV());
    
    bool insertR = false;
    assert(pedge->source() && pedge->target());
        
    map<PartEdgePtr, RecvMLValPtr>::iterator melem;
    melem = recvMLValMap.find(pedge);
    // If the key exists in the map
    if(melem != recvMLValMap.end()) {
      // First assert ML is the same
      RecvMLValPtr relem = melem->second;
      if(ml->mayEqualML(relem->getMemLocObject(), pedge)) {
        MPICommValueObjectPtr oval = relem->getValueObject();        
        if(mpiCommAnalysisDebugLevel() >= 2) dbg << "OldVal=" << oval->str() << endl;
        // Join the current value into old value
        insertR = oval->meetUpdateV(mval, pedge);
        if(mpiCommAnalysisDebugLevel() >= 2) {
          dbg << "newV=" << oval->str() << endl;
          dbg << "insertR=" << insertR << endl;
        }
        // If the value changes remove the old mapping and insert the new mapping
        if(insertR) {
          RecvMLValPtr newelem = boost::make_shared<RecvMLVal>(ml, oval);
          // Erase the mapping first (melem no longer is valid after this point)
          // Also stl insert doesnt insert if key already exists!
          recvMLValMap.erase(pedge);
          // Insert the new mapping
          recvMLValMap.insert(std::pair<PartEdgePtr, RecvMLValPtr>(pedge, newelem));
        }
      }
      // This case is a little bit more complicated
      // We have a key pedge in the map but the tuple's ml does not mayEqual the ml to be inserted        
      // This case arises when multiple different recv operation contexts are approximated into one.
      else {
        // The conservative solution is to join the two ml and their values
        MemLocObjectPtr oml = relem->getMemLocObject();
        MPICommValueObjectPtr oval = relem->getValueObject();
        if(mpiCommAnalysisDebugLevel() >= 2) {
          dbg << "oldML=" << oml->str() << endl;
          dbg << "oldV=" << oval->str() << endl;
        }
        // First copy the ML
        MemLocObjectPtr omlc = oml->copyML();
        // Join the already mapped ML and the ml to be inserted
        omlc->meetUpdateML(ml, pedge);
        // Now merge the values
        oval->meetUpdateV(mval, pedge);
        RecvMLValPtr newelem = boost::make_shared<RecvMLVal>(omlc, oval);
        // Erase the mapping first (melem no longer is valid after this point)
        // Also stl insert doesnt insert if key already exists!
        recvMLValMap.erase(pedge);          
        recvMLValMap.insert(std::pair<PartEdgePtr, RecvMLValPtr>(pedge, newelem));
        insertR = true;
        if(mpiCommAnalysisDebugLevel() >= 2) {
          dbg << "newML=" << oml->str() << endl;
          dbg << "newV=" << oval->str() << endl;
        }
      }
    }
    // When this is the first time we insert into the map
    else {
      RecvMLValPtr rmlval = boost::make_shared<RecvMLVal>(ml, mvalc);
      recvMLValMap.insert(std::pair<PartEdgePtr, RecvMLValPtr>(pedge, rmlval));
      insertR = true;
    }    
    return insertR;
  }

  string MPICommAnalysis::stringifyRecvMLValMap() const {
    ostringstream oss;
    map<PartEdgePtr, RecvMLValPtr>::const_iterator it = recvMLValMap.begin();
    oss << "<table border=\"1\">";
    oss << "<th> RecvMLValMap </th>";
    for( ; it != recvMLValMap.end(); ++it) {
      PartEdgePtr pedge = it->first;
      RecvMLValPtr rmlval = it->second;
      oss << "<tr>";
      oss << "<td width=\"40%\"> " << pedge->str() << "</td>";
      oss << "<td>" << rmlval->str() << "</td>";
      oss << "</tr>";
    }
    oss << "</table>";
    return oss.str();
  }

  // Return all matching PartEdges corresponding to pedge
  list<PartEdgePtr> MPICommAnalysis::matchingPartEdges(PartEdgePtr pedge) const {
    scope reg(txt() << "MPICommAnalysis::matchingPartEdges(pedge=" << pedge->str() << ")",
              scope::medium,
              attrGE("mpiCommAnalysisDebugLevel", 3));
    list<PartEdgePtr> pedges;
    int count=0;
    map<PartEdgePtr, RecvMLValPtr>::const_iterator elem;
    // Edge has both source and target
    if(pedge->source() && pedge->target()) {
      elem = recvMLValMap.find(pedge);
      if(elem != recvMLValMap.end()) {
        pedges.push_back(elem->first);
        if(mpiCommAnalysisDebugLevel() >= 3) {
          dbg << "pedges[" << count++ << "]=" << elem->first->str() << endl;
        }
      }
    }
    // wildcard edge of the form source -> *
    else if(pedge->source()) {
      map<PartEdgePtr, RecvMLValPtr>::const_iterator it = recvMLValMap.begin();
      for( ; it != recvMLValMap.end(); ++it) {
        PartEdgePtr pe = it->first;
        if(pe->source() == pedge->source()) {
          pedges.push_back(pe);
          if(mpiCommAnalysisDebugLevel() >= 3) {
            dbg << "pedges[" << count++ << "]=" << pe->str() << endl;
          }          
        }
      }
    }
    // wildcard edge of the form * -> target
    else if(pedge->target()) {
      map<PartEdgePtr, RecvMLValPtr>::const_iterator it = recvMLValMap.begin();
      for( ; it != recvMLValMap.end(); ++it) {
        PartEdgePtr pe = it->first;
        if(pe->target() == pedge->target()) {
          pedges.push_back(pe);
          if(mpiCommAnalysisDebugLevel() >= 3) {
            dbg << "pedges[" << count++ << "]=" << pe->str() << endl;
          }          
        }
      }
    }
    else {
      map<PartEdgePtr, RecvMLValPtr>::const_iterator it = recvMLValMap.begin();
      for( ; it != recvMLValMap.end(); ++it) {
        PartEdgePtr pe = it->first;
        pedges.push_back(pe);
        if(mpiCommAnalysisDebugLevel() >= 3) {
          dbg << "pedges[" << count++ << "]=" << pe->str() << endl;
        }                  
      }
    }
    return pedges;
  }

  list<RecvMLValPtr> MPICommAnalysis::getRecvMLVal(PartEdgePtr pedge) const {
    scope reg(txt() << "MPICommAnalysis::getRecvMLVal(pedge" << pedge->str() << ")",
              scope::medium,
              attrGE("mpiCommAnalysisDebugLevel", 3));
    list<RecvMLValPtr> rmlvals;
    list<PartEdgePtr> pedges = matchingPartEdges(pedge);
    int count = 0;
    list<PartEdgePtr>::const_iterator it = pedges.begin();
    for( ; it != pedges.end(); ++it) {
      PartEdgePtr pe = *it;
      map<PartEdgePtr, RecvMLValPtr>::const_iterator elem;
      elem = recvMLValMap.find(pe);
      assert(elem != recvMLValMap.end());
      rmlvals.push_back(elem->second);
      if(mpiCommAnalysisDebugLevel() >= 3) {
        dbg << "rmlvals[" << count++ << "]=" << elem->second->str() << endl;
      }
    }
    return rmlvals;
  }

  MPICommValueObjectPtr MPICommAnalysis::mergeMayEqualMLVal(MemLocObjectPtr ml, PartEdgePtr pedge,
                                                            list<RecvMLValPtr>& rmlvals) {
    scope reg(txt() << "MPICommAnalysis::getMayEqualMLVal(ml=" << ml->str() << ")",
              scope::medium,
              attrGE("mpiCommAnalysisDebugLevel", 3));
    MPICommValueObjectPtr retV = boost::make_shared<MPICommValueObject>(pedge);
    list<RecvMLValPtr>::const_iterator it = rmlvals.begin();
    for( ; it != rmlvals.end(); ++it) {
      MemLocObjectPtr that = (*it)->getMemLocObject();
      if(ml->mayEqualML(that, pedge)) {
        MPICommValueObjectPtr thatV = (*it)->getValueObject();
        retV->meetUpdateV(thatV, pedge);
        if(mpiCommAnalysisDebugLevel() >=3 ) {
          dbg << "thatV=" << thatV->str() << endl;
          dbg << "retV=" << retV->str() << endl;
        }
      }
    }
    return retV;
  }
  
  ValueObjectPtr MPICommAnalysis::Expr2Val(SgNode* sgn, PartEdgePtr pedge) {
    scope reg(sight::txt() << "MPICommAnalysis::Expr2Val(sgn=" << SgNode2Str(sgn) << ",pedge=" << pedge->str() << ")",
          scope::medium,
          attrGE("mpiCommAnalysisDebugLevel", 2));
    
    list<RecvMLValPtr> rmlvals = getRecvMLVal(pedge);
    MPICommValueObjectPtr mvo;
    dbg << "rmlvals.size()=" << rmlvals.size() << endl;
    if(rmlvals.size() != 0) {
      MemLocObjectPtr ml = getComposer()->Expr2MemLoc(sgn, pedge, this);
      mvo = mergeMayEqualMLVal(ml, pedge, rmlvals);
    }
    else {
      ValueObjectPtr val = getComposer()->Expr2Val(sgn, pedge, this);
      if(mpiCommAnalysisDebugLevel() >= 2) {
        dbg << "Composer Val=" << val->str() << endl;
      }
      mvo = boost::make_shared<MPICommValueObject>(val, pedge);
    }
    assert(mvo->getKind());
    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << "Expr2Val(sgn)=" << mvo->str() << endl;
    }
    return mvo;
    // Composer* composer = getComposer();
    // MemLocObjectPtr ml = composer->Expr2MemLoc(sgn, pedge, this);

    // AbstractObjectMap* latticeMap;
    // if(pedge->target()) {
    //   PartPtr part = pedge->target();
    //   NodeState* state = NodeState::getNodeState(this, part);
    //   assert(state);
    //   latticeMap = dynamic_cast<AbstractObjectMap*>(state->getLatticeAbove(this, pedge, 0));
    // }
    // else if(pedge->source()) {
    //   PartPtr part = pedge->source();     
    //   NodeState* state = NodeState::getNodeState(this, part);
    //   assert(state);
    //   latticeMap = dynamic_cast<AbstractObjectMap*>(state->getLatticeBelow(this, pedge, 0));
    // }
    // else assert(0);

    // assert(latticeMap);
    // dbg << "latticeMap=" << latticeMap->str() << endl;
    // LatticePtr latVal = latticeMap->get(ml);
    // assert(latVal);

    // MPICommValueObjectPtr mvo = boost::dynamic_pointer_cast<MPICommValueObject>(latVal); assert(mvo);
    // if(mvo->isFullV(pedge)) {
    //   ValueObjectPtr vo = composer->Expr2Val(sgn, pedge, this);
    //   mvo = boost::make_shared<MPICommValueObject>(vo, pedge);
    // }
  }


  string MPICommAnalysis::str(std::string indent) const {
    return "MPICommAnalysis";
  }
}
