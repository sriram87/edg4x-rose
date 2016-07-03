#include "sage3basic.h"
#include "mpi_utils.h"
#include "sight.h"
#include "sight_control.h"

using namespace std;
using namespace sight;

namespace fuse {
  /*************
   * MPICommOp *
   *************/
  MPICommOp::MPICommOp(const Function& func) {
    string name = func.get_name().getString();
    if(name.compare("MPI_Send") == 0) optype = MPICommOp::SEND;
    else if(name.compare("MPI_Recv") == 0) optype = MPICommOp::RECV;
    else if(name.compare("MPI_Barrier") == 0) optype = MPICommOp::BARRIER;
    else if(name.compare("MPI_Comm_rank") == 0) optype = MPICommOp::COMMRANK;
    else if(name.compare("MPI_Comm_size") == 0) optype = MPICommOp::COMMSIZE;
    else if(name.compare("MPI_Bcast") == 0) optype = MPICommOp::BCAST;
    else if(name.compare("MPI_Reduce") == 0) optype = MPICommOp::REDUCE;
    else if(name.compare("MPI_Init") == 0) optype = MPICommOp::INIT;
    else if(name.compare("MPI_Finalize") == 0) optype = MPICommOp::FINALIZE;
    else optype = MPICommOp::UNKNOWN;
  }

  MPICommOp::MPICommOp(const MPICommOp& that) : optype(that.optype) {
  }

  bool MPICommOp::isMPISendOp() const {
    return optype == MPICommOp::SEND;
  }

  bool MPICommOp::isMPIRecvOp() const {
    return optype == MPICommOp::RECV;
  }

  bool MPICommOp::isMPIBarrierOp() const {
    return optype == MPICommOp::BARRIER;
  }

  bool MPICommOp::isMPIBcastOp() const {
    return optype == MPICommOp::BCAST;
  }

  bool MPICommOp::isMPIReduceOp() const {
    return optype == MPICommOp::REDUCE;
  }

  bool MPICommOp::isMPIUnknownOp() const {
    return optype == MPICommOp::UNKNOWN;
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
    case V_SgPntrArrRefExp:
      return expr;
    case V_SgIntVal:
      return expr;
    default:
      cerr << "Unhandled buffer Expr in traverseATS(expr=" 
           << SgNode2Str(expr) << ")\n"
           << ", file:" << __FILE__
           << ",line: " << __LINE__ << endl;
      exit(EXIT_FAILURE);
    }
  }

  SgNode* MPISendOpCallSite::getSendBuffer() {
    SgExpressionPtrList& args = sgn->get_args()->get_expressions();
    SgExpression* bexpr = traverseAST(args[0]); assert(bexpr);
    return bexpr;
  }

  bool MPISendOpCallSite::isSendOp() {
    string name = mpif_.get_name().getString();
    return (name.compare("MPI_Send") == 0);          
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
    return (name.compare("MPI_Recv") == 0);
            
  }

  /**********************
   * MPIBcastOpCallSite *
   **********************/
  MPIBcastOpCallSite::MPIBcastOpCallSite(const Function& mpif_,
                                         SgFunctionCallExp* sgn)
    : mpif_(mpif_), sgn(sgn) { }

  SgNode* MPIBcastOpCallSite::getBcastBuffer() {
    SgExpressionPtrList& args = sgn->get_args()->get_expressions();
    SgExpression* bexpr = traverseAST(args[0]); assert(bexpr);
    return bexpr;
  }

  SgNode* MPIBcastOpCallSite::getBcastRootExpr() {
    SgExpressionPtrList& args = sgn->get_args()->get_expressions();
    SgExpression* rexpr = traverseAST(args[3]); assert(rexpr);
    return rexpr;
  }

  /***********************
   * MPIReduceOpCallSite *
   ***********************/
  MPIReduceOpCallSite::MPIReduceOpCallSite(const Function& mpif_,
                                           SgFunctionCallExp* sgn)
    : mpif_(mpif_), sgn(sgn) { }

  SgNode* MPIReduceOpCallSite::getReduceSBuffer() {
    SgExpressionPtrList& args = sgn->get_args()->get_expressions();
    SgExpression* bexpr = traverseAST(args[0]); assert(bexpr);
    return bexpr;
  }

  SgNode* MPIReduceOpCallSite::getReduceRBuffer() {
    SgExpressionPtrList& args = sgn->get_args()->get_expressions();
    SgExpression* bexpr = traverseAST(args[1]); assert(bexpr);
    return bexpr;
  }

  SgNode* MPIReduceOpCallSite::getReduceRootExpr() {
    SgExpressionPtrList& args = sgn->get_args()->get_expressions();
    SgExpression* rexpr = traverseAST(args[5]); assert(rexpr);
    return rexpr;
  }


  /*******************
   * ValueObject2Int *
   *******************/
  typedef boost::shared_ptr<SgValueExp> SgValueExpPtr;
  typedef std::set<SgValueExpPtr> SgValueExpPtrSet;

  ValueObject2Int::ValueObject2Int(Composer* composer, PartEdgePtr pedge, 
                                   ComposedAnalysis* analysis, int debugLevel)
  : composer(composer), 
    pedge(pedge), 
    analysis(analysis),
    debugLevel(debugLevel) { }

  int ValueObject2Int::operator()(SgNode* sgn) {
    SIGHT_VERB_DECL(scope, ("ValueObject2Int", scope::low),
                    3, debugLevel) 
    SIGHT_VERB(dbg << "sgn=" << SgNode2Str(sgn) << endl, 3, debugLevel)
    SIGHT_VERB(dbg << "pedge=" << pedge->str() << endl, 3, debugLevel)

    ValueObjectPtr vo = composer->Expr2Val(sgn, pedge, analysis);
    SIGHT_VERB(dbg << vo->str() << endl, 3, debugLevel)

      assert(vo->isConcrete());

    SgValueExpPtrSet cvalues = vo->getConcreteValue();
    SIGHT_VERB_IF(2, debugLevel)
      SgValueExpPtrSet::iterator it = cvalues.begin();
    for(int i = 0; it != cvalues.end(); ++it, ++i) {
      dbg << "cvalues[" << i << "]=" << SgNode2Str(it->get()) << endl;
    }
    SIGHT_VERB_FI()
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
      // Dont't know how MPI arguments are promoted to this type
      // but a concrete evidence was seen in heat_mpi.c 
    case V_SgDoubleVal: {
      return (int)isSgDoubleVal(sgval.get())->get_value();
    }
      // TODO: Fill out cases for other int types
    default: {
      SIGHT_VERB(dbg << "unhandled type=" << SgNode2Str(sgval.get()) << endl, 3, debugLevel)
        assert(0);      
    } // end default
    } // end switch case
  }

  /*********************
   * OpValueObject2Int *
   *********************/
  OpValueObject2Int::OpValueObject2Int(Composer* composer, PartEdgePtr pedge, 
                                   ComposedAnalysis* analysis, int debugLevel)
  : composer(composer), 
    pedge(pedge), 
    analysis(analysis),
    debugLevel(debugLevel) { }

  int OpValueObject2Int::operator()(SgNode* anchor, SgNode* op) {
    SIGHT_VERB_DECL(scope, ("OpValueObject2Int", scope::low),
                    3, debugLevel) 
    SIGHT_VERB(dbg << "anchor=" << SgNode2Str(anchor) << endl, 3, debugLevel)
    SIGHT_VERB(dbg << "op=" << SgNode2Str(op) << endl, 3, debugLevel)
    SIGHT_VERB(dbg << "pedge=" << pedge->str() << endl, 3, debugLevel)

    ValueObjectPtr vo = composer->OperandExpr2Val(anchor, op, pedge, analysis);
    SIGHT_VERB(dbg << vo->str() << endl, 3, debugLevel)

    assert(vo->isConcrete());

    SgValueExpPtrSet cvalues = vo->getConcreteValue();
    SIGHT_VERB_IF(2, debugLevel)
      SgValueExpPtrSet::iterator it = cvalues.begin();
    for(int i = 0; it != cvalues.end(); ++it, ++i) {
      dbg << "cvalues[" << i << "]=" << SgNode2Str(it->get()) << endl;
    }
    SIGHT_VERB_FI()
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
      // Dont't know how MPI arguments are promoted to this type
      // but a concrete evidence was seen in heat_mpi.c 
    case V_SgDoubleVal: {
      return (int)isSgDoubleVal(sgval.get())->get_value();
    }
      // TODO: Fill out cases for other int types
    default: {
      SIGHT_VERB(dbg << "unhandled type=" << SgNode2Str(sgval.get()) << endl, 3, debugLevel)
        assert(0);      
    } // end default
    } // end switch case
  }


  /*********************
   * Utility Functions *
   *********************/
  void unsupportedMPICommOpExit(int line) {
    cerr << "Unsupported MPI Function at line: " << line << "\n";
    exit(EXIT_FAILURE);
  }

  void usageErrorExit(int line) {
    cerr << "Function Usage Error at line:" << line << "\n";
    exit(EXIT_FAILURE);
  }

  SgInitializedName* getSendOpTarget(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPISendOp()) usageErrorExit(__LINE__);
   
    // We are processing MPI_Send operation
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* target = arglist[3]; assert(target);
    return target;
  }

  SgInitializedName* getSendOpTag(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPISendOp()) usageErrorExit(__LINE__);
   
    // We are processing MPI_Send operation
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* tag = arglist[4]; assert(tag);
    return tag;
  }

  SgInitializedName* getSendOpComm(Function mpifunc) { 
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPISendOp()) usageErrorExit(__LINE__);
   
    // We are processing MPI_Send operation
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* comm = arglist[5]; assert(comm);
    return comm;
  }

  SgInitializedName* getRecvOpSource(Function mpifunc) { 
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIRecvOp()) usageErrorExit(__LINE__);
   
    // We are processing MPI_Recv operation
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* source = arglist[3]; assert(source);
    return source;
  }

  SgInitializedName* getRecvOpTag(Function mpifunc) { 
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIRecvOp()) usageErrorExit(__LINE__);
   
    // We are processing MPI_Recv operation
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* tag = arglist[4]; assert(tag);
    return tag;
  }

  SgInitializedName* getRecvOpComm(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIRecvOp()) usageErrorExit(__LINE__);
   
    // We are processing MPI_Recv operation
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* comm = arglist[5]; assert(comm);
    return comm;
  }  

  SgInitializedName* getBarrierComm(Function mpifunc) { 
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIBarrierOp()) usageErrorExit(__LINE__);
    
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* comm = arglist[0]; assert(comm);
    return comm;    
  }

  SgInitializedName* getBcastOpRoot(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIBcastOp()) usageErrorExit(__LINE__);
    
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* root = arglist[3]; assert(root);
    return root;
  }

  SgInitializedName* getBcastOpComm(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIBcastOp()) usageErrorExit(__LINE__);
    
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* comm = arglist[4]; assert(comm);
    return comm;
  }

  SgInitializedName* getReduceOpSBuff(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIReduceOp()) usageErrorExit(__LINE__);
    
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* sbuff = arglist[0]; assert(sbuff);
    return sbuff;
  }

  SgInitializedName* getReduceOpRBuff(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIReduceOp()) usageErrorExit(__LINE__);
    
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* rbuff = arglist[1]; assert(rbuff);
    return rbuff;
  }

  SgInitializedName* getReduceOpRoot(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIReduceOp()) usageErrorExit(__LINE__);
    
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* root = arglist[5]; assert(root);
    return root;
  }

  SgInitializedName* getReduceOpComm(Function mpifunc) {
    MPICommOp op(mpifunc);
    if(op.isMPIUnknownOp()) unsupportedMPICommOpExit(__LINE__);
    if(!op.isMPIReduceOp()) usageErrorExit(__LINE__);
    
    SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
    SgInitializedNamePtrList& arglist = *arglist_p;
    SgInitializedName* comm = arglist[6]; assert(comm);
    return comm;
  }


  bool isReduceOpSbuffDerefExp(Function mpifunc, SgPointerDerefExp* sgn) {
    SgInitializedName* sbuff = getReduceOpSBuff(mpifunc);
    SgVarRefExp* sbuffVarRef = isSgVarRefExp(sgn->get_operand());
    if(sbuff && sbuffVarRef) {
      SgSymbol* sbuffSymbol = sbuff->search_for_symbol_from_symbol_table();
      SgVariableSymbol* sbuffVarRefSymbol = sbuffVarRef->get_symbol();
      if(!sbuffSymbol || !sbuffVarRefSymbol) return false;
      else return sbuffSymbol == sbuffVarRefSymbol;
    }
    return false;
  }

  bool isReduceOpSbuffDerefExp(Function mpifunc, SgPointerDerefExp* sgn, 
                               Composer* composer, PartEdgePtr pedge, ComposedAnalysis* analysis,
                               int debugLevel) {
    SIGHT_VERB_DECL(scope, ("isReduceOpSbuffDerefExp", scope::low),
                    3, debugLevel)
    SgInitializedName* sbuff = getReduceOpSBuff(mpifunc);
    SgVarRefExp* sbuffVarRef = isSgVarRefExp(sgn->get_operand());
    SIGHT_VERB(dbg << "sbuff=" << SgNode2Str(sbuff) << endl, 3, debugLevel)
    SIGHT_VERB(dbg << "sbuffVarRef=" << SgNode2Str(sbuffVarRef) << endl, 3, debugLevel)
    MemLocObjectPtr sbuffML = composer->Expr2MemLoc(sbuff, pedge, analysis);
    MemLocObjectPtr sbuffVarRefML = composer->OperandExpr2MemLoc(sgn, sbuffVarRef, pedge, analysis);
    SIGHT_VERB(dbg << "sbuffML=" << sbuffML->str() << endl, 3, debugLevel)
    SIGHT_VERB(dbg << "sbuffVarRefML=" << sbuffVarRefML->str() << endl, 3, debugLevel)
    return sbuffML->mustEqualML(sbuffVarRefML, pedge);
  }

  bool isReduceOpRbuffDerefExp(Function mpifunc, SgPointerDerefExp* sgn, 
                               Composer* composer, PartEdgePtr pedge, ComposedAnalysis* analysis,
                               int debugLevel) {
    SIGHT_VERB_DECL(scope, ("isReduceOpSbuffDerefExp", scope::low),
                    3, debugLevel)
    SgInitializedName* rbuff = getReduceOpRBuff(mpifunc);
    SgVarRefExp* rbuffVarRef = isSgVarRefExp(sgn->get_operand());
    SIGHT_VERB(dbg << "rbuff=" << SgNode2Str(rbuff) << endl, 3, debugLevel)
    SIGHT_VERB(dbg << "rbuffVarRef=" << SgNode2Str(rbuffVarRef) << endl, 3, debugLevel)
    MemLocObjectPtr rbuffML = composer->Expr2MemLoc(rbuff, pedge, analysis);
    MemLocObjectPtr rbuffVarRefML = composer->OperandExpr2MemLoc(sgn, rbuffVarRef, pedge, analysis);
    SIGHT_VERB(dbg << "rbuffML=" << rbuffML->str() << endl, 3, debugLevel)
    SIGHT_VERB(dbg << "rbuffVarRefML=" << rbuffVarRefML->str() << endl, 3, debugLevel)
    return rbuffML->mustEqualML(rbuffVarRefML, pedge);
  }

  // SgPointerDerefExp* MPICommOp::getCommOpBufferDerefExpr() const {
  //   SgPointerDerefExp* buffDerefExpr;
  //   if(isMPICommSendOp() || isMPICommRecvOp()) {
  //     SgInitializedNamePtrList* arglist_p = mpifunc.get_args();
  //     SgInitializedNamePtrList& arglist = *arglist_p;
  //     SgVariableSymbol* buffSymbol = isSgVariableSymbol(arglist[0]->search_for_symbol_from_symbol_table());
  //     assert(buffSymbol);
  //     SgVarRefExp* buffVarRefExpr = SageBuilder::buildVarRefExp(buffSymbol);
  //     buffDerefExpr = SageBuilder::buildUnaryExpression<SgPointerDerefExp>(buffVarRefExpr);
  //     assert(buffDerefExpr);
  //   }
  //   else assert(false);
  //   return buffDerefExpr;
  // }

} // end namespace
