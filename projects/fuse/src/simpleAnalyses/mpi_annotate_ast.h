#ifndef _MPI_ANNOTATE_AST_H
#define _MPI_ANNOTATE_AST_H

/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/
#include <string>

/*
 * MPI functions have side-effects
 * It is useful to know what MPI functions are side-effecting
 * and what variables are modified as a result of the MPI functions
 * We implement AST traversals that traverses the AST looking for MPI
 * functions and annotating the side-effecting functions by adding 
 * AstAttribute. The traversal also identifies the arguments modified
 * by the MPI functions.
 */
namespace fuse {
  class UnknownSideEffectsAttribute : public AstAttribute {
  public:
    std::string str(std::string indent="");
  };

  class ValueUnknownAttribute : public AstAttribute {
  public:
    std::string str(std::string indent="");
  };
  
  class AnnotateMPISideEffects : public AstSimpleProcessing {
  public:
    std::string getFunctionName(SgFunctionRefExp* sgn);
    void annotateExprValueUnknown(SgExpression* sgn);
    void visit(SgNode* sgn);
  };
};

#endif
