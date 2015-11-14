#include <iostream>
#include "sage3basic.h"
#include "SgNodeHelper.h"

using namespace std;

class SgTypeVisitor : public AstSimpleProcessing {
  SgType* type;
public:
  SgTypeVisitor() : type(0) { }
  void visit(SgNode* sgn) {
    if(SgExpression* expr = isSgExpression(sgn)) {
      SgType* type_l = expr->get_type();
      cout << "type_l.addr=" << type_l << ", type_l=" << SgNodeHelper::nodeToString(type_l) << endl;
    }
    type = SageBuilder::buildIntType();
    cout << "type.addr=" << type << ", type=" << SgNodeHelper::nodeToString(type) << endl;
  }

  ~SgTypeVisitor() {    
  }
};

int main(int argc, char* argv[]) {
  SgProject* project = frontend(argc, argv);
  SgTypeVisitor typeVisitor;
  typeVisitor.traverse(project, preorder);
  return backend(project);
}

