/*****************************************
 * Author: Sriram Aananthakrishnan, 2016 *
 *****************************************/

#include "sage3basic.h"
#include "mpi_dot_value_analysis.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


namespace fuse {
  DEBUG_LEVEL(mpiDotValueAnalysisDebugLevel, 2);
  
  boost::uuids::random_generator gen;
  /*********************
   * MPIDotValueObject *
   *********************/
  MPIDotValueObject::MPIDotValueObject(PartEdgePtr pedge)
     : Lattice(pedge), FiniteLattice(pedge), ValueObject(0) {
    dotvalue="";
  }
  
  MPIDotValueObject::MPIDotValueObject(const CFGNode& cn, PartEdgePtr pedge)
    : Lattice(pedge), FiniteLattice(pedge), ValueObject(0) {
    int flag;
    MPI_Initialized(&flag); assert(flag);
    int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ostringstream oss;
    oss << cn.id() << "_rank_" << rank;
    dotvalue = oss.str();
  }

  MPIDotValueObject::MPIDotValueObject(ValueObjectPtr v, PartEdgePtr pedge)
    : Lattice(pedge), FiniteLattice(pedge), ValueObject(0) {
    if(v->isConcrete()) {
      SgType* vtype = v->getConcreteType();
      if(isSgTypeString(vtype)) {
        set<boost::shared_ptr<SgValueExp> > vset = v->getConcreteValue();
        if(vset.size() == 1) {
          boost::shared_ptr<SgValueExp> svalue_p = *vset.begin();
          SgStringVal* sgsval = boost::dynamic_pointer_cast<SgStringVal>(svalue_p).get(); assert(sgsval);
          dotvalue = sgsval->get_value();
          dbg << "MPIDotValueObject::MPIDotValueObject(dotvalue=" << dotvalue << ")\n";
        }
        else dotvalue = "unknown";
      }
      else dotvalue="unknown";
    }
    else {
      dotvalue="unknown";
    }
  }
  
  MPIDotValueObject::MPIDotValueObject(const MPIDotValueObject& that)
    : Lattice(that), FiniteLattice(that), ValueObject(that) {
    dotvalue = that.dotvalue;
  }

  ValueObjectPtr MPIDotValueObject::copyV () const {
    return boost::make_shared<MPIDotValueObject>(*this);
  }

  string MPIDotValueObject::get_dot_value() const {
    return dotvalue;
  }

  void MPIDotValueObject::initialize() {
    dotvalue="";
  }
  
  Lattice* MPIDotValueObject::copy() const {
    return new MPIDotValueObject(*this);
  }
  
  bool MPIDotValueObject::meetUpdate(Lattice* that) {
    MPIDotValueObject* thatL = dynamic_cast<MPIDotValueObject*>(that);
    assert(thatL);
    string thatDotV = thatL->get_dot_value();
    if(dotvalue.compare(thatDotV) == 0) return false;
    dotvalue = "unknown";
    return true;
  }
  
  bool MPIDotValueObject::meetUpdateV (ValueObjectPtr that, PartEdgePtr pedge) {
    MPIDotValueObjectPtr thatV = boost::dynamic_pointer_cast<MPIDotValueObject>(that);
    // if that object is not a MPIDotValueObject then conservatively set to top
    if(!thatV) {
      dotvalue = "unknown";
      return true;
    }
    // If that is also a MPIDotValueObject check if the strings are equal
    if(dotvalue.compare(thatV->get_dot_value())==0) return false;
    // Else set to unknown and return true
    dotvalue = "unknown";
    return true;
  }

  // Each Dot Value is unique
  bool MPIDotValueObject::operator==(Lattice* that) {
    MPIDotValueObject* thatMDV = dynamic_cast<MPIDotValueObject*>(that);
    return dotvalue.compare(thatMDV->get_dot_value()) == 0;
  }
  
  bool MPIDotValueObject::equalSetV (ValueObjectPtr o, PartEdgePtr pedge) {
    MPIDotValueObjectPtr thatMDV = boost::dynamic_pointer_cast<MPIDotValueObject>(o);
    if(!thatMDV) return false;
    return dotvalue.compare(thatMDV->get_dot_value()) == 0;
  }
  
  bool MPIDotValueObject::setToFull() {
    dotvalue = "unknown";
    return true;
  }
  
  bool MPIDotValueObject::setToEmpty() {
    dotvalue = "";
    return true;
  }
  
  bool MPIDotValueObject::isFullLat() {
    return dotvalue.compare("unknown")==0;
  }

  bool MPIDotValueObject::isFullV(PartEdgePtr pedge) {
    return isFullLat();
  }
  
  bool MPIDotValueObject::isEmptyLat() {
    return dotvalue.compare("")==0;
  }

  bool MPIDotValueObject::isEmptyV(PartEdgePtr pedge) {
    return isEmptyLat();
  }
  
  bool MPIDotValueObject::setMLValueToFull (MemLocObjectPtr ml) {
    assert(false);
  }

  bool MPIDotValueObject::mayEqualV (ValueObjectPtr o, PartEdgePtr pedge) {
    // It does not matter if the value object is MPIDotValueObject or not
    // We don't know their actual values and they could all be overlapping
    return true;
  }
  
  bool MPIDotValueObject::mustEqualV (ValueObjectPtr o, PartEdgePtr pedge) {
    return false;
  }
  
  bool MPIDotValueObject::subSetV (ValueObjectPtr o, PartEdgePtr pedge) {
    return false;
  }

  bool MPIDotValueObject::isConcrete() {
    return true;
  }
  
  SgType* MPIDotValueObject::getConcreteType() {
    SgStringVal* sval = SageBuilder::buildStringVal(dotvalue);
    return sval->get_type();
  }
  
  set<boost::shared_ptr< SgValueExp> > MPIDotValueObject::getConcreteValue() {
    set<boost::shared_ptr<SgValueExp> > vset;
    boost::shared_ptr<SgValueExp> sval_sp =
      boost::shared_ptr<SgStringVal>(SageBuilder::buildStringVal(dotvalue));
    vset.insert(sval_sp);
    return vset;
  }
  
  string MPIDotValueObject::str(std::string indent) const {
    ostringstream oss;
    oss << "[MPIDotValue: " << dotvalue << "]";
    return oss.str();
  }

  /*********************
   * MPISendOpCallSite *
   *********************/
  MPISendOpCallSite::MPISendOpCallSite(const Function& mpif_,
                                       SgFunctionCallExp* sgn)
    : mpif_(mpif_), sgn(sgn) { }

  // Traverse the expression tree to find the buffer expr for the MPI operations
  SgExpression* traverseAST(SgExpression* expr) {
    switch(expr->variantT()) {
    case V_SgVarRefExp:
      return expr;
    case V_SgAddressOfOp:
      return traverseAST(isSgUnaryOp(expr)->get_operand());
    case V_SgCastExp:
      return traverseAST(isSgUnaryOp(expr)->get_operand());
    default:
      dbg << "Unhandled buffer Expr in traverseATS(expr=" << SgNode2Str(expr) << ")\n";
      assert(0);
    }
  }

  SgNode* MPISendOpCallSite::getSendBuffer() {
    SgExpressionPtrList& args = sgn->get_args()->get_expressions();
    SgExpression* bexpr = traverseAST(args[0]); assert(bexpr);
    return bexpr;
  }

  bool MPISendOpCallSite::isSendOp() {
    string name = mpif_.get_name().getString();
    return (name.compare("MPI_Send") == 0 ||
            name.compare("MPI_Isend") == 0);
  }
  
  /*********************
   * MPIRecvOpCallSite *
   *********************/
  MPIRecvOpCallSite::MPIRecvOpCallSite(const Function& mpif_,
                                       SgFunctionCallExp* sgn)
    : mpif_(mpif_), sgn(sgn) { }

  SgNode* MPIRecvOpCallSite::getRecvBuffer() {
    SgExpressionPtrList& args = sgn->get_args()->get_expressions();
    SgExpression* bexpr = traverseAST(args[0]); assert(bexpr);
    return bexpr;
  }

  bool MPIRecvOpCallSite::isRecvOp() {
    string name = mpif_.get_name().getString();
    return (name.compare("MPI_Recv") == 0 ||
            name.compare("MPI_Irecv") == 0);
  }
  
  /**********************
   * MDVTransferVisitor *
   **********************/
  MDVTransferVisitor::MDVTransferVisitor(PartPtr part,
                                         CFGNode cn,
                                         NodeState& state,
                                         map<PartEdgePtr, vector<Lattice*> >& dfInfo,
                                         MPIDotValueAnalysis* analysis)
    : DFTransferVisitor(part, cn, state, dfInfo),
      analysis(analysis), modified(false) {
    assert(dfInfo.size()==1);
    assert(dfInfo[part->inEdgeFromAny()].size()==1);
    assert(*dfInfo[part->inEdgeFromAny()].begin());

    Lattice *l = *dfInfo[part->inEdgeFromAny()].begin(); assert(l);
    aMapState = dynamic_cast<AbstractObjectMap*>(l); assert(aMapState);
  }

  bool MDVTransferVisitor::isMPISendOp(Function& mpif_) {
    string name = mpif_.get_name().getString();
    if(name.compare("MPI_Send")  == 0 ||
       name.compare("MPI_Isend") == 0) {
      return true;
    }
    return false;
  }

  bool MDVTransferVisitor::isMPIRecvOp(Function& mpif_) {
    string name = mpif_.get_name().getString();
    if(name.compare("MPI_Recv")  == 0 ||
       name.compare("MPI_Irecv") == 0) {
      return true;
    }
    return false;
  }

  void MDVTransferVisitor::visit(SgAddressOfOp* sgn) { }
  
  void MDVTransferVisitor::visit(SgBitComplementOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgCastExp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgMinusMinusOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgMinusOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgNotOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgPlusPlusOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  } 
  
  void MDVTransferVisitor::visit(SgUnaryAddOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgUnaryOp* sgn) {
    dbg << "Unhandled SgUnaryOp=" << SgNode2Str(sgn) << endl;
    assert(false);
  }

  void MDVTransferVisitor::visit(SgPointerDerefExp* sgn) { }

  void MDVTransferVisitor::visit(SgAddOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);    
  }
  
  void MDVTransferVisitor::visit(SgAndOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgArrowExp* sgn) { }
  void MDVTransferVisitor::visit(SgArrowStarOp* sgn) { }
  
  void MDVTransferVisitor::visit(SgAssignOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MemLocObjectPtr lml = analysis->getComposer()->Expr2MemLoc(sgn->get_lhs_operand(), part->inEdgeFromAny(), analysis); 
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    MPIDotValueObjectPtr lmdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull(); lmdv->setToFull();
    modified = aMapState->insert(ml, mdv) || modified;
    modified = aMapState->insert(lml, lmdv) || modified;
  }
  
  void MDVTransferVisitor::visit(SgBitAndOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgBitOrOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgBitXorOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgCommaOpExp* sgn) { assert(0); }

  void MDVTransferVisitor::visit(SgCompoundAssignOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MemLocObjectPtr lml = analysis->getComposer()->Expr2MemLoc(sgn->get_lhs_operand(), part->inEdgeFromAny(), analysis); 
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    MPIDotValueObjectPtr lmdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull(); lmdv->setToFull();
    modified = aMapState->insert(ml, mdv) || modified;
    modified = aMapState->insert(lml, lmdv) || modified;
  }
  
  void MDVTransferVisitor::visit(SgDivideOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgDotExp* sgn) { }
  void MDVTransferVisitor::visit(SgDotStarOp* sgn) { }
  
  void MDVTransferVisitor::visit(SgEqualityOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgGreaterOrEqualOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgGreaterThanOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgLessOrEqualOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgLessThanOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgLshiftOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgModOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgMultiplyOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgNotEqualOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgOrOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgPntrArrRefExp* sgn) { }
  
  void MDVTransferVisitor::visit(SgSubtractOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgBinaryOp* sgn) {
    dbg << "Unhandled Binary Op = " << SgNode2Str(sgn) << endl;
    assert(false);
  }

  void MDVTransferVisitor::visit(SgConditionalExp* sgn) { assert(0); }
  
  void MDVTransferVisitor::visit(SgDeleteExp* sgn) { }
  void MDVTransferVisitor::visit(SgExprListExp* sgn) { }  
  void MDVTransferVisitor::visit(SgFunctionRefExp* sgn) { }

  // Nothing to do here
  // SgInitializedName is set to unknown instead
  void MDVTransferVisitor::visit(SgInitializer* sgn) { }
  
  void MDVTransferVisitor::visit(SgNewExp* sgn) { }
  void MDVTransferVisitor::visit(SgSizeOfOp* sgn) { }
  
  void MDVTransferVisitor::visit(SgValueExp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);    
  }
  
  void MDVTransferVisitor::visit(SgVarRefExp* sgn) { }
  
  void MDVTransferVisitor::visit(SgExpression* sgn) {
    dbg << "Unhandled expr=" << SgNode2Str(sgn) << endl;
    assert(0);
  }

  void MDVTransferVisitor::visit(SgInitializedName* sgn) {
    if(sgn->get_initializer()) {
      MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
      MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
      mdv->setToFull();
      modified = aMapState->insert(ml, mdv);
    }
  }

  void MDVTransferVisitor::havocUnknownValues(SgFunctionCallExp* sgn) {
    if(Part::isOutgoingFuncCall(cn)) {
      // Check if it has side-effects attribute
      if(sgn->getAttribute("fuse:UnknownSideEffectsAttribute")) {
        SgExprListExp* args = sgn->get_args();
        const SgExpressionPtrList& argsExprList = args->get_expressions();
        SgExpressionPtrList::const_iterator it = argsExprList.begin();
        // Iterate through the argument list
        // Check for ValueUnknownAttribute
        for( ; it != argsExprList.end(); ++it) {
          SgExpression* uexpr = *it;
          if(uexpr->getAttribute("fuse:ValueUnknownAttribute")) {
            // Find the expr whose value was modified by this function
            // It could be hiding under address of expr: &x
            // It could be hiding under cast expr: (int) x
            SgExpression* mexpr = traverseAST(uexpr);
            MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(mexpr, part->inEdgeFromAny(), analysis);
            analysis->getComposer()->HavocMLValue(ml, dfInfo);
          }
        }
      }
    }
  }

  // void MDVTransferVisitor::visit(SgInitializedName* sgn) {
  //   MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
  //   analysis->getComposer()->HavocMLValue(ml, dfInfo);
  // }

  void MDVTransferVisitor::visit(SgFunctionCallExp* sgn) {
    Function mpif_(sgn);
    if(Part::isOutgoingFuncCall(cn)) {
      havocUnknownValues(sgn);      
      if(isMPISendOp(mpif_)) {
        modified = transferSendOp(mpif_, sgn);
      }
    }
    else if(Part::isIncomingFuncCall(cn)) {
      if(isMPIRecvOp(mpif_)) {
        modified = transferRecvOp(mpif_, sgn);
      }
    }
  }

  bool MDVTransferVisitor::transferSendOp(Function& mpif_, SgFunctionCallExp* sgn) {
    scope reg(sight::txt() << "MDVTransferVisitor::transferSendOp",
              scope::medium,
              attrGE("mpiDotValueAnalysisDebugLevel", 2));
    assert(isMPISendOp(mpif_));
    MPISendOpCallSite sendcs(mpif_, sgn);
    SgNode* bexpr = sendcs.getSendBuffer();
    MemLocObjectPtr bml = analysis->getComposer()->Expr2MemLoc(bexpr, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(cn, part->inEdgeFromAny());
    if(mpiDotValueAnalysisDebugLevel() >= 2) {    
      dbg << "bml=" << bml->str() << endl;
      dbg << "mdv=" << mdv->str() << endl;
    }
    return aMapState->insert(bml, boost::dynamic_pointer_cast<Lattice>(mdv));                            
  }

  bool MDVTransferVisitor::transferRecvOp(Function& mpif_, SgFunctionCallExp* sgn) {
    scope reg(sight::txt() << "MDVTransferVisitor::transferRecvOp",
              scope::medium,
              attrGE("mpiDotValueAnalysisDebugLevel", 2));
    assert(isMPIRecvOp(mpif_));
    MPIRecvOpCallSite recvcs(mpif_, sgn);
    SgNode* bexpr = recvcs.getRecvBuffer();
    MemLocObjectPtr bml = analysis->getComposer()->Expr2MemLoc(bexpr, part->inEdgeFromAny(), analysis);
    ValueObjectPtr v = analysis->getComposer()->Expr2Val(bexpr, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(v, part->inEdgeFromAny());
    if(mpiDotValueAnalysisDebugLevel() >= 2) {
      dbg << "Expr2Val(bexpr) = " << v->str() << endl;
      dbg << "mdv=" << mdv->str() << endl;
    }
    // assert(!mdv->isFullLat());
    return aMapState->insert(bml, mdv);
  }

  bool MDVTransferVisitor::finish() {
    return modified;
  }

  /***********************
   * MPIDotValueAnalysis *
   ***********************/
  void MPIDotValueAnalysis::initAnalysis(std::set<PartPtr>& startingParts) {
  }

  void MPIDotValueAnalysis::genInitLattice(PartPtr part, PartEdgePtr pedge, 
                                           std::vector<Lattice*>& initLattices) {
    AbstractObjectMap* amap = new AbstractObjectMap(boost::make_shared<MPIDotValueObject>(pedge),
                                                    pedge,
                                                    getComposer(),
                                                    this);
    initLattices.push_back(amap);
  }
  
  bool MPIDotValueAnalysis::transfer(PartPtr part, CFGNode cn, NodeState& state, 
                                   map<PartEdgePtr, vector<Lattice*> >& dfInfo) {
    assert(false);
  }

  boost::shared_ptr<DFTransferVisitor>
  MPIDotValueAnalysis::getTransferVisitor(PartPtr part, 
                                          CFGNode cfgn, 
                                          NodeState& state, 
                                          map<PartEdgePtr, vector<Lattice*> >& dfInfo) {
    // can't use boost::make_shared here
    return boost::shared_ptr<MDVTransferVisitor>(new MDVTransferVisitor(part,cfgn,state,dfInfo, this));
  }

  ValueObjectPtr MPIDotValueAnalysis::Expr2Val(SgNode* sgn, PartEdgePtr pedge) {
    scope reg(sight::txt() << "MPIDotValueAnalysis::Expr2Val(sgn=" << SgNode2Str(sgn) << ",pedge=" << pedge->str() << ")",
              scope::medium,
              attrGE("mpiDotValueAnalysisDebugLevel", 2));
    Composer* composer = getComposer();
    assert(pedge->source() || pedge->target());
    NodeState* state;
    Lattice* l;
    // outEdge
    if(pedge->source()) {
      state = NodeState::getNodeState(this, pedge->source());
      l = state->getLatticeBelow(this, pedge, 0);
    }
    // inEdge
    else if(pedge->target()) {
      state = NodeState::getNodeState(this, pedge->target());
      l = state->getLatticeAbove(this, pedge, 0);
    }
    else assert(0); // should never reach here

    assert(l);
    AbstractObjectMap* aMapState = dynamic_cast<AbstractObjectMap*>(l); assert(aMapState);

    MemLocObjectPtr ml = composer->Expr2MemLoc(sgn, pedge, this);
    MPIDotValueObjectPtr mdv = boost::dynamic_pointer_cast<MPIDotValueObject>(aMapState->get(ml));
    if(mpiDotValueAnalysisDebugLevel() >= 2) {
      dbg << "ml=" << ml->str() << endl;
      dbg << "mdv=" << mdv->str() << endl;
    }
    if(mdv->isFullLat()) return composer->Expr2Val(sgn, pedge, this);
    else if(mdv->isEmptyLat()) assert(0);
    return mdv;
  }

  string MPIDotValueAnalysis::str(string indent) const {
    return "MPIDotValueAnalysis";
  }

}; // end namespace
