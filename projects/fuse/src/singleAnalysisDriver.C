/*****************************************
 * author: Sriram Aananthakrishnan, 2013 *
 *****************************************/

// quick way to set up the composer andtest individual analysis

#include "sage3basic.h"
#include "compose.h"
#include "const_prop_analysis.h"
#include "backwardSlicingAnalysis.h"
#include <iostream>

using namespace std;
using namespace fuse;

int main(int argc, char** argv)
{
  FuseInit(argc, argv);
  printf("========== S T A R T ==========\n");
    // Run the front end
  SgProject* project = frontend(argc, argv);
  //generatePDF(*project);
  cout << "Frontend done\n";

  SliceCriterionsList sliceCriterions;
  std::vector<SgReturnStmt*> stmtsOfInterest = SageInterface::querySubTree<SgReturnStmt>(project);
  std::vector<SgReturnStmt*>::iterator it;
  for(it = stmtsOfInterest.begin(); it != stmtsOfInterest.end(); ++it) {
    sliceCriterions.addSliceCriterionFromStmt(*it);
  }

  std::list<ComposedAnalysis*> analyses;
  //analyses.push_back(new ConstantPropagationAnalysis());
  analyses.push_back(new BackwardSlicingAnalysis(sliceCriterions));
  checkDataflowInfoPass* cdip = new checkDataflowInfoPass();
  ChainComposer cc(analyses, cdip, true);
  cc.runAnalysis();
  printf("==========  E  N  D  ==========\n");

  return 0;
}

