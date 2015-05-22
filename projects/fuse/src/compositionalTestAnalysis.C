#include "sage3basic.h"
#include "compose.h"
#include "tight_composer.h"
#include "const_prop_analysis.h"
#include "live_dead_analysis.h"
#include "call_context_sensitivity_analysis.h"
#include "dead_path_elim_analysis.h"
#include "printAnalysisStates.h"
#include "pointsToAnalysis.h"
#include "virtualMethodAnalysis.h"
#include "dynamicMonitor.h"
//#include "constantAnalysis.h"
//#include "valueNumbering.h"
#include "defsAnalysis.h"
#include "analysis_tester.h"
#include <vector>
#include <ctype.h>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include "sageBuilder.h"
#include "sageInterface.h"
#include "sight.h"
using namespace sight;
#include <sys/time.h>
#include "fuse.h"

#include "staticCFG.h"
#include "interproceduralCFG.h"

using namespace std;
using namespace fuse;
using namespace boost::xpressive;
using namespace SageBuilder;
using namespace SageInterface;
using namespace fuse;

// Prints out the Fuse CFG, including the string representations of nodes and edges
void printFuseCFG(SgProject* project, Fuse& f);

void FuseSSA2DOT(SgProject* project, Fuse& f, const std::string& fname);
void FuseSSA2DOT(SgProject* project, Fuse& f, ostream& dot);

int main(int argc, char** argv)
{
  printf("========== S T A R T ==========\n");

  Rose_STL_Container<string> args = CommandlineProcessing::generateArgListFromArgcArgv(argc, argv);
  // Strip the dataflow analysis options

  // Run the front end
  struct timeval start, end;
  gettimeofday(&start, NULL);

  SgProject* project = frontend(argc, argv);

  gettimeofday(&end, NULL);
  cout << "  Front End Elapsed="<<((end.tv_sec*1000000+end.tv_usec) -
                                               (start.tv_sec*1000000+start.tv_usec))/1000000.0<<"s"<<endl;

  //generatePDF(*project);

#if 0
  // DQ (6/1/2014): Added generation of graphs to support debugging.
     generateDOTforMultipleFile(*project);

  // Output an optional graph of the AST (the whole graph, of bounded complexity, when active)
     const int MAX_NUMBER_OF_IR_NODES_TO_GRAPH_FOR_WHOLE_GRAPH = 10000;
     generateAstGraph(project,MAX_NUMBER_OF_IR_NODES_TO_GRAPH_FOR_WHOLE_GRAPH,"");
#endif

  //printf("Frontend done\n");fflush(stdout);

  string fuseCmd = "";
  Rose_STL_Container<string> dataflowoptions = CommandlineProcessing::generateOptionList(args, "-fuse:");
  //std::vector<std::string>  dataflowoptions = project->get_originalCommandLineArgumentList();
  if(dataflowoptions.size()>1) { cout << "ERROR: too many -dataflow options!"; exit(-1); }
  // If the caller provided a description of the composition structure on the command line, set fuseCmd to it
  else if(dataflowoptions.size()==1) {
    fuseCmd = dataflowoptions[0];
  // Otherwise, read it from the fuse pragmas in the source
  } else {
    Rose_STL_Container<SgNode*> pragmas = NodeQuery::querySubTree(project, V_SgPragma);
    for(Rose_STL_Container<SgNode*>::iterator p=pragmas.begin(); p!=pragmas.end(); p++) {
      SgPragma* pragma = isSgPragma(*p);
      assert(pragma);

      //cout << "pragma: "<< pragma->get_pragma() << endl;
      sregex pragmaLine = *_s >> as_xpr("fuse") >> *_s >> (s1=+~_n);
      boost::xpressive::smatch what;

      if(boost::xpressive::regex_match(pragma->get_pragma(), what, pragmaLine)) {
        assert(what.size()==2);
        fuseCmd.append(what[1]);
      }
    }
  }

/*
  // Remove leading spaces
  unsigned int startNonSpace=0;
  while(startNonSpace<fuseCmd.length() && (fuseCmd[startNonSpace]==' ' || fuseCmd[startNonSpace]=='\t')) startNonSpace++;
  if(startNonSpace>0) fuseCmd.erase(0, startNonSpace);

  // Remove trailing spaces
  unsigned int endNonSpace=fuseCmd.length()-1;
  while(endNonSpace>=0 && (fuseCmd[endNonSpace]==' ' || fuseCmd[endNonSpace]=='\t')) endNonSpace--;
  if(endNonSpace<fuseCmd.length()-1) fuseCmd.erase(endNonSpace+1);*/

  cout << "fuseCmd = \""<<fuseCmd<<"\"\n";

  // If this is a command for the compositional framework
  if(fuseCmd.size()>0) {
    Fuse f(argc, argv);
    f.run(project, fuseCmd);
    //f.cfgToDot("cfg.dot");
    //printFuseCFG(project, f);
    //FuseSSA2DOT(project, f, "ssa.dot");
    f.placeConstantPropagationAnnotations(/*verbose*/ true);
    f.placeUseDefAnnotations(/*verbose*/ true);

  } else
    cout << "ERROR: no Fuse command specified either on the command line or as a pragma inside the target files!"<<endl;

  cout << "==========  E  N  D  ==========\n";

  return backend (project);
  //return 0;
}

class FuseCFGMap {
  public:
  virtual void mapNode(FuseCFGNodePtr node) {}
  virtual void mapEdge(FuseCFGEdgePtr edge) {}
  
  void iterateOnceOverFuseCFG(std::set<FuseCFGNodePtr> startStates) {
    std::set<FuseCFGNodePtr> worklist = startStates;
    std::set<FuseCFGNodePtr> visited;
    while(worklist.size()>0) {
      FuseCFGNodePtr cur = *worklist.begin();
      worklist.erase(worklist.begin());
      visited.insert(cur);
  
      mapNode(cur);
  
      vector<FuseCFGEdgePtr> out=cur->outEdges();
      for(vector<FuseCFGEdgePtr>::iterator e=out.begin(); e!=out.end(); ++e) {
        if(visited.find((*e)->target()) == visited.end()) {
          mapEdge(*e);
  
          worklist.insert((*e)->target());
        }
      }
    }
  }
};


// Prints out the Fuse CFG, including the string representations of nodes and edges
void printFuseCFG(SgProject* project, Fuse& f) {
  StaticCFG::CFG icfg(project, /*is_filtered*/ false);
  std::set<CFGNode> worklist;
  CFGNode entry = icfg.toCFGNode(icfg.getEntry());
  //cout << "entry = "<<CFGNode2Str(entry)<<endl;
  worklist.insert(entry);
  std::set<CFGNode> visited;
  while(worklist.size()>0) {
    CFGNode cur = *worklist.begin();
    worklist.erase(worklist.begin());
    visited.insert(cur);

    cout << "node: "<<CFGNode2Str(cur)<<endl;
    std::set<FuseCFGNodePtr> FuseNodes = f.GetATSNodes(cur);
    for(std::set<FuseCFGNodePtr>::iterator n=FuseNodes.begin(); n!=FuseNodes.end(); ++n)
      cout << "    "<<(*n)->toString()<<endl;

    vector<CFGEdge> out=cur.outEdges();
    for(vector<CFGEdge>::iterator e=out.begin(); e!=out.end(); ++e) {
      if(visited.find(e->target()) == visited.end()) {
        cout << "edge: "<<CFGEdge2Str(*e)<<endl;
        std::set<FuseCFGEdgePtr> FuseEdges = f.GetATSEdges(*e);
        for(std::set<FuseCFGEdgePtr>::iterator fe=FuseEdges.begin(); fe!=FuseEdges.end(); ++fe)
          cout << "    "<<(*fe)->toStringFuse()<<endl;

        worklist.insert(e->target());
      }
    }
  }
}

// Relaces all instances of string search in subject with replace
// From http://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-of-a-string-with-another-string
void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}


void FuseSSA2DOT(SgProject* project, Fuse& f, const std::string& fname) {
  ofstream ofile (fname.c_str(), ios::out);
  FuseSSA2DOT(project, f, ofile);
}

void FuseSSA2DOT(SgProject* project, Fuse& f, ostream& dot) {
  dot << "digraph SSA {"<<endl;

  std::set<FuseCFGNodePtr> startStates = f.GetStartATSNodes();

  // First, assign unique nodeIDs to all parts in the ATS and emit node descriptions for all parts
  class PartIDMap: public FuseCFGMap {
    public:
    std::map<FuseCFGNodePtr, int> partID;
    ostream& dot;
    PartIDMap(ostream& dot): dot(dot) {}
    void mapNode(FuseCFGNodePtr node) {
      int ID=partID.size();
      string mlLabel = node->toString();
      // Replace all the line breaks in mlLabel with explicit text "\n";
      ReplaceStringInPlace(mlLabel, "\n", "\\n");
      dot << "node"<<ID<<" [label=\""<<mlLabel<<"\"";
      if(node->isPhiNode())
        dot << " color=red"<<endl;
      dot << "];"<<endl;
      partID[node] = ID;

    }
  };
  PartIDMap pidM(dot);
  pidM.iterateOnceOverFuseCFG(startStates);

  // Next, add transition system edges
  /*for(fw_graphEdgeIterator<FuseCFGEdgePtr, FuseCFGNodePtr> state(startStates, / *incrementalGraph* / false); !state.isEnd(); state++) {
    PartPtr src = state.getPartEdge()->source();
    PartPtr tgt = state.getPartEdge()->target();
    dot << "node"<<partID[src]<<" -> node"<<partID[tgt]<<";"<<endl;
  }*/
  class TransEdgesMap: public FuseCFGMap {
    public:
    ostream& dot;
    std::map<FuseCFGNodePtr, int> partID;
    TransEdgesMap(ostream& dot, std::map<FuseCFGNodePtr, int>& partID): dot(dot), partID(partID) {}
    void mapNode(FuseCFGNodePtr node) {
      vector<FuseCFGEdgePtr> out = node->outEdges();
      for(vector<FuseCFGEdgePtr>::iterator e=out.begin(); e!=out.end(); e++) {
        dot << "node"<<partID[node]<<" -> node"<<partID[(*e)->target()]<<";"<<endl;
      }
    }
  };
  TransEdgesMap teM(dot,pidM.partID);
  teM.iterateOnceOverFuseCFG(startStates);

  // Add edges for def-use relations
  //dot << "DefUseMap"<<endl;
  class DefUseMap: public FuseCFGMap {
    public:
    ostream& dot;
    std::map<FuseCFGNodePtr, int> partID;
    Fuse& f;
    DefUseMap(ostream& dot, std::map<FuseCFGNodePtr, int>& partID, Fuse& f): dot(dot), partID(partID), f(f) {}
    void mapNode(FuseCFGNodePtr node) {
      std::set<SSAMemLocObjectPtr> uses = node->getUses();
      //dot << "part="<<node->getPart()->str()<<" #uses="<<uses.size()<<endl;
      for(std::set<SSAMemLocObjectPtr>::iterator u=uses.begin(); u!=uses.end(); ++u) {
        const std::set<SSAMemLocObjectPtr>& defs = f.getDirectDefs(*u);
        for(std::set<SSAMemLocObjectPtr>::const_iterator d=defs.begin(); d!=defs.end(); ++d) {
          string mlLabel = (*d)->str();
          // Replace all the line breaks in mlLabel with explicit text "\n";
          ReplaceStringInPlace(mlLabel, "\n", "\\n");
          dot << "node"<<partID[(*d)->getFuseCFGNodeLoc(&f)]<<" -> node"<<partID[(*u)->getFuseCFGNodeLoc(&f)]<<" [label=\""<<mlLabel<<"\" color=blue];"<<endl;
        }
      }
    }
  };
  DefUseMap duM(dot,pidM.partID, f);
  duM.iterateOnceOverFuseCFG(startStates);
  
  // Add edges for def-phiDef relations
  //dot << "DefPhiDefMap"<<endl;
  class DefPhiDefMap: public FuseCFGMap {
    public:
    ostream& dot;
    std::map<FuseCFGNodePtr, int> partID;
    Fuse& f;
    DefPhiDefMap(ostream& dot, std::map<FuseCFGNodePtr, int>& partID, Fuse& f): dot(dot), partID(partID), f(f) {}
    void mapNode(FuseCFGNodePtr node) {
      if(node->isPhiNode()) {
        const map<SSAMemLocObjectPtr, std::set<SSAMemLocObjectPtr> >& phiDU = node->getDefsUsesAtPhiNode();
        //dot << "#phiDU="<<phiDU.size()<<endl;
        for(map<SSAMemLocObjectPtr, std::set<SSAMemLocObjectPtr> >::const_iterator phiD=phiDU.begin(); phiD!=phiDU.end(); ++phiD) {
          for(std::set<SSAMemLocObjectPtr>::const_iterator def=phiD->second.begin(); def!=phiD->second.end(); ++def) {
            string mlLabel = phiD->first->str();
            // Replace all the line breaks in mlLabel with explicit text "\n";
            ReplaceStringInPlace(mlLabel, "\n", "\\n");
            dot << "node"<<partID[(*def)->getFuseCFGNodeLoc(&f)]<<" -> node"<<partID[phiD->first->getFuseCFGNodeLoc(&f)]<<" [label=\""<<mlLabel<<"\" color=green];"<<endl;
          }
        }
      }
    }
  };
  DefPhiDefMap dpdM(dot,pidM.partID, f);
  dpdM.iterateOnceOverFuseCFG(startStates);

  dot << "}"<<endl;
}
