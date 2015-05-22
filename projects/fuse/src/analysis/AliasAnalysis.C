#include "AliasAnalysis.h"

namespace SPRAY {
AliasAnalysis::AliasAnalysis() {}
AliasAnalysis::~AliasAnalysis() {}

bool AliasAnalysis::isMayAlias(SgExpression* e1, SgExpression* e2) {
  return true;
}

bool AliasAnalysis::isMustAlias(SgExpression* e1, SgExpression* e2) {
  return false;
}

}; // namespace SPRAY

