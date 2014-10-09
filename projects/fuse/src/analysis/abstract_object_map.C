#include "sage3basic.h"
#include <iostream>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include "abstract_object_map.h"
#include "nodeState.h"
#include "analysis.h"
#include "sight_verbosity.h"

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

using namespace std;
using namespace boost;
using namespace sight;

namespace fuse {

#define AbstractObjectMapDebugLevel 0

AbstractObjectMapKindPtr NULLAbstractObjectMapKind;

/*****************************
 ***** AbstractObjectMap *****
 *****************************/
AbstractObjectMap::AbstractObjectMap(const AbstractObjectMap& that) : 
                                   Lattice(that.latPEdge)/*,
                                   defaultLat  (that.defaultLat),
                                   mapState    (that.mapState),
                                   comp        (that.comp),
                                   analysis    (that.analysis)*/
{
  defaultLat = that.defaultLat;
  mapState   = that.mapState;
  comp       = that.comp;
  analysis   = that.analysis;

  if(mapState==between) {
    ROSE_ASSERT(that.implementation);
    implementation = that.implementation->copy();
    implementation->setParent(this);
  }
}

// Create a copy of that object but using the given implementation. The implementation object
// is incorporated directly without making any additional copies.
AbstractObjectMap::AbstractObjectMap(const AbstractObjectMap& that, AbstractObjectMapKindPtr implementation) :
                                   Lattice(that.latPEdge),
                                   implementation(implementation),
                                   defaultLat  (that.defaultLat),
                                   mapState    (that.mapState),
                                   comp        (that.comp),
                                   analysis    (that.analysis)
{
  implementation->setParent(this);
}
AbstractObjectMap::AbstractObjectMap(LatticePtr defaultLat_, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) :
  Lattice(pedge), defaultLat(defaultLat_), mapState(empty), comp(comp), analysis(analysis) { }

AbstractObjectMap::~AbstractObjectMap() {}

// Initializes the AOM implementation that will be used inside this AbstractObjectMap. 
// The choice of implementation will be based on the semantics that are implemented
// by the key provided as the argument, which are assumed to be implemented by all
// other keys that may ever be provided.
void AbstractObjectMap::initImplementation(AbstractObjectPtr key) {
  if(implementation==NULL) {
//    cout << "AbstractObjectMap::initImplementation() key="<<key->str()<<", key->isHierarchy()="<<key->isHierarchy()<<endl;
    /*if(key->isDisjoint()) implementation = ???;
     else */
    if(key->isHierarchy() && getenv("DISABLE_HIER_AO")==NULL) implementation = boost::make_shared<HierarchicalAOM>(this);
    else implementation = boost::make_shared<GenericAOM>(this);
  }
}

// Add a new memory object --> lattice pair to the frontier.
// Return true if this causes the map to change and false otherwise.
// It is assumed that the given Lattice is now owned by the AbstractObjectMap and can be modified and deleted by it.
bool AbstractObjectMap::insert(AbstractObjectPtr key, LatticePtr val) {
  initImplementation(key);
  // If the map was empty, the addition of a new mapping will make it non-empty
  // If it was full, the new mapping will constrain it so that it no longer denotes all possible mappings
  mapState = between;
  return implementation->insert(key, val);
}

// Removes the key matching the argument from the frontier.
// Return true if this causes the map to change and false otherwise.
bool AbstractObjectMap::remove(AbstractObjectPtr key) {
  // If this map corresponds to all possible mappings, all removals are redundant
  if(mapState==full) { return false; }
  ROSE_ASSERT(mapState==between);
  initImplementation(key);
  return implementation->remove(key);
}

// Get all x-frontier for a given abstract memory object
LatticePtr AbstractObjectMap::get(AbstractObjectPtr key) {
  // If this map corresponds to all possible mappings, the only mapping that exists for any object is the full lattice
  if(mapState==full) { 
    LatticePtr fullLat(defaultLat->copy());
    fullLat->setToFull();
    return fullLat;
  }
  // If this map corresponds to the empty set of mappings, the only mapping that exists for any object is the empty lattice
  else if(mapState==empty) { 
    LatticePtr emptyLat(defaultLat->copy());
    emptyLat->setToEmpty();
    return emptyLat;
  } else {
    ROSE_ASSERT(mapState==between);
    initImplementation(key);
    return implementation->get(key);
  }
}

// Set this Lattice object to represent the set of all possible execution prefixes.
// Return true if this causes the object to change and false otherwise.
bool AbstractObjectMap::setToFull() {
  bool modified = (mapState != full);
  mapState=full;
  // Remove the implementation since we don't need to maintain the prior mappings
  implementation = NULLAbstractObjectMapKind;
  return modified;
}

// Set this Lattice object to represent the of no execution prefixes (empty set).
// Return true if this causes the object to change and false otherwise.
bool AbstractObjectMap::setToEmpty() {
  bool modified = (mapState != empty);
  mapState=empty;
  // Remove the implementation since we don't need to maintain the prior mappings
  implementation = NULLAbstractObjectMapKind;
  return modified;
}

// Set all the information associated Lattice object with this MemLocObjectPtr to full.
// Return true if this causes the object to change and false otherwise.
bool AbstractObjectMap::setMLValueToFull(MemLocObjectPtr ml) {
  // If the map is full, there is nothing to do. IF it is empty, there is no key to set to full.
  if(mapState == full || mapState == empty) return false;
  else {
    ROSE_ASSERT(implementation);
    return implementation->setMLValueToFull(ml);
  }
}

// Returns whether this lattice denotes the set of all possible execution prefixes.
bool AbstractObjectMap::isFullLat() {
  if(mapState == full) return true;
  else if(mapState == empty) return false;
  else {
    ROSE_ASSERT(implementation);
    return implementation->isFullLat();
  }
}

// Returns whether this lattice denotes the empty set.
bool AbstractObjectMap::isEmptyLat() {
  if(mapState == full) return false;
  else if(mapState == empty) return true;
  else {
    ROSE_ASSERT(implementation);
    return implementation->isEmptyLat();
  }
}

std::string AbstractObjectMap::str(std::string indent) const {
  if(mapState == full)  return txt()<<"[AbstractObjectMap: FULL, pedge = "<<latPEdge->str()<<"]";
  else if(mapState == empty) return txt()<<"[AbstractObjectMap: EMPTY, pedge = "<<latPEdge->str()<<"]";
  else {
    ROSE_ASSERT(implementation);
    return implementation->str(indent);
  }
}

// Variant of the str method that can produce information specific to the current Part.
// Useful since AbstractObjects can change from one Part to another.
std::string AbstractObjectMap::strp(PartEdgePtr pedge, std::string indent) const {
  if(mapState == full)  return "[AbstractObjectMap: FULL]";
  else if(mapState == empty) return "[AbstractObjectMap: EMPTY]";
  else {
    ROSE_ASSERT(implementation);
    return implementation->str(indent);
  }
}

// -----------------
// Lattice methods
// initializes this Lattice to its default state, if it is not already initialized
void AbstractObjectMap::initialize() {
  if(mapState == between) {
    ROSE_ASSERT(implementation);
    return implementation->initialize();
  }
}

// returns a copy of this lattice
Lattice* AbstractObjectMap::copy() const {
  return new AbstractObjectMap(*this);
}

// overwrites the state of this Lattice with that of that Lattice
void AbstractObjectMap::copy(Lattice* thatL) {
  AbstractObjectMap* that = dynamic_cast<AbstractObjectMap*>(thatL);
  ROSE_ASSERT(that);
  
  if(that->mapState==full) setToFull();
  else if(that->mapState==empty) setToEmpty();
  else {
    ROSE_ASSERT(that->implementation);
    mapState = between;
    if(implementation) implementation->copy(that->implementation);
    else {
      implementation = that->implementation->copy();
      implementation->setParent(this);
    }
  }
}

// Called by analyses to transfer this lattice's contents from across function scopes from a caller function 
//    to a callee's scope and vice versa. If this this lattice maintains any information on the basis of 
//    individual MemLocObjects these mappings must be converted, with MemLocObjects that are keys of the ml2ml 
//    replaced with their corresponding values. If a given key of ml2ml does not appear in the lattice, it must
//    be added to the lattice and assigned a default initial value. In many cases (e.g. over-approximate sets 
//    of MemLocObjects) this may not require any actual insertions. If the value of a given ml2ml mapping is 
//    NULL (empty boost::shared_ptr), any information for MemLocObjects that must-equal to the key should be 
//    deleted.    
// Since the function is called for the scope change across some Part, it needs to account for the fact that
//    the keys in ml2ml are in scope on one side of Part, while the values on the other side. Specifically, it is
//    guaranteed that the keys are in scope at fromPEdge while the values are in scope at the edge returned 
//    by getPartEdge().
// remapML must return a freshly-allocated object.
Lattice* AbstractObjectMap::remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge) {
  if(mapState==between) {
    ROSE_ASSERT(implementation);
    return new AbstractObjectMap(*this, implementation->remapML(ml2ml, fromPEdge));
  // If this is a full or empty map, its remapped version is itself
  } else
    return copy();
}

// Adds information about the MemLocObjects in newL to this Lattice, overwriting any information previously 
//    maintained in this lattice about them.
// Returns true if the Lattice state is modified and false otherwise.
bool AbstractObjectMap::replaceML(Lattice* newL) {
  if(mapState==between) {
    ROSE_ASSERT(implementation); 
    AbstractObjectMap* newAOM = dynamic_cast<AbstractObjectMap*>(newL);
    ROSE_ASSERT(newAOM);
    return implementation->replaceML(newAOM->implementation);
  // If this is a full or empty map, nothing is changed
  } else
    return false;
}

// computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool AbstractObjectMap::meetUpdate(Lattice* thatL) {
  SIGHT_VERB_DECL(scope, ("AbstractObjectMap::meetUpdate()", scope::medium), 2, AbstractObjectMapDebugLevel)
  AbstractObjectMap* that = dynamic_cast<AbstractObjectMap*>(thatL);
  ROSE_ASSERT(that);

  SIGHT_VERB(dbg << "mapState="<<state2Str(mapState)<<", that->mapState="<<state2Str(that->mapState)<<endl, 2, AbstractObjectMapDebugLevel)
  
  // Full sets cannot be expanded via union operations
  if(mapState==full) return false;
  
  // The union of an empty set and another set is the other set
  else if(mapState==empty) {
    if(that->mapState==full) return setToFull();
    else if(that->mapState==between) {
      ROSE_ASSERT(implementation==NULL);
      implementation = that->implementation->copy();
      implementation->setParent(this);
      mapState=between;
      return true;
    } else
      return false;
  }
  
  // Forward call to the implementation object
  else if(mapState==between) {
    ROSE_ASSERT(implementation);
    if(that->mapState==full) return setToFull();
    else if(that->mapState==empty) return false;
    else {
      ROSE_ASSERT(that->implementation);
      SIGHT_VERB(dbg << "Calling implementation->meetUpdate()"<<endl, 2, AbstractObjectMapDebugLevel)
      return implementation->meetUpdate(that->implementation);
    }
  }
  
  ROSE_ASSERT(0);
}

bool AbstractObjectMap::finiteLattice() {
  if(mapState==between) {
    ROSE_ASSERT(implementation);
    return implementation->finiteLattice();
  // If this is a full or empty map, assume the lattice is finite
  } else
    return true;
}

bool AbstractObjectMap::operator==(Lattice* thatL) {
  AbstractObjectMap* that = dynamic_cast<AbstractObjectMap*>(thatL);
  ROSE_ASSERT(that);
  
  if(mapState==full || mapState==empty) return mapState==that->mapState;
  else if(that->mapState==between) {
    ROSE_ASSERT(implementation);
    ROSE_ASSERT(that->implementation);
    return *implementation.get() == that->implementation;
  } else 
    return false;
}
  
/**********************
 ***** GenericAOM *****
 **********************/


// Add a new memory object --> lattice pair to the frontier.
// Return true if this causes the map to change and false otherwise.
// It is assumed that the given Lattice is now owned by the AbstractObjectMap and can be modified and deleted by it.
bool GenericAOM::insert(AbstractObjectPtr o, LatticePtr lattice) {
  SIGHT_VERB_DECL(scope, ("GenericAOM::insert()", scope::medium), 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB_IF(1, AbstractObjectMapDebugLevel)
    dbg << "&nbsp;&nbsp;&nbsp;&nbsp;o="<<o->strp(parent->latPEdge, "")<<" lattice="<<lattice->str("&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
    dbg << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"<<str("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
  SIGHT_VERB_FI()
  
  // Do not insert mappings for dead keys
  if(!o->isLive(parent->latPEdge, parent->comp, parent->analysis)) { 
    SIGHT_VERB(dbg << "<b>GenericAOM::insert() WARNING: attempt to insert dead mapping "<<o->strp(parent->latPEdge)<<" =&gt; "<<lattice->str()<<"<\b>"<<endl, 1, AbstractObjectMapDebugLevel)
    return false;
  }
  
  isFinite = isFinite && lattice->finiteLattice();
  
  bool retVal = false;
  bool insertDone = false;
  // Points to the Lattice mapped to key o after it has been inserted
  LatticePtr insertedLattice;
  bool mustEqualSeen = false;
  
  // First, check if there is a key on the frontier that must-equals o to determine
  // if we can just combine the new lattice with the old mapping
  list<MapElement>::iterator it;
  int i=0;
  for(it = items.begin(); it != items.end(); i++) {
    AbstractObjectPtr keyElement = it->first;
    SIGHT_VERB(dbg << "&nbsp;&nbsp;&nbsp;&nbsp;keyElement="<<keyElement->str("            ")<<" mustEqual(o, keyElement, parent->latPEdge)="<<o->mustEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis)<<" insertDone="<<insertDone<<" mustEqualSeen="<<mustEqualSeen<<endl, 2, AbstractObjectMapDebugLevel)

    // If we're done inserting, don't do it again
    if(insertDone) {
      // If o is mustEqual to this element and it is not the first match, remove this element
      if(o->mustEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis)) {
        //if(mustEqualSeen) {
          items.erase(it++);
        //} else 
        //  it++;
        mustEqualSeen = true;
      // If o denotes the same set as keyElement and has already been inserted into the map, any get that mayEqual
      // to o is guaranteed to be mayEqual to keyElement. As such, we just meet keyElement's lattice with o's lattice
      // and remove the keyElement's mapping
      } else if(o->equalSet(keyElement, parent->latPEdge, parent->comp, parent->analysis)) {
         retVal = insertedLattice->meetUpdate(it->second) || retVal;
         items.erase(it++);
      } else 
        it++;
      continue;
    }
  
    // If the o-frontier contains an object that must-equal to 
    if(o->mustEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis)) {
      SIGHT_VERB(dbg << "&nbsp;&nbsp;&nbsp;&nbsp;keyElement="<<keyElement->str("            ")<<" mustEqual(o, keyElement, parent->latPEdge)="<<o->mustEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis)<<" insertDone="<<insertDone<<" mustEqualSeen="<<mustEqualSeen<<endl, 1, AbstractObjectMapDebugLevel)
      SIGHT_VERB_IF(1, AbstractObjectMapDebugLevel)
        dbg << "&nbsp;&nbsp;&nbsp;&nbsp;Must Equal"<<endl;
        dbg << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;lattice="<<lattice->str("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
        dbg << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;it="<<it->second->str("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
      SIGHT_VERB_FI()

      // If the old and new mappings of o are different,  we remove the old mapping and add a new one 
      if(!it->second->equiv(lattice))
      {
        SIGHT_VERB(dbg << "&nbsp;&nbsp;&nbsp;&nbsp;keyElement="<<keyElement->str("            ")<<" mustEqual(o, keyElement, parent->latPEdge)="<<o->mustEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis)<<" insertDone="<<insertDone<<" mustEqualSeen="<<mustEqualSeen<<endl, 1, AbstractObjectMapDebugLevel)
        SIGHT_VERB(dbg << "&nbsp;&nbsp;&nbsp;&nbsp;Removing i="<<i<<", inserting "<<o->strp(parent->latPEdge, "        ")<<"=&gt;"<<lattice->str("        ")<<endl, 1, AbstractObjectMapDebugLevel)
        items.erase(it++);
        items.push_front(MapElement(o, lattice));
        retVal = true;
      } else {
        //dbg << "&nbsp;&nbsp;&nbsp;&nbsp;No Change"<<endl;
        it++;
        // Otherwise, they're identical and thus there is no need to modify the map
        retVal = false;
      }
      insertDone = true;
      insertedLattice = lattice;
      mustEqualSeen = true;
    // If the new element and the original actually denote the same set
    } else if(o->equalSet(keyElement, parent->latPEdge, parent->comp, parent->analysis)) {
      // Meet their respective lattices of the 
      //dbg << "o="<<o->str()<<" <b>equalSet</b> "<<keyElement<<" keyElement="<<keyElement->str()<<endl;
      retVal = it->second->meetUpdate(lattice) || retVal;
      insertedLattice = it->second;
      it++;
      insertDone = true;
    // If the element on the o-frontier may-equals o (their sets overlap) then insert a new o->lattice mapping 
    // since the new lattice cannot be combined with the mapping of *it
    } else if(o->mayEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis)) {
      //dbg << "o="<<o->str()<<" <b>mayEqual</b> "<<keyElement<<" keyElement="<<keyElement->str()<<endl;
      items.push_front(MapElement(o, lattice));
      retVal = true;
      insertedLattice = lattice;
      it++;
      insertDone = true;
    } else
      it++;
  }
  
  if(!insertDone) {
    // There are no objects within this map on the o-frontier. As such, add an o->lattice mapping
    items.push_front(MapElement(o, lattice));
    
    retVal = true;
  }
  
  // Having inserted the new item we need to clean up the map to ensure that it stays bounded in size
  // Step 1: call isEmpty to check for any keys mapped to empty sets
  isEmptyLat();
  // Step 2: if the map is larger than some fixed bound, merge some key->value mappings together
  // !!! TODO !!!
  
  SIGHT_VERB_IF(1, AbstractObjectMapDebugLevel)
    indent ind();
    dbg << "retVal="<<retVal<<" insertDone="<<insertDone<<" mustEqualSeen="<<mustEqualSeen<<endl;
    dbg << str()<<endl;
  SIGHT_VERB_FI()
  return retVal;
};

// Removes the key matching the argument from the frontier.
// Return true if this causes the map to change and false otherwise.
bool GenericAOM::remove(AbstractObjectPtr abstractObjectPtr) {
  for (list<MapElement>::iterator it = items.begin();
       it != items.end(); it++) {
    AbstractObjectPtr keyElement = it->first;
    // For remove operation, we use must equal policy                                                               
    if (abstractObjectPtr->mustEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis)) {
      it = items.erase(it);
      return true;
    }
  }
  return false;
};

// Get all x-frontier for a given abstract memory object                                                            
LatticePtr GenericAOM::get(AbstractObjectPtr abstractObjectPtr) {
  SIGHT_VERB_DECL(scope, ("GenericAOM::get()", scope::medium), 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB_IF(1, AbstractObjectMapDebugLevel)
    dbg << "&nbsp;&nbsp;&nbsp;&nbsp;o="<<abstractObjectPtr->strp(parent->latPEdge, "&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
    dbg << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"<<str("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
  SIGHT_VERB_FI()

  LatticePtr ret;
  for (list<MapElement>::iterator it = items.begin();
       it != items.end(); it++) {
    AbstractObjectPtr keyElement = it->first;
    bool eq = abstractObjectPtr->mayEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis);
    if(AbstractObjectMapDebugLevel>=2 || (AbstractObjectMapDebugLevel>=1 && eq)) dbg << "&nbsp;&nbsp;&nbsp;&nbsp;keyElement(equal="<<eq<<")="<<keyElement->str("&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
    if(eq) {
      // If this is the first matching Lattice, copy this Lattice to ret
      if(!ret) ret = boost::shared_ptr<Lattice>(it->second->copy());
      // Otherwise, merge this latice into ret
      else     ret->meetUpdate(it->second);
      
      // If the current key must-equals the given object, its assignment must have overwritten any prior assignment
      // to this object, meaning that prior assignments can be ignored
      if(abstractObjectPtr->mustEqual(keyElement, parent->latPEdge, parent->comp, parent->analysis)) {
        SIGHT_VERB(dbg << "&nbsp;&nbsp;&nbsp;&nbsp;Stopping search since mustEqual, ret="<<ret->str("&nbsp;&nbsp;&nbsp;&nbsp;")<<endl, 1, AbstractObjectMapDebugLevel)
        break;
      }
      
      SIGHT_VERB(dbg << "&nbsp;&nbsp;&nbsp;&nbsp;ret="<<ret->str("&nbsp;&nbsp;&nbsp;&nbsp;")<<endl, 1, AbstractObjectMapDebugLevel)
    }
  }
  
  SIGHT_VERB(dbg << "ret="<<(ret ? ret->str("&nbsp;&nbsp;&nbsp;&nbsp;"): "NULL")<<endl, 1, AbstractObjectMapDebugLevel)
  if(ret) return ret;
  // If there is no match for abstractObjectPtr, return a copy of the default lattice
  return LatticePtr(parent->defaultLat->copy());
};

// Set all the information associated Lattice object with this MemLocObjectPtr to full.
// Return true if this causes the object to change and false otherwise.
bool GenericAOM::setMLValueToFull(MemLocObjectPtr ml)
{
  bool modified = false;
  
  // Iterate through all the keys in the items list. If any key is mayEqual(ml) then its associated 
  // value is set to full. Note that this works even if the keys are not MemLobObjectPtrs since in 
  // that case mustEqual will return false.
  for(list<MapElement>::iterator it = items.begin(); it != items.end(); it++) {
    AbstractObjectPtr keyElement = it->first;
    if(keyElement->mayEqual(ml, parent->latPEdge, parent->comp, parent->analysis)) {
      modified = it->second->setToFull() || modified;
    }
  }
  return modified;
}

// Returns whether this lattice denotes the set of all possible execution prefixes.
bool GenericAOM::isFullLat()
{
  return false;
}

// Returns whether this lattice denotes the empty set.
bool GenericAOM::isEmptyLat()
{
  SIGHT_VERB(scope s("GenericAOM::isEmpty()", scope::medium), 2, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg << "this="<<str()<<endl, 2, AbstractObjectMapDebugLevel)
  
  // Check if all items are empty
  for(std::list<MapElement>::iterator i=items.begin(); i!=items.end();) {
    SIGHT_VERB_IF(2, AbstractObjectMapDebugLevel)
      indent ind;
      dbg << "i->first="<<i->first->str()<<endl;
      dbg << "i->second="<<i->second->str()<<endl;
    SIGHT_VERB_FI()
    // If at least one is not empty, return false
    if(!(i->first)->isEmpty(parent->getPartEdge(), parent->comp, parent->analysis) && 
       !(i->second)->isEmptyLat()) return false;
    
    // If this item mapping is empty, remove it from the items list
    items.erase(i++);
  }
  // If all are empty, return true
  assert(items.size()==0);
  return true;
}

std::string GenericAOM::str(std::string indent) const {
  return strp(parent->latPEdge, indent);
}

// Variant of the str method that can produce information specific to the current Part.
// Useful since AbstractObjects can change from one Part to another.
std::string GenericAOM::strp(PartEdgePtr pedge, std::string indent) const
{
  ostringstream oss;
  oss << "<u>AbstractObjectMap:</u>"; 
  oss << "<table border=1><tr><td>Key</td><td>Value</td>";
  for(list<MapElement>::const_iterator it = items.begin();
       it != items.end(); it++) {
    //printf("\n%s%p =&gt; %p\n", indent.c_str(), it->first.get(), it->second.get()); fflush(stdout);
    oss << "<tr><td>";
    oss << it->first->strp(pedge, indent+"&nbsp;&nbsp;&nbsp;&nbsp;")<<" =&gt; ";
    oss << "</td><td>";
    oss << it->second->str(indent+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
    oss << "</td></tr>";
  }
  oss << "</table>"<<endl;
  return oss.str();
}

// initializes this Lattice to its default state, if it is not already initialized
void GenericAOM::initialize()
{
  // Nothing to do here since Peter P's fixes will eliminate the need for lattices to maintain their own initialized state
}

// returns a copy of this lattice
AbstractObjectMapKindPtr GenericAOM::copy() const
{ return boost::make_shared<GenericAOM>(*this); }

// overwrites the state of this Lattice with that of that Lattice
void GenericAOM::copy(AbstractObjectMapKindPtr thatL) {
  GenericAOMPtr that = boost::dynamic_pointer_cast <GenericAOM> (thatL);
  ROSE_ASSERT(that);
  items = that->items;
  isFinite = that->isFinite;
}

// Called by analyses to transfer this lattice's contents from across function scopes from a caller function 
//    to a callee's scope and vice versa. If this this lattice maintains any information on the basis of 
//    individual MemLocObjects these mappings must be converted, with MemLocObjects that are keys of the ml2ml 
//    replaced with their corresponding values. If a given key of ml2ml does not appear in the lattice, it must
//    be added to the lattice and assigned a default initial value. In many cases (e.g. over-approximate sets 
//    of MemLocObjects) this may not require any actual insertions. If the value of a given ml2ml mapping is 
//    NULL (empty boost::shared_ptr), any information for MemLocObjects that must-equal to the key should be 
//    deleted.
// Since the function is called for the scope change across some Part, it needs to account for the fact that
//    the keys in ml2ml are in scope on one side of Part, while the values on the other side. Specifically, it is
//    guaranteed that the keys are in scope at fromPEdge while the values are in scope at the edge returned 
//    by parent->getPartEdge().
// remapML must return a freshly-allocated object.
AbstractObjectMapKindPtr GenericAOM::remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge)
{
  // Do nothing on empty maps or those where the keys are not MemLocObjects
  if(items.size()==0 || !(items.begin()->first->isMemLocObject())) { return copy(); }
  
  SIGHT_VERB_DECL(scope, ("GenericAOM::remapML", scope::medium), 1, AbstractObjectMapDebugLevel)
  
  SIGHT_VERB_IF(1, AbstractObjectMapDebugLevel)
    // If either the key or the value of this mapping is dead within its respective part, we skip it.
    // Print notices of this skipping once
    for(std::set<MLMapping>::const_iterator m=ml2ml.begin(); m!=ml2ml.end(); m++) {
      if(!m->from) continue;
      // If either the key or the value of this mapping is dead within its respective part, skip it
      if(!m->from->isLive(fromPEdge, parent->comp, parent->analysis) || (m->to && !m->to->isLive(parent->latPEdge, parent->comp, parent->analysis)))
        dbg << "<b>GenericAOM::remapML() WARNING: Skipping dead ml2ml mapping "<<m->from->strp(fromPEdge)<<"(live="<<m->from->isLive(fromPEdge, parent->comp, parent->analysis)<<") =&gt; "<<(m->to ? m->to->strp(parent->latPEdge) : "NULL")<<"(live="<<(m->to ? m->to->isLive(parent->latPEdge, parent->comp, parent->analysis) : -1)<<")"<<endl
                 << "&nbsp;&nbsp;&nbsp;&nbsp;fromPEdge=["<<fromPEdge->str()<<"]"<<endl
                 << "&nbsp;&nbsp;&nbsp;&nbsp;parent->latPEdge=["<<parent->latPEdge->str()<<"]</b>"<<endl;
    }
  
  {
    scope reg("ml2ml", scope::medium);
    for(std::set<MLMapping>::const_iterator m=ml2ml.begin(); m!=ml2ml.end(); m++) {
      if(!m->from) continue;
      dbg << m->from.get()->str() << " =&gt; " << (m->to? m->to.get()->strp(parent->latPEdge): "NULL") << endl;
    }
    //dbg << "this="<<str()<<endl;
  }
  SIGHT_VERB_FI()
  
  // Copy of this map where the keys in ml2ml have been remapped to their corresponding values
  boost::shared_ptr<GenericAOM> newM = boost::make_shared<GenericAOM>(*this);
  
  // Vector of flags that indicate whether a given key in ml2ml has been added to newM or not
  vector<bool> ml2mlAdded;
  
  // Initialize ml2mlAdded to all false
  for(std::set<MLMapping>::const_iterator m=ml2ml.begin(); m!=ml2ml.end(); m++)
    ml2mlAdded.push_back(false);
  
  SIGHT_VERB(dbg << "newM="<<newM->str()<<endl, 2, AbstractObjectMapDebugLevel)
  
  // Iterate over all the mappings <key, val> n ml2ml and for each mapping consider 
  // each item in newM. If the key mustEquals to some item newM, that item is replaced 
  // by val. If the key mayEquals some item in newM, val is placed at the front of 
  // the list. If the key does not appear in newM at all, val is placed at the front of 
  // the list.
  for(std::list<MapElement>::iterator i=newM->items.begin(); i!=newM->items.end(); ) {
    SIGHT_VERB_DECL(indent, (), 1, AbstractObjectMapDebugLevel)
    SIGHT_VERB(dbg << "i="<<i->first->str()<<endl, 1, AbstractObjectMapDebugLevel)
  
    int mIdx=0;
    std::set<MLMapping>::const_iterator m=ml2ml.begin();
    for(; m!=ml2ml.end(); m++, mIdx++) {
      if(!m->from) continue;
      
      SIGHT_VERB_DECL(indent, (), 1, AbstractObjectMapDebugLevel)
      SIGHT_VERB(dbg << mIdx << ": m-&gt;key="<<m->from->strp(fromPEdge)<<endl, 1, AbstractObjectMapDebugLevel)
      
      SIGHT_VERB_DECL(indent, (), 1, AbstractObjectMapDebugLevel)
      // If the current item in newM may- or must-equals a key in ml2ml, record this and update newM
      SIGHT_VERB_IF(1, AbstractObjectMapDebugLevel)
        dbg << "i-&gt;first mustEqual m-&gt;from = "<<i->first->mustEqual(m->from, fromPEdge, parent->comp, parent->analysis)<<endl;
        dbg << "i-&gt;first mayEqual m-&gt;from = "<<i->first->mayEqual(m->from, fromPEdge, parent->comp, parent->analysis)<<endl;
      SIGHT_VERB_FI()
      if(i->first->mustEqual(m->from, fromPEdge, parent->comp, parent->analysis) && m->replaceMapping) {
        // If the value of the current ml2ml mapping is not-NULL
        if(m->to) {
          // Replace the current item in newM with the value of the current pair in ml2ml
          *i = make_pair(boost::static_pointer_cast<AbstractObject>(m->to), i->second);

          // Advance onward in newM and remove any items that are must-equal to the value of the current ml2ml mapping
          //scope reg("Deleting items that are must-equal to value", scope::medium));
          std::list<MapElement>::iterator iNext = i; iNext++;
          for(std::list<MapElement>::iterator j=iNext; j!=newM->items.end(); ) {
            SIGHT_VERB_IF(2, AbstractObjectMapDebugLevel)
              dbg << "j="<<j->first<<" => "<<j->second<<endl;
              dbg << mIdx << ": m-&gt;value="<<m->to->strp(fromPEdge)<<endl;
              dbg << "j-&gt;first mustEqual m-&gt;to = "<<j->first->mustEqual(m->to, fromPEdge, parent->comp, parent->analysis)<<endl;
            SIGHT_VERB_FI() 
            if(j->first->mustEqual(m->to, fromPEdge, parent->comp, parent->analysis)) {
              SIGHT_VERB(dbg << "Erasing j="<<j->first->str()<<" => "<<j->second->str()<<endl, 2, AbstractObjectMapDebugLevel)
              j = newM->items.erase(j);
              //break;
            } else
              j++;
          }
        // If the value of the current ml2ml mapping is NULL (i.e. the key is a MemLoc with a lifetime that is limited
        // to a given function and it does not carry over across function boundaries)
        } else {
          // Erase this mapping
          i = newM->items.erase(i);
          break;
        }
        ml2mlAdded[mIdx]=true;
      } else if(i->first->mayEqual(m->from, fromPEdge, parent->comp, parent->analysis)) {
        // Insert the value in the current ml2ml mapping immediately before the current item
        SIGHT_VERB(dbg << "Inserting before i: "<<m->to->str()<<" => "<<i->second->str()<<endl, 1, AbstractObjectMapDebugLevel)
        newM->items.insert(i, make_pair(boost::static_pointer_cast<AbstractObject>(m->to), i->second));
        ml2mlAdded[mIdx]=true;
      }
    }
    
    // If we broke out early, we must have erased the current element in newM, meaning that we shouldn't advance
    // i again. Otherwise, advance i.
    if(m==ml2ml.end())
      i++;
  }
  
  // Iterate through the false mappings in ml2mlAdded (ml2ml keys that were not mapped to any items in this map)
  // and add to newM a mapping of their values to parent->defaultLat (as long as the values are not NULL)
  int mIdx=0;
  for(std::set<MLMapping>::iterator m=ml2ml.begin(); m!=ml2ml.end(); m++, mIdx++) {
    if(!m->from) continue;
    
    //dbg << "False mapping "<<m->from->str()<<endl;
    // If either the key or the value of this mapping is dead within its respective part, skip it
    if(!m->from->isLive(fromPEdge, parent->comp, parent->analysis) || !(m->to && m->to->isLive(parent->latPEdge, parent->comp, parent->analysis))) continue;
    
    if(!ml2mlAdded[mIdx] && m->to)
      newM->items.push_back(make_pair(m->to, parent->defaultLat->copy()));
  }
  
  return newM;
}

// Adds information about the MemLocObjects in newL to this Lattice, overwriting any information previously 
//    maintained in this lattice about them.
// Returns true if the Lattice state is modified and false otherwise.
bool GenericAOM::replaceML(AbstractObjectMapKindPtr newL)
{
  boost::shared_ptr<GenericAOM> calleeAOM = boost::dynamic_pointer_cast<GenericAOM>(newL);
  assert(calleeAOM);
  
  bool modified = false;
  
  for(std::list<MapElement>::iterator i=calleeAOM->items.begin(); i!=calleeAOM->items.end(); i++) {
    // Do not copy over mappings with keys that are dead in this map's host PartEdge
    if(!i->first->isLive(parent->latPEdge, parent->comp, parent->analysis)) continue;
    modified = insert(i->first, i->second) || modified;
  }
  
  return modified;
}

// parent->computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
// The part of this object is to be used for AbstractObject parent->comparisons.
bool GenericAOM::meetUpdate(AbstractObjectMapKindPtr thatL)
{
  SIGHT_VERB_DECL(scope, ("GenericAOM::meetUpdate()", scope::medium), 2, AbstractObjectMapDebugLevel);

  // Both incorporateVars() and meetUpdate currently call merge. This is clearly not
  // right but we'll postpone fixing it until we have the right algorithm for merges
  bool modified = false;
  try {
    boost::shared_ptr<GenericAOM> that = boost::dynamic_pointer_cast<GenericAOM>(thatL);
    assert(that);
    
    SIGHT_VERB_IF(2, AbstractObjectMapDebugLevel)
      dbg << "parent->latPEdge="<<parent->latPEdge->str()<<endl;
      { scope thisreg("this", scope::medium);
      dbg << str()<<endl; }
      { scope thisreg("that", scope::medium);
      dbg << that->str()<<endl; }
    SIGHT_VERB_FI()
    
    // This algorithm is based on the following insights:
    // Given two AbstractObjectMaps: 
    //                     A: a_0 => a_1 => ... => a_n, and
    //                     B: b_0 => b_1 => ... => b_m
    //    where x => y if x is inserted after y
    // The only non-conservative thing that a merge algorithm can do is take a_i and b_j, where
    //   mustEqual(a_i, b_j) and place one before the other in the merged list. For these cases
    //   it is necessary to insert a single entry a_i -> A[a_i] merge B[b_j].
    // All elements that do not have a mustEqual partner in the same AbstractObjectMap or the other AbstractObjectMap 
    //   can be inserted into the new map in arbitrary order. GenericAOM::insert ensures that no two elements
    //   that are mustEqual to each other may be in the same map. For elements across maps the following
    //   holds. If a_p and b_q are mayEqual to each other, then keeping them in the reverse order from
    //   their correct assignment order is conservative but may lose precision. This is because if 
    //   a_p => b_q but are stored in the opposite order and for some x it is true that mayEqual(x, b_p), mustEqual(x, a_p),
    //   then GenericAOM::get(x) will merge the lattices stored at the two keys whereas the precise result would
    //   be to return the lattice stored under a_p. If a_p and b_q are not mayEqual to each other,
    //   either order is fine with no loss of precision.
    // As an additional enhancement suppose a_i and b_j are not must-equal but denote the same set. In this case, 
    //   although it is conservative to place them in either order, it is also useless since any abstract object
    //   that mayEquals a_i, must also mayEqual b_j. As such, we insert a single entry for a_i -> A[a_i] merge B[b_j]
    //   if a_i and b_j are mustEqual or equalSet. Since mustEqual implies equalSet, we only check equalSet.
    // The algorithm below chooses a simple order that is likely to work well in practice. It connects
    //   the pairs of elements in this->items(A) and that->items(B) that are mustEqual and then
    //   scans over each such pair <a_i, b_j> in the order they appear A, copying all the elements between
    //   the b_j and b_j+1 over to this map between a_i and a_i+1 if they've not already been copied over and 
    //   if they don't have a mustEquals partner in A (these are handled by merging, as described above).
    // For example, 
    //   A: a_0 => a_1 => r => a_2 => s => a_3 => t
    //   B: b_0 =>        r => b_1 =>             t => b_2 => b_3 => s
    //   A.mergeUpdate(B): a_0 => a_1 => b_0 => r => a_2 => b_1 => b_2 => b_3 => s => a_3 => t
    // Further, 
    
    // For each element x in this->items pointers that is mustEqual to an element y in
    // that->items, keeps the triple
    //    - iterator that points to x in this->items
    //    - iterator that points to y in that->items
    //    - index of y in that->items
    // Maintained in order of this->items.
    list<pair<list<MapElement>::iterator, pair<list<MapElement>::iterator, int> > > thisMustEq2thatMustEq;

    // For each element in that->items keeps true if this element is mustEquals to some
    // element in this->items and false otherwise.
    list<bool> thatMustEq;

    // Initialize thatMustEq to all false
    for(list<MapElement>::iterator itThat=that->items.begin(); itThat!=that->items.end(); itThat++)
      thatMustEq.push_back(false);

    SIGHT_VERB_IF(2, AbstractObjectMapDebugLevel)
      scope thisreg("that->items", scope::medium);
      for(list<MapElement>::iterator itThat=that->items.begin(); itThat!=that->items.end(); itThat++)
      dbg << "that: "<<itThat->first->str()<<" ==&gt; "<<itThat->second->str()<<endl;
    SIGHT_VERB_FI()
    
    // Determine which elements in this->items are mustEqual to elements in that->items
    // and for these pairs merge the lattices from that->items to this->items.
    for(list<MapElement>::iterator itThis=items.begin(); 
       itThis!=items.end(); itThis++) {
      SIGHT_VERB(scope thisreg("itThis", scope::medium), 2, AbstractObjectMapDebugLevel)
      SIGHT_VERB(dbg << "this: "<<itThis->first->str()<<" ==&gt; "<<itThis->second->str()<<endl, 2, AbstractObjectMapDebugLevel)
      
      int i=0;
      list<bool>::iterator thatMEIt=thatMustEq.begin();
      for(list<MapElement>::iterator itThat=that->items.begin(); 
         itThat!=that->items.end(); itThat++, i++, thatMEIt++) {
      
        SIGHT_VERB(scope thisreg("itThat", scope::medium), 2, AbstractObjectMapDebugLevel)
        SIGHT_VERB(dbg << "that: "<<itThat->first->str()<<" ==&gt; "<<itThat->second->str()<<endl, 2, AbstractObjectMapDebugLevel)
        
        // If we've found a pair of keys in this and that that are mustEqual or denote the same set
        //if(mustEqual(itThis->first, itThat->first, parent->latPEdge, parent->comp, parent->analysis)) {
        if(itThis->first->equalSet(itThat->first, parent->latPEdge, parent->comp, parent->analysis)) {
          // Record this pair
          thisMustEq2thatMustEq.push_back(make_pair(itThis, make_pair(itThat, i)));
          *thatMEIt = true;
          
          SIGHT_VERB(scope meetreg(txt()<<"Meeting", scope::medium), 2, AbstractObjectMapDebugLevel)
          
          // Update the lattice at *itThis to incorporate information at *itThat
          {
            // First copy the lattice since it may change. We don't deep-copy lattices when we copy
            // AbstractObjectMaps, so multiple maps may contain references to the same lattice.
            // As such, instead of updating lattices in-place (this would update the same lattice
            // in other maps) we first copy them and update into the copy.
            itThis->second = LatticePtr(itThis->second->copy());
            SIGHT_VERB(scope meetreg(txt()<<"Meeting "<<itThis->first->str(), scope::medium), 2, AbstractObjectMapDebugLevel)
            SIGHT_VERB(scope befreg("before", scope::low); dbg << itThis->second->str()<<endl, 2, AbstractObjectMapDebugLevel)
            modified = itThis->second->meetUpdate(itThat->second) || modified;
            SIGHT_VERB_IF(2, AbstractObjectMapDebugLevel)
            { scope aftreg("after", scope::low); dbg << itThis->second->str()<<endl; }
            dbg << "modified="<<modified<<endl;
            SIGHT_VERB_FI()
          }
        }
      }
    }
    SIGHT_VERB_IF(2, AbstractObjectMapDebugLevel)
      scope eqreg("thisMustEq2thatMustEq", scope::medium);
      for(list<pair<list<MapElement>::iterator, pair<list<MapElement>::iterator, int> > >::iterator it=thisMustEq2thatMustEq.begin();
          it!=thisMustEq2thatMustEq.end(); it++) {
        dbg << (it->first)->first->str() << " =&gt; " << (it->second).first->second->str() << endl;
      }
    SIGHT_VERB_FI()
    
    { SIGHT_VERB(scope insreg("inserting that->this", scope::medium), 2, AbstractObjectMapDebugLevel);
    
    // Copy over the mappings of all the elements in that->items that were not mustEqual
    // to any elements in this->items. Although any order will work for these elements,
    // keep them their order in that->items.
    int thatIdx=0;
    list<MapElement>::iterator thatIt = that->items.begin();
    list<bool>::iterator thatMEIt=thatMustEq.begin();
    for(list<pair<list<MapElement>::iterator, pair<list<MapElement>::iterator, int> > >::iterator meIt=thisMustEq2thatMustEq.begin();
       meIt!=thisMustEq2thatMustEq.end(); meIt++) {
      SIGHT_VERB(scope mapreg(txt()<<"mustEqual mapping "<<meIt->second.second<<": "<<(meIt->first)->first->str(), scope::medium), 2, AbstractObjectMapDebugLevel)
      SIGHT_VERB_IF(2, AbstractObjectMapDebugLevel)
        dbg << "this: "<<meIt->first->first->str() << " =&gt; " << meIt->first->second->str() <<endl;
        dbg << "that: "<<(meIt->second).first->first->str() << " =&gt; " << (meIt->second).first->second->str() << endl;
        dbg << "thatIdx="<<thatIdx<<endl;
      SIGHT_VERB_FI()
      
      // Copy over all the mappings from that->items from thatIt to meIt's partner in that->items
      // if they have not already been copied because elements that are mustEqual to each other were ordered
      // differently in this->items and that->items
      if(meIt->second.second >= thatIdx) {
        for(; thatIt!=meIt->second.first; thatIt++, thatIdx++, thatMEIt++) {
          // Copy over the current element from that->items if it doesn't have a mustEqual 
          // partner in this->items (i.e. its already been handled)
          if(!(*thatMEIt)) {
            SIGHT_VERB(dbg << "Inserting at meIt->first="<<(meIt->first)->first->str()<<" mapping "<<thatIt->first->str()<<" ==&gt; "<<thatIt->second->str()<<endl, 2, AbstractObjectMapDebugLevel)
            // NOTE: we do not currently update the part field in the lattice thatIt->second
            //       to refer to this->parent->latPEdge. Perhaps we should make a copy of it and update it.
            items.insert(meIt->first, *thatIt);
            modified = true;
          } else 
            SIGHT_VERB(dbg << "mustEqual partner exists in this"<<endl, 2, AbstractObjectMapDebugLevel)
        }
        // Advance thatIt and thatIdx once more to account for the partner in that->items 
        // of the current entry in this->items
        thatIt++;
        thatIdx++;
      }
      //if(AbstractObjectMapDebugLevel()>=2) dbg << "modified="<<modified<<endl;
    }
    
    // Add all the elements from that->items that remain
    for(; thatIt!=that->items.end(); thatIt++) {
      SIGHT_VERB(dbg << "Pushing end "<<thatIt->first->str()<<" ==&gt; "<<thatIt->second->str()<<endl, 2, AbstractObjectMapDebugLevel)
      // NOTE: we do not currently update the part field in the lattice thatIt->second
      //       to refer to this->parent->latPEdge. Perhaps we should make a copy of it and update it.
      items.push_back(*thatIt);
      modified = true;
    }
    //dbg << "&nbsp;&nbsp;&nbsp;&nbsp;items.size()="<<items.size()<<"\n";
   }
    
    // parent->compress all the elements from that are now mustEqual to each other in this->parent->latPEdge.
    // Note: the optimal way to do this is to parent->compress that->mustEqual first and then
    //       merge but we're not allowed to modify that so the parent->compression would need
    //       to be done non-destructively via some additional datastructure. We avoid
    //       this parent->complication for now but should revisit this question if we identify
    //       this code region as a performance bottleneck.
    //dbg << "Before mustEq parent->compression "<<str()<<endl;
    // GB: I don't think we need this parent->compression since we've already performed all the mustEqual
    //     (actually equalSet) matching and therefore should not have any keys that are mustEqual to each other.
    //parent->compressMustEq();
    
    // Remove all the dead keys
    //dbg << "Before dead parent->compression "<<str()<<endl;
    compressDead();
    
    //dbg << "Final "<<str()<<endl;
  } catch (bad_cast & bc) { 
    assert(false);
  }
  SIGHT_VERB(dbg << "Final modified="<<modified<<endl, 2, AbstractObjectMapDebugLevel)
  return modified;
}

// Identify keys that are must-equal to each other and merge their lattices
// Return true if this causes the object to change and false otherwise.
bool GenericAOM::compressMustEq()
{
  /*dbg << "parent->compressMustEq()"<<endl;
  dbg << "&nbsp;&nbsp;&nbsp;&nbsp;"<<str("&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;*/
  
  bool modified = false;
  int xIdx=0;
  for(list<MapElement>::iterator x = items.begin(); x != items.end(); x++, xIdx++) {
    //dbg << "&nbsp;&nbsp;&nbsp;&nbsp;"<<xIdx<<" : x="<<x->first->str("")<<endl;
    // y starts from the element that follows x
    list<MapElement>::iterator y = x;
    y++;
    int yIdx = xIdx+1;
    for(; y != items.end(); yIdx++) {
      //dbg << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"<<yIdx<<" : y="<<y->first->str("")<<endl;
      // If x and y are equal, merge their lattices and remove the later one
      if(x->first->mustEqual(y->first, parent->latPEdge, parent->comp, parent->analysis)) {
        //dbg << "&nbsp;&nbsp;&nbsp;&nbsp;MERGING and REMOVING"<<endl;
        // First copy the lattice since it may change. We don't deep-copy lattices when we copy
        // AbstractObjectMaps, so multiple maps may contain references to the same lattice.
        // As such, instead of updating lattices in-place (this would update the same lattice
        // in other maps) we first copy them and update into the copy.
        x->second = LatticePtr(x->second->copy());
        modified = x->second->meetUpdate(y->second) || modified;
        
        list<MapElement>::iterator tmp = y;
        y++;
        items.erase(tmp);
        
        //dbg << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;map="<<str("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
        
        modified = true;
      } else
        y++;
    }
  }

  return modified;
};

// Remove all mappings with dead keys from this map.
// Return true if this causes the object to change and false otherwise.
bool GenericAOM::compressDead()
{
  SIGHT_VERB_DECL(scope, ("compressDead", scope::low), 2, AbstractObjectMapDebugLevel)        
  bool modified = false;
  for(list<MapElement>::iterator i = items.begin(); i != items.end(); ) {
    SIGHT_VERB(dbg << "i: "<<i->first.get()->str()<<" ==&gt"<<endl<<"          "<<i->second.get()->str()<<endl, 2, AbstractObjectMapDebugLevel)
    
    // Remove mappings with dead keys
    if(!(i->first->isLive(parent->latPEdge, parent->comp, parent->analysis))) {
      list<MapElement>::iterator nextI = i;
      nextI++;
      
      SIGHT_VERB(dbg << "Erasing "<<i->first.get()->str()<<endl, 2, AbstractObjectMapDebugLevel)
      items.erase(i);
      modified = true;
      
      i = nextI;
    } else
      i++;
  }
  
  return modified;
}

bool GenericAOM::finiteLattice()
{
  return isFinite;
}

bool GenericAOM::operator==(AbstractObjectMapKindPtr that_arg)
{
  GenericAOMPtr that = boost::dynamic_pointer_cast<GenericAOM>(that_arg);
  ROSE_ASSERT(that);
  ROSE_ASSERT(parent->latPEdge == that->parent->getPartEdge());
  // This will be written once we have the merging algorithm to test
  // these maps' frontiers for semantic equivalence
  return false;
}

/***************************
 ***** HierarchicalAOM *****
 ***************************/

HierarchicalAOM::NodePtr HierarchicalAOM::NULLNode;

HierarchicalAOM::Node::Node() {
  isObjSingleton = false;
  originalVal = true;
}

// Creates a sub-tree that holds the remaining portions of the key, from subKey until keyEnd
// and places val at the leaf of this sub-tree
HierarchicalAOM::Node::Node(comparablePtr myKey, std::list<comparablePtr>::const_iterator subKey, std::list<comparablePtr>::const_iterator keyEnd, 
                            AbstractObjectHierarchy::hierKeyPtr fullKey, AbstractObjectPtr obj, LatticePtr val) {
  std::list<comparablePtr>::const_iterator next = subKey; ++next;
  // If this is the last sub-key in the key, place obj and val inside this Node
  if(subKey==keyEnd) {
    this->isObjSingleton = isSingleton(obj);
    this->fullKey = fullKey;
    this->val = val;
  // Otherwise, create a Node of the next sub-key in the key
  } else {
    this->isObjSingleton = false;
    subsets[*subKey] = boost::make_shared<Node>(*subKey, next, keyEnd, fullKey, obj, val);
  }
  
  key = myKey;
  originalVal = true;
}

HierarchicalAOM::Node::Node(const NodePtr& that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::Node::Node()", scope::medium), 2, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg << "that="; that->print(dbg); dbg<<endl, 2, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg << "that->key="<<(that->key? that->key->str(): "NULL")<<", that->fullKey="<<(that->fullKey? string(txt()<<that->fullKey): "NULL")<<endl, 2, AbstractObjectMapDebugLevel)
  
  // Copy over that node's obj and val
  isObjSingleton = that->isObjSingleton;
  key = that->key;
  fullKey = that->fullKey;
  val = that->val;
  // This val is original if that one is. 
  // If that val is a copy, we set this one to be original so that we know 
  //    to not modify it without first making a private copy.
  originalVal = true;
  
  // Copy subsets, recursively invoking this constructor on this node's children
  for(map<comparablePtr, NodePtr>::const_iterator sub=that->subsets.begin(); sub!=that->subsets.end(); sub++) {
    SIGHT_VERB(dbg << "sub="<<sub->second<<endl, 2, AbstractObjectMapDebugLevel)
    if(sub->second->fullKey) {
      SIGHT_VERB(dbg << "sub->second->fullKey="<<string(txt()<<sub->second->fullKey)<<endl, 2, AbstractObjectMapDebugLevel)
      SIGHT_VERB(dbg << "live="<<sub->second->fullKey->isLive(pedge, comp, analysis)<<endl, 2, AbstractObjectMapDebugLevel)
    }
    // Only create sub-keys for live keys
    if(!sub->second->fullKey || sub->second->fullKey->isLive(pedge, comp, analysis)) {
      NodePtr newNode = boost::make_shared<Node>(sub->second, pedge, comp, analysis);
      // If the copied sub-tree didn't end up being empty (due to the fullKeys being out of scope), add it
      if(!newNode->isEmptyVal())
        subsets[sub->first] = newNode;
    }
  }
}

// Returns whether the set denoted by thet given object is a singleton
bool HierarchicalAOM::Node::isSingleton(AbstractObjectPtr obj)
{ return (obj->isConcrete() && obj->concreteSetSize()==1); }

// Set the value at this node to full.
// Return true if this causes the object to change and false otherwise.
bool HierarchicalAOM::Node::setValToFull() {
  bool modified=false;
  
  // Set the value of this Node to full, creating a new Lattice object if needed
  if(originalVal) { 
    val = val->copySharedPtr();
    originalVal = false;
  }
  
  modified = val->setToFull() || modified;
  
  return modified;
}

// Set the value at this node AND all of its sub-trees to full.
// Return true if this causes the object to change and false otherwise.
bool HierarchicalAOM::Node::setSubTreeToFull() {
  bool modified=false;
  
  modified = setValToFull() || modified;
    
  // Cut off the children of this node to have the same effect of setting all 
  // children's lattices to full
  modified = (subsets.size()>0) || modified;
  subsets.clear();
  
  return modified;
}

// Removes the given child node from the given parent node
void HierarchicalAOM::Node::remove(NodePtr child) {
  SIGHT_VERB(dbg << "child->key="<<child->key->str()<<endl, 1, AbstractObjectMapDebugLevel)
  ROSE_ASSERT(subsets.erase(child->key)==1);
}

// Union the values of this node and all of its children in to the given lattice.
// Return whether this causes lat to change.
bool HierarchicalAOM::Node::meetUpdate(LatticePtr lat) {
  bool modified=false;
  if(val) modified = lat->meetUpdate(val.get()) || modified;
  
  for(map<comparablePtr, NodePtr>::iterator sub=subsets.begin(); sub!=subsets.end(); sub++) 
    modified = sub->second->meetUpdate(lat) || modified;
    
  return modified;
}

SightStream& HierarchicalAOM::Node::print(SightStream& s) const {
  s << "Node: key="<<(key?key->str():"NULL")<<", fullKey="<<(fullKey? string(txt()<<fullKey): "NULL")<<", isObjSingleton="<<isObjSingleton<<", val="<<(val?val->str():"NULL")<<endl;
  s << "  subsets="<<endl;
  s << "<table>"<<endl;
  for(map<comparablePtr, NodePtr>::const_iterator sub=subsets.begin(); sub!=subsets.end(); sub++) {
    s << "<tr><td>";
    s << sub->first->str();
    s << "</td><td>"; 
    sub->second->print(s); 
    s << "</td></tr>";
  }
  s << "</table>"<<endl;
  return s;
}

// Returns whether the fullKey at this node may be live at the given pedge
bool HierarchicalAOM::Node::isLive(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  // It may be live if
  // - fullKey is NULL, so we can't be sure
  if(!fullKey) return true;
  // - fullKey is known to be live
  if(fullKey->isLive(pedge, comp, analysis)) return true;
  
  return false;
}

// Returns whether the set that this node maps its key to is empty.
// This is the case if all the values in its sub-tree denote the empty set
bool HierarchicalAOM::Node::isEmptyVal() const {
  // val is empty
  if(!val || val->isEmptyLat()) {
    for(map<comparablePtr, NodePtr>::const_iterator sub=subsets.begin(); sub!=subsets.end(); sub++) {
      // If some sub-tree is not empty
      if(!sub->second->isEmptyVal()) return false;
    }
    return true;
  // val is not empty
  } else 
    return false;
}

std::ostream& operator<<(std::ostream& s, const HierarchicalAOM::NodePtr& node) {
  s << "<div style=\"border-style:solid; border-color:#888888; border-width=1px\">";
  s << "Node: "<</*key="<<(node->key?node->key->str():"NULL")<<", */" isObjSingleton="<<node->isObjSingleton<<endl;
  s << "    val="<<(node->val?node->val->str():"NULL");
  //s << "  <table border=1>";
  for(map<comparablePtr, HierarchicalAOM::NodePtr>::const_iterator sub=node->subsets.begin(); sub!=node->subsets.end(); sub++) {
    s << "  <table border=0>";
    s << "<tr><td>";
    s << sub->first->str();
    s << "</td><td>";
    s << sub->second;
    s << "</td></tr>";
    s << "</table>";
  }
  //s << "</table>"<<endl;
  s << "</div>";
  return s;
}

HierarchicalAOM::HierarchicalAOM(const HierarchicalAOM& that, AbstractObjectMap* parent) : 
			   AbstractObjectMapKind(parent),
                           tree    (that.tree),
			   isFinite(that.isFinite)
{ tree = boost::make_shared<Node>(that.tree, parent->latPEdge, parent->comp, parent->analysis); }

HierarchicalAOM::HierarchicalAOM(const HierarchicalAOM& that) : 
			   AbstractObjectMapKind(that.parent),
			   tree    (that.tree),
			   isFinite(that.isFinite)
{ tree = boost::make_shared<Node>(that.tree, parent->latPEdge, parent->comp, parent->analysis); }

HierarchicalAOM::HierarchicalAOM(AbstractObjectMap* parent) : AbstractObjectMapKind(parent), isFinite(true)
{ tree = boost::make_shared<Node>(); }

// Add a new memory object --> lattice pair to the frontier.
// Return true if this causes the map to change and false otherwise.
// It is assumed that the given Lattice is now owned by the AbstractObjectMap and can be modified and deleted by it.
bool HierarchicalAOM::insert(AbstractObjectPtr obj_arg, LatticePtr val, bool originalVal) {
  AbstractObjectHierarchyPtr obj = boost::dynamic_pointer_cast<AbstractObjectHierarchy>(obj_arg);
  ROSE_ASSERT(obj);

  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::insert()", scope::medium), 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB_IF(1, AbstractObjectMapDebugLevel)
    indent ind;
    dbg << "obj(live="<<obj_arg->isLive(parent->latPEdge, parent->comp, parent->analysis)<<")="<<obj_arg->strp(parent->latPEdge, "")<<endl;
    dbg << "    key="<<obj->getHierKey()<<endl;
    dbg << "lattice(empty="<<val->isEmptyLat()<<")="<<val->str("&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
    indent ind2;
    dbg << str()<<endl;
  SIGHT_VERB_FI()
  
  // Do not insert mappings for dead keys
  if(!obj_arg->isLive(parent->latPEdge, parent->comp, parent->analysis)) { 
    SIGHT_VERB(dbg << "<b>GenericAOM::insert() WARNING: attempt to insert dead mapping "<<obj_arg->strp(parent->latPEdge)<<" =&gt; "<<val->str()<<"<\b>"<<endl, 1, AbstractObjectMapDebugLevel)
    return false;
  }
  
  isFinite = isFinite && val->finiteLattice();
  
  // Don't add empty values since that wouldn't have any information content
  if(val->isEmptyLat()) return false;
  
  return insert(tree, obj->getHierKey()->begin(), obj->getHierKey()->end(), obj->getHierKey(), obj_arg, val, originalVal);
}

// Recursive body of insert
bool HierarchicalAOM::insert(NodePtr subTree,
             std::list<comparablePtr>::const_iterator subKey, std::list<comparablePtr>::const_iterator keyEnd,
             AbstractObjectHierarchy::hierKeyPtr fullKey, AbstractObjectPtr obj, LatticePtr val, bool originalVal)
{
  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::insert()", scope::medium), 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg << "subKey="<<(subKey!=keyEnd? (*subKey)->str(): "END")<<", end of key="<<(subKey==keyEnd)<<", tree root="<<(subTree==tree)<<endl, 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg << "subTree="<<subTree<<endl,  1, AbstractObjectMapDebugLevel)
  // If this is the last sub-key in the key
  if(subKey==keyEnd) {
    // If this is the root node of the tree, the key must have been empty/the object 
    // must have been the full set. In this case we place val into the Node without 
    // placing obj.
    if(subTree == tree) {
      subTree->fullKey = fullKey;
      subTree->val = val;
      subTree->originalVal = originalVal;
      return true;
      // Note: we should return true only if the set was modified but lattices have no equalSet method
    } else {
      // If obj is equal to the current obj in the node in all possible executions, replace the original
      // value with the new one
      ROSE_ASSERT(subTree->isObjSingleton == Node::isSingleton(obj));
      if(subTree->isObjSingleton) {
        ROSE_ASSERT(subTree->fullKey);
        subTree->val = val;
        // If an object is must-equal, it must be a singleton set and thus, cannot have any subsets
        ROSE_ASSERT(subTree->subsets.size()==0);
        return true;
      } else {
        // The old and new objects denote the same set but in different executions the actual concrete 
        // objects they denote may be different, so union the original and new lattices
        //ROSE_ASSERT(subTree->fullKey);
        
        // Create a fresh copy of subTree->val if we have not yet done so
        if(subTree->originalVal) {
          subTree->val = subTree->val->copySharedPtr();
          subTree->originalVal = false;
        }
        return subTree->val->meetUpdate(val);
      }
    }
  // If this is not the last sub-key, place it deeper in the tree
  } else {
    // If the current sub-key exists in the current tree node, recurse deeper
    map<comparablePtr, NodePtr>::iterator skIt = subTree->subsets.find(*subKey);
    std::list<comparablePtr>::const_iterator next = subKey; ++next;
    if(skIt != subTree->subsets.end()) {
      return insert(skIt->second, next, keyEnd, fullKey, obj, val, originalVal);
    // If it does not exist in the current tree node, add a sub-tree for it, which
    // will contain nodes for each sub-key between subKey and keyEnd
    } else {
      subTree->subsets[*subKey] = boost::make_shared<Node>(*subKey, next, keyEnd, fullKey, obj, val);
      return true;
    }
  }
}

// Removes the key matching the argument from the frontier.
// Return true if this causes the map to change and false otherwise.
bool HierarchicalAOM::remove(AbstractObjectPtr obj_arg) {
  AbstractObjectHierarchyPtr obj = boost::dynamic_pointer_cast<AbstractObjectHierarchy>(obj_arg);
  ROSE_ASSERT(obj);
  pair<pair<NodePtr, NodePtr>, bool> f = find(obj);
  NodePtr foundN = f.first.first;
  NodePtr parentN = f.first.second;
  // For remove operation, we use must equal policy
  if(foundN->isObjSingleton) {
    parentN->remove(foundN);
    return true;
  }
  return false;
}

// Returns the lattice object at the given sub-tree or an empty lattice if there isn't one
LatticePtr HierarchicalAOM::getVal(NodePtr subTree) {
  // If val is NULL, create an empty one
  if(!subTree->val) {
    // Create a fresh empty set at this sub-tree, recording that this set is 
    // not an original object from the user
    ROSE_ASSERT(subTree->originalVal);
    subTree->val = parent->defaultLat->copySharedPtr();
    subTree->val->setToEmpty();
    subTree->originalVal = false;
  }
  return subTree->val;
}

// Get all x-frontier for a given abstract memory object
LatticePtr HierarchicalAOM::get(AbstractObjectPtr obj_arg) {
  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::get()", scope::medium), 1, AbstractObjectMapDebugLevel)
  AbstractObjectHierarchyPtr obj = boost::dynamic_pointer_cast<AbstractObjectHierarchy>(obj_arg);
  ROSE_ASSERT(obj);
  SIGHT_VERB(dbg << "obj="<<obj_arg->str()<<endl, 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg << "key="<<obj->getHierKey()<<endl, 1, AbstractObjectMapDebugLevel)

  list<comparablePtr>::const_iterator curKey = obj->getHierKey()->begin(),
                                      keyEnd = obj->getHierKey()->end();

  
  // If the key is empty, return the lattice object at the root node
  if(curKey==keyEnd) return getVal(tree);

  // If the key is non-empty, iterate into the tree, following the key's sub-keys
  // and unioning any lattice objects found along the way.
  NodePtr subTree = tree;
          
  // The lattice object we'll return
  LatticePtr resLat(parent->defaultLat->copy());
  resLat->setToEmpty();
  
  do {
    SIGHT_VERB(dbg << "curKey="<<curKey->str()<<", found="<<(subTree->subsets.find(*curKey)!=subTree->subsets.end())<<endl, 1, AbstractObjectMapDebugLevel)
    SIGHT_VERB(dbg << "subTree="<<subTree<<endl, 2, AbstractObjectMapDebugLevel)
            
    // Union the current node's lattice object into resLat
    resLat->meetUpdate(getVal(subTree));
    SIGHT_VERB(dbg << "resLat="<<resLat->str()<<endl, 2, AbstractObjectMapDebugLevel)
            
    map<comparablePtr, NodePtr>::iterator sub = subTree->subsets.find(*curKey);
    // If the current sub-key exists in subsets
    if(sub!=subTree->subsets.end()) {
      // Advance to its sub-tree
      subTree = sub->second;
    // If we've reached the end of the tree
    } else {
      // Return the lattice object we've already computed
      return resLat;
    }
    
    ++curKey;
  } while(curKey != keyEnd);
  
  // We've now reached the end of the key but there may be more information deeper
  // in the tree about subsets of obj. Union their values into resLat.
  subTree->meetUpdate(resLat);
  SIGHT_VERB(dbg << "after sub-tree meet resLat="<<resLat->str()<<endl, 2, AbstractObjectMapDebugLevel)
  
  return resLat;
}

// Return < <n:NodePtr, p:NodePtr>, f:bool>, where:
// n - Node that corresponds to the key of the given object,
// p - The parent of n or NULLPart,
// f - boolean that indicates whether the node corresponds to the full key (true) or a prefix
//     of the key (false).
std::pair<std::pair<HierarchicalAOM::NodePtr, HierarchicalAOM::NodePtr>, bool> 
                            HierarchicalAOM::find(AbstractObjectHierarchyPtr obj) {
  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::find()", scope::medium), 2, AbstractObjectMapDebugLevel)
  list<comparablePtr>::const_iterator curKey = obj->getHierKey()->begin(),
                                      keyEnd = obj->getHierKey()->end();

  // If the key is empty, return the lattice object at the root node
  if(curKey==keyEnd) return make_pair(make_pair(tree, NULLNode), true);
  
  // If the key is non-empty, iterate into the tree, following the key's sub-keys
  // and unioning any lattice objects found along the way.
  NodePtr parent = NULLNode;
  NodePtr subTree = tree;
  
  do {
    SIGHT_VERB(dbg << "curKey="<<curKey->str()<<endl, 2, AbstractObjectMapDebugLevel)
    SIGHT_VERB(dbg << "found="<<(subTree->subsets.find(*curKey)!=subTree->subsets.end())<<endl, 2, AbstractObjectMapDebugLevel)
    SIGHT_VERB(dbg << "subTree="<<subTree<<endl, 2, AbstractObjectMapDebugLevel)
    map<comparablePtr, NodePtr>::iterator sub = subTree->subsets.find(*curKey);
    // If the current sub-key exists in subsets
    if(sub!=subTree->subsets.end()) {
      // Advance to its sub-tree
      parent = subTree;
      subTree = sub->second;
    // If we've reached the end of the tree
    } else {
      return make_pair(make_pair(subTree, parent), false);
    }
    
    ++curKey;
  } while(curKey != keyEnd);
  
  return make_pair(make_pair(subTree, parent), true);
}

// Set all the information associated Lattice object with this MemLocObjectPtr to full.
// Return true if this causes the object to change and false otherwise.
// This function does nothing because it is a set of abstract objects rather than a map from some abstract objects
// to others. 
bool HierarchicalAOM::setMLValueToFull(MemLocObjectPtr obj_arg) {
  bool modified=false;
  AbstractObjectHierarchyPtr obj = boost::dynamic_pointer_cast<AbstractObjectHierarchy>(obj_arg);
  ROSE_ASSERT(obj);
  
  // Search for the node that contains the closest key to ml's in the tree,
  // setting their lattices to NULL
  list<comparablePtr>::const_iterator curKey = obj->getHierKey()->begin(),
                                      keyEnd = obj->getHierKey()->end();
  
  // If the key is non-empty, iterate into the tree, following the key's sub-keys
  // and unioning any lattice objects found along the way.
  NodePtr subTree = tree;
  
  while(curKey != keyEnd) {
    // Set the current 
    modified = subTree->setValToFull() || modified;
    
    map<comparablePtr, NodePtr>::iterator sub = subTree->subsets.find(*curKey);
    // If the current sub-key exists in subsets
    if(sub!=subTree->subsets.end()) {
      // Advance to its sub-tree
      subTree = sub->second;
    // If we've reached the end of the tree
    } else {
      return modified;
    }
    
    ++curKey;
  }
  
  // If there is a tree node that matches ml's full key, make the whole
  // subtree full
  modified = subTree->setSubTreeToFull() || modified;
  
  return modified;
}

// Returns whether this lattice denotes the set of all possible execution prefixes.
bool HierarchicalAOM::isFullLat() {
  if(tree->val && tree->val->isFullLat()) {
    // If the root node has a full lattice, then we clean out its sub-trees since 
    // they're now storing redundant info
    tree->setSubTreeToFull();
    return false;
  }
  return true;
}
// Returns whether this lattice denotes the empty set.
bool HierarchicalAOM::isEmptyLat() {
  // Return true if the tree is empty
  return !tree->val && tree->subsets.size()==0;
}

std::string HierarchicalAOM::str(std::string indent) const {
  ostringstream s;
  s << "pedge = "<<parent->latPEdge->str()<<endl;
  s << tree;
  return s.str();
}

// Variant of the str method that can produce information specific to the current Part.
// Useful since AbstractObjects can change from one Part to another.
std::string HierarchicalAOM::strp(PartEdgePtr pedge, std::string indent) const {
  return str(indent);
}

// initializes this Lattice to its default state, if it is not already initialized
void HierarchicalAOM::initialize() {}

// returns a copy of this lattice
AbstractObjectMapKindPtr HierarchicalAOM::copy() const
{ return boost::make_shared<HierarchicalAOM>(*this); }

// overwrites the state of this Lattice with that of that Lattice
void HierarchicalAOM::copy(AbstractObjectMapKindPtr thatL) {
  HierarchicalAOMPtr that = boost::dynamic_pointer_cast <HierarchicalAOM> (thatL);
  ROSE_ASSERT(that);
  tree = that->tree;
  isFinite = that->isFinite;
}

// Called by analyses to transfer this lattice's contents from across function scopes from a caller function 
//    to a callee's scope and vice versa. If this lattice maintains any information on the basis of
//    individual MemLocObjects these mappings must be converted, with MemLocObjects that are keys of the ml2ml 
//    replaced with their corresponding values. If a given key of ml2ml does not appear in the lattice, it must
//    be added to the lattice and assigned a default initial value. In many cases (e.g. over-approximate sets 
//    of MemLocObjects) this may not require any actual insertions. If the value of a given ml2ml mapping is 
//    NULL (empty boost::shared_ptr), any information for MemLocObjects that must-equal to the key should be 
//    deleted.    
// Since the function is called for the scope change across some Part, it needs to account for the fact that
//    the keys in ml2ml are in scope on one side of Part, while the values on the other side. Specifically, it is
//    guaranteed that the keys are in scope at fromPEdge while the values are in scope at the edge returned 
//    by getPartEdge().
// remapML must return a freshly-allocated object.
AbstractObjectMapKindPtr HierarchicalAOM::remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge) {
  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::remapML()", scope::high), 1, AbstractObjectMapDebugLevel)
  HierarchicalAOMPtr newAOM = boost::dynamic_pointer_cast<HierarchicalAOM>(copy());
  for(set<MLMapping>::const_iterator m=ml2ml.begin(); m!=ml2ml.end(); m++) {
    AbstractObjectHierarchyPtr fromHier = boost::dynamic_pointer_cast<AbstractObjectHierarchy>(m->from);
    ROSE_ASSERT(fromHier);
    SIGHT_VERB(dbg << "from="<<m->from->str()<<endl, 1, AbstractObjectMapDebugLevel)
    SIGHT_VERB(dbg << "from key="<<fromHier->getHierKey()<<endl, 1, AbstractObjectMapDebugLevel)
    SIGHT_VERB(dbg << "to="<<(m->to?m->to->str():"NULL")<<endl, 1, AbstractObjectMapDebugLevel)
  
    pair<pair<NodePtr, NodePtr>, bool> f = newAOM->find(fromHier);
    NodePtr foundN = f.first.first;
    NodePtr parentN = f.first.second;
    bool fullKeyFound = f.second;
    if(parentN) { SIGHT_VERB(dbg << "parentN="<<parentN<<endl, 1, AbstractObjectMapDebugLevel) }
    if(foundN)  { SIGHT_VERB(dbg << "foundN="<<foundN<<endl, 1, AbstractObjectMapDebugLevel) }
    SIGHT_VERB(dbg << "fullKeyFound="<<fullKeyFound<<endl, 1, AbstractObjectMapDebugLevel)

    // If m->from mustEqual f->first, remove that node and insert under m->to
    if(fullKeyFound && foundN->isObjSingleton) {
      ROSE_ASSERT(foundN->isObjSingleton == Node::isSingleton(m->from));
      // If we need to replace from with to
      if(m->replaceMapping) {
        parentN->remove(foundN);

        // If the value of the current ml2ml mapping is not NULL (i.e. the key is a MemLoc
        // with a lifetime that is not limited to a given function and it carries over
        // across function boundaries)
        if(m->to)
          newAOM->insert(m->to, foundN->val);

      // If we add to but keep from
      } else {
        // If the value of the current ml2ml mapping is not NULL (i.e. the key is a MemLoc
        // with a lifetime that is not limited to a given function and it carries over
        // across function boundaries)
        if(m->to)
          newAOM->insert(m->to, foundN->val->copySharedPtr(), /*originalVal*/ false);
      }
    // If foundN equalsSet m->from or contains it, insert the mapping m->to => val but leave the prior one behind
    } else {
      if(m->to) {
        // Get the lattice object that describes the information just associated with
        // m->from. We focus in this way because f may be a strict superset of m->from
        // and some of the lattice objects in its sub-tree may correspond to objects 
        // that are disjoint from m->from.
        LatticePtr fromLat = get(m->from);
        // Insert this lattice under m->to, informing the HierarchicalAOM that it can treat
        // fromLat as a privately-owned object since no user code will ever directly see it
        // (it is generated inside of get()
        newAOM->insert(m->to, fromLat, /*originalVal*/ true);
      }
    }
  }
  return newAOM;
}

// Adds information about the MemLocObjects in newL to this Lattice, overwriting any information previously 
//    maintained in this lattice about them.
// Returns true if the Lattice state is modified and false otherwise.
bool HierarchicalAOM::replaceML(AbstractObjectMapKindPtr that_arg) {
  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::replaceML()", scope::high), 1, AbstractObjectMapDebugLevel)
  HierarchicalAOMPtr that = boost::dynamic_pointer_cast <HierarchicalAOM> (that_arg);
  bool modified = false;
  modified = (isFinite != (isFinite || that->isFinite)) || modified;
  isFinite = isFinite || that->isFinite;
  SIGHT_VERB(dbg<<"this="<<str()<<endl, 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg<<"that="<<that_arg->str()<<endl, 1, AbstractObjectMapDebugLevel)
  
  return replaceML(tree, that->tree) || modified;
}

// Recursive body of replaceML
bool HierarchicalAOM::replaceML(NodePtr thisST, NodePtr thatST) {
  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::replaceML()", scope::medium), 1, AbstractObjectMapDebugLevel)
  bool modified = false;
  
  ROSE_ASSERT(thisST->isObjSingleton == thatST->isObjSingleton);
  
  SIGHT_VERB(dbg<<"thisST->val="<<(thisST->val? thisST->val->str(): "NULL")<<endl, 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg<<"thatST->val="<<(thatST->val? thatST->val->str(): "NULL")<<endl, 1, AbstractObjectMapDebugLevel)
  
  // We currently don't check if val was modified since equalSet doesn't exist for Lattices
  modified = true;
  thisST->val = thatST->val;
  thisST->originalVal = true;
  
  // Iterate over the subsets of thatST, copying from their sub-trees into this
  for(map<comparablePtr, NodePtr>::iterator thatSub=thatST->subsets.begin(); thatSub!=thatST->subsets.end(); thatSub++) {
    SIGHT_VERB(dbg<<"thatSub key="<<thatSub->first->str()<<", found="<<(thisST->subsets.find(thatSub->first)!=thisST->subsets.end())<<endl, 1, AbstractObjectMapDebugLevel)
    
    // If the current sub-tree of that exists in this
    map<comparablePtr, NodePtr>::iterator thisSub=thisST->subsets.find(thatSub->first);
    if(thisSub!=thisST->subsets.end()) {
      // replace over from that sub-tree to this one
      modified = replaceML(thisSub->second, thatSub->second) || modified;
    // If it doesn't, copy the sub-tree from that into this
    } else {
      //thisST->subsets[thatSub->first] = boost::make_shared<Node>(thatSub->second);
      // Only copy if that sub-tree is live at this AOM's pedge
      if(thatSub->second->isLive(parent->latPEdge, parent->comp, parent->analysis)) {
        NodePtr newNode = boost::make_shared<Node>(thatSub->second, parent->latPEdge, parent->comp, parent->analysis);
        // If the copied sub-tree didn't end up being empty (due to the keys being out of scope), add it
        if(!newNode->isEmptyVal()) {
          thisST->subsets[thatSub->first] = newNode;
          modified = true;
        }
      }
    }
  }
  
  return modified;
}

// computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool HierarchicalAOM::meetUpdate(AbstractObjectMapKindPtr that_arg) {
  SIGHT_VERB_DECL(scope, ("HierarchicalAOM::meetUpdate()", scope::high), 1, AbstractObjectMapDebugLevel)
  HierarchicalAOMPtr that = boost::dynamic_pointer_cast <HierarchicalAOM> (that_arg);
  
  bool modified = false;
  modified = (isFinite != (isFinite || that->isFinite)) || modified;
  isFinite = isFinite || that->isFinite;
  
  return meetUpdate(tree, that->tree) || modified;
}

// Recursive body of meetUpdate
bool HierarchicalAOM::meetUpdate(NodePtr thisST, NodePtr thatST) {
  bool modified = false;

  ROSE_ASSERT(thisST->isObjSingleton == thatST->isObjSingleton);
  
  // Meet the values at this and that node
  SIGHT_VERB(dbg<<"thisST->val="<<(thisST->val? thisST->val->str(): "NULL")<<endl, 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB(dbg<<"thatST->val="<<(thatST->val? thatST->val->str(): "NULL")<<endl, 1, AbstractObjectMapDebugLevel)
  // If they're both non-NULL, call meetUpdate
  if(thisST->val && thatST->val) {
    // If val is an original copy from the user, copy it before modifying it
    if(thisST->originalVal) {
      thisST->val = thisST->val->copySharedPtr();
      thisST->originalVal = false;
    }
    modified = thisST->val->meetUpdate(thatST->val) || modified;
  // If this is NULL, and that is not, copy from that to this
  } else if(!thisST->val && thatST->val) {
    thisST->val = thatST->val;
    thisST->originalVal = true;
    modified = !thisST->val->isEmptyLat() || modified;
  }
  SIGHT_VERB(dbg<<"merged thisST->val="<<(thisST->val? thisST->val->str(): "NULL")<<endl, 1, AbstractObjectMapDebugLevel)
  SIGHT_VERB_DECL(indent, (), 1, AbstractObjectMapDebugLevel)
  // Otherwise, this doesn't change
  
  // Iterate over the subsets of thatST, unioning their sub-trees into this
  for(map<comparablePtr, NodePtr>::iterator thatSub=thatST->subsets.begin(); thatSub!=thatST->subsets.end(); thatSub++) {
    SIGHT_VERB(dbg<<"thatSub key="<<thatSub->first->str()<<", found="<<(thisST->subsets.find(thatSub->first)!=thisST->subsets.end())<<", live="<<thatSub->second->isLive(parent->latPEdge, parent->comp, parent->analysis)<<endl, 1, AbstractObjectMapDebugLevel)
    // If the current sub-tree of that exists in this
    map<comparablePtr, NodePtr>::iterator thisSub=thisST->subsets.find(thatSub->first);
    if(thisSub!=thisST->subsets.end()) {
      // Meet both sub-trees
      modified = meetUpdate(thisSub->second, thatSub->second) || modified;
      SIGHT_VERB(dbg<<"merged thisSub="<<thisSub->second<<endl, 1, AbstractObjectMapDebugLevel)
    // If it doesn't, copy the sub-tree from that into this
    } else {
      //thisST->subsets[thatSub->first] = boost::make_shared<Node>(thatSub->second);
      if(thatSub->second->isLive(parent->latPEdge, parent->comp, parent->analysis)) {
        NodePtr newNode = boost::make_shared<Node>(thatSub->second, parent->latPEdge, parent->comp, parent->analysis);
        // If the copied sub-tree didn't end up being empty (due to the keys being out of scope), add it
        if(!newNode->isEmptyVal()) {
          thisST->subsets[thatSub->first] = newNode;
          SIGHT_VERB_IF(2, AbstractObjectMapDebugLevel)
            dbg << "original node "<<thatSub->second<<endl;
            dbg << "new node "<<newNode<<endl;
          SIGHT_VERB_FI()
          modified = true;
        }
      }
    }
  }
  
  return modified;
}

bool HierarchicalAOM::finiteLattice()
{ return isFinite; }

bool HierarchicalAOM::operator==(AbstractObjectMapKindPtr that_arg) {
  HierarchicalAOMPtr that = boost::dynamic_pointer_cast<HierarchicalAOM>(that_arg);
  ROSE_ASSERT(that);
  ROSE_ASSERT(parent->latPEdge == that->parent->getPartEdge());
  // This will be written once we have the merging algorithm to test
  // these maps' frontiers for semantic equivalence
  return false;
}

}; // namespace fuse
