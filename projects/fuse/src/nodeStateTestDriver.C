#include "sage3basic.h"
#include "compose.h"
#include "const_prop_analysis.h"

using namespace std;
using namespace fuse;
using namespace sight;

class BoolNodeStateML : public MemLocObject
{
  MemLocObjectPtr base;
public:
  BoolNodeStateML(MemLocObjectPtr ml) : MemLocObject(*ml), base(ml) { }  
  string str(string indent="") {
    ostringstream oss;
    oss << "[BoolNodeStateML: " << base->str() << "]";
    return oss.str();
  }
};

class BoolNodeStateAnalysisFW : public FWDataflow
{
 
  public:
  BoolNodeStateAnalysisFW() { }
  
  // Initializes the state of analysis lattices, for analyses that produce the same lattices above and below each node
  void genInitLattice(PartPtr part, PartEdgePtr pedge, std::vector<Lattice*>& initLattices) {
    initLattices.push_back((Lattice*)(new BoolAndLattice(0, pedge)));
  }

  ComposedAnalysisPtr copy() {
    return boost::make_shared<BoolNodeStateAnalysisFW>();
  }

  bool transfer(PartPtr p, CFGNode cn, NodeState& state, 
                std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo) {  
    
    Lattice* lattice = dfInfo[NULLPartEdge][0];
    BoolAndLattice* bal = dynamic_cast<BoolAndLattice*>(lattice);
    assert(bal);
    bool modified = bal->set(true);
    dbg << "BAL=" << bal->str() << endl;
    return modified;
  }

  bool implementsExpr2MemLoc() {
    return true;
  }

  MemLocObjectPtr Expr2MemLoc(SgNode* n, PartEdgePtr pedge) {
    scope reg("BoolNodeState::Expr2MemLoc", scope::high, attrGE("composedAnalysisDebugLevel", 1));
    MemLocObjectPtr parent = getComposer()->Expr2MemLoc(n, pedge, this);
    boost::shared_ptr<BoolNodeStateML> bml = boost::make_shared<BoolNodeStateML>(parent);
    if(pedge->source() && pedge->target()) {
      NodeState* state = NodeState::getNodeState(this, pedge->source());
      BoolAndLattice* bal = dynamic_cast<BoolAndLattice*>(state->getLatticeBelow(this, pedge, 0));
      assert(bal);
      dbg << "bal=" << bal->str() << endl;
    }
    else if(pedge->source()) {
      // outEdgeToAny
      NodeState* state = NodeState::getNodeState(this, pedge->source());
      BoolAndLattice* balA = dynamic_cast<BoolAndLattice*>(state->getLatticeBelow(this, pedge, 0));
      assert(balA);
      dbg << "balA=" << balA->str() << endl;
    }
    else if(pedge->target()) {
      // inEdgeFromAny
      NodeState* state = NodeState::getNodeState(this, pedge->target());
      BoolAndLattice* balB = dynamic_cast<BoolAndLattice*>(state->getLatticeAbove(this, pedge, 0));
      assert(balB);
      dbg << "balB=" << balB->str() << endl;
    }
    else assert(0);
    dbg << bml->str() << endl;
    return bml;
  }
  
  // pretty print for the object
  std::string str(std::string indent="")
  { return "BoolNodeStateAnalysisFW"; }
};

class BoolNodeStateAnalysisBW : public BWDataflow
{
 
  public:
  BoolNodeStateAnalysisBW() { }
  
  // Initializes the state of analysis lattices, for analyses that produce the same lattices above and below each node
  void genInitLattice(PartPtr part, PartEdgePtr pedge, std::vector<Lattice*>& initLattices) {
    initLattices.push_back((Lattice*)(new BoolAndLattice(0, pedge)));
  }

  ComposedAnalysisPtr copy() {
    return boost::make_shared<BoolNodeStateAnalysisBW>();
  }

  bool transfer(PartPtr p, CFGNode cn, NodeState& state, 
                std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo) {  
    
    Lattice* lattice = dfInfo[NULLPartEdge][0];
    BoolAndLattice* bal = dynamic_cast<BoolAndLattice*>(lattice);
    assert(bal);
    bool modified = bal->set(true);
    dbg << "BAL=" << bal->str() << endl;
    return modified;
  }

  bool implementsExpr2MemLoc() {
    return true;
  }

  MemLocObjectPtr Expr2MemLoc(SgNode* n, PartEdgePtr pedge) {
    scope reg("BoolNodeState::Expr2MemLoc", scope::high, attrGE("composedAnalysisDebugLevel", 1));
    MemLocObjectPtr parent = getComposer()->Expr2MemLoc(n, pedge, this);
    boost::shared_ptr<BoolNodeStateML> bml;
    if(pedge->source() && pedge->target()) {
      NodeState* state = NodeState::getNodeState(this, pedge->source());
      BoolAndLattice* bal = dynamic_cast<BoolAndLattice*>(state->getLatticeBelow(this, pedge, 0));
      assert(bal);
      if(bal) bml = boost::make_shared<BoolNodeStateML>(parent);
      dbg << "bal=" << bal->str() << endl;
    }
    else if(pedge->source()) {
      // outEdgeToAny
      NodeState* state = NodeState::getNodeState(this, pedge->source());
      BoolAndLattice* balA = dynamic_cast<BoolAndLattice*>(state->getLatticeBelow(this, pedge, 0));
      assert(balA);
      dbg << "balA=" << balA->str() << endl;
      if(balA) bml = boost::make_shared<BoolNodeStateML>(parent);      
    }
    else if(pedge->target()) {
      // inEdgeFromAny
      NodeState* state = NodeState::getNodeState(this, pedge->target());
      BoolAndLattice* balB = dynamic_cast<BoolAndLattice*>(state->getLatticeAbove(this, pedge, 0));
      assert(balB);
      dbg << "balB=" << balB->str() << endl;
      if(balB) bml = boost::make_shared<BoolNodeStateML>(parent);      
    }
    else assert(0);
    assert(bml);
    dbg << bml->str() << endl;
    return bml;
  }
  
  // pretty print for the object
  std::string str(std::string indent="")
  { return "BoolNodeStateAnalysisBW"; }
};

class BoolMLQuery : public FWDataflow
{
public:
  BoolMLQuery() { }

  void genInitLattice(PartPtr part, PartEdgePtr pedge, std::vector<Lattice*>& initLattices) {
    initLattices.push_back((Lattice*)(new BoolAndLattice(0, pedge)));
  }

  ComposedAnalysisPtr copy() {
    return boost::make_shared<BoolMLQuery>();
  }

  bool transfer(PartPtr p, CFGNode cn, NodeState& state, 
                std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo) {
    SgNode* sgn = cn.getNode();
    if(isSgExpression(sgn)) {
      MemLocObjectPtr mlf = getComposer()->Expr2MemLoc(sgn, p->inEdgeFromAny(), this);
      MemLocObjectPtr mlb = getComposer()->Expr2MemLoc(sgn, p->outEdgeToAny(), this);
      assert(mlf && mlb);
      dbg << "BoolMLQueryFW: " << mlf->str() << endl;
      dbg << "BoolMLQueryBW: " << mlb->str() << endl;
    }
    return false;
  }

  std::string str(string indent) {
    return "BoolMLQuery";
  }
};

int main(int argc, char** argv)
{
  FuseInit(argc, argv);  
  printf("========== S T A R T ==========\n");
  SgProject* project = frontend(argc, argv); 
  printf("Frontend done\n");fflush(stdout);

  list<ComposedAnalysis*> mySubAnalyses;
  // ConstantPropagationAnalysis* cpa = new ConstantPropagationAnalysis();
  BoolNodeStateAnalysisFW* bnsafw = new BoolNodeStateAnalysisFW();
  BoolNodeStateAnalysisBW* bnsabw = new BoolNodeStateAnalysisBW();
  BoolMLQuery* bmlq = new BoolMLQuery();
  checkDataflowInfoPass* cdip = new checkDataflowInfoPass();  

  // mySubAnalyses.push_back(cpa);
  mySubAnalyses.push_back(bnsafw);
  mySubAnalyses.push_back(bmlq);
  mySubAnalyses.push_back(bnsabw);
  mySubAnalyses.push_back(bmlq);
  ChainComposer* cc = new ChainComposer(mySubAnalyses, cdip, true);
  cc->runAnalysis();
  printf("==========  E  N  D  ==========\n");
  return 0;
}
