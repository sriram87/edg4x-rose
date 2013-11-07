/*****************************************
 * author: Sriram Aananthakrishnan, 2013 *
 *****************************************/
#include "sage3basic.h"
#include "backwardSlicingAnalysis.h"

#include <boost/bind.hpp>

using namespace sight;
using namespace std;

namespace fuse {

  DEBUG_LEVEL(backwardSlicingDebugLevel, 2);

  /*******************
   * SliceCriterions *
   *******************/
  SliceCriterionsList::SliceCriterion
  SliceCriterionsList::buildSliceCriterionFromStmt(SgStatement* stmt) {
    vector<SgVarRefExp*> varRefExps = SageInterface::querySubTree<SgVarRefExp>(stmt);
    SliceCriterionsList::SliceCriterion sc;
    sc.first = stmt;
    sc.second.insert(varRefExps.begin(), varRefExps.end());
    return sc;
  }

  string SliceCriterionsList::str() {
    ostringstream oss;
    vector<SliceCriterion>::iterator it = sliceCriterions.begin();
    set<SgVarRefExp*>::iterator sit;
    for( ; it != sliceCriterions.end(); ++it) {
      oss << "[<SgStatement=" << (*it).first->unparseToString() << ", ";
      oss << "set<SgVarRefExp>={";
      for(sit = (*it).second.begin(); sit != (*it).second.end(); ) {
        oss << (*sit)->unparseToString();
        ++sit;
        if(sit != (*it).second.end())
          oss << ", ";
      }
      oss << "}>]\n";
    }
    return oss.str();
  }


  /**************************
   * BackwardSlicingLattice *
   **************************/
  void BackwardSlicingLattice::initialize() {
    // function will be deprecated
  }

  // returns a fresh copy of this lattice
  Lattice* BackwardSlicingLattice::copy() const {
    BackwardSlicingLattice* bsl = new BackwardSlicingLattice(*this);
    ROSE_ASSERT(bsl);
    return bsl;
  }
  
  // overwrite this lattice with that
  void BackwardSlicingLattice::copy(Lattice* that)  {
    try {
      BackwardSlicingLattice* bsl = dynamic_cast<BackwardSlicingLattice*>(that);      
      this->relevantMLRef->copy(bsl->relevantMLRef);
      this->relevantMLVal->copy(bsl->relevantMLVal);
      this->relevantAS = bsl->relevantAS;
    }
    catch(bad_cast& bc) {
      dbg << "BackwardSlicingLattice::copy(Lattice*): " << bc.what() << "\n";
      ROSE_ASSERT(false);
    }   
  }

  // union the two sets
  bool BackwardSlicingLattice::meetUpdate(Lattice* that) {
    bool modified = false;
    scope reg("BackwardSlicingLattice::meetUpdate", scope::medium, attrGE("backwardSlicingDebugLevel", 1));
    try {
      BackwardSlicingLattice* thatBSL = dynamic_cast<BackwardSlicingLattice*>(that);      
      // if this lattice is full, nothing is updated
      if(isFull()) {
        return false;
      }
      // if that is full, set this to full
      if(thatBSL->isFull()) {
        this->setToFull();
        modified = true;
        return true;
      }
      // union the relevantMLRef set
      modified = relevantMLRef->meetUpdate(thatBSL->relevantMLRef) || modified;
      // union the relevantMLVal set
      modified = relevantMLVal->meetUpdate(thatBSL->relevantMLVal) || modified;
      // union the relevantAS set
      unsigned int size_b = relevantAS.second.size();
      relevantAS.second.insert(thatBSL->relevantAS.second.begin(), thatBSL->relevantAS.second.end());
      unsigned int size_a = relevantAS.second.size();
      modified = (size_b != size_a) || modified;
    }
    catch(bad_cast& bc) {
      dbg << "BackwardSlicingLattice::meetUpdate(Lattice*): " << bc.what() << "\n";
      ROSE_ASSERT(false);
    }
    if(backwardSlicingDebugLevel() >=2 ) {
      dbg << "After meetUpdate: bsl= " << this->str() << endl;
      dbg << "modified=" << modified << endl;
    }
    return modified;
  }

  bool BackwardSlicingLattice::operator==(Lattice* that) {
    try {
      BackwardSlicingLattice* thatBSL = dynamic_cast<BackwardSlicingLattice*>(that);
      // check if the two sets are equal otherwise
      if(isFull() && thatBSL->isFull()) {
        // both are full - return true
        return true;
      }
      else if(!isFull() && !thatBSL->isFull()) {
        // both are not full
        // check the two sets
        return relevantMLRef == thatBSL->relevantMLRef && 
          relevantMLVal == thatBSL->relevantMLVal &&
          relevantAS == thatBSL->relevantAS;
      }
      else {
        // one of the two are full
        return false;
      }
      
    }
    catch (bad_cast& bc) {
      dbg << "BackwardSlicingLattice::operator==(Lattice*): " << bc.what() << "\n";
      ROSE_ASSERT(false);
    }
  }

  bool BackwardSlicingLattice::setToFull() {
    // if it was already full - nothing is modified
    if(isFull())
      return false;
    relevantMLRef->setToFull();
    relevantMLVal->setToFull();
    relevantAS.second.clear();
    relevantAS.first = true;
    return true;
  }

  bool BackwardSlicingLattice::setToEmpty() {
    // if it was already empty - nothing is modified
    if(isEmpty())
      return false;
    relevantMLRef->setToEmpty();
    relevantMLVal->setToEmpty();
    relevantAS.second.clear();
    relevantAS.first = false;
    return true;
  }

  bool BackwardSlicingLattice::setMLValueToFull(MemLocObjectPtr ml) {
    // return false as this lattice is a set
    return false;
  }

  bool BackwardSlicingLattice::isFull() {
    // shouldn't be a case when only one of them is full
    if(relevantMLRef->isFullLat() || relevantMLVal->isFullLat() || relevantAS.first) {
      ROSE_ASSERT(false);
    }
      
    return relevantMLRef->isFullLat() && relevantMLVal->isFullLat() && relevantAS.first;
  }

  bool BackwardSlicingLattice::isEmpty() {
    bool empty = relevantMLRef->isEmptyLat() && relevantMLVal->isEmptyLat();
    empty = empty && relevantAS.first == false && relevantAS.second.size() == 0;
    return empty;
  }

  bool BackwardSlicingLattice::insertRelMLRef(MemLocObjectPtr ml) {
    return relevantMLRef->insert(ml);
  }

  bool BackwardSlicingLattice::insertRelMLVal(MemLocObjectPtr ml) {
    return relevantMLVal->insert(ml);
  }

  bool BackwardSlicingLattice::insertRelAS(PartPtr part) {
    pair<set<PartPtr>::iterator, bool> rv = relevantAS.second.insert(part);
    return rv.second;
  }

  bool BackwardSlicingLattice::insertRelAS(set<PartPtr>& that) {
    unsigned int size_b = relevantAS.second.size();
    relevantAS.second.insert(that.begin(), that.end());
    unsigned int size_a = relevantAS.second.size();
    return size_b != size_a;    
  }

  bool BackwardSlicingLattice::removeRelMLVal(MemLocObjectPtr ml) {
    return relevantMLVal->remove(ml);
  }

  string BackwardSlicingLattice::str(set<PartPtr>& partPtrSet) {
    ostringstream oss;
    set<PartPtr>::iterator it = partPtrSet.begin();
    oss << "[";
    for( ;it != partPtrSet.end(); ) {
      PartPtr part = *it;
      oss << part->str();
      ++it;
      if(it != partPtrSet.end())
        oss << ", ";
    }
    oss << "]";
    return oss.str();
  }

  bool BackwardSlicingLattice::containsML(AbstractObjectSet* relevantML, MemLocObjectPtr ml) {
    return relevantML->containsMay(ml);
  }

  bool BackwardSlicingLattice::containsRelMLVal(MemLocObjectPtr ml) {
    return containsML(relevantMLVal, ml);
  }

  bool BackwardSlicingLattice::containsRelMLRef(MemLocObjectPtr ml) {
    return containsML(relevantMLRef, ml);
  }

  bool BackwardSlicingLattice::containsAS(PartPtr part) {
    return (relevantAS.second.find(part) != relevantAS.second.end());
  }

  string BackwardSlicingLattice::strp(PartEdgePtr pedge, string indent) {
    ostringstream oss;
    oss << "BackwardSlicingLattice@";
    oss << "pedge=" << pedge->str() << endl;
    oss << "< relevantMLRef=" << relevantMLRef->strp(pedge, indent) << ">\n";
    oss << "< relevantMLVal=" << relevantMLVal->strp(pedge, indent) << ">\n";
    oss << "< relevantAS=" << str(relevantAS.second) << ">\n";
    return oss.str();
  }

  string BackwardSlicingLattice::str(string indent) {
    return strp(latPEdge, indent);
  }

  /***************************
   * BackwardSlicingAnalysis *
   ***************************/
  // SliceCriterion is a pair <SgStatement, set<SgVarRefExp>
  // 
  void BackwardSlicingAnalysis::initBSLForPart(SliceCriterionsList::SliceCriterion sc,
                                                            PartPtr part,
                                                            BackwardSlicingLattice* bsl)
  {
    // get the set of CFGNodes at this abstract state (part)
    const set<CFGNode>& CFGNodesAtThisPart = part->CFGNodes();
    SgStatement* stmt = sc.first;
    CFGNode stmtCFGNode = stmt->cfgForEnd();
    // if the stmtCFGNode is in this abstract state (part) then populate
    // relevantCFGNSet and relevantMLSet
    if(CFGNodesAtThisPart.find(stmtCFGNode) != CFGNodesAtThisPart.end()) {
      bsl->insertRelAS(part); 
      set<SgVarRefExp*> varRefExpSet = sc.second;
      set<SgVarRefExp*>::iterator vresIt = varRefExpSet.begin();
      for( ; vresIt != varRefExpSet.end(); ++ vresIt) {
        MemLocObjectPtr ml = composer->Expr2MemLoc(*vresIt, part->inEdgeFromAny(), this);
        bsl->insertRelMLRef(ml);
        bsl->insertRelMLVal(ml);
      }
    }
  }

  void BackwardSlicingAnalysis::genInitLattice(PartPtr part, 
                                               PartEdgePtr pedge,
                                               vector<Lattice*>& initLattices) {
    // set the scope for debugging
    string regString = "BackwardSlicingAnalysis::genInitLattice@" + part->str();
    scope reg(regString, scope::medium, attrGE("backwardSlicingDebugLevel", 1));
    AbstractObjectSet relevantML(pedge,
                                 composer,
                                 this,                          
                                 AbstractObjectSet::may);
    set<PartPtr> ASSet;
    pair<bool, set<PartPtr> > relevantAS = make_pair(false, ASSet);
    BackwardSlicingLattice* bsl = new BackwardSlicingLattice(pedge, relevantML, relevantML, relevantAS);
    
    // iterate over the slice criterion list and build it if relevant to this part
    const vector<SliceCriterionsList::SliceCriterion>& scList = sc.getSliceCriterions();
    vector<SliceCriterionsList::SliceCriterion>::const_iterator scIt = scList.begin();
    for( ; scIt != scList.end(); ++scIt) {
      initBSLForPart(*scIt, part, bsl);
    }    
    if(backwardSlicingDebugLevel() >= 2) {
      dbg << "pedge= " << pedge->str("") << ", bsl=" << bsl->str("") << endl;
    }
    initLattices.push_back(bsl);
  }

  boost::shared_ptr<DFTransferVisitor> 
  BackwardSlicingAnalysis::getTransferVisitor(PartPtr part,
                                              CFGNode cfgn,
                                              NodeState& state,
                                              map<PartEdgePtr, vector<Lattice*> >& dfInfo) {
    return boost::shared_ptr<DFTransferVisitor>(new BackwardSlicingTransfer(part, cfgn, state, dfInfo, getComposer(), this));
  }

  /***************************
   * BackwardSlicingTransfer *
   ***************************/

  void BackwardSlicingTransfer::setBSLFromDfInfo() {
    assert(dfInfo.size()==1);
    assert(dfInfo[NULLPartEdge].size()==1);
    assert(*dfInfo[NULLPartEdge].begin());
    Lattice *l = *dfInfo[NULLPartEdge].begin();
    bsl = (dynamic_cast<BackwardSlicingLattice*>(l));
    assert(bsl);
  }

  void BackwardSlicingTransfer::visit(SgExpression* sgn) {
    BackwardSlicingExprTransfer bset(*this);
    sgn->accept(bset);
    modified = bset.isStateModified();
  }

  void BackwardSlicingTransfer::visit(SgVariableDeclaration* sgn) {
    const SgInitializedNamePtrList& variables = sgn->get_variables();
    SgInitializedNamePtrList::const_iterator it = variables.begin();
    for( ; it != variables.end(); ++it) {
      MemLocObjectPtr ml = composer->Expr2MemLoc(*it, part->inEdgeFromAny(), bsa);
      // if the ml is in ref set then this part is relevant
      // if certain variables in this list are relevant then
      // we need to think about what this part should be!!
      if(bsl->containsRelMLRef(ml)) {
        modified = bsl->insertRelAS(part) || modified;
      }
    }
  }

  void BackwardSlicingTransfer::visit(SgInitializedName* sgn) {
    // process this as an expression
    BackwardSlicingExprTransfer bset(*this);
    sgn->accept(bset);
    modified = bset.isStateModified();
  }

  bool BackwardSlicingTransfer::finish() {
    scope reg("BackwardSlicingTransfer::finish", 
              scope::medium, attrGE("backwardSlicingDebugLevel", 1));

    if(backwardSlicingDebugLevel() >= 3 && modified) {
      dbg << "Transferred BSL=" << bsl->str() << "\n";
    }
    return modified;
  }

  /*******************************
   * BackwardSlicingExprTransfer *
   *******************************/

  list<PartPtr> BackwardSlicingExprTransfer::getOperandPartPtr(SgNode* anchor, SgNode* operand) {
    list<PartEdgePtr> operandPEdges = (part->inEdgeFromAny())->getOperandPartEdge(anchor, operand);
    list<PartPtr> operandParts;
    list<PartEdgePtr>::iterator it = operandPEdges.begin();
    for( ; it != operandPEdges.end(); ++it) {
      PartPtr operandPart = (*it)->source();
      operandParts.push_back(operandPart);
    }
    return operandParts;
  }

  void BackwardSlicingExprTransfer::updateBSLState() {    
    scope reg("BackwardSlicingExprTransfer::updateRelMLSet", 
              scope::medium, attrGE("backwardSlicingDebugLevel", 1));

    if(backwardSlicingDebugLevel() >=2 ) {
      dbg << "defML=" << defML.str() << "\n";
      dbg << "refML=" << refML.str() << "\n";
      dbg << "relAS=" << bsl->str(relAS) << "\n";
    }

    bool removed = false;
    // if the defML was not empty remove defML from relevantMLSet
    unsigned int defMLSize = defML.size();
    if(defMLSize > 0) {
      for(AbstractObjectSet::const_iterator it = defML.begin(); it != defML.end(); ++it) {
        MemLocObjectPtr ml = boost::dynamic_pointer_cast<MemLocObject> (*it);
        removed = bsl->removeRelMLVal(ml) || removed;
      }
    }

    unsigned int refMLSize = refML.size();
    bool added = false;
    // if not empty add refML to relevantMLSet
    if(refMLSize > 0) {
      for(AbstractObjectSet::const_iterator it = refML.begin(); it != refML.end(); ++it) {
        MemLocObjectPtr ml = boost::dynamic_pointer_cast<MemLocObject> (*it);
        added = bsl->insertRelMLRef(ml) || added;
        added = bsl->insertRelMLVal(ml) || added;
      }
    }

    bool partSetModified = false;
    if(relAS.size() > 0) {
      partSetModified = bsl->insertRelAS(relAS);
    }

    modified = removed || added || partSetModified;

    if(backwardSlicingDebugLevel() >= 2) {
      dbg << "bsl=" << bsl->str() << "\n";
      dbg << "modified=" << (modified? "true" : "false") << "\n";
    }
  }

  template <typename T>
  void BackwardSlicingExprTransfer::binaryExprTransfer(SgBinaryOp* sgn, T transferFunctor) {
    scope reg("BackwardSlicingExprTransfer::binaryExprTransfer", 
              scope::medium, attrGE("backwardSlicingDebugLevel", 1));

    transferFunctor(sgn, sgn->get_lhs_operand(), sgn->get_rhs_operand());
    updateBSLState();
  }

  void BackwardSlicingExprTransfer::transferAssignment(SgNode* bexp,
                                                       SgNode* lexp, 
                                                       SgNode* rexp) {
    MemLocObjectPtr mlExp = composer->Expr2MemLoc(bexp, part->inEdgeFromAny(), bsa);
    MemLocObjectPtr mlLHS = composer->OperandExpr2MemLoc(bexp, lexp, part->inEdgeFromAny(), bsa);
    MemLocObjectPtr mlRHS = composer->OperandExpr2MemLoc(bexp, rexp, part->inEdgeFromAny(), bsa);

    if(bsl->containsRelMLVal(mlExp) ||
       bsl->containsRelMLVal(mlLHS)) {
      defML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlLHS));
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlRHS));
      relAS.insert(part);
      // insert the current, left and rhs parts
      list<PartPtr> lhsPartPtrs = getOperandPartPtr(bexp, lexp);
      list<PartPtr> rhsPartPtrs = getOperandPartPtr(bexp, rexp);
      relAS.insert(lhsPartPtrs.begin(), lhsPartPtrs.end());
      relAS.insert(rhsPartPtrs.begin(), rhsPartPtrs.end());
    }
  }

  void BackwardSlicingExprTransfer::transferBinaryOpNoMod(SgNode* bexp,
                                                          SgNode* lexp, 
                                                          SgNode* rexp) {
    MemLocObjectPtr mlExp = composer->Expr2MemLoc(bexp, part->inEdgeFromAny(), bsa);
    MemLocObjectPtr mlLHS = composer->OperandExpr2MemLoc(bexp, lexp, part->inEdgeFromAny(), bsa);
    MemLocObjectPtr mlRHS = composer->OperandExpr2MemLoc(bexp, rexp, part->inEdgeFromAny(), bsa);
    if(bsl->containsRelMLVal(mlExp)) {
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlLHS));
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlRHS));
      // insert the left and right parts
      // no need to insert this part
      // if we are here the part was already in the relevantAS set
      ROSE_ASSERT(bsl->containsAS(part));
      list<PartPtr> lhsPartPtrs = getOperandPartPtr(bexp, lexp);
      list<PartPtr> rhsPartPtrs = getOperandPartPtr(bexp, rexp);
      relAS.insert(lhsPartPtrs.begin(), lhsPartPtrs.end());
      relAS.insert(rhsPartPtrs.begin(), rhsPartPtrs.end());
    }
  }

  void BackwardSlicingExprTransfer::visit(SgAssignOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferAssignment,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgCompoundAssignOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferAssignment,
                                        this,
                                        _1, _2, _3));
  }

  void BackwardSlicingExprTransfer::visit(SgAddOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgAndOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgArrowExp* sgn) {
    dbg << "Unhandled Expression sgn= " << sgn->class_name() 
        << "expr=" << sgn->unparseToString() << endl;
    ROSE_ASSERT(false);
  }
  void BackwardSlicingExprTransfer::visit(SgArrowStarOp* sgn) {
    dbg << "Unhandled Expression sgn= " << sgn->class_name() 
        << "expr=" << sgn->unparseToString() << endl;
    ROSE_ASSERT(false);
  }
  void BackwardSlicingExprTransfer::visit(SgBitAndOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgBitOrOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgBitXorOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgCommaOpExp* sgn) {
    dbg << "Unhandled Expression sgn= " << sgn->class_name() 
        << "expr=" << sgn->unparseToString() << endl;
    ROSE_ASSERT(false);
  }
  void BackwardSlicingExprTransfer::visit(SgDivideOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgDotExp* sgn) {
    dbg << "Unhandled Expression sgn= " << sgn->class_name() 
        << "expr=" << sgn->unparseToString() << endl;
    ROSE_ASSERT(false);
  }
  void BackwardSlicingExprTransfer::visit(SgDotStarOp* sgn) {
    dbg << "Unhandled Expression sgn= " << sgn->class_name() 
        << "expr=" << sgn->unparseToString() << endl;
    ROSE_ASSERT(false);
  }
  void BackwardSlicingExprTransfer::visit(SgEqualityOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgGreaterOrEqualOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgGreaterThanOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgLessOrEqualOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgLessThanOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgLshiftOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgModOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgMultiplyOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgNotEqualOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgOrOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgPntrArrRefExp* sgn) {
    dbg << "Unhandled Expression sgn= " << sgn->class_name() 
        << "expr=" << sgn->unparseToString() << endl;
    ROSE_ASSERT(false);
  }
  void BackwardSlicingExprTransfer::visit(SgRshiftOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }
  void BackwardSlicingExprTransfer::visit(SgSubtractOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3));
  }

  void BackwardSlicingExprTransfer::visit(SgInitializedName* sgn) {
    MemLocObjectPtr ml = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), bsa);
    if(bsl->containsRelMLVal(ml)) {
      // initialized name is relevant
      // add it to defML
      defML.insert(boost::dynamic_pointer_cast<AbstractObject>(ml));
      relAS.insert(part);
      SgInitializer* initializer = sgn->get_initializer();
      // if initializer is not NULL
      // process it as an expression
      if(initializer) {
        MemLocObjectPtr iml = composer->Expr2MemLoc(initializer, part->inEdgeFromAny(), bsa);
        ROSE_ASSERT(iml.get());
        refML.insert(boost::dynamic_pointer_cast<AbstractObject>(iml));
      }
      // update the state based on computed information
      updateBSLState();
    }
  }

  void BackwardSlicingExprTransfer::visit(SgAssignInitializer* sgn) {
    MemLocObjectPtr ml = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), bsa);
    if(bsl->containsRelMLVal(ml)) {
      // get the operand and process it
      SgExpression* operand = sgn->get_operand();
      MemLocObjectPtr oml = composer->Expr2MemLoc(operand, part->inEdgeFromAny(), bsa);
      refML.insert(oml);
      relAS.insert(part);
      list<PartPtr> opParts = getOperandPartPtr(sgn, operand);
      // add the operand parts here
      relAS.insert(opParts.begin(), opParts.end());
      updateBSLState();
    }
  }

  void BackwardSlicingExprTransfer::visit(SgVarRefExp* sgn) {    
  }

  void BackwardSlicingExprTransfer::visit(SgValueExp* sgn) {
  }

}; // end namespace
