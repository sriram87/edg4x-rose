#include "sage3basic.h"
#include "compose.h"
#include "mpi_value_analysis.h"
#include "const_prop_analysis.h"
#include "dead_path_elim_analysis.h"
#include "tight_composer.h"
#include "mpi_comm_context_analysis.h"
#include "call_context_sensitivity_analysis.h"
#include "pointsToAnalysis.h"
#include "mpi_comm_analysis.h"
#include "address_taken_analysis.h"
#include "mpi_annotate_ast.h"
#include "sight.h"
#include "mpi.h"
#include <exception>

#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>

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
  setenv("SIGHT_LAYOUT_EXEC", (txt()<<ROSE_PREFIX<<"/projects/fuse/src/fuseLayout").c_str(), 1);

  int flag = false;
  MPI_Initialized(&flag); assert(flag);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  string title = (txt() << "Process " << rank << " Debug Output").c_str();
  string workdir = (txt() << "dbg" << rank).c_str();
  SightInit(argc, argv, title, workdir);
}

class CompositionalCommand {
public:
  typedef enum CompositionType{seq, tight, unknown} CType;
private:
  list<ComposedAnalysis*> subanalyses;
  CType ctype;
public:
  CompositionalCommand() : ctype(unknown) { }
  void setCType(CompositionType type) {
    ctype = type;
  }

  CompositionType getCType() const {
    return ctype;
  }
  
  void push_back(ComposedAnalysis* analysis) {
    // cout << "Adding analysis=" << analysis->str() << endl;
    subanalyses.push_back(analysis);
  }

  list<ComposedAnalysis*> getSubAnalysisList() const {
    return subanalyses;
  }

  string str() const {
    cout.flush();
    ostringstream oss;
    oss << "[type=";
    if(ctype==seq) oss << "seq";
    else if(ctype==tight) oss << "tight";
    else oss << "unknown";

    oss << ", analyses={";

    list<ComposedAnalysis*>::const_iterator i = subanalyses.begin();
    for( ; i != subanalyses.end(); ) {
      oss << (*i)->str();
      ++i;
      if(i != subanalyses.end()) oss << ", ";
    }
    oss << "}]";
    return oss.str();
  }
};

sregex seqcomp, tightcomp, constprop, mpicommcontext,
  mpivalue, mpicomm, deadpath, pointsto, analysis,
  analysisList, composer, atailseq, scommand, ctailseq, tailseq, headseq, command;

void init_sregex() {
  constprop = icase("cp");
  mpicommcontext = icase("mcc");
  mpivalue = icase("mv");
  mpicomm = icase("mco");
  pointsto = icase("pt");
  deadpath = icase("dp");
  seqcomp = icase("seq");
  tightcomp = icase("tight");
  analysis = by_ref(constprop) | by_ref(mpicommcontext) | by_ref(mpivalue) | by_ref(mpicomm) | by_ref(pointsto) | by_ref(deadpath);
  composer = by_ref(seqcomp) | by_ref(tightcomp);
  atailseq = *_s >> as_xpr(',') >> *_s >> analysis >> *_s;
  analysisList = '(' >> *_s >> analysis >> *by_ref(atailseq) >> ')';
  scommand = composer >> analysisList;
  ctailseq = *_s >> as_xpr(',') >> *_s >> by_ref(scommand) >> *_s;
  tailseq = atailseq | ctailseq;
  headseq = analysis | scommand;
  command = composer >> '(' >> headseq >> *tailseq >> ')';
}

void match_analysis(smatch what, CompositionalCommand& cc);
void match_composer(smatch what, CompositionalCommand& cc);
void match_headseq(smatch what, CompositionalCommand& cc);
void match_tailseq(smatch what, CompositionalCommand& cc);
void match_command_nested(smatch what, CompositionalCommand& cc);
void match_command(smatch what, CompositionalCommand& cc);

ComposedAnalysis* make_command(CompositionalCommand& cc) {
  // All composers also inherit from ComposedAnalysis
  ComposedAnalysis* command;
  if(cc.getCType() == CompositionalCommand::seq) {
    const list<ComposedAnalysis*>& subanalyses = cc.getSubAnalysisList();
    checkDataflowInfoPass* cdip = new checkDataflowInfoPass();
    command = new ChainComposer(subanalyses, cdip, true);
  }
  else if(cc.getCType() == CompositionalCommand::tight) {
    const list<ComposedAnalysis*>& subanalyses = cc.getSubAnalysisList();
    command = new TightComposer(subanalyses);
  }
  else assert(false);
  
  return command;
}

class MatchException : public std::exception {
  string what_s;
public:
  MatchException(string what) : what_s(what) { }
  virtual const char* what() const throw() {
    return what_s.c_str();
  }
  ~MatchException() throw() { }
};

// get the string and match it against all analyses
void match_analysis(smatch what, CompositionalCommand& cc) {
  string analysis_s = what[0];
  if(regex_match(analysis_s, constprop)) {
    cc.push_back(new ConstantPropagationAnalysis());
    // cout << "analysis=cp" << endl;
  }
  else if(regex_match(analysis_s, mpicommcontext)) {
    cc.push_back(new MPICommContextAnalysis());
    // cout << "analysis=mcc" << endl;
  }
  else if(regex_match(analysis_s, mpivalue)) {
    cc.push_back(new MPIValueAnalysis());
    // cout << "analysis=mv" << endl;
  }
  else if(regex_match(analysis_s, mpicomm)) {
    cc.push_back(new MPICommAnalysis());
    // cout << "analysis=mpicomm" << endl;
  }
  else if(regex_match(analysis_s, pointsto)) {
    cc.push_back(new PointsToAnalysis());
    // cout << "analysis=pointsto" << endl;
  }
  else if(regex_match(analysis_s, deadpath)) {
    cc.push_back(new DeadPathElimAnalysis());
    // cout << "analysis=deadpath" << endl;
  }
  else {
    ostringstream oss;
    oss << "No match in match_analysis(what=" << what[0] << ")\n";
    throw MatchException(oss.str());
  }
  // cout << "match_analyis() " << cc.str() << endl;
}

void match_composer(smatch what, CompositionalCommand& cc) {
  string compinput = what[0];
  smatch cwhat;

  if(regex_match(compinput, cwhat, seqcomp)) {
    cc.setCType(CompositionalCommand::seq);
    // cout << "composer=" << cwhat[0] << endl;
  }
  else if(regex_match(compinput, cwhat, tightcomp)) {
    cc.setCType(CompositionalCommand::tight);
    // cout << "composer=" << cwhat[0] << endl;
  }
  else {
    ostringstream oss;
    oss << "No match in match_composer(what=" << what[0] << ")\n";
    throw MatchException(oss.str());
  }
  // cout << "match_composer() " << cc.str() << endl;
}

// headseq is analysis or scommand
void match_headseq(smatch what, CompositionalCommand& cc) {
  sregex hregex = analysis | scommand;
  string hstring = what[0];

  sregex_token_iterator begin(hstring.begin(), hstring.end(), hregex), end;
  for( ; begin != end; ++begin) {
    string token = *begin;
    smatch hwhat;
    if(regex_match(token, hwhat, analysis)) {
      match_analysis(hwhat, cc);
    }
    else if(regex_match(token, hwhat, scommand)) {
      CompositionalCommand* scc = new CompositionalCommand();
      match_command(hwhat, *scc);
      ComposedAnalysis* subcommand = make_command(*scc);
      cc.push_back(subcommand);
    }
    else {
      ostringstream oss;
      oss << "No match in match_headseq(what=" << what[0] << ")\n";
      throw MatchException(oss.str());
    }      
  }
  // cout << "match_headseq() " << cc.str() << endl;
}

void match_tailseq(smatch what, CompositionalCommand& cc) {
  sregex tregex = analysis | scommand;
  string tailstring = what[0];
  
  sregex_token_iterator begin(tailstring.begin(), tailstring.end(), tregex), end;
  for( ; begin != end; ++begin) {
    string token = *begin;
    smatch twhat;
    if(regex_match(token, twhat, analysis)) {
      match_analysis(twhat, cc);
    }
    else if(regex_match(token, twhat, scommand)) {
      CompositionalCommand* scc = new CompositionalCommand();
      match_command(twhat, *scc);
      ComposedAnalysis* subcommand = make_command(*scc);
      cc.push_back((ComposedAnalysis*)subcommand);
    }
    else {
      ostringstream oss;
      oss << "No match in match_tailseq(what=" << what[0] << ")\n";
      throw MatchException(oss.str());
    }
  }
  // cout << "match_tailseq() " << cc.str() << endl;
}

// Every command has 3 tokens
// It consists of composer followed by '(' and headseq, tailseq and ')'
// Use the filter iterator to break it into parts
void match_command_nested(smatch what, CompositionalCommand& cc) {
  // composer
  sregex_id_filter_predicate composer_id(composer.regex_id());
  sregex_id_filter_predicate headseq_id(headseq.regex_id());
  sregex_id_filter_predicate tailseq_id(tailseq.regex_id());

  if(composer_id(what)) {
    match_composer(what, cc);
  }
  else if(headseq_id(what)) {
    match_headseq(what, cc);
  }
  else if(tailseq_id(what)) {
    match_tailseq(what, cc);
  }
  else {
    ostringstream oss;
    oss << "No match in match_command_nested(what=" << what[0] << ")\n";
    throw MatchException(oss.str());
  }
  // cout << "match_nested_command() " << cc.str() << endl;
}

void match_command(string what_s, CompositionalCommand& cc) {
  smatch cwhat;
  if(regex_match(what_s, cwhat, command)) {
    smatch::nested_results_type::const_iterator begin = cwhat.nested_results().begin();
    smatch::nested_results_type::const_iterator end = cwhat.nested_results().end();
    smatch::nested_results_type::const_iterator c;  
    for(c = begin; c != end; ++c) {
      smatch nwhat = *c;
      match_command_nested(nwhat, cc);
    }
  }
  else {
    ostringstream oss;
    oss << "No match in match_command(what=" << what_s << ")\n";
    throw MatchException(oss.str());
  }
  // cout << "match_command() " << cc.str() << endl;
}

void match_command(smatch what, CompositionalCommand& cc) {
  string match_s = what[0];
  assert(match_s.size() > 0);
  match_command(match_s, cc);
}

void run_analysis_composition_cmd(string cmd_s) {
  try {
    CompositionalCommand* cc = new CompositionalCommand();
    match_command(cmd_s, *cc);
    ComposedAnalysis* command = make_command(*cc);      
    // cout << command->str() << endl;

    ((ChainComposer*) command)->runAnalysis();
  }
  catch(MatchException& e) {
    cerr << e.what() << endl;
  }
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
  sregex seq_cmd = *_s >> as_xpr("fuse-mpi-seq") >> *_s >> (s1=+~_n);
  sregex tc_cmd = *_s >> as_xpr("fuse-mpi-tc") >> *_s >> (s1=+~_n);
  smatch what;
  string seq_cmd_s = "" , tc_cmd_s = "";

  Rose_STL_Container<SgNode*> pragmas = NodeQuery::querySubTree(project, V_SgPragma);
  for(Rose_STL_Container<SgNode*>::iterator p=pragmas.begin(); p!=pragmas.end(); p++) {
    SgPragma* pragma = isSgPragma(*p);
    assert(pragma);

    //cout << "pragma: "<< pragma->get_pragma() << endl;
    if(regex_match(pragma->get_pragma(), what, seq_cmd)) {
      assert(what.size() == 2);
      seq_cmd_s.append(what[1]);
    }
    else if(regex_match(pragma->get_pragma(), what, tc_cmd)) {
      assert(what.size() == 2);
      tc_cmd_s.append(what[1]);
    }
  }

  init_sregex();

  run_analysis_composition_cmd(seq_cmd_s);

  MPI_Barrier(MPI_COMM_WORLD);

  run_analysis_composition_cmd(tc_cmd_s);
  
  // Sequential composer    
  // scanalyses.push_back(new FlowInSensAddrTakenAnalysis(project));

  // scanalyses.push_back(new CallContextSensitivityAnalysis(1, CallContextSensitivityAnalysis::callSite));
  // scanalyses.push_back(new MPICommContextAnalysis());
  // scanalyses.push_back(new PointsToAnalysis());
  // scanalyses.push_back(new ConstantPropagationAnalysis());
  // scanalyses.push_back(new MPIValueAnalysis());
  // scanalyses.push_back(new ConstantPropagationAnalysis());
  // scanalyses.push_back(new DeadPathElimAnalysis());
  // scanalyses.push_back(new PointsToAnalysis());  
  // scanalyses.push_back(new ConstantPropagationAnalysis());
  // scanalyses.push_back(new MPICommAnalysis());
  // scanalyses.push_back(new ConstantPropagationAnalysis());
  // scanalyses.push_back(new MPICommAnalysis());
  // scanalyses.push_back(new ConstantPropagationAnalysis());

  // Tight composition of analyses
  // tcanalyses.push_back(new ConstantPropagationAnalysis());
  // tcanalyses.push_back(new MPICommAnalysis());
  // TightComposer* tightcomposer = new TightComposer(tcanalyses);
  // scanalyses.push_back(tightcomposer);
  
  // checkDataflowInfoPass* cdip = new checkDataflowInfoPass();
  // ChainComposer cc(scanalyses, cdip, false);

  // cc.runAnalysis();

  // if(cdip->getNumErrors() > 0) cout << cdip->getNumErrors() << " Errors Reported!"<<endl;
  // else                         cout << "PASS"<<endl;

  MPI_Finalize();

  cout << "==========  E  N  D  ==========\n";
  return 0;
}
