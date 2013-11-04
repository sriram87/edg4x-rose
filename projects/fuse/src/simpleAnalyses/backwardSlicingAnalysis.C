/*****************************************
 * author: Sriram Aananthakrishnan, 2013 *
 *****************************************/
#include "sage3basic.h"
#include "backwardSlicingAnalysis.h"

#include <boost/bind.hpp>

using namespace dbglog;
using namespace std;

namespace fuse {

  int backwardSlicingDebugLevel = 2;

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
    scope reg("BackwardSlicingLattice::meetUpdate", scope::medium, 2, backwardSlicingDebugLevel);
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
    if(backwardSlicingDebugLevel >=2 ) {
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
    if(relevantMLRef->isFull() || relevantMLVal->isFull() || relevantAS.first) {
      ROSE_ASSERT(false);
    }
      
    return relevantMLRef->isFull() && relevantMLVal->isFull() && relevantAS.first;
  }

  bool BackwardSlicingLattice::isEmpty() {
    bool empty = relevantMLRef->isEmpty() && relevantMLVal->isEmpty();
    empty = empty && relevantAS.first == false && relevantAS.second.size() == 0;
    return empty;
  }

  bool BackwardSlicingLattice::insertRelevantMLRef(MemLocObjectPtr ml) {
    return relevantMLRef->insert(ml);
  }

  bool BackwardSlicingLattice::insertRelevantMLVal(MemLocObjectPtr ml) {
    return relevantMLVal->insert(ml);
  }

  bool BackwardSlicingLattice::insertRelevantAS(PartPtr part) {
    pair<set<PartPtr>::iterator, bool> rv = relevantAS.second.insert(part);
    return rv.second;
  }

  bool BackwardSlicingLattice::removeRelevantMLVal(MemLocObjectPtr ml) {
    return relevantMLVal->remove(ml);
  }

  string BackwardSlicingLattice::relevantASToStr(pair<bool, set<PartPtr> >& _relevantAS) {
    ostringstream oss;
    set<PartPtr>::iterator it = _relevantAS.second.begin();
    oss << "[";
    for( ;it != _relevantAS.second.end(); ) {
      PartPtr part = *it;
      oss << part->str();
      ++it;
      if(it != _relevantAS.second.end())
        oss << ", ";
    }
    oss << "]";
    return oss.str();
  }

  bool BackwardSlicingLattice::containsML(AbstractObjectSet* relevantML, MemLocObjectPtr ml) {
    return relevantML->containsMay(ml);
  }

  bool BackwardSlicingLattice::relevantMLValContainsML(MemLocObjectPtr ml) {
    return containsML(relevantMLVal, ml);
  }

  string BackwardSlicingLattice::strp(PartEdgePtr pedge, string indent) {
    ostringstream oss;
    oss << "BackwardSlicingLattice@";
    oss << "pedge=" << pedge->str() << endl;
    oss << "relevantMLRef=" << relevantMLRef->strp(pedge, indent) << endl;
    oss << "relevantMLVal=" << relevantMLVal->strp(pedge, indent) << endl;
    oss << "relevantAS=" << relevantASToStr(relevantAS) << endl;
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
      bsl->insertRelevantAS(part); 
      set<SgVarRefExp*> varRefExpSet = sc.second;
      set<SgVarRefExp*>::iterator vresIt = varRefExpSet.begin();
      for( ; vresIt != varRefExpSet.end(); ++ vresIt) {
        MemLocObjectPtr ml = composer->Expr2MemLoc(*vresIt, part->inEdgeFromAny(), this);
        bsl->insertRelevantMLRef(ml);
        bsl->insertRelevantMLVal(ml);
      }
    }
  }

  void BackwardSlicingAnalysis::genInitLattice(PartPtr part, 
                                               PartEdgePtr pedge,
                                               vector<Lattice*>& initLattices) {
    // set the scope for debugging
    scope reg("BackwardSlicingAnalysis::genInitLattice", scope::medium, 2, backwardSlicingDebugLevel);
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
    if(backwardSlicingDebugLevel >= 2) {
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

  bool BackwardSlicingTransfer::finish() {
    scope reg("BackwardSlicingTransfer::finish", 
              scope::medium, 2, backwardSlicingDebugLevel);

    if(backwardSlicingDebugLevel >= 2 && modified) {
      dbg << "Transferred BSL=" << bsl->str() << "\n";
    }
    return modified;
  }

  /*******************************
   * BackwardSlicingExprTransfer *
   *******************************/

  template <typename T>
  void BackwardSlicingExprTransfer::binaryExprTransfer(SgBinaryOp* sgn, T transferFunctor) {
    scope reg("BackwardSlicingExprTransfer::binaryExprTransfer", 
              scope::medium, 2, backwardSlicingDebugLevel);

    MemLocObjectPtr mlExp = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), bsa);
    MemLocObjectPtr mlLHS = composer->OperandExpr2MemLoc(sgn, sgn->get_lhs_operand(), 
                                                         part->inEdgeFromAny(), bsa);
    MemLocObjectPtr mlRHS = composer->OperandExpr2MemLoc(sgn, sgn->get_rhs_operand(), 
                                                         part->inEdgeFromAny(), bsa);
    if(backwardSlicingDebugLevel >= 2) {
      dbg << "mlExp=" << mlExp->strp(part->inEdgeFromAny()) << ",";
      dbg << "mlLHS=" << mlLHS->strp(part->inEdgeFromAny()) << ",";
      dbg << "mlRHS=" << mlRHS->strp(part->inEdgeFromAny()) << "\n";
    }

    transferFunctor(mlExp, mlLHS, mlRHS);
    updateRelevantML();
    updateRelevantAS();
  }

  void BackwardSlicingExprTransfer::updateRelevantML() {    
    scope reg("BackwardSlicingExprTransfer::updateRelevantMLSet", 
              scope::medium, 2, backwardSlicingDebugLevel);

    if(backwardSlicingDebugLevel >=2 ) {
      dbg << "defML=" << defML.str() << "\n";
      dbg << "refML=" << refML.str() << "\n";
    }

    bool removed = false;
    // if the defML was not empty remove defML from relevantMLSet
    unsigned int defMLSize = defML.size();
    if(defMLSize > 0) {
      for(AbstractObjectSet::const_iterator it = defML.begin(); it != defML.end(); ++it) {
        MemLocObjectPtr ml = boost::dynamic_pointer_cast<MemLocObject> (*it);
        removed = bsl->removeRelevantMLVal(ml) || removed;
      }
    }

    unsigned int refMLSize = refML.size();
    bool added = false;
    // if not empty add refML to relevantMLSet
    if(refMLSize > 0) {
      for(AbstractObjectSet::const_iterator it = refML.begin(); it != refML.end(); ++it) {
        MemLocObjectPtr ml = boost::dynamic_pointer_cast<MemLocObject> (*it);
        added = bsl->insertRelevantMLRef(ml) || added;
        added = bsl->insertRelevantMLVal(ml) || added;
      }
    }
    modified = removed || added;
    if(backwardSlicingDebugLevel >= 2) {
      dbg << "bsl=" << bsl->str() << "\n";
      dbg << "modified=" << (modified? "true" : "false") << "\n";
    }
  }

  void BackwardSlicingExprTransfer::updateRelevantAS() {    
    scope reg("BackwardSlicingExprTransfer::updateRelevantCFGNSet", 
              scope::medium, 2, backwardSlicingDebugLevel);

    if(backwardSlicingDebugLevel >= 2) {
      dbg << "StateModified? = " << (modified? "true":"false") << "\n";
    }
    
    // if the relevantMLSet was updated then the statement is relevant
    // modified flag is set by updateRelevantMLSet
    if(modified) {
      bsl->insertRelevantAS(bst.getPartPtr());
    }
  }

  void BackwardSlicingExprTransfer::transferAssignment(MemLocObjectPtr mlExp, 
                                                       MemLocObjectPtr mlLHS, 
                                                       MemLocObjectPtr mlRHS) {
    if(bsl->relevantMLValContainsML(mlExp) ||
       bsl->relevantMLValContainsML(mlLHS)) {
      defML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlLHS));
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlRHS));
    }
  }

  void BackwardSlicingExprTransfer::transferBinaryOpNoMod(MemLocObjectPtr mlExp,
                                                          MemLocObjectPtr mlLHS,
                                                          MemLocObjectPtr mlRHS) {
    if(bsl->relevantMLValContainsML(mlExp)) {
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlLHS));
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlRHS));
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

  void BackwardSlicingExprTransfer::visit(SgVarRefExp* sgn) {    
  }

  void BackwardSlicingExprTransfer::visit(SgValueExp* sgn) {
  }

}; // end namespace
