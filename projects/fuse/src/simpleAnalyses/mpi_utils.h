#ifndef _MPI_UTILS_H
#define _MPI_UTILS_H

#include "CallGraphTraverse.h"
#include "compose.h"

namespace fuse {

  /*********
   * MPIOp *
   *********/
  //! List of Supported MPI Operations in the ParFuse framework
  class MPICommOp {
    enum OpType {
      COMMRANK,
      COMMSIZE,
      SEND,
      RECV,
      BARRIER,
      BCAST,
      REDUCE,
      INIT,
      FINALIZE,
      UNKNOWN
    };
    OpType optype;
  public:
    MPICommOp(const Function& func);
    MPICommOp(const MPICommOp& that);
    bool isMPISendOp() const;
    bool isMPIRecvOp() const;
    bool isMPIBarrierOp() const;
    bool isMPIBcastOp() const;
    bool isMPIReduceOp() const;
    bool isMPIUnknownOp() const;
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

  /**********************
   * MPIBcastOpCallSite *
   **********************/
  class MPIBcastOpCallSite {
    const Function mpif_;
    SgFunctionCallExp* sgn;
  public:
    MPIBcastOpCallSite(const Function& mpif_, SgFunctionCallExp* sgn);
    SgNode* getBcastBuffer();
    SgNode* getBcastRootExpr();
  };

  /***********************
   * MPIReduceOpCallSite *
   ***********************/
  class MPIReduceOpCallSite {
    const Function mpif_;
    SgFunctionCallExp* sgn;
  public:
    MPIReduceOpCallSite(const Function& mpif_, SgFunctionCallExp* sgn);
    SgNode* getReduceSBuffer();
    SgNode* getReduceRBuffer();
    SgNode* getReduceRootExpr();
  };

  /*******************
   * ValueObject2Int *
   *******************/
  class ValueObject2Int {
    Composer* composer;
    PartEdgePtr pedge;
    ComposedAnalysis* analysis;
    int debugLevel;
  public:
    ValueObject2Int(Composer* composer, PartEdgePtr pedge, 
                    ComposedAnalysis* analysis, int debugLevel);
    int operator()(SgNode* sgn);
  };

  /*********************
   * OpValueObject2Int *
   *********************/
  class OpValueObject2Int {
    Composer* composer;
    PartEdgePtr pedge;
    ComposedAnalysis* analysis;
    int debugLevel;
  public:
    OpValueObject2Int(Composer* composer, PartEdgePtr pedge, 
                    ComposedAnalysis* analysis, int debugLevel);
    int operator()(SgNode* anchor, SgNode* op);
  };

  //! Traverse the expression tree to find the buffer expr for the MPI operations
  SgExpression* traverseAST(SgExpression* expr);
  SgInitializedName* getSendOpTarget(Function function);
  SgInitializedName* getSendOpTag(Function function);
  SgInitializedName* getSendOpComm(Function function);

  SgInitializedName* getRecvOpSource(Function function);
  SgInitializedName* getRecvOpTag(Function function);
  SgInitializedName* getRecvOpComm(Function function);  

  SgInitializedName* getBarrierComm(Function function);

  SgInitializedName* getBcastOpRoot(Function function);
  SgInitializedName* getBcastOpComm(Function function);

  SgInitializedName* getReduceOpSbuff(Function function);
  SgInitializedName* getReduceOpRbuff(Function function);
  SgInitializedName* getReduceOpRoot(Function mpifunc);
  SgInitializedName* getReduceOpComm(Function mpifunc);

  bool isReduceOpSbuffDerefExp(Function mpifunc, SgPointerDerefExp* sgn);
  bool isReduceOpSbuffDerefExp(Function mpifunc, SgPointerDerefExp* sgn, 
                               Composer* composer, PartEdgePtr pedge, ComposedAnalysis* analysis,
                               int debugLevel);
  bool isReduceOpRbuffDerefExp(Function mpifunc, SgPointerDerefExp* sgn, 
                               Composer* composer, PartEdgePtr pedge, ComposedAnalysis* analysis,
                               int debugLevel);
};

#endif
