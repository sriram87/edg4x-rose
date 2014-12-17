#include "sage3basic.h"
#include "abstract_object.h"
#include <iostream>
#include "sight.h"
#include "compose.h"

using namespace std;
using namespace sight;

namespace fuse {

AbstractObjectPtr NULLAbstractObject;

AbstractObject::~AbstractObject() {
}

// Functions that identify the type of AbstractObject this is. Should be over-ridden by derived
// classes to save the cost of a dynamic cast.
bool AbstractObject::isValueObject() {
  //return dynamic_cast<ValueObject*>(this);
  return getAOType() == Value;
}
bool AbstractObject::isCodeLocObject() {
  //return dynamic_cast<CodeLocObject*>(this);
  return getAOType() == CodeLoc;
}
bool AbstractObject::isMemRegionObject() {
  //return dynamic_cast<MemRegionObject*>(this);
  return getAOType() == MemRegion;
}
bool AbstractObject::isMemLocObject() {
  //return dynamic_cast<MemLocObject*>(this);
  return getAOType() == MemLoc;
}

bool AbstractObject::isMappedAO()
{
/*  cout << "AbstractObject::isMappedAO() "<<endl;
  cout << str()<<endl;
  cout << "boost::dynamic_cast<MappedCodeLocObject*>(this)="  <<dynamic_cast<MappedCodeLocObject<ComposedAnalysis*>*>(this)  <<endl;
  cout << "boost::dynamic_cast<MappedValueObject*>(this)="    <<dynamic_cast<MappedValueObject<ComposedAnalysis*>*>(this)    <<endl;
  cout << "boost::dynamic_cast<MappedMemLocObject*>(this)="   <<dynamic_cast<MappedMemLocObject<ComposedAnalysis*>*>(this)   <<endl;
  cout << "boost::dynamic_cast<MappedMemRegionObject*>(this)="<<dynamic_cast<MappedMemRegionObject<ComposedAnalysis*>*>(this)<<endl;
  //return false;
  return dynamic_cast<MappedCodeLocObject<ComposedAnalysis*>*>(this) ||
         dynamic_cast<MappedValueObject<ComposedAnalysis*>*>(this)   ||
         dynamic_cast<MappedMemLocObject<ComposedAnalysis*>*>(this)  ||
         dynamic_cast<MappedMemRegionObject<ComposedAnalysis*>*>(this);*/
  return false;
}

/*
 // Simple equality test that just checks whether the two objects correspond to the same expression
 bool AbstractObject::mustEqualExpr(AbstractObjectPtr o, PartEdgePtr pedge)
 {
 // GREG: I'm not sure if this is actually valid since the same expression can denote different sets at different loop iterations

 // If both AbstractObjects have non-NULL bases, we can tell that they're must-equal by simply confirming
 // that their bases are equal
 if(base && o->base && base==o->base) {
 dbg << "AbstractObject::mustEqualExpr() base="<<SgNode2Str(base)<<" o->base="<<SgNode2Str(o->base)<<endl;
 return true;
 // Otherwise, we don't know and must answer conservatively
 } else return false;
 }*/

// General versions of equalSet() that accounts for framework details before routing the call to the 
// derived class' equalSet() check. Specifically, it routes the call through the composer to make 
// sure the equalSet() call gets the right PartEdge.
/*bool AbstractObject::equalSet(AbstractObjectPtr that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis)
 {
 / * GB 2013-09-16 - No longer checking for equality to FuncResultMemLocObject since the FuncResultMemRegionObject
 *                 checks for this correctly on its own. We may return this code if we discover that checking this
 *                 early before we propagate the query through the composer is an important performance gain.
 * // If either this or that are FuncResultMemLocObject, they denote the same set if they both are
 if(isMemLocObject() && that->isMemLocObject()) {
 MemLocObject*   thisML = dynamic_cast<MemLocObject*>(this);
 MemLocObjectPtr thatML = boost::dynamic_pointer_cast<MemLocObject*>(that);

 FuncResultMemRegionObject* frmlcoThis  = dynamic_cast<FuncResultMemLocObject*>(thisML->getRegion());
 FuncResultMemRegionObjectPtr frmlcoThat = boost::dynamic_pointer_cast<FuncResultMemLocObject>(thatML->getRegion());
 if(frmlcoThis) return frmlcoThat;
 else if(frmlcoThat) return false;

 } else if(isMemRegionObject() && that->isMemRegionObject()) {
 MemRegionObject*   thisMR = dynamic_cast<MemRegionObject*>(this);
 MemRegionObjectPtr thatMR = boost::dynamic_pointer_cast<MemRegionObject*>(that);

 // If either this or that are FuncResultMemLocObject, they denote the same set if they both are
 FuncResultMemRegionObject* frmlcoThis  = dynamic_cast<FuncResultMemLocObject*>(thisMR);
 FuncResultMemRegionObjectPtr frmlcoThat = boost::dynamic_pointer_cast<FuncResultMemLocObject>(thatMR);
 if(frmlcoThis) return frmlcoThat;
 else if(frmlcoThat) return false;
 }* /

 return comp->equalSet(shared_from_this(), that, pedge, analysis);
 }*/

// General versions of equalSet() that accounts for framework details before routing the call to the 
// derived class' subSet() check. Specifically, it routes the call through the composer to make 
// sure the subSet() call gets the right PartEdge.
/*bool AbstractObject::subSet(AbstractObjectPtr that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis)
 {
 / * GB 2013-09-16 - No longer checking for equality to FuncResultMemLocObject since the FuncResultMemRegionObject
 *                 checks for this correctly on its own. We may return this code if we discover that checking this
 *                 early before we propagate the query through the composer is an important performance gain.
 if(isMemLocObject() && that->isMemLocObject()) {
 // If either this or that are FuncResultMemLocObject, they denote the same set if they both are
 FuncResultMemLocObject* frmlcoThis  = dynamic_cast<FuncResultMemLocObject*>(this);
 FuncResultMemLocObjectPtr frmlcoThat = boost::dynamic_pointer_cast<FuncResultMemLocObject>(that);
 if(frmlcoThis) return frmlcoThat;
 else if(frmlcoThat) return false;
 }* /
 return comp->subSet(shared_from_this(), that, pedge, analysis);
 }*/

/*// General versions of isFull() and isEmpty that account for framework details before routing the call to the
 // derived class' isFull() and isEmpty()  check. Specifically, it routes the call through the composer to make
 // sure the isFull(PartEdgePtr) and isEmpty(PartEdgePtr) call gets the right PartEdge.
 bool AbstractObject::isFull(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis)
 { return comp->isFull(shared_from_this(), pedge, analysis); }

 bool AbstractObject::isEmpty(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis)
 { return comp->isEmpty(shared_from_this(), pedge, analysis); }*/

/*******************
 ***** hierKey *****
 *******************/

AbstractionHierarchy::hierKey::hierKey(bool endOfHierarchy) :
    endOfHierarchy(endOfHierarchy) {
}
AbstractionHierarchy::hierKey::hierKey(
    const std::list<comparablePtr>& keyList, bool endOfHierarchy) :
    keyList(keyList), endOfHierarchy(endOfHierarchy) {
}
AbstractionHierarchy::hierKey::hierKey(comparablePtr subKey,
    bool endOfHierarchy) :
    endOfHierarchy(endOfHierarchy) {
  keyList.push_back(subKey);
}
AbstractionHierarchy::hierKey::hierKey(hierKeyPtr that) {
  keyList = that->keyList;
  endOfHierarchy = that->endOfHierarchy;
}

std::list<comparablePtr>::const_iterator AbstractionHierarchy::hierKey::begin() {
  return keyList.begin();
}
std::list<comparablePtr>::const_iterator AbstractionHierarchy::hierKey::end() {
  return keyList.end();
}
void AbstractionHierarchy::hierKey::add(comparablePtr c) {
  if (!endOfHierarchy)
    keyList.push_back(c);
}
void AbstractionHierarchy::hierKey::add(
    std::list<comparablePtr>::const_iterator beginIt,
    std::list<comparablePtr>::const_iterator endIt) {
  if (!endOfHierarchy)
    keyList.insert(keyList.end(), beginIt, endIt);
}
const std::list<comparablePtr>& AbstractionHierarchy::hierKey::getList() {
  return keyList;
}

void AbstractionHierarchy::hierKey::reachedEndOfHierarchy() {
  endOfHierarchy = true;
}

bool AbstractionHierarchy::hierKey::equal(
    const comparable& that_arg) const {
  const hierKey& that = dynamic_cast<const hierKey&>(that_arg);
  if (keyList.size() != that.keyList.size())
    return false;

  list<comparablePtr>::const_iterator iThis = keyList.begin(), iThat =
      that.keyList.begin();
  for (; iThis != keyList.end(); ++iThis, ++iThat)
    if (*iThis != *iThat)
      return false;

  return true;
}

bool AbstractionHierarchy::hierKey::less(
    const comparable& that_arg) const {
  const hierKey& that = dynamic_cast<const hierKey&>(that_arg);
  if (keyList.size() != that.keyList.size())
    return keyList.size() < that.keyList.size();

  list<comparablePtr>::const_iterator iThis = keyList.begin(), iThat =
      that.keyList.begin();
  for (; iThis != keyList.end(); ++iThis, ++iThat) {
    if (*iThis < *iThat)
      return true;
    if (*iThis > *iThat)
      return false;
  }

  // this == that
  return false;
}

// Stringification of hierKeys
std::ostream& operator<<(std::ostream& s, AbstractionHierarchy::hierKey* k) {
  s << "[hierKey: " << k->getList() << ", endOfHierarchy="
      << k->endOfHierarchy << "]";
  return s;
}
std::ostream& operator<<(std::ostream& s, AbstractionHierarchy::hierKeyPtr k) {
  s << k.get();
  return s;
}

// IntersectHierKeys a lists of intersectComparables, each of which contains a list of comparablePtrs from
// different analyses.
// Equality checking is implemented as:
//    equal: if all comparable objects within an IntersectComparable says that the objects are equal
//    not equal: if any say they're not equal.
// Less checking:
//    - Iterate through the comparable objects within an IntersectComparable in fixed order
//    - Find the first that says they're less, use its less operator
// This will not lead to cycles in the partial order. Suppose our IntersectHierKey is built from key lists
// produced by two analyses, that impose the following partial order
//    Analysis 1: objA < objB = objC
//    Analysis 2: objA > objB > objC
// The above algorithm will induce the following order:
//                objA < objB > objC
// The order among objB and objC is irrelevant to analysis 1 and if they're oddly ordered, it
// must be consistent with its order because from its perspective objB=objC.

/******************************
 ***** IntersectComparable ****
 ******************************/

AbstractionHierarchy::IntersectComparable::IntersectComparable() {}
AbstractionHierarchy::IntersectComparable::IntersectComparable(const std::list<comparablePtr>& subComp) :
    subComp(subComp) {}

// Add a new comparablePtr to this intersection
void AbstractionHierarchy::IntersectComparable::add(comparablePtr newC)
{ subComp.push_back(newC); }

// This == That
bool AbstractionHierarchy::IntersectComparable::equal(const comparable& that_arg) const {
  const IntersectComparable& that = dynamic_cast<const IntersectComparable&>(that_arg);

  if(subComp.size() != that.subComp.size()) return false;

  list<comparablePtr>::const_iterator thisI=subComp.begin(),
                                      thatI=that.subComp.begin();
  for(; thisI!=subComp.end(); ++thisI, ++thatI)
    if(*thisI != *thatI) return false;

  return true;
}

// This < That
bool AbstractionHierarchy::IntersectComparable::less(const comparable& that_arg) const {
  const IntersectComparable& that = dynamic_cast<const IntersectComparable&>(that_arg);

  if(subComp.size() < that.subComp.size()) return true;
  if(subComp.size() > that.subComp.size()) return false;

  list<comparablePtr>::const_iterator thisI=subComp.begin(),
                                      thatI=that.subComp.begin();
  for(; thisI!=subComp.end(); ++thisI, ++thatI) {
    if(*thisI < *thatI) return true;
    if(*thisI > *thatI) return false;
  }

  // If we reach this point, this == that
  return false;
}

// String method
std::string AbstractionHierarchy::IntersectComparable::str(std::string indent) const {
  ostringstream s;

  if(subComp.size()==1) {
    s << "[IComp:"<<(*subComp.begin())->str(indent)<<"]";
    //return (*subComp.begin())->str(indent);
  } else {
    //s << "[IComp:"<<endl;
    s << "<table border=1>";
    for(list<comparablePtr>::const_iterator i=subComp.begin(); i!=subComp.end(); ++i) {
      s << "<tr><td>"<<(*i)->str(indent)<<"</td></tr>";//<<endl;
    }
    s << "</table>";
    //s << "]";
  }
  return s.str();
}

/************************************
 ***** IntersectMappedComparable ****
 ************************************/

// IntersectMappedComparable works just like IntersectComparable except that it allows for the
// possibility different analyses that contributed comparable objects to a full key, provided
// different numbers of keys. We assume that if a given IntersectMappedComparable doesn't have a
// comparable object from a given analysis, appropriate differentiating info was provided in earlier
// objects in the list and thus, at this point the remaining comparable objects (mapped to
// the appropriate analyses) are to be used for comparison.
// Given two IntersectMappedComparable objects with comparable objects mapped to different keys,
// the common keys are used for comparison.

template<typename Key>
AbstractionHierarchy::IntersectMappedComparable<Key>::IntersectMappedComparable() {}

template<typename Key>
AbstractionHierarchy::IntersectMappedComparable<Key>::IntersectMappedComparable(const std::map<Key, comparablePtr>& subComp) :
    subComp(subComp) {}

// Add a new comparablePtr to this intersection
template<typename Key>
void AbstractionHierarchy::IntersectMappedComparable<Key>::add(Key k, comparablePtr newC)
{ subComp[k] = newC; }

// This == That
template<typename Key>
bool AbstractionHierarchy::IntersectMappedComparable<Key>::equal(const comparable& that_arg) const {
  const IntersectMappedComparable<Key>& that = dynamic_cast<const IntersectMappedComparable<Key>&>(that_arg);

  int numSharedKeys=0;
  for(typename map<Key, comparablePtr>::const_iterator thisI=subComp.begin(); thisI!=subComp.end(); ++thisI) {
    typename map<Key, comparablePtr>::const_iterator thatI = that.subComp.find(thisI->first);
    if(thatI != that.subComp.end()) {
      if(thisI->second != thatI->second) return false;
      ++numSharedKeys;
    }
  }

  // If we couldn't find a difference between the two objects they're equal, OR
  if(numSharedKeys>0) return true;
  // If they don't share any keys, not sure what to do here since these objects denote unrelatable sets.
  // However, this should never happen
  else
    ROSE_ASSERT(0);
}

// This < That
template<typename Key>
bool AbstractionHierarchy::IntersectMappedComparable<Key>::less(const comparable& that_arg) const {
  const IntersectMappedComparable<Key>& that = dynamic_cast<const IntersectMappedComparable<Key>&>(that_arg);

  int numSharedKeys=0;
  for(typename map<Key, comparablePtr>::const_iterator thisI=subComp.begin(); thisI!=subComp.end(); ++thisI) {
    typename map<Key, comparablePtr>::const_iterator thatI = that.subComp.find(thisI->first);
    if(thatI != that.subComp.end()) {
      if(thisI->second < thatI->second) return true;
      if(thisI->second > thatI->second) return false;
      ++numSharedKeys;
    }
  }

  // If we couldn't find a key along which this object is different from that, they're equal, OR
  if(numSharedKeys>0) return false;
  // If they don't share any keys, not sure what to do here since these objects denote unrelatable sets.
  // However, this should never happen
  else
    ROSE_ASSERT(0);
}

// String method
template<typename Key>
std::string AbstractionHierarchy::IntersectMappedComparable<Key>::str(std::string indent) const {
  ostringstream s;

  if(subComp.size()==1) {
    s << "[IMComp:"<<subComp.begin()->second->str(indent)<<"]";
    //return (*subComp.begin())->str(indent);
  } else {
    //s << "[IComp:"<<endl;
    s << "<table border=1>";
    for(typename map<Key, comparablePtr>::const_iterator i=subComp.begin(); i!=subComp.end(); ++i) {
      s << "<tr><td>"<<i->first->str("")<<"</td><td>"<<i->second->str(indent)<<"</td></tr>";//<<endl;
    }
    s << "</table>";
    //s << "]";
  }
  return s.str();
}

/****************************
 ***** IntersectHierKey *****
 ****************************/

AbstractionHierarchy::IntersectHierKey::IntersectHierKey(const std::list<hierKeyPtr>& subKeys) : subKeys(subKeys) {
  vector<list<comparablePtr>::const_iterator> curSubKeyList;
  vector<list<comparablePtr>::const_iterator> endSubKeyList;

  scope s("AbstractionHierarchy::IntersectHierKey::IntersectHierKey");
  for (list<hierKeyPtr>::const_iterator cur = subKeys.begin(); cur != subKeys.end(); ++cur) {
    dbg << *cur << endl;
  }

  // Initialize curSubKeyList/endSubKeyList to point to the start/end of the keyLists of all the subKeys and
  // compute their maximum length
  unsigned int maxLength = 0;
  for (list<hierKeyPtr>::const_iterator cur = subKeys.begin();
      cur != subKeys.end(); ++cur) {
    curSubKeyList.push_back((*cur)->keyList.begin());
    endSubKeyList.push_back((*cur)->keyList.end());

    maxLength = (
        (*cur)->keyList.size() > maxLength ?
            (*cur)->keyList.size() : maxLength);
    //cout << "(*cur)->keyList.size()="<<(*cur)->keyList.size()<<", maxLength="<<maxLength<<endl;
  }

  // If all the subKeys are prefixes of each other, the intersection object is just equal
  // to the longest subKey that the others are prefixes of.
  // Otherwise, it is the empty set.

  // Iterate over all the subKeys and find the common hierarchy among them or detect
  // the difference among the hierarchies.
  for (unsigned int i = 0; i < maxLength; ++i) {
    dbg << i << ":" << endl;
    IntersectComparablePtr intersectComp = makePtr<IntersectComparable>();
    for (unsigned int j = 0; j < curSubKeyList.size(); ++j) {
      // If we haven't yet reached the end of the current subKey
      if (curSubKeyList[j] != endSubKeyList[j]) {
        /*cout << "    curSubKeyList[" << j << "]=" << curSubKeyList[j]->str()
            << ", intersectComp=" << intersectComp << endl;
        // If this is the first subKey for which we have the i-th entry, record its comparable object
        if (!intersectComp) {
          intersectComp = *curSubKeyList[j];
          //cout << "hi"<<endl;
        }
        // If it is not the first, make sure that they're equal. Otherwise, this object denotes the
        // empty set, which we currently don't handle.
        else {
          if (!(*intersectComp == **curSubKeyList[j]))
            ROSE_ASSERT(0);
        }*/
        /*// Add it to keyList
        keyList.push_back(*curSubKeyList[j]);*/

        intersectComp->add(*curSubKeyList[j]);
        dbg << "Pushing "<<(*curSubKeyList[j])->str()<<endl;

        // Advance the current iterator
        ++curSubKeyList[j];
      }
    }

    // Push the comparison object that is common to all the subKeys at this level
    // onto the intersection's keyList
    ROSE_ASSERT(intersectComp);
    keyList.push_back(intersectComp);
  }

  /*// endOfHierarchy is true if it is true for all the subKeys of length=maxLength
  endOfHierarchy = true;
  for (list<hierKeyPtr>::const_iterator cur = subKeys.begin();
      cur != subKeys.end(); ++cur) {
    if ((*cur)->keyList.size() == maxLength
        && (*cur)->endOfHierarchy == false)
      endOfHierarchy = false;
  }*/

  // endOfHierarchy is true if it is true for all of the subKeys
  endOfHierarchy = true;
  for (list<hierKeyPtr>::const_iterator cur = subKeys.begin(); cur != subKeys.end(); ++cur) {
    endOfHierarchy = endOfHierarchy && (*cur)->endOfHierarchy;
  }

  ostringstream oss; oss << "this="<<((hierKey*)this)<<endl;
  dbg << oss.str();
}

// Returns whether the set denoted by key is live at the given PartEdge
bool AbstractionHierarchy::IntersectHierKey::isLive(PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // An intersection is dead if any of the sub-objects is dead
  for (list<hierKeyPtr>::iterator cur = subKeys.begin(); cur != subKeys.end(); ++cur) {
    if (!(*cur)->isLive(pedge, comp, analysis))
      return false;
  }
  return true;
}

/**********************************
 ***** IntersectMappedHierKey *****
 **********************************/

template<class Key>
AbstractionHierarchy::IntersectMappedHierKey<Key>::IntersectMappedHierKey
                        (const std::map<Key, hierKeyPtr>& subKeys) : subKeys(subKeys) {
  // The key fields in the subKeys map
  vector<Key> KeyInSubKey;
  // The begin and end iterators to the hierKeys in the subKeys map
  vector<list<comparablePtr>::const_iterator> curSubKeyList;
  vector<list<comparablePtr>::const_iterator> endSubKeyList;

  scope s("AbstractionHierarchy::IntersectMappedHierKey::IntersectMappedHierKey");
  for (typename std::map<Key, hierKeyPtr>::const_iterator cur = subKeys.begin(); cur != subKeys.end(); ++cur) {
    dbg << cur->first->str() << ": "<<cur->second << endl;
  }

  // Initialize KeyInSubKey, curSubKeyList end SubKeyList to point to the start/end of the keyLists of all the
  // subKeys and compute their maximum length
  unsigned int maxLength = 0;
  for (typename std::map<Key, hierKeyPtr>::const_iterator cur = subKeys.begin();
      cur != subKeys.end(); ++cur) {
    KeyInSubKey.push_back(cur->first);
    curSubKeyList.push_back(cur->second->keyList.begin());
    endSubKeyList.push_back(cur->second->keyList.end());

    maxLength = (
        cur->second->keyList.size() > maxLength ?
            cur->second->keyList.size() : maxLength);
    //cout << "(*cur)->keyList.size()="<<(*cur)->keyList.size()<<", maxLength="<<maxLength<<endl;
  }

  // Iterate over all the subKeys and find the common hierarchy among them or detect
  // the difference among the hierarchies.
  for (unsigned int i = 0; i < maxLength; ++i) {
    dbg << i << ":" << endl;
    CompSharedPtr<IntersectMappedComparable<Key> > intersectComp = makePtr<IntersectMappedComparable<Key> >();
    for (unsigned int j = 0; j < curSubKeyList.size(); ++j) {
      // If we haven't yet reached the end of the current subKey
      if (curSubKeyList[j] != endSubKeyList[j]) {
        /*cout << "    curSubKeyList[" << j << "]=" << curSubKeyList[j]->str()
            << ", intersectComp=" << intersectComp << endl;
        // If this is the first subKey for which we have the i-th entry, record its comparable object
        if (!intersectComp) {
          intersectComp = *curSubKeyList[j];
          //cout << "hi"<<endl;
        }
        // If it is not the first, make sure that they're equal. Otherwise, this object denotes the
        // empty set, which we currently don't handle.
        else {
          if (!(*intersectComp == **curSubKeyList[j]))
            ROSE_ASSERT(0);
        }*/
        /*// Add it to keyList
        keyList.push_back(*curSubKeyList[j]);*/

        intersectComp->add(KeyInSubKey[j], *curSubKeyList[j]);
        dbg << "Pushing "<<(*curSubKeyList[j])->str()<<endl;

        // Advance the current iterator
        ++curSubKeyList[j];
      }
    }

    // Push the comparison object that is common to all the subKeys at this level
    // onto the intersection's keyList
    ROSE_ASSERT(intersectComp);
    keyList.push_back(intersectComp);
  }

  /*// endOfHierarchy is true if it is true for all the subKeys of length=maxLength
  endOfHierarchy = true;
  for (list<hierKeyPtr>::const_iterator cur = subKeys.begin();
      cur != subKeys.end(); ++cur) {
    if ((*cur)->keyList.size() == maxLength
        && (*cur)->endOfHierarchy == false)
      endOfHierarchy = false;
  }*/

  // endOfHierarchy is true if it is true for all of the subKeys
  endOfHierarchy = true;
  for(typename map<Key, hierKeyPtr>::const_iterator cur = subKeys.begin(); cur != subKeys.end(); ++cur) {
    endOfHierarchy = endOfHierarchy && cur->second->endOfHierarchy;
  }

  ostringstream oss; oss << "this="<<((hierKey*)this)<<endl;
  dbg << oss.str();
}

// Returns whether the set denoted by key is live at the given PartEdge
template<class Key>
bool AbstractionHierarchy::IntersectMappedHierKey<Key>::isLive(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  // An intersection is dead if any of the sub-objects is dead
  for(typename map<Key, hierKeyPtr>::const_iterator cur = subKeys.begin(); cur != subKeys.end(); ++cur) {
    if (!cur->second->isLive(pedge, comp, analysis))
      return false;
  }
  return true;
}

/***********************************
 ***** AbstractionHierarchy *****
 ***********************************/

AbstractionHierarchy::AOSHierKey::AOSHierKey(AbstractObjectPtr obj,
    bool endOfHierarchy) :
    hierKey(endOfHierarchy), obj(obj) {
}
//AOSHierKey(AbstractObjectPtr obj, const std::list<comparablePtr>& keyList, bool endOfHierarchy=false): hierKey(keyList, endOfHierarchy), obj(obj) {}
//AOSHierKey(AbstractObjectPtr obj, comparablePtr subKey, bool endOfHierarchy=false): hierKey(subKey, endOfHierarchy), obj(obj) { }
AbstractionHierarchy::AOSHierKey::AOSHierKey(AbstractObjectPtr obj,
    hierKeyPtr subHierKey) :
    hierKey(subHierKey), obj(obj) {
}

bool AbstractionHierarchy::AOSHierKey::isLive(PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
//  cout << "AbstractionHierarchy::AOSHierKey::isLive: "<<obj->isLive(pedge, comp, analysis)<<", obj="<<obj->str()<<endl;
  return obj->isLive(pedge, comp, analysis);
}

AbstractionHierarchy::hierRel AbstractionHierarchy::hierCompare(
    AbstractionHierarchyPtr left, AbstractionHierarchyPtr right) {
  const AbstractionHierarchy::hierKeyPtr& leftKey = left->getHierKey();
  const AbstractionHierarchy::hierKeyPtr& rightKey = right->getHierKey();
  std::list<comparablePtr>::const_iterator l = leftKey->begin(), r =
      rightKey->begin();
  for (; l != leftKey->end() && r != rightKey->end(); l++, r++) {
    if (*l != *r)
      return disjoint;
  }
  // If we've reached the end of the right key but not the left and the prefix of the
  // left key is equal to the entirety of the right key
  if (l != leftKey->end())
    return rightContains;
  // Inverse of above
  if (r != rightKey->end())
    return leftContains;

  // The two keys must be the same
  return equal;
}

/******************************************
 ***** AbstractObjectDisjointHierWrap *****
 ****************************************** /

 // Returns a key that uniquely identifies this particular AbstractObject in the
 // set hierarchy.
 const AbstractionHierarchy::hierKeyPtr& AbstractObjectDisjointHierWrap::getHierKey() const {
 if(!isHierKeyCached) {
 // Set key to refer to the keyCode, which is allocated on the spot to wrap the hashCode()
 // It is assumed that the hashCode doesn't change during an object's lifetime.
 ((AbstractObjectDisjointHierWrap*)this)->cachedHierKey = boost::make_shared<AOSHierKey>(shared_from_this(), makePtr<hashCodeWrapper>(getHashCode()));
 ((AbstractObjectDisjointHierWrap*)this)->isHierKeyCached = true;
 }
 return cachedHierKey;
 }
 */

/* ##########################
   ##### AbstractObject #####
   ########################## */

// It is often useful to create an Abstraction object that denotes the union or intersection
// of multiple other objects. There are multiple ways to do this:
// - We can create a copy of one of the objects to be unioned and call its meetUpdate() and
//   intersectUpdate() methods to union/intersect the others in. This is supported for all
//   Abstractions.
// - We can create an object that maps some keys to Abstractions and implements all relevant operations
//   by forwarding them to the Abstractions within it and returning the most (intersection) or least (union)
//   conservative answer. This only works for AbstractObjects via the MappedAbstractObject class.
//   It is made more challenging by the fact that the keys may be any type, which hidden from the users
//   of these objects.
// The functions below allow either of the above methods to be used to create unions and intersections.
// They take as argument a MAOMap, which maps some unknown keys to Abstractions. This is a good choice
// because it allows users to iterate over the mapped abstractions without knowing anything about the
// type of the keys, and because they can return MappedAbstractObjects that contain the Abstractions
// that are mapped inside of them MAOMap::getMappedObj().
AbstractionPtr AbstractObject::genUnion(MAOMapPtr maoMap) {
  // Create a MappedAbstractObject that denotes the union of the AbstractObjects in mapMap
  return maoMap->getMappedObj(Union);
}

AbstractionPtr AbstractObject::genIntersection(MAOMapPtr maoMap) {
  // Create a MappedAbstractObject that denotes the union of the AbstractObjects in mapMap
  return maoMap->getMappedObj(Intersection);
}

/* ################################
   ##### MappedAbstractObject #####
   ################################ */

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::
      MappedAbstractObject(uiType ui, int nFull, ComposedAnalysis* analysis) :
  AOSubType(NULL), MappedAbstractionBase(ui, nFull, analysis)
{
  initializedPOKey=false;
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::
      MappedAbstractObject(uiType ui, int nFull, ComposedAnalysis* analysis, const std::map<Key, boost::shared_ptr<AOSubType> >& aoMap) :
  AOSubType(NULL), MappedAbstractionBase(ui, nFull, analysis), aoMap(aoMap)
{
  initializedPOKey=false;
}


template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::
      MappedAbstractObject(const MappedAbstractObject& that) :
  AOSubType(that), MappedAbstractionBase(that.ui, that.nFull, that.analysis), aoMap(that.aoMap)
{
  initializedPOKey=false;
}


// Returns true if this is a MappedAbstractObject and false otherwise
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isMappedAO()
{
  //cout << "MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isMappedAO()"<<endl;
  return true; }

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
SgNode* MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::getBase() const {
  // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
  SgNode* base = NULL;
  for (typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator it = aoMap.begin(); it != aoMap.end(); ++it) {
    if (it == aoMap.begin())
      base = it->second->getBase();
    else if (base != it->second->getBase())
      return NULL;
  }

  return base;
}

/*  // Allocates a copy of this object and returns a pointer to it
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
AbstractObjectPtr MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::copyAO() const {
  return copyAOType();
}*/


// Functions that identify the type of AbstractObject this is. Should be over-ridden by derived
// classes to save the cost of a dynamic cast.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
AbstractObject::AOType MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::getAOType() const
{ return type; }

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::add(Key key, boost::shared_ptr<AOSubType> ao,
                                               PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  // If the object is already full don't add anything
  if (isUnion() && isFull(pedge, comp, analysis))
    return;

  // If the ao is not full add/update the map
  if (!ao->isFull(pedge, comp, analysis)) {
    aoMap[key] = ao;
  } else {
    nFull++;
    if (isUnion())
      setAOToFull();
  }
}

// Allocates a copy of this object and returns a pointer to it
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
boost::shared_ptr<AOSubType> MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::copyAOType() const {
  map<Key, boost::shared_ptr<AOSubType> > newAOMap;  
  for(typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator it = aoMap.begin(); it != aoMap.end(); ++it)
    newAOMap[it->first] = it->second->copyAOType();
  
  return boost::make_shared<MappedAOSubType >(ui, nFull, analysis, newAOMap);
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::mayEqualWithKey(Key key,
    const map<Key, boost::shared_ptr<AOSubType> >& thatAOMap, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator s_it = thatAOMap.find(key);
  if (s_it == thatAOMap.end())
    return true;
  return aoMap[key]->mayEqual(s_it->second, pedge, comp, analysis);
}

//! Two AO objects are may equals if there is atleast one execution or sub-exectuion
//! in which they represent the same code location.
//! Analyses are conservative as they start with full set of executions.
//! Dataflow facts (predicates) shrink the set of sub-executions.
//! We do not explicity store set of sub-executions and they are described 
//! by the abstract objects computed from dataflow fact exported by the analysis.
//! Unless the analyses discover otherwise, the conservative answer for mayEqualAO is true.
//! Mapped AOs are keyed using either ComposedAnalysis* or PartEdgePtr.
//! Each keyed AO object correspond to some dataflow facts computed by Key=Analysis* or 
//! computed at Key=PartEdgePtrthat describes some sets of executions.
//! MayEquality check on mapped AO is performed on intersection of sub-executions
//! or union of sub-executions over the keyed AO objects.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::mayEqualAO(boost::shared_ptr<AOSubType> thatAO, PartEdgePtr pedge)
{ return mayEqual(thatAO, pedge, analysis->getComposer(), analysis); }

// Returns whether this object may/must be equal to o within the given Part p
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::mayEqual(AbstractObjectPtr thatAO, PartEdgePtr pedge,
                                                    Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > thatAO_p =
      boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(thatAO);
  assert(thatAO_p);

  // This object denotes full set of AO (full set of executions)
  if (isFull(pedge, comp, analysis))
    return true;

  // denotes empty set
  if (isEmpty(pedge, comp, analysis))
    return false;

  // presence of one more full objects will result in full set over union
  if (isUnion() && nFull > 0)
    return true;

  // Two cases reach here [1] isUnion()=true && nFull_AO=0 [2] intersect=true && nFullAO=0 or nFull_AO!=0.
  // For both cases iterate on the AO map and discharge the mayEqualAO query to individual objects
  // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
  const map<Key, boost::shared_ptr<AOSubType> > thatAOMap = thatAO_p->getAOMap();
  typename map<Key, boost::shared_ptr<AOSubType> >::iterator it;
  for (it = aoMap.begin(); it != aoMap.end(); ++it) {
    // discharge query
    bool isMayEq = mayEqualWithKey(it->first, thatAOMap, pedge, comp, analysis);

    // 1. Union of sub-executions and the object does not contain any full objects.
    // If the discharged query comes back as true for this case then we have found atleast one execution
    // under which the two objects are same and the set can only grow and the result of this query is not going
    // to change due to union.
    // If false we iterate further as any AO can add more executions under which the objects are may equals.
    if (isUnion() && isMayEq == true)
      return true;

    // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
    // The sub-executions are intersected and therefore it does not matter if we have full objects.
    // If the discharged query returns false then return false.
    // We did not find one execution in which the two objects are may equals.
    // Note that set of executions are contained over keyed objects (analyses are conservative).
    // This set only shrinks during intersection and it is not going to affect the result of this query.
    // If it returns true iterate further as some executions corresponding to true may be dropped.
    else if (isIntersection() && isMayEq == false)
      return false;
  }

  // All the keyed objects returned false for the discharged query under union.
  // We haven't found a single execution under which the two objects are may equals.
  if (isUnion())
    return false;
  // All the keyed objects returned true for the discharged query under intersection.
  // We have atleast one execution in common in which the two objects are may equals.
  else if (isIntersection())
    return true;
  else
    assert(0);
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::mustEqualWithKey(Key key,
    const map<Key, boost::shared_ptr<AOSubType> >& thatAOMap, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator s_it = thatAOMap.find(key);
  if (s_it == thatAOMap.end())
    return false;
  return aoMap[key]->mustEqual(s_it->second, pedge, comp, analysis);
}

//! Two AO objects are must equals if they represent the same code
//! location on all executions.
//! Analyses are conservative as they start with full set of executions.
//! Dataflow facts (predicates) shrink the set of sub-executions.
//! We do not explicity store set of sub-executions and they are described
//! by the abstract objects computed from dataflow fact exported by the analysis.
//! Unless the analyses discover otherwise conservative answer for mustEqualAO is false.
//! Mapped AOs are keyed using either ComposedAnalysis* or PartEdgePtr.
//! Each keyed AO object correspond to some dataflow facts computed by Key=Analysis* or
//! computed at Key=PartEdgePtrthat describes some sets of executions.
//! MustEquality check on mapped AO is performed on intersection (mostAccurate=true) of sub-executions
//! or union (mostAccurate=false) of sub-executions over the keyed AO objects.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::mustEqualAO(boost::shared_ptr<AOSubType> thatAO, PartEdgePtr pedge)
{ return mustEqual(thatAO, pedge, analysis->getComposer(), analysis); }

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::mustEqual(AbstractObjectPtr thatAO, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > thatAO_p =
      boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(thatAO);
  assert(thatAO_p);

  // This object denotes full set of AO (full set of executions)
  if (isFull(pedge, comp, analysis))
    return false;

  // denotes empty set
  if (isEmpty(pedge, comp, analysis))
    return false;

  // presence of one more full objects will result in full set over union
  if (isUnion() && nFull > 0)
    return true;

  // Two cases reach here [1] isUnion()=true && nFull_AO=0 [2] intersect=true && nFullAO=0 or nFull_AO!=0.
  // For both cases iterate on the AO map and discharge the mayEqualAO query to individual objects
  // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
  const map<Key, boost::shared_ptr<AOSubType> > thatAOMap = thatAO_p->getAOMap();
  typename map<Key, boost::shared_ptr<AOSubType> >::iterator it;
  for (it = aoMap.begin(); it != aoMap.end(); ++it) {
    // discharge query
    bool isMustEq = mustEqualWithKey(it->first, thatAOMap, pedge, comp, analysis);

    // 1. Union of sub-executions and the object does not contain any full objects
    // If the discharged query comes back as false for this case then we have found atleast one execution
    // under which the two objects are not same and the set can only grow and the result of this query is not going
    // to change due to union.
    // If it returns true we iterate further as any AO can add more executions under which the objects are not must equals.
    if (isUnion() && isMustEq == false)
      return false;

    // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
    // The sub-executions are intersected and therefore it does not matter if we have full objects.
    // If the discharged query returns true then return true.
    // Under all sub-executions (corresponding to the AO) the two objects must equal.
    // Note that set of executions are contained over keyed objects as the analyses are conservative.
    // This set only shrinks during intersection and it is not going to affect the result of this query.
    // If it returns false iterate further as some executions corresponding to false may be dropped.
    else if (isIntersection() && isMustEq == true)
      return true;
  }

  // All the keyed objects returned true for the discharged query under union.
  // We haven't found a single execution under which the two objects are not equal.
  if (isUnion())
    return true;
  // All the keyed objects returned false for the discharged query under intersection.
  // We have atleast one execution in common in which the two objects are not equal.
  else if (isIntersection())
    return false;
  else
    assert(0);
}

//! Discharge the query to the corresponding AO
//! If key not found in thatAOMap return false
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::equalSetWithKey(Key key,
    const map<Key, boost::shared_ptr<AOSubType> >& thatAOMap, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator s_it = thatAOMap.find(key);
  if (s_it == thatAOMap.end())
    return false;
  return aoMap[key]->equalSet(s_it->second, pedge, comp, analysis);
}

//! Two objects are equal sets if they denote the same set of memory locations
//! The boolean parameter mostAccurate is not releveant as this query is not
//! answered based on union or intersection of sub-executions.
//! Simply discharge the queries to all keyed CodeLoc objects
//! If all the discharged queries come back equal then the two objects are equal otherwise not.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::equalSetAO(boost::shared_ptr<AOSubType> thatAO, PartEdgePtr pedge)
{ return equalSet(thatAO, pedge, analysis->getComposer(), analysis); }

// general versions of equalset() that accounts for framework details before routing the call to the
// derived class' equalset() check. specifically, it routes the call through the composer to make
// sure the equalset() call gets the right partedge.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::equalSet(AbstractObjectPtr thatAO, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > thatAO_p =
      boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(thatAO);
  assert(thatAO_p);

  // This object denotes full set of AO (full set of executions)
  if (isFull(pedge, comp, analysis))
    return thatAO_p->isFull(pedge, comp, analysis);

  // denotes empty set
  if (isEmpty(pedge, comp, analysis))
    return thatAO_p->isEmpty(pedge, comp, analysis);

  const map<Key, boost::shared_ptr<AOSubType> > thatAOMap = thatAO_p->getAOMap();
  typename map<Key, boost::shared_ptr<AOSubType> >::iterator it;
  for (it = aoMap.begin(); it != aoMap.end(); ++it) {
    // discharge query
    // break even if one of them returns false
    if (equalSetWithKey(it->first, thatAOMap, pedge, comp, analysis) == false)
      return false;
  }

  return true;
}

//! Discharge the query to the corresponding AO
//! If key not found in thatAOMap return true as the
//! keyed object on thatAOMap denotes full set
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::subSetWithKey(Key key,
    const map<Key, boost::shared_ptr<AOSubType> >& thatAOMap, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator s_it = thatAOMap.find(key);
  if (s_it == thatAOMap.end())
    return true;
  return aoMap[key]->equalSet(s_it->second, pedge, comp, analysis);
}

//! This object is a non-strict subset of the other if the set of memory locations denoted by this
//! is a subset of the set of memory locations denoted by that.
//! The boolean parameter mostAccurate is not releveant as this query is not
//! answered based on union or intersection of sub-executions.
//! Simply discharge the queries to all keyed CodeLoc objects
//! If all the discharged queries come back true then this is a subset of that otherwise not.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::subSetAO(boost::shared_ptr<AOSubType> thatAO, PartEdgePtr pedge)
{ return subSet(thatAO, pedge, analysis->getComposer(), analysis); }

// General versions of subSet() that accounts for framework details before routing the call to the
// derived class' subSet() check. Specifically, it routes the call through the composer to make
// sure the subSet() call gets the right PartEdge.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::subSet(AbstractObjectPtr thatAO, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > thatAO_p =
      boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(thatAO);
  assert(thatAO_p);

  // This object denotes full set of AO (full set of executions)
  if (isFull(pedge, comp, analysis))
    return thatAO_p->isFull(pedge, comp, analysis);

  // denotes empty set
  // thatAO could be empty or non-empty eitherway this will be a non-strict subset of that.
  if (isEmpty(pedge, comp, analysis))
    return true;

  // If both objects have the same keys discharge
  // If this object has a key and that does not then
  // the keyed object is subset of that (return true) implemented by subsetAOWithKey
  // If any of the discharged query return false then return false.
  const map<Key, boost::shared_ptr<AOSubType> > thatAOMap = thatAO_p->getAOMap();
  typename map<Key, boost::shared_ptr<AOSubType> >::iterator it;
  for (it = aoMap.begin(); it != aoMap.end(); ++it) {
    // discharge query
    // break even if one of them returns false
    if (subSetWithKey(it->first, thatAOMap, pedge, comp, analysis) == false)
      return false;
  }

  // If this object doesn't have the key and that object has the key then
  // return false as this object has full object mapped to the key
  typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator c_it;
  for (c_it = thatAOMap.begin(); c_it != thatAOMap.end() && (nFull != 0);
      ++c_it) {
    if (aoMap.find(c_it->first) == aoMap.end())
      return false;
  }

  return true;
}

//! Mapped object liveness is determined based on finding executions
//! in which it may be live.
//! It can be answered based on union (mostAccurate=false) or intersection
//! (mostAccurate=true) of executions
//! The conservative answer is to assume that the object is live
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isLiveAO(PartEdgePtr pedge)
{ return isLive(pedge, analysis->getComposer(), analysis); }

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool  MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isLive(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  // If this object is full return the conservative answer
  if (isFull(pedge, comp, analysis))
    return true;

  // If it has one or more full objects added to it
  // and if the object has mostAccurate=false then return true (weakest answer)
  if (nFull > 0 && isUnion())
    return true;

  // 1. This object may have have one or more full objects under intersection
  // 2. This object doesnt have any full objects added to it under union
  // Under both cases the answer is based on how individual analysis respond to the query
  typename map<Key, boost::shared_ptr<AOSubType> >::iterator it = aoMap.begin();
  for (; it != aoMap.end(); ++it) {
    bool isLive = it->second->isLive(pedge, comp, analysis);
    if (isUnion() && isLive == true)
      return true;
    else if (isIntersection() && isLive == false)
      return false;
  }

  // leftover cases
  if (isUnion())
    return false;
  else if (isIntersection())
    return true;
  else
    assert(0);
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::meetUpdateAO(boost::shared_ptr<AOSubType> thatAO, PartEdgePtr pedge)
{ return meetUpdate(thatAO, pedge, analysis->getComposer(), analysis); }

//! meetUpdateAO performs the join operation of abstractions of two mls
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::meetUpdate(AbstractObjectPtr that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > thatAO_p =
      boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(
          that);
  assert(thatAO_p);

  // if this object is already full
  if (isFull(pedge, comp, analysis))
    return false;

  // If that object is full set this object to full
  if (thatAO_p->isFull(pedge, comp, analysis)) {
    nFull++;
    setAOToFull();
    return true;
  }

  // Both objects are not full
  const map<Key, boost::shared_ptr<AOSubType> > thatAOMap = thatAO_p->getAOMap();

  typename map<Key, boost::shared_ptr<AOSubType> >::iterator it = aoMap.begin();
  typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator s_it; // search iterator for thatAOMap

  bool modified = false;
  while (it != aoMap.end()) {
    s_it = thatAOMap.find(it->first);
    // If two objects have the same key then discharge meetUpdate to the corresponding keyed AO objects
    if (s_it != thatAOMap.end()) {
      modified = (it->second)->meetUpdate(s_it->second, pedge, comp, analysis) || modified;
    }

    // Remove the current AO object (current iterator it) from the map if the mapepd object is full.
    // Two cases under which the current AO object can be full.
    // (1) If current key is not found in thatAOMap then the mapped object
    // in thatAOMap is full and the meetUpdate of the current AO with that is also full.
    // (2) meetUpdateAO above of the two keyed objects resulted in this mapped object being full.
    // Under both cases remove the mapped ml from this map
    if (s_it == thatAOMap.end() || (it->second)->isFull(pedge, comp, analysis)) {
      // Current mapped AO has become full as a result of (1) or (2).
      // Remove the item from the map.
      // Note that post-increment which increments the iterator and returns the old value for deletion.
      aoMap.erase(it++);
      nFull++;
      modified = true;

      // If mostAccurate=false then set this entire object to full and return
      if (isUnion()) {
        setAOToFull();
        return true;
      }
    } else
      ++it;
  }
  return modified;
}

//! Method that sets this mapped object to full
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::setAOToFull() {
  assert(nFull > 0);
  if (aoMap.size() > 0)
    aoMap.clear();
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isFullAO(PartEdgePtr pedge)
{ return isFull(pedge, analysis->getComposer(), analysis); }

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isFull(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  if (nFull > 0 && aoMap.size() == 0)
    return true;
  return false;
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isEmptyAO(PartEdgePtr pedge)
{ return isEmpty(pedge, analysis->getComposer(), analysis); }

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isEmpty(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  if (nFull == 0 && aoMap.size() == 0)
    return true;
  return false;
}
  // Returns the type of the concrete value (if there is one)
  SgType* getConcreteType();

  // Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
  // the normal ROSE mechanisms to decode it
  std::set<boost::shared_ptr<SgValueExp> > getConcreteValue();
 
// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isConcrete() {
  typename map<Key, boost::shared_ptr<AOSubType> >::iterator it;
  for (it = aoMap.begin(); it != aoMap.end(); ++it)
    if (!it->second->isConcrete())
      return false;
  return true;
}

// Returns the number of concrete values in this set
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
int MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::concreteSetSize() {
  // This is an over-approximation of the set size that assumes that all the concrete sets of
  // the sub-CodeLocs are disjoint
  int size = 0;
  typename map<Key, boost::shared_ptr<AOSubType> >::iterator it;
  for (it = aoMap.begin(); it != aoMap.end(); ++it)
    size += it->second->concreteSetSize();
  return size;
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
string MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::str(string indent) const {
  ostringstream oss;
  oss << "<table border=\"1\">";
  oss << "<tr>";
  oss << "<th>"
      << (isUnion() ?
          "UnionMappedAO" : "IntersectMappedAO:")
      << "</th>";
  if (nFull > 0 && aoMap.size() == 0)
    oss << "<th> Full </th> </tr>";
  else if (nFull == 0 && aoMap.size() == 0)
    oss << "<th> Empty </th> </tr>";
  else {
    oss << "</tr>";
    typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator it =
        aoMap.begin();
    for (; it != aoMap.end(); ++it) {
      oss << "<tr>";
      oss << "<td>" << (it->first)->str(indent) << "</td>";
      oss << "<td>" << (it->second)->str(indent) << "</td>";
      oss << "</tr>";
    }
  }
  oss << "</table>";
  return oss.str();
}

// Returns whether all instances of this class form a hierarchy. Every instance of the same
// class created by the same analysis must return the same value from this method!
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isHierarchy() const {
//  cout << "MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::isHierarchy()"<<endl;
  //return false;
  if(aoMap.size()==1 && aoMap.begin()->second->isHierarchy()) return true;
  else return false;
/*  // Combined CodeLocs form hierarchy if:
  // - All the sub-CodeLocs form hierarchies of their own, AND
  // - The combination is an intersection.
  //   If the combination is a union then consider the following:
  //            MLa     MLb
  //   comb1 = {a,b}, {w, x}
  //   comb2 = {a,b}, {y, z}
  //   Note that MLs from analyses a and b are either identical sets or disjoint
  //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
  //   not hold unless we work out a new hierarchy for MLb.

  // Unions are not hierarchical unless they're singletons
  if (isUnion()) {
    if (aoMap.size() == 1)
      return aoMap.begin()->second->isHierarchy();
    else
      return false;
  }

  typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator it;
  for (it = aoMap.begin(); it != aoMap.end(); ++it)
    if (!it->second->isHierarchy())
      return false;
  return true;*/
}

// Returns whether all instances of all the AbstractObjects within this MAO form a hierachy
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::membersIsHierarchy() const {
  typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator it;
  for (it = aoMap.begin(); it != aoMap.end(); ++it)
    if (!it->second->isHierarchy())
      return false;
  return true;
}

// Returns a key that uniquely identifies this particular AbstractObject in the
// set hierarchy.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
const AbstractionHierarchy::hierKeyPtr& MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::getHierKey() const {
  if(aoMap.size()==1 && aoMap.begin()->second->isHierarchy()) return aoMap.begin()->second->getHierKey();
  else assert(0);
}
//  // The intersection of multiple objects is just a filtering process from the full
//  // set of objects to the exact one, with each key in the hierarchy further filtering
//  // the set down. As such, a hierarchical key for the intersection of multiple objects
//  // is just the concatenation of the keys of all the individual objects.
//  if (!AOSubType::isHierKeyCached) {
//    /*((MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>*) this)->cachedHierKey =
//     boost::make_shared<AOSHierKey>(
//     ((MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>*) this)->shared_from_this());
//
//     typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator it;
//     for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//     AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<
//     AbstractionHierarchy>(it->second);
//     ROSE_ASSERT(hierIt);
//
//     ((MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>*) this)->cachedHierKey->add(
//     hierIt->getHierKey()->begin(), hierIt->getHierKey()->end());
//     }*/
//
//    map<Key, typename AOSubType::hierKeyPtr> subHierKeys;
//    for(typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator it = aoMap.begin();
//        it != aoMap.end(); ++it) {
//      AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(it->second);
//      ROSE_ASSERT(hierIt);
//      subHierKeys[it->first] = hierIt->getHierKey();
//    }
//
//    ((MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>*) this)->cachedHierKey =
//        boost::make_shared<AbstractionHierarchy::IntersectMappedHierKey<Key> >(subHierKeys);
//
//    ((MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>*) this)->AOSubType::isHierKeyCached = true;
//  }
//  return AOSubType::cachedHierKey;
//}

// Returns whether sets of the given type form a partial order
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::()
{ return membersIsHierarchy(); }

// If isPartialOrder() returns true, classes must implement the methods below to establish
// the partial order relationship among class instances.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::MappedPartialOrderKey::isPOLessThan(const PartialOrderKey& that_arg)
{
  try{
    const MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::MappedPartialOrderKey& that =
        dynamic_cast<const MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::MappedPartialOrderKey& >(that_arg);
    return key < that.key;
  } catch(std::bad_cast c)
  { assert(0); }
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
bool MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::MappedPartialOrderKey::isPOEqual(const PartialOrderKey& that_arg)
{
  try{
    const MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::MappedPartialOrderKey& that =
        dynamic_cast<const MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::MappedPartialOrderKey& >(that_arg);
    return key == that.key;
  } catch(std::bad_cast c)
  { assert(0); }
}

// Computes the partial order key that describes this object and stores it in partialOrderKey
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::computePartialOrderKey() {
  // The intersection of multiple objects is just a filtering process from the full
  // set of objects to the exact one, with each key in the hierarchy further filtering
  // the set down. As such, a hierarchical key for the intersection of multiple objects
  // is just the concatenation of the keys of all the individual objects.
  if(!initializedPOKey) {
    assert(isPartialOrder());

    partialOrderKey = boost::make_shared<MappedPartialOrderKey>();
    for(typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator it = aoMap.begin();
        it != aoMap.end(); ++it) {
      AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(it->second);
      ROSE_ASSERT(hierIt);
      partialOrderKey->key[it->first] = hierIt->getHierKey();
    }
    initializedPOKey=true;
  }
}

// Returns the PartialOrderKey object that defines this Abstraction's location within its partial order
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
PartialOrderKeyPtr MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::getPOKey() {
  computePartialOrderKey();
  return partialOrderKey;
}

/* ################################################
   ##### MappedAbstractObject::ConcreteMAOMap #####
   ################################################ */

// Creates an empty map with the same keys as the given MappedAbstractObject
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::ConcreteMAOMap
    (boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > parent): parent(parent) {
  // Initialize data to map the keys in the parent MappedAO to NULL pointers
  for(typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator aoIter = parent->aoMap.begin();
      aoIter!=parent->aoMap.end(); ++aoIter)
  data[aoIter->first] = boost::shared_ptr<void>();
}

// Creates a new map based on the given map:
// If emptyMap==true: the new map will be created empty but initialized with the keys of the given map
// Otherwise: the new map will be a shallow copy of the given map (the copy methods of the keys and values are not called)
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::ConcreteMAOMap
    (const ConcreteMAOMap& that, bool emptyMap) {
  for(typename map<Key, boost::shared_ptr<void> >::const_iterator dIter = that.data.begin(); dIter != that.data.end(); ++dIter)
    data[dIter->first] = (emptyMap? boost::shared_ptr<void>():
                                    dIter->second);
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::set(AbstractionPtr mappedAO_arg, setMapFunc& f) {
  boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > mappedAO = boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(mappedAO_arg);
  assert(mappedAO);

  for(typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator aoIter = mappedAO->aoMap.begin();
      aoIter != mappedAO->aoMap.end(); ++aoIter) {
    typename map<Key, boost::shared_ptr<void> >::iterator dIter = data.find(aoIter->first);
    if(dIter != data.end()) dIter->second = f(aoIter->second, dIter->second, true);
    else                    data[aoIter->first] = f(aoIter->second, boost::shared_ptr<void>(), false);
  }
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::get(AbstractionPtr mappedAO_arg, getMapFunc& f)  {
  boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > mappedAO = boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(mappedAO_arg);
  assert(mappedAO);

  for(typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator aoIter = mappedAO->aoMap.begin();
      aoIter != mappedAO->aoMap.end(); ++aoIter) {
    typename map<Key, boost::shared_ptr<void> >::iterator dIter = data.find(aoIter->first);
    if(dIter != data.end()) f(aoIter->second, dIter->second, true);
    else                    f(aoIter->second, boost::shared_ptr<void>(), false);
  }
}

// Applies the given functor to all the keys in this map
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::applyStr(applyStrMapFunc& f)  {
  for(typename map<Key, boost::shared_ptr<void> >::iterator dIter = data.begin(); dIter != data.end(); ++dIter) {
    f(dIter->first->str(), dIter->second);
  }
}

// Applies the given functor to all the keys in this map
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::apply(applyMapFunc& f)  {
  for(typename map<Key, boost::shared_ptr<void> >::iterator dIter = data.begin(); dIter != data.end(); ++dIter) {
    f(dIter->second);
  }
}

// Applies the given functor to all the keys in this map and sets the value returned by each invocation
// of the function to the key that corresponds to the call
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::setApply(setApplyMapFunc& f) {
  for(typename map<Key, boost::shared_ptr<void> >::iterator dIter = data.begin(); dIter != data.end(); ++dIter) {
      dIter->second = f(dIter->second);
    }
}

// Applies f to the keys shared by this map and that map
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::applyJoin(MAOMapPtr that_arg, applyJoinMapFunc& f) {
  boost::shared_ptr<ConcreteMAOMap> that = boost::dynamic_pointer_cast<ConcreteMAOMap>(that_arg);
  assert(that);

  for(typename map<Key, boost::shared_ptr<void> >::iterator dThisIter = data.begin(); dThisIter != data.end(); ++dThisIter) {
    typename map<Key, boost::shared_ptr<void> >::const_iterator dThatIter = that->data.find(dThisIter->first);
    if(dThatIter != that->data.end())
      f(dThisIter->second, dThatIter->second);
  }
}

// Applies f to the keys shared by this map and that map. Assigns the return value of each function
// call to the key for which the function was called
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::setJoin(MAOMapPtr that_arg, setJoinMapFunc& f) {
  boost::shared_ptr<ConcreteMAOMap> that = boost::dynamic_pointer_cast<ConcreteMAOMap>(that_arg);
  assert(that);

  for(typename map<Key, boost::shared_ptr<void> >::iterator dThisIter = data.begin(); dThisIter != data.end(); ++dThisIter) {
    typename map<Key, boost::shared_ptr<void> >::const_iterator dThatIter = that->data.find(dThisIter->first);
    if(dThatIter != that->data.end())
      dThisIter->second = f(dThisIter->second, dThatIter->second);
  }
}

// Applies f to the keys shared by maps this map and all the objects in objs.
// Assigns the return value of each function call to the key for which the function was called
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::setObjVecJoin
                 (const std::vector<AbstractionPtr>& objs, setMapObjVecJoinMapFunc& f) {
  // Convert objs, which is a set of generic Abstractions into a vector of MappedAbstractObjects and store it in mappedAOs
  scope s("MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::setObjVecJoin");
  vector<boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > > mappedAOs;
  for(vector<AbstractionPtr>::const_iterator o=objs.begin(); o!=objs.end(); ++o) {
    dbg << "o="<<(*o? (*o)->str():"NULL")<<endl;
    if(*o) {
      boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > mappedAO =
          boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(*o);
      //assert(mappedAO);
      if(mappedAO) mappedAOs.push_back(mappedAO);
      else mappedAOs.push_back(boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >());
    // If this object is NULL, add a NULL value for it
    } else
      mappedAOs.push_back(boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >());
  }

  // Iterate over all the keys in this map
  for(typename map<Key, boost::shared_ptr<void> >::iterator dThisIter = data.begin(); dThisIter != data.end(); ++dThisIter) {
    // Find the current key of this map in all the objects in mappedAOs.
    // Store the values mapped in those objects at the current key in subAOs.
    vector<AbstractObjectPtr> subAOs;
    unsigned int i=0;
    for(typename vector<typename boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > >::iterator o=mappedAOs.begin();
        o!=mappedAOs.end(); ++o, ++i) {
      if(*o) {
        typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator i=(*o)->aoMap.find(dThisIter->first);
        // If the current key is not found in the current object, skip this key
        if(i==(*o)->aoMap.end()) break;
        subAOs.push_back(i->second);
      // If this object is NULL, add a NULL value for this key
      } else {
        //subAOs.push_back(NULLAbstractObject);
        subAOs.push_back(boost::dynamic_pointer_cast<AbstractObject>(objs[i]));
      }
    }

    // If all the objects had the current key mapped, run the function
    if(subAOs.size() == objs.size())
      dThisIter->second = f(dThisIter->second, subAOs);
  }
}

// Applies f to the keys shared by maps this map, obj and thatMap. Assigns the return value of each function
// call to the key for which the function was called
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::setMapObjMapJoin
      (AbstractionPtr mappedAO_arg, MAOMapPtr thatMap_arg, setMapObjMapJoinMapFunc& f) {
  boost::shared_ptr<ConcreteMAOMap> thatMap = boost::dynamic_pointer_cast<ConcreteMAOMap>(thatMap_arg);
  assert(thatMap);

  boost::shared_ptr<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> > mappedAO = boost::dynamic_pointer_cast<MappedAbstractObject<Key, AOSubType, type, MappedAOSubType> >(mappedAO_arg);
  assert(mappedAO);

  for(typename map<Key, boost::shared_ptr<void> >::iterator dThisIter = data.begin(); dThisIter != data.end(); ++dThisIter) {
    typename map<Key, boost::shared_ptr<AOSubType> >::const_iterator aoIter = mappedAO->aoMap.find(dThisIter->first);
    typename map<Key, boost::shared_ptr<void> >::const_iterator dThatIter = thatMap->data.find(dThisIter->first);
    if(aoIter != mappedAO->aoMap.end() && dThatIter != thatMap->data.end())
      dThisIter->second = f(dThisIter->second, aoIter->second, dThatIter->second);
  }
}

// Applies f to the keys shared by maps this map and the other two maps. Assigns the return value of each function
// call to the key for which the function was called
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
void MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::set3MapJoin
      (MAOMapPtr thatMap1_arg, MAOMapPtr thatMap2_arg, set3MapJoinMapFunc& f) {
  boost::shared_ptr<ConcreteMAOMap> thatMap1 = boost::dynamic_pointer_cast<ConcreteMAOMap>(thatMap1_arg);
  assert(thatMap1);
  boost::shared_ptr<ConcreteMAOMap> thatMap2 = boost::dynamic_pointer_cast<ConcreteMAOMap>(thatMap2_arg);
  assert(thatMap2);

  for(typename map<Key, boost::shared_ptr<void> >::iterator dThisIter = data.begin(); dThisIter != data.end(); ++dThisIter) {
    typename map<Key, boost::shared_ptr<void> >::const_iterator dThatIter1 = thatMap1->data.find(dThisIter->first);
    typename map<Key, boost::shared_ptr<void> >::const_iterator dThatIter2 = thatMap2->data.find(dThisIter->first);
    if(dThatIter1 != thatMap1->data.end() && dThatIter2 != thatMap2->data.end())
      dThisIter->second = f(dThisIter->second, dThatIter1->second, dThatIter2->second);
  }
}

// Returns a freshly-allocated empty instance of this map type that maps
// the keys of this map to void pointers
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
MAOMapPtr MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::create() const
{ return boost::make_shared<ConcreteMAOMap>(*this, /* emptyMap */ true); }

// Returns a freshly-allocated copy of this map that maps the same keys to the same values as the originl
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
MAOMapPtr MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::copy() const
{ return boost::make_shared<ConcreteMAOMap>(*this, /* emptyMap */ false); }

// Returns an instance of a Mapped Abstraction that corresponds to the
// values and keys within this map. It is assumed by this call that the values
// in this map are a sub-type of Abstraction that is compatible with the map's
// implementation. Concretely, maps focused on AbstractObjects will have
// AbstractObject values and maps focused on Lattices will have Lattice values.
template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
AbstractionPtr MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::getMappedObj(uiType ui) {
  scope s("MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::ConcreteMAOMap::getMappedObj(uiType ui)");
  // Convert data, which maps Keys to void pointers into a map where AOSubType pointers are values
  map<Key, boost::shared_ptr<AOSubType> > aoMap;
  for(typename map<Key, boost::shared_ptr<void> >::iterator dIter=data.begin(); dIter!=data.end(); ++dIter) {
    boost::shared_ptr<AOSubType> val = boost::static_pointer_cast<AOSubType>(dIter->second);
    assert(val);
    aoMap[dIter->first] = val;
  }

  dbg << "#aoMap="<<aoMap.size()<<endl;
  return boost::make_shared<MappedAOSubType>(ui, 0, parent->analysis, aoMap);
}

template<class Key, class AOSubType, AbstractObject::AOType type, class MappedAOSubType>
MAOMapPtr MappedAbstractObject<Key, AOSubType, type, MappedAOSubType>::genMappedAOMap()
{ return boost::make_shared<ConcreteMAOMap>(shared_from_this()); }

/* #########################
   ##### CodeLocObject #####
   ######################### */

CodeLocObjectPtr NULLCodeLocObject;

// Returns whether this object may/must be equal to o within the given Part p
// These methods are called by composers and should not be called by analyses.
bool CodeLocObject::mayEqualAO(CodeLocObjectPtr that, PartEdgePtr pedge) {
  // If either object denotes the set of all parts, they're may-equal
  if (part == NULLPart || that->part == NULLPart)
    return true;

  // If the two objects denote different parts, they're not may-equal
  if (part != that->part)
    return false;

  // If either object denotes the set of all CFGNodes within the same part, they're may-equal
  if (cfgNode.getNode() == NULL || that->cfgNode.getNode() == NULL)
    return true;

  // If the two objects denote different CFGNodes within the same part, they're not equal
  if (cfgNode != that->cfgNode)
    return false;

  // The two objects denote the same part and CFGNode within it
  return true;
}

bool CodeLocObject::mustEqualAO(CodeLocObjectPtr that, PartEdgePtr pedge) {
  // The two objects are must-equal if they denote the same concrete part and CFGNode within it
  return part != NULLPart && that->part != NULLPart && part == that->part
      && cfgNode.getNode() != NULL && that->cfgNode.getNode() != NULL
      && cfgNode == that->cfgNode;
}

// General version of mayEqual and mustEqual that implements may/must equality with respect to ExprObj
// and uses the derived class' may/mustEqual check for all the other cases
// GREG: Currently nothing interesting here since we don't support ExprObjs for CodeLocObjects
bool CodeLocObject::mayEqual(CodeLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  return mayEqualAO(that, pedge);
}

bool CodeLocObject::mustEqual(CodeLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  return mustEqualAO(that, pedge);
}

bool CodeLocObject::mayEqual(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  CodeLocObjectPtr co = boost::dynamic_pointer_cast<CodeLocObject>(o);
  if (co)
    return mayEqual(co, pedge, comp, analysis);
  else
    return false;
}

bool CodeLocObject::mustEqual(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  //if(AbstractObject::mustEqualExpr(o, pedge)) return true;

  CodeLocObjectPtr co = boost::dynamic_pointer_cast<CodeLocObject>(o);
  if (co)
    return mustEqual(co, pedge, comp, analysis);
  else
    return false;
}

// Returns whether the two abstract objects denote the same set of concrete objects
// These methods are called by composers and should not be called by analyses.
bool CodeLocObject::equalSetAO(CodeLocObjectPtr that, PartEdgePtr pedge) {
  return mayEqualAO(that, pedge);
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
// These methods are called by composers and should not be called by analyses.
bool CodeLocObject::subSetAO(CodeLocObjectPtr that, PartEdgePtr pedge) {
  // If that object denotes the set of all parts, this is a subset of that
  if (that->part == NULLPart)
    return true;

  // If this object denotes the set of all parts, but that does not, this is not a subset
  if (part == NULLPart)
    return false;

  // If the two objects denote different parts, this is not a subset
  if (part != that->part)
    return false;

  // If that object denotes the set of all CFGNodes within the same part, this is a subset
  if (that->cfgNode.getNode() == NULL)
    return true;

  // If this object denotes the set of all CFGNodes within the same part but that does not, this is not a subset
  if (cfgNode.getNode() == NULL)
    return false;

  // If the two objects denote different CFGNodes within the same part, this is not a subset
  if (cfgNode != that->cfgNode)
    return false;

  // The two objects denote the same part and CFGNode within it
  return true;
}

bool CodeLocObject::equalSet(CodeLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  return equalSetAO(that, pedge);
}

bool CodeLocObject::subSet(CodeLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  return subSetAO(that, pedge);
}

bool CodeLocObject::equalSet(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  CodeLocObjectPtr co = boost::dynamic_pointer_cast<CodeLocObject>(o);
  if (co)
    return equalSet(co, pedge, comp, analysis);
  else
    return false;
}

bool CodeLocObject::subSet(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  CodeLocObjectPtr co = boost::dynamic_pointer_cast<CodeLocObject>(o);
  if (co)
    return subSet(co, pedge, comp, analysis);
  else
    return false;
}

// Returns true if this object is live at the given part and false otherwise.
// This method is called by composers and should not be called by analyses.
bool CodeLocObject::isLiveAO(PartEdgePtr pedge) {
  // Code Locations are live by definition
  return true;
}

// General version of isLive that accounts for framework details before routing the call to the derived class' 
// isLiveCL check. Specifically, it routes the call through the composer to make sure the isLiveCL call gets the 
// right PartEdge
bool CodeLocObject::isLive(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  return isLiveAO(pedge);
}

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool CodeLocObject::meetUpdateAO(CodeLocObjectPtr that, PartEdgePtr pedge) {
  // If the two objects denote different parts, set the meet to denote the set of all parts and all CFGNodes
  if (part != that->part) {
    part = NULLPart;
    cfgNode = CFGNode();
    return true;
  }

  // If the two objects denote the same part but different CFGNodes within it, set the meet to denote
  // the set of all CFGNodes within this part
  if (cfgNode != that->cfgNode) {
    cfgNode = CFGNode();
    return true;
  }

  // The two objects are identical
  return true;
}

// General version of meetUpdate that accounts for framework details before routing the call to the derived class' 
// meetUpdateCL check. Specifically, it routes the call through the composer to make sure the meetUpdateCL 
// call gets the right PartEdge
bool CodeLocObject::meetUpdate(CodeLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  return meetUpdateAO(that, pedge);
}

bool CodeLocObject::meetUpdate(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  CodeLocObjectPtr cl = boost::dynamic_pointer_cast<CodeLocObject>(that);
  assert(cl);
  return meetUpdate(cl, pedge, comp, analysis);
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool CodeLocObject::isFullAO(PartEdgePtr pedge) {
  return part == NULLPart && cfgNode.getNode() == NULL;
}

// Returns whether this AbstractObject denotes the empty set.
bool CodeLocObject::isEmptyAO(PartEdgePtr pedge) {
  // It is not possible to create an empty CodeLocObject
  return false;
}

// General versions of isFull() and isEmpty that account for framework details before routing the call to the 
// derived class' isFull() and isEmpty()  check. Specifically, it routes the call through the composer to make 
// sure the isFull(PartEdgePtr) and isEmpty(PartEdgePtr) call gets the right PartEdge.
// These functions are just aliases for the real implementations in AbstractObject
bool CodeLocObject::isFull(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  return isFullAO(pedge);
}

bool CodeLocObject::isEmpty(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  return isEmptyAO(pedge);
}

// Allocates a copy of this object and returns a pointer to it
AbstractObjectPtr CodeLocObject::copyAO() const {
  return copyAOType();
}

std::string CodeLocObject::str(std::string indent) const { // pretty print for the object
  ostringstream oss;
  oss << "[CodeLocObject: part="
      << (part ? part->str(indent + "    ") : "ANY") << ", " << endl;
  oss << indent << "                cfgNode="
      << (cfgNode.getNode() ? CFGNode2Str(cfgNode) : "ANY") << "]";
  return oss.str();
}

/* #################################
   ##### FullCodeLocObject   #####
   ################################# */

bool FullCodeLocObject::mayEqualAO(CodeLocObjectPtr o, PartEdgePtr pedge) {
  return true;
}

bool FullCodeLocObject::mustEqualAO(CodeLocObjectPtr o, PartEdgePtr pedge) {
  return false;
}

bool FullCodeLocObject::equalSetAO(CodeLocObjectPtr o, PartEdgePtr pedge) {
  return isFullAO(pedge);
}

bool FullCodeLocObject::subSetAO(CodeLocObjectPtr o, PartEdgePtr pedge) {
  return isFullAO(pedge);
}

bool FullCodeLocObject::isLiveAO(PartEdgePtr pedge) {
  return true;
}

bool FullCodeLocObject::meetUpdateAO(CodeLocObjectPtr that,
    PartEdgePtr pedge) {
  return false;
}

bool FullCodeLocObject::isFullAO(PartEdgePtr pedge) {
  return true;
}

bool FullCodeLocObject::isEmptyAO(PartEdgePtr pedge) {
  return false;
}

// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
bool FullCodeLocObject::isConcrete() {
  return false;
}

// Returns the number of concrete values in this set
int FullCodeLocObject::concreteSetSize() {
  return -1;
}

CodeLocObjectPtr FullCodeLocObject::copyAOType() const {
  return boost::make_shared<FullCodeLocObject>();
}

string FullCodeLocObject::str(string indent) const {
  return "FullCodeLocObject";
}

/* #################################
   ##### CombinedCodeLocObject   #####
   ################################# */

///*template <bool defaultMayEq>
// CombinedCodeLocObject<defaultMayEq>::CombinedCodeLocObject(CodeLocObjectPtr codeLoc): CodeLocObject(NULL) {
// codeLocs.push_back(codeLoc);
// }
//
// template <bool defaultMayEq>
// CombinedCodeLocObject<defaultMayEq>::CombinedCodeLocObject(const list<CodeLocObjectPtr>& codeLocs) : CodeLocObject(NULL), codeLocs(codeLocs) {}
//
// template <bool defaultMayEq>
// void CombinedCodeLocObject<defaultMayEq>::add(CodeLocObjectPtr codeLoc) {
// codeLocs.push_back(codeLoc);
// }*/
//
//template<bool defaultMayEq>
//SgNode* CombinedCodeLocObject<defaultMayEq>::getBase() const {
//  // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
//  SgNode* base = NULL;
//  for (list<CodeLocObjectPtr>::const_iterator it = codeLocs.begin();
//      it != codeLocs.end(); it++) {
//    if (it == codeLocs.begin())
//      base = (*it)->getBase();
//    else if (base != (*it)->getBase())
//      return NULL;
//  }
//
//  return base;
//}
//
//// Returns whether this object may/must be equal to o within the given Part p
//// These methods are private to prevent analyses from calling them directly.
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::mayEqualAO(CodeLocObjectPtr o,
//    PartEdgePtr pedge) {
//  boost::shared_ptr<CombinedCodeLocObject<defaultMayEq> > that =
//      boost::dynamic_pointer_cast<CombinedCodeLocObject<defaultMayEq> >(o);
//  // If the two combination objects include different numbers of CodeLocObjects, say that they may be equal since
//  // we can't be sure either way.
//  if (codeLocs.size() != that->codeLocs.size())
//    return true;
//
//  // Compare all the pairs of CodeLocObjects in codeLocs and that.codeLocs, returning defaultMayEq if any pair
//  // returns defaultMayEq since we're looking for the tightest (if defaultMayEq=false) / loosest (if defaultMayEq=true)
//  // answer that any CodeLocObject in codeLocs can give
//  for (list<CodeLocObjectPtr>::iterator thisIt = codeLocs.begin(), thatIt =
//      that->codeLocs.begin(); thisIt != codeLocs.end(); thisIt++, thatIt++) {
//    if ((*thisIt)->mayEqualAO(*thatIt, pedge) == defaultMayEq)
//      return defaultMayEq;
//  }
//
//  return !defaultMayEq;
//}
//
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::mustEqualAO(CodeLocObjectPtr o,
//    PartEdgePtr pedge) {
//  boost::shared_ptr<CombinedCodeLocObject<defaultMayEq> > that =
//      boost::dynamic_pointer_cast<CombinedCodeLocObject<defaultMayEq> >(o);
//  // If the two combination  objects include different numbers of CodeLocObjects, say that they are not must equal since
//  // we can't be sure either way.
//  if (codeLocs.size() != that->codeLocs.size())
//    return false;
//
//  // Compare all the pairs of CodeLocObjects in codeLocs and that.codeLocs, returning !defaultMayEq if any pair
//  // returns !defaultMayEqual since we're looking for the tightest answer that any CodeLocObject in codeLocs can give
//  for (list<CodeLocObjectPtr>::iterator thisIt = codeLocs.begin(), thatIt =
//      that->codeLocs.begin(); thisIt != codeLocs.end(); thisIt++, thatIt++) {
//    if ((*thisIt)->mustEqualAO(*thatIt, pedge) == !defaultMayEq)
//      return !defaultMayEq;
//  }
//
//  return defaultMayEq;
//}
//
//// Returns whether the two abstract objects denote the same set of concrete objects
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::equalSetAO(CodeLocObjectPtr o,
//    PartEdgePtr pedge) {
//  boost::shared_ptr<CombinedCodeLocObject<defaultMayEq> > that =
//      boost::dynamic_pointer_cast<CombinedCodeLocObject<defaultMayEq> >(o);
//  assert(that);
//  assert(codeLocs.size() == that->codeLocs.size());
//
//  // Two unions and intersections denote the same set of their components individually denote the same set
//  // (we can get a more precise answer if we could check set containment relations as well)
//  list<CodeLocObjectPtr>::const_iterator clThis = codeLocs.begin();
//  list<CodeLocObjectPtr>::const_iterator clThat = that->codeLocs.begin();
//  for (; clThis != codeLocs.end(); clThis++, clThat++)
//    if (!(*clThis)->equalSetAO(*clThat, pedge))
//      return false;
//  return true;
//}
//
//// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
//// by the given abstract object.
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::subSetAO(CodeLocObjectPtr o,
//    PartEdgePtr pedge) {
//  boost::shared_ptr<CombinedCodeLocObject<defaultMayEq> > that =
//      boost::dynamic_pointer_cast<CombinedCodeLocObject<defaultMayEq> >(o);
//  assert(that);
//  assert(codeLocs.size() == that->codeLocs.size());
//
//  // Compare all the pairs of CodeLocObjects in memLocs and that.memLocs, returning defaultMayEq if any pair
//  // returns defaultMayEq since we're looking for the tightest (if defaultMayEq=false) / loosest (if defaultMayEq=true)
//  // answer that any CodeLocObject in memLocs can give
//  for (list<CodeLocObjectPtr>::iterator thisIt = codeLocs.begin(), thatIt =
//      that->codeLocs.begin(); thisIt != codeLocs.end(); thisIt++, thatIt++) {
//    if ((*thisIt)->subSetAO(*thatIt, pedge) == defaultMayEq)
//      return defaultMayEq;
//  }
//  return !defaultMayEq;
//}
//
//// Returns true if this object is live at the given part and false otherwise
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::isLiveAO(PartEdgePtr pedge) {
//  // If this is a union type (defaultMayEq=true), an object is live if any of its components are live (weakest constraint)
//  // If this is an intersection type (defaultMayEq=false), an object is dead if any of its components are dead (strongest constraint)
//  for (list<CodeLocObjectPtr>::const_iterator cl = codeLocs.begin();
//      cl != codeLocs.end(); cl++)
//    if ((*cl)->isLiveAO(pedge) == defaultMayEq)
//      return defaultMayEq;
//
//  return !defaultMayEq;
//}
//
//// Computes the meet of this and that and saves the result in this
//// returns true if this causes this to change and false otherwise
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::meetUpdateAO(CodeLocObjectPtr o,
//    PartEdgePtr pedge) {
//  boost::shared_ptr<CombinedCodeLocObject<defaultMayEq> > that =
//      boost::dynamic_pointer_cast<CombinedCodeLocObject<defaultMayEq> >(o);
//  assert(that);
//  assert(codeLocs.size() == that->codeLocs.size());
//  bool modified = false;
//
//  // Perform the meetUpdate operation on all member codeLocss
//  list<CodeLocObjectPtr>::const_iterator clThis = codeLocs.begin();
//  list<CodeLocObjectPtr>::const_iterator clThat = that->codeLocs.begin();
//  for (; clThis != codeLocs.end(); clThis++, clThat++)
//    modified = (*clThis)->meetUpdateAO(*clThat, pedge) || modified;
//  return modified;
//}
//
//// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::isFullAO(PartEdgePtr pedge) {
//  // If this is a union type (defaultMayEq=true), an object is full if any of its components are full (weakest constraint)
//  // If this is an intersection type (defaultMayEq=false), an object is not full if any of its components are not full (strongest constraint)
//  for (list<CodeLocObjectPtr>::const_iterator cl = codeLocs.begin();
//      cl != codeLocs.end(); cl++)
//    if ((*cl)->isFullAO(pedge) == defaultMayEq)
//      return defaultMayEq;
//
//  return !defaultMayEq;
//}
//
//// Returns whether this AbstractObject denotes the empty set.
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::isEmptyAO(PartEdgePtr pedge) {
//  // If this is a union type (defaultMayEq=true), an object is not empty if any of its components are not empty (weakest constraint)
//  // If this is an intersection type (defaultMayEq=false), an object is empty if any of its components are empty (strongest constraint)
//  for (list<CodeLocObjectPtr>::const_iterator cl = codeLocs.begin();
//      cl != codeLocs.end(); cl++)
//    if ((*cl)->isEmptyAO(pedge) != defaultMayEq)
//      return !defaultMayEq;
//
//  return defaultMayEq;
//}
//
///*// Returns true if this object is live at the given part and false otherwise
// template <bool defaultMayEq>
// bool CombinedCodeLocObject<defaultMayEq>::isLive(PartEdgePtr pedge)
// {
// // If this is a union type (defaultMayEq=true), an object is live if any of its components are live (weakest constraint)
// // If this is an intersection type (defaultMayEq=false), an object is dead if any of its components are dead (strongest constraint)
// for(list<CodeLocObjectPtr>::const_iterator cl=codeLocs.begin(); cl!=codeLocs.end(); cl++)
// if((*cl)->isLive(pedge) == defaultMayEq) return defaultMayEq;
//
// return !defaultMayEq;
// }*/
//
//// Returns true if this CodeLocObject corresponds to a concrete value that is statically-known
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::isConcrete() {
//  // The combined object is concrete if
//  // intersect (defaultMayEq=false) : any sub-cl is concrete
//  // union (defaultMayEq=true) : all the sub-cl are concrete
//
//  // Intersection
//  if (defaultMayEq == false) {
//    for (list<CodeLocObjectPtr>::iterator cl = codeLocs.begin();
//        cl != codeLocs.end(); cl++) {
//      if ((*cl)->isConcrete())
//        return true;
//    }
//    return false;
//    // Union
//  } else {
//    assert(codeLocs.size() > 0);
//    // The union is not concrete if
//    for (list<CodeLocObjectPtr>::iterator cl = codeLocs.begin();
//        cl != codeLocs.end(); cl++) {
//      // Any sub-value is not concrete
//      if (!(*cl)->isConcrete())
//        return false;
//    }
//    return true;
//  }
//}
//
//// Returns the number of concrete values in this set
//template<bool defaultMayEq>
//int CombinedCodeLocObject<defaultMayEq>::concreteSetSize() {
//  assert(isConcrete());
//  // This is an over-approximation of the set size that assumes that all the concrete sets of
//  // the sub-CodeLocs are disjoint
//  int size = 0;
//  for (list<CodeLocObjectPtr>::const_iterator cl = codeLocs.begin();
//      cl != codeLocs.end(); cl++)
//    size += (*cl)->concreteSetSize();
//  return size;
//}
//
//// Allocates a copy of this object and returns a pointer to it
//template<bool defaultMayEq>
//CodeLocObjectPtr CombinedCodeLocObject<defaultMayEq>::copyAOType() const {
//  return boost::make_shared<CombinedCodeLocObject>(codeLocs);
//}
//
//template<bool defaultMayEq>
//std::string CombinedCodeLocObject<defaultMayEq>::str(
//    std::string indent) const {
//  ostringstream oss;
//  if (codeLocs.size() > 1)
//    oss << "[" << (defaultMayEq ? "UnionCL" : "IntersectCL") << ": ";
//  if (codeLocs.size() > 1)
//    oss << endl;
//  for (list<CodeLocObjectPtr>::const_iterator cl = codeLocs.begin();
//      cl != codeLocs.end();) {
//    if (cl != codeLocs.begin())
//      oss << indent << "&nbsp;&nbsp;&nbsp;&nbsp;";
//    oss << (*cl)->str(indent + "&nbsp;&nbsp;&nbsp;&nbsp;");
//    cl++;
//    if (cl != codeLocs.end())
//      oss << endl;
//  }
//  if (codeLocs.size() > 1)
//    oss << "]";
//
//  return oss.str();
//}
//
//// Returns whether all instances of this class form a hierarchy. Every instance of the same
//// class created by the same analysis must return the same value from this method!
//template<bool defaultMayEq>
//bool CombinedCodeLocObject<defaultMayEq>::isHierarchy() const {
//  // Combined CodeLocs form hierarchy if:
//  // - All the sub-CodeLocs form hierarchies of their own, AND
//  // - The combination is an intersection.
//  //   If the combination is a union then consider the following:
//  //            MLa     MLb
//  //   comb1 = {a,b}, {w, x}
//  //   comb2 = {a,b}, {y, z}
//  //   Note that MLs from analyses a and b are either identical sets or disjoint
//  //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
//  //   not hold unless we work out a new hierarchy for MLb.
//
//  // Unions are not hierarchical unless they're singletons
//  if (defaultMayEq) {
//    if (codeLocs.size() == 1)
//      return (*codeLocs.begin())->isHierarchy();
//    else
//      return false;
//  }
//
//  for (list<CodeLocObjectPtr>::const_iterator ml = codeLocs.begin();
//      ml != codeLocs.end(); ml++)
//    if (!(*ml)->isHierarchy())
//      return false;
//  return true;
//}
//
//// Returns a key that uniquely identifies this particular AbstractObject in the
//// set hierarchy.
//template<bool defaultMayEq>
//const AbstractionHierarchy::hierKeyPtr& CombinedCodeLocObject<defaultMayEq>::getHierKey() const {
//  // The intersection of multiple objects is just a filtering process from the full
//  // set of objects to the exact one, with each key in the hierarchy further filtering
//  // the set down. As such, a hierarchical key for the intersection of multiple objects
//  // is just the concatenation of the keys of all the individual objects.
//  if (!isHierKeyCached) {
//    /*((CombinedCodeLocObject<defaultMayEq>*)this)->cachedHierKey = boost::make_shared<AOSHierKey>(((CombinedCodeLocObject<defaultMayEq>*)this)->shared_from_this());
//
//     for(list<CodeLocObjectPtr>::const_iterator i=codeLocs.begin(); i!=codeLocs.end(); i++) {
//     AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(*i);
//     ROSE_ASSERT(hierIt);
//
//     ((CombinedCodeLocObject<defaultMayEq>*)this)->cachedHierKey->add(hierIt->getHierKey()->begin(), hierIt->getHierKey()->end());
//     }*/
//    list<hierKeyPtr> subHierKeys;
//    for (list<CodeLocObjectPtr>::const_iterator i = codeLocs.begin(); i != codeLocs.end(); i++) {
//      AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(*i);
//      ROSE_ASSERT(hierIt);
//      subHierKeys.push_back(hierIt->getHierKey());
//    }
//
//    ((CombinedCodeLocObject<defaultMayEq>*) this)->cachedHierKey = boost::make_shared<IntersectHierKey>(subHierKeys);
//
//    ((CombinedCodeLocObject<defaultMayEq>*) this)->isHierKeyCached = true;
//  }
//  return cachedHierKey;
//}
//
//// Create a function that uses examples of combined objects to force the compiler to generate these classes
//static void exampleCombinedCodeLocObjects2(CodeLocObjectPtr cl,
//    std::list<CodeLocObjectPtr> cls, IntersectCodeLocObject& i,
//    UnionCodeLocObject& u, IntersectCodeLocObject& i2,
//    UnionCodeLocObject& u2);
//static void exampleCombinedCodeLocObjects(CodeLocObjectPtr cl,
//    std::list<CodeLocObjectPtr> cls) {
//  IntersectCodeLocObject exampleIntersectObject(cl);
//  UnionCodeLocObject exampleUnionObject(cl);
//  IntersectCodeLocObject exampleIntersectObject2(cls);
//  UnionCodeLocObject exampleUnionObject2(cls);
//  exampleCombinedCodeLocObjects2(cl, cls, exampleIntersectObject,
//      exampleUnionObject, exampleIntersectObject2, exampleUnionObject2);
//}
//static void exampleCombinedCodeLocObjects2(CodeLocObjectPtr cl,
//    std::list<CodeLocObjectPtr> cls, IntersectCodeLocObject& i,
//    UnionCodeLocObject& u, IntersectCodeLocObject& i2,
//    UnionCodeLocObject& u2) {
//  exampleCombinedCodeLocObjects(cl, cls);
//}

/* ###############################
     ##### MappedCodeLocObject   #####
     ############################### */

//  template<class Key>
//  SgNode* MappedCodeLocObject<Key>::getBase() const {
//    // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
//    SgNode* base = NULL;
//    for (typename map<Key, CodeLocObjectPtr>::const_iterator it =
//        aoMap.begin(); it != aoMap.end(); ++it) {
//      if (it == aoMap.begin())
//        base = it->second->getBase();
//      else if (base != it->second->getBase())
//        return NULL;
//    }
//
//    return base;
//  }
//
//  template<class Key>
//  void MappedCodeLocObject<Key>::add(Key key,
//      CodeLocObjectPtr cl_p, PartEdgePtr pedge, Composer* comp,
//      ComposedAnalysis* analysis) {
//    // If the object is already full don't add anything
//    if (isUnion() && isFullAO(pedge))
//      return;
//
//    // If the cl_p is not full add/update the map
//    if (!cl_p->isFullAO(pedge)) {
//      aoMap[key] = cl_p;
//    } else {
//      nFull++;
//      if (isUnion())
//        setCLToFull();
//    }
//  }
//
//  template<class Key>
//  bool MappedCodeLocObject<Key>::mayEqualCLWithKey(Key key,
//      const map<Key, CodeLocObjectPtr>& thatCLMap, PartEdgePtr pedge) {
//    typename map<Key, CodeLocObjectPtr>::const_iterator s_it;
//    s_it = thatCLMap.find(key);
//    if (s_it == thatCLMap.end())
//      return true;
//    return aoMap[key]->mayEqualAO(s_it->second, pedge);
//  }
//
////! Two CL objects are may equals if there is atleast one execution or sub-exectuion
////! in which they represent the same code location.
////! Analyses are conservative as they start with full set of executions.
////! Dataflow facts (predicates) shrink the set of sub-executions.
////! We do not explicity store set of sub-executions and they are described 
////! by the abstract objects computed from dataflow fact exported by the analysis.
////! Unless the analyses discover otherwise, the conservative answer for mayEqualCL is true.
////! Mapped CLs are keyed using either ComposedAnalysis* or PartEdgePtr.
////! Each keyed CL object correspond to some dataflow facts computed by Key=Analysis* or 
////! computed at Key=PartEdgePtrthat describes some sets of executions.
////! MayEquality check on mapped CL is performed on intersection of sub-executions
////! or union of sub-executions over the keyed CL objects.
//  template<class Key>
//  bool MappedCodeLocObject<Key>::mayEqualAO(
//      CodeLocObjectPtr thatCL, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedCodeLocObject<Key> > thatCL_p =
//        boost::dynamic_pointer_cast<MappedCodeLocObject<Key> >(
//            thatCL);
//    assert(thatCL_p);
//
//    // This object denotes full set of CL (full set of executions)
//    if (isFullAO(pedge))
//      return true;
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return false;
//
//    // presence of one more full objects will result in full set over union
//    if (isUnion() && nFull > 0)
//      return true;
//
//    // Two cases reach here [1] isUnion()=true && nFull_CL=0 [2] intersect=true && nFullCL=0 or nFull_CL!=0.
//    // For both cases iterate on the CL map and discharge the mayEqualCL query to individual objects
//    // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
//    const map<Key, CodeLocObjectPtr> thatCLMap = thatCL_p->getCodeLocsMap();
//    typename map<Key, CodeLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      bool isMayEq = mayEqualCLWithKey(it->first, thatCLMap, pedge);
//
//      // 1. Union of sub-executions and the object does not contain any full objects.
//      // If the discharged query comes back as true for this case then we have found atleast one execution
//      // under which the two objects are same and the set can only grow and the result of this query is not going
//      // to change due to union.
//      // If false we iterate further as any CL can add more executions under which the objects are may equals.
//      if (isUnion() && isMayEq == true)
//        return true;
//
//      // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
//      // The sub-executions are intersected and therefore it does not matter if we have full objects.
//      // If the discharged query returns false then return false.
//      // We did not find one execution in which the two objects are may equals.
//      // Note that set of executions are contained over keyed objects (analyses are conservative).
//      // This set only shrinks during intersection and it is not going to affect the result of this query.
//      // If it returns true iterate further as some executions corresponding to true may be dropped.
//      else if (isIntersection() && isMayEq == false)
//        return false;
//    }
//
//    // All the keyed objects returned false for the discharged query under union.
//    // We haven't found a single execution under which the two objects are may equals.
//    if (isUnion())
//      return false;
//    // All the keyed objects returned true for the discharged query under intersection.
//    // We have atleast one execution in common in which the two objects are may equals.
//    else if (isIntersection())
//      return true;
//    else
//      assert(0);
//  }
//
//  template<class Key>
//  bool MappedCodeLocObject<Key>::mustEqualCLWithKey(Key key,
//      const map<Key, CodeLocObjectPtr>& thatCLMap, PartEdgePtr pedge) {
//    typename map<Key, CodeLocObjectPtr>::const_iterator s_it;
//    s_it = thatCLMap.find(key);
//    if (s_it == thatCLMap.end())
//      return false;
//    return aoMap[key]->mustEqualAO(s_it->second, pedge);
//  }
//
////! Two CL objects are must equals if they represent the same code
////! location on all executions.
////! Analyses are conservative as they start with full set of executions.
////! Dataflow facts (predicates) shrink the set of sub-executions.
////! We do not explicity store set of sub-executions and they are described
////! by the abstract objects computed from dataflow fact exported by the analysis.
////! Unless the analyses discover otherwise conservative answer for mustEqualCL is false.
////! Mapped CLs are keyed using either ComposedAnalysis* or PartEdgePtr.
////! Each keyed CL object correspond to some dataflow facts computed by Key=Analysis* or
////! computed at Key=PartEdgePtrthat describes some sets of executions.
////! MustEquality check on mapped CL is performed on intersection (mostAccurate=true) of sub-executions
////! or union (mostAccurate=false) of sub-executions over the keyed CL objects.
//  template<class Key>
//  bool MappedCodeLocObject<Key>::mustEqualAO(
//      CodeLocObjectPtr thatCL, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedCodeLocObject<Key> > thatCL_p =
//        boost::dynamic_pointer_cast<MappedCodeLocObject<Key> >(
//            thatCL);
//    assert(thatCL_p);
//
//    // This object denotes full set of CL (full set of executions)
//    if (isFullAO(pedge))
//      return false;
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return false;
//
//    // presence of one more full objects will result in full set over union
//    if (isUnion() && nFull > 0)
//      return true;
//
//    // Two cases reach here [1] isUnion()=true && nFull_CL=0 [2] intersect=true && nFullCL=0 or nFull_CL!=0.
//    // For both cases iterate on the CL map and discharge the mayEqualCL query to individual objects
//    // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
//    const map<Key, CodeLocObjectPtr> thatCLMap = thatCL_p->getCodeLocsMap();
//    typename map<Key, CodeLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      bool isMustEq = mustEqualCLWithKey(it->first, thatCLMap, pedge);
//
//      // 1. Union of sub-executions and the object does not contain any full objects
//      // If the discharged query comes back as false for this case then we have found atleast one execution
//      // under which the two objects are not same and the set can only grow and the result of this query is not going
//      // to change due to union.
//      // If it returns true we iterate further as any CL can add more executions under which the objects are not must equals.
//      if (isUnion() && isMustEq == false)
//        return false;
//
//      // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
//      // The sub-executions are intersected and therefore it does not matter if we have full objects.
//      // If the discharged query returns true then return true.
//      // Under all sub-executions (corresponding to the CL) the two objects must equal.
//      // Note that set of executions are contained over keyed objects as the analyses are conservative.
//      // This set only shrinks during intersection and it is not going to affect the result of this query.
//      // If it returns false iterate further as some executions corresponding to false may be dropped.
//      else if (isIntersection() && isMustEq == true)
//        return true;
//    }
//
//    // All the keyed objects returned true for the discharged query under union.
//    // We haven't found a single execution under which the two objects are not equal.
//    if (isUnion())
//      return true;
//    // All the keyed objects returned false for the discharged query under intersection.
//    // We have atleast one execution in common in which the two objects are not equal.
//    else if (isIntersection())
//      return false;
//    else
//      assert(0);
//  }
//
////! Discharge the query to the corresponding CL
////! If key not found in thatCLMap return false
//  template<class Key>
//  bool MappedCodeLocObject<Key>::equalSetCLWithKey(Key key,
//      const map<Key, CodeLocObjectPtr>& thatCLMap, PartEdgePtr pedge) {
//    typename map<Key, CodeLocObjectPtr>::const_iterator s_it;
//    s_it = thatCLMap.find(key);
//    if (s_it == thatCLMap.end())
//      return false;
//    return aoMap[key]->equalSetAO(s_it->second, pedge);
//  }
//
////! Two objects are equal sets if they denote the same set of memory locations
////! The boolean parameter mostAccurate is not releveant as this query is not
////! answered based on union or intersection of sub-executions.
////! Simply discharge the queries to all keyed CodeLoc objects
////! If all the discharged queries come back equal then the two objects are equal otherwise not.
//  template<class Key>
//  bool MappedCodeLocObject<Key>::equalSetAO(
//      CodeLocObjectPtr thatCL, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedCodeLocObject<Key> > thatCL_p =
//        boost::dynamic_pointer_cast<MappedCodeLocObject<Key> >(
//            thatCL);
//    assert(thatCL_p);
//
//    // This object denotes full set of CL (full set of executions)
//    if (isFullAO(pedge))
//      return thatCL_p->isFullAO(pedge);
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return thatCL_p->isEmptyAO(pedge);
//
//    const map<Key, CodeLocObjectPtr> thatCLMap = thatCL_p->getCodeLocsMap();
//    typename map<Key, CodeLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      // break even if one of them returns false
//      if (equalSetCLWithKey(it->first, thatCLMap, pedge) == false)
//        return false;
//    }
//
//    return true;
//  }
//
////! Discharge the query to the corresponding CL
////! If key not found in thatCLMap return true as the
////! keyed object on thatCLMap denotes full set
//  template<class Key>
//  bool MappedCodeLocObject<Key>::subSetCLWithKey(Key key,
//      const map<Key, CodeLocObjectPtr>& thatCLMap, PartEdgePtr pedge) {
//    typename map<Key, CodeLocObjectPtr>::const_iterator s_it;
//    s_it = thatCLMap.find(key);
//    if (s_it == thatCLMap.end())
//      return true;
//    return aoMap[key]->equalSetAO(s_it->second, pedge);
//  }
//
////! This object is a non-strict subset of the other if the set of memory locations denoted by this
////! is a subset of the set of memory locations denoted by that.
////! The boolean parameter mostAccurate is not releveant as this query is not
////! answered based on union or intersection of sub-executions.
////! Simply discharge the queries to all keyed CodeLoc objects
////! If all the discharged queries come back true then this is a subset of that otherwise not.
//  template<class Key>
//  bool MappedCodeLocObject<Key>::subSetAO(CodeLocObjectPtr thatCL,
//      PartEdgePtr pedge) {
//    boost::shared_ptr<MappedCodeLocObject<Key> > thatCL_p =
//        boost::dynamic_pointer_cast<MappedCodeLocObject<Key> >(
//            thatCL);
//    assert(thatCL_p);
//
//    // This object denotes full set of CL (full set of executions)
//    if (isFullAO(pedge))
//      return thatCL_p->isFullAO(pedge);
//
//    // denotes empty set
//    // thatCL could be empty or non-empty eitherway this will be a non-strict subset of that.
//    if (isEmptyAO(pedge))
//      return true;
//
//    // If both objects have the same keys discharge
//    // If this object has a key and that does not then
//    // the keyed object is subset of that (return true) implemented by subsetCLWithKey
//    // If any of the discharged query return false then return false.
//    const map<Key, CodeLocObjectPtr> thatCLMap = thatCL_p->getCodeLocsMap();
//    typename map<Key, CodeLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      // break even if one of them returns false
//      if (subSetCLWithKey(it->first, thatCLMap, pedge) == false)
//        return false;
//    }
//
//    // If this object doesn't have the key and that object has the key then
//    // return false as this object has full object mapped to the key
//    typename map<Key, CodeLocObjectPtr>::const_iterator c_it;
//    for (c_it = thatCLMap.begin(); c_it != thatCLMap.end() && (nFull != 0);
//        ++c_it) {
//      if (aoMap.find(c_it->first) == aoMap.end())
//        return false;
//    }
//
//    return true;
//  }
//
////! Mapped object liveness is determined based on finding executions
////! in which it may be live.
////! It can be answered based on union (mostAccurate=false) or intersection
////! (mostAccurate=true) of executions
////! The conservative answer is to assume that the object is live
//  template<class Key>
//  bool MappedCodeLocObject<Key>::isLiveAO(PartEdgePtr pedge) {
//    // If this object is full return the conservative answer
//    if (isFullAO(pedge))
//      return true;
//
//    // If it has one or more full objects added to it
//    // and if the object has mostAccurate=false then return true (weakest answer)
//    if (nFull > 0 && isUnion())
//      return true;
//
//    // 1. This object may have have one or more full objects under intersection
//    // 2. This object doesnt have any full objects added to it under union
//    // Under both cases the answer is based on how individual analysis respond to the query
//    typename map<Key, CodeLocObjectPtr>::iterator it = aoMap.begin();
//    for (; it != aoMap.end(); ++it) {
//      bool isLive = it->second->isLiveAO(pedge);
//      if (isUnion() && isLive == true)
//        return true;
//      else if (isIntersection() && isLive == false)
//        return false;
//    }
//
//    // leftover cases
//    if (isUnion())
//      return false;
//    else if (isIntersection())
//      return true;
//    else
//      assert(0);
//  }
//
////! meetUpdateCL performs the join operation of abstractions of two mls
//  template<class Key>
//  bool MappedCodeLocObject<Key>::meetUpdateAO(
//      CodeLocObjectPtr that, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedCodeLocObject<Key> > thatCL_p =
//        boost::dynamic_pointer_cast<MappedCodeLocObject<Key> >(
//            that);
//    assert(thatCL_p);
//
//    // if this object is already full
//    if (isFullAO(pedge))
//      return false;
//
//    // If that object is full set this object to full
//    if (thatCL_p->isFullAO(pedge)) {
//      nFull++;
//      setCLToFull();
//      return true;
//    }
//
//    // Both objects are not full
//    const map<Key, CodeLocObjectPtr> thatCLMap = thatCL_p->getCodeLocsMap();
//
//    typename map<Key, CodeLocObjectPtr>::iterator it = aoMap.begin();
//    typename map<Key, CodeLocObjectPtr>::const_iterator s_it; // search iterator for thatCLMap
//
//    bool modified = false;
//    while (it != aoMap.end()) {
//      s_it = thatCLMap.find(it->first);
//      // If two objects have the same key then discharge meetUpdate to the corresponding keyed CL objects
//      if (s_it != thatCLMap.end()) {
//        modified = (it->second)->meetUpdateAO(s_it->second, pedge) || modified;
//      }
//
//      // Remove the current CL object (current iterator it) from the map if the mapepd object is full.
//      // Two cases under which the current CL object can be full.
//      // (1) If current key is not found in thatCLMap then the mapped object
//      // in thatCLMap is full and the meetUpdate of the current CL with that is also full.
//      // (2) meetUpdateCL above of the two keyed objects resulted in this mapped object being full.
//      // Under both cases remove the mapped ml from this map
//      if (s_it == thatCLMap.end() || (it->second)->isFullAO(pedge)) {
//        // Current mapped CL has become full as a result of (1) or (2).
//        // Remove the item from the map.
//        // Note that post-increment which increments the iterator and returns the old value for deletion.
//        aoMap.erase(it++);
//        nFull++;
//        modified = true;
//
//        // If mostAccurate=false then set this entire object to full and return
//        if (isUnion()) {
//          setCLToFull();
//          return true;
//        }
//      } else
//        ++it;
//    }
//    return modified;
//  }
//
////! Method that sets this mapped object to full
//  template<class Key>
//  void MappedCodeLocObject<Key>::setCLToFull() {
//    assert(nFull > 0);
//    if (aoMap.size() > 0)
//      aoMap.clear();
//  }
//
//  template<class Key>
//  bool MappedCodeLocObject<Key>::isFullAO(PartEdgePtr pedge) {
//    if (nFull > 0 && aoMap.size() == 0)
//      return true;
//    return false;
//  }
//
//  template<class Key>
//  bool MappedCodeLocObject<Key>::isEmptyAO(PartEdgePtr pedge) {
//    if (nFull == 0 && aoMap.size() == 0)
//      return true;
//    return false;
//  }
//
//// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
//  template<class Key>
//  bool MappedCodeLocObject<Key>::isConcrete() {
//    typename map<Key, CodeLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it)
//      if (!it->second->isConcrete())
//        return false;
//    return true;
//  }
//
//// Returns the number of concrete values in this set
//  template<class Key>
//  int MappedCodeLocObject<Key>::concreteSetSize() {
//    // This is an over-approximation of the set size that assumes that all the concrete sets of
//    // the sub-CodeLocs are disjoint
//    int size = 0;
//    typename map<Key, CodeLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it)
//      size += it->second->concreteSetSize();
//    return size;
//  }
//
//  template<class Key>
//  CodeLocObjectPtr MappedCodeLocObject<Key>::copyAO() const {
//    return boost::make_shared<MappedCodeLocObject<Key> >(*this);
//  }
//
//  template<class Key>
//  string MappedCodeLocObject<Key>::str(string indent) const {
//    ostringstream oss;
//    oss << "<table border=\"1\">";
//    oss << "<tr>";
//    oss << "<th>"
//        << (isUnion() ?
//            "UnionMappedCodeLocObject:" : "IntersectMappedCodeLocObject:")
//        << "</th>";
//    if (nFull > 0 && aoMap.size() == 0)
//      oss << "<th> Full </th> </tr>";
//    else if (nFull == 0 && aoMap.size() == 0)
//      oss << "<th> Empty </th> </tr>";
//    else {
//      oss << "</tr>";
//      typename map<Key, CodeLocObjectPtr>::const_iterator it =
//          aoMap.begin();
//      for (; it != aoMap.end(); ++it) {
//        oss << "<tr>";
//        oss << "<td>" << (it->first)->str(indent) << "</td>";
//        oss << "<td>" << (it->second)->str(indent) << "</td>";
//        oss << "</tr>";
//      }
//    }
//    oss << "</table>";
//    return oss.str();
//  }
//
//// Returns whether all instances of this class form a hierarchy. Every instance of the same
//// class created by the same analysis must return the same value from this method!
//  template<class Key>
//  bool MappedCodeLocObject<Key>::isHierarchy() const {
//    // Combined CodeLocs form hierarchy if:
//    // - All the sub-CodeLocs form hierarchies of their own, AND
//    // - The combination is an intersection.
//    //   If the combination is a union then consider the following:
//    //            MLa     MLb
//    //   comb1 = {a,b}, {w, x}
//    //   comb2 = {a,b}, {y, z}
//    //   Note that MLs from analyses a and b are either identical sets or disjoint
//    //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
//    //   not hold unless we work out a new hierarchy for MLb.
//
//    // Unions are not hierarchical unless they're singletons
//    if (isUnion()) {
//      if (aoMap.size() == 1)
//        return aoMap.begin()->second->isHierarchy();
//      else
//        return false;
//    }
//
//    typename map<Key, CodeLocObjectPtr>::const_iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it)
//      if (!it->second->isHierarchy())
//        return false;
//    return true;
//  }
//
//// Returns a key that uniquely identifies this particular AbstractObject in the
//// set hierarchy.
//  template<class Key>
//  const AbstractionHierarchy::hierKeyPtr& MappedCodeLocObject<Key>::getHierKey() const {
//    // The intersection of multiple objects is just a filtering process from the full
//    // set of objects to the exact one, with each key in the hierarchy further filtering
//    // the set down. As such, a hierarchical key for the intersection of multiple objects
//    // is just the concatenation of the keys of all the individual objects.
//    if (!isHierKeyCached) {
//      /*((MappedCodeLocObject<Key>*) this)->cachedHierKey =
//       boost::make_shared<AOSHierKey>(
//       ((MappedCodeLocObject<Key>*) this)->shared_from_this());
//
//       typename map<Key, CodeLocObjectPtr>::const_iterator it;
//       for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//       AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<
//       AbstractionHierarchy>(it->second);
//       ROSE_ASSERT(hierIt);
//
//       ((MappedCodeLocObject<Key>*) this)->cachedHierKey->add(
//       hierIt->getHierKey()->begin(), hierIt->getHierKey()->end());
//       }*/
//
//      map<Key, hierKeyPtr> subHierKeys;
//      for(typename map<Key, CodeLocObjectPtr>::const_iterator it = aoMap.begin();
//          it != aoMap.end(); ++it) {
//        AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(it->second);
//        ROSE_ASSERT(hierIt);
//        subHierKeys[it->first] = hierIt->getHierKey();
//      }
//
//      ((MappedCodeLocObject<Key>*) this)->cachedHierKey =
//          boost::make_shared<IntersectMappedHierKey<Key> >(subHierKeys);
//
//      ((MappedCodeLocObject<Key>*) this)->isHierKeyCached = true;
//    }
//    return cachedHierKey;
//  }

/* ############################
 # PartEdgeUnionCodeLocObject #
   ############################ */

PartEdgeUnionCodeLocObject::PartEdgeUnionCodeLocObject() :
    CodeLocObject(NULL) {
}

PartEdgeUnionCodeLocObject::PartEdgeUnionCodeLocObject(
    const PartEdgeUnionCodeLocObject& thatCL) :
    CodeLocObject(thatCL), unionCL_p(thatCL.copyAOType()) {
}

SgNode* PartEdgeUnionCodeLocObject::getBase() const {
  return unionCL_p->getBase();
}

void PartEdgeUnionCodeLocObject::add(CodeLocObjectPtr cl_p, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is the very first object
  if (!unionCL_p)
    unionCL_p = cl_p->copyAOType();
  // If Full return without adding
  else if (isFullAO(pedge))
    return;
  // Else meetUpdate with the existing unionCL_p
  else
    unionCL_p->meetUpdateAO(cl_p, pedge);
}

bool PartEdgeUnionCodeLocObject::mayEqualAO(CodeLocObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionCodeLocObject> thatCL_p =
      boost::dynamic_pointer_cast<PartEdgeUnionCodeLocObject>(that);
  assert(thatCL_p);
  return unionCL_p->mayEqualAO(thatCL_p->getUnionAO(), pedge);
}

bool PartEdgeUnionCodeLocObject::mustEqualAO(CodeLocObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionCodeLocObject> thatCL_p =
      boost::dynamic_pointer_cast<PartEdgeUnionCodeLocObject>(that);
  assert(thatCL_p);
  return unionCL_p->mustEqualAO(thatCL_p->getUnionAO(), pedge);
}

bool PartEdgeUnionCodeLocObject::equalSetAO(CodeLocObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionCodeLocObject> thatCL_p =
      boost::dynamic_pointer_cast<PartEdgeUnionCodeLocObject>(that);
  assert(thatCL_p);
  return unionCL_p->equalSetAO(thatCL_p->getUnionAO(), pedge);
}

bool PartEdgeUnionCodeLocObject::subSetAO(CodeLocObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionCodeLocObject> thatCL_p =
      boost::dynamic_pointer_cast<PartEdgeUnionCodeLocObject>(that);
  assert(thatCL_p);
  return unionCL_p->subSetAO(thatCL_p->getUnionAO(), pedge);
}

bool PartEdgeUnionCodeLocObject::meetUpdateAO(CodeLocObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionCodeLocObject> thatCL_p =
      boost::dynamic_pointer_cast<PartEdgeUnionCodeLocObject>(that);
  assert(thatCL_p);
  return unionCL_p->meetUpdateAO(thatCL_p->getUnionAO(), pedge);
}

bool PartEdgeUnionCodeLocObject::isLiveAO(PartEdgePtr pedge) {
  return unionCL_p->isLiveAO(pedge);
}

bool PartEdgeUnionCodeLocObject::isFullAO(PartEdgePtr pedge) {
  return unionCL_p->isFullAO(pedge);
}

bool PartEdgeUnionCodeLocObject::isEmptyAO(PartEdgePtr pedge) {
  return unionCL_p->isEmptyAO(pedge);
}

// Returns true if this ValueObject corresponds to a concrete value that is statically-known
bool PartEdgeUnionCodeLocObject::isConcrete() {
  return unionCL_p->isConcrete();
}

// Returns the number of concrete values in this set
int PartEdgeUnionCodeLocObject::concreteSetSize() {
  return unionCL_p->concreteSetSize();
}

CodeLocObjectPtr PartEdgeUnionCodeLocObject::copyAOType() const {
  return boost::make_shared<PartEdgeUnionCodeLocObject>(*this);
}

void PartEdgeUnionCodeLocObject::setAOToFull() {
  unionCL_p = boost::make_shared<FullCodeLocObject>();
}

string PartEdgeUnionCodeLocObject::str(string indent) const {
  ostringstream oss;
  oss << "[UnionCL=" << unionCL_p->str(indent) << "]";
  return oss.str();
}

/* #######################
   ##### ValueObject #####
   ####################### */

ValueObjectPtr NULLValueObject;

// Returns whether this object may/must be equal to o within the given Part p
// by propagating the call through the composer
bool ValueObject::mayEqual(ValueObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  /*if(mayEqualCache.find(o) == mayEqualCache.end())
   mayEqualCache[o] = comp->mayEqualAO(shared_from_this(), o, pedge, analysis);
   return mayEqualCache[o];*/
  return comp->mayEqualV(shared_from_this(), o, pedge, analysis);
}

// Returns whether this object may/must be equal to o within the given Part p
// by propagating the call through the composer
bool ValueObject::mustEqual(ValueObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  /*if(mustEqualCache.find(o) == mustEqualCache.end())
   mustEqualCache[o] = comp->mustEqualAO(shared_from_this(), o, pedge, analysis);
   return mustEqualCache[o];*/
  return comp->mustEqualV(shared_from_this(), o, pedge, analysis);
}

// Returns whether this object may/must be equal to o within the given Part p
// by propagating the call through the composer
bool ValueObject::mayEqual(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  //if(AbstractObject::mustEqualExpr(boost::static_pointer_cast<AbstractObject>(o), pedge)) return true;

  ValueObjectPtr vo = boost::dynamic_pointer_cast<ValueObject>(o);
  if (vo)
    return mayEqual(vo, pedge, comp, analysis);
  else
    return false;
}

// Returns whether this object may/must be equal to o within the given Part p
// by propagating the call through the composer
bool ValueObject::mustEqual(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  //if(AbstractObject::mustEqualExpr(boost::static_pointer_cast<AbstractObject>(o), pedge)) return true;

  ValueObjectPtr vo = boost::dynamic_pointer_cast<ValueObject>(o);
  if (vo)
    return mustEqual(vo, pedge, comp, analysis);
  else
    return false;
}

bool ValueObject::equalSet(ValueObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  /*if(equalSetCache.find(that) == equalSetCache.end())
   equalSetCache[that] = comp->equalSetAO(shared_from_this(), that, pedge, analysis);
   return equalSetCache[that];*/
  return comp->equalSetV(shared_from_this(), that, pedge, analysis);
}

bool ValueObject::subSet(ValueObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  /*if(subSetCache.find(that) == subSetCache.end())
   subSetCache[that] = comp->equalSetAO(shared_from_this(), that, pedge, analysis);
   return subSetCache[that];*/
  return comp->equalSetV(shared_from_this(), that, pedge, analysis);
}

bool ValueObject::equalSet(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  ValueObjectPtr co = boost::dynamic_pointer_cast<ValueObject>(o);
  if (co)
    return equalSet(co, pedge, comp, analysis);
  else
    return false;
}

bool ValueObject::subSet(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  ValueObjectPtr co = boost::dynamic_pointer_cast<ValueObject>(o);
  if (co)
    return subSet(co, pedge, comp, analysis);
  else
    return false;
}

// General version of meetUpdate that accounts for framework details before routing the call to the derived class'
// meetUpdateV check. Specifically, it routes the call through the composer to make sure the meetUpdateV
// call gets the right PartEdge
bool ValueObject::meetUpdate(ValueObjectPtr that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  return comp->meetUpdateV(shared_from_this(), that, pedge, analysis);
}

bool ValueObject::meetUpdate(AbstractObjectPtr that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  ValueObjectPtr v = boost::dynamic_pointer_cast<ValueObject>(that);
  assert(v);
  return meetUpdate(v, pedge, comp, analysis);
}

// General version of isFull/isEmpty that accounts for framework details before routing the call to the
// derived class' isFullV/isEmptyV check. Specifically, it routes the call through the composer to make
// sure the isFullV/isEmptyV call gets the right PartEdge
bool ValueObject::isFull(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  return comp->isFullV(shared_from_this(), pedge, analysis);
}

bool ValueObject::isEmpty(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  return comp->isEmptyV(shared_from_this(), pedge, analysis);
}

// Returns true if this SgValueExp is convertible into a boolean
bool ValueObject::isValueBoolCompatible(boost::shared_ptr<SgValueExp> val) {
  return isSgCharVal(val.get()) || isSgBoolValExp(val.get())
      || isSgEnumVal(val.get()) || isSgIntVal(val.get())
      || isSgLongIntVal(val.get()) || isSgLongLongIntVal(val.get())
      || isSgShortVal(val.get()) || isSgUnsignedCharVal(val.get())
      || isSgUnsignedLongVal(val.get())
      || isSgUnsignedLongLongIntVal(val.get())
      || isSgUnsignedShortVal(val.get()) || isSgWcharVal(val.get());
}

// Convert the value of the given SgValueExp, cast to a boolean
bool ValueObject::SgValue2Bool(boost::shared_ptr<SgValueExp> val) {
  if (isSgCharVal(val.get()))
    return isSgCharVal(val.get())->get_value();
  else if (isSgBoolValExp(val.get()))
    return isSgBoolValExp(val.get())->get_value();
  else if (isSgEnumVal(val.get()))
    return isSgEnumVal(val.get())->get_value();
  else if (isSgIntVal(val.get()))
    return isSgIntVal(val.get())->get_value();
  else if (isSgLongIntVal(val.get()))
    return isSgLongIntVal(val.get())->get_value();
  else if (isSgLongLongIntVal(val.get()))
    return isSgLongLongIntVal(val.get())->get_value();
  else if (isSgShortVal(val.get()))
    return isSgShortVal(val.get())->get_value();
  else if (isSgUnsignedCharVal(val.get()))
    return isSgUnsignedCharVal(val.get())->get_value();
  else if (isSgUnsignedLongVal(val.get()))
    return isSgUnsignedLongVal(val.get())->get_value();
  else if (isSgUnsignedLongLongIntVal(val.get()))
    return isSgUnsignedLongLongIntVal(val.get())->get_value();
  else if (isSgUnsignedShortVal(val.get()))
    return isSgUnsignedShortVal(val.get())->get_value();
  else if (isSgWcharVal(val.get()))
    return isSgWcharVal(val.get())->get_valueUL();
  else {
    dbg << "val=" << SgNode2Str(val.get()) << endl;
    assert(0);
  }
}

// Allocates a copy of this object and returns a pointer to it
AbstractObjectPtr ValueObject::copyAO() const {
  return copyAOType();
}

// Returns true if the two SgValueExps correspond to the same value when cast to the given type (if t!=NULL)
bool ValueObject::equalValueExp(SgValueExp* e1, SgValueExp* e2, SgType* t) {
  // Currently not handling type conversions
  assert(t==NULL);

  if (e1->variantT() != e2->variantT())
    return false;

  if (isSgBoolValExp(e1)) {
    return isSgBoolValExp(e1)->get_value() == isSgBoolValExp(e2)->get_value();
  }
  if (isSgCharVal(e1)) {
    return isSgCharVal(e1)->get_value() == isSgCharVal(e2)->get_value();
  }
  if (isSgComplexVal(e1)) {
    return equalValueExp(isSgComplexVal(e1)->get_real_value(),
        isSgComplexVal(e2)->get_real_value())
        && equalValueExp(isSgComplexVal(e1)->get_imaginary_value(),
            isSgComplexVal(e2)->get_imaginary_value());
  }
  if (isSgDoubleVal(e1)) {
    return isSgDoubleVal(e1)->get_value() == isSgDoubleVal(e2)->get_value();
  }
  if (isSgEnumVal(e1)) {
    return isSgEnumVal(e1)->get_value() == isSgEnumVal(e2)->get_value();
  }
  if (isSgFloatVal(e1)) {
    return isSgFloatVal(e1)->get_value() == isSgFloatVal(e2)->get_value();
  }
  if (isSgIntVal(e1)) {
    return isSgIntVal(e1)->get_value() == isSgIntVal(e2)->get_value();
  }
  if (isSgLongDoubleVal(e1)) {
    return isSgLongDoubleVal(e1)->get_value()
        == isSgLongDoubleVal(e2)->get_value();
  }
  if (isSgLongIntVal(e1)) {
    return isSgLongIntVal(e1)->get_value() == isSgLongIntVal(e2)->get_value();
  }
  if (isSgLongLongIntVal(e1)) {
    return isSgLongLongIntVal(e1)->get_value()
        == isSgLongLongIntVal(e2)->get_value();
  }
  if (isSgShortVal(e1)) {
    return isSgShortVal(e1)->get_value() == isSgShortVal(e2)->get_value();
  }
  if (isSgStringVal(e1)) {
    return isSgStringVal(e1)->get_value() == isSgStringVal(e2)->get_value();
  }
  if (isSgWcharVal(e1)) {
    return isSgWcharVal(e1)->get_value() == isSgWcharVal(e2)->get_value();
  }
  if (isSgUnsignedCharVal(e1)) {
    return isSgUnsignedCharVal(e1)->get_value()
        == isSgUnsignedCharVal(e2)->get_value();
  }
  if (isSgUnsignedIntVal(e1)) {
    return isSgUnsignedIntVal(e1)->get_value()
        == isSgUnsignedIntVal(e2)->get_value();
  }
  if (isSgUnsignedLongLongIntVal(e1)) {
    return isSgUnsignedLongLongIntVal(e1)->get_value()
        == isSgUnsignedLongLongIntVal(e2)->get_value();
  }
  if (isSgUnsignedLongVal(e1)) {
    return isSgUnsignedLongVal(e1)->get_value()
        == isSgUnsignedLongVal(e2)->get_value();
  }
  if (isSgUnsignedShortVal(e1)) {
    return isSgWcharVal(e1)->get_value()
        == isSgUnsignedShortVal(e2)->get_value();
  }
  if (isSgUpcMythread(e1)) {
    return isSgUpcMythread(e1)->get_value()
        == isSgUpcMythread(e2)->get_value();
  }
  if (isSgUpcThreads(e1)) {
    return isSgUpcThreads(e1)->get_value() == isSgUpcThreads(e2)->get_value();
  }

  assert(0);
}

/* ##############################
   ##### FullValueObject   #####
   ############################## */

// Returns whether this object may/must be equal to o within the given Part p
// These methods are private to prevent analyses from calling them directly.
bool FullValueObject::mayEqualAO(ValueObjectPtr o, PartEdgePtr pedge) {
  // Since this object denotes the set of all values, it may-equals all value sets
  return true;
}

bool FullValueObject::mustEqualAO(ValueObjectPtr o, PartEdgePtr pedge) {
  // Since this object denotes the set of all values, which has unbounded size, it is not must-equal to any value set
  return false;
}

// Returns whether the two abstract objects denote the same set of concrete objects
bool FullValueObject::equalSetAO(ValueObjectPtr o, PartEdgePtr pedge) {
  // This object is only equal to objects that also denote the set of all values
  return o->isFullAO(pedge);
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
bool FullValueObject::subSetAO(ValueObjectPtr o, PartEdgePtr pedge) {
  // This object is only a subset of objects that also denote the set of all values
  return o->isFullAO(pedge);
}

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool FullValueObject::meetUpdateAO(ValueObjectPtr that, PartEdgePtr pedge) {
  // There is no way to make this object denote a larger set of values since it already denotes
  // the set of all values
  return false;
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool FullValueObject::isFullAO(PartEdgePtr pedge) {
  return true;
}

// Returns whether this AbstractObject denotes the empty set.
bool FullValueObject::isEmptyAO(PartEdgePtr pedge) {
  return false;
}

// Returns true if this ValueObject corresponds to a concrete value that is statically-known
bool FullValueObject::isConcrete() {
  return false;
}

// Returns the number of concrete values in this set
int FullValueObject::concreteSetSize() {
  return -1;
}

// Returns the type of the concrete value (if there is one)
SgType* FullValueObject::getConcreteType() {
  return NULL;
}

// Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
// the normal ROSE mechanisms to decode it
set<boost::shared_ptr<SgValueExp> > FullValueObject::getConcreteValue() {
  return set<boost::shared_ptr<SgValueExp> >();
}

// Allocates a copy of this object and returns a pointer to it
ValueObjectPtr FullValueObject::copyAOType() const {
  return boost::make_shared<FullValueObject>();
}

std::string FullValueObject::str(std::string indent) const {
  return "[FullValueObject]";
}

/* ################################
   ##### CombinedValueObject #####
   ################################ * /

/ *template <bool defaultMayEq>
 CombinedValueObject<defaultMayEq>::CombinedValueObject(ValueObjectPtr val) : ValueObject(NULL) {
 vals.push_back(val);
 }* /

/ *template <bool defaultMayEq>
 CombinedValueObject<defaultMayEq>::CombinedValueObject(const list<ValueObjectPtr>& vals) : ValueObject(NULL), vals(vals) {}* /

template<bool defaultMayEq>
SgNode* CombinedValueObject<defaultMayEq>::getBase() const {
  // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
  SgNode* base = NULL;
  for (list<ValueObjectPtr>::const_iterator it = vals.begin();
      it != vals.end(); it++) {
    if (it == vals.begin())
      base = (*it)->getBase();
    else if (base != (*it)->getBase())
      return NULL;
  }

  return base;
}

template<bool defaultMayEq>
void CombinedValueObject<defaultMayEq>::add(ValueObjectPtr val) {
  vals.push_back(val);
}

// Returns whether this object may/must be equal to o within the given Part p
// These methods are private to prevent analyses from calling them directly.
template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::mayEqualAO(ValueObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedValueObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedValueObject<defaultMayEq> >(o);
  // If the two combination objects include different numbers of ValueObjects, say that they may be equal since
  // we can't be sure either way.
  if (vals.size() != that->vals.size())
    return true;

  // Compare all the pairs of ValueObjects in vals and that.vals, returning defaultMayEq if any pair
  // returns defaultMayEq since we're looking for the tightest (if defaultMayEq=false) / loosest (if defaultMayEq=true)
  // answer that any ValueObject in vals can give
  for (list<ValueObjectPtr>::iterator thisIt = vals.begin(), thatIt =
      that->vals.begin(); thisIt != vals.end(); thisIt++, thatIt++) {
    if ((*thisIt)->mayEqualAO(*thatIt, pedge) == defaultMayEq)
      return defaultMayEq;
  }

  return !defaultMayEq;
}

template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::mustEqualAO(ValueObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedValueObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedValueObject<defaultMayEq> >(o);
  // If the two combination  objects include different numbers of ValueObjects, say that they are not must equal since
  // we can't be sure either way.
  if (vals.size() != that->vals.size())
    return false;

  // Compare all the pairs of ValueObjects in vals and that.vals, returning !defaultMayEq if any pair
  // returns !defaultMayEqual since we're looking for the tightest answer that any ValueObject in vals can give
  for (list<ValueObjectPtr>::iterator thisIt = vals.begin(), thatIt =
      that->vals.begin(); thisIt != vals.end(); thisIt++, thatIt++) {
    if ((*thisIt)->mustEqualAO(*thatIt, pedge) == !defaultMayEq)
      return !defaultMayEq;
  }

  return defaultMayEq;
}

// Returns whether the two abstract objects denote the same set of concrete objects
template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::equalSetAO(ValueObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedValueObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedValueObject<defaultMayEq> >(o);
  assert(that);
  assert(vals.size() == that->vals.size());

  // Two unions and intersections denote the same set of their components individually denote the same set
  // (we can get a more precise answer if we could check set containment relations as well)
  list<ValueObjectPtr>::const_iterator vThis = vals.begin();
  list<ValueObjectPtr>::const_iterator vThat = that->vals.begin();
  for (; vThis != vals.end(); vThis++, vThat++)
    if (!(*vThis)->equalSetAO(*vThat, pedge))
      return false;
  return true;
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::subSetAO(ValueObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedValueObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedValueObject<defaultMayEq> >(o);
  assert(that);
  assert(vals.size() == that->vals.size());

  // Compare all the pairs of ValueObjects in memLocs and that.memLocs, returning defaultMayEq if any pair
  // returns defaultMayEq since we're looking for the tightest (if defaultMayEq=false) / loosest (if defaultMayEq=true)
  // answer that any ValueObject in memLocs can give

  // Two unions and intersections denote the same set of their components individually denote the same set
  // (we can get a more precise answer if we could check set containment relations as well)
  for (list<ValueObjectPtr>::iterator thisIt = vals.begin(), thatIt =
      that->vals.begin(); thisIt != vals.end(); thisIt++, thatIt++) {
    if ((*thisIt)->subSetAO(*thatIt, pedge) == defaultMayEq)
      return defaultMayEq;
  }
  return !defaultMayEq;
}

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::meetUpdateAO(ValueObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedValueObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedValueObject<defaultMayEq> >(o);
  assert(that);
  assert(vals.size() == that->vals.size());
  bool modified = false;

  // Perform the meetUpdate operation on all member Values
  list<ValueObjectPtr>::const_iterator vThis = vals.begin();
  list<ValueObjectPtr>::const_iterator vThat = that->vals.begin();
  for (; vThis != vals.end(); vThis++, vThat++)
    modified = (*vThis)->meetUpdateAO(*vThat, pedge) || modified;
  return modified;
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::isFullAO(PartEdgePtr pedge) {
  // If this is a union type (defaultMayEq=true), an object is full if any of its components are full (weakest constraint)
  // If this is an intersection type (defaultMayEq=false), an object is not full if any of its components are not full (strongest constraint)
  for (list<ValueObjectPtr>::const_iterator v = vals.begin(); v != vals.end();
      v++)
    if ((*v)->isFullAO(pedge) == defaultMayEq)
      return defaultMayEq;

  return !defaultMayEq;
}

// Returns whether this AbstractObject denotes the empty set.
template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::isEmptyAO(PartEdgePtr pedge) {
  // If this is a union type (defaultMayEq=true), an object is not empty if any of its components are not empty (weakest constraint)
  // If this is an intersection type (defaultMayEq=false), an object is empty if any of its components are empty (strongest constraint)
  for (list<ValueObjectPtr>::const_iterator v = vals.begin(); v != vals.end();
      v++)
    if ((*v)->isEmptyAO(pedge) != defaultMayEq)
      return !defaultMayEq;

  return defaultMayEq;
}

// Returns true if this ValueObject corresponds to a concrete value that is statically-known
template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::isConcrete() {
  // The combined object is concrete if
  // intersect (defaultMayEq=false) : any sub-value is concrete
  // union (defaultMayEq=true) : all the sub-values are concrete and have the same type and value

  // Intersection
  if (defaultMayEq == false) {
    for (list<ValueObjectPtr>::iterator v = vals.begin(); v != vals.end();
        v++) {
      if ((*v)->isConcrete())
        return true;
    }
    return false;
    // Union
  } else {
    assert(vals.size() > 0);
    list<ValueObjectPtr>::iterator firstI = vals.begin();
    ValueObjectPtr first = *firstI;

    // The union is not concrete if
    for (list<ValueObjectPtr>::iterator v = vals.begin(); v != vals.end();
        v++) {
      // Any sub-value is not concrete, OR
      if (!(*v)->isConcrete())/ * ||
       // Any pair of sub-values have different types, OR
       (*v)->getConcreteType()->variantT() != first->getConcreteType()->variantT() ||
       // Any pair of sub-values have different values
       !ValueObject::equalValueExp((*v)->getConcreteValue().get(), first->getConcreteValue().get())) {* /
        return false;
      //}
    }
    return true;
  }
}

// Returns the number of concrete values in this set
template<bool defaultMayEq>
int CombinedValueObject<defaultMayEq>::concreteSetSize() {
  assert(isConcrete());
  return getConcreteValue().size();
}

// Returns the type of the concrete value (if there is one)
template<bool defaultMayEq>
SgType* CombinedValueObject<defaultMayEq>::getConcreteType() {
  assert(isConcrete());

  return (*vals.begin())->getConcreteType();
}

// Returns the concrete value (if there is one) as an SgValueExp, which allows callers to use
// the normal ROSE mechanisms to decode it
template<bool defaultMayEq>
std::set<boost::shared_ptr<SgValueExp> > CombinedValueObject<defaultMayEq>::getConcreteValue() {
  assert(isConcrete());

  // If this is a union type (defaultMayEq=true), the result is the Union of the sets returned by getConcreteValue() on all the vals.
  // If this is an intersection type (defaultMayEq=false), an object is their Intersection.

  // Maps each concrete value to the number of elements in vals for which it was returned
  std::map<boost::shared_ptr<SgValueExp>, size_t> concreteVals;
  for (list<ValueObjectPtr>::iterator v = vals.begin(); v != vals.end();
      v++) {
    // Iterate through the current sub-ValueObject's concrete values and increment each
    // concrete value's counter in concreteVals.
    std::set<boost::shared_ptr<SgValueExp> > curConcr =
        (*v)->getConcreteValue();
    for (std::set<boost::shared_ptr<SgValueExp> >::iterator i1 =
        curConcr.begin(); i1 != curConcr.end(); i1++) {
      // Find the key in concrete vals with an equivalent SgValueExp to *i1
      std::map<boost::shared_ptr<SgValueExp>, size_t>::iterator i2 =
          concreteVals.begin();
      for (; i2 != concreteVals.end(); i2++) {
        if (ValueObject::equalValueExp(i1->get(), i2->first.get()))
          ++i2->second;
      }

      // If the current concrete value *i1 does not appear in concreteVals, add it
      if (i2 == concreteVals.end())
        concreteVals[*i1] = 1;
    }
  }

  // Collect the union or intersection of all results from concreteVals as a set
  std::set<boost::shared_ptr<SgValueExp> > ret;
  for (std::map<boost::shared_ptr<SgValueExp>, size_t>::iterator i =
      concreteVals.begin(); i != concreteVals.end(); i++) {
    // Union: add every key in concreteVals to ret
    if (defaultMayEq)
      ret.insert(i->first);
    // Intersection: only add the keys that appear in every ValueObject in vals
    else if (!defaultMayEq && i->second == vals.size())
      ret.insert(i->first);
  }

  return ret;
}

// Allocates a copy of this object and returns a pointer to it
template<bool defaultMayEq>
ValueObjectPtr CombinedValueObject<defaultMayEq>::copyAOType() const {
  return boost::make_shared<CombinedValueObject>(vals);
}

template<bool defaultMayEq>
std::string CombinedValueObject<defaultMayEq>::str(std::string indent) const {
  ostringstream oss;
  if (vals.size() > 1)
    oss << "[" << (defaultMayEq ? "UnionV" : "IntersectV") << ": ";
  if (vals.size() > 1)
    oss << endl;
  for (list<ValueObjectPtr>::const_iterator v = vals.begin(); v != vals.end();
      ) {
    if (v != vals.begin())
      oss << indent << "&nbsp;&nbsp;&nbsp;&nbsp;";
    oss << (*v)->str(indent + "&nbsp;&nbsp;&nbsp;&nbsp;");
    v++;
    if (v != vals.end())
      oss << endl;
  }
  if (vals.size() > 1)
    oss << "]";

  return oss.str();
}

// GREG: Since Sriram's fix to explicitly declare templated Combined*Objects, we don't need the code below
/ *
 // Create a function that uses examples of combined objects to force the compiler to generate these classes
 static void exampleCombinedValueObjects2(ValueObjectPtr val, std::list<ValueObjectPtr> vals, IntersectValueObject& i, UnionValueObject& u, IntersectValueObject& i2, UnionValueObject& u2);
 static void exampleCombinedValueObjects(ValueObjectPtr val, std::list<ValueObjectPtr> vals)
 {
 IntersectValueObject exampleIntersectObject(val);
 UnionValueObject     exampleUnionObject(val);
 IntersectValueObject exampleIntersectObject2(vals);
 UnionValueObject     exampleUnionObject2(vals);
 exampleCombinedValueObjects2(val, vals, exampleIntersectObject, exampleUnionObject, exampleIntersectObject2, exampleUnionObject2);
 }
 static void exampleCombinedValueObjects2(ValueObjectPtr val, std::list<ValueObjectPtr> vals, IntersectValueObject& i, UnionValueObject& u, IntersectValueObject& i2, UnionValueObject& u2)
 {
 exampleCombinedValueObjects(val, vals);
 }
 * /

// Returns whether all instances of this class form a hierarchy. Every instance of the same
// class created by the same analysis must return the same value from this method!
template<bool defaultMayEq>
bool CombinedValueObject<defaultMayEq>::isHierarchy() const {
  // Combined Values form hierarchy if:
  // - All the sub-Values form hierarchies of their own, AND
  // - The combination is an intersection.
  //   If the combination is a union then consider the following:
  //            MLa     MLb
  //   comb1 = {a,b}, {w, x}
  //   comb2 = {a,b}, {y, z}
  //   Note that MLs from analyses a and b are either identical sets or disjoint
  //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
  //   not hold unless we work out a new hierarchy for MLb.

  // Unions are not hierarchical unless they're singletons
  if (defaultMayEq) {
    if (vals.size() == 1)
      return (*vals.begin())->isHierarchy();
    else
      return false;
  }

  for (list<ValueObjectPtr>::const_iterator ml = vals.begin();
      ml != vals.end(); ml++)
    if (!(*ml)->isHierarchy())
      return false;
  return true;
}

// Returns a key that uniquely identifies this particular AbstractObject in the
// set hierarchy.
template<bool defaultMayEq>
const AbstractionHierarchy::hierKeyPtr& CombinedValueObject<defaultMayEq>::getHierKey() const {
  // The intersection of multiple objects is just a filtering process from the full
  // set of objects to the exact one, with each key in the hierarchy further filtering
  // the set down. As such, a hierarchical key for the intersection of multiple objects
  // is just the concatenation of the keys of all the individual objects.
  //dbg << "CombinedValueObject<defaultMayEq>::getHierKey() isHierKeyCached="<<isHierKeyCached<<endl;
  if (!isHierKeyCached) {
    / *((CombinedValueObject<defaultMayEq>*) this)->cachedHierKey =
     boost::make_shared<AOSHierKey>(
     ((CombinedValueObject<defaultMayEq>*) this)->shared_from_this());

     for (list<ValueObjectPtr>::const_iterator i = vals.begin();
     i != vals.end(); i++) {
     AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<
     AbstractionHierarchy>(*i);
     ROSE_ASSERT(hierIt);

     ((CombinedValueObject<defaultMayEq>*) this)->cachedHierKey->add(
     hierIt->getHierKey()->begin(), hierIt->getHierKey()->end());

     //dbg << "CombinedValueObject<defaultMayEq>::getHierKey() ((CombinedValueObject<defaultMayEq>*)this)->cachedHierKey="<<((CombinedValueObject<defaultMayEq>*)this)->cachedHierKey<<endl;
     }* /
    list<hierKeyPtr> subHierKeys;
    for (list<ValueObjectPtr>::const_iterator i = vals.begin(); i != vals.end(); i++) {
      AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(*i);
      ROSE_ASSERT(hierIt);
      subHierKeys.push_back(hierIt->getHierKey());
    }

    ((CombinedValueObject<defaultMayEq>*) this)->cachedHierKey = boost::make_shared<IntersectHierKey>(subHierKeys);

    ((CombinedValueObject<defaultMayEq>*) this)->isHierKeyCached = true;
  }
  return cachedHierKey;
}*/

/* ###########################
 #### MappedValueObject ####
   ########################### */

//  template<class Key>
//  SgNode* MappedValueObject<Key>::getBase() const {
//    // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
//    SgNode* base = NULL;
//    for (typename map<Key, ValueObjectPtr>::const_iterator it =
//        aoMap.begin(); it != aoMap.end(); ++it) {
//      if (it == aoMap.begin())
//        base = it->second->getBase();
//      else if (base != it->second->getBase())
//        return NULL;
//    }
//
//    return base;
//  }
//
////! Method to add values to the map.
////! Vs that are full are never added to the map.
////! If v_p is FullV or v_p->isFullV=true then mapped V is set to full only if mostAccurate=false.
//  template<class Key>
//  void MappedValueObject<Key>::add(Key key, ValueObjectPtr v_p,
//      PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
//    // If the object is already full don't add anything
//    if (isUnion() && isFullAO(pedge))
//      return;
//
//    // If the v_p is not full add/update the map
//    if (!v_p->isFullAO(pedge)) {
//      aoMap[key] = v_p;
//    } else {
//      nFull++;
//      if (isUnion())
//        setVToFull();
//    }
//  }
//
//  template<class Key>
//  bool MappedValueObject<Key>::mayEqualVWithKey(Key key,
//      const map<Key, ValueObjectPtr>& thatVMap, PartEdgePtr pedge) {
//    typename map<Key, ValueObjectPtr>::const_iterator s_it;
//    s_it = thatVMap.find(key);
//    if (s_it == thatVMap.end())
//      return true;
//    return aoMap[key]->mayEqualAO(s_it->second, pedge);
//  }
//
////! Two Value objects are may equals if there is atleast one execution or sub-exectuion
////! in which they represent the same value.
////! Analyses are conservative as they start with full set of executions.
////! Dataflow facts (predicates) shrink the set of sub-executions.
////! We do not explicity store set of sub-executions and they are described
////! by the abstract objects computed from dataflow fact exported by the analysis.
////! Unless the analyses discover otherwise, the conservative answer for mayEqualV is true.
////! Mapped Values are keyed using either ComposedAnalysis* or PartEdgePtr.
////! Each keyed Value object correspond to some dataflow facts computed by Key=Analysis* or
////! computed at Key=PartEdgePtrthat describes some sets of executions.
////! MayEquality check on mapped Value is performed on intersection of sub-executions
////! or union of sub-executions over the keyed Value objects.
//  template<class Key>
//  bool MappedValueObject<Key>::mayEqualAO(ValueObjectPtr thatV,
//      PartEdgePtr pedge) {
//    boost::shared_ptr<MappedValueObject<Key> > thatV_p =
//        boost::dynamic_pointer_cast<MappedValueObject<Key> >(
//            thatV);
//    assert(thatV_p);
//
//    // This object denotes full set of Values (full set of executions)
//    if (isFullAO(pedge))
//      return true;
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return false;
//
//    // presence of one more full objects will result in full set over union
//    if (isUnion() && nFull > 0)
//      return true;
//
//    // Two cases reach here [1] isUnion()=true && nFull_V=0 [2] intersect=true && nFullV=0 or nFull_V!=0.
//    // For both cases iterate on the Value map and discharge the mayEqualV query to individual objects
//    // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
//    const map<Key, ValueObjectPtr> thatVMap = thatV_p->getValuesMap();
//    typename map<Key, ValueObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      bool isMayEq = mayEqualVWithKey(it->first, thatVMap, pedge);
//
//      // 1. Union of sub-executions and the object does not contain any full objects.
//      // If the discharged query comes back as true for this case then we have found atleast one execution
//      // under which the two objects are same and the set can only grow and the result of this query is not going
//      // to change due to union.
//      // If false we iterate further as any Value can add more executions under which the objects are may equals.
//      if (isUnion() && isMayEq == true)
//        return true;
//
//      // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
//      // The sub-executions are intersected and therefore it does not matter if we have full objects.
//      // If the discharged query returns false then return false.
//      // We did not find one execution in which the two objects are may equals.
//      // Note that set of executions are contained over keyed objects (analyses are conservative).
//      // This set only shrinks during intersection and it is not going to affect the result of this query.
//      // If it returns true iterate further as some executions corresponding to true may be dropped.
//      else if (isIntersection() && isMayEq == false)
//        return false;
//    }
//
//    // All the keyed objects returned false for the discharged query under union.
//    // We haven't found a single execution under which the two objects are may equals.
//    if (isUnion())
//      return false;
//    // All the keyed objects returned true for the discharged query under intersection.
//    // We have atleast one execution in common in which the two objects are may equals.
//    else if (isIntersection())
//      return true;
//    else
//      assert(0);
//  }
//
//  template<class Key>
//  bool MappedValueObject<Key>::mustEqualVWithKey(Key key,
//      const map<Key, ValueObjectPtr>& thatVMap, PartEdgePtr pedge) {
//    typename map<Key, ValueObjectPtr>::const_iterator s_it;
//    s_it = thatVMap.find(key);
//    if (s_it == thatVMap.end())
//      return false;
//    return aoMap[key]->mustEqualAO(s_it->second, pedge);
//  }
//
////! Two Value objects are must equals if they represent the same value
////! on all executions.
////! Analyses are conservative as they start with full set of executions.
////! Dataflow facts (predicates) shrink the set of sub-executions.
////! We do not explicity store set of sub-executions and they are described
////! by the abstract objects computed from dataflow fact exported by the analysis.
////! Unless the analyses discover otherwise conservative answer for mustEqualV is false.
////! Mapped Values are keyed using either ComposedAnalysis* or PartEdgePtr.
////! Each keyed Value object correspond to some dataflow facts computed by Key=Analysis* or
////! computed at Key=PartEdgePtrthat describes some sets of executions.
////! MustEquality check on mapped Value is performed on intersection (mostAccurate=true) of sub-executions
////! or union (mostAccurate=false) of sub-executions over the keyed Value objects.
//  template<class Key>
//  bool MappedValueObject<Key>::mustEqualAO(ValueObjectPtr thatV,
//      PartEdgePtr pedge) {
//    boost::shared_ptr<MappedValueObject<Key> > thatV_p =
//        boost::dynamic_pointer_cast<MappedValueObject<Key> >(
//            thatV);
//    assert(thatV_p);
//
//    // This object denotes full set of Value (full set of executions)
//    if (isFullAO(pedge))
//      return false;
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return false;
//
//    // presence of one more full objects will result in full set over union
//    if (isUnion() && nFull > 0)
//      return true;
//
//    // Two cases reach here [1] isUnion()=true && nFull_V=0 [2] intersect=true && nFullV=0 or nFull_V!=0.
//    // For both cases iterate on the Value map and discharge the mayEqualV query to individual objects
//    // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
//    const map<Key, ValueObjectPtr> thatVMap = thatV_p->getValuesMap();
//    typename map<Key, ValueObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      bool isMustEq = mustEqualVWithKey(it->first, thatVMap, pedge);
//
//      // 1. Union of sub-executions and the object does not contain any full objects
//      // If the discharged query comes back as false for this case then we have found atleast one execution
//      // under which the two objects are not same and the set can only grow and the result of this query is not going
//      // to change due to union.
//      // If it returns true we iterate further as any Value can add more executions under which the objects are not must equals.
//      if (isUnion() && isMustEq == false)
//        return false;
//
//      // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
//      // The sub-executions are intersected and therefore it does not matter if we have full objects.
//      // If the discharged query returns true then return true.
//      // Under all sub-executions (corresponding to the Value) the two objects must equal.
//      // Note that set of executions are contained over keyed objects as the analyses are conservative.
//      // This set only shrinks during intersection and it is not going to affect the result of this query.
//      // If it returns false iterate further as some executions corresponding to false may be dropped.
//      else if (isIntersection() && isMustEq == true)
//        return true;
//    }
//
//    // All the keyed objects returned true for the discharged query under union.
//    // We haven't found a single execution under which the two objects are not equal.
//    if (isUnion())
//      return true;
//    // All the keyed objects returned false for the discharged query under intersection.
//    // We have atleast one execution in common in which the two objects are not equal.
//    else if (isIntersection())
//      return false;
//    else
//      assert(0);
//  }
//
////! Discharge the query to the corresponding VO
////! If key not found in thatVMap return false
//  template<class Key>
//  bool MappedValueObject<Key>::equalSetVWithKey(Key key,
//      const map<Key, ValueObjectPtr>& thatVMap, PartEdgePtr pedge) {
//    typename map<Key, ValueObjectPtr>::const_iterator s_it;
//    s_it = thatVMap.find(key);
//    if (s_it == thatVMap.end())
//      return false;
//    return aoMap[key]->equalSetAO(s_it->second, pedge);
//  }
//
////! Two objects are equal sets if they denote the same set of values.
////! The boolean parameter mostAccurate is not releveant as this query is not
////! answered based on union or intersection of sub-executions.
////! Simply discharge the queries to all keyed Value objects
////! If all the discharged queries come back equal then the two objects are equal otherwise not.
//  template<class Key>
//  bool MappedValueObject<Key>::equalSetAO(ValueObjectPtr thatV,
//      PartEdgePtr pedge) {
//    boost::shared_ptr<MappedValueObject<Key> > thatV_p =
//        boost::dynamic_pointer_cast<MappedValueObject<Key> >(
//            thatV);
//    assert(thatV_p);
//
//    // This object denotes full set of Values (full set of executions)
//    if (isFullAO(pedge))
//      return thatV_p->isFullAO(pedge);
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return thatV_p->isEmptyAO(pedge);
//
//    const map<Key, ValueObjectPtr> thatVMap = thatV_p->getValuesMap();
//    typename map<Key, ValueObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      // break even if one of them returns false
//      if (equalSetVWithKey(it->first, thatVMap, pedge) == false)
//        return false;
//    }
//
//    return true;
//  }
//
////! Discharge the query to the corresponding VO
////! If key not found in thatVMap return true as the
////! keyed object on thatVMap denotes full set
//  template<class Key>
//  bool MappedValueObject<Key>::subSetVWithKey(Key key,
//      const map<Key, ValueObjectPtr>& thatVMap, PartEdgePtr pedge) {
//    typename map<Key, ValueObjectPtr>::const_iterator s_it;
//    s_it = thatVMap.find(key);
//    if (s_it == thatVMap.end())
//      return true;
//    return aoMap[key]->subSetAO(s_it->second, pedge);
//  }
//
////! This object is a non-strict subset of the other if the set of values denoted by this
////! is a subset of the set of values denoted by that.
////! The boolean parameter mostAccurate is not releveant as this query is not
////! answered based on union or intersection of sub-executions.
////! Simply discharge the queries to all keyed Value objects
////! If all the discharged queries come back true then this is a subset of that otherwise not.
//  template<class Key>
//  bool MappedValueObject<Key>::subSetAO(ValueObjectPtr thatV,
//      PartEdgePtr pedge) {
//    boost::shared_ptr<MappedValueObject<Key> > thatV_p =
//        boost::dynamic_pointer_cast<MappedValueObject<Key> >(
//            thatV);
//    assert(thatV_p);
//
//    // This object denotes full set of Values (full set of executions)
//    if (isFullAO(pedge))
//      return thatV_p->isFullAO(pedge);
//
//    // denotes empty set
//    // thatV could be empty or non-empty eitherway this will be a non-strict subset of that.
//    if (isEmptyAO(pedge))
//      return true;
//
//    // If both objects have the same keys discharge
//    // If this object has a key and that does not then
//    // the keyed object is subset of that (return true) implemented by subsetVWithKey
//    // If any of the discharged query return false then return false.
//    const map<Key, ValueObjectPtr> thatVMap = thatV_p->getValuesMap();
//    typename map<Key, ValueObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      // break even if one of them returns false
//      if (subSetVWithKey(it->first, thatVMap, pedge) == false)
//        return false;
//    }
//
//    // If this object doesn't have the key and that object has the key then
//    // return false as this object has full object mapped to the key
//    typename map<Key, ValueObjectPtr>::const_iterator c_it;
//    for (c_it = thatVMap.begin(); c_it != thatVMap.end() && (nFull != 0);
//        ++c_it) {
//      if (aoMap.find(c_it->first) == aoMap.end())
//        return false;
//    }
//
//    return true;
//  }
//
//  template<class Key>
//  bool MappedValueObject<Key>::isLiveAO(PartEdgePtr pedge) {
//    // If this object is full return the conservative answer
//    if (isFullAO(pedge))
//      return true;
//
//    // If it has one or more full objects added to it
//    // and if the object has mostAccurate=false then return true (weakest answer)
//    if (nFull > 0 && isUnion())
//      return true;
//
//    // 1. This object may have have one or more full objects under intersection
//    // 2. This object doesnt have any full objects added to it under union
//    // Under both cases the answer is based on how individual analysis respond to the query
//    typename map<Key, ValueObjectPtr>::iterator it = aoMap.begin();
//    for (; it != aoMap.end(); ++it) {
//      bool isLive = it->second->isLiveAO(pedge);
//      if (isUnion() && isLive == true)
//        return true;
//      else if (isIntersection() && isLive == false)
//        return false;
//    }
//
//    // leftover cases
//    if (isUnion())
//      return false;
//    else if (isIntersection())
//      return true;
//    else
//      assert(0);
//  }
//
////! meetUpdateV performs the join operation of abstractions of two value objects
//  template<class Key>
//  bool MappedValueObject<Key>::meetUpdateAO(ValueObjectPtr that,
//      PartEdgePtr pedge) {
//    boost::shared_ptr<MappedValueObject<Key> > thatV_p =
//        boost::dynamic_pointer_cast<MappedValueObject<Key> >(
//            that);
//    assert(thatV_p);
//
//    // if this object is already full
//    if (isFullAO(pedge))
//      return false;
//
//    // If that object is full set this object to full
//    if (thatV_p->isFullAO(pedge)) {
//      nFull++;
//      setVToFull();
//      return true;
//    }
//
//    // Both objects are not full
//    const map<Key, ValueObjectPtr> thatVMap = thatV_p->getValuesMap();
//
//    typename map<Key, ValueObjectPtr>::iterator it = aoMap.begin();
//    typename map<Key, ValueObjectPtr>::const_iterator s_it; // search iterator for thatVMap
//
//    bool modified = false;
//    while (it != aoMap.end()) {
//      s_it = thatVMap.find(it->first);
//      // If two objects have the same key then discharge meetUpdate to the corresponding keyed Value objects
//      if (s_it != thatVMap.end()) {
//        modified = (it->second)->meetUpdateAO(s_it->second, pedge) || modified;
//      }
//
//      // Remove the current Value object (current iterator it) from the map if the mapepd object is full.
//      // Two cases under which the current Value object can be full.
//      // (1) If current key is not found in thatVMap then the mapped object
//      // in thatVMap is full and the meetUpdate of the current V with that is also full.
//      // (2) meetUpdateV above of the two keyed objects resulted in this mapped object being full.
//      // Under both cases remove the mapped ml from this map
//      if (s_it == thatVMap.end() || (it->second)->isFullAO(pedge)) {
//        // Current mapped Value object has become full as a result of (1) or (2).
//        // Remove the item from the map.
//        // Note that post-increment which increments the iterator and returns the old value for deletion.
//        aoMap.erase(it++);
//        nFull++;
//        modified = true;
//
//        // If union then set this entire object to full and return
//        if (isUnion()) {
//          setVToFull();
//          return true;
//        }
//      } else
//        ++it;
//    }
//    return modified;
//  }
//
////! Method that sets this mapped object to full
//  template<class Key>
//  void MappedValueObject<Key>::setVToFull() {
//    assert(nFull > 0);
//    if (aoMap.size() > 0)
//      aoMap.clear();
//  }
//
//  template<class Key>
//  bool MappedValueObject<Key>::isFullAO() {
//    if (nFull > 0 && aoMap.size() == 0)
//      return true;
//    return false;
//  }
//
//  template<class Key>
//  bool MappedValueObject<Key>::isFullAO(PartEdgePtr pedge) {
//    return isFullAO();
//  }
//
//  template<class Key>
//  bool MappedValueObject<Key>::isEmptyAO() {
//    if (nFull == 0 && aoMap.size() == 0)
//      return true;
//    return false;
//  }
//
//  template<class Key>
//  bool MappedValueObject<Key>::isEmptyAO(PartEdgePtr pedge) {
//    return isEmptyAO();
//  }
//
//  template<class Key>
//  bool MappedValueObject<Key>::isConcrete() {
//
//    if (isFullAO())
//      return false;
//    if (isEmptyAO())
//      return false;
//    if (isUnion() && nFull > 0)
//      return false;
//
//    typename map<Key, ValueObjectPtr>::iterator it = aoMap.begin();
//    for (; it != aoMap.end(); ++it) {
//      bool isConc = it->second->isConcrete();
//      // we have atleast one that is not concrete under union
//      if (isUnion() && !isConc)
//        return false;
//      // we have atleast one that is concrete under intersection
//      else if (isIntersection() && isConc)
//        return true;
//    }
//
//    // All the objects are concrete (return true above) union
//    if (isUnion())
//      return true;
//    // All the objects not concrete (return false above) under intersection
//    else if (isIntersection())
//      return false;
//    else
//      assert(0);
//  }
//
//// Returns the number of concrete values in this set
//  template<class Key>
//  int MappedValueObject<Key>::concreteSetSize() {
//    assert(isConcrete());
//    return getConcreteValue().size();
//  }
//
//  template<class Key>
//  SgType* MappedValueObject<Key>::getConcreteType() {
//    assert(isConcrete());
//    typename map<Key, ValueObjectPtr>::iterator it = aoMap.begin();
//    SgType* c_type = it->second->getConcreteType();
//    // assert that all other objects have the same type
//    for (++it; it != aoMap.end(); ++it) {
//      SgType* votype = it->second->getConcreteType();
//      assert(c_type == votype);
//    }
//    return c_type;
//  }
//
//  template<class Key>
//  set<boost::shared_ptr<SgValueExp> > MappedValueObject<Key>::getConcreteValue() {
//    assert(isConcrete());
//    // If this is a union type (defaultMayEq=true), the result is the Union of the sets returned by getConcrete() on all the memRegions.
//    // If this is an intersection type (defaultMayEq=false), an object is their Intersection.
//
//    // Maps each concrete value to the number of elements in aoMap for which it was returned
//    std::map<boost::shared_ptr<SgValueExp>, size_t> concreteVals;
//    for (typename map<Key, ValueObjectPtr>::iterator v_it = aoMap.begin();
//        v_it != aoMap.end(); ++v_it) {
//      // Iterate through the current sub-MemRegion's concrete values and increment each
//      // concrete value's counter in concreteMRs.
//      std::set<boost::shared_ptr<SgValueExp> > c_valueSet =
//          v_it->second->getConcreteValue();
//      for (std::set<boost::shared_ptr<SgValueExp> >::iterator s_it =
//          c_valueSet.begin(); s_it != c_valueSet.end(); ++s_it) {
//        map<boost::shared_ptr<SgValueExp>, size_t>::iterator c_it =
//            concreteVals.begin();
//        for (; c_it != concreteVals.end(); ++c_it) {
//          // If we've found the same value, increment its counter
//          if (ValueObject::equalValueExp(c_it->first.get(), (*s_it).get())) {
//            c_it->second++;
//            break;
//          }
//        }
//
//        // If we did not find the value, add it to concreteVals;
//        if (c_it == concreteVals.end())
//          concreteVals[*s_it] = 1;
//      }
//    }
//
//    // Collect the union or intersection of all results from concreteMRs as a set
//    std::set<boost::shared_ptr<SgValueExp> > ret;
//    for (std::map<boost::shared_ptr<SgValueExp>, size_t>::iterator i =
//        concreteVals.begin(); i != concreteVals.end(); i++) {
//      // Union: add every key in concreteMRs to ret
//      if (isUnion())
//        ret.insert(i->first);
//      // Intersection: only add the keys that appear in every MemRegion in memRegions
//      else if (isIntersection() && i->second == aoMap.size())
//        ret.insert(i->first);
//    }
//
//    return ret;
//  }
//
//  template<class Key>
//  ValueObjectPtr MappedValueObject<Key>::copyAO() const {
//    return boost::make_shared<MappedValueObject<Key> >(*this);
//  }
//
//  template<class Key>
//  string MappedValueObject<Key>::str(string indent) const {
//    ostringstream oss;
//    oss << "<table border=\"1\">";
//    oss << "<tr>";
//    oss << "<th>"
//        << (isUnion() ? "UnionMappedValueObject:" : "IntersectMappedValueObject:")
//        << "</th>";
//    if (nFull > 0 && aoMap.size() == 0)
//      oss << "<th> Full </th> </tr>";
//    else if (nFull == 0 && aoMap.size() == 0)
//      oss << "<th> Empty </th> </tr>";
//    else {
//      oss << "</tr>";
//      typename map<Key, ValueObjectPtr>::const_iterator it = aoMap.begin();
//      for (; it != aoMap.end(); ++it) {
//        oss << "<tr>";
//        oss << "<td>" << (it->first)->str(indent) << "</td>";
//        oss << "<td>" << (it->second)->str(indent) << "</td>";
//        oss << "</tr>";
//      }
//    }
//    oss << "</table>";
//    return oss.str();
//  }
//
//// Returns whether all instances of this class form a hierarchy. Every instance of the same
//// class created by the same analysis must return the same value from this method!
//  template<class Key>
//  bool MappedValueObject<Key>::isHierarchy() const {
//    // Combined Values form hierarchy if:
//    // - All the sub-Values form hierarchies of their own, AND
//    // - The combination is an intersection.
//    //   If the combination is a union then consider the following:
//    //            MLa     MLb
//    //   comb1 = {a,b}, {w, x}
//    //   comb2 = {a,b}, {y, z}
//    //   Note that MLs from analyses a and b are either identical sets or disjoint
//    //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
//    //   not hold unless we work out a new hierarchy for MLb.
//
//    // Unions are not hierarchical unless they're singletons
//    if (isUnion()) {
//      if (aoMap.size() == 1)
//        return aoMap.begin()->second->isHierarchy();
//      else
//        return false;
//    }
//
//    typename map<Key, ValueObjectPtr>::const_iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it)
//      if (!it->second->isHierarchy())
//        return false;
//    return true;
//  }
//
//// Returns a key that uniquely identifies this particular AbstractObject in the
//// set hierarchy.
//  template<class Key>
//  const AbstractionHierarchy::hierKeyPtr& MappedValueObject<Key>::getHierKey() const {
//    // The intersection of multiple objects is just a filtering process from the full
//    // set of objects to the exact one, with each key in the hierarchy further filtering
//    // the set down. As such, a hierarchical key for the intersection of multiple objects
//    // is just the concatenation of the keys of all the individual objects.
//    if (!isHierKeyCached) {
//      /*((MappedValueObject<Key>*) this)->cachedHierKey =
//       boost::make_shared<AOSHierKey>(
//       ((MappedValueObject<Key>*) this)->shared_from_this());
//
//       typename map<Key, ValueObjectPtr>::const_iterator it;
//       for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//       AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<
//       AbstractionHierarchy>(it->second);
//       ROSE_ASSERT(hierIt);
//
//       ((MappedValueObject<Key>*) this)->cachedHierKey->add(
//       hierIt->getHierKey()->begin(), hierIt->getHierKey()->end());
//       }*/
//      map<Key, hierKeyPtr> subHierKeys;
//      for(typename map<Key, ValueObjectPtr>::const_iterator it = aoMap.begin();
//          it != aoMap.end(); ++it) {
//        AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(it->second);
//        ROSE_ASSERT(hierIt);
//        subHierKeys[it->first] = hierIt->getHierKey();
//      }
//
//      ((MappedValueObject<Key>*) this)->cachedHierKey =
//          boost::make_shared<IntersectMappedHierKey<Key> >(subHierKeys);
//
//      ((MappedValueObject<Key>*) this)->isHierKeyCached = true;
//    }
//    return cachedHierKey;
//  }

template<class Key>
SgType* MappedValueObject<Key>::getConcreteType() {
  assert(MappedValueObject<Key>::isConcrete());
  typename map<Key, ValueObjectPtr>::iterator it =
      MappedAbstractObject<Key, ValueObject, AbstractObject::Value, MappedValueObject<Key> >::aoMap.begin();
  SgType* c_type = it->second->getConcreteType();
  // assert that all other objects have the same type
  for (++it; it != MappedAbstractObject<Key, ValueObject, AbstractObject::Value, MappedValueObject<Key> >::aoMap.end(); ++it) {
    SgType* votype = it->second->getConcreteType();
    assert(c_type == votype);
  }
  return c_type;
}

template<class Key>
set<boost::shared_ptr<SgValueExp> > MappedValueObject<Key>::getConcreteValue() {
  if(!MappedAbstractObject<Key, ValueObject, AbstractObject::Value, MappedValueObject<Key> >::isConcrete()) assert(0);
  // If this is a union type (defaultMayEq=true), the result is the Union of the sets returned by getConcrete() on all the memRegions.
  // If this is an intersection type (defaultMayEq=false), an object is their Intersection.

  // Maps each concrete value to the number of elements in aoMap for which it was returned
  std::map<boost::shared_ptr<SgValueExp>, size_t> concreteVals;
  for(typename map<Key, ValueObjectPtr>::iterator v_it =
      MappedAbstractObject<Key, ValueObject, AbstractObject::Value, MappedValueObject<Key> >::aoMap.begin();
      v_it != MappedAbstractObject<Key, ValueObject, AbstractObject::Value, MappedValueObject<Key> >::aoMap.end(); ++v_it) {
    // Iterate through the current sub-MemRegion's concrete values and increment each
    // concrete value's counter in concreteMRs.
    std::set<boost::shared_ptr<SgValueExp> > c_valueSet =
        v_it->second->getConcreteValue();
    for (std::set<boost::shared_ptr<SgValueExp> >::iterator s_it =
        c_valueSet.begin(); s_it != c_valueSet.end(); ++s_it) {
      map<boost::shared_ptr<SgValueExp>, size_t>::iterator c_it =
          concreteVals.begin();
      for (; c_it != concreteVals.end(); ++c_it) {
        // If we've found the same value, increment its counter
        if (ValueObject::equalValueExp(c_it->first.get(), (*s_it).get())) {
          c_it->second++;
          break;
        }
      }

      // If we did not find the value, add it to concreteVals;
      if (c_it == concreteVals.end())
        concreteVals[*s_it] = 1;
    }
  }

  // Collect the union or intersection of all results from concreteMRs as a set
  std::set<boost::shared_ptr<SgValueExp> > ret;
  for (std::map<boost::shared_ptr<SgValueExp>, size_t>::iterator i =
      concreteVals.begin(); i != concreteVals.end(); i++) {
    // Union: add every key in concreteMRs to ret
    if (MappedAbstractObject<Key, ValueObject, AbstractObject::Value, MappedValueObject<Key> >::isUnion())
      ret.insert(i->first);
    // Intersection: only add the keys that appear in every MemRegion in memRegions
    else if (MappedAbstractObject<Key, ValueObject, AbstractObject::Value, MappedValueObject<Key> >::isIntersection() &&
             i->second == MappedAbstractObject<Key, ValueObject, AbstractObject::Value, MappedValueObject<Key> >::aoMap.size())
      ret.insert(i->first);
  }

  return ret;
}

/* ##################################
   #### PartEdgeUnionValueObject ####
   ################################## */

PartEdgeUnionValueObject::PartEdgeUnionValueObject() :
    ValueObject(NULL) {
}

PartEdgeUnionValueObject::PartEdgeUnionValueObject(
    const PartEdgeUnionValueObject& thatV) :
    ValueObject(thatV), unionV_p(thatV.copyAOType()) {
}

SgNode* PartEdgeUnionValueObject::getBase() const {
  return unionV_p->getBase();
}

void PartEdgeUnionValueObject::add(ValueObjectPtr v_p, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is the very first object
  if (!unionV_p)
    unionV_p = v_p->copyAOType();
  // If Full return without adding
  else if (isFullAO(pedge))
    return;
  // Else meetUpdate with the existing unionV_p
  else
    unionV_p->meetUpdateAO(v_p, pedge);
}

bool PartEdgeUnionValueObject::mayEqualAO(ValueObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionValueObject> thatV_p =
      boost::dynamic_pointer_cast<PartEdgeUnionValueObject>(that);
  assert(thatV_p);
  return unionV_p->mayEqualAO(thatV_p->getUnionAO(), pedge);
}

bool PartEdgeUnionValueObject::mustEqualAO(ValueObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionValueObject> thatV_p =
      boost::dynamic_pointer_cast<PartEdgeUnionValueObject>(that);
  assert(thatV_p);
  return unionV_p->mustEqualAO(thatV_p->getUnionAO(), pedge);
}

bool PartEdgeUnionValueObject::equalSetAO(ValueObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionValueObject> thatV_p =
      boost::dynamic_pointer_cast<PartEdgeUnionValueObject>(that);
  assert(thatV_p);
  return unionV_p->equalSetAO(thatV_p->getUnionAO(), pedge);
}

bool PartEdgeUnionValueObject::subSetAO(ValueObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionValueObject> thatV_p =
      boost::dynamic_pointer_cast<PartEdgeUnionValueObject>(that);
  assert(thatV_p);
  return unionV_p->subSetAO(thatV_p->getUnionAO(), pedge);
}

bool PartEdgeUnionValueObject::meetUpdateAO(ValueObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionValueObject> thatV_p =
      boost::dynamic_pointer_cast<PartEdgeUnionValueObject>(that);
  assert(thatV_p);
  return unionV_p->meetUpdateAO(thatV_p->getUnionAO(), pedge);
}

bool PartEdgeUnionValueObject::isLiveAO(PartEdgePtr pedge) {
  return unionV_p->isLiveAO(pedge);
}

bool PartEdgeUnionValueObject::isFullAO(PartEdgePtr pedge) {
  return unionV_p->isFullAO(pedge);
}

bool PartEdgeUnionValueObject::isEmptyAO(PartEdgePtr pedge) {
  return unionV_p->isEmptyAO(pedge);
}

ValueObjectPtr PartEdgeUnionValueObject::copyAOType() const {
  return boost::make_shared<PartEdgeUnionValueObject>(*this);
}

void PartEdgeUnionValueObject::setAOToFull() {
  unionV_p = boost::make_shared<FullValueObject>();
}

bool PartEdgeUnionValueObject::isConcrete() {
  return unionV_p->isConcrete();
}

// Returns the number of concrete values in this set
int PartEdgeUnionValueObject::concreteSetSize() {
  return unionV_p->concreteSetSize();
}

SgType* PartEdgeUnionValueObject::getConcreteType() {
  return unionV_p->getConcreteType();
}

set<boost::shared_ptr<SgValueExp> > PartEdgeUnionValueObject::getConcreteValue() {
  return unionV_p->getConcreteValue();
}

string PartEdgeUnionValueObject::str(string indent) const {
  ostringstream oss;
  oss << "[UnionV=" << unionV_p->str(indent) << "]";
  return oss.str();
}

/* ###########################
   ##### MemRegionObject #####
   ########################### */

MemRegionObjectPtr NULLMemRegionObject;

// General version of mayEqual and mustEqual that accounts for framework details before routing the call to the
// derived class' may/mustEqual check. Specifically, it checks may/must equality with respect to ExprObj and routes
// the call through the composer to make sure the may/mustEqual call gets the right PartEdge
bool MemRegionObject::mayEqual(MemRegionObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If either this or that are FuncResultMemRegionObject, they mayEqual iff they correspond to the same function
  FuncResultMemRegionObject* frmlcoThis =
      dynamic_cast<FuncResultMemRegionObject*>(this);
  FuncResultMemRegionObjectPtr frmlcoThat = boost::dynamic_pointer_cast<
      FuncResultMemRegionObject>(that);
  if (frmlcoThis)
    return frmlcoThis->mayEqualAO(frmlcoThat, pedge);
  else if (frmlcoThat)
    return false;

  // If both this and that are both expression objects or both not expression objects, use the
  // derived class' equality check
  //dbg << "MemRegionObject::mayEqual() dynamic_cast<const ExprObj*>(this)="<<dynamic_cast<const ExprObj*>(this)<<" dynamic_cast<const ExprObj*>(o.get())="<<dynamic_cast<const ExprObj*>(o.get())<<endl;

  // GB 2013-09-16 - Commenting this out since we currently don't have code to return specifically expression
  //                 regions but this capability may be brought back for performance reasons (equality checks
  //                 can be sped up with we keep track of expression objects, for which identity is easy to check)
  // if((dynamic_cast<const ExprObj*>(this)  && dynamic_cast<const ExprObj*>(that.get())) ||
  //    (!dynamic_cast<const ExprObj*>(this) && !dynamic_cast<const ExprObj*>(that.get())))
  // { return mayEqualAO(that, pedge); }
  /*if(mayEqualCache.find(that) == mayEqualCache.end())
   // Route the check through the composer, which makes sure to call the derived class' check at the correct PartEdge
   mayEqualCache[that] = comp->mayEqualAO(shared_from_this(), that, pedge, analysis);
   return mayEqualCache[that];*/
  return comp->mayEqualMR(shared_from_this(), that, pedge, analysis);
}

// General version of mayEqual and mustEqual that accounts for framework details before routing the call to the
// derived class' may/mustEqual check. Specifically, it checks may/must equality with respect to ExprObj and routes
// the call through the composer to make sure the may/mustEqual call gets the right PartEdge
bool MemRegionObject::mustEqual(MemRegionObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If either this or that are FuncResultMemRegionObject, they mustEqual iff they both are
  FuncResultMemRegionObject* frmlcoThis =
      dynamic_cast<FuncResultMemRegionObject*>(this);
  FuncResultMemRegionObjectPtr frmlcoThat = boost::dynamic_pointer_cast<
      FuncResultMemRegionObject>(that);
  if (frmlcoThis)
    return frmlcoThat->mustEqualAO(frmlcoThat, pedge);
  else if (frmlcoThat)
    return frmlcoThis;

  // Efficiently compute must equality for simple cases where the two MemRegionObjects correspond to the same SgNode
  //if(AbstractObject::mustEqualExpr(boost::static_pointer_cast<AbstractObject>(that), pedge)) return true;

  // GB 2013-09-16 - Commenting this out since we currently don't have code to return specifically expression
  //                 regions but this capability may be brought back for performance reasons (equality checks
  //                 can be sped up with we keep track of expression objects, for which identity is easy to check)
  // // If both this and that are both expression objects or both not expression objects, use the
  // // derived class' equality check
  // //dbg << "MemRegionObject::mustEqual() dynamic_cast<const ExprObj*>(this)="<<dynamic_cast<const ExprObj*>(this)<<"="<<const_cast<MemRegionObject*>(this)->str("")<<endl;
  // //dbg << "&nbsp;&nbsp;&nbsp;&nbsp;dynamic_cast<const ExprObj*>(o.get())="<<dynamic_cast<const ExprObj*>(o.get())<<"="<<o->str("")<<endl;
  // if((dynamic_cast<const ExprObj*>(this)  && dynamic_cast<const ExprObj*>(that.get())) ||
  //    (!dynamic_cast<const ExprObj*>(this) && !dynamic_cast<const ExprObj*>(that.get())))
  // //{ return mustEqualAO(that, pedge); }

  /*if(mustEqualCache.find(that) == mustEqualCache.end())
   // Route the check through the composer, which makes sure to call the derived class' check at the correct PartEdge
   mustEqualCache[that] = comp->mustEqualAO(shared_from_this(), that, pedge, analysis);
   return mustEqualCache[that];*/
  return comp->mustEqualMR(shared_from_this(), that, pedge, analysis);
}

// Check whether that is a MemRegionObject and if so, call the version of mayEqual specific to MemRegionObjects
bool MemRegionObject::mayEqual(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  MemRegionObjectPtr mo = boost::dynamic_pointer_cast<MemRegionObject>(that);
  if (mo)
    return mayEqual(mo, pedge, comp, analysis);
  else
    return false;
}

// Check whether that is a MemRegionObject and if so, call the version of mustEqual specific to MemRegionObjects
bool MemRegionObject::mustEqual(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  //if(AbstractObject::mustEqualExpr(that, pedge)) return true;

  MemRegionObjectPtr mo = boost::dynamic_pointer_cast<MemRegionObject>(that);
  if (mo)
    return mustEqual(mo, pedge, comp, analysis);
  else
    return false;
}

bool MemRegionObject::equalSet(MemRegionObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If either this or that are FuncResultMemRegionObject, they denote equal sets iff they correspond to the same function
  FuncResultMemRegionObject* frmlcoThis =
      dynamic_cast<FuncResultMemRegionObject*>(this);
  FuncResultMemRegionObjectPtr frmlcoThat = boost::dynamic_pointer_cast<
      FuncResultMemRegionObject>(that);
  if (frmlcoThis)
    return frmlcoThis->equalSetAO(frmlcoThat, pedge);
  else if (frmlcoThat)
    return false;

  /*if(equalSetCache.find(that) == equalSetCache.end())
   equalSetCache[that] = comp->equalSetAO(shared_from_this(), that, pedge, analysis);
   return equalSetCache[that];*/
  return comp->equalSetMR(shared_from_this(), that, pedge, analysis);
}

bool MemRegionObject::subSet(MemRegionObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If either this or that are FuncResultMemRegionObject, they denote equal sets iff they correspond to the same function
  FuncResultMemRegionObject* frmlcoThis =
      dynamic_cast<FuncResultMemRegionObject*>(this);
  FuncResultMemRegionObjectPtr frmlcoThat = boost::dynamic_pointer_cast<
      FuncResultMemRegionObject>(that);
  if (frmlcoThis)
    return frmlcoThis->subSetAO(frmlcoThat, pedge);
  else if (frmlcoThat)
    return false;

  /*if(subSetCache.find(that) == subSetCache.end())
   subSetCache[that] = comp->subSetAO(shared_from_this(), that, pedge, analysis);
   return subSetCache[that];*/
  return comp->subSetMR(shared_from_this(), that, pedge, analysis);
}

bool MemRegionObject::equalSet(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  MemRegionObjectPtr co = boost::dynamic_pointer_cast<MemRegionObject>(o);
  if (co)
    return equalSet(co, pedge, comp, analysis);
  else
    return false;
}

bool MemRegionObject::subSet(AbstractObjectPtr o, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  MemRegionObjectPtr co = boost::dynamic_pointer_cast<MemRegionObject>(o);
  if (co)
    return subSet(co, pedge, comp, analysis);
  else
    return false;
}

// General version of isLive that accounts for framework details before routing the call to the derived class'
// isLiveML check. Specifically, it routes the call through the composer to make sure the isLiveML call gets the
// right PartEdge
bool MemRegionObject::isLive(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  return comp->isLiveMR(shared_from_this(), pedge, analysis);
}

// General version of meetUpdate() that accounts for framework details before routing the call to the derived class'
// meetUpdateMR check. Specifically, it routes the call through the composer to make sure the meetUpdateMR
// call gets the right PartEdge
bool MemRegionObject::meetUpdate(MemRegionObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  return comp->meetUpdateMR(shared_from_this(), that, pedge, analysis);
}

bool MemRegionObject::meetUpdate(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  MemRegionObjectPtr mr = boost::dynamic_pointer_cast<MemRegionObject>(that);
  assert(mr);
  return meetUpdate(mr, pedge, comp, analysis);
}

// General version of isFull/isEmpty that accounts for framework details before routing the call to the
// derived class' isFullMR/isEmptyMR check. Specifically, it routes the call through the composer to make
// sure the isFullMR/isEmptyMR call gets the right PartEdge
bool MemRegionObject::isFull(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  return comp->isFullMR(shared_from_this(), pedge, analysis);
}

bool MemRegionObject::isEmpty(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  return comp->isEmptyMR(shared_from_this(), pedge, analysis);
}

// General version of getRegionSize that accounts for framework details before routing the call to the
// derived class' getRegionSizeAO(). Specifically, it routes the call through the composer to make
// sure the getRegionSize call gets the right PartEdge
ValueObjectPtr MemRegionObject::getRegionSize(PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  return comp->getRegionSizeMR(shared_from_this(), pedge, analysis);
}

/* #####################################
   ##### FuncResultMemRegionObject #####
   ##################################### */

// Special MemRegionObject used internally by the framework to associate with the return value of a function
bool FuncResultMemRegionObject::FuncResultMemRegionObject_comparable::equal(
    const comparable& that_arg) const {
  try {
    const FuncResultMemRegionObject_comparable& that =
        dynamic_cast<const FuncResultMemRegionObject_comparable&>(that_arg);
    // Since the argument has FuncResultMemRegionObject type, they're equal
    return true;
  } catch (std::bad_cast bc) {
    // Since the argument does not have FuncResultMemRegionObject type, they're not equal
    return false;
  }
}

bool FuncResultMemRegionObject::FuncResultMemRegionObject_comparable::less(
    const comparable& that_arg) const {
  try {
    const FuncResultMemRegionObject_comparable& that =
        dynamic_cast<const FuncResultMemRegionObject_comparable&>(that_arg);
    // Since the argument has FuncResultMemRe1gionObject type, they're equal, but not less-than
    return false;
  } catch (std::bad_cast bc) {
    // Since the argument does not have FuncResultMemRegionObject type, they're not equal.
    // We'll order FuncResultMemRegionObjects before all others, so return that this < that
    return true;
  }
}

FuncResultMemRegionObject::FuncResultMemRegionObject(Function func) :
    MemRegionObject(NULL), func(func) {
}

// Returns whether this object may/must be equal to o within the given Part p
bool FuncResultMemRegionObject::mayEqualAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
// FuncResultMemRegionObjects are disjoint. They're either equal or not subsets.
  return mustEqualAO(o, pedge);
}

bool FuncResultMemRegionObject::mustEqualAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  //The two objects denote the same set iff they're both FuncResultMemRegionObjects that correspond to the same function
  FuncResultMemRegionObjectPtr that = boost::dynamic_pointer_cast<
      FuncResultMemRegionObject>(o);
  return that && func == that->func;
}

// Returns whether the two abstract objects denote the same set of concrete objects
bool FuncResultMemRegionObject::equalSetAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  // FuncResultMemRegionObjects are disjoint. They're either equal or not subsets.
  return mustEqualAO(o, pedge);
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
bool FuncResultMemRegionObject::subSetAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  // FuncResultMemRegionObjects are disjoint. They're either equal or not subsets.
  return mustEqualAO(o, pedge);
}

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool FuncResultMemRegionObject::meetUpdateAO(MemRegionObjectPtr that,
    PartEdgePtr pedge) {
  assert(0);
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool FuncResultMemRegionObject::isFullAO(PartEdgePtr pedge) {
  return false;
}

// Returns whether this AbstractObject denotes the empty set.
bool FuncResultMemRegionObject::isEmptyAO(PartEdgePtr pedge) {
  return false;
}

// Returns a ValueObject that denotes the size of this memory region
ValueObjectPtr FuncResultMemRegionObject::getRegionSizeAO(PartEdgePtr pedge) {
  // The size of a function result is irrelevant since its internals cannot be accessed directly
  // (its possible to access the internals of its SgFunctionCallExp though) so we return an unknown size.
  return boost::make_shared<FullValueObject>();
}

MemRegionObjectPtr FuncResultMemRegionObject::copyAOType() const {
  return boost::make_shared<FuncResultMemRegionObject>(func);
}

// Returns a key that uniquely identifies this particular AbstractObject in the 
// set hierarchy.
const AbstractionHierarchy::hierKeyPtr& FuncResultMemRegionObject::getHierKey() const {
  if (!isHierKeyCached) {
    ((FuncResultMemRegionObject*) this)->cachedHierKey = boost::make_shared<
        AOSHierKey>(((FuncResultMemRegionObject*) this)->shared_from_this());

    ((FuncResultMemRegionObject*) this)->cachedHierKey->add(
        boost::make_shared<FuncResultMemRegionObject_comparable>());
    ((FuncResultMemRegionObject*) this)->isHierKeyCached = true;
  }
  return cachedHierKey;
}

/* ##################################
   ##### FullMemRegionObject   #####
   ################################## */

bool FullMemRegionObject::mayEqualAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  return true;
}

bool FullMemRegionObject::mustEqualAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  return false;
}

bool FullMemRegionObject::equalSetAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  return isFullAO(pedge);
}

bool FullMemRegionObject::subSetAO(MemRegionObjectPtr o, PartEdgePtr pedge) {
  return isFullAO(pedge);
}

MemRegionObjectPtr FullMemRegionObject::copyAOType() const {
  return boost::make_shared<FullMemRegionObject>();
}

bool FullMemRegionObject::isLiveAO(PartEdgePtr pedge) {
  return true;
}

bool FullMemRegionObject::meetUpdateAO(MemRegionObjectPtr that,
    PartEdgePtr pedge) {
  return false;
}

bool FullMemRegionObject::isFullAO(PartEdgePtr pedge) {
  return true;
}

bool FullMemRegionObject::isEmptyAO(PartEdgePtr pedge) {
  return false;
}

// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
bool FullMemRegionObject::isConcrete() {
  return false;
}

// Returns the number of concrete values in this set
int FullMemRegionObject::concreteSetSize() {
  return -1;
}

ValueObjectPtr FullMemRegionObject::getRegionSizeAO(PartEdgePtr pedge) {
  return boost::make_shared<FullValueObject>();
}

string FullMemRegionObject::str(string indent) const {
  return "FullMemRegionObject";
}

/* ###################################
   ##### CombinedMemRegionObject   #####
   ################################### * /

/*
 // Creates a new CombinedMemRegionObject instance of the generic CombinedMemRegionObject class.
 template <bool defaultMayEq>
 boost::shared_ptr<CombinedMemRegionObject<defaultMayEq> > CombinedMemRegionObject<defaultMayEq>::create(MemRegionObjectPtr memReg)
 {
 list<MemRegionObjectPtr> memRegions; memRegions.push_back(memReg);
 return boost::make_shared<CombinedMemRegionObject<defaultMayEq> >(memRegions);

 // dbg << "<font color=\"$#ff0000\">"<<memLoc->str()<<"</font>"<<endl;
 }

 // Sriram: gcc 4.1.2 complains of undefined references to unused to template functions
 // fix: explicit template instantiation
 template boost::shared_ptr<CombinedMemRegionObject<true> > CombinedMemRegionObject<true>::create(MemRegionObjectPtr memReg);
 template boost::shared_ptr<CombinedMemRegionObject<false> > CombinedMemRegionObject<false>::create(MemRegionObjectPtr memReg);

 template <bool defaultMayEq>
 boost::shared_ptr<CombinedMemRegionObject<defaultMayEq> > CombinedMemRegionObject<defaultMayEq>::create(const std::list<MemRegionObjectPtr>& memRegions)
 {
 //dbg << "CombinedMemRegionObject<"<<defaultMayEq<<">::create() generic "<< endl;
 return boost::make_shared<CombinedMemRegionObject<defaultMayEq> >(memRegions);
 }

 // Sriram: gcc 4.1.2 complains of undefined references to unused to template functions
 // fix: explicit template instantiation
 template boost::shared_ptr<CombinedMemRegionObject<true> > CombinedMemRegionObject<true>::create(const std::list<MemRegionObjectPtr>& memRegions);
 template boost::shared_ptr<CombinedMemRegionObject<false> > CombinedMemRegionObject<false>::create(const std::list<MemRegionObjectPtr>& memRegions);
 * /

template<bool defaultMayEq>
SgNode* CombinedMemRegionObject<defaultMayEq>::getBase() const {
  // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
  SgNode* base = NULL;
  for (list<MemRegionObjectPtr>::const_iterator it = memRegions.begin();
      it != memRegions.end(); it++) {
    if (it == memRegions.begin())
      base = (*it)->getBase();
    else if (base != (*it)->getBase())
      return NULL;
  }

  return base;
}

template<bool defaultMayEq>
void CombinedMemRegionObject<defaultMayEq>::add(MemRegionObjectPtr memReg) {
  memRegions.push_back(memReg);
}

// Returns whether this object may/must be equal to o within the given Part p
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::mayEqualAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  //dbg << "Comparing " << this->str("    ") << "with " << o->str("    ") << endl;

  boost::shared_ptr<CombinedMemRegionObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemRegionObject<defaultMayEq> >(o);
  assert(that);

  // If the two combination objects include different numbers of MemRegionObjects, say that they may be equal since
  // we can't be sure either way.
  if (memRegions.size() != that->memRegions.size())
    return true;

  assert(memRegions.size() > 0);

  // Compare all the pairs of MemRegionObjects in memLocs and that.memLocs, returning defaultMayEq if any pair
  // returns defaultMayEq since we're looking for the tightest (if defaultMayEq=false) / loosest (if defaultMayEq=true)
  // answer that any MemRegionObject in memLocs can give
  for (list<MemRegionObjectPtr>::iterator thisIt = memRegions.begin(),
      thatIt = that->memRegions.begin(); thisIt != memRegions.end();
      thisIt++, thatIt++) {
    if ((*thisIt)->mayEqualAO(*thatIt, pedge) == defaultMayEq)
      return defaultMayEq;
  }

  return !defaultMayEq;
}

template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::mustEqualAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedMemRegionObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemRegionObject<defaultMayEq> >(o);
  assert(that);

  // If the two combination  objects include different numbers of MemRegionObjects, say that they are not must equal since
  // we can't be sure either way.
  if (memRegions.size() != that->memRegions.size())
    return false;

  assert(memRegions.size() > 0);

  // Compare all the pairs of MemRegionObjects in memLocs and that.memLocs, returning !defaultMayEq if any pair
  // returns !defaultMayEqual since we're looking for the tightest answer that any MemRegionObject in memLocs can give
  for (list<MemRegionObjectPtr>::iterator thisIt = memRegions.begin(),
      thatIt = that->memRegions.begin(); thisIt != memRegions.end();
      thisIt++, thatIt++) {
    if ((*thisIt)->mustEqualAO(*thatIt, pedge) == !defaultMayEq)
      return !defaultMayEq;
  }

  return defaultMayEq;
}

// Returns whether the two abstract objects denote the same set of concrete objects
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::equalSetAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedMemRegionObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemRegionObject<defaultMayEq> >(o);
  assert(that);
  assert(memRegions.size() == that->memRegions.size());
  assert(memRegions.size() > 0);

  // Two unions and intersections denote the same set of their components individually denote the same set
  // (we can get a more precise answer if we could check set containment relations as well)
  list<MemRegionObjectPtr>::const_iterator mrThis = memRegions.begin();
  list<MemRegionObjectPtr>::const_iterator mrThat = that->memRegions.begin();
  for (; mrThis != memRegions.end(); mrThis++, mrThat++)
    if (!(*mrThis)->equalSetAO(*mrThat, pedge))
      return false;
  return true;
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::subSetAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedMemRegionObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemRegionObject<defaultMayEq> >(o);
  assert(that);
  assert(memRegions.size() == that->memRegions.size());
  assert(memRegions.size() > 0);

  // Compare all the pairs of MemRegionObjects in memLocs and that.memLocs, returning defaultMayEq if any pair
  // returns defaultMayEq since we're looking for the tightest (if defaultMayEq=false) / loosest (if defaultMayEq=true)
  // answer that any MemRegionObject in memLocs can give
  for (list<MemRegionObjectPtr>::iterator thisIt = memRegions.begin(),
      thatIt = that->memRegions.begin(); thisIt != memRegions.end();
      thisIt++, thatIt++) {
    if ((*thisIt)->subSetAO(*thatIt, pedge) == defaultMayEq)
      return defaultMayEq;
  }
  return !defaultMayEq;
}

// Allocates a copy of this object and returns a pointer to it
template<bool defaultMayEq>
MemRegionObjectPtr CombinedMemRegionObject<defaultMayEq>::copyAOType() const {
  return boost::make_shared<CombinedMemRegionObject>(memRegions);
}

// Returns true if this object is live at the given part and false otherwise
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::isLiveAO(PartEdgePtr pedge) {
  assert(memRegions.size() > 0);
//cout << "CombinedMemRegionObject<defaultMayEq>::isLiveMR"<<endl;
  // If this is a union type (defaultMayEq=true), an object is live if any of its components are live (weakest constraint)
  // If this is an intersection type (defaultMayEq=false), an object is dead if any of its components are dead (strongest constraint)
  for (list<MemRegionObjectPtr>::const_iterator mr = memRegions.begin();
      mr != memRegions.end(); mr++)
    if ((*mr)->isLiveAO(pedge) == defaultMayEq)
      return defaultMayEq;

  return !defaultMayEq;
}

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::meetUpdateAO(MemRegionObjectPtr o,
    PartEdgePtr pedge) {
  boost::shared_ptr<CombinedMemRegionObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemRegionObject<defaultMayEq> >(o);
  assert(that);
  assert(memRegions.size() == that->memRegions.size());
  assert(memRegions.size() > 0);
  bool modified = false;

  // Perform the meetUpdate operation on all member MemLocs
  list<MemRegionObjectPtr>::const_iterator mrThis = memRegions.begin();
  list<MemRegionObjectPtr>::const_iterator mrThat = that->memRegions.begin();
  for (; mrThis != memRegions.end(); mrThis++, mrThat++)
    modified = (*mrThis)->meetUpdateAO(*mrThat, pedge) || modified;
  return modified;
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::isFullAO(PartEdgePtr pedge) {
  assert(memRegions.size() > 0);

  // If this is a union type (defaultMayEq=true), an object is full if any of its components are full (weakest constraint)
  // If this is an intersection type (defaultMayEq=false), an object is not full if any of its components are not full (strongest constraint)
  for (list<MemRegionObjectPtr>::const_iterator mr = memRegions.begin();
      mr != memRegions.end(); mr++)
    if ((*mr)->isFullAO(pedge) == defaultMayEq)
      return defaultMayEq;

  return !defaultMayEq;
}

// Returns whether this AbstractObject denotes the empty set.
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::isEmptyAO(PartEdgePtr pedge) {
  assert(memRegions.size() > 0);

  // If this is a union type (defaultMayEq=true), an object is not empty if any of its components are not empty (weakest constraint)
  // If this is an intersection type (defaultMayEq=false), an object is empty if any of its components are empty (strongest constraint)
  for (list<MemRegionObjectPtr>::const_iterator mr = memRegions.begin();
      mr != memRegions.end(); mr++)
    if ((*mr)->isEmptyAO(pedge) != defaultMayEq)
      return !defaultMayEq;

  return defaultMayEq;
}

// Returns true if this MemRegionObject denotes a finite set of concrete regions
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::isConcrete() {
  SgType* commonType = NULL;

  // If this is a union type (defaultMayEq=true), an object is concrete if all of its components are concrete weakest constraint).
  // If this is an intersection type (defaultMayEq=false), an object is concrete if any of its components are concrete (strongest constraint)
  // Further, in both cases an MR is not concrete if there is disagreement about its type
  for (list<MemRegionObjectPtr>::const_iterator mr = memRegions.begin();
      mr != memRegions.end(); mr++) {
    if ((*mr)->isConcrete() != defaultMayEq)
      return !defaultMayEq;

    if (commonType == NULL)
      commonType = (*mr)->getConcreteType();
    else if (commonType != (*mr)->getConcreteType())
      return false;
  }

  return defaultMayEq;
}

// Returns the number of concrete values in this set
template<bool defaultMayEq>
int CombinedMemRegionObject<defaultMayEq>::concreteSetSize() {
  assert(isConcrete());
  return getConcrete().size();
}

// Returns the type of the concrete regions (if there is one)
template<bool defaultMayEq>
SgType* CombinedMemRegionObject<defaultMayEq>::getConcreteType() {
  SgType* commonType = NULL;
  for (list<MemRegionObjectPtr>::const_iterator mr = memRegions.begin();
      mr != memRegions.end(); mr++) {
    if (commonType == NULL)
      commonType = (*mr)->getConcreteType();
    else if (commonType != (*mr)->getConcreteType())
      return NULL;
  }
  assert(commonType);
  return commonType;
}

// Returns the set of concrete memory regions as SgExpressions, which allows callers to use
// the normal ROSE mechanisms to decode it
template<bool defaultMayEq>
std::set<SgNode*> CombinedMemRegionObject<defaultMayEq>::getConcrete() {
  assert(isConcrete());

  // If this is a union type (defaultMayEq=true), the result is the Union of the sets returned by getConcrete() on all the memRegions.
  // If this is an intersection type (defaultMayEq=false), an object is their Intersection.

  // Maps each concrete value to the number of elements in memRegions for which it was returned
  std::map<SgNode*, size_t> concreteMRs;
  for (list<MemRegionObjectPtr>::iterator mr = memRegions.begin();
      mr != memRegions.end(); mr++) {
    // Iterate through the current sub-MemRegion's concrete values and increment each
    // concrete value's counter in concreteMRs.
    std::set<SgNode*> curConcr = (*mr)->getConcrete();
    for (std::set<SgNode*>::iterator i1 = curConcr.begin();
        i1 != curConcr.end(); i1++) {
      // Find the key in concrete memRegions with an equivalent SgExpression to *i1
      std::map<SgNode*, size_t>::iterator i2 = concreteMRs.find(*i1);
      if (i2 != concreteMRs.end())
        ++i2->second;

      // If the current concrete value *i1 does not appear in concreteMRs, add it
      if (i2 == concreteMRs.end())
        concreteMRs[*i1] = 1;
    }
  }

  // Collect the union or intersection of all results from concreteMRs as a set
  std::set<SgNode*> ret;
  for (std::map<SgNode*, size_t>::iterator i = concreteMRs.begin();
      i != concreteMRs.end(); i++) {
    // Union: add every key in concreteMRs to ret
    if (defaultMayEq)
      ret.insert(i->first);
    // Intersection: only add the keys that appear in every MemRegion in memRegions
    else if (!defaultMayEq && i->second == memRegions.size())
      ret.insert(i->first);
  }

  return ret;
}

// Returns a ValueObject that denotes the size of this memory region
template<bool defaultMayEq>
ValueObjectPtr CombinedMemRegionObject<defaultMayEq>::getRegionSizeAO(
    PartEdgePtr pedge) {
  assert(memRegions.size() > 0);

  ValueObjectPtr res;
  // Merge the ValueObjects returned by calls to gerRegionSize on all sub-regions
  for (list<MemRegionObjectPtr>::const_iterator mr = memRegions.begin();
      mr != memRegions.end(); mr++) {
    if (mr == memRegions.begin())
      res = (*mr)->getRegionSizeAO(pedge)->copyAOType();
    else
      // !!! GB 2012-09-16 : this is not quite right. We should be calling meetUpdate() to make sure
      //        the call gets routed through the generic ValueObject machinery, if any, before being
      //        forwarded to the meetUpdateAO() method.
      // SA 2014/6/13: This will probably break if the ValueObjects stored in this collection are different
      res->meetUpdateAO((*mr)->getRegionSizeAO(pedge), pedge);
  }
  return res;
}

template<bool defaultMayEq>
std::string CombinedMemRegionObject<defaultMayEq>::str(
    std::string indent) const {
  ostringstream oss;
  if (memRegions.size() > 1)
    oss << "[" << (defaultMayEq ? "UnionMR" : "IntersectMR") << ": ";
  if (memRegions.size() > 1)
    oss << endl;
  for (list<MemRegionObjectPtr>::const_iterator mr = memRegions.begin();
      mr != memRegions.end();) {
    if (mr != memRegions.begin())
      oss << indent << "&nbsp;&nbsp;&nbsp;&nbsp;";
    oss << (*mr)->str(indent + "&nbsp;&nbsp;&nbsp;&nbsp;");
    mr++;
    if (mr != memRegions.end())
      oss << endl;
  }
  if (memRegions.size() > 1)
    oss << "]";

  return oss.str();
}

// Returns whether all instances of this class form a hierarchy. Every instance of the same
// class created by the same analysis must return the same value from this method!
template<bool defaultMayEq>
bool CombinedMemRegionObject<defaultMayEq>::isHierarchy() const {
  // Combined MemRegions form hierarchy if:
  // - All the sub-MemRegions form hierarchies of their own, AND
  // - The combination is an intersection.
  //   If the combination is a union then consider the following:
  //            MLa     MLb
  //   comb1 = {a,b}, {w, x}
  //   comb2 = {a,b}, {y, z}
  //   Note that MLs from analyses a and b are either identical sets or disjoint
  //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
  //   not hold unless we work out a new hierarchy for MLb.

  // Unions are not hierarchical unless they're singletons
  if (defaultMayEq) {
    if (memRegions.size() == 1)
      return (*memRegions.begin())->isHierarchy();
    else
      return false;
  }

  for (list<MemRegionObjectPtr>::const_iterator ml = memRegions.begin(); ml != memRegions.end(); ml++)
    if (!(*ml)->isHierarchy())
      return false;
  return true;
}

// Returns a key that uniquely identifies this particular AbstractObject in the
// set hierarchy.
template<bool defaultMayEq>
const AbstractionHierarchy::hierKeyPtr& CombinedMemRegionObject<defaultMayEq>::getHierKey() const {
  // The intersection of multiple objects is just a filtering process from the full
  // set of objects to the exact one, with each key in the hierarchy further filtering
  // the set down. As such, a hierarchical key for the intersection of multiple objects
  // is just the concatenation of the keys of all the individual objects.
  if (!isHierKeyCached) {
    /*((CombinedMemRegionObject<defaultMayEq>*) this)->cachedHierKey =
     boost::make_shared<AOSHierKey>(
     ((CombinedMemRegionObject<defaultMayEq>*) this)->shared_from_this());

     for (list<MemRegionObjectPtr>::const_iterator i = memRegions.begin();
     i != memRegions.end(); i++) {
     AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<
     AbstractionHierarchy>(*i);
     ROSE_ASSERT(hierIt);

     ((CombinedMemRegionObject<defaultMayEq>*) this)->cachedHierKey->add(
     hierIt->getHierKey()->begin(), hierIt->getHierKey()->end());
     }* /
    list<hierKeyPtr> subHierKeys;
    for (list<MemRegionObjectPtr>::const_iterator i = memRegions.begin(); i != memRegions.end(); i++) {
      AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(*i);
      ROSE_ASSERT(hierIt);
      subHierKeys.push_back(hierIt->getHierKey());
    }

    ((CombinedMemRegionObject<defaultMayEq>*) this)->cachedHierKey = boost::make_shared<IntersectHierKey>(subHierKeys);

    ((CombinedMemRegionObject<defaultMayEq>*) this)->isHierKeyCached = true;
  }
  return cachedHierKey;
}*/

/* #################################
   ##### MappedMemRegionObject   #####
   ################################# */

//  template<class Key>
//  SgNode* MappedMemRegionObject<Key>::getBase() const {
//    // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
//    SgNode* base = NULL;
//    for (typename map<Key, MemRegionObjectPtr>::const_iterator it =
//        aoMap.begin(); it != aoMap.end(); ++it) {
//      if (it == aoMap.begin())
//        base = it->second->getBase();
//      else if (base != it->second->getBase())
//        return NULL;
//    }
//
//    return base;
//  }
//
////! Method to add mrs to the map.
////! MRs that are full are never added to the map.
////! If mr_p is FullMR or mr_p->isFullMR=true then mapped MR is set to full only if mostAccurate=false.
//  template<class Key>
//  void MappedMemRegionObject<Key>::add(Key key,
//      MemRegionObjectPtr mr_p, PartEdgePtr pedge, Composer* comp,
//      ComposedAnalysis* analysis) {
//    // If the object is already full don't add anything
//    if (isUnion() && isFullAO(pedge))
//      return;
//
//    // If the mr_p is not full add/update the map
//    if (!mr_p->isFullAO(pedge)) {
//      aoMap[key] = mr_p;
//    } else {
//      nFull++;
//      if (isUnion())
//        setMRToFull();
//    }
//  }
//
//  template<class Key>
//  bool MappedMemRegionObject<Key>::mayEqualMRWithKey(Key key,
//      const map<Key, MemRegionObjectPtr>& thatMRMap, PartEdgePtr pedge) {
//    typename map<Key, MemRegionObjectPtr>::const_iterator s_it;
//    s_it = thatMRMap.find(key);
//    if (s_it == thatMRMap.end())
//      return true;
//    return aoMap[key]->mayEqualAO(s_it->second, pedge);
//  }
//
////! Two MR objects are may equals if there is atleast one execution or sub-exectuion
////! in which they represent the same memory location.
////! Analyses are conservative as they start with full set of executions.
////! Dataflow facts (predicates) shrink the set of sub-executions.
////! We do not explicity store set of sub-executions and they are described
////! by the abstract objects computed from dataflow fact exported by the analysis.
////! Unless the analyses discover otherwise, the conservative answer for mayEqualMR is true.
////! Mapped MRs are keyed using either ComposedAnalysis* or PartEdgePtr.
////! Each keyed MR object correspond to some dataflow facts computed by Key=Analysis* or
////! computed at Key=PartEdgePtrthat describes some sets of executions.
////! MayEquality check on mapped MR is performed on intersection of sub-executions
////! or union of sub-executions over the keyed MR objects.
//  template<class Key>
//  bool MappedMemRegionObject<Key>::mayEqualAO(
//      MemRegionObjectPtr thatMR, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedMemRegionObject<Key> > thatMR_p =
//        boost::dynamic_pointer_cast<MappedMemRegionObject<Key> >(
//            thatMR);
//    assert(thatMR_p);
//
//    // This object denotes full set of MR (full set of executions)
//    if (isFullAO(pedge))
//      return true;
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return false;
//
//    // presence of one more full objects will result in full set over union
//    if (isUnion() && nFull > 0)
//      return true;
//
//    // Two cases reach here [1] isUnion()=true && nFull_MR=0 [2] intersect=true && nFullMR=0 or nFull_MR!=0.
//    // For both cases iterate on the MR map and discharge the mayEqualMR query to individual objects
//    // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
//    const map<Key, MemRegionObjectPtr> thatMRMap = thatMR_p->getMemRegionsMap();
//    typename map<Key, MemRegionObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      bool isMayEq = mayEqualMRWithKey(it->first, thatMRMap, pedge);
//
//      // 1. Union of sub-executions and the object does not contain any full objects.
//      // If the discharged query comes back as true for this case then we have found atleast one execution
//      // under which the two objects are same and the set can only grow and the result of this query is not going
//      // to change due to union.
//      // If false we iterate further as any MR can add more executions under which the objects are may equals.
//      if (isUnion() && isMayEq == true)
//        return true;
//
//      // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
//      // The sub-executions are intersected and therefore it does not matter if we have full objects.
//      // If the discharged query returns false then return false.
//      // We did not find one execution in which the two objects are may equals.
//      // Note that set of executions are contained over keyed objects (analyses are conservative).
//      // This set only shrinks during intersection and it is not going to affect the result of this query.
//      // If it returns true iterate further as some executions corresponding to true may be dropped.
//      else if (isIntersection() && isMayEq == false)
//        return false;
//    }
//
//    // All the keyed objects returned false for the discharged query under union.
//    // We haven't found a single execution under which the two objects are may equals.
//    if (isUnion())
//      return false;
//    // All the keyed objects returned true for the discharged query under intersection.
//    // We have atleast one execution in common in which the two objects are may equals.
//    else if (isIntersection())
//      return true;
//    else
//      assert(0);
//  }
//
//  template<class Key>
//  bool MappedMemRegionObject<Key>::mustEqualMRWithKey(Key key,
//      const map<Key, MemRegionObjectPtr>& thatMRMap, PartEdgePtr pedge) {
//    typename map<Key, MemRegionObjectPtr>::const_iterator s_it;
//    s_it = thatMRMap.find(key);
//    if (s_it == thatMRMap.end())
//      return false;
//    return aoMap[key]->mustEqualAO(s_it->second, pedge);
//  }
//
////! Two MR objects are must equals if they represent the same single memory
////! location on all executions.
////! Analyses are conservative as they start with full set of executions.
////! Dataflow facts (predicates) shrink the set of sub-executions.
////! We do not explicity store set of sub-executions and they are described
////! by the abstract objects computed from dataflow fact exported by the analysis.
////! Unless the analyses discover otherwise conservative answer for mustEqualMR is false.
////! Mapped MRs are keyed using either ComposedAnalysis* or PartEdgePtr.
////! Each keyed MR object correspond to some dataflow facts computed by Key=Analysis* or
////! computed at Key=PartEdgePtrthat describes some sets of executions.
////! MustEquality check on mapped MR is performed on intersection (mostAccurate=true) of sub-executions
////! or union (mostAccurate=false) of sub-executions over the keyed MR objects.
//  template<class Key>
//  bool MappedMemRegionObject<Key>::mustEqualAO(
//      MemRegionObjectPtr thatMR, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedMemRegionObject<Key> > thatMR_p =
//        boost::dynamic_pointer_cast<MappedMemRegionObject<Key> >(
//            thatMR);
//    assert(thatMR_p);
//
//    // This object denotes full set of MR (full set of executions)
//    if (isFullAO(pedge))
//      return false;
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return false;
//
//    // presence of one more full objects will result in full set over union
//    if (isUnion() && nFull > 0)
//      return true;
//
//    // Two cases reach here [1] isUnion()=true && nFull_MR=0 [2] intersect=true && nFullMR=0 or nFull_MR!=0.
//    // For both cases iterate on the MR map and discharge the mayEqualMR query to individual objects
//    // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
//    const map<Key, MemRegionObjectPtr> thatMRMap = thatMR_p->getMemRegionsMap();
//    typename map<Key, MemRegionObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      bool isMustEq = mustEqualMRWithKey(it->first, thatMRMap, pedge);
//
//      // 1. Union of sub-executions and the object does not contain any full objects
//      // If the discharged query comes back as false for this case then we have found atleast one execution
//      // under which the two objects are not same and the set can only grow and the result of this query is not going
//      // to change due to union.
//      // If it returns true we iterate further as any MR can add more executions under which the objects are not must equals.
//      if (isUnion() && isMustEq == false)
//        return false;
//
//      // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
//      // The sub-executions are intersected and therefore it does not matter if we have full objects.
//      // If the discharged query returns true then return true.
//      // Under all sub-executions (corresponding to the MR) the two objects must equal.
//      // Note that set of executions are contained over keyed objects as the analyses are conservative.
//      // This set only shrinks during intersection and it is not going to affect the result of this query.
//      // If it returns false iterate further as some executions corresponding to false may be dropped.
//      else if (isIntersection() && isMustEq == true)
//        return true;
//    }
//
//    // All the keyed objects returned true for the discharged query under union.
//    // We haven't found a single execution under which the two objects are not equal.
//    if (isUnion())
//      return true;
//    // All the keyed objects returned false for the discharged query under intersection.
//    // We have atleast one execution in common in which the two objects are not equal.
//    else if (isIntersection())
//      return false;
//    else
//      assert(0);
//  }
//
////! Discharge the query to the corresponding MR
////! If key not found in thatMRMap return false
//  template<class Key>
//  bool MappedMemRegionObject<Key>::equalSetMRWithKey(Key key,
//      const map<Key, MemRegionObjectPtr>& thatMRMap, PartEdgePtr pedge) {
//    typename map<Key, MemRegionObjectPtr>::const_iterator s_it;
//    s_it = thatMRMap.find(key);
//    if (s_it == thatMRMap.end())
//      return false;
//    return aoMap[key]->equalSetAO(s_it->second, pedge);
//  }
//
////! Two objects are equal sets if they denote the same set of memory locations
////! The boolean parameter mostAccurate is not releveant as this query is not
////! answered based on union or intersection of sub-executions.
////! Simply discharge the queries to all keyed MemRegion objects
////! If all the discharged queries come back equal then the two objects are equal otherwise not.
//  template<class Key>
//  bool MappedMemRegionObject<Key>::equalSetAO(
//      MemRegionObjectPtr thatMR, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedMemRegionObject<Key> > thatMR_p =
//        boost::dynamic_pointer_cast<MappedMemRegionObject<Key> >(
//            thatMR);
//    assert(thatMR_p);
//
//    // This object denotes full set of MR (full set of executions)
//    if (isFullAO(pedge))
//      return thatMR_p->isFullAO(pedge);
//
//    // denotes empty set
//    if (isEmptyAO(pedge))
//      return thatMR_p->isEmptyAO(pedge);
//
//    const map<Key, MemRegionObjectPtr> thatMRMap = thatMR_p->getMemRegionsMap();
//    typename map<Key, MemRegionObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      // break even if one of them returns false
//      if (equalSetMRWithKey(it->first, thatMRMap, pedge) == false)
//        return false;
//    }
//
//    return true;
//  }
//
////! Discharge the query to the corresponding MR
////! If key not found in thatMRMap return true as the
////! keyed object on thatMRMap denotes full set
//  template<class Key>
//  bool MappedMemRegionObject<Key>::subSetMRWithKey(Key key,
//      const map<Key, MemRegionObjectPtr>& thatMRMap, PartEdgePtr pedge) {
//    typename map<Key, MemRegionObjectPtr>::const_iterator s_it;
//    s_it = thatMRMap.find(key);
//    if (s_it == thatMRMap.end())
//      return true;
//    return aoMap[key]->subSetAO(s_it->second, pedge);
//  }
//
////! This object is a non-strict subset of the other if the set of memory locations denoted by this
////! is a subset of the set of memory locations denoted by that.
////! The boolean parameter mostAccurate is not releveant as this query is not
////! answered based on union or intersection of sub-executions.
////! Simply discharge the queries to all keyed MemRegion objects
////! If all the discharged queries come back true then this is a subset of that otherwise not.
//  template<class Key>
//  bool MappedMemRegionObject<Key>::subSetAO(
//      MemRegionObjectPtr thatMR, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedMemRegionObject<Key> > thatMR_p =
//        boost::dynamic_pointer_cast<MappedMemRegionObject<Key> >(
//            thatMR);
//    assert(thatMR_p);
//
//    // This object denotes full set of MR (full set of executions)
//    if (isFullAO(pedge))
//      return thatMR_p->isFullAO(pedge);
//
//    // denotes empty set
//    // thatMR could be empty or non-empty eitherway this will be a non-strict subset of that.
//    if (isEmptyAO(pedge))
//      return true;
//
//    // If both objects have the same keys discharge
//    // If this object has a key and that does not then
//    // the keyed object is subset of that (return true) implemented by subsetMRWithKey
//    // If any of the discharged query return false then return false.
//    const map<Key, MemRegionObjectPtr> thatMRMap = thatMR_p->getMemRegionsMap();
//    typename map<Key, MemRegionObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      // break even if one of them returns false
//      if (subSetMRWithKey(it->first, thatMRMap, pedge) == false)
//        return false;
//    }
//
//    // If this object doesn't have the key and that object has the key then
//    // return false as this object has full object mapped to the key
//    typename map<Key, MemRegionObjectPtr>::const_iterator c_it;
//    for (c_it = thatMRMap.begin(); c_it != thatMRMap.end() && (nFull != 0);
//        ++c_it) {
//      if (aoMap.find(c_it->first) == aoMap.end())
//        return false;
//    }
//
//    return true;
//  }
//
////! Mapped object liveness is determined based on finding executions
////! in which it may be live.
////! It can be answered based on union (mostAccurate=false) or intersection
////! (mostAccurate=true) of executions
////! The conservative answer is to assume that the object is live
//  template<class Key>
//  bool MappedMemRegionObject<Key>::isLiveAO(PartEdgePtr pedge) {
//    // If this object is full return the conservative answer
//    if (isFullAO(pedge))
//      return true;
//
//    // If it has one or more full objects added to it
//    // and if the object has mostAccurate=false then return true (weakest answer)
//    if (nFull > 0 && isUnion())
//      return true;
//
//    // 1. This object may have have one or more full objects under intersection
//    // 2. This object doesnt have any full objects added to it under union
//    // Under both cases the answer is based on how individual analysis respond to the query
//    typename map<Key, MemRegionObjectPtr>::iterator it = aoMap.begin();
//    for (; it != aoMap.end(); ++it) {
//      bool isLive = it->second->isLiveAO(pedge);
//      if (isUnion() && isLive == true)
//        return true;
//      else if (isIntersection() && isLive == false)
//        return false;
//    }
//
//    // leftover cases
//    if (isUnion())
//      return false;
//    else if (isIntersection())
//      return true;
//    else
//      assert(0);
//  }
//
////! meetUpdateMR performs the join operation of abstractions of two mls
//  template<class Key>
//  bool MappedMemRegionObject<Key>::meetUpdateAO(
//      MemRegionObjectPtr that, PartEdgePtr pedge) {
//    boost::shared_ptr<MappedMemRegionObject<Key> > thatMR_p =
//        boost::dynamic_pointer_cast<MappedMemRegionObject<Key> >(
//            that);
//    assert(thatMR_p);
//
//    // if this object is already full
//    if (isFullAO(pedge))
//      return false;
//
//    // If that object is full set this object to full
//    if (thatMR_p->isFullAO(pedge)) {
//      nFull++;
//      setMRToFull();
//      return true;
//    }
//
//    // Both objects are not full
//    const map<Key, MemRegionObjectPtr> thatMRMap = thatMR_p->getMemRegionsMap();
//
//    typename map<Key, MemRegionObjectPtr>::iterator it = aoMap.begin();
//    typename map<Key, MemRegionObjectPtr>::const_iterator s_it; // search iterator for thatMRMap
//
//    bool modified = false;
//    while (it != aoMap.end()) {
//      s_it = thatMRMap.find(it->first);
//      // If two objects have the same key then discharge meetUpdate to the corresponding keyed MR objects
//      if (s_it != thatMRMap.end()) {
//        modified = (it->second)->meetUpdateAO(s_it->second, pedge) || modified;
//      }
//
//      // Remove the current MR object (current iterator it) from the map if the mapepd object is full.
//      // Two cases under which the current MR object can be full.
//      // (1) If current key is not found in thatMRMap then the mapped object
//      // in thatMRMap is full and the meetUpdate of the current MR with that is also full.
//      // (2) meetUpdateMR above of the two keyed objects resulted in this mapped object being full.
//      // Under both cases remove the mapped ml from this map
//      if (s_it == thatMRMap.end() || (it->second)->isFullAO(pedge)) {
//        // Current mapped MR has become full as a result of (1) or (2).
//        // Remove the item from the map.
//        // Note that post-increment which increments the iterator and returns the old value for deletion.
//        aoMap.erase(it++);
//        nFull++;
//        modified = true;
//
//        // If union then set this entire object to full and return
//        if (isUnion()) {
//          setMRToFull();
//          return true;
//        }
//      } else
//        ++it;
//    }
//    return modified;
//  }
//
////! Method that sets this mapped object to full
//  template<class Key>
//  void MappedMemRegionObject<Key>::setMRToFull() {
//    assert(nFull > 0);
//    if (aoMap.size() > 0)
//      aoMap.clear();
//  }
//
//  template<class Key>
//  bool MappedMemRegionObject<Key>::isFullAO(PartEdgePtr pedge) {
//    return isFullAO();
//  }
//
//  template<class Key>
//  bool MappedMemRegionObject<Key>::isEmptyAO(PartEdgePtr pedge) {
//    return isEmptyAO();
//  }
//
//  template<class Key>
//  bool MappedMemRegionObject<Key>::isFullAO() {
//    if (nFull > 0 && aoMap.size() == 0)
//      return true;
//    return false;
//  }
//
//  template<class Key>
//  bool MappedMemRegionObject<Key>::isEmptyAO() {
//    if (nFull == 0 && aoMap.size() == 0)
//      return true;
//    return false;
//  }
//
//  template<class Key>
//  bool MappedMemRegionObject<Key>::isConcrete() {
//    if (isFullAO())
//      return false;
//    if (isEmptyAO())
//      return false;
//    if (isUnion() && nFull > 0)
//      return false;
//
//    typename map<Key, MemRegionObjectPtr>::iterator it = aoMap.begin();
//    for (; it != aoMap.end(); ++it) {
//      bool isConc = it->second->isConcrete();
//      // we have atleast one that is not concrete under union
//      if (isUnion() && !isConc)
//        return false;
//      // we have atleast one that is concrete under intersection
//      else if (isIntersection() && isConc)
//        return true;
//    }
//
//    // All the objects are concrete (return true above) union
//    if (isUnion())
//      return true;
//    // All the objects not concrete (return false above) under intersection
//    else if (isIntersection())
//      return false;
//    else
//      assert(0);
//  }
//
//// Returns the number of concrete values in this set
//  template<class Key>
//  int MappedMemRegionObject<Key>::concreteSetSize() {
//    assert(isConcrete());
//    return getConcrete().size();
//  }
//
template<class Key>
SgType* MappedMemRegionObject<Key>::getConcreteType() {
  assert(MappedMemRegionObject<Key>::isConcrete());
  typename map<Key, MemRegionObjectPtr>::iterator it = MappedMemRegionObject<Key>::aoMap.begin();
  SgType* c_type = it->second->getConcreteType();
  // assert that all other objects have the same type
  for (++it; it != MappedMemRegionObject<Key>::aoMap.end(); ++it) {
    SgType* votype = it->second->getConcreteType();
    assert(c_type == votype);
  }
  return c_type;
}

template<class Key>
set<SgNode*> MappedMemRegionObject<Key>::getConcrete() {
  assert(MappedMemRegionObject<Key>::isConcrete());
  // If this is a union type (defaultMayEq=true), the result is the Union of the sets returned by getConcrete() on all the memRegions.
  // If this is an intersection type (defaultMayEq=false), an object is their Intersection.

  // Maps each concrete value to the number of elements in memRegions for which it was returned
  std::map<SgNode*, size_t> concreteMRs;
  for (typename map<Key, MemRegionObjectPtr>::iterator mr_it = MappedMemRegionObject<Key>::aoMap.begin();
       mr_it != MappedMemRegionObject<Key>::aoMap.end(); ++mr_it) {
    // Iterate through the current sub-MemRegion's concrete values and increment each
    // concrete value's counter in concreteMRs.
    std::set<SgNode*> c_memregionSet = mr_it->second->getConcrete();
    for (std::set<SgNode*>::iterator s_it = c_memregionSet.begin();
        s_it != c_memregionSet.end(); s_it++) {
      // Find the key in concrete memRegions with an equivalent SgExpression to *s_it
      std::map<SgNode*, size_t>::iterator c_it = concreteMRs.find(*s_it);
      if (c_it != concreteMRs.end())
        ++c_it->second;
      // If the current concrete value *s_it does not appear in concreteMRs, add it
      else
        concreteMRs[*s_it] = 1;
    }
  }

  // Collect the union or intersection of all results from concreteMRs as a set
  std::set<SgNode*> ret;
  for (std::map<SgNode*, size_t>::iterator i = concreteMRs.begin();
      i != concreteMRs.end(); i++) {
    // Union: add every key in concreteMRs to ret
    if (MappedMemRegionObject<Key>::isUnion())
      ret.insert(i->first);
    // Intersection: only add the keys that appear in every MemRegion in memRegions
    else if (MappedMemRegionObject<Key>::isIntersection() &&
             i->second == MappedMemRegionObject<Key>::aoMap.size())
      ret.insert(i->first);
  }

  return ret;
}
//
////! Size of the memory region denoted by this memory object represented by a ValueObject
////! Useful only if the object is not full
//  template<class Key>
//  ValueObjectPtr MappedMemRegionObject<Key>::getRegionSizeAO(
//      PartEdgePtr pedge) {
//    // Assert for atleast one element in the map
//    // Should we handle full MR by returning FullValueObject?
//    assert(aoMap.size() > 0);
//
//    // getRegionSize on each object returns different ValueObject for each key
//    // We cannot do meetUpdate as the objects are from different analysis
//    // Return a MappedValueObject based on those objects and the corresponding key
//    boost::shared_ptr<MappedValueObject<Key> > mvo_p =
//        boost::make_shared<MappedValueObject<Key> >(type);
//    typename map<Key, MemRegionObjectPtr>::const_iterator it =
//        aoMap.begin();
//    for (; it != aoMap.end(); ++it) {
//      ValueObjectPtr vo_p = it->second->getRegionSizeAO(pedge);
//      Key k = it->first;
//      mvo_p->add(k, vo_p, pedge, /*comp, analysis*/NULL, NULL);
//    }
//
//    return mvo_p;
//  }
//
//  template<class Key>
//  MemRegionObjectPtr MappedMemRegionObject<Key>::copyAO() const {
//    return boost::make_shared<MappedMemRegionObject<Key> >(*this);
//  }
//
//  template<class Key>
//  string MappedMemRegionObject<Key>::str(string indent) const {
//    ostringstream oss;
//    oss << "<table border=\"1\">";
//    oss << "<tr>";
//    oss << "<th>"
//        << (isUnion() ?
//            "UnionMappedMemRegionObject:" : "IntersectMappedMemRegionObject:")
//        << "</th>";
//    if (nFull > 0 && aoMap.size() == 0)
//      oss << "<th> Full </th> </tr>";
//    else if (nFull == 0 && aoMap.size() == 0)
//      oss << "<th> Empty </th> </tr>";
//    else {
//      oss << "</tr>";
//      typename map<Key, MemRegionObjectPtr>::const_iterator it =
//          aoMap.begin();
//      for (; it != aoMap.end(); ++it) {
//        oss << "<tr>";
//        oss << "<td>" << (it->first)->str(indent) << "</td>";
//        oss << "<td>" << (it->second)->str(indent) << "</td>";
//        oss << "</tr>";
//      }
//    }
//    oss << "</table>";
//    return oss.str();
//  }
//
//// Returns whether all instances of this class form a hierarchy. Every instance of the same
//// class created by the same analysis must return the same value from this method!
//  template<class Key>
//  bool MappedMemRegionObject<Key>::isHierarchy() const {
//    // Combined MemRegions form hierarchy if:
//    // - All the sub-MemRegions form hierarchies of their own, AND
//    // - The combination is an intersection.
//    //   If the combination is a union then consider the following:
//    //            MLa     MLb
//    //   comb1 = {a,b}, {w, x}
//    //   comb2 = {a,b}, {y, z}
//    //   Note that MLs from analyses a and b are either identical sets or disjoint
//    //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
//    //   not hold unless we work out a new hierarchy for MLb.
//
//    // Unions are not hierarchical unless they're singletons
//    if (isUnion()) {
//      if (aoMap.size() == 1)
//        return aoMap.begin()->second->isHierarchy();
//      else
//        return false;
//    }
//
//    typename map<Key, MemRegionObjectPtr>::const_iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it)
//      if (!it->second->isHierarchy())
//        return false;
//    return true;
//  }
//
//// Returns a key that uniquely identifies this particular AbstractObject in the
//// set hierarchy.
//  template<class Key>
//  const AbstractionHierarchy::hierKeyPtr& MappedMemRegionObject<Key>::getHierKey() const {
//    // The intersection of multiple objects is just a filtering process from the full
//    // set of objects to the exact one, with each key in the hierarchy further filtering
//    // the set down. As such, a hierarchical key for the intersection of multiple objects
//    // is just the concatenation of the keys of all the individual objects.
//    if (!isHierKeyCached) {
//      /*((MappedMemRegionObject<Key>*) this)->cachedHierKey =
//       boost::make_shared<AOSHierKey>(
//       ((MappedMemRegionObject<Key>*) this)->shared_from_this());
//
//       typename map<Key, MemRegionObjectPtr>::const_iterator it;
//       for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//       AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<
//       AbstractionHierarchy>(it->second);
//       ROSE_ASSERT(hierIt);
//
//       ((MappedMemRegionObject<Key>*) this)->cachedHierKey->add(
//       hierIt->getHierKey()->begin(), hierIt->getHierKey()->end());
//       }*/
//
//      map<Key, hierKeyPtr> subHierKeys;
//      for(typename map<Key, MemRegionObjectPtr>::const_iterator it = aoMap.begin();
//          it != aoMap.end(); ++it) {
//        AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(it->second);
//        ROSE_ASSERT(hierIt);
//        subHierKeys[it->first] = hierIt->getHierKey();
//      }
//
//      ((MappedMemRegionObject<Key>*) this)->cachedHierKey =
//          boost::make_shared<IntersectMappedHierKey<Key> >(subHierKeys);
//
//      ((MappedMemRegionObject<Key>*) this)->isHierKeyCached = true;
//    }
//    return cachedHierKey;
//  }

//! Size of the memory region denoted by this memory object represented by a ValueObject
//! Useful only if the object is not full
template<class Key>
ValueObjectPtr MappedMemRegionObject<Key>::getRegionSizeAO(
    PartEdgePtr pedge) {
  // Assert for atleast one element in the map
  // Should we handle full MR by returning FullValueObject?
  if(!MappedAbstractObject<Key, MemRegionObject, AbstractObject::MemRegion, MappedMemRegionObject<Key> >::aoMap.size() > 0) assert(0);

  // getRegionSize on each object returns different ValueObject for each key
  // We cannot do meetUpdate as the objects are from different analysis
  // Return a MappedValueObject based on those objects and the corresponding key
  boost::shared_ptr<MappedValueObject<Key> > mvo_p =
      boost::make_shared<MappedValueObject<Key> >(MappedMemRegionObject<Key>::ui, MappedMemRegionObject<Key>::analysis);
  typename map<Key, MemRegionObjectPtr>::const_iterator it =
      MappedAbstractObject<Key, MemRegionObject, AbstractObject::MemRegion, MappedMemRegionObject<Key> >::aoMap.begin();
  for (; it != MappedAbstractObject<Key, MemRegionObject, AbstractObject::MemRegion, MappedMemRegionObject<Key> >::aoMap.end(); ++it) {
    ValueObjectPtr vo_p = it->second->getRegionSizeAO(pedge);
    Key k = it->first;
    mvo_p->add(k, vo_p, pedge, /*comp, analysis*/NULL, NULL);
  }

  return mvo_p;
}

/*template<class Key>
MemRegionObjectPtr MappedMemRegionObject<Key>::copyAO() const {
  return boost::make_shared<MappedMemRegionObject<Key> >(*this);
}

template<class Key>
string MappedMemRegionObject<Key>::str(string indent) const {
  ostringstream oss;
  oss << "<table border=\"1\">";
  oss << "<tr>";
  oss << "<th>"
      << (isUnion() ?
          "UnionMappedMemRegionObject:" : "IntersectMappedMemRegionObject:")
      << "</th>";
  if (nFull > 0 && aoMap.size() == 0)
    oss << "<th> Full </th> </tr>";
  else if (nFull == 0 && aoMap.size() == 0)
    oss << "<th> Empty </th> </tr>";
  else {
    oss << "</tr>";
    typename map<Key, MemRegionObjectPtr>::const_iterator it =
        aoMap.begin();
    for (; it != aoMap.end(); ++it) {
      oss << "<tr>";
      oss << "<td>" << (it->first)->str(indent) << "</td>";
      oss << "<td>" << (it->second)->str(indent) << "</td>";
      oss << "</tr>";
    }
  }
  oss << "</table>";
  return oss.str();
}*/

/* ################################
 # PartEdgeUnionMemRegionObject #
   ################################ */

PartEdgeUnionMemRegionObject::PartEdgeUnionMemRegionObject() :
    MemRegionObject(NULL) {
}

PartEdgeUnionMemRegionObject::PartEdgeUnionMemRegionObject(
    const PartEdgeUnionMemRegionObject& thatMR) :
    MemRegionObject(thatMR), unionMR_p(thatMR.copyAOType()) {
}

SgNode* PartEdgeUnionMemRegionObject::getBase() const {
  return unionMR_p->getBase();
}

void PartEdgeUnionMemRegionObject::add(MemRegionObjectPtr mr_p,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  // If this is the very first object
  if (!unionMR_p)
    unionMR_p = mr_p->copyAOType();
  // If Full return without adding
  else if (isFullAO(pedge))
    return;
  // Else meetUpdate with the existing unionMR_p
  else
    unionMR_p->meetUpdateAO(mr_p, pedge);
}

bool PartEdgeUnionMemRegionObject::mayEqualAO(MemRegionObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionMemRegionObject> thatMR_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemRegionObject>(that);
  assert(thatMR_p);
  return unionMR_p->mayEqualAO(thatMR_p->getUnionAO(), pedge);
}

bool PartEdgeUnionMemRegionObject::mustEqualAO(MemRegionObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionMemRegionObject> thatMR_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemRegionObject>(that);
  assert(thatMR_p);
  return unionMR_p->mustEqualAO(thatMR_p->getUnionAO(), pedge);
}

bool PartEdgeUnionMemRegionObject::equalSetAO(MemRegionObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionMemRegionObject> thatMR_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemRegionObject>(that);
  assert(thatMR_p);
  return unionMR_p->equalSetAO(thatMR_p->getUnionAO(), pedge);
}

bool PartEdgeUnionMemRegionObject::subSetAO(MemRegionObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionMemRegionObject> thatMR_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemRegionObject>(that);
  assert(thatMR_p);
  return unionMR_p->subSetAO(thatMR_p->getUnionAO(), pedge);
}

bool PartEdgeUnionMemRegionObject::meetUpdateAO(MemRegionObjectPtr that,
    PartEdgePtr pedge) {
  boost::shared_ptr<PartEdgeUnionMemRegionObject> thatMR_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemRegionObject>(that);
  assert(thatMR_p);
  return unionMR_p->meetUpdateAO(thatMR_p->getUnionAO(), pedge);
}

bool PartEdgeUnionMemRegionObject::isLiveAO(PartEdgePtr pedge) {
  return unionMR_p->isLiveAO(pedge);
}

bool PartEdgeUnionMemRegionObject::isFullAO(PartEdgePtr pedge) {
  return unionMR_p->isFullAO(pedge);
}

bool PartEdgeUnionMemRegionObject::isEmptyAO(PartEdgePtr pedge) {
  return unionMR_p->isEmptyAO(pedge);
}

MemRegionObjectPtr PartEdgeUnionMemRegionObject::copyAOType() const {
  return boost::make_shared<PartEdgeUnionMemRegionObject>(*this);
}

void PartEdgeUnionMemRegionObject::setAOToFull() {
  unionMR_p = boost::make_shared<FullMemRegionObject>();
}

// Returns true if this MemRegionObject denotes a finite set of concrete regions
bool PartEdgeUnionMemRegionObject::isConcrete() {
  return unionMR_p->isConcrete();
}
// Returns the number of concrete values in this set
int PartEdgeUnionMemRegionObject::concreteSetSize() {
  return unionMR_p->concreteSetSize();
}
// Returns the type of the concrete regions (if there is one)
SgType* PartEdgeUnionMemRegionObject::getConcreteType() {
  return unionMR_p->getConcreteType();
}
// Returns the set of concrete memory regions as SgExpressions, which allows callers to use
// the normal ROSE mechanisms to decode it
std::set<SgNode*> PartEdgeUnionMemRegionObject::getConcrete() {
  return unionMR_p->getConcrete();
}

ValueObjectPtr PartEdgeUnionMemRegionObject::getRegionSizeAO(
    PartEdgePtr pedge) {
  return unionMR_p->getRegionSizeAO(pedge);
}

string PartEdgeUnionMemRegionObject::str(string indent) const {
  ostringstream oss;
  oss << "[UnionMR=" << unionMR_p.get() << ", " << unionMR_p->str(indent)
      << "]";
  return oss.str();
}

/* ########################
   ##### MemLocObject #####
   ######################## */

MemLocObjectPtr NULLMemLocObject;

MemLocObject::MemLocObject(const MemLocObject& that) :
    AbstractObject(that), /*AbstractionHierarchy(that),*/ region(
        that.region != NULLMemRegionObject ?
            that.region->copyAOType() : NULLMemRegionObject), index(
        that.index != NULLValueObject ? that.index->copyAOType() : NULLValueObject) {
  //cout << "MemLocObject::MemLocObject("<<this<<") isHierKeyCached="<<isHierKeyCached<<", region="<<region.get()<<" that.region="<<that.region.get()<<"="<<(that.region? that.region->str(): "NULL")<<endl;

}

MemRegionObjectPtr MemLocObject::getRegion() const { /*cout << "MemLocObject::getRegion("<<this<<") region="<<(region==NULLMemRegionObject?"NULL":region->str())<<endl; */
  return region;
}
ValueObjectPtr MemLocObject::getIndex() const { /*cout << "MemLocObject::getIndex("<<this<<") index="<<(index==NULLValueObject?"NULL":index->str())<<endl; */
  return index;
}

//// Returns whether this object may/must be equal to o within the given Part p
//// These methods are called by composers and should not be called by analyses.
//bool MemLocObject::mayEqualAO(MemLocObjectPtr that, PartEdgePtr pedge) {
//  /*scope s("MemLocObject::mayEqualML");
//  dbg << "this="<<str()<<endl;
//  dbg << "that="<<that->str()<<endl;
//
//  dbg << "eqRegion="<<(region->mayEqualAO(that->getRegion(), pedge))<<endl;
//  if(index && that->index) dbg << "eqIndex="<<(index->mayEqualAO(that->getIndex(), pedge))<<endl;*/
//
//  return region && region->mayEqualAO(that->getRegion(), pedge) &&
//         ((!index && !that->index) || index->mayEqualAO(that->getIndex(), pedge));
//}
//
//bool MemLocObject::mustEqualAO(MemLocObjectPtr that, PartEdgePtr pedge) {
//  /*scope s("MemLocObject::mustEqualML");
//  dbg << "eqRegion="<<(region->mustEqualAO(that->getRegion(), pedge))<<endl;
//  if(index && that->index) dbg << "eqIndex="<<(index->mustEqualAO(that->getIndex(), pedge))<<endl;*/
//
//  return region->mustEqualAO(that->getRegion(), pedge) &&
//         ((!index && !that->index) || index->mustEqualAO(that->getIndex(), pedge));
//}

// General version of mayEqual and mustEqual that accounts for framework details before routing the call to the
// derived class' may/mustEqual check. Specifically, it checks may/must equality with respect to ExprObj and routes
// the call through the composer to make sure the may/mustEqual call gets the right PartEdge
bool MemLocObject::mayEqual(MemLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  /*scope s("MemLocObject::mayEqual");
   dbg << "this="<<str()<<endl;
   dbg << "that="<<that->str()<<endl;*/

  // Returns true only if both this region and this index denot the same set as that region and index, respectively
  // If this and that region overlap
  if (getRegion()->mayEqual(that->getRegion(), pedge, comp, analysis)) {
    if (getIndex() == NULLValueObject) {
      // If both this->index and that->index are NULL, then their sets (full) overlap
      if (that->getIndex() == NULLValueObject)
        return true;
      // If both this->index is NULL (denotes the full set) but that->index is not, they overlap
      else
        return true;
    } else {
      // If both that->index is NULL (denotes the full set) but this->index is not, they overlap
      if (that->getIndex() == NULLValueObject)
        return true;
      // If both are not NULL, forward the query to the implementation of this->index
      else
        return getIndex()->mayEqual(that->getIndex(), pedge, comp, analysis);
    }
  } else
    return false;
}

// General version of mayEqual and mustEqual that accounts for framework details before routing the call to the
// derived class' may/mustEqual check. Specifically, it checks may/must equality with respect to ExprObj and routes
// the call through the composer to make sure the may/mustEqual call gets the right PartEdge
bool MemLocObject::mustEqual(MemLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // Returns true only if both this region and this index denot the same set as that region and index, respectively

  // If this region denotes the same set as that region in all executions
  if (getRegion()->mustEqual(that->getRegion(), pedge, comp, analysis)) {
    if (getIndex() == NULLValueObject) {
      // If both this->index and that->index are NULL, we assume they're not mustEqual since
      // NULL corresponds to the Full set of values
      if (that->getIndex() == NULLValueObject)
        return false;
      // If both this->index is NULL (denotes the full set) but that->index is not, they're not mustEqual
      else
        return false;
    } else {
      // If both that->index is NULL (denotes the full set) but this->index is not, they're not mustEqual
      if (that->getIndex() == NULLValueObject)
        return false;
      // If both are not NULL, forward the query to the implementation of this->index
      else
        return getIndex()->mustEqual(that->getIndex(), pedge, comp, analysis);
    }
  } else
    return false;
}

// Check whether that is a MemLocObject and if so, call the version of mayEqual specific to MemLocObjects
bool MemLocObject::mayEqual(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is a FuncResultMemLocObject and that is not or vice versa, they're not may-equal
  if ((dynamic_cast<FuncResultMemLocObject*>(this) == NULL)
      != (boost::dynamic_pointer_cast<FuncResultMemLocObject>(that) == NULL))
    return false;
  // If that is a FullMemLocObject, it overlaps with this
  if (boost::dynamic_pointer_cast<FullMemLocObject>(that))
    return true;

  MemLocObjectPtr mo = boost::dynamic_pointer_cast<MemLocObject>(that);
  if (mo)
    return mayEqual(mo, pedge, comp, analysis);
  else
    return false;
}

// Check whether that is a MemLocObject and if so, call the version of mustEqual specific to MemLocObjects
bool MemLocObject::mustEqual(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is a FuncResultMemLocObject and that is not or vice versa, they're not must-equal
  if ((dynamic_cast<FuncResultMemLocObject*>(this) == NULL)
      != (boost::dynamic_pointer_cast<FuncResultMemLocObject>(that) == NULL))
    return false;
  // If that is a FullMemLocObject, it is not must-equal to anything
  if (boost::dynamic_pointer_cast<FullMemLocObject>(that))
    return false;

  MemLocObjectPtr mo = boost::dynamic_pointer_cast<MemLocObject>(that);
  if (mo)
    return mustEqual(mo, pedge, comp, analysis);
  else
    return false;
}

//// Returns whether the two abstract objects denote the same set of concrete objects
//// These methods are called by composers and should not be called by analyses.
//bool MemLocObject::equalSetAO(MemLocObjectPtr that, PartEdgePtr pedge) {
//  return region->equalSetAO(that->getRegion(), pedge) &&
//         ((!index && !that->index) || index->equalSetAO(that->getIndex(), pedge));
//}
//// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
//// by the given abstract object.
//// These methods are called by composers and should not be called by analyses.
//bool MemLocObject::subSetAO(MemLocObjectPtr that, PartEdgePtr pedge) {
//  return region->subSetAO(that->getRegion(), pedge) &&
//         ((!index && !that->index) || index->subSetAO(that->getIndex(), pedge));
//}

// Returns whether the two abstract objects denote the same set of concrete objects
bool MemLocObject::equalSet(MemLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // Returns true only if both this region and this index denot the same set as that region and index, respectively

  /*cout << "getRegion()="<<(region?region->str():"NULL")<<endl;
   cout << "getRegion="<<(getRegion()?getRegion()->str():"NULL")<<endl;
   cout << "this="<<str()<<endl;

   cout << "that->region="<<(that->region?that->region->str():"NULL")<<endl;
   cout << "that->getRegion="<<(that->getRegion()?that->getRegion()->str():"NULL")<<endl;
   cout << "that="<<that->str()<<endl;*/

  // If this region is denotes the same set as that region
  if (getRegion()->equalSet(that->getRegion(), pedge, comp, analysis)) {
    if (getIndex() == NULLValueObject) {
      // If both this->index and that->index are NULL, then the subset property holds
      if (that->getIndex() == NULLValueObject)
        return true;
      // If both this->index is NULL (denotes the full set) but that->index is not,
      // this->index is a equal to that->index only if that->index is Full
      else
        return that->getIndex()->isFull(pedge, comp, analysis);
    } else {
      // If both that->index is NULL (denotes the full set) but this->index is not,
      // this->index is a equal to that->index only if this->index is Full
      if (that->getIndex() == NULLValueObject)
        return getIndex()->isFull(pedge, comp, analysis);
      // If both are not NULL, forward the query to the implementation of this->index
      else
        return getIndex()->equalSet(that->getIndex(), pedge, comp, analysis);
    }
  } else
    return false;
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
bool MemLocObject::subSet(MemLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // Returns true only if both this region and this index are a subset of that region and index, respectively

  // If this region is a subset of that region
  if (getRegion()->subSet(that->getRegion(), pedge, comp, analysis)) {
    if (getIndex() == NULLValueObject) {
      // If both this->index and that->index are NULL, then the subset property holds
      if (that->getIndex() == NULLValueObject)
        return true;
      // If both this->index is NULL (denotes the full set) but that->index is not,
      // this->index is a subset of that->index only if that->index is Full
      else
        return that->getIndex()->isFull(pedge, comp, analysis);
    } else {
      // If that->index is NULL (denotes the full set) but this->index is not
      if (that->getIndex() == NULLValueObject)
        return true;
      // If both are not NULL, forward the query to the implementation of this->index
      else
        return getIndex()->subSet(that->getIndex(), pedge, comp, analysis);
    }
  } else
    return false;
}

bool MemLocObject::equalSet(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is a FuncResultMemLocObject and that is not or vice versa, one is not a subset of the other
  if ((dynamic_cast<FuncResultMemLocObject*>(this) == NULL)
      != (boost::dynamic_pointer_cast<FuncResultMemLocObject>(that) == NULL))
    return false;
  // If this is a FullMemLocObject and that is not or vice versa, the Full set contains the non-Full set
  if ((dynamic_cast<FullMemLocObject*>(this) == NULL)
      != (boost::dynamic_pointer_cast<FullMemLocObject>(that) == NULL))
    return boost::dynamic_pointer_cast<FullMemLocObject>(that) != NULL;

  MemLocObjectPtr co = boost::dynamic_pointer_cast<MemLocObject>(that);
  if (co)
    return equalSet(co, pedge, comp, analysis);
  else
    return false;
}

bool MemLocObject::subSet(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is a FuncResultMemLocObject and that is not or vice versa, the sets they denote are not equal
  if ((dynamic_cast<FuncResultMemLocObject*>(this) == NULL)
      != (boost::dynamic_pointer_cast<FuncResultMemLocObject>(that) == NULL))
    return false;
  // If this is a FullMemLocObject and that is not or vice versa, the sets they denote are not equal
  if ((dynamic_cast<FullMemLocObject*>(this) == NULL)
      != (boost::dynamic_pointer_cast<FullMemLocObject>(that) == NULL))
    return false;

  MemLocObjectPtr co = boost::dynamic_pointer_cast<MemLocObject>(that);
  if (co)
    return subSet(co, pedge, comp, analysis);
  else
    return false;
}

//// Returns true if this object is live at the given part and false otherwise
//bool MemLocObject::isLiveAO(PartEdgePtr pedge) {
////cout << "MemLocObject::isLiveAO(), region="<<region->str()<<endl;
//  return region->isLiveAO(pedge);
//}

// General version of isLive that accounts for framework details before routing the call to the derived class'
// isLiveML check. Specifically, it routes the call through the composer to make sure the isLiveML call gets the
// right PartEdge
bool MemLocObject::isLive(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis)
//{ return isLiveAO(pedge); }
    {
  return getRegion()->isLive(pedge, comp, analysis)
      && (!getIndex() || getIndex()->isLive(pedge, comp, analysis));
}

//// Computes the meet of this and that and saves the result in this
//// returns true if this causes this to change and false otherwise
//bool MemLocObject::meetUpdateAO(MemLocObjectPtr that, PartEdgePtr pedge) {
//  bool modified = false;
//  modified = region->meetUpdateAO(that->getRegion(), pedge) || modified;
//  if(index) modified = index->meetUpdateAO(that->getIndex(), pedge) || modified;
//  return modified;
//}

// General version of meetUpdate() that accounts for framework details before routing the call to the derived class'
// meetUpdateML check. Specifically, it routes the call through the composer to make sure the meetUpdateML
// call gets the right PartEdge
bool MemLocObject::meetUpdate(MemLocObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  bool modified = false;
  modified = getRegion()->meetUpdate(that->getRegion(), pedge, comp, analysis)
      || modified;
  if (getIndex() != NULLValueObject)
    modified = getIndex()->meetUpdate(that->getIndex(), pedge, comp, analysis)
        || modified;
  return modified;
}

bool MemLocObject::meetUpdate(AbstractObjectPtr that, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is a FuncResultMemLocObject and that is not or vice versa, we'll need to make this object full
  if ((dynamic_cast<FuncResultMemLocObject*>(this) == NULL)
      != (boost::dynamic_pointer_cast<FuncResultMemLocObject>(that) == NULL))
    assert(0);
  // If this is a FullMemLocObject and that is not or vice versa, we'll need to make this object full
  if ((dynamic_cast<FullMemLocObject*>(this) == NULL)
      != (boost::dynamic_pointer_cast<FullMemLocObject>(that) == NULL))
    assert(0);

  MemLocObjectPtr ml = boost::dynamic_pointer_cast<MemLocObject>(that);
  assert(ml);
  return meetUpdate(ml, pedge, comp, analysis);
}

//// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
//bool MemLocObject::isFullAO(PartEdgePtr pedge) {
// return region->isFullAO(pedge) && (!index || index->isFullAO(pedge));
//}
//
//// Returns whether this AbstractObject denotes the empty set.
//bool MemLocObject::isEmptyAO(PartEdgePtr pedge) {
//  return region->isEmptyAO(pedge) && (!index || index->isEmptyAO(pedge));
//}

// General versions of isFull() and isEmpty that account for framework details before routing the call to the
// derived class' isFull() and isEmpty()  check. Specifically, it routes the call through the composer to make
// sure the isFull(PartEdgePtr) and isEmpty(PartEdgePtr) call gets the right PartEdge.
// These functions are just aliases for the real implementations in AbstractObject
bool MemLocObject::isFull(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis)
//{ return isFullAO(pedge); }
// This MemLocObject is full if is region is Full and its index is either NULL or says its Full
    {
  return getRegion()->isFull(pedge, comp, analysis)
      && (!getIndex() || getIndex()->isFull(pedge, comp, analysis));
}

bool MemLocObject::isEmpty(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis)
//{ return isEmptyAO(pedge); }
// This MemLocObject is Empty if either its region is empty or its index is empty,
// with a NULL index considered to denote the full set
    {
  return getRegion()->isEmpty(pedge, comp, analysis)
      || (!getIndex() && getIndex()->isEmpty(pedge, comp, analysis));
}

// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
bool MemLocObject::isConcrete() {
  return getRegion()->isConcrete()
      && (!getIndex() || getIndex()->isConcrete());
}

// Returns the number of concrete values in this set
int MemLocObject::concreteSetSize() {
  return getRegion()->concreteSetSize()
      && (getIndex() ? getIndex()->concreteSetSize() : 1);
}

// Allocates a copy of this object and returns a shared pointer to it
MemLocObjectPtr MemLocObject::copyAOType() const {
  return boost::make_shared<MemLocObject>(*this);
}

// Allocates a copy of this object and returns a regular pointer to it
MemLocObject* MemLocObject::copyMLPtr() const {
  return new MemLocObject(*this);
}

std::string MemLocObject::str(std::string indent) const { // pretty print for the object
  ostringstream oss;
  oss << "[MemLocObject region=" << getRegion()->str(indent + "    ");
  if (getIndex()) {
    oss << ", " << endl << indent << "             index=" << getIndex()->str(indent + "    ");
  }
  oss << "]";
  return oss.str();
}

// Returns whether all instances of this class form a hierarchy. Every instance of the same
// class created by the same analysis must return the same value from this method!
bool MemLocObject::isHierarchy() const {
  //cout << "MemLocObject::isHierarchy() getRegion()->isHierarchy()="<<getRegion()->isHierarchy()<<", getIndex()->isHierarchy()="<<(getIndex()? getIndex()->isHierarchy(): -1)<<endl;
  // MemLocs form a hierarchy only if their regions and indexes do
  return getRegion()->isHierarchy() && (!getIndex() || getIndex()->isHierarchy());
}

// Returns a key that uniquely identifies this particular AbstractObject in the
// set hierarchy.
const AbstractionHierarchy::hierKeyPtr& MemLocObject::getHierKey() const {
  // The MemLoc denoted by region and index is essentially the intersection of the
  // constraints imposed by each one. Thus, the key of a MemLoc combines
  // their individual keys.
  //dbg << "MemLocObject::getHierKey() isHierKeyCached="<<isHierKeyCached<<endl;
  if (!isHierKeyCached) {
    AbstractionHierarchyPtr hierRegion = boost::dynamic_pointer_cast<AbstractionHierarchy>(getRegion());
    ROSE_ASSERT(hierRegion);

    ((MemLocObject*) this)->cachedHierKey = boost::make_shared<AOSHierKey>(((MemLocObject*) this)->shared_from_this(), hierRegion->getHierKey());

    if (getIndex()) {
      AbstractionHierarchyPtr hierIndex = boost::dynamic_pointer_cast<AbstractionHierarchy>(getIndex());
      ROSE_ASSERT(hierIndex);

      //scope s(txt()<< "index="<<getIndex()->str());
      ((MemLocObject*) this)->cachedHierKey->add(hierIndex->getHierKey()->begin(), hierIndex->getHierKey()->end());
    }

    ((MemLocObject*) this)->isHierKeyCached = true;
  }
  return cachedHierKey;
}

/* ##################################
   ##### FuncResultMemLocObject   #####
   ################################## */

// Special MemLocObject used internally by the framework to associate with the return value of a function
// This is just a MemLoc dedicated to wrapping FuncResultMemLocRegion
FuncResultMemLocObject::FuncResultMemLocObject(Function func) :
    MemLocObject(boost::make_shared<FuncResultMemRegionObject>(func),
        NULLValueObject, NULL) {
}

FuncResultMemLocObject::FuncResultMemLocObject(
    const FuncResultMemLocObject& that) :
    MemLocObject(that) {
}

MemLocObjectPtr FuncResultMemLocObject::copyAOType() const {
  return boost::make_shared<FuncResultMemLocObject>(*this);
}

// Returns a key that uniquely identifies this particular AbstractObject in the
// set hierarchy.
const AbstractionHierarchy::hierKeyPtr& FuncResultMemLocObject::getHierKey() const {
  AbstractionHierarchyPtr hierRegion = boost::dynamic_pointer_cast<
      AbstractionHierarchy>(getRegion());
  ROSE_ASSERT(hierRegion);
  return hierRegion->getHierKey();
}

/* ##########################
 #### FullMemLocObject ####
   ########################## */

bool FullMemLocObject::mayEqualAO(MemLocObjectPtr o, PartEdgePtr pedge) {
  return true;
}

bool FullMemLocObject::mustEqualAO(MemLocObjectPtr o, PartEdgePtr pedge) {
  return false;
}

bool FullMemLocObject::equalSetAO(MemLocObjectPtr o, PartEdgePtr pedge) {
  return isFullAO(pedge);
}

bool FullMemLocObject::subSetAO(MemLocObjectPtr o, PartEdgePtr pedge) {
  return isFullAO(pedge);
}

MemLocObjectPtr FullMemLocObject::copyAOType() const {
  return boost::make_shared<FullMemLocObject>();
}

bool FullMemLocObject::isLiveAO(PartEdgePtr pedge) {
  return true;
}

bool FullMemLocObject::meetUpdateAO(MemLocObjectPtr that, PartEdgePtr pedge) {
  return false;
}

bool FullMemLocObject::isFullAO(PartEdgePtr pedge) {
  return true;
}

bool FullMemLocObject::isEmptyAO(PartEdgePtr pedge) {
  return false;
}

// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
bool FullMemLocObject::isConcrete() {
  return false;
}

// Returns the number of concrete values in this set
int FullMemLocObject::concreteSetSize() {
  return -1;
}

string FullMemLocObject::str(string indent) const {
  return "FullMemLocObject";
}

/* ################################
   ##### CombinedMemLocObject   #####
   ################################ * /

/*template <bool defaultMayEq>
 boost::shared_ptr<CombinedMemLocObject<defaultMayEq> > CombinedMemLocObject<defaultMayEq>::create(const std::list<MemLocObjectPtr>& memLocs)
 {
 //dbg << "CombinedMemLocObject<"<<defaultMayEq<<">::create() generic "<< endl;
 return boost::make_shared<CombinedMemLocObject<defaultMayEq> >(memLocs);
 }

 // Sriram: gcc 4.1.2 complains of undefined references to unused to template functions
 // fix: explicit template instantiation
 template boost::shared_ptr<CombinedMemLocObject<true> > CombinedMemLocObject<true>::create(const std::list<MemLocObjectPtr>& memLocs);
 template boost::shared_ptr<CombinedMemLocObject<false> > CombinedMemLocObject<false>::create(const std::list<MemLocObjectPtr>& memLocs);
 * /

template<bool defaultMayEq>
SgNode* CombinedMemLocObject<defaultMayEq>::getBase() const {
  // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
  SgNode* base = NULL;
  for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
      ml != memLocs.end(); ml++) {
    if (ml == memLocs.begin())
      base = (*ml)->getBase();
    else if (base != (*ml)->getBase())
      return NULL;
  }

  return base;
}

template<bool defaultMayEq>
MemRegionObjectPtr CombinedMemLocObject<defaultMayEq>::getRegion() const {
  //cout << "CombinedMemLocObject::getRegion("<<this<<")"<<endl;
  if (region == NULLMemRegionObject) {
    // Collect all the memRegions of the memLocs in this object and create a CombinedMemRegionObject out of them
    std::list<MemRegionObjectPtr> memRegions;
    for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
        ml != memLocs.end(); ml++)
      memRegions.push_back((*ml)->getRegion());

    ((CombinedMemLocObject<defaultMayEq>*) this)->region = boost::make_shared<
        CombinedMemRegionObject<defaultMayEq> >(memRegions);
  }
  return region;
}

template<bool defaultMayEq>
ValueObjectPtr CombinedMemLocObject<defaultMayEq>::getIndex() const {
  if (index == NULLValueObject) {
    // Collect all the indexes of the memLocs in this object and create a CombinedValueObject out of them
    std::list<ValueObjectPtr> indexes;
    for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
        ml != memLocs.end(); ml++)
      indexes.push_back((*ml)->getIndex());

    ((CombinedMemLocObject<defaultMayEq>*) this)->index = boost::make_shared<
        CombinedValueObject<defaultMayEq> >(indexes);
  }
  return index;
}

template<bool defaultMayEq>
void CombinedMemLocObject<defaultMayEq>::add(MemLocObjectPtr memLoc) {
  memLocs.push_back(memLoc);
}

// Returns whether this object may/must be equal to o within the given Part p
template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::mayEqual(MemLocObjectPtr o,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<CombinedMemLocObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemLocObject<defaultMayEq> >(o);
  assert(that);

  // If the two combination objects include different numbers of MemLocObjects, say that they may be equal since
  // we can't be sure either way.
  if (memLocs.size() != that->memLocs.size())
    return true;

  // Compare all the pairs of MemLocObjects in memLocs and that.memLocs, returning defaultMayEq if any pair
  // returns defaultMayEq since we're looking for the tightest (if defaultMayEq=false) / loosest (if defaultMayEq=true)
  // answer that any MemLocObject in memLocs can give
  for (list<MemLocObjectPtr>::iterator thisIt = memLocs.begin(), thatIt =
      that->memLocs.begin(); thisIt != memLocs.end(); thisIt++, thatIt++) {
    //if((*thisIt)->mayEqualAO(*thatIt, pedge) == defaultMayEq) return defaultMayEq;
    if ((*thisIt)->mayEqual(*thatIt, pedge, comp, analysis) == defaultMayEq)
      return defaultMayEq;
  }

  return !defaultMayEq;
}

template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::mustEqual(MemLocObjectPtr o,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<CombinedMemLocObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemLocObject<defaultMayEq> >(o);
  assert(that);

  // If the two combination  objects include different numbers of MemLocObjects, say that they are not must equal since
  // we can't be sure either way.
  if (memLocs.size() != that->memLocs.size())
    return false;

  // Compare all the pairs of MemLocObjects in memLocs and that.memLocs, returning !defaultMayEq if any pair
  // returns !defaultMayEqual since we're looking for the tightest answer that any MemLocObject in memLocs can give
  for (list<MemLocObjectPtr>::iterator thisIt = memLocs.begin(), thatIt =
      that->memLocs.begin(); thisIt != memLocs.end(); thisIt++, thatIt++) {
    //if((*thisIt)->mustEqualAO(*thatIt, pedge) == !defaultMayEq) return !defaultMayEq;
    if ((*thisIt)->mustEqual(*thatIt, pedge, comp, analysis) == !defaultMayEq)
      return !defaultMayEq;
  }

  return defaultMayEq;
}

// Returns whether the two abstract objects denote the same set of concrete objects
template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::equalSet(MemLocObjectPtr o,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<CombinedMemLocObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemLocObject<defaultMayEq> >(o);
  assert(that);
  assert(memLocs.size() == that->memLocs.size());

  // Two unions and intersections denote the same set of their components individually denote the same set
  // (we can get a more precise answer if we could check set containment relations as well)
  list<MemLocObjectPtr>::const_iterator mlThis = memLocs.begin();
  list<MemLocObjectPtr>::const_iterator mlThat = that->memLocs.begin();
  for (; mlThis != memLocs.end(); mlThis++, mlThat++)
    //if(!(*mlThis)->equalSetAO(*mlThat, pedge)) return false;
    if (!(*mlThis)->equalSet(*mlThat, pedge, comp, analysis))
      return false;
  return true;
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::subSet(MemLocObjectPtr o,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<CombinedMemLocObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemLocObject<defaultMayEq> >(o);
  assert(that);
  assert(memLocs.size() == that->memLocs.size());

  // Compare all the pairs of MemLocObjects in memLocs and that.memLocs, returning defaultMayEq if any pair
  // returns defaultMayEq since we're looking for the tightest (if defaultMayEq=false) / loosest (if defaultMayEq=true)
  // answer that any MemLocObject in memLocs can give
  for (list<MemLocObjectPtr>::iterator thisIt = memLocs.begin(), thatIt =
      that->memLocs.begin(); thisIt != memLocs.end(); thisIt++, thatIt++) {
    //if((*thisIt)->subSetAO(*thatIt, pedge) == defaultMayEq) return defaultMayEq;
    if ((*thisIt)->subSet(*thatIt, pedge, comp, analysis) == defaultMayEq)
      return defaultMayEq;
  }
  return !defaultMayEq;
}

// Returns true if this object is live at the given part and false otherwise
template<bool defaultMayEq>
// General version of isLive that accounts for framework details before routing the call to the derived class'
// isLiveML check. Specifically, it routes the call through the composer to make sure the isLiveML call gets the
// right PartEdge
bool CombinedMemLocObject<defaultMayEq>::isLive(PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
//cout << "CombinedMemLocObject<defaultMayEq>::isLiveML union="<<defaultMayEq<<", #memLocs="<<memLocs.size()<<endl;
  // If this is a union type (defaultMayEq=true), an object is live if any of its components are live (weakest constraint)
  // If this is an intersection type (defaultMayEq=false), an object is dead if any of its components are dead (strongest constraint)
  for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
      ml != memLocs.end(); ml++) {
//    cout << "  ml="<<(*ml)->str()<<endl;
    //if((*ml)->isLiveAO(pedge) == defaultMayEq) { / *cout << "    LIVE"<<endl; * /return defaultMayEq; }
    if ((*ml)->isLive(pedge, comp, analysis) == defaultMayEq) { / *cout << "    LIVE"<<endl; * /
      return defaultMayEq;
    }
  }

  return !defaultMayEq;
}

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::meetUpdate(MemLocObjectPtr o,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<CombinedMemLocObject<defaultMayEq> > that =
      boost::dynamic_pointer_cast<CombinedMemLocObject<defaultMayEq> >(o);
  assert(that);
  assert(memLocs.size() == that->memLocs.size());
  bool modified = false;

  // Perform the meetUpdate operation on all member MemLocs
  list<MemLocObjectPtr>::const_iterator mlThis = memLocs.begin();
  list<MemLocObjectPtr>::const_iterator mlThat = that->memLocs.begin();
  for (; mlThis != memLocs.end(); mlThis++, mlThat++)
    //modified = (*mlThis)->meetUpdateAO(*mlThat, pedge) || modified;
    modified = (*mlThis)->meetUpdate(*mlThat, pedge, comp, analysis)
        || modified;
  return modified;
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::isFull(PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is a union type (defaultMayEq=true), an object is full if any of its components are full (weakest constraint)
  // If this is an intersection type (defaultMayEq=false), an object is not full if any of its components are not full (strongest constraint)
  for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
      ml != memLocs.end(); ml++)
    //if((*ml)->isFullAO(pedge) == defaultMayEq) return defaultMayEq;
    if ((*ml)->isFull(pedge, comp, analysis) == defaultMayEq)
      return defaultMayEq;

  return !defaultMayEq;
}

// Returns whether this AbstractObject denotes the empty set.
template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::isEmpty(PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is a union type (defaultMayEq=true), an object is not empty if any of its components are not empty (weakest constraint)
  // If this is an intersection type (defaultMayEq=false), an object is empty if any of its components are empty (strongest constraint)
  for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
      ml != memLocs.end(); ml++)
    //if((*ml)->isEmptyAO(pedge) != defaultMayEq) return !defaultMayEq;
    if ((*ml)->isEmpty(pedge, comp, analysis) != defaultMayEq)
      return !defaultMayEq;

  return defaultMayEq;
}

// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::isConcrete() {
  for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
      ml != memLocs.end(); ml++)
    if (!(*ml)->isConcrete())
      return false;
  return true;
}

// Returns the number of concrete values in this set
template<bool defaultMayEq>
int CombinedMemLocObject<defaultMayEq>::concreteSetSize() {
  // This is an over-approximation of the set size that assumes that all the concrete sets of
  // the sub-MemLocs are disjoint
  int size = 0;
  for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
      ml != memLocs.end(); ml++)
    size += (*ml)->concreteSetSize();
  return size;
}

// Allocates a copy of this object and returns a pointer to it
template<bool defaultMayEq>
MemLocObjectPtr CombinedMemLocObject<defaultMayEq>::copyAOType() const {
  return boost::make_shared<CombinedMemLocObject>(memLocs);
}

template<bool defaultMayEq>
std::string CombinedMemLocObject<defaultMayEq>::str(
    std::string indent) const {
  ostringstream oss;
  if (memLocs.size() > 1)
    oss << "[" << (defaultMayEq ? "UnionML" : "IntersectML") << ": ";
  if (memLocs.size() > 1)
    oss << endl;
  for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
      ml != memLocs.end();) {
    if (ml != memLocs.begin())
      oss << indent << "&nbsp;&nbsp;&nbsp;&nbsp;";
    oss << (*ml)->str(indent + "&nbsp;&nbsp;&nbsp;&nbsp;");
    ml++;
    if (ml != memLocs.end())
      oss << endl;
  }
  if (memLocs.size() > 1)
    oss << "]";

  return oss.str();
}

// Returns whether all instances of this class form a hierarchy. Every instance of the same
// class created by the same analysis must return the same value from this method!
template<bool defaultMayEq>
bool CombinedMemLocObject<defaultMayEq>::isHierarchy() const {
  // Combined MemLocs form hierarchy if:
  // - All the sub-MemLocs form hierarchies of their own, AND
  // - The combination is an intersection.
  //   If the combination is a union then consider the following:
  //            MLa     MLb
  //   comb1 = {a,b}, {w, x}
  //   comb2 = {a,b}, {y, z}
  //   Note that MLs from analyses a and b are either identical sets or disjoint
  //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
  //   not hold unless we work out a new hierarchy for MLb.

  // Unions are not hierarchical unless they're singletons
  if (defaultMayEq) {
    if (memLocs.size() == 1)
      return (*memLocs.begin())->isHierarchy();
    else
      return false;
  }

  for (list<MemLocObjectPtr>::const_iterator ml = memLocs.begin();
      ml != memLocs.end(); ml++)
    if (!(*ml)->isHierarchy())
      return false;
  return true;
}

// Returns a key that uniquely identifies this particular AbstractObject in the
// set hierarchy.
template<bool defaultMayEq>
const AbstractionHierarchy::hierKeyPtr& CombinedMemLocObject<defaultMayEq>::getHierKey() const {
  // The intersection of multiple objects is just a filtering process from the full
  // set of objects to the exact one, with each key in the hierarchy further filtering
  // the set down. As such, a hierarchical key for the intersection of multiple objects
  // is just the concatenation of the keys of all the individual objects.
  //dbg << "CombinedMemLocObject<defaultMayEq>::getHierKey() isHierKeyCached="<<isHierKeyCached<<", #memLocs="<<memLocs.size()<<endl;
  //cout << "CombinedMemLocObject<defaultMayEq>::getHierKey() isHierKeyCached="<<isHierKeyCached<<", #memLocs="<<memLocs.size()<<endl;
  if (!isHierKeyCached) {
    / *    ((CombinedMemLocObject<defaultMayEq>*)this)->cachedHierKey = boost::make_shared<AOSHierKey>(((CombinedMemLocObject<defaultMayEq>*)this)->shared_from_this());

     // Records whether endOfHierarchy was set on any of the key
     bool endOfHierarchyAny=false;
     for(list<MemLocObjectPtr>::const_iterator ml=memLocs.begin(); ml!=memLocs.end(); ml++) {
     //dbg << "cur Key="<<(*ml)->getHierKey()<<endl;
     ((CombinedMemLocObject<defaultMayEq>*)this)->cachedHierKey->add((*ml)->getHierKey()->begin(), (*ml)->getHierKey()->end());
     }* /
    list<hierKeyPtr> subHierKeys;
    for (list<MemLocObjectPtr>::const_iterator i = memLocs.begin(); i != memLocs.end(); i++) {
      subHierKeys.push_back((*i)->getHierKey());
    }

    ((CombinedMemLocObject<defaultMayEq>*) this)->cachedHierKey = boost::make_shared<IntersectHierKey>(subHierKeys);

    ((CombinedMemLocObject<defaultMayEq>*) this)->isHierKeyCached = true;
  }
  //dbg << "cachedHierKey="<<cachedHierKey<<endl;
  //cout << ">>> isHierKeyCached=" << isHierKeyCached << "   cachedHierKey="<< cachedHierKey << endl;
  return cachedHierKey;
}*/

/* ##############################
   ##### MappedMemLocObject   #####
   ############################## */

//  template<class Key>
//  SgNode* MappedMemLocObject<Key>::getBase() const {
//    // Returns the base SgNode shared by all the MemLocs in this object or NULL, if the base SgNodes are different
//    SgNode* base = NULL;
//    for (typename map<Key, MemLocObjectPtr>::const_iterator it =
//        aoMap.begin(); it != aoMap.end(); ++it) {
//      if (it == aoMap.begin())
//        base = it->second->getBase();
//      else if (base != it->second->getBase())
//        return NULL;
//    }
//
//    return base;
//  }
//
template<class Key>
MemRegionObjectPtr MappedMemLocObject<Key>::getRegion() const {
  if (MappedMemLocObject<Key>::region == NULLMemRegionObject) {
    // Collect all the memRegions of the memLocs in this object and create a CombinedMemRegionObject out of them
    map<Key, MemRegionObjectPtr> memRegions;
    for (typename map<Key, MemLocObjectPtr>::const_iterator it = MappedMemLocObject<Key>::aoMap.begin();
         it != MappedMemLocObject<Key>::aoMap.end(); ++it) {
      memRegions[it->first] = it->second->getRegion();
    }

    ((MappedMemLocObject<Key>*) this)->region =
        boost::make_shared<MappedMemRegionObject<Key> >(MappedMemLocObject<Key>::ui, MappedMemLocObject<Key>::analysis, memRegions);
  }
  return MappedMemLocObject<Key>::region;
}

template<class Key>
ValueObjectPtr MappedMemLocObject<Key>::getIndex() const {
  if (MappedMemLocObject<Key>::index == NULLValueObject) {
    // Collect all the indexes of the memlocs in aoMap and create a CombinedValueObject out of them
    map<Key, ValueObjectPtr> indexes;
    for (typename map<Key, MemLocObjectPtr>::const_iterator it = MappedMemLocObject<Key>::aoMap.begin();
         it != MappedMemLocObject<Key>::aoMap.end(); ++it) {
      indexes[it->first] = it->second->getIndex();
    }

    ((MappedMemLocObject<Key>*) this)->index =
        boost::make_shared<MappedValueObject<Key> >(MappedMemLocObject<Key>::ui, MappedMemLocObject<Key>::analysis, indexes);
  }
  return MappedMemLocObject<Key>::index;
}
//
////! Method to add mls to the map.
////! MLs that are full are never added to the map.
////! If ml_p is FullML or ml_p->isFullML=true then mapped ML is set to full only if mostAccurate=false.
//  template<class Key>
//  void MappedMemLocObject<Key>::add(Key key, MemLocObjectPtr ml_p,
//      PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
//    // If the object is already full don't add anything
//    //if(isUnion() && isFullAO(pedge)) return;
//    if (isUnion() && isFull(pedge, comp, analysis))
//      return;
//
//    // If the ml_p is not full add/update the map
//    //if(!ml_p->isFullAO(pedge)) {
//    if (!ml_p->isFull(pedge, comp, analysis)) {
//      aoMap[key] = ml_p;
//    } else {
//      nFull++;
//      if (isUnion())
//        setMLToFull();
//    }
//  }
//
//  template<class Key>
//  bool MappedMemLocObject<Key>::mayEqualMLWithKey(Key key,
//      const map<Key, MemLocObjectPtr>& thatMLMap, PartEdgePtr pedge,
//      Composer* comp, ComposedAnalysis* analysis) {
//    typename map<Key, MemLocObjectPtr>::const_iterator s_it;
//    s_it = thatMLMap.find(key);
//    if (s_it == thatMLMap.end())
//      return true;
//    //return aoMap[key]->mayEqualAO(s_it->second, pedge);
//    return aoMap[key]->mayEqual(s_it->second, pedge, comp, analysis);
//  }
//
////! Two ML objects are may equals if there is atleast one execution or sub-exectuion
////! in which they represent the same memory location.
////! Analyses are conservative as they start with full set of executions.
////! Dataflow facts (predicates) shrink the set of sub-executions.
////! We do not explicity store set of sub-executions and they are described
////! by the abstract objects computed from dataflow fact exported by the analysis.
////! Unless the analyses discover otherwise, the conservative answer for mayEqualML is true.
////! Mapped MLs are keyed using either ComposedAnalysis* or PartEdgePtr.
////! Each keyed ML object correspond to some dataflow facts computed by Key=Analysis* or
////! computed at Key=PartEdgePtrthat describes some sets of executions.
////! MayEquality check on mapped ML is performed on intersection of sub-executions
////! or union of sub-executions over the keyed ML objects.
//  template<class Key>
////bool MappedMemLocObject<Key>::mayEqualAO(MemLocObjectPtr thatML, PartEdgePtr pedge) {
//  bool MappedMemLocObject<Key>::mayEqual(MemLocObjectPtr thatML,
//      PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
//    // scope s(txt()<<"MappedMemLocObject::mayEqualML", scope::medium);
//    boost::shared_ptr<MappedMemLocObject<Key> > thatML_p =
//        boost::dynamic_pointer_cast<MappedMemLocObject<Key> >(
//            thatML);
//    assert(thatML_p);
//
//    // dbg << "thisML=" << str() << endl;
//    // dbg << "thatML=" << thatML_p->str() << endl;
//
//    // This object denotes full set of ML (full set of executions)
//    //if(isFullAO(pedge)) return true;
//    if (isFull(pedge, comp, analysis))
//      return true;
//
//    // denotes empty set
//    //if(isEmptyAO(pedge)) return false;
//    if (isEmpty(pedge, comp, analysis))
//      return false;
//
//    // presence of one more full objects will result in full set over union
//    if (isUnion() && nFull > 0)
//      return true;
//
//    // Two cases reach here [1] isUnion()=true && nFull_ML=0 [2] intersect=true && nFullML=0 or nFull_ML!=0.
//    // For both cases iterate on the ML map and discharge the mayEqualML query to individual objects
//    // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
//    const map<Key, MemLocObjectPtr> thatMLMap = thatML_p->getMemLocsMap();
//    typename map<Key, MemLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      bool isMayEq = mayEqualMLWithKey(it->first, thatMLMap, pedge, comp,
//          analysis);
//
//      // dbg << "key=" << (it->first)->str() << ", isMayEq=" << isMayEq << endl;
//
//      // 1. Union of sub-executions and the object does not contain any full objects.
//      // If the discharged query comes back as true for this case then we have found atleast one execution
//      // under which the two objects are same and the set can only grow and the result of this query is not going
//      // to change due to union.
//      // If false we iterate further as any ML can add more executions under which the objects are may equals.
//      if (isUnion() && isMayEq == true)
//        return true;
//
//      // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
//      // The sub-executions are intersected and therefore it does not matter if we have full objects.
//      // If the discharged query returns false then return false.
//      // We did not find one execution in which the two objects are may equals.
//      // Note that set of executions are contained over keyed objects (analyses are conservative).
//      // This set only shrinks during intersection and it is not going to affect the result of this query.
//      // If it returns true iterate further as some executions corresponding to true may be dropped.
//      else if (isIntersection() && isMayEq == false)
//        return false;
//    }
//
//    // All the keyed objects returned false for the discharged query under union.
//    // We haven't found a single execution under which the two objects are may equals.
//    if (isUnion())
//      return false;
//    // All the keyed objects returned true for the discharged query under intersection.
//    // We have atleast one execution in common in which the two objects are may equals.
//    else if (isIntersection())
//      return true;
//    else
//      assert(0);
//  }
//
//  template<class Key>
//  bool MappedMemLocObject<Key>::mustEqualMLWithKey(Key key,
//      const map<Key, MemLocObjectPtr>& thatMLMap, PartEdgePtr pedge,
//      Composer* comp, ComposedAnalysis* analysis) {
//    typename map<Key, MemLocObjectPtr>::const_iterator s_it;
//    s_it = thatMLMap.find(key);
//    if (s_it == thatMLMap.end())
//      return false;
//    //return aoMap[key]->mustEqualAO(s_it->second, pedge);
//    return aoMap[key]->mustEqual(s_it->second, pedge, comp, analysis);
//  }
//
////! Two ML objects are must equals if they represent the same single memory
////! location on all executions.
////! Analyses are conservative as they start with full set of executions.
////! Dataflow facts (predicates) shrink the set of sub-executions.
////! We do not explicity store set of sub-executions and they are described 
////! by the abstract objects computed from dataflow fact exported by the analysis.
////! Unless the analyses discover otherwise conservative answer for mustEqualML is false.
////! Mapped MLs are keyed using either ComposedAnalysis* or PartEdgePtr.
////! Each keyed ML object correspond to some dataflow facts computed by Key=Analysis* or 
////! computed at Key=PartEdgePtrthat describes some sets of executions.
////! MustEquality check on mapped ML is performed on intersection (mostAccurate=true) of sub-executions
////! or union (mostAccurate=false) of sub-executions over the keyed ML objects.
//  template<class Key>
////bool MappedMemLocObject<Key>::mustEqualAO(MemLocObjectPtr thatML, PartEdgePtr pedge) {
//  bool MappedMemLocObject<Key>::mustEqual(MemLocObjectPtr thatML,
//      PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
//    boost::shared_ptr<MappedMemLocObject<Key> > thatML_p =
//        boost::dynamic_pointer_cast<MappedMemLocObject<Key> >(
//            thatML);
//    assert(thatML_p);
//
//    // This object denotes full set of ML (full set of executions)
//    //if(isFullAO(pedge)) return false;
//    if (isFull(pedge, comp, analysis))
//      return false;
//
//    // denotes empty set
//    //if(isEmptyAO(pedge)) return false;
//    if (isEmpty(pedge, comp, analysis))
//      return false;
//
//    // presence of one more full objects will result in full set over union
//    if (isUnion() && nFull > 0)
//      return true;
//
//    // Two cases reach here [1] isUnion()=true && nFull_ML=0 [2] intersect=true && nFullML=0 or nFull_ML!=0.
//    // For both cases iterate on the ML map and discharge the mayEqualML query to individual objects
//    // which are answered based on its set of sub-executions (or its dataflow facts) computed by the corresponding analysis.
//    const map<Key, MemLocObjectPtr> thatMLMap = thatML_p->getMemLocsMap();
//    typename map<Key, MemLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      bool isMustEq = mustEqualMLWithKey(it->first, thatMLMap, pedge, comp,
//          analysis);
//
//      // 1. Union of sub-executions and the object does not contain any full objects
//      // If the discharged query comes back as false for this case then we have found atleast one execution
//      // under which the two objects are not same and the set can only grow and the result of this query is not going
//      // to change due to union.
//      // If it returns true we iterate further as any ML can add more executions under which the objects are not must equals.
//      if (isUnion() && isMustEq == false)
//        return false;
//
//      // 2. Intersection of sub-executions and the object may contain full objects (nFull != 0).
//      // The sub-executions are intersected and therefore it does not matter if we have full objects.
//      // If the discharged query returns true then return true.
//      // Under all sub-executions (corresponding to the ML) the two objects must equal.
//      // Note that set of executions are contained over keyed objects as the analyses are conservative.
//      // This set only shrinks during intersection and it is not going to affect the result of this query.
//      // If it returns false iterate further as some executions corresponding to false may be dropped.
//      else if (isIntersection() && isMustEq == true)
//        return true;
//    }
//
//    // All the keyed objects returned true for the discharged query under union.
//    // We haven't found a single execution under which the two objects are not equal.
//    if (isUnion())
//      return true;
//    // All the keyed objects returned false for the discharged query under intersection.
//    // We have atleast one execution in common in which the two objects are not equal.
//    else if (isIntersection())
//      return false;
//    else
//      assert(0);
//  }
//
////! Discharge the query to the corresponding ML
////! If key not found in thatMLMap return false
//  template<class Key>
//  bool MappedMemLocObject<Key>::equalSetMLWithKey(Key key,
//      const map<Key, MemLocObjectPtr>& thatMLMap, PartEdgePtr pedge,
//      Composer* comp, ComposedAnalysis* analysis) {
//    typename map<Key, MemLocObjectPtr>::const_iterator s_it;
//    s_it = thatMLMap.find(key);
//    if (s_it == thatMLMap.end())
//      return false;
//    //return aoMap[key]->equalSetAO(s_it->second, pedge);
//    return aoMap[key]->equalSet(s_it->second, pedge, comp, analysis);
//  }
//
////! Two objects are equal sets if they denote the same set of memory locations
////! The boolean parameter mostAccurate is not releveant as this query is not
////! answered based on union or intersection of sub-executions.
////! Simply discharge the queries to all keyed MemLoc objects
////! If all the discharged queries come back equal then the two objects are equal otherwise not.
//  template<class Key>
////bool MappedMemLocObject<Key>::equalSetAO(MemLocObjectPtr thatML, PartEdgePtr pedge) {
//  bool MappedMemLocObject<Key>::equalSet(MemLocObjectPtr thatML,
//      PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
//    boost::shared_ptr<MappedMemLocObject<Key> > thatML_p =
//        boost::dynamic_pointer_cast<MappedMemLocObject<Key> >(
//            thatML);
//    assert(thatML_p);
//
//    // This object denotes full set of ML (full set of executions)
//    //if(isFullAO(pedge)) return thatML_p->isFullAO(pedge);
//    if (isFull(pedge, comp, analysis))
//      return thatML_p->isFull(pedge, comp, analysis);
//
//    // denotes empty set
//    //if(isEmptyAO(pedge)) return thatML_p->isEmptyAO(pedge);
//    if (isEmpty(pedge, comp, analysis))
//      return thatML_p->isEmpty(pedge, comp, analysis);
//
//    const map<Key, MemLocObjectPtr> thatMLMap = thatML_p->getMemLocsMap();
//    typename map<Key, MemLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      // break even if one of them returns false
//      if (equalSetMLWithKey(it->first, thatMLMap, pedge, comp, analysis)
//          == false)
//        return false;
//    }
//
//    return true;
//  }
//
////! Discharge the query to the corresponding ML
////! If key not found in thatMLMap return true as the
////! keyed object on thatMLMap denotes full set
//  template<class Key>
//  bool MappedMemLocObject<Key>::subSetMLWithKey(Key key,
//      const map<Key, MemLocObjectPtr>& thatMLMap, PartEdgePtr pedge,
//      Composer* comp, ComposedAnalysis* analysis) {
//    typename map<Key, MemLocObjectPtr>::const_iterator s_it;
//    s_it = thatMLMap.find(key);
//    if (s_it == thatMLMap.end())
//      return true;
//    //return aoMap[key]->subSetAO(s_it->second, pedge);
//    return aoMap[key]->subSet(s_it->second, pedge, comp, analysis);
//  }
//
////! This object is a non-strict subset of the other if the set of memory locations denoted by this
////! is a subset of the set of memory locations denoted by that.
////! The boolean parameter mostAccurate is not releveant as this query is not
////! answered based on union or intersection of sub-executions.
////! Simply discharge the queries to all keyed MemLoc objects
////! If all the discharged queries come back true then this is a subset of that otherwise not.
//  template<class Key>
////bool MappedMemLocObject<Key>::subSetAO(MemLocObjectPtr thatML, PartEdgePtr pedge) {
//  bool MappedMemLocObject<Key>::subSet(MemLocObjectPtr thatML,
//      PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
//    boost::shared_ptr<MappedMemLocObject<Key> > thatML_p =
//        boost::dynamic_pointer_cast<MappedMemLocObject<Key> >(
//            thatML);
//    assert(thatML_p);
//
//    // This object denotes full set of ML (full set of executions)
//    //if(isFullAO(pedge)) return thatML_p->isFullAO(pedge);
//    if (isFull(pedge, comp, analysis))
//      return thatML_p->isFull(pedge, comp, analysis);
//
//    // denotes empty set
//    // thatML could be empty or non-empty eitherway this will be a non-strict subset of that.
//    //if(isEmpty(pedge)) return true;
//    if (isEmpty(pedge, comp, analysis))
//      return true;
//
//    // If both objects have the same keys discharge
//    // If this object has a key and that does not then
//    // the keyed object is subset of that (return true) implemented by subsetMLWithKey
//    // If any of the discharged query return false then return false.
//    const map<Key, MemLocObjectPtr> thatMLMap = thatML_p->getMemLocsMap();
//    typename map<Key, MemLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it) {
//      // discharge query
//      // break even if one of them returns false
//      if (subSetMLWithKey(it->first, thatMLMap, pedge, comp, analysis) == false)
//        return false;
//    }
//
//    // If this object doesn't have the key and that object has the key then
//    // return false as this object has full object mapped to the key
//    typename map<Key, MemLocObjectPtr>::const_iterator c_it;
//    for (c_it = thatMLMap.begin(); c_it != thatMLMap.end() && (nFull != 0);
//        ++c_it) {
//      if (aoMap.find(c_it->first) == aoMap.end())
//        return false;
//    }
//
//    return true;
//  }
//
////! Mapped object liveness is determined based on finding executions
////! in which it may be live.
////! It can be answered based on union (mostAccurate=false) or intersection
////! (mostAccurate=true) of executions
////! The conservative answer is to assume that the object is live
//  template<class Key>
////bool MappedMemLocObject<Key>::isLiveAO(PartEdgePtr pedge) {
//// General version of isLive that accounts for framework details before routing the call to the derived class'
//// isLiveML check. Specifically, it routes the call through the composer to make sure the isLiveML call gets the
//// right PartEdge
//  bool MappedMemLocObject<Key>::isLive(PartEdgePtr pedge,
//      Composer* comp, ComposedAnalysis* analysis) {
//    // If this object is full return the conservative answer
////  if(isFullAO(pedge)) return true;
//    if (isFull(pedge, comp, analysis))
//      return true;
//
//    // If it has one or more full objects added to it
//    // and if the object has mostAccurate=false then return true (weakest answer)
//    if (nFull > 0 && isUnion())
//      return true;
//
//    // 1. This object may have have one or more full objects under intersection
//    // 2. This object doesnt have any full objects added to it under union
//    // Under both cases the answer is based on how individual analysis respond to the query
//    typename map<Key, MemLocObjectPtr>::iterator it = aoMap.begin();
//    for (; it != aoMap.end(); ++it) {
//      //bool isLive = it->second->isLiveAO(pedge);
//      bool isLive = it->second->isLive(pedge, comp, analysis);
//      if (isUnion() && isLive == true)
//        return true;
//      else if (isIntersection() && isLive == false)
//        return false;
//    }
//
//    // leftover cases
//    if (isUnion())
//      return false;
//    else if (isIntersection())
//      return true;
//    else
//      assert(0);
//  }
//
////! meetUpdateML performs the join operation of abstractions of two mls
//  template<class Key>
////bool MappedMemLocObject<Key>::meetUpdateAO(MemLocObjectPtr that, PartEdgePtr pedge) {
//  bool MappedMemLocObject<Key>::meetUpdate(MemLocObjectPtr that,
//      PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
//    boost::shared_ptr<MappedMemLocObject<Key> > thatML_p =
//        boost::dynamic_pointer_cast<MappedMemLocObject<Key> >(
//            that);
//    assert(thatML_p);
//
//    // if this object is already full
////  if(isFullAO(pedge)) return false;
//    if (isFull(pedge, comp, analysis))
//      return false;
//
//    // If that object is full set this object to full
////  if(thatML_p->isFullAO(pedge)) {
//    if (thatML_p->isFull(pedge, comp, analysis)) {
//      nFull++;
//      setMLToFull();
//      return true;
//    }
//
//    // Both objects are not full
//    const map<Key, MemLocObjectPtr> thatMLMap = thatML_p->getMemLocsMap();
//
//    typename map<Key, MemLocObjectPtr>::iterator it = aoMap.begin();
//    typename map<Key, MemLocObjectPtr>::const_iterator s_it; // search iterator for thatMLMap
//
//    bool modified = false;
//    while (it != aoMap.end()) {
//      s_it = thatMLMap.find(it->first);
//      // If two objects have the same key then discharge meetUpdate to the corresponding keyed ML objects
//      if (s_it != thatMLMap.end()) {
////      modified = (it->second)->meetUpdateAO(s_it->second, pedge) || modified;
//        modified = (it->second)->meetUpdate(s_it->second, pedge, comp, analysis)
//            || modified;
//      }
//
//      // Remove the current ML object (current iterator it) from the map if the mapepd object is full.
//      // Two cases under which the current ML object can be full.
//      // (1) If current key is not found in thatMLMap then the mapped object
//      // in thatMLMap is full and the meetUpdate of the current ML with that is also full.
//      // (2) meetUpdateML above of the two keyed objects resulted in this mapped object being full.
//      // Under both cases remove the mapped ml from this map
////    if(s_it == thatMLMap.end() || (it->second)->isFullAO(pedge)) {
//      if (s_it == thatMLMap.end()
//          || (it->second)->isFull(pedge, comp, analysis)) {
//        // Current mapped ML has become full as a result of (1) or (2).
//        // Remove the item from the map.
//        // Note that post-increment which increments the iterator and returns the old value for deletion.
//        aoMap.erase(it++);
//        nFull++;
//        modified = true;
//
//        // If union then set this entire object to full and return
//        if (isUnion()) {
//          setMLToFull();
//          return true;
//        }
//      } else
//        ++it;
//    }
//    return modified;
//  }
//
////! Method that sets this mapped object to full
//  template<class Key>
//  void MappedMemLocObject<Key>::setMLToFull() {
//    assert(nFull > 0);
//    if (aoMap.size() > 0)
//      aoMap.clear();
//  }
//
//  template<class Key>
////bool MappedMemLocObject<Key>::isFullAO(PartEdgePtr pedge) {
//  bool MappedMemLocObject<Key>::isFull(PartEdgePtr pedge,
//      Composer* comp, ComposedAnalysis* analysis) {
//    if (nFull > 0 && aoMap.size() == 0)
//      return true;
//    return false;
//  }
//
//  template<class Key>
////bool MappedMemLocObject<Key>::isEmptyAO(PartEdgePtr pedge) {
//  bool MappedMemLocObject<Key>::isEmpty(PartEdgePtr pedge,
//      Composer* comp, ComposedAnalysis* analysis) {
//    if (nFull == 0 && aoMap.size() == 0)
//      return true;
//    return false;
//  }
//
//// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
//  template<class Key>
//  bool MappedMemLocObject<Key>::isConcrete() {
//    typename map<Key, MemLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it)
//      if (!it->second->isConcrete())
//        return false;
//    return true;
//  }
//
//// Returns the number of concrete values in this set
//  template<class Key>
//  int MappedMemLocObject<Key>::concreteSetSize() {
//    // This is an over-approximation of the set size that assumes that all the concrete sets of
//    // the sub-MemLocs are disjoint
//    int size = 0;
//    typename map<Key, MemLocObjectPtr>::iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it)
//      size += it->second->concreteSetSize();
//    return size;
//  }
//
//  template<class Key>
//  MemLocObjectPtr MappedMemLocObject<Key>::copyAO() const {
//    return boost::make_shared<MappedMemLocObject<Key> >(*this);
//  }
//
//  template<class Key>
//  string MappedMemLocObject<Key>::str(string indent) const {
//    ostringstream oss;
//    oss << "<table border=\"1\">";
//    oss << "<tr>";
//    oss << "<th>" << (isUnion() ? "UnionMappedML:" : "IntersectMappedML:")
//        << "</th>";
//    if (nFull > 0 && aoMap.size() == 0)
//      oss << "<th> Full </th> </tr>";
//    else if (nFull == 0 && aoMap.size() == 0)
//      oss << "<th> Empty </th> </tr>";
//    else {
//      oss << "</tr>";
//      typename map<Key, MemLocObjectPtr>::const_iterator it =
//          aoMap.begin();
//      for (; it != aoMap.end(); ++it) {
//        oss << "<tr>";
//        oss << "<td>" << (it->first)->str(indent) << "</td>";
//        oss << "<td>" << (it->second)->str(indent) << "</td>";
//        oss << "</tr>";
//      }
//    }
//    oss << "</table>";
//    return oss.str();
//  }
//
//// Returns whether all instances of this class form a hierarchy. Every instance of the same
//// class created by the same analysis must return the same value from this method!
//  template<class Key>
//  bool MappedMemLocObject<Key>::isHierarchy() const {
//    // Combined MemLocs form hierarchy if:
//    // - All the sub-MemLocs form hierarchies of their own, AND
//    // - The combination is an intersection.
//    //   If the combination is a union then consider the following:
//    //            MLa     MLb
//    //   comb1 = {a,b}, {w, x}
//    //   comb2 = {a,b}, {y, z}
//    //   Note that MLs from analyses a and b are either identical sets or disjoint
//    //   However, comb1 U comb2 = {a, b, w, x, y, z}, for which this property does
//    //   not hold unless we work out a new hierarchy for MLb.
//
//    // Unions are not hierarchical unless they're singletons
//    if (isUnion()) {
//      if (aoMap.size() == 1)
//        return aoMap.begin()->second->isHierarchy();
//      else
//        return false;
//    }
//
//    typename map<Key, MemLocObjectPtr>::const_iterator it;
//    for (it = aoMap.begin(); it != aoMap.end(); ++it)
//      if (!it->second->isHierarchy())
//        return false;
//    return true;
//  }
//
//// Returns a key that uniquely identifies this particular AbstractObject in the 
//// set hierarchy.
//  template<class Key>
//  const AbstractionHierarchy::hierKeyPtr& MappedMemLocObject<Key>::getHierKey() const {
//    // The intersection of multiple objects is just a filtering process from the full
//    // set of objects to the exact one, with each key in the hierarchy further filtering
//    // the set down. As such, a hierarchical key for the intersection of multiple objects
//    // is just the concatenation of the keys of all the individual objects.
//    if (!isHierKeyCached) {
//      /*((MappedMemLocObject<Key>*)this)->cachedHierKey = boost::make_shared<AOSHierKey>(((MappedMemLocObject<Key>*)this)->shared_from_this());
//       typename map<Key, MemLocObjectPtr>::const_iterator it;
//       for(it = aoMap.begin(); it != aoMap.end(); ++it) {
//       AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(it->second);
//       ROSE_ASSERT(hierIt);
//
//       ((MappedMemLocObject<Key>*)this)->cachedHierKey->add(hierIt->getHierKey()->begin(), hierIt->getHierKey()->end());
//       }*/
//      map<Key, hierKeyPtr> subHierKeys;
//      for(typename map<Key, MemLocObjectPtr>::const_iterator it = aoMap.begin();
//          it != aoMap.end(); ++it) {
//        AbstractionHierarchyPtr hierIt = boost::dynamic_pointer_cast<AbstractionHierarchy>(it->second);
//        ROSE_ASSERT(hierIt);
//        subHierKeys[it->first] = hierIt->getHierKey();
//      }
//
//      ((MappedMemLocObject<Key>*) this)->cachedHierKey =
//          boost::make_shared<IntersectMappedHierKey<Key> >(subHierKeys);
//
//      ((MappedMemLocObject<Key>*) this)->isHierKeyCached = true;
//    }
//    return cachedHierKey;
//  }

/* #############################
 # PartEdgeUnionMemLocObject #
   ############################# */

PartEdgeUnionMemLocObject::PartEdgeUnionMemLocObject() :
    MemLocObject(NULL) {
}

PartEdgeUnionMemLocObject::PartEdgeUnionMemLocObject(
    const PartEdgeUnionMemLocObject& thatML) :
    MemLocObject(thatML), unionML_p(thatML.copyAOType()) {
}

SgNode* PartEdgeUnionMemLocObject::getBase() const {
  return unionML_p->getBase();
}

void PartEdgeUnionMemLocObject::add(MemLocObjectPtr ml_p, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  // If this is the very first object
  if (!unionML_p)
    unionML_p = ml_p->copyAOType();
  // If Full return without adding
  //else if(isFullAO(pedge)) return;
  else if (isFull(pedge, comp, analysis))
    return;
  // Else meetUpdate with the existing unionML_p
  //else unionML_p->meetUpdateAO(ml_p, pedge);
  else
    unionML_p->meetUpdate(ml_p, pedge, comp, analysis);
}

//bool PartEdgeUnionMemLocObject::mayEqualAO(MemLocObjectPtr that, PartEdgePtr pedge) {
bool PartEdgeUnionMemLocObject::mayEqual(MemLocObjectPtr that,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<PartEdgeUnionMemLocObject> thatML_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemLocObject>(that);
  assert(thatML_p);

  assert(unionML_p);
  //return unionML_p->mayEqualAO(thatML_p->getUnionAO(), pedge);
  return unionML_p->mayEqual(thatML_p->getUnionAO(), pedge, comp, analysis);
}

//bool PartEdgeUnionMemLocObject::mustEqualAO(MemLocObjectPtr that, PartEdgePtr pedge) {
bool PartEdgeUnionMemLocObject::mustEqual(MemLocObjectPtr that,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<PartEdgeUnionMemLocObject> thatML_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemLocObject>(that);
  assert(thatML_p);

  assert(unionML_p);
  //return unionML_p->mustEqualAO(thatML_p->getUnionAO(), pedge);
  return unionML_p->mustEqual(thatML_p->getUnionAO(), pedge, comp, analysis);
}

//bool PartEdgeUnionMemLocObject::equalSetAO(MemLocObjectPtr that, PartEdgePtr pedge) {
bool PartEdgeUnionMemLocObject::equalSet(MemLocObjectPtr that,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<PartEdgeUnionMemLocObject> thatML_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemLocObject>(that);
  assert(thatML_p);

  assert(unionML_p);
  //return unionML_p->equalSetAO(thatML_p->getUnionAO(), pedge);
  return unionML_p->equalSet(thatML_p->getUnionAO(), pedge, comp, analysis);
}

//bool PartEdgeUnionMemLocObject::subSetAO(MemLocObjectPtr that, PartEdgePtr pedge) {
bool PartEdgeUnionMemLocObject::subSet(MemLocObjectPtr that,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<PartEdgeUnionMemLocObject> thatML_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemLocObject>(that);
  assert(thatML_p);

  assert(unionML_p);
  //return unionML_p->subSetAO(thatML_p->getUnionAO(), pedge);
  return unionML_p->subSet(thatML_p->getUnionAO(), pedge, comp, analysis);
}

//bool PartEdgeUnionMemLocObject::meetUpdateAO(MemLocObjectPtr that, PartEdgePtr pedge) {
bool PartEdgeUnionMemLocObject::meetUpdate(MemLocObjectPtr that,
    PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  boost::shared_ptr<PartEdgeUnionMemLocObject> thatML_p =
      boost::dynamic_pointer_cast<PartEdgeUnionMemLocObject>(that);
  assert(thatML_p);

  assert(unionML_p);
  //return unionML_p->meetUpdateAO(thatML_p->getUnionAO(), pedge);
  return unionML_p->meetUpdate(thatML_p->getUnionAO(), pedge, comp, analysis);
}

//bool PartEdgeUnionMemLocObject::isLiveAO(PartEdgePtr pedge) {
bool PartEdgeUnionMemLocObject::isLive(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  assert(unionML_p);
  //return unionML_p->isLiveAO(pedge);
  return unionML_p->isLive(pedge, comp, analysis);
}

//bool PartEdgeUnionMemLocObject::isFullAO(PartEdgePtr pedge) {
bool PartEdgeUnionMemLocObject::isFull(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  assert(unionML_p);
  //return unionML_p->isFullAO(pedge);
  return unionML_p->isFull(pedge, comp, analysis);
}

//bool PartEdgeUnionMemLocObject::isEmptyAO(PartEdgePtr pedge) {
bool PartEdgeUnionMemLocObject::isEmpty(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  assert(unionML_p);
  //return unionML_p->isEmptyAO(pedge);
  return unionML_p->isEmpty(pedge, comp, analysis);
}

// Returns true if this AbstractObject corresponds to a concrete value that is statically-known
bool PartEdgeUnionMemLocObject::isConcrete() {
  return unionML_p->isConcrete();
}

// Returns the number of concrete values in this set
int PartEdgeUnionMemLocObject::concreteSetSize() {
  return unionML_p->concreteSetSize();
}

MemLocObjectPtr PartEdgeUnionMemLocObject::copyAOType() const {
  return boost::make_shared<PartEdgeUnionMemLocObject>(*this);
}

void PartEdgeUnionMemLocObject::setAOToFull() {
  unionML_p = boost::make_shared<FullMemLocObject>();
}

string PartEdgeUnionMemLocObject::str(string indent) const {
  ostringstream oss;
  assert(unionML_p);
  oss << "[UnionML=" << unionML_p->str(indent) << "]";
  return oss.str();
}

// Returns whether all instances of this class form a hierarchy. Every instance of the same
// class created by the same analysis must return the same value from this method!
bool PartEdgeUnionMemLocObject::isHierarchy() const {
  return unionML_p->isHierarchy();
}

// Returns a key that uniquely identifies this particular AbstractObject in the 
// set hierarchy.
const AbstractionHierarchy::hierKeyPtr& PartEdgeUnionMemLocObject::getHierKey() const {
  return unionML_p->getHierKey();
}

/* #######################
   ##### IndexVector ##### 
   ####################### */

//std::string IndexVector::str(const string& indent)
// pretty print for the object
std::string IndexVector::str(std::string indent) const {
  dbg
      << "Error. Direct call to base class (IndexVector)'s str() is not allowed."
      << endl;
  //assert (false);
  return "";
}
bool IndexVector::mayEqual(IndexVectorPtr other, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  cerr
      << "Error. Direct call to base class (IndexVector)'s mayEqual() is not allowed."
      << endl;
  assert(false);
  return false;
}
bool IndexVector::mustEqual(IndexVectorPtr other, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  cerr
      << "Error. Direct call to base class (IndexVector)'s mustEqual() is not allowed."
      << endl;
  assert(false);
  return false;
}

// Returns whether the two abstract index vectors denote the same set of concrete vectors.
bool IndexVector::equalSet(IndexVectorPtr other, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  cerr
      << "Error. Direct call to base class (IndexVector)'s equalSet() is not allowed."
      << endl;
  assert(false);
  return false;
}

// Returns whether this abstract index vector denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract index vector.
bool IndexVector::subSet(IndexVectorPtr other, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  cerr
      << "Error. Direct call to base class (IndexVector)'s subSet() is not allowed."
      << endl;
  assert(false);
  return false;
}

bool IndexVector::meetUpdate(IndexVectorPtr other, PartEdgePtr pedge,
    Composer* comp, ComposedAnalysis* analysis) {
  cerr
      << "Error. Direct call to base class (IndexVector)'s meetUpdate() is not allowed."
      << endl;
  assert(false);
  return false;
}

bool IndexVector::isFull(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  cerr
      << "Error. Direct call to base class (IndexVector)'s isFull() is not allowed."
      << endl;
  assert(false);
  return false;
}

bool IndexVector::isEmpty(PartEdgePtr pedge, Composer* comp,
    ComposedAnalysis* analysis) {
  cerr
      << "Error. Direct call to base class (IndexVector)'s isEmpty() is not allowed."
      << endl;
  assert(false);
  return false;
}

// Sriram: gcc 4.1.2 complains of undefined references to unused to template functions
// fix: explicit template instantiation
/*template class CombinedCodeLocObject<true> ;
template class CombinedCodeLocObject<false> ;
template class CombinedValueObject<true> ;
template class CombinedValueObject<false> ;
template class CombinedMemLocObject<true> ;
template class CombinedMemLocObject<false> ;
template class CombinedMemRegionObject<true> ;
template class CombinedMemRegionObject<false> ;*/

/*  template class MappedCodeLocObject<ComposedAnalysis*, true> ;
template class MappedCodeLocObject<ComposedAnalysis*, false> ;
template class MappedValueObject<ComposedAnalysis*, true> ;
template class MappedValueObject<ComposedAnalysis*, false> ;
template class MappedMemRegionObject<ComposedAnalysis*, true> ;
template class MappedMemRegionObject<ComposedAnalysis*, false> ;
template class MappedMemLocObject<ComposedAnalysis*, true> ;
template class MappedMemLocObject<ComposedAnalysis*, false> ;*/
template class MappedCodeLocObject<ComposedAnalysis*>;
template class MappedMemLocObject<ComposedAnalysis*>;
template class MappedValueObject<ComposedAnalysis*> ;
template class MappedMemRegionObject<ComposedAnalysis*>;
template class MappedAbstractObject<ComposedAnalysis*, CodeLocObject,   AbstractObject::CodeLoc,   MappedCodeLocObject<ComposedAnalysis*> >;
template class MappedAbstractObject<ComposedAnalysis*, ValueObject,     AbstractObject::Value,     MappedValueObject<ComposedAnalysis*> >;
template class MappedAbstractObject<ComposedAnalysis*, MemRegionObject, AbstractObject::MemRegion, MappedMemRegionObject<ComposedAnalysis*> >;
template class MappedAbstractObject<ComposedAnalysis*, MemLocObject,    AbstractObject::MemLoc,    MappedMemLocObject<ComposedAnalysis*> >;

template class MappedCodeLocObject<UInt>;
template class MappedValueObject<UInt>;
template class MappedMemRegionObject<UInt>;
template class MappedMemLocObject<UInt>;
template class MappedAbstractObject<UInt, CodeLocObject,   AbstractObject::CodeLoc,   MappedCodeLocObject<UInt> >;
template class MappedAbstractObject<UInt, ValueObject,     AbstractObject::Value,     MappedValueObject<UInt> >;
template class MappedAbstractObject<UInt, MemRegionObject, AbstractObject::MemRegion, MappedMemRegionObject<UInt> >;
template class MappedAbstractObject<UInt, MemLocObject,    AbstractObject::MemLoc,    MappedMemLocObject<UInt> >;

/*template class MappedValueObject<ComposedAnalysis*> ;
template class MappedMemRegionObject<ComposedAnalysis*> ;
template class MappedMemLocObject<ComposedAnalysis*> ;*/

} //namespace fuse
