/*****************************************
 * author: Sriram Aananthakrishnan, 2013 *
 *****************************************/
#include "sage3basic.h"
#include "backwardSlicingAnalysis.h"

using namespace dbglog;

namespace fuse {
  /*******************
   * SliceCriterions *
   *******************/
  SliceCriterions::SlicingCriterion SliceCriterions::buildSlicingCriterionFromStmt(SgStatement* stmt) {
    std::vector<SgVarRefExp*> varRefExps = SageInterface::querySubTree<SgVarRefExp>(stmt);
    SliceCriterions::SlicingCriterion sc;
    sc.first = stmt;
    sc.second.insert(varRefExps.begin(), varRefExps.end());
    return sc;
  }

  std::string SliceCriterions::str() {
    std::ostringstream oss;
    std::vector<SlicingCriterion>::iterator it = sliceCriterions.begin();
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
      relevantCFGNSet.insert(bsl->relevantCFGNSet.begin(), relevantCFGNSet.end());
      unsigned int size_a = relevantCFGNSet.size();
      modified = modified || (size_b != size_a);
    }
    catch(std::bad_cast& bc) {
      dbg << "BackwardSlicingLattice::meetUpdate(Lattice*): " << bc.what() << "\n";
      ROSE_ASSERT(false);
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

  std::string BackwardSlicingLattice::strp(PartEdgePtr pedge, std::string indent) {
    std::ostringstream oss;
    oss << "BackwardSlicingLattice@";
    oss << "pedge=" << pedge->str() << std::endl;
    oss << "relevantMLSet=" << relevantMLSet->str(indent);
    oss << "relevantCFGNset=[";
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

  std::string BackwardSlicingLattice::str(std::string indent="") {
    return strp(latPEdge, indent);
  }

  /***************************
   * BackwardSlicingAnalysis *
   ***************************/
  void BackwardSlicingAnalysis::genInitLattice(PartPtr part, 
                                               PartEdgePtr pedge,
                                               std::vector<Lattice*>& initLattices) {
    // use the slicing criterion to initialize the BackwardSlicingLattice
    std::vector<SliceCriterions::SlicingCriterion> allSliceCriterions = sc.getSlicingCriterions();
    std::vector<SliceCriterions::SlicingCriterion>::iterator sc_it;
    std::set<SgNode*> relevantCFGNSet;
    AbstractObjectSet aos(pedge,
                          composer,
                          this,                          
                          AbstractObjectSet::may);
    for(sc_it = allSliceCriterions.begin(); sc_it != allSliceCriterions.end(); ++sc_it) {
      SgStatement* stmt = (*sc_it).first;
      std::set<SgVarRefExp*> varRefExpsSet = (*sc_it).second;
      relevantCFGNSet.insert(stmt);
      // TODO: insert ML corresponding to SgVarRefExp
    }
    BackwardSlicingLattice* bsl = new BackwardSlicingLattice(pedge, aos, relevantCFGNSet);
    initLattices.push_back(bsl);
  }

  boost::shared_ptr<DFTransferVisitor> 
  BackwardSlicingAnalysis::getTransferVisitor(PartPtr part,
                                              CFGNode cfgn,
                                              NodeState& state,
                                              std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo) {
    return boost::shared_ptr<DFTransferVisitor>(new BackwardSlicingTransfer(part, cfgn, state, dfInfo));
  }

  void BackwardSlicingTransfer::visit(SgAssignOp* sgn) {
  }

  void BackwardSlicingTransfer::visit(SgNode* sgn) {
    std::cout << "spinning\n";
  }
}; // end namespace
