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
  string workdir = (txt() << "dbg").c_str();
  SightInit(argc, argv, title, workdir);
}

int main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);
  // FuseMPIInit(argc, argv);
  // cout << "========== S T A R T ==========\n";

  // Run the front end
  SgProject* project = frontend(argc, argv);

  AnnotateMPISideEffects annotateMPI;
  annotateMPI.traverseInputFiles(project, preorder);

  // printf("Frontend done\n");fflush(stdout);

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

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start, end;
  MPI_Barrier(MPI_COMM_WORLD);
  start = MPI_Wtime();

  FuseCommandParser parser;
  FuseCommand* cmd = parser(cmd_s);
  cmd->initFuseCommand();

  if(rank == 0) cerr << "=========== Executing Analyisis Composition ===========\n";
  cmd->execute();
  if(rank == 0) cerr << "=========== Execution complete ===========\n";

  // MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();  
  double time=end-start;
  double min, max, sum, avg;

  MPI_Reduce(&time, &min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Reduce(&time, &max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&time, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


  if(rank == 0) {
    avg = sum/size;
    cout << "Analysis Done:";
    cout << "min=" << min << ", max=" << max << ", avg=" << avg;
  }

  if(cmd->hasMPIDotValue()) {
    MPIDotValueAnalysis* mdvanalysis = cmd->getLastMPIDotValueAnalysis();
    MPIDotGraphGenerator dotgen(mdvanalysis);
    dotgen.generateDot();
    // dotgen.generateDotFile();
  }

  MPI_Finalize();

  // cout << "==========  E  N  D  ==========\n";
  return 0;
}
