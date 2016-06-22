/*****************************************
 * Author: Sriram Aananthakrishnan, 2016 *
 *****************************************/

#include "sage3basic.h"
#include "mpi_dot_value_analysis.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <algorithm>
#include "mpi_utils.h"

namespace fuse {
  // DEBUG_LEVEL(mpiDotValueAnalysisDebugLevel, 0);
  #define mpiDotValueAnalysisDebugLevel 2
  #if mpiDotValueAnalysisDebugLevel==0
  #define DISABLE_SIGHT
  #endif
  
  boost::uuids::random_generator gen;

  using namespace std;

  /*******************
   * MPIDotValueKind *
   *******************/
  MPIDotValueKind::MPIDotValueKind(MPIDotValueKind::KType ktype) : ktype_(ktype) { }
  MPIDotValueKind::MPIDotValueKind(const MPIDotValueKind& that) : ktype_(that.ktype_) { }
  MPIDotValueKind::KType MPIDotValueKind::getType() const { 
    return ktype_; 
  }

  /**************************
   * MPIDotValueDefaultKind *
   **************************/
  MPIDotValueDefaultKind::MPIDotValueDefaultKind() 
    : MPIDotValueKind(MPIDotValueKind::empty) { }

  MPIDotValueDefaultKind::MPIDotValueDefaultKind(const MPIDotValueDefaultKind& that) 
    : MPIDotValueKind(that) { }
  
  // Two default kinds are may equal
  // 1. if thatK is unknown
  // 2. if thisK and thatK are both default
  bool MPIDotValueDefaultKind::mayEqualK(MPIDotValueKindPtr thatK) { 
    if(thatK->getType() == MPIDotValueKind::unknown) return true;
    else if(thatK->getType() == MPIDotValueKind::empty) return true;
    return false;
  }
  
  // Two default kind are mustEqual if this and thatK are default
  bool MPIDotValueDefaultKind::mustEqualK(MPIDotValueKindPtr thatK) { 
    return thatK->getType() == MPIDotValueKind::empty;
  }
  
  bool MPIDotValueDefaultKind::equalSetK(MPIDotValueKindPtr thatK) { 
    return thatK->getType() == MPIDotValueKind::empty;
  }
  
  bool MPIDotValueDefaultKind::subSetK(MPIDotValueKindPtr thatK) { 
    return true;
  }
  
  bool MPIDotValueDefaultKind::isEmptyK() { 
    return true;
  }
  
  bool MPIDotValueDefaultKind::isFullK() { 
    return false;
  }
  
  MPIDotValueKindPtr MPIDotValueDefaultKind::copyK() { 
    return boost::make_shared<MPIDotValueDefaultKind>(*this);
  }
  
  std::string MPIDotValueDefaultKind::str(std::string indent) { 
    return "empty";
  }

  /**************************
   * MPIDotValueUnknownKind *
   **************************/
  MPIDotValueUnknownKind::MPIDotValueUnknownKind() 
    : MPIDotValueKind(MPIDotValueKind::unknown) { }

  MPIDotValueUnknownKind::MPIDotValueUnknownKind(const MPIDotValueUnknownKind& that) 
    : MPIDotValueKind(that) { }

  bool MPIDotValueUnknownKind::mayEqualK(MPIDotValueKindPtr thatK) {
    return true;
  }
  bool MPIDotValueUnknownKind::mustEqualK(MPIDotValueKindPtr thatK) { 
    return false;
  }

  bool MPIDotValueUnknownKind::equalSetK(MPIDotValueKindPtr thatK) { 
    return thatK->getType() == MPIDotValueKind::unknown;
  }

  bool MPIDotValueUnknownKind::subSetK(MPIDotValueKindPtr thatK) { 
    return thatK->getType() == MPIDotValueKind::unknown;
  }

  bool MPIDotValueUnknownKind::isEmptyK() { 
    return false;
  }

  bool MPIDotValueUnknownKind::isFullK() {
    return true;
  }

  MPIDotValueKindPtr MPIDotValueUnknownKind::copyK() { 
    return boost::make_shared<MPIDotValueUnknownKind>(*this);
  }

  std::string MPIDotValueUnknownKind::str(std::string indent) { 
    return "unknown";
  }

  /*************************
   * MPIDotValueStringKind *
   *************************/
  MPIDotValueStringKind::MPIDotValueStringKind(set<string> dotvalues)
    : MPIDotValueKind(MPIDotValueKind::stringT),
      dotvalues(dotvalues) {
  }

  MPIDotValueStringKind::MPIDotValueStringKind(std::string dotvalue) 
    : MPIDotValueKind(MPIDotValueKind::stringT) {
    dotvalues.insert(dotvalue);
  }
  
  MPIDotValueStringKind::MPIDotValueStringKind(const MPIDotValueStringKind& that) 
    : MPIDotValueKind(that), dotvalues(that.dotvalues) {
  }

  set<string> MPIDotValueStringKind::get_dot_values() const { 
    return dotvalues;
  }

  // If we find one element in common they are mayEquals
  bool MPIDotValueStringKind::compareMayK(MPIDotValueStringKindPtr thatK) {
    const set<string>& thatDotValues = thatK->get_dot_values();
    set<string>::const_iterator thisD = dotvalues.begin(), thatD = thatDotValues.begin();
    for( ; thisD != dotvalues.end() && thatD != thatDotValues.end(); 
         ++thisD, ++thatD) {
      if(*thisD == *thatD) return true;
    }
    return false;
  }

  // All elements
  bool MPIDotValueStringKind::compareEqualK(MPIDotValueStringKindPtr thatK) {
    const set<string>& thatDotValues = thatK->get_dot_values();
    set<string>::const_iterator thisD = dotvalues.begin(), thatD = thatDotValues.begin();
    for( ; thisD != dotvalues.end(); ++thisD, ++thatD) {
      if(*thisD != *thatD) return false;
    }
    if(thatD != thatDotValues.end()) return false;
    else return true;
  }

  bool MPIDotValueStringKind::compareSubK(MPIDotValueStringKindPtr thatK) {
    const set<string>& thatDotValues = thatK->get_dot_values();
    set<string>::const_iterator thisD = dotvalues.begin(), thatD = thatDotValues.begin();
    for( ; thisD != dotvalues.end(); ++thisD, ++thatD) {
      if(*thisD != *thatD) return false;
    }
    // It doesn't matter we have any elements to iterate in thatDotValues or not
    return true;
  }

  bool MPIDotValueStringKind::mayEqualK(MPIDotValueKindPtr thatK) { 
    if(thatK->getType() == MPIDotValueKind::unknown) return true;
    else if(thatK->getType() == MPIDotValueKind::stringT) 
      return compareMayK(boost::dynamic_pointer_cast<MPIDotValueStringKind>(thatK));
    else return false;
  }

  bool MPIDotValueStringKind::mustEqualK(MPIDotValueKindPtr thatK) { 
    return false;
  }

  bool MPIDotValueStringKind::equalSetK(MPIDotValueKindPtr thatK) { 
    if(thatK->getType() == MPIDotValueKind::stringT) 
      return compareEqualK(boost::dynamic_pointer_cast<MPIDotValueStringKind>(thatK));
    else return false;
  }

  bool MPIDotValueStringKind::subSetK(MPIDotValueKindPtr thatK) { 
    if(thatK->getType() == MPIDotValueKind::stringT) 
      return compareSubK(boost::dynamic_pointer_cast<MPIDotValueStringKind>(thatK));
    else return false;
  }

  bool MPIDotValueStringKind::mergeStringKind(MPIDotValueStringKindPtr thatK) {
    // Not doing a in place merge
    const set<string>& thatDotValues = thatK->get_dot_values();
    set<string>::const_iterator thatD = thatDotValues.begin();
    bool insert = false;
    typedef pair<set<string>::iterator, bool> InsertRV;
    InsertRV irv;
    for( ; thatD != thatDotValues.end(); ++thatD) {
      irv = dotvalues.insert(*thatD);
      insert = insert || irv.second;
    }
    return insert;
  }

  bool MPIDotValueStringKind::isEmptyK() { 
    return false;
  }

  bool MPIDotValueStringKind::isFullK() { 
    return false;
  }

  MPIDotValueKindPtr MPIDotValueStringKind::copyK() { 
    return boost::make_shared<MPIDotValueStringKind>(*this);
  }

  string MPIDotValueStringKind::str(string indent) { 
    ostringstream oss;
    oss << "[";
    set<string>::iterator d = dotvalues.begin();
    for( ; d != dotvalues.end(); ) {
      oss << *d;
      ++d;
      if(d != dotvalues.end()) oss << ", ";
    }
    oss << "]";
    return oss.str();
  }

  typedef boost::shared_ptr<SgValueExp> SgValueExpPtr;
  typedef set<boost::shared_ptr<SgValueExp> > SgValueExpPtrSet;
  string SgStringVal2String(SgStringVal* val) {
    return val->get_value();
  }

  string SgValueExpPtr2String(SgValueExpPtr sgval) {
   SgStringVal* sgsval = boost::dynamic_pointer_cast<SgStringVal>(sgval).get(); 
   if(!sgsval) {
     cerr << "Error: Building string from non-string SgValueExp"
          << " line:" << __LINE__
          << " file:" << __FILE__ << endl;
     exit(EXIT_FAILURE);
   }
   return sgsval->get_value();
  }

  set<string> SgValueExpPtrSet2String(SgValueExpPtrSet& sgValSet) {
    set<string> dotvalues;
    SgValueExpPtrSet::iterator i = sgValSet.begin();
    for( ; i != sgValSet.end(); ++i) {
      string dotvalue = SgValueExpPtr2String(*i);
      dotvalues.insert(dotvalue);
    }
    return dotvalues;
  }

  SgValueExpPtr String2SgValueExpPtr(string s) {
    SgValueExpPtr sgVal = boost::shared_ptr<SgStringVal>(SageBuilder::buildStringVal(s));
    return sgVal;
  }

  SgValueExpPtrSet String2SgValueExpPtrSet(set<string> svalues) {
    SgValueExpPtrSet sgValSet;
    set<string>::iterator s = svalues.begin();
    for( ; s != svalues.end(); ++s) {
      sgValSet.insert(String2SgValueExpPtr(*s));
    }
    return sgValSet;
  }

  /*********************
   * MPIDotValueObject *
   *********************/
  MPIDotValueObject::MPIDotValueObject(PartEdgePtr pedge)
     : Lattice(pedge), FiniteLattice(pedge), ValueObject(0) {
    kind = boost::make_shared<MPIDotValueDefaultKind>();
  }
  
  MPIDotValueObject::MPIDotValueObject(string dotvalue, PartEdgePtr pedge)
    : Lattice(pedge), FiniteLattice(pedge), ValueObject(0) {    
    kind = boost::make_shared<MPIDotValueStringKind>(dotvalue);
  }

  MPIDotValueObject::MPIDotValueObject(ValueObjectPtr vo, PartEdgePtr pedge)
    : Lattice(pedge), FiniteLattice(pedge), ValueObject(0) {
    SIGHT_VERB_DECL(scope, (sight::txt() << "MPIDotValueObject::MPIDotValueObject()", scope::medium),
                    2, mpiDotValueAnalysisDebugLevel)
    SIGHT_VERB(dbg << "vo=" << vo->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
    if(vo->isEmptyV(pedge)) kind = boost::make_shared<MPIDotValueDefaultKind>();
    else if(vo->isFullV(pedge)) {
      SIGHT_VERB(dbg << "isFull=" << vo->isFullV(pedge) << endl, 2, mpiDotValueAnalysisDebugLevel)
      kind = boost::make_shared<MPIDotValueUnknownKind>();
    }
    else if(vo->isConcrete()) {
      SgType* vtype = vo->getConcreteType();
      if(isSgTypeString(vtype)) {
        SgValueExpPtrSet vset = vo->getConcreteValue();
        // We will assume all elements are of same type
        // The set can have values from multiple analyses
        // under which the assumption above may not hold anymore
        // We will use a special type of SgType* to denote such set of values
        // Until then we will assume all values of a given value object have identical SgType
        set<string> dotvalues = SgValueExpPtrSet2String(vset);
        kind = boost::make_shared<MPIDotValueStringKind>(dotvalues);
      }
      else {
        cerr << "Warning: Non-string concrete type for MPIDotValue " << SgNode2Str(vtype)
             << " line:" << __LINE__
             << " file:" << __FILE__ << endl;
        // We will create an object of unknown type
        kind = boost::make_shared<MPIDotValueUnknownKind>();
      }  
    }
    // there is not really another possibility here
    // if we reach here we should create MPIDotValueUnknownKind
    else kind = boost::make_shared<MPIDotValueUnknownKind>();
    SIGHT_VERB(dbg << "kind=" << kind->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
  }
  
  MPIDotValueObject::MPIDotValueObject(const MPIDotValueObject& that)
    : Lattice(that), FiniteLattice(that), ValueObject(that) {
    kind = that.getKind()->copyK();
  }

  ValueObjectPtr MPIDotValueObject::copyV () const {
    return boost::make_shared<MPIDotValueObject>(*this);
  }

  MPIDotValueKindPtr MPIDotValueObject::getKind() const {
    return kind;
  }

  void MPIDotValueObject::initialize() {
    assert(false);
  }
  
  Lattice* MPIDotValueObject::copy() const {
    return new MPIDotValueObject(*this);
  }

  bool MPIDotValueObject::operator==(Lattice* that) {
    MPIDotValueObject* thatV = dynamic_cast<MPIDotValueObject*>(that); assert(thatV);
    MPIDotValueKindPtr thatK = thatV->getKind();
    return kind->equalSetK(thatK);
  }
  
  bool MPIDotValueObject::meetUpdate(Lattice* that) {
    MPIDotValueObject* thatV = dynamic_cast<MPIDotValueObject*>(that); assert(thatV);
    MPIDotValueKindPtr thatK = thatV->getKind();
    // If this is full nothing to join
    // of that is empty
    if(kind->isFullK() || thatK->isEmptyK()) return false;
    // this->kind is not full
    // thatK is not empty
    // If thatK is full set this to full
    else if(thatK->isFullK()) return setToFull();
    // If this->kind is empty
    // this->kind = thatK
    else if(kind->isEmptyK()) kind = thatK->copyK();
    // Both this->kind and thatK are not full or empty
    // Merge the dot values of the MPIDotValueStringKind
    else if(kind->getType() == MPIDotValueKind::stringT &&
            thatK->getType() == MPIDotValueKind::stringT) {
      MPIDotValueStringKindPtr skindThis = boost::dynamic_pointer_cast<MPIDotValueStringKind>(kind);
      MPIDotValueStringKindPtr skindThat = boost::dynamic_pointer_cast<MPIDotValueStringKind>(thatK);
      return skindThis->mergeStringKind(skindThat);
    }
    else {
      cerr << "Error: Unhandled case in meet of MPIDotValue"
           << "line: " << __LINE__
           << " file: " << __FILE__ << endl;
      exit(EXIT_FAILURE);
    }
  }
  
  bool MPIDotValueObject::meetUpdateV (ValueObjectPtr o, PartEdgePtr pedge) {
    MPIDotValueObjectPtr thatV = boost::dynamic_pointer_cast<MPIDotValueObject>(o); assert(thatV);
    Lattice* thatL = dynamic_cast<Lattice*>(thatV.get()); assert(thatL);
    return meetUpdate(thatL);
  }
  
  bool MPIDotValueObject::setToFull() {
    kind = boost::make_shared<MPIDotValueUnknownKind>();
    return true;
  }
  
  bool MPIDotValueObject::setToEmpty() {
    kind = boost::make_shared<MPIDotValueDefaultKind>();
    return true;
  }
  
  bool MPIDotValueObject::isFullLat() {
    return kind->isFullK();
  }

  bool MPIDotValueObject::isFullV(PartEdgePtr pedge) {
    return kind->isFullK();
  }
  
  bool MPIDotValueObject::isEmptyLat() {
    return kind->isEmptyK();
  }

  bool MPIDotValueObject::isEmptyV(PartEdgePtr pedge) {
    return kind->isEmptyK();
  }
  
  bool MPIDotValueObject::setMLValueToFull (MemLocObjectPtr ml) {
    assert(false);
  }

  bool MPIDotValueObject::mayEqualV (ValueObjectPtr o, PartEdgePtr pedge) {
    // It does not matter if the value object is MPIDotValueObject or not
    // We don't know their actual values and they could all be overlapping
    MPIDotValueObjectPtr that = boost::dynamic_pointer_cast<MPIDotValueObject>(o); assert(that);
    MPIDotValueKindPtr thatK = that->getKind();
    return kind->mayEqualK(thatK);
  }
  
  bool MPIDotValueObject::mustEqualV (ValueObjectPtr o, PartEdgePtr pedge) {
    MPIDotValueObjectPtr that = boost::dynamic_pointer_cast<MPIDotValueObject>(o); assert(that);
    MPIDotValueKindPtr thatK = that->getKind();
    return kind->mustEqualK(thatK);
  }
  
  bool MPIDotValueObject::subSetV (ValueObjectPtr o, PartEdgePtr pedge) {
    MPIDotValueObjectPtr that = boost::dynamic_pointer_cast<MPIDotValueObject>(o); assert(that);
    MPIDotValueKindPtr thatK = that->getKind();
    return kind->subSetK(thatK);
  }

  bool MPIDotValueObject::equalSetV (ValueObjectPtr o, PartEdgePtr pedge) {
    MPIDotValueObjectPtr thatV = boost::dynamic_pointer_cast<MPIDotValueObject>(o); assert(thatV);
    MPIDotValueKindPtr thatK = thatV->getKind();
    return kind->equalSetK(thatK);
  } 

  bool MPIDotValueObject::isConcrete() {
    if(kind->getType() == MPIDotValueKind::stringT) return true;
    else if(kind->getType() == MPIDotValueKind::empty ||
            kind->getType() == MPIDotValueKind::unknown) return false;
    else {
      cerr << "Error: Unhandled concrete type " << kind->str()
           << " line: " << __LINE__
           << " file: " << __FILE__ << endl;
      exit(EXIT_FAILURE);
    }    
  }
  
  SgType* MPIDotValueObject::getConcreteType() {
    if(kind->getType() == MPIDotValueKind::stringT) return SageBuilder::buildStringType();
    else {
      cerr << "Error: Unhandled concrete type " << kind->str()
           << " line: " << __LINE__
           << " file: " << __FILE__ << endl;
      exit(EXIT_FAILURE);
    }    
  }
  
  set<boost::shared_ptr< SgValueExp> > MPIDotValueObject::getConcreteValue() {
    if(kind->getType() == MPIDotValueKind::stringT) {
      MPIDotValueStringKindPtr skind = boost::dynamic_pointer_cast<MPIDotValueStringKind>(kind);
      set<string> dotvalues = skind->get_dot_values();
      return String2SgValueExpPtrSet(dotvalues);
    }
    else {
      cerr << "Error: Unhandled concrete type " << kind->str()
           << " line: " << __LINE__
           << " file: " << __FILE__ << endl;
      exit(EXIT_FAILURE);
    }    
  }
  
  string MPIDotValueObject::str(std::string indent) const {
    ostringstream oss;
    oss << "[MPIDotValue: " << kind->str() << "]";
    return oss.str();
  }
  
  /**********************
   * MDVTransferVisitor *
   **********************/
  MDVTransferVisitor::MDVTransferVisitor(PartPtr part,
                                         CFGNode cn,
                                         NodeState& state,
                                         map<PartEdgePtr, vector<Lattice*> >& dfInfo,
                                         MPIDotValueAnalysis* analysis)
    : DFTransferVisitor(part, cn, state, dfInfo),
      analysis(analysis), modified(false) {
    assert(dfInfo.size()==1);
    assert(dfInfo[part->inEdgeFromAny()].size()==1);
    assert(*dfInfo[part->inEdgeFromAny()].begin());

    Lattice *l = *dfInfo[part->inEdgeFromAny()].begin(); assert(l);
    aMapState = dynamic_cast<AbstractObjectMap*>(l); assert(aMapState);
  }

  bool MDVTransferVisitor::isMPISendOp(Function& mpif_) {
    string name = mpif_.get_name().getString();
    if(name.compare("MPI_Send")  == 0 ||
       name.compare("MPI_Isend") == 0) {
      return true;
    }
    return false;
  }

  bool MDVTransferVisitor::isMPIRecvOp(Function& mpif_) {
    string name = mpif_.get_name().getString();
    if(name.compare("MPI_Recv")  == 0 ||
       name.compare("MPI_Irecv") == 0) {
      return true;
    }
    return false;
  }

  // SgNullExpression for empty return statements
  void MDVTransferVisitor::visit(SgNullExpression* sgn) { }

  void MDVTransferVisitor::visit(SgAddressOfOp* sgn) { }
  
  void MDVTransferVisitor::visit(SgBitComplementOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgCastExp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgMinusMinusOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgMinusOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgNotOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgPlusPlusOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  } 
  
  void MDVTransferVisitor::visit(SgUnaryAddOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgUnaryOp* sgn) {
    dbg << "Unhandled SgUnaryOp=" << SgNode2Str(sgn) << endl;
    assert(false);
  }

  void MDVTransferVisitor::visit(SgPointerDerefExp* sgn) { }

  void MDVTransferVisitor::visit(SgAddOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);    
  }
  
  void MDVTransferVisitor::visit(SgAndOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgArrowExp* sgn) { }
  void MDVTransferVisitor::visit(SgArrowStarOp* sgn) { }
  
  void MDVTransferVisitor::visit(SgAssignOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MemLocObjectPtr lml = analysis->getComposer()->Expr2MemLoc(sgn->get_lhs_operand(), part->inEdgeFromAny(), analysis); 
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    MPIDotValueObjectPtr lmdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull(); lmdv->setToFull();
    modified = aMapState->insert(ml, mdv) || modified;
    modified = aMapState->insert(lml, lmdv) || modified;
  }
  
  void MDVTransferVisitor::visit(SgBitAndOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgBitOrOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgBitXorOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgCommaOpExp* sgn) { assert(0); }

  void MDVTransferVisitor::visit(SgCompoundAssignOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MemLocObjectPtr lml = analysis->getComposer()->Expr2MemLoc(sgn->get_lhs_operand(), part->inEdgeFromAny(), analysis); 
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    MPIDotValueObjectPtr lmdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull(); lmdv->setToFull();
    modified = aMapState->insert(ml, mdv) || modified;
    modified = aMapState->insert(lml, lmdv) || modified;
  }
  
  void MDVTransferVisitor::visit(SgDivideOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgDotExp* sgn) { }
  void MDVTransferVisitor::visit(SgDotStarOp* sgn) { }
  
  void MDVTransferVisitor::visit(SgEqualityOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgGreaterOrEqualOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgGreaterThanOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgLessOrEqualOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgLessThanOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgLshiftOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgModOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgMultiplyOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgNotEqualOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgOrOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgPntrArrRefExp* sgn) { }
  
  void MDVTransferVisitor::visit(SgSubtractOp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);
  }
  
  void MDVTransferVisitor::visit(SgBinaryOp* sgn) {
    SIGHT_VERB(dbg << "Unhandled Binary Op = " << SgNode2Str(sgn) << endl,
               2, mpiDotValueAnalysisDebugLevel)
    assert(false);
  }

  void MDVTransferVisitor::visit(SgConditionalExp* sgn) { assert(0); }
  
  void MDVTransferVisitor::visit(SgDeleteExp* sgn) { }
  void MDVTransferVisitor::visit(SgExprListExp* sgn) { }  
  void MDVTransferVisitor::visit(SgFunctionRefExp* sgn) { }

  // Nothing to do here
  // SgInitializedName is set to unknown instead
  void MDVTransferVisitor::visit(SgInitializer* sgn) { }
  
  void MDVTransferVisitor::visit(SgNewExp* sgn) { }
  void MDVTransferVisitor::visit(SgSizeOfOp* sgn) { }
  
  void MDVTransferVisitor::visit(SgValueExp* sgn) {
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
    mdv->setToFull();
    modified = aMapState->insert(ml, mdv);    
  }
  
  void MDVTransferVisitor::visit(SgVarRefExp* sgn) { }
  
  void MDVTransferVisitor::visit(SgExpression* sgn) {
    SIGHT_VERB(dbg << "Unhandled expr=" << SgNode2Str(sgn) << endl,
               2, mpiDotValueAnalysisDebugLevel)    
    assert(0);
  }

  void MDVTransferVisitor::visit(SgInitializedName* sgn) {
    if(sgn->get_initializer()) {
      MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
      MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(part->inEdgeFromAny());
      mdv->setToFull();
      modified = aMapState->insert(ml, mdv);
    }
  }

  void MDVTransferVisitor::havocUnknownValues(SgFunctionCallExp* sgn) {
    if(Part::isOutgoingFuncCall(cn)) {
      // Check if it has side-effects attribute
      if(sgn->getAttribute("fuse:UnknownSideEffectsAttribute")) {
        SgExprListExp* args = sgn->get_args();
        const SgExpressionPtrList& argsExprList = args->get_expressions();
        SgExpressionPtrList::const_iterator it = argsExprList.begin();
        // Iterate through the argument list
        // Check for ValueUnknownAttribute
        for( ; it != argsExprList.end(); ++it) {
          SgExpression* uexpr = *it;
          if(uexpr->getAttribute("fuse:ValueUnknownAttribute")) {
            // Find the expr whose value was modified by this function
            // It could be hiding under address of expr: &x
            // It could be hiding under cast expr: (int) x
            SgExpression* mexpr = traverseAST(uexpr);
            MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(mexpr, part->inEdgeFromAny(), analysis);
            analysis->getComposer()->HavocMLValue(ml, dfInfo);
          }
        }
      }
    }
  }

  // void MDVTransferVisitor::visit(SgInitializedName* sgn) {
  //   MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
  //   analysis->getComposer()->HavocMLValue(ml, dfInfo);
  // }

  void MDVTransferVisitor::visit(SgFunctionCallExp* sgn) {
    Function mpif_(sgn);
    if(Part::isOutgoingFuncCall(cn)) {
      havocUnknownValues(sgn);
      // If this is a MPI function
      if(mpif_.get_name().getString().find("MPI_", 0) == 0) {
        MPICommOp op(mpif_);
        if(op.isMPISendOp()) {  
          modified = transferSendOp(mpif_, sgn);
        }
        else if(op.isMPIBcastOp()) {
          modified = transferOutBcastOp(mpif_, sgn);
        }
        else if(op.isMPIUnknownOp()) {
          cerr << "Unhandled MPI Function " << mpif_.get_name().getString() 
               << ", file:" << __FILE__ << ", line: " << __LINE__ << endl;
          exit(EXIT_FAILURE);
        }
      }
    }
    else if(Part::isIncomingFuncCall(cn) &&
            mpif_.get_name().getString().find("MPI_", 0) == 0) {
      MPICommOp op(mpif_);
      if(op.isMPIRecvOp()) {
        modified = transferRecvOp(mpif_, sgn);
      }
      else if(op.isMPIBcastOp()) {
        modified = transferInBcastOp(mpif_, sgn);
      }
      else if(op.isMPIUnknownOp()) {
        cerr << "Unhandled MPI Function " << mpif_.get_name().getString() 
             << ", file:" << __FILE__ << ", line: " << __LINE__ << endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  bool MDVTransferVisitor::transferSendOp(Function& mpif_, SgFunctionCallExp* sgn) {
    SIGHT_VERB_DECL(scope, (sight::txt() << "MDVTransferVisitor::transferSendOp", scope::medium),
                    2, mpiDotValueAnalysisDebugLevel)
    assert(isMPISendOp(mpif_));
    MPISendOpCallSite sendcs(mpif_, sgn);
    SgNode* bexpr = sendcs.getSendBuffer();
    MemLocObjectPtr bml = analysis->getComposer()->Expr2MemLoc(bexpr, part->inEdgeFromAny(), analysis);
    string dotvalue = analysis->part2dotid(part);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(dotvalue, part->inEdgeFromAny());

    SIGHT_VERB(dbg << "bml=" << bml->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
    SIGHT_VERB(dbg << "mdv=" << mdv->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
      // cout << "transferSend: bml=" << bml->str() << endl;
      // cout << "transferSend: mdv=" << mdv->str() << endl;

    return aMapState->insert(bml, boost::dynamic_pointer_cast<Lattice>(mdv));                            
  }

  bool MDVTransferVisitor::transferRecvOp(Function& mpif_, SgFunctionCallExp* sgn) {
    SIGHT_VERB_DECL(scope, (sight::txt() << "MDVTransferVisitor::transferRecvOp", scope::medium),
                    2, mpiDotValueAnalysisDebugLevel)
    assert(isMPIRecvOp(mpif_));
    MPIRecvOpCallSite recvcs(mpif_, sgn);
    SgNode* bexpr = recvcs.getRecvBuffer();
    MemLocObjectPtr bml = analysis->getComposer()->Expr2MemLoc(bexpr, part->inEdgeFromAny(), analysis);
    ValueObjectPtr v = analysis->getComposer()->Expr2Val(bexpr, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(v, part->inEdgeFromAny());
    SIGHT_VERB(dbg << "Expr2Val(bexpr) = " << v->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
    SIGHT_VERB(dbg << "mdv=" << mdv->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
    // cout << "transferRecv: bml = " << bml->str() << endl;
    // cout << "transferRecv: mdv=" << mdv->str() << endl;
    return aMapState->insert(bml, mdv);
  }

  bool MDVTransferVisitor::transferOutBcastOp(Function& mpif_, SgFunctionCallExp* sgn) {
    SIGHT_VERB_DECL(scope, (sight::txt() << "MDVTransferVisitor::transferOutBcastOp", scope::medium),
                    2, mpiDotValueAnalysisDebugLevel)
    MPIBcastOpCallSite bcastcs(mpif_, sgn);
    SgNode* rexpr = bcastcs.getBcastRootExpr();
    ValueObject2Int vo2int(analysis->getComposer(), part->inEdgeFromAny(), 
                           analysis, mpiDotValueAnalysisDebugLevel);
    int root = vo2int(rexpr);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // If my rank is same as the root
    // Set the value of the buffer
    if(root == rank) {
      SgNode* bexpr = bcastcs.getBcastBuffer();
      MemLocObjectPtr bml = analysis->getComposer()->Expr2MemLoc(bexpr, part->inEdgeFromAny(), analysis);
      string dotvalue = analysis->part2dotid(part);
      MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(dotvalue, part->inEdgeFromAny());
      SIGHT_VERB(dbg << "bml=" << bml->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
      SIGHT_VERB(dbg << "mdv=" << mdv->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
      return aMapState->insert(bml, mdv);
    }
    else return false;
  }

  bool MDVTransferVisitor::transferInBcastOp(Function& mpif_, SgFunctionCallExp* sgn) {
    SIGHT_VERB_DECL(scope, (sight::txt() << "MDVTransferVisitor::transferInBcastOp", scope::medium),
                    2, mpiDotValueAnalysisDebugLevel)
    MPIBcastOpCallSite bcastcs(mpif_, sgn);
    SgNode* bexpr = bcastcs.getBcastBuffer();
    MemLocObjectPtr bml = analysis->getComposer()->Expr2MemLoc(bexpr, part->inEdgeFromAny(), analysis);
    ValueObjectPtr v = analysis->getComposer()->Expr2Val(bexpr, part->inEdgeFromAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::make_shared<MPIDotValueObject>(v, part->inEdgeFromAny());
    SIGHT_VERB(dbg << "Expr2Val(bexpr) = " << v->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
    SIGHT_VERB(dbg << "mdv=" << mdv->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
    //assert(!mdv->isFullLat());
    return aMapState->insert(bml, mdv);
  }

  bool MDVTransferVisitor::finish() {
    return modified;
  }

  /***********************
   * MPIDotValueAnalysis *
   ***********************/
  void MPIDotValueAnalysis::initAnalysis(std::set<PartPtr>& startingParts) {
  }

  void MPIDotValueAnalysis::genInitLattice(PartPtr part, PartEdgePtr pedge, 
                                           std::vector<Lattice*>& initLattices) {
    AbstractObjectMap* amap = new AbstractObjectMap(boost::make_shared<MPIDotValueObject>(pedge),
                                                    pedge,
                                                    getComposer(),
                                                    this);
    initLattices.push_back(amap);
  }
  
  bool MPIDotValueAnalysis::transfer(PartPtr part, CFGNode cn, NodeState& state, 
                                   map<PartEdgePtr, vector<Lattice*> >& dfInfo) {
    assert(false);
  }

  void MPIDotValueAnalysis::createContext2dotid(PartContextPtr pcontext) {
    if(context2DotStrMap.find(pcontext) != context2DotStrMap.end()) return;
    ostringstream oss;
    oss << "cc" << contextid++;
    context2DotStrMap[pcontext] = oss.str();
  }

  string MPIDotValueAnalysis::context2dotid(PartContextPtr pcontext) const {
    map<PartContextPtr, string>::const_iterator f = context2DotStrMap.find(pcontext);
    assert(f != context2DotStrMap.end());
    return f->second;
  }

  string MPIDotValueAnalysis::part2dotid(PartPtr part) const {
    int flag;
    MPI_Initialized(&flag); assert(flag);
    int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    set<CFGNode> cfgnset = part->CFGNodes();
    assert(cfgnset.size() == 1);
    CFGNode cn = *cfgnset.begin();

    ostringstream oss;    
    oss << cn.id() << "_rank_" << rank << "_" << context2dotid(part->getPartContext());
    string id = oss.str();

    // Sanity checks
    std::replace(id.begin(), id.end(), ' ', '_');
    std::replace(id.begin(), id.end(), ':', '_');

    return id;
  }
  
  boost::shared_ptr<DFTransferVisitor>
  MPIDotValueAnalysis::getTransferVisitor(PartPtr part, 
                                          CFGNode cfgn, 
                                          NodeState& state, 
                                          map<PartEdgePtr, vector<Lattice*> >& dfInfo) {
    // For each new context create a dot id
    createContext2dotid(part->getPartContext());    
    // can't use boost::make_shared here
    return boost::shared_ptr<MDVTransferVisitor>(new MDVTransferVisitor(part,cfgn,state,dfInfo, this));
  }

  ValueObjectPtr MPIDotValueAnalysis::Expr2Val(SgNode* sgn, PartEdgePtr pedge) {
    SIGHT_VERB_DECL(scope, (sight::txt() << "MPIDotValueAnalysis::Expr2Val(sgn=" 
                            << SgNode2Str(sgn) << ",pedge=" 
                            << pedge->str() << ")", scope::medium),
                    2, mpiDotValueAnalysisDebugLevel)
    Composer* composer = getComposer();
    assert(pedge->source() || pedge->target());
    NodeState* state;
    Lattice* l;
    // outEdge
    if(pedge->source()) {
      state = NodeState::getNodeState(this, pedge->source());
      l = state->getLatticeBelow(this, pedge, 0);
    }
    // inEdge
    else if(pedge->target()) {
      state = NodeState::getNodeState(this, pedge->target());
      l = state->getLatticeAbove(this, pedge, 0);
    }
    else assert(0); // should never reach here

    assert(l);
    AbstractObjectMap* aMapState = dynamic_cast<AbstractObjectMap*>(l); assert(aMapState);

    MemLocObjectPtr ml = composer->Expr2MemLoc(sgn, pedge, this);
    MPIDotValueObjectPtr mdv = boost::dynamic_pointer_cast<MPIDotValueObject>(aMapState->get(ml));
    SIGHT_VERB(dbg << "ml=" << ml->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
    SIGHT_VERB(dbg << "mdv=" << mdv->str() << endl, 2, mpiDotValueAnalysisDebugLevel)
      // cout << "Expr2Val: ml=" << ml->str() << endl;
      // cout << "Expr2Val: mdv=" << mdv->str() << endl;    
    if(mdv->isFullLat()) return composer->Expr2Val(sgn, pedge, this);
    else if(mdv->isEmptyLat()) assert(0);
    return mdv;
  }

  string MPIDotValueAnalysis::str(string indent) const {
    return "MPIDotValueAnalysis";
  }

  /************************
   * MPIDotGraphGenerator *
   ************************/
  MPIDotGraphGenerator::MPIDotGraphGenerator(MPIDotValueAnalysis* analysis) : analysis(analysis) { }

  bool isMPIRecvOp(Function mpif_) {
    string mpifname = mpif_.get_name().getString();
    if(mpifname.compare("MPI_Recv") == 0 ||
       mpifname.compare("MPI_Irecv") == 0) { return true; }
    return false;       
  }

  bool MPIDotGraphGenerator::isMPIOpATSNode(PartPtr part) {
    SgFunctionCallExp* callexp = part->mustSgNodeAll<SgFunctionCallExp>();
    if(callexp) {
      Function mpif_(callexp);
      return mpif_.get_name().getString().find("MPI_", 0) == 0;
    }
    return false;
  }

  //! Determine if we need to add a communication edge at this MPI function call
  //! MPI_Recv
  //! MPI_Bcast
  //! MPI_Reduce
  bool MPIDotGraphGenerator::isMPICommOpATSNode(PartPtr part) {
    SgFunctionCallExp* callexp = part->mustSgNodeAll<SgFunctionCallExp>();
    if(callexp) {
      Function mpif_(callexp);
      MPICommOp op(mpif_);
      if(op.isMPIRecvOp()) return true;
      else if(op.isMPIBcastOp()) return true;
      else return false;
    }
    return false;
  }

  bool MPIDotGraphGenerator::isRecvOpATSNode(PartPtr part) {
    SgFunctionCallExp* callexp = part->mustSgNodeAll<SgFunctionCallExp>();
    if(callexp) {
      Function mpif_(callexp);
      MPIRecvOpCallSite recvcs(mpif_, callexp);
      if(recvcs.isRecvOp()) return true;
    }
    return false;
  }

  bool MPIDotGraphGenerator::isBcastOpATSNode(PartPtr part) {
    SgFunctionCallExp* callexp = part->mustSgNodeAll<SgFunctionCallExp>();
    if(callexp) {
      Function mpif_(callexp);
      MPICommOp op(mpif_);
      return op.isMPIBcastOp();
    }
    return false;
  }

  set<string> MPIDotGraphGenerator::getBcastMPIDotValue(PartPtr part) {
    SgFunctionCallExp* callexp = part->mustSgNodeAll<SgFunctionCallExp>(); assert(callexp);
    Function mpif_(callexp);
    MPIBcastOpCallSite bcastcs(mpif_, callexp);
    NodeState* state = NodeState::getNodeState(analysis, part); assert(state);
    Lattice* l = state->getLatticeBelow(analysis, part->outEdgeToAny(), 0); assert(l);
    AbstractObjectMap* aMapState = dynamic_cast<AbstractObjectMap*>(l); assert(aMapState);
    SgNode* bexpr = bcastcs.getBcastBuffer();
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(bexpr, part->outEdgeToAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::dynamic_pointer_cast<MPIDotValueObject>(aMapState->get(ml));
    // cout << "getRecvMPIDotValue:ml=" << ml->str() << endl;
    // cout << "getRecvMPIDotValue:mdv=" << mdv->str() << endl;
    assert(mdv && !mdv->isFullLat());
    MPIDotValueKindPtr kind = mdv->getKind();
    MPIDotValueStringKindPtr skind = boost::dynamic_pointer_cast<MPIDotValueStringKind>(kind); assert(skind);
    return skind->get_dot_values();   
  }

  set<string> MPIDotGraphGenerator::getRecvMPIDotValue(PartPtr part) {
    SgFunctionCallExp* callexp = part->mustSgNodeAll<SgFunctionCallExp>(); assert(callexp);
    Function mpif_(callexp);
    MPIRecvOpCallSite recvcs(mpif_, callexp);
    NodeState* state = NodeState::getNodeState(analysis, part); assert(state);
    Lattice* l = state->getLatticeBelow(analysis, part->outEdgeToAny(), 0); assert(l);
    AbstractObjectMap* aMapState = dynamic_cast<AbstractObjectMap*>(l); assert(aMapState);
    SgNode* bexpr = recvcs.getRecvBuffer();
    MemLocObjectPtr ml = analysis->getComposer()->Expr2MemLoc(bexpr, part->outEdgeToAny(), analysis);
    MPIDotValueObjectPtr mdv = boost::dynamic_pointer_cast<MPIDotValueObject>(aMapState->get(ml));
    // cout << "getRecvMPIDotValue:ml=" << ml->str() << endl;
    // cout << "getRecvMPIDotValue:mdv=" << mdv->str() << endl;
    assert(mdv && !mdv->isFullLat());
    MPIDotValueKindPtr kind = mdv->getKind();
    MPIDotValueStringKindPtr skind = boost::dynamic_pointer_cast<MPIDotValueStringKind>(kind); assert(skind);
    return skind->get_dot_values();   
  }

  string MPIDotGraphGenerator::cfgn2label(CFGNode cfgn) {
    ostringstream oss;
    string node_s;
    SgNode* sgn = cfgn.getNode();
    // node_s = SageInterface::get_name(sgn);
    node_s = CFGNode2Str(cfgn);
    if(node_s.length() > 20) {
      node_s.resize(20); node_s += "...";
    }
    oss << node_s << "\n<" << sgn->class_name() << "> "
        << " line:" << sgn->get_startOfConstruct()->get_line();
    return oss.str();
  }

  string MPIDotGraphGenerator::part2label(PartPtr part) {
    set<CFGNode> cfgnset = part->CFGNodes();
    ostringstream oss;
    set<CFGNode>::iterator ci = cfgnset.begin();
    for( ; ci != cfgnset.end(); ) {      
      oss << cfgn2label(*ci);
      ++ci;
      if(ci != cfgnset.end()) oss << ", ";      
    }

    int flag;
    MPI_Initialized(&flag);
    if(flag) {
      int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      oss << " rank: " << rank;
    }
    return oss.str();
  }
  
  string MPIDotGraphGenerator::part2dot(PartPtr part) {
    ostringstream oss;
    string nodeColor = "black";
    if (part->mustSgNodeAll<SgStatement>()) nodeColor = "blue";
    else if (isMPIOpATSNode(part)) nodeColor = "firebrick1";
    else if (part->mustSgNodeAll<SgExpression>()) nodeColor = "darkgreen";
    else if (part->mustSgNodeAll<SgInitializedName>()) nodeColor = "brown";
    // First write the id
    oss << analysis->part2dotid(part) << " ";
    // Write the attributes
    oss << "[label=\"" << escapeString(part2label(part)) << "\""
        << ", color=" << nodeColor << "];\n";
    return oss.str();
  }

  string MPIDotGraphGenerator::partedge2dot(PartEdgePtr pedge) {
    ostringstream oss;
    PartPtr s = pedge->source();
    PartPtr t = pedge->target();
    // First write the dot edge
    oss << analysis->part2dotid(s) << " -> " << analysis->part2dotid(t);
    oss << " [color=\"black\"];" << endl;
    return oss.str();
  }

  string MPIDotGraphGenerator::commedge2dot(string sdotvalue, PartPtr target) {
    ostringstream oss;
    string tdotvalue = analysis->part2dotid(target);    
    oss << sdotvalue << " -> " << tdotvalue << " [color=\"firebrick1\", constraint=false, weight=20];" << endl;
    return oss.str();
  }

  string MPIDotGraphGenerator::commedges2dot(set<string> sdotvalues, PartPtr target) {
    ostringstream oss;
    string tdotvalue = analysis->part2dotid(target);
    set<string>::iterator s = sdotvalues.begin();
    for( ; s != sdotvalues.end(); ++s) {
      oss << *s << " -> " << tdotvalue << " [color=\"firebrick1\", constraint=false, weight=20];" << endl;
    }
    return oss.str();
  }

  string MPIDotGraphGenerator::recvcommedge2dot(PartPtr part) {
    set<string> rdotvalues = getRecvMPIDotValue(part);
    return commedges2dot(rdotvalues, part);
  }

  string MPIDotGraphGenerator::bcastcommedge2dot(PartPtr part) {
    SgFunctionCallExp* callexp = part->mustSgNodeAll<SgFunctionCallExp>(); assert(callexp);
    Function mpif_(callexp);
    MPIBcastOpCallSite bcastcs(mpif_, callexp);
    SgNode* rexpr = bcastcs.getBcastRootExpr();
    
    int root;
    OpValueObject2Int opvo2int(analysis->getComposer(), part->outEdgeToAny(), 
                               analysis, mpiDotValueAnalysisDebugLevel);
    root = opvo2int(callexp, rexpr);
    int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(root != rank) {
      set<string> rdotvalues = getBcastMPIDotValue(part);
      return commedges2dot(rdotvalues, part);
    }
    return "";
  }
  
  void MPIDotGraphGenerator::generateDot() {
    Composer* composer = analysis->getComposer();
    set<PartPtr> initial = composer->GetStartAStates(analysis);
    fw_dataflowGraphEdgeIterator<PartEdgePtr, PartPtr> ei;

    set<PartPtr>::iterator ip = initial.begin();
    for( ; ip != initial.end(); ++ip) {
      ei.addStart(*ip);
    }

    int flag;
    MPI_Initialized(&flag);
    int rank = 0;
    if(flag) {
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    }

    nodess.clear(); edgess.clear();

    nodess << "\n subgraph cluster_" << rank << " {\n";
    string indent = "    ";

    while(ei != fw_dataflowGraphEdgeIterator<PartEdgePtr, PartPtr>::end()) {
      PartPtr part = ei.getPart();
      // Write the node
      nodess << indent << part2dot(part);

      // Write the edges
      list<PartEdgePtr> oedges = part->outEdges();
      list<PartEdgePtr>::iterator oe = oedges.begin();
      for( ; oe != oedges.end(); ++oe) {
        edgess << indent << partedge2dot(*oe);               
      }

      // Write comm edges if any
      set<CFGNode> cfgnodes;
      // Check if we have a commuication edge on this SgFunctionCallExp for MPI_Recv
      if(isMPIOpATSNode(part) && part->mustIncomingFuncCall(cfgnodes)) {
        if(isRecvOpATSNode(part)) edgess << indent << recvcommedge2dot(part);
        else if(isBcastOpATSNode(part)) {
          string bcastcommedge = bcastcommedge2dot(part);
          // When the process is root this string might be empty
          if(bcastcommedge.size() > 0) edgess << indent << bcastcommedge;
        }
      }
      ei.pushAllDescendants();      
      ei++;
    }
    nodess << indent << "}" << endl;
  }

  void MPIDotGraphGenerator::generateDotFile() {
    MPI_File file;

    int iflag;
    MPI_Initialized(&iflag);
    assert(iflag);
    if(iflag) {
    
      SgProject* project = SageInterface::getProject();
      SgFile* sgfile = project->get_files()[0]; assert(sgfile);
      string filename = StringUtility::stripPathFromFileName(sgfile->getFileName()) + ".comm.ats.dot"; 
      MPI_File_open(MPI_COMM_WORLD, filename.c_str(),
                    MPI_MODE_CREATE | MPI_MODE_WRONLY,
                    MPI_INFO_NULL, &file);

      int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      int size; MPI_Comm_size(MPI_COMM_WORLD, &size);

      ostringstream oss;
      if(rank == 0) {
        oss << "digraph G {" << endl;
      }
    
      oss << nodess.str();
      oss << edgess.str() << endl;

      if(rank == size-1) {
        oss << "}";
      }
    
      string buff = oss.str();
      char * writable = new char[buff.size() + 1];
      std::copy(buff.begin(), buff.end(), writable);
      writable[buff.size()] = '\0'; // don't forget the terminating 0
      unsigned int buffsize = strlen(writable);

      // do all gather

      MPI_Barrier(MPI_COMM_WORLD);
      unsigned int bufflen[size]; //offsets is where a process need to start writing from
      MPI_Allgather(&buffsize, 1, MPI_UNSIGNED, bufflen, 1, MPI_UNSIGNED, MPI_COMM_WORLD);

      unsigned int offsets[size];
      int reduce = 0;
      for(int oi = 1; oi < size; ++oi) {
        reduce = reduce + bufflen[oi-1];
        offsets[oi] = reduce;        
      }
      offsets[0] = 0;

      MPI_Offset offset, c_offset;
      offset = (long long) offsets[rank] * sizeof(char);

      // MPI_File_seek(file, offset, MPI_SEEK_SET);
      MPI_Status status;
      MPI_File_write_at(file, offset, writable, buffsize, MPI_CHAR, &status);

      // MPI_File_get_position(fh, &my_current_offset);
  
      // printf ("%3d: my current offset is %lld\n", my_rank, my_current_offset);

      MPI_File_close(&file);
    
      // MPI_File_set_view(file, rank * buffsize * sizeof(char),
      //                    MPI_CHAR, MPI_CHAR, "native",
      //                    MPI_INFO_NULL);
      // MPI_File_set_atomicity(file, 1);

      delete[] writable;
    } // end if flag
  }
}; // end namespace
