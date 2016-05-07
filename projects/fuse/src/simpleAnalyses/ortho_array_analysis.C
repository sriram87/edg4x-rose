#include "sage3basic.h"
#include "ortho_array_analysis.h"
#include <boost/make_shared.hpp>

using namespace sight;
using namespace std;

namespace fuse {
  DEBUG_LEVEL(arrayAnalysisDebugLevel, 2);

  /***********
   * ArrayML *
   ***********/
  ArrayML::ArrayML(SgNode* sgn, MemRegionObjectPtr region, IntersectValueObjectPtr iv) 
    : MemLocObject(sgn),
      array_ref(sgn), region(region), iv(iv), mtype(array) { }

  ArrayML::ArrayML(SgNode* sgn, MemLocObjectPtr ml)
    : MemLocObject(sgn),
      array_ref(0), nonArrayML(ml), mtype(notarray) { }

  ArrayML::ArrayML(const ArrayML& that) : MemLocObject(that) {
    assert(false);
  }

  ArrayML::MType ArrayML::getMType() const {
    return mtype;
  }

  MemLocObjectPtr ArrayML::getNonArrayML() const {
    return nonArrayML;
  }

  MemRegionObjectPtr ArrayML::getArrayMR() const {
    return region;
  }

  IntersectValueObjectPtr ArrayML::getArrayIndexValue() const {
    return iv;
  }

  MemLocObjectPtr ArrayML::copyML() const { 
    return boost::make_shared<ArrayML>(*this);
  }

  bool ArrayML::mayEqualML(MemLocObjectPtr that, PartEdgePtr pedge) {
    ArrayMLPtr thatML = boost::dynamic_pointer_cast<ArrayML>(that); assert(thatML);
    if(mtype == ArrayML::full ||
       thatML->getMType() == ArrayML::full) return true;

    // When the two types are not the same
    else if((mtype == ArrayML::array && thatML->getMtype() == ArrayML::notarray) ||
            (mtype == ArrayML::notarray && thatML->getMtype() == ArrayML::array)) {
    }

    // When both ArrayML are 'notarray' type
    else if(mtype == ArrayML::notarray &&
            thatML->getMType() == ArrayML::notarray) {
      return nonArrayML->mayEqualML(thatML->getNonArrayML(), pedge);
    }

    // When both ArrayML are 'array' type
    else if(mtype == ArrayML::array &&
            thatML->getMType() == ArrayML::array) {
      // When the regions are mustEqualMR
      // Check if their index mayEquals
      if(region->mustEqualMR(thatML->getArrayMR(), pedge)) {
        return iv->mayEqualV(thatML->getArrayIndexValue(), pedge);
      }
      // When the regions are not mustEqualMR
      else return region->mayEqualMR(thatML->getArrayMR(), pedge);
    }

    // mtype == ArrayML::empty
    else {
    }
  }

  bool ArrayML::mustEqualML(MemLocObjectPtr that, PartEdgePtr pedge) {
    ArrayMLPtr thatML = boost::dynamic_pointer_cast<ArrayML>(that); assert(thatML);
    if(mtype != thatML->getMType()) return false;

    // both mtype are same
    if(mtype == ArrayML::full) return false;

    if(mtype == ArrayML::notarray) {
      return nonArrayML->mustEqualML(thatML->getNonArrayML(), pedge);
    }

    else if(mtype == ArrayML::array) {
      MemRegionObjectPtr thatArrMR = thatML->getArrayMR();
      if(!region->mustEqualMR(thatArrMR,pedge)) return false;
      IntersectValueObjectPtr thatArrInd = thatML->getArrayIndexValue();
      return iv->mustEqualV(thatArrInd, pedge);
    }

    else if(mtype == ArrayML::empty) {
      return thatML->getMType() == mtype;
    }
    else return false;
  }
    
  // Returns whether the two abstract objects denote the same set of concrete objects
  bool ArrayML::equalSetML(MemLocObjectPtr that, PartEdgePtr pedge) {
    ArrayMLPtr thatML = boost::dynamic_pointer_cast<ArrayML>(that); assert(thatML);
    if(mtype != thatML->getMType()) return false;

    // both mtype are same
    if(mtype == ArrayML::full || mtype == ArrayML::empty)
      return thatML->getMType() == mtype;

    if(mtype == ArrayML::notarray) {
      return nonArrayML->equalSetML(thatML->getNonArrayML(), pedge);
    }

    // only mtype left = ArrayML::array
    else {
      MemRegionObjectPtr thatArrMR = thatML->getArrayMR();
      if(!region->equalSetMR(thatArrMR, pedge)) return false;
      IntersectValueObjectPtr thatArrInd = thatML->getArrayIndexValue();
      return iv->equalSetV(thatArrInd, pedge);
    }
  }
    
  // Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given abstract object.
  bool ArrayML::subSetML(MemLocObjectPtr that, PartEdgePtr pedge) {
    ArrayMLPtr thatML = boost::dynamic_pointer_cast<ArrayML>(that); assert(thatML);
    if(mtype != thatML->getMType()) return false;

    // both mtype are same
    if(mtype == ArrayML::full || mtype == ArrayML::empty) 
      return thatML->getMType() == mtype;

    if(mtype == ArrayML::notarray) {
      return nonArrayML->subSetML(thatML->getNonArrayML(), pedge);
    }

    // only mtype left = ArrayML::array
    else {
      MemRegionObjectPtr thatArrMR = thatML->getArrayMR();
      if(!region->subSetMR(thatArrMR, pedge)) return false;
      IntersectValueObjectPtr thatArrInd = thatML->getArrayIndexValue();
      return iv->subSetV(thatArrInd, pedge);
    }
  }
    
  bool ArrayML::isLiveML(PartEdgePtr pedge) {
    if(mtype == ArrayML::array) {
      return region->isLiveMR(pedge);
    }
    else if(mtype == ArrayML::notarray) {
      return nonArrayML->isLiveML(pedge);
    }
    else return true;
  }
    
  // Computes the meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  bool ArrayML::meetUpdateML(MemLocObjectPtr that, PartEdgePtr pedge) {
    assert(0);
    return false;
  }
    
  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  bool ArrayML::isFullML(PartEdgePtr pedge) {
    return mtype == ArrayML::full;
  }

  // Returns whether this AbstractObject denotes the empty set.
  bool ArrayML::isEmptyML(PartEdgePtr pedge) {
    return mtype == ArrayML::empty;
  }
    
  // Set this object to represent the set of all possible MemLocs
  // Return true if this causes the object to change and false otherwise.
  bool ArrayML::setToFull() {
    assert(0);
    return false;
  }

  // Set this Lattice object to represent the empty set of MemLocs.
  // Return true if this causes the object to change and false otherwise.
  bool ArrayML::setToEmpty() {
    assert(0);
    return false;
  }
  // pretty print
  std::string ArrayML::str(std::string indent) const {
    ostringstream oss;
    oss << "[ArrayML:";

    // Print Mtype
    if(mtype == ArrayML::array) oss << "arr ";
    else if(mtype == ArrayML::notarray) oss << "notarr ";
    else if(mtype == ArrayML::empty) oss << "empty ";
    else oss << "full ";
    
    // If array type print region and index
    if(mtype == ArrayML::array) {
      oss << "reg: " << region->str();
      oss << ", iv: " << iv->str();
    }
    // if non array type print its ml
    else if(mtype == ArrayML::notarray) {
      oss << "ml: " << nonArrayML->str();
    }
    oss << "]";
    return oss.str();
  }

  /*****************
   * ArrayAnalysis *
   *****************/  
  MemLocObjectPtr  ArrayAnalysis::Expr2MemLoc(SgNode* n, PartEdgePtr pedge) {
    scope s(sight::txt() 
            << "ArrayAnalysis::Expr2MemLoc(n="<<SgNode2Str(n) 
            << ", pedge="<<pedge->str()<<")", scope::medium, attrGE("arrayAnalysisDebugLevel", 2));
    MemLocObjectPtr ml;
    if(isSgPntrArrRefExp(n) && 
       (!isSgPntrArrRefExp (n->get_parent()) || 
        !isSgPntrArrRefExp (isSgPntrArrRefExp (n->get_parent())->get_lhs_operand()))) {
      assert(false);
    }
    else {
      MemLocObjectPtr nonArrML = getComposer()->Expr2MemLoc(n, pedge, this);
      ArrayMLPtr arrayML = boost::make_shared<ArrayML>(n, nonArrML);
      ml = arrayML;
    }
  
    return ml;
  }

}; // end namespace
