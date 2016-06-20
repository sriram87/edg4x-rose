/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

// Include this first before anything else
// All basic rose definitions are contained here
#include "sage3basic.h"
#include "mpi_annotate_ast.h"

using namespace std;

namespace fuse {
  string UnknownSideEffectsAttribute::str(string indent) {
    return "fuse:UnknownSideEffectsAttribute";
  }

  string ValueUnknownAttribute::str(string indent) {
    return "fuse:ValueUnknownAttribute";
  }

  string AnnotateMPISideEffects::getFunctionName(SgFunctionRefExp* sgn) {
    if(SgFunctionSymbol* symbol = sgn->get_symbol()) {
      SgName name = symbol->get_name();
      return name.getString();
    }
    // name is not determinable
    // return empty string
    return "";
  }

  void AnnotateMPISideEffects::annotateExprValueUnknown(SgExpression* expr) {
    expr->setAttribute("fuse:ValueUnknownAttribute", new ValueUnknownAttribute());
  }

  //! Traversal method that identifies the MPI side-effecting functions
  //! Identified functions are annotated with UnknownSideEffectsAttribute
  //! Expressions modified by the functions are annotated with ValueUnknownAttribute
  void AnnotateMPISideEffects::visit(SgNode* sgn) {
    if(SgFunctionCallExp* callexp = isSgFunctionCallExp(sgn)) {
      // MPI functions have a call ref expression
      if(SgFunctionRefExp* callref = isSgFunctionRefExp(callexp->get_function())) {
        string name = getFunctionName(callref);
        SgExprListExp* args = callexp->get_args();
        SgExpressionPtrList argsList = args->get_expressions();
        
        // Both comm_rank and comm_size modify the second variable
        if(name.compare("MPI_Comm_rank") == 0 ||
           name.compare("MPI_Comm_size") == 0) {
          sgn->setAttribute("fuse:UnknownSideEffectsAttribute", new UnknownSideEffectsAttribute());
          annotateExprValueUnknown(argsList[1]);
        }
        // Buffer argument is modified by MPI_Recv
        // TODO: do we need to consider MPI_Status?
        else if(name.compare("MPI_Recv") == 0 ||
                name.compare("MPI_Irecv") == 0) {
          sgn->setAttribute("fuse:UnknownSideEffectsAttribute", new UnknownSideEffectsAttribute());
          annotateExprValueUnknown(argsList[0]);
        }
        else if(name.compare("MPI_Send") == 0) {
          sgn->setAttribute("fuse:UnknowSideEffectsAttribute", new UnknownSideEffectsAttribute());
        }
        else if(name.compare("MPI_Barrier") == 0){
          sgn->setAttribute("fuse:UnknowSideEffectsAttribute", new UnknownSideEffectsAttribute());
        }        
        else if(name.compare("MPI_Bcast") == 0){
          sgn->setAttribute("fuse:UnknowSideEffectsAttribute", new UnknownSideEffectsAttribute());
          annotateExprValueUnknown(argsList[0]);
        }        
        else if(name.compare("MPI_Reduce") == 0){
          assert(0);
        }
        else if(name.compare("MPI_Gather") == 0){
          assert(0);
        }
        else if(name.compare("MPI_Scatter") == 0){
          assert(0);
        }
        else if(name.compare("MPI_Allgather") == 0){
          assert(0);
        }
        else if(name.compare("MPI_Allreduce") == 0){
          assert(0);
        }
        else if(name.compare("MPI_Init") == 0) { }
        else if(name.compare("MPI_Finalize") == 0) { }
        else if(name.find("MPI_", 0) != string::npos) {
          cout << name << endl;
          assert(0);
        }
      } // end isSgFunctionRefExp
    } // end isSgFunctionCallExp
  }
};
