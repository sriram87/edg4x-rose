#ifndef _MPI_COMM_ANALYSIS_H
#define _MPI_COMM_ANALYSIS_H

/****************************************
 * Author: Sriram Aananthakrishnan, 2014 *
 *****************************************/

#include "compose.h"

namespace fuse {

  class MPICommAnalysis;

  /*********
   * MPIOp *
   *********/
  //! Type of MPI operations
  class MPIOp {
    enum MPIOp_t { SEND, RECV, ISEND, IRECV, BARRIER, NOOP};
    MPIOp_t op;
  public:
    MPIOp(const Function& mpif);
    MPIOp(const MPIOp& that);
    bool operator<(const MPIOp& that) const;
    bool operator==(const MPIOp& that) const;
  };

  /************
   * MPIOpAbs *
   ************/
  class MPIOpAbs;
  typedef CompSharedPtr<MPIOpAbs> MPIOpAbsPtr;

  //! Abstract base class for grouping MPI operations.
  class MPIOpAbs {
  public:    
    virtual bool operator==(const MPIOpAbsPtr& that) const = 0;
    virtual bool operator<(const MPIOpAbsPtr& that) const = 0;
    virtual std::string str(std::string indent="") const = 0;
  };

  // /****************
  //  * MPIOpAbsType *
  //  ****************/
  // //! Group MPI operations by the type of MPI operation
  // class MPIOpAbsType : public MPIOpAbs {
  //   MPIOp op;
  // public:
  //   MPIOpAbsType(const Function& mpif);
  //   MPIOpAbsType(const MPIOpAbsType& that);
  //   bool operator<(const MPIOpAbsPtr& that) const;
  //   bool operator==(const MPIOpAbsPtr& that) const;
  // };

  // typedef CompSharedPtr<MPIOpAbsType> MPIOpAbsTypePtr;

  /*********************
   * MPIOpAbsTargetVal *
   *********************/
  //! Group MPI operations by type and value of target expression
  //  class MPIOpAbsTargetVal : public MPIOpAbs {
  //  };
  //  typedef CompSharedPtr<MPIOpAbsTargetVal> MPIOpAbsTargetValPtr;

  /********************
   * MPIOpAbsCallSite *
   ********************/
  //! Group MPI operations by type and call site
  class MPIOpAbsCallSite : public MPIOpAbs {
    MPIOp op;
    //! Part that denotes the callsite at which this call was made
    PartPtr callsite;
  public:
    MPIOpAbsCallSite(const Function& mpif, PartPtr pCallSite);
    MPIOpAbsCallSite(const MPIOpAbsCallSite& that);
    bool operator<(const MPIOpAbsPtr& that) const;
    bool operator==(const MPIOpAbsPtr& that) const;
    std::string str(std::string indent="") const;
  };
  typedef CompSharedPtr<MPIOpAbsCallSite> MPIOpAbsCallSitePtr;

  //! Methods for creating MPI operation abstraction
  // MPIOpAbsPtr createMPIOpAbs(const Function& mpif);
  MPIOpAbsPtr createMPIOpAbs(PartPtr callsite);

  /***************
   * CommContext *
   ***************/
  //! Contexts are additional information used to refine a function entry Part reachable through multiple edges.
  //! Abstract based class for context associated with parts of CommATS.
  //! Subclassed by two types of context MPICommContext and NonMPICommContext.
  //! MPICommContext are used only at call sites of MPI operations
  class CommContext : public PartContext {
  public:
    std::list<PartContextPtr> getSubPartContexts() const=0;
    //! For two parts that are equal both parts have NonMPICommContexts or MPICommContexts.
    //! This is due to association of MPICommContext only with MPI functions.
    //! The cases where two parts are equal and one having a NonMPICommContext and
    //! the other having MPICommContext is rare or impossible.
    //! However such a case should arise for two equal parts order its NonMPICommContext before its MPICommContext.
    bool operator<(const PartContextPtr& that) const=0;
    //! For two parts that are equal both parts have NonMPICommContexts or MPICommContexts.
    //! This is due to association of MPICommContext only with MPI functions.
    //! The cases where two parts are equal and one having a NonMPICommContext and
    //! the other having MPICommContext is rare or impossible.
    //! However such a case should arise for two equal parts order its NonMPICommContext before its MPICommContext.
    bool operator==(const PartContextPtr& that) const=0;
  };
  typedef CompSharedPtr<CommContext> CommContextPtr;

  /******************
   * MPICommContext *
   ******************/
  //! Provides a context to CommATSPart based on the MPI operation used.
  //! Two MPICommContext are differentiated based on the abstraction used for MPI operations.
  //! The MPI operation abstraction selectively blows up the context of MPI call sites
  //! Communication analysis carries out the semantics of MPI operation by issuing the operation
  //! to runtime based on the context.
  //! Each context is executed at least once by the communication analysis.
  class MPICommContext : public CommContext {
    MPIOpAbsPtr mpiopabs_p;
  public:
    MPICommContext(MPIOpAbsPtr mpiopabs_p);
    MPICommContext(const MPICommContext& that);
    std::list<PartContextPtr> getSubPartContexts() const;
    bool operator<(const PartContextPtr& that) const;
    bool operator==(const PartContextPtr& that) const;
    std::string str(std::string indent="") const;
  };
  typedef CompSharedPtr<MPICommContext> MPICommContextPtr;

  //! All non MPI parts are assigned this context.
  //! Two NonMPICommContexts are differentiated based on the parent context.
  /*********************
   * NonMPICommContext *
   *********************/
  class NonMPICommContext : public CommContext {
    PartContextPtr parentContext_p;
  public:
    NonMPICommContext(PartContextPtr parentContext_p);
    NonMPICommContext(const NonMPICommContext& that);
    std::list<PartContextPtr> getSubPartContexts() const;
    bool operator<(const PartContextPtr& that) const;
    bool operator==(const PartContextPtr& that) const;
    std::string str(std::string indent="") const;
  };
  typedef CompSharedPtr<NonMPICommContext> NonMPICommContextPtr;

  /***************
   * CommATSPart *
   ***************/
  class CommContextLattice;
  class CommATSPart;
  typedef CompSharedPtr<CommATSPart> CommATSPartPtr;

  class CommATSPart : public Part {
    CommContextPtr context;
    MPICommAnalysis* mpicommanalysis_p;
    PartPtr base;
  public:
    CommATSPart(PartPtr base, MPICommAnalysis* analysis, CommContextPtr context);
    CommATSPart(const CommATSPart& that);

    CommATSPartPtr get_shared_this();

    // Part interface
    std::list<PartEdgePtr> outEdges();
    std::list<PartEdgePtr> inEdges();

    std::set<CFGNode> CFGNodes() const;

    std::set<PartPtr> matchingCallParts() const;

    PartEdgePtr outEdgeToAny();
    PartEdgePtr inEdgeFromAny();

    bool equal(const PartPtr& that) const;
    bool less(const PartPtr& that) const;

    std::string str(std::string indent="") const;

    // Helper methods
    //! Get the CommContextLattice above from NodeState
    //! @param part NodeState at this part
    //! @param pedge Lattice info along this pedge
    CommContextLattice* getCommContextLatticeAbove(PartPtr part, PartEdgePtr pedge);
    //! Get the CommContextLattice below from NodeState
    //! @param part NodeState at this part
    //! @param pedge Lattice info along this pedge
    CommContextLattice* getCommContextLatticeBelow(PartPtr part, PartEdgePtr pedge);
    //! Checks if NonMPICommContext is preserved along edgefrom and edgeto
    //! @param edgefromccl_p CommContextLattice on edgefrom
    //! @param edgetoccl_p CommContextLattice on edgeto
    bool contextFromNonMPItoNonMPI(CommContextLattice* edgefromccl_p, CommContextLattice* edgetoccl_p);
    //! Checks if CommContext switches from NonMPI to MPI along edgefrom and edgeto
    //! @param edgefromccl_p CommContextLattice on edgefrom
    //! @param edgetoccl_p CommContextLattice on edgeto
    bool contextFromNonMPItoMPI(CommContextLattice* edgefromccl_p, CommContextLattice* edgetoccl_p);
    //! Checks if CommContext switches from MPI to NonMPI along edgefrom and edgeto
    //! @param edgefromccl_p CommContextLattice on edgefrom
    //! @param edgetoccl_p CommContextLattice on edgeto
    bool contextFromMPItoNonMPI(CommContextLattice* edgefromccl_p, CommContextLattice* edgetoccl_p);
    //! Checks if MPICommContext is preserved along edgefrom and edgeto
    //! @param edgefromccl_p CommContextLattice on edgefrom
    //! @param edgetoccl_p CommContextLattice on edgeto
    bool contextFromMPItoMPI(CommContextLattice* edgefromccl_p, CommContextLattice* edgetoccl_p);
  };  

  /*******************
   * CommATSPartEdge *
   *******************/
  class CommATSPartEdge;
  typedef CompSharedPtr<CommATSPartEdge> CommATSPartEdgePtr;

  class CommATSPartEdge : public PartEdge {
    PartEdgePtr base;
    CommATSPartPtr src, tgt;
  public:
    CommATSPartEdge(PartEdgePtr base, MPICommAnalysis* analysis, CommATSPartPtr source, CommATSPartPtr target);
    CommATSPartEdge(const CommATSPartEdge& that);

    PartPtr source() const;
    PartPtr target() const;

    std::list<PartEdgePtr> getOperandPartEdge(SgNode* anchor, SgNode* operand);
    std::map<CFGNode, boost::shared_ptr<SgValueExp> > getPredicateValue();

    bool equal(const PartEdgePtr& that) const;
    bool less(const PartEdgePtr& that) const;

    std::string str(std::string indent="") const;
  };  

  /**********************
   * CommContextLattice *
   **********************/
  class CommContextLattice : public FiniteLattice {
    enum CommContextLatticeElem {
      NOCONTEXT,
      NONMPICOMMCONTEXT,
      MPICOMMCONTEXT,
      UNKNOWN
    };

    CommContextLatticeElem elem;
  public:
    CommContextLattice(PartEdgePtr edge_p);
    CommContextLattice(const CommContextLattice& that);

    CommContextLatticeElem getCCLatElem() const;

    void initialize();
    bool setCCLatElemMPI();
    bool setCCLatElemNonMPI();
    bool setToFull();
    bool setToEmpty();
    bool meetUpdate(Lattice* that);
    Lattice* copy() const;
    void copy(Lattice* that);
    bool copy(CommContextLattice* that);
    bool operator==(Lattice* that);
    bool isCCLatElemMPI() const;
    bool isCCLatElemNonMPI() const;
    bool isFullLat();
    bool isEmptyLat();
    bool setMLValueToFull(MemLocObjectPtr ml_p);
    std::string str(std::string indent="") const;
  };

  /*******************
   * MPICommAnalysis *
   *******************/
  typedef std::set<CommATSPartPtr> CAPartSet;
  typedef std::map<CommATSPartPtr, CAPartSet> CAPart2CAPartSetMap;
  typedef std::pair<CommATSPartPtr, CAPartSet> CAPart2CAPartSetMapElement;

  class MPICommAnalysis : public FWDataflow {
  public:
    MPICommAnalysis();
    virtual void initAnalysis(std::set<PartPtr>& startingParts);

    ComposedAnalysisPtr copy() { return boost::make_shared<MPICommAnalysis>(); }

    // Initializes the state of analysis lattices at the given function, part and edge into our out of the part
    // by setting initLattices to refer to freshly-allocated Lattice objects.
    void genInitLattice(PartPtr part, PartEdgePtr pedge, 
                        std::vector<Lattice*>& initLattices);

    bool transfer(PartPtr part, CFGNode cn, NodeState& state, 
                  std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo);
   
    bool implementsExpr2MemRegion() { return false; }
    bool implementsExpr2MemLoc() { return false; }
    bool implementsExpr2Val() { return false; }

    //! Fuse functionality implemented by this analysis
    bool implementsATSGraph() { return true; }

    bool isMPIFuncCall(SgFunctionCallExp* sgn);

    //! Return the start abstract state (Part) of the application
    std::set<PartPtr> GetStartAStates_Spec();
    //! Return the end abstract state (Part) of the application
    std::set<PartPtr> GetEndAStates_Spec();

    CommATSPartPtr getCommATSPartPtr(PartPtr part_p);
    CAPartSet get_pred(PartPtr part_p);
    CAPartSet get_succ(PartPtr part_p);

  private:
    CAPartSet get_pred(CommATSPartPtr cap_p);
    CAPartSet get_succ(CommATSPartPtr cap_p);

  public:    
    // pretty print for the object
    std::string str(std::string indent="") const;
  };
};

#endif
