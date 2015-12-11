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

namespace fuse {

  DEBUG_LEVEL(mpiCommAnalysisDebugLevel, 2);

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

  ConcreteValuePtr makeConcreteValue(const SgValueExpPtr sgvalue) {
    switch(sgvalue->variantT()) {
    case V_SgIntVal: 
      {
        boost::shared_ptr<SgIntVal> ivalue = boost::dynamic_pointer_cast<SgIntVal>(sgvalue);
        return boost::make_shared<IntegerConcreteValue>(ivalue->get_value());
      }
    default:
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
    MPICommValueKind(that), concreteValues(concreteValues) {
  }

  MPICommValueConcreteKind::MPICommValueConcreteKind(const std::set<ConcreteValuePtr>& concreteValues) :
    MPICommValueKind(MPICommValueKind::concrete), concreteValues(concreteValues) {
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
      ValueObject(that),
      kind(that.kind) {
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
    assert(dfInfo.size()==1);
    PartEdgePtr wildCardPartEdge = part->inEdgeFromAny();
    assert(dfInfo[wildCardPartEdge][0]);
    Lattice *l = dfInfo[wildCardPartEdge][0];
    latticeMap = (dynamic_cast<AbstractObjectMap*>(l));
    assert(latticeMap);
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
    scope("MPICommAnalysisTransfer::ValueObject2Int", scope::low, 
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
  }
  
  void MPICommAnalysisTransfer::visit(SgFunctionParameterList* sgn) {
  }

  void MPICommAnalysisTransfer::transferMPISendOp(SgPointerDerefExp* sgn, const MPICommOp& commop) {
    scope("MPICommAnalysisTransfer::transferMPISendOp", scope::low, 
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
    modified = latticeMap->insert(bml, bmvo) || modified;

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

    if(mpiCommAnalysisDebugLevel() >= 2) {
      dbg << "target=" << target << ", tag= " << tag << ", MPI_Comm= " << comm << endl;
    }

    string sdata = serialize(bmvo);
    const char* sdata_p = sdata.c_str();

    // Issue the MPI_Send operation to runtime from analysis
    MPI_Send(sdata_p, sdata.size(), MPI_CHAR, target, tag, comm);    
  }

  void MPICommAnalysisTransfer::transferMPIRecvOp(SgPointerDerefExp* sgn, const MPICommOp& commop) {
    scope("MPICommAnalysisTransfer::transferMPIRecvOp", scope::medium, 
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
    
    // space of 1000 char to receive dataflow info
    char* sdata_p = new char[1000];

    // Issue the MPI_Recv operation to runtime from analysis
    MPI_Status status;
    MPI_Recv(sdata_p, 1000, MPI_CHAR, source, tag, comm, &status);

    // deserialize
    string sdata(sdata_p);
    MPICommValueObjectPtr mvo = deserialize(sdata);

    // Update the latticeMap
    MemLocObjectPtr buffML = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    modified = latticeMap->insert(buffML, mvo) || modified;
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
  }
 
  void MPICommAnalysisTransfer::visit(SgNode* sgn) {
    // identity transfer
  }


  bool MPICommAnalysisTransfer::finish() {
    return modified;
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
    AbstractObjectMap* aomap = new AbstractObjectMap(boost::make_shared<MPICommValueObject>(pedge), 
                                                     pedge, 
                                                     getComposer(), 
                                                     this);
    initLattices.push_back(aomap);
  }

  boost::shared_ptr<DFTransferVisitor> MPICommAnalysis::getTransferVisitor(PartPtr part, 
                                                                           CFGNode cn, 
                                                                           NodeState& state, 
                                                                           map<PartEdgePtr, vector<Lattice*> >& dfInfo) {
    return boost::shared_ptr<MPICommAnalysisTransfer>(new MPICommAnalysisTransfer(part, cn, state, dfInfo, this));
  }

  ValueObjectPtr MPICommAnalysis::Expr2Val(SgNode* sgn, PartEdgePtr pedge) {
    scope(sight::txt() << "MPICommAnalysis::Expr2Val(sgn=" << SgNode2Str(sgn) << ",pedge=" << pedge->str() << ")",
          scope::medium,
          attrGE("mpiCommAnalysisDebugLevel", 2));
    Composer* composer = getComposer();
    MemLocObjectPtr ml = composer->Expr2MemLoc(sgn, pedge, this);

    AbstractObjectMap* latticeMap;
    if(pedge->target()) {
      PartPtr part = pedge->target();
      NodeState* state = NodeState::getNodeState(this, part);
      assert(state);
      latticeMap = dynamic_cast<AbstractObjectMap*>(state->getLatticeAbove(this, pedge, 0));
    }
    else if(pedge->source()) {
      PartPtr part = pedge->source();     
      NodeState* state = NodeState::getNodeState(this, part);
      assert(state);
      latticeMap = dynamic_cast<AbstractObjectMap*>(state->getLatticeBelow(this, pedge, 0));
    }
    else assert(0);

    assert(latticeMap);
    dbg << "latticeMap=" << latticeMap->str() << endl;
    LatticePtr latVal = latticeMap->get(ml);
    assert(latVal);

    MPICommValueObjectPtr mvo = boost::dynamic_pointer_cast<MPICommValueObject>(latVal); assert(mvo);
    if(mvo->isFullV(pedge)) {
      ValueObjectPtr vo = composer->Expr2Val(sgn, pedge, this);
      mvo = boost::make_shared<MPICommValueObject>(vo, pedge);
    }
    return mvo;
  }

  // bool MPICommAnalysis::controllingAgentInitiate() {
  //   for(int p = 1; p < size; ++p) {
  //     MPI_Send()
  //   }
  // }

  // void MPICommAnalysis::runControllingAgent() {
  //   int terminate = 0;
  //   while(!terminate) {
  //     MPI_Iprobe();
  //     MPI_Iprobe();

  //     while(state == active) {
  //       runAnalysis;
  //     }
  //   }
  // }

  // void MPICommAnalysis::runNonControllingAgent() {
  //   int terminate = 0;
  //   while(!terminate) {
  //     // Listen for control message from controlling agent on the controlComm
  //     MPI_Iprobe();
  //     // Listen for dataflow message from any process on the general MPI_COMM_WORLD
  //     MPI_Iprobe();
      
  //     while(state==active) {
  //       runAnalysis();
  //     }
  //   }
  //   state = idle;
    
    
  // }

  // void MPICommAnalysis::runMPIAnalysis() {
  //   if(isControllingAgent()) {
  //     controllingAgentInitiate();
  //   }
  //   else {
  //   }
  // }

  string MPICommAnalysis::str(std::string indent) const {
    return "MPICommAnalysis";
  }
}
