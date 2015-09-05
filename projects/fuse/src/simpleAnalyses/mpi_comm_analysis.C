/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

#include "sage3basic.h"
#include "mpi_comm_analysis.h"
#include "serialization.h"
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
  IntegerConcreteValue::IntegerConcreteValue(int value)
    : ConcreteValue(),
      value(value) { }

  IntegerConcreteValue::IntegerConcreteValue(const IntegerConcreteValue& that)
    : ConcreteValue(that),
      value(that.value) { }

  int IntegerConcreteValue::get_value() const {
    return value;
  }

  bool IntegerConcreteValue::operator<(const ConcreteValuePtr& that) const {
    IntegerConcreteValuePtr thatV = dynamicConstPtrCast<IntegerConcreteValue>(that);
    assert(thatV);
    return value < thatV->get_value();
  }

  bool IntegerConcreteValue::operator==(const ConcreteValuePtr& that) const {
    IntegerConcreteValuePtr thatV = dynamicConstPtrCast<IntegerConcreteValue>(that);
    assert(thatV);
    return value == thatV->get_value();
  }

  bool IntegerConcreteValue::operator!=(const ConcreteValuePtr& that) const {
    IntegerConcreteValuePtr thatV = dynamicConstPtrCast<IntegerConcreteValue>(that);
    assert(thatV);
    return value != thatV->get_value();
  }

  boost::shared_ptr<SgType> IntegerConcreteValue::getSgType() const {
    return boost::shared_ptr<SgTypeInt>(SageBuilder::buildIntType());
  }

  SgValueExpPtr IntegerConcreteValue::getSgValueExpPtr() const {
    return boost::shared_ptr<SgValueExp>(SageBuilder::buildIntVal(value));
  }

  string IntegerConcreteValue::str(string indent) const {
    ostringstream oss;
    oss << value;
    return oss.str();
  }

  set<ConcreteValuePtr> buildConcreteValueSet(SgType* valueType, const SgValueExpPtrSet& valueExpSet) {
    switch(valueType->variantT()) {
    case V_SgTypeInt:
      return buildIntegerConcreteValueSet(valueType, valueExpSet);
    default:
      dbg << "type=" << SgNode2Str(valueType) << "not supported\n";
      assert(0);
    }
  }

  set<ConcreteValuePtr> buildIntegerConcreteValueSet(SgType* valueType, const SgValueExpPtrSet& valueExpSet) {
    set<ConcreteValuePtr> intConcreteValues;
    SgValueExpPtrSet::const_iterator cIt = valueExpSet.begin();
    for( ; cIt != valueExpSet.end(); ++cIt) {
      SgIntVal* val = dynamic_cast<SgIntVal*>(cIt->get());
      assert(val);
      IntegerConcreteValuePtr ival = boost::make_shared<IntegerConcreteValue>(val->get_value());
      intConcreteValues.insert(ival);
    }
    return intConcreteValues;
  }
  
  /****************************
   * MPICommValueConcreteKind *
   ****************************/
  MPICommValueConcreteKind::MPICommValueConcreteKind(SgType* valueType, const SgValueExpPtrSet& valueExpSet) :
    MPICommValueKind(MPICommValueKind::concrete) {
    concreteValues = buildConcreteValueSet(valueType, valueExpSet);
  }

  MPICommValueConcreteKind::MPICommValueConcreteKind(const set<ConcreteValuePtr>& concreteValues) :
    MPICommValueKind(MPICommValueKind::concrete),
    concreteValues(concreteValues) { }

  MPICommValueConcreteKind::MPICommValueConcreteKind(const MPICommValueConcreteKind& that) :
    MPICommValueKind(that),
    concreteValues(that.concreteValues) { }

  MPICommValueKindPtr MPICommValueConcreteKind::copyK() {
    return boost::make_shared<MPICommValueConcreteKind>(*this);
  }

  SgType* MPICommValueConcreteKind::getConcreteType() const {
    ConcreteValuePtr first = *concreteValues.begin();
    return first->getSgType().get();
  }

  SgValueExpPtrSet MPICommValueConcreteKind::getConcreteValue() const {
    SgValueExpPtrSet cvalSet;
    set<ConcreteValuePtr>::const_iterator cIt = concreteValues.begin();
    for( ; cIt != concreteValues.end(); ++cIt) {
      ConcreteValuePtr cval = *cIt;
      SgValueExpPtr sgValExp = cval->getSgValueExpPtr();
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

  /**********************
   * MPICommValueObject *
   **********************/
  MPICommValueObject::MPICommValueObject(PartEdgePtr pedge) 
    : Lattice(pedge),
      FiniteLattice(pedge),
      ValueObject(NULL) {
    kind = boost::make_shared<MPICommValueDefaultKind>();
  }

  MPICommValueObject::MPICommValueObject(PartEdgePtr pedge, MPICommValueKindPtr thatK)
    : Lattice(pedge),
      FiniteLattice(pedge),
      ValueObject(0) {
    kind = thatK->copyK();
  }
      

  MPICommValueObject::MPICommValueObject(PartEdgePtr pedge, ValueObjectPtr vo)
    : Lattice(pedge),
      FiniteLattice(pedge),
      ValueObject(*vo.get()) {
    if(vo->isConcrete()) {
      kind = boost::make_shared<MPICommValueConcreteKind>(vo->getConcreteType(), vo->getConcreteValue());
    }
    else {
      kind = boost::make_shared<MPICommValueUnknownKind>();
    }
  }

  MPICommValueObject::MPICommValueObject(const MPICommValueObject& that)
    : Lattice(that),
      FiniteLattice(that),
      ValueObject(that) {
    kind = that.kind->copyK();
  }

  MPICommValueKindPtr MPICommValueObject::getKind() const {
    return kind;
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
    return kind->equalSetK(thatV->getKind());
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
    oss << "[MPICommValueObject: " << kind->str() << "]";
    return oss.str();
  }

  /********************
   * MPICommOpCallExp *
   ********************/

  MPICommOp::OpType buildMPICommOpType(const Function& mpifunc) {
    MPICommOp::OpType optype;
    string name = mpifunc.get_name().getString();
    if(name.compare("MPI_Send") == 0) optype = MPICommOp::SEND;
    else if(name.compare("MPI_Recv") == 0) optype = MPICommOp::RECV;
    else if(name.compare("MPI_Init") == 0) optype = MPICommOp::INIT;
    else if(name.compare("MPI_Finalize") == 0) optype = MPICommOp::FINALIZE;
    else optype = MPICommOp::NOOP;
    return optype;
  }

  MPICommOpCallExp::MPICommOpCallExp(const Function& func,
                                     SgExprListExp* arglist) 
    : mpifunc(func),
      argList(arglist) {
    optype = buildMPICommOpType(mpifunc);
  }

  MPICommOpCallExp::MPICommOpCallExp(const MPICommOpCallExp& that)
    : mpifunc(that.mpifunc),
      argList(that.argList),
      optype(that.optype) { }


  SgExpression* MPICommOpCallExp::getCommOpBufferExpr() {
    SgExpressionPtrList& exprPtrList = argList->get_expressions();
    SgExpression* expr0 = exprPtrList[0];
    SgExpression* buffExpr;
    buffExpr = expr0;
    // switch(expr0->variantT()) {
    // case V_SgCastExp:
    //   buffExpr = isSgCastExp(expr0)->get_operand();
    //   break;
    // case V_SgVarRefExp:
    //   buffExpr = expr0;
    //   break;
    // default: assert(0);
    // }
    return buffExpr;
  }

  SgExpression* MPICommOpCallExp::getCommOpDestExpr() {
    SgExpressionPtrList& exprPtrList = argList->get_expressions();
    SgExpression* expr3 = exprPtrList[3];
    SgExpression* destExpr;
    switch(expr3->variantT()) {
    case V_SgIntVal:
    case V_SgVarRefExp:
    case V_SgAddOp:
    case V_SgSubtractOp:
      destExpr = expr3;
      break;
    default: 
      dbg << "destExpr=" << SgNode2Str(expr3) << endl;
      assert(0);
    }
    return destExpr;
  }

  SgExpression* MPICommOpCallExp::getCommOpTagExpr() {
    SgExpressionPtrList& exprPtrList = argList->get_expressions();
    SgExpression* expr4 = exprPtrList[4];
    SgExpression* tagExpr;
    switch(expr4->variantT()) {
    case V_SgIntVal:
    case V_SgVarRefExp:
      tagExpr = expr4;
      break;
    default: assert(0);
    }
    return tagExpr;
  }

  bool MPICommOpCallExp::isMPICommOp() {
    return (optype == MPICommOp::SEND ||
            optype == MPICommOp::RECV);
  }

  /**************************
   * MPICommOpCallParamList *
   **************************/
  MPICommOpCallParamList::MPICommOpCallParamList(const Function& func, const SgInitializedNamePtrList& argList) 
    : mpifunc(func), argList(argList) {
    optype = buildMPICommOpType(mpifunc);
  }

  MPICommOpCallParamList::MPICommOpCallParamList(const MPICommOpCallParamList& that)
    : mpifunc(that.mpifunc), argList(that.argList), optype(that.optype) { }

  SgInitializedName* MPICommOpCallParamList::getCommOpBuffer() {
    return argList[0];
  }

  SgPointerDerefExp* MPICommOpCallParamList::getCommOpBufferDerefExpr() {
    SgVariableSymbol* buffSymbol = isSgVariableSymbol(argList[0]->search_for_symbol_from_symbol_table());
    assert(buffSymbol);
    SgVarRefExp* buffVarRefExpr = SageBuilder::buildVarRefExp(buffSymbol);
    SgPointerDerefExp* buffDerefExpr = SageBuilder::buildUnaryExpression<SgPointerDerefExp>(buffVarRefExpr);
    assert(buffDerefExpr);
    return buffDerefExpr;
  }

  bool MPICommOpCallParamList::isMPICommOp() {
    return (optype == MPICommOp::SEND ||
            optype == MPICommOp::RECV);
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

  bool MPICommAnalysisTransfer::isMPICommOpFuncCall(const Function& func) const {
    string name = func.get_name().getString();
    if(name.compare("MPI_Send") == 0) return true;
    else if(name.compare("MPI_Recv") == 0) return true;
    else return false;
  }

  void MPICommAnalysisTransfer::visit(SgFunctionCallExp* sgn) {
  }
  
  void MPICommAnalysisTransfer::visit(SgFunctionParameterList* sgn) {
  }

  void MPICommAnalysisTransfer::visit(SgPointerDerefExp* sgn) {
    Function func = getFunction(sgn);
    if(isMPICommOpFuncCall(func)) {
      Composer* composer = analysis->getComposer();
      MemLocObjectPtr buffML = composer->Expr2MemLoc(sgn, part->inEdgeFromAny());
      ValueObjectPtr  buffVO = composer->Expr2Val(sgn, part->inEdgeFromAny());
        dbg << "buffML=" << buffML->str();
        dbg << "buffVO=" << buffVO->str();
        MPICommValueObjectPtr buffmpiVO = boost::make_shared<MPICommValueObject>(part->inEdgeFromAny(), buffVO);
        modified = latticeMap->insert(buffML, buffmpiVO);
        dbg << "buffVO=" << buffmpiVO->str() << endl;

        stringstream ss;
        boost::archive::text_oarchive oa(ss);
        MPICommValueObject* mvo_p = buffmpiVO.get();
        oa << mvo_p;

        std::cout << ss.str() << endl;

        boost::archive::text_iarchive ia(ss);
        MPICommValueObject* mvo_dsp;
        ia >> mvo_dsp;
        std::cout << mvo_dsp->str() << endl;
        
        assert(false);
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
    return boost::make_shared<FullValueObject>();
  }

  string MPICommAnalysis::str(std::string indent) const {
    return "MPICommAnalysis";
  }
}
