#include "sage3basic.h"
#include "compose.h"
#include "mpi_annotate_ast.h"
#include "fuseCommandParser.h"
#include "mpi_value_analysis.h"
#include "const_prop_analysis.h"
#include "dead_path_elim_analysis.h"
#include "tight_composer.h"
#include "mpi_comm_context_analysis.h"
#include "call_context_sensitivity_analysis.h"
#include "pointsToAnalysis.h"
#include "mpi_comm_analysis.h"
#include "mpi_dot_value_analysis.h"
#include "address_taken_analysis.h"
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include <exception>
#include "sight.h"
#include "mpi.h"


using namespace std;
using namespace fuse;
using namespace sight;
using namespace boost::xpressive;

void FuseMPIInit(int argc, char** argv) {
  // Command to set up the enviroment variable to find the binary fuseLayout
  // fuseLayout is required to run fuse
  // fuseLayout binary is at the same level as fuse in the build tree
  // When compiling fuse ROSE_PREFIX is defined as -DROSE_PREFIX="\"${top_builddir}\"" which
  // is top of the build tree
  // If fuse fails to find fuseLayout set up this environment variable appropriately. 
  // setenv("SIGHT_LAYOUT_EXEC", (txt()<<ROSE_PREFIX<<"/projects/fuse/src/fuseLayout").c_str(), 1);

  int flag = false;
  MPI_Initialized(&flag); assert(flag);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  string title = (txt() << "Process " << rank << " Debug Output").c_str();
  string workdir = (txt() << "dbg" << rank).c_str();
  SightInit(argc, argv, title, workdir);
}

int main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);
  FuseMPIInit(argc, argv);
  cout << "========== S T A R T ==========\n";

  // Run the front end
  SgProject* project = frontend(argc, argv);

  AnnotateMPISideEffects annotateMPI;
  annotateMPI.traverseInputFiles(project, preorder);

  printf("Frontend done\n");fflush(stdout);

  std::list<ComposedAnalysis*> scanalyses;
  std::list<ComposedAnalysis*> tcanalyses;

  // Check if the analysis sequence is described as pragmas
  sregex cmd_regex = *_s >> as_xpr("fuse") >> *_s >> (s1=+~_n);
  smatch what;
  string cmd_s;

  Rose_STL_Container<SgNode*> pragmas = NodeQuery::querySubTree(project, V_SgPragma);
  for(Rose_STL_Container<SgNode*>::iterator p=pragmas.begin(); p!=pragmas.end(); p++) {
    SgPragma* pragma = isSgPragma(*p);
    assert(pragma);

    // currently processing only one fuse command
    if(regex_match(pragma->get_pragma(), what, cmd_regex)) {
      assert(what.size() == 2);
      cmd_s.append(what[1]);
      break;
    }
  }

  if(cmd_s.length() == 0) {
    cerr << "No Fuse Command Found!" << endl;
  }

  FuseCommandParser parser;
  FuseCommand* cmd = parser(cmd_s);
  cmd->initFuseCommand();
  cmd->execute();
  if(cmd->hasMPIDotValue()) {
    MPIDotValueAnalysis* mdvanalysis = cmd->getLastMPIDotValueAnalysis();
    MPIDotGraphGenerator dotgen(mdvanalysis);
    dotgen.generateDot();
    dotgen.generateDotFile();
  }

  // FuseMPIDotValueCommand dotvalueanaysis;
  // dotvalueanaysis.execute();
  // dotvalueanaysis.generateDot();

  // init_sregex();

  // run_analysis_composition_cmd(seq_cmd_s);

  // MPI_Barrier(MPI_COMM_WORLD);

  // run_analysis_composition_cmd(tc_cmd_s);
  
  // Sequential composer    
  // scanalyses.push_back(new FlowInSensAddrTakenAnalysis(project));

  // scanalyses.push_back(new CallContextSensitivityAnalysis(1, CallContextSensitivityAnalysis::callSite));
  // scanalyses.push_back(new MPICommContextAnalysis());
  // scanalyses.push_back(new PointsToAnalysis());
  // scanalyses.push_back(new ConstantPropagationAnalysis());
  // scanalyses.push_back(new MPIValueAnalysis());
  // scanalyses.push_back(new ConstantPropagationAnalysis());
  // scanalyses.push_back(new DeadPathElimAnalysis());  
  // scanalyses.push_back(new MPIDotValueAnalysis());
  // scanalyses.push_back(new MPICommAnalysis());
  // MPIDotValueAnalysis* mdvanalysis = new MPIDotValueAnalysis();
  // scanalyses.push_back(mdvanalysis);
  // // scanalyses.push_back(new ConstantPropagationAnalysis());
  // // scanalyses.push_back(new MPICommAnalysis());
  // // scanalyses.push_back(new ConstantPropagationAnalysis());

  // // Tight composition of analyses
  // // tcanalyses.push_back(new ConstantPropagationAnalysis());
  // // tcanalyses.push_back(new MPICommAnalysis());
  // // TightComposer* tightcomposer = new TightComposer(tcanalyses);
  // // scanalyses.push_back(tightcomposer);
  
  // checkDataflowInfoPass* cdip = new checkDataflowInfoPass();
  // ChainComposer cc(scanalyses, cdip, false);

  // cc.runAnalysis();

  // if(cdip->getNumErrors() > 0) cout << cdip->getNumErrors() << " Errors Reported!"<<endl;
  // else                         cout << "PASS"<<endl;

  // MPIDotGraphGenerator dotgen(mdvanalysis);
  // dotgen.generateDot();
  // dotgen.generateDotFile();
  MPI_Finalize();

  cout << "==========  E  N  D  ==========\n";
  return 0;
}
