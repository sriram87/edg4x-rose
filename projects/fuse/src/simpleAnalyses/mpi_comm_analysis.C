/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

#include "sage3basic.h"
#include "mpi_comm_analysis.h"
#include "abstract_object_map.h"

using namespace std;
using namespace boost;

namespace fuse {

  DEBUG_LEVEL(mpiCommAnalysisDebugLevel, 2);

  /********************
   * MPICommValueKind *
   ********************/
  MPICommValueKind::MPICommValueKind(ValueKind kind) : kind(kind) { }
  
  MPICommValueKind::ValueKind MPICommValueKind::getKindType() const {
    return kind;
  }

  bool MPICommValueKind::mayEqualK(MPICommValueKindPtr that) {
    if(kind != that->getKindType()) return false;
    else return mayEqualVK(that);
  }

  bool MPICommValueKind::mustEqualK(MPICommValueKindPtr that) {
    if(kind != that->getKindType()) return false;
    else return mustEqualVK(that);
  }

  bool MPICommValueKind::equalSetK(MPICommValueKindPtr that) {
    if(kind != that->getKindType()) return false;
    else return equalSetVK(that);
  }

  bool MPICommValueKind::subSetK(MPICommValueKindPtr that) {
    if(that->getKindType == unknown) return true;
    else if(kind == unknown && that->getKindType() == concrete) return false;
    // both are concrete kind
    else return subSetVK(that);
  }

  bool MPICommValueKind::meetUpdateK(MPICommValueKindPtr that) {
    if(kind == unknown) return false;
    else if(kind == concrete && that->getKindType() == unknown) {
      kind 
    }
  }

  bool MPICommValueKind::isEmptyK() {
    return false;
  }

  bool MPICommValueKind::isFullK() {
    return kind == unknown;
  }

  /****************************
   * MPICommValueConcreteKind *
   ****************************/

  /***************************
   * MPICommValueUnknownKind *
   ***************************/
  MPICommValueUnknownKind::MPICommValueUnknownKind() : MPICommValueKind(MPICommValueKind::unknown) { }
  MPICommValueUnknownKind::MPICommValueUnknownKind(const MPICommValueUnknownKind& that) : MPICommValueKind(that) { }

  MPICommValueKindPtr MPICommValueUnknownKind::copyK() {
    return boost::make_shared<MPICommValueUnknownKind>(*this);
  }

  bool MPICommValueUnknownKind::mayEqualVK(MPICommValueVKindPtr thatVK) {
    return true;
  }

  bool MPICommValueUnknownKind::mustEqualVK(MPICommValueVKindPtr thatVK) {
    return false;
  }

  bool MPICommValueUnknownKind::meetUpdateVK(MPICommValueVKindPtr thatVK) {
    return false;
  }
  bool MPICommValueUnknownKind::equalSetVK(MPICommValueVKindPtr thatVK) {}
  bool MPICommValueUnknownKind::subSetVK(MPICommValueVKindPtr thatVK) {}
  string MPICommValueUnknownKind::str(std::string indent="") const {}



  /**********************
   * MPICommValueObject *
   **********************/

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
    MPICommValueObject* thatV = dynamic_cast<MPICommValueObject*>(thatL);
    assert(thatV);
    if(kind->getKindType() == MPICommValueKind::unknown) return false;
    else if(that->getKind()->getKindType() == MPICommValueKind::unknown) {
      kind = boost::make_shared<MPICommValueUnknownKind>();
      return true;
    }
    // both are concrete values    
    else {
      MPICommValueConcreteKindPtr thisCK = boost::dynamic_pointer_cast<MPICommValueConcretKind>(kind);
      MPICommValueConcreteKindPtr thatCK = thatV->getKind();
      assert(thisCK &&t thatCK);
      // if types are different
      if(kind->getType() != thatK->getType()) {
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
    Lattice* thatL = static_cast<Lattice*>(vo.get());
    assert(thatL);
    return meetUpdate(thatL);
  }

  bool MPICommValueObject::isEmptyV() {
    return kind->isEmptyK();
  }

  bool MPICommValueObject::isFullV() {
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

  ValueObjectPtr MPICommValueObject::copyV() {
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
    : DFTransferVisitor(part, cfgn, state, dfInfo), analysis(_analysis) { 
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
    scope("MPICommAnalysisTransfer::visit(SgFunctionCallExp* sgn)", 
          scope::medium, attrGE("mpiCommAnalysisDebugLevel", 2));
    Function func = getFunction(sgn);
    MPICommOpCallExp commOpCallExp(func, sgn->get_args());
    if(commOpCallExp.isMPICommOp()) {
      // Check if this CFGNode is a outgoing function call cfgIndex=2
      if(Part::isOutgoingFuncCall(cn) && commOpCallExp.isMPICommOp()) {
        SgExpression* buffExpr = commOpCallExp.getCommOpBufferExpr();
        Composer* composer = analysis->getComposer();
        MemLocObjectPtr buffML = composer->OperandExpr2MemLoc(sgn, buffExpr, part->inEdgeFromAny());
        ValueObjectPtr  buffVO = composer->OperandExpr2Val(sgn, buffExpr, part->inEdgeFromAny());
        dbg << "buffML=" << buffML->str();
        dbg << "buffVO=" << buffVO->str();
      }
    }
  }
  
  void MPICommAnalysisTransfer::visit(SgFunctionParameterList* sgn) {
    Function func = getFunction(sgn);
    MPICommOpCallParamList commOpCallParamList(func, sgn->get_args());
  }

  void MPICommAnalysisTransfer::visit(SgPointerDerefExp* sgn) {
    Function func = getFunction(sgn);
    if(isMPICommOpFuncCall(func)) {
      Composer* composer = analysis->getComposer();
      MemLocObjectPtr buffML = composer->Expr2MemLoc(sgn, part->inEdgeFromAny());
      ValueObjectPtr  buffVO = composer->Expr2Val(sgn, part->inEdgeFromAny());
        dbg << "buffML=" << buffML->str();
        dbg << "buffVO=" << buffVO->str();
    }    
  }

  void MPICommAnalysisTransfer::visit(SgNode* sgn) {
    // identity transfer
  }

  bool MPICommAnalysisTransfer::finish() {
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
