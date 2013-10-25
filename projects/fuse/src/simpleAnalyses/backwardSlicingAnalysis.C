/*****************************************
 * author: Sriram Aananthakrishnan, 2013 *
 *****************************************/
#include "sage3basic.h"
#include "backwardSlicingAnalysis.h"

#include <boost/bind.hpp>

using namespace dbglog;

namespace fuse {

  int backwardSlicingDebugLevel = 2;

  /*******************
   * SliceCriterions *
   *******************/
  SliceCriterionsList::SliceCriterion
  SliceCriterionsList::buildSliceCriterionFromStmt(SgStatement* stmt) {
    std::vector<SgVarRefExp*> varRefExps = SageInterface::querySubTree<SgVarRefExp>(stmt);
    SliceCriterionsList::SliceCriterion sc;
    sc.first = stmt;
    sc.second.insert(varRefExps.begin(), varRefExps.end());
    return sc;
  }

  std::string SliceCriterionsList::str() {
    std::ostringstream oss;
    std::vector<SliceCriterion>::iterator it = sliceCriterions.begin();
    std::set<SgVarRefExp*>::iterator sit;
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
      this->relevantCFGNSetIsFull = bsl->relevantCFGNSetIsFull;
      this->relevantMLSet->copy(bsl->relevantMLSet);
      this->relevantCFGNSet = bsl->relevantCFGNSet;
    }
    catch(std::bad_cast& bc) {
      dbg << "BackwardSlicingLattice::copy(Lattice*): " << bc.what() << "\n";
      ROSE_ASSERT(false);
    }   
  }

  // union the two sets
  bool BackwardSlicingLattice::meetUpdate(Lattice* that) {
    bool modified = false;
    scope reg("BackwardSlicingLattice::meetUpdate", scope::medium, 2, backwardSlicingDebugLevel);
    try {
      BackwardSlicingLattice* bsl = dynamic_cast<BackwardSlicingLattice*>(that);      
      // if this lattice is full, nothing is updated
      if(relevantMLSet->isFull() && relevantCFGNSetIsFull) {
        return false;
      }
      // if that is full, set this to full
      if(bsl->isFull()) {
        this->setToFull();
        modified = true;
      }
      // union the relevantML set
      modified = relevantMLSet->meetUpdate(bsl->relevantMLSet);

      // union the relevantCFGNSet
      unsigned int size_b = relevantCFGNSet.size();
      relevantCFGNSet.insert(bsl->relevantCFGNSet.begin(), bsl->relevantCFGNSet.end());
      unsigned int size_a = relevantCFGNSet.size();
      modified = modified || (size_b != size_a);
    }
    catch(std::bad_cast& bc) {
      dbg << "BackwardSlicingLattice::meetUpdate(Lattice*): " << bc.what() << "\n";
      ROSE_ASSERT(false);
    }
    if(backwardSlicingDebugLevel >=2 ) {
      dbg << "After meetUpdate: bsl= " << str("") << std::endl;
      dbg << "modified=" << modified << std::endl;
    }
    return modified;
  }

  bool BackwardSlicingLattice::operator==(Lattice* that) {
    try {
      BackwardSlicingLattice* bsl = dynamic_cast<BackwardSlicingLattice*>(that);
      // check if the two sets are equal otherwise
      if(isFull() && bsl->isFull()) {
        // both are full - return true
        return true;
      }
      else if(!isFull() && !bsl->isFull()) {
        // both are not full
        // check the two sets
        return relevantMLSet == bsl->relevantMLSet && relevantCFGNSet == bsl->relevantCFGNSet;
      }
      else {
        // one of the two are full
        return false;
      }      
    }
    catch (std::bad_cast& bc) {
      dbg << "BackwardSlicingLattice::operator==(Lattice*): " << bc.what() << "\n";
      ROSE_ASSERT(false);
    }
  }

  bool BackwardSlicingLattice::setToFull() {
    // if it was already full - nothing is modified
    if(isFull())
      return false;
    relevantMLSet->setToFull();
    relevantCFGNSetIsFull = true;
    relevantCFGNSet.clear();
    return true;
  }

  bool BackwardSlicingLattice::setToEmpty() {
    // if it was already empty - nothing is modified
    if(isEmpty())
      return false;
    relevantMLSet->setToEmpty();
     relevantCFGNSetIsFull = false;
    relevantCFGNSet.clear();
    return true;
  }

  bool BackwardSlicingLattice::setMLValueToFull(MemLocObjectPtr ml) {
    // return false as this lattice is a set
    return false;
  }

  bool BackwardSlicingLattice::isFull() {
    return relevantMLSet->isFull() && relevantCFGNSetIsFull;
  }

  bool BackwardSlicingLattice::isEmpty() {
    return relevantMLSet->isEmpty() && relevantCFGNSet.empty();
  }
 
  std::string BackwardSlicingLattice::relevantCFGNSetToStr(std::set<SgNode*>& relevantCFGNSet) {
    std::ostringstream oss;
    oss << "[";
    for(std::set<SgNode*>::iterator it = relevantCFGNSet.begin();
        it != relevantCFGNSet.end(); ) {
      oss << "<" << *it << ", " << (*it)->class_name() << ">";
      ++it;
      if(it != relevantCFGNSet.end())
        oss << ", ";
    }
    oss << "]\n";
    return oss.str();
  }

  std::string BackwardSlicingLattice::strp(PartEdgePtr pedge, std::string indent) {
    std::ostringstream oss;
    oss << "BackwardSlicingLattice@";
    oss << "pedge=" << pedge->str() << std::endl;
    oss << "relevantMLSet=" << relevantMLSet->str(indent) << std::endl;
    oss << "relevantCFGNset=" << relevantCFGNSetToStr(relevantCFGNSet);
    return oss.str();
  }

  std::string BackwardSlicingLattice::str(std::string indent="") {
    return strp(latPEdge, indent);
  }

  /***************************
   * BackwardSlicingAnalysis *
   ***************************/
  // SliceCriterion is a pair <SgStatement, set<SgVarRefExp>
  // if the 
  void BackwardSlicingAnalysis::createSliceCriterionForPart(SliceCriterionsList::SliceCriterion sc,
                                                            PartPtr part,
                                                            AbstractObjectSet& relevantMLSet,
                                                            std::set<SgNode*>& relevantCFGNSet)
  {
    // get the set of CFGNodes at this abstract state (part)
    const std::set<CFGNode>& CFGNodesAtThisPart = part->CFGNodes();
    SgStatement* stmt = sc.first;
    CFGNode stmtCFGNode = stmt->cfgForEnd();
    // if the stmtCFGNode is in this abstract state (part) then populate
    // relevantCFGNSet and relevantMLSet
    if(CFGNodesAtThisPart.find(stmtCFGNode) != CFGNodesAtThisPart.end()) {
      // populate the two sets based on the slice criterion
      relevantCFGNSet.insert(stmtCFGNode.getNode());
      std::set<SgVarRefExp*> varRefExpSet = sc.second;
      std::set<SgVarRefExp*>::iterator vresIt = varRefExpSet.begin();
      for( ; vresIt != varRefExpSet.end(); ++ vresIt) {
        MemLocObjectPtr mlop = composer->Expr2MemLoc(*vresIt, part->inEdgeFromAny(), this);
        relevantMLSet.insert(mlop);
      }
    }
  }

  void BackwardSlicingAnalysis::genInitLattice(PartPtr part, 
                                               PartEdgePtr pedge,
                                               std::vector<Lattice*>& initLattices) {
    // set the scope for debugging
    scope reg("BackwardSlicingAnalysis::genInitLattice", scope::medium, 2, backwardSlicingDebugLevel);
    AbstractObjectSet relevantMLSet(pedge,
                                    composer,
                                    this,                          
                                    AbstractObjectSet::may);
    std::set<SgNode*> relevantCFGNSet;
    
    // iterate over the slice criterion list and build it if relevant to this part
    const std::vector<SliceCriterionsList::SliceCriterion>& scList = sc.getSliceCriterions();
    std::vector<SliceCriterionsList::SliceCriterion>::const_iterator scIt = scList.begin();
    for( ; scIt != scList.end(); ++scIt) {
      createSliceCriterionForPart(*scIt, part, relevantMLSet, relevantCFGNSet);
    }
    
    // initialize the lattice with relevantMLSet, relevantCFGNSet populated by createSliceCriterion
    BackwardSlicingLattice* bsl = new BackwardSlicingLattice(pedge, relevantMLSet, relevantCFGNSet);
    if(backwardSlicingDebugLevel >= 2) {
      dbg << "pedge= " << pedge->str("") << ", bsl=" << bsl->str("") << std::endl;
    }
    initLattices.push_back(bsl);
  }

  boost::shared_ptr<DFTransferVisitor> 
  BackwardSlicingAnalysis::getTransferVisitor(PartPtr part,
                                              CFGNode cfgn,
                                              NodeState& state,
                                              std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo) {
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
    if(backwardSlicingDebugLevel >=2 ) {
      dbg << "Transferred BSL=" << bsl->str() << "\n";
    }
  }

  /*******************************
   * BackwardSlicingExprTransfer *
   *******************************/

  template <typename T>
  void BackwardSlicingExprTransfer::binaryExprTransfer(SgBinaryOp* sgn, T transferFunctor) {
    scope reg("BackwardSlicingExprTransfer::binaryExprTransfer", 
              scope::medium, 2, backwardSlicingDebugLevel);
    Composer* composer = bst.getComposer();
    PartPtr part = bst.getPartPtr();
    BackwardSlicingAnalysis* bsa = bst.getBSA();

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

    BackwardSlicingLattice* bsl = bst.getBSL();
    AbstractObjectSet& relevantMLSetIN = bsl->getRelevantMLSet();

    transferFunctor(mlExp, mlLHS, mlRHS, relevantMLSetIN);
    updateRelevantMLSet();
    updateRelevantCFGNSet();
  }

  void BackwardSlicingExprTransfer::updateRelevantMLSet() {    
    scope reg("BackwardSlicingExprTransfer::updateRelevantMLSet", 
              scope::medium, 2, backwardSlicingDebugLevel);
    AbstractObjectSet& relevantMLSet = bst.getBSL()->getRelevantMLSet();

    if(backwardSlicingDebugLevel >=2 ) {
      dbg << "defML=" << defML.str() << "\n";
      dbg << "refML=" << refML.str() << "\n";
      dbg << "relevantMLSetIN=" << relevantMLSet.str() << "\n";
    }

    bool removed = false;
    // if the defML was not empty remove defML from relevantMLSet
    unsigned int defMLSize = defML.size();
    if(defMLSize > 0) {
      for(AbstractObjectSet::const_iterator it = defML.begin(); it != defML.end(); ++it) {
        removed = relevantMLSet.remove(*it) || removed;
      }
    }

    unsigned int refMLSize = refML.size();
    bool added = false;
    // if not empty add refML to relevantMLSet
    if(refMLSize > 0) {
      for(AbstractObjectSet::const_iterator it = refML.begin(); it != refML.end(); ++it) {
        added = relevantMLSet.insert(*it) || added;
      }
    }
    modified = removed || added;
    if(backwardSlicingDebugLevel >= 2) {
      dbg << "relevantMLSetOUT=" << relevantMLSet.str() << "\n";
      dbg << "modified=" << (modified? "true" : "false") << "\n";
    }
  }

  void BackwardSlicingExprTransfer::updateRelevantCFGNSet() {    
    scope reg("BackwardSlicingExprTransfer::updateRelevantCFGNSet", 
              scope::medium, 2, backwardSlicingDebugLevel);

    BackwardSlicingLattice* bsl = bst.getBSL();
    std::set<SgNode*>& relevantCFGNSet = bsl->getRelevantCFGNSet();

    if(backwardSlicingDebugLevel >= 2) {
      dbg << "relevantCFGNSetIN=" << bsl->relevantCFGNSetToStr(relevantCFGNSet);
      dbg << "StateModified? = " << (modified? "true":"false") << "\n";
    }
    
    // if the relevantMLSet was updated then the statement is relevant
    // modified flag is set by updateRelevantMLSet
    if(modified) {
      CFGNode cfgn = bst.getCFGN();
      SgStatement* stmt = SageInterface::getEnclosingStatement(cfgn.getNode());
      typedef std::set<SgNode*>::iterator SgnSetIterator;
      std::pair<SgnSetIterator, bool> rv = relevantCFGNSet.insert(stmt);
      if(rv.second) {
        // relevantCFGNSet was updated print it out
        if(backwardSlicingDebugLevel >= 2)
          dbg << "relevantCFGNOUT=" << bsl->relevantCFGNSetToStr(relevantCFGNSet);
      }
    }
  }

  void BackwardSlicingExprTransfer::transferAssignment(MemLocObjectPtr mlExp, 
                                                       MemLocObjectPtr mlLHS, 
                                                       MemLocObjectPtr mlRHS,
                                                       AbstractObjectSet& relevantMLSetIN) {
    if(relevantMLSetIN.containsMay(mlLHS) || 
       relevantMLSetIN.containsMay(mlExp)) {
      defML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlLHS));
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlRHS));
    }
  }

  void BackwardSlicingExprTransfer::transferBinaryOpNoMod(MemLocObjectPtr mlExp,
                                                          MemLocObjectPtr mlLHS,
                                                          MemLocObjectPtr mlRHS,
                                                          AbstractObjectSet& relevantMLSetIN) {
    if(relevantMLSetIN.containsMay(mlExp)) {
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlLHS));
      refML.insert(boost::dynamic_pointer_cast<AbstractObject>(mlRHS));
    }
  }

  void BackwardSlicingExprTransfer::visit(SgAssignOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferAssignment,
                                        this,
                                        _1, _2, _3, _4));
  }

  void BackwardSlicingExprTransfer::visit(SgAddOp* sgn) {
    binaryExprTransfer(sgn, boost::bind(&BackwardSlicingExprTransfer::transferBinaryOpNoMod,
                                        this,
                                        _1, _2, _3, _4));

  }
}; // end namespace
