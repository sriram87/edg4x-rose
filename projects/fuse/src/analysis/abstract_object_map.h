#pragma once
#include "compose.h"
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <ostream>
#include <fstream>
#include <sstream>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/operators.hpp>
#include <filteredCFG.h>
#include <boost/unordered_map.hpp>
#include "CallGraph.h"
#include "abstract_object.h"
#include "latticeFull.h"
#include "data_structures.h"

#include <list>
#include <iostream>

namespace fuse {
  //extern int AbstractObjectMapDebugLevel;
  
  using namespace boost;
  using namespace std;
  //using namespace AbstractMemoryObject;
  
  typedef std::pair<AbstractObjectPtr, LatticePtr> MapElement;

  class AbstractObjectMap;
  
  // Abstract class that defines the methods that must be implemented by different kinds of
  // AbstractObjectMaps
  class AbstractObjectMapKind;
  typedef boost::shared_ptr<AbstractObjectMapKind> AbstractObjectMapKindPtr;
  class AbstractObjectMapKind {
    protected:
    AbstractObjectMap* parent;

    public:
    AbstractObjectMap* getParent() { return parent; }
    void setParent(AbstractObjectMap* newParent) { parent = newParent; }
    
    AbstractObjectMapKind() {}
    AbstractObjectMapKind(AbstractObjectMap* parent): parent(parent) {}
      
    // Add a new memory object --> lattice pair to the frontier.
    // Return true if this causes the map to change and false otherwise.
    // It is assumed that the given Lattice is now owned by the AbstractObjectMap and can be modified and deleted by it.
    virtual bool insert(AbstractObjectPtr key, LatticePtr val)=0;
    
    // Removes the key matching the argument from the frontier.
    // Return true if this causes the map to change and false otherwise.
    virtual bool remove(AbstractObjectPtr key)=0;
    
    // Get all x-frontier for a given abstract memory object
    virtual LatticePtr get(AbstractObjectPtr key)=0;
    
    // Set all the information associated Lattice object with this MemLocObjectPtr to full.
    // Return true if this causes the object to change and false otherwise.
    // This function does nothing because it is a set of abstract objects rather than a map from some abstract objects
    // to others. 
    virtual bool setMLValueToFull(MemLocObjectPtr ml)=0;

    // Returns whether this lattice denotes the set of all possible execution prefixes.
    virtual bool isFullLat()=0;
    // Returns whether this lattice denotes the empty set.
    virtual bool isEmptyLat()=0;

    virtual std::string str(std::string indent="") const=0;
    // Variant of the str method that can produce information specific to the current Part.
    // Useful since AbstractObjects can change from one Part to another.
    virtual std::string strp(PartEdgePtr pedge, std::string indent="") const=0;
    
    // -----------------
    // Lattice methods
    // initializes this Lattice to its default state, if it is not already initialized
    virtual void initialize()=0;
    
    // returns a copy of this AbstractObjectMapKind
    virtual AbstractObjectMapKindPtr copy() const=0;
    
    // overwrites the state of this Lattice with that of that Lattice
    virtual void copy(AbstractObjectMapKindPtr thatAOM)=0;
    
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
    virtual AbstractObjectMapKindPtr remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge)=0;

    // Adds information about the MemLocObjects in newL to this Lattice, overwriting any information previously 
    //    maintained in this lattice about them.
    // Returns true if the Lattice state is modified and false otherwise.
    virtual bool replaceML(AbstractObjectMapKindPtr newL)=0;
    
    // computes the meet of this and that and saves the result in this
    // returns true if this causes this to change and false otherwise
    virtual bool meetUpdate(AbstractObjectMapKindPtr that)=0;
       
    virtual bool finiteLattice()=0;
    
    virtual bool operator==(AbstractObjectMapKindPtr that)=0;
  }; // class AbstractObjectMapKind
  
  class AbstractObjectMap : public Lattice {
    public: 
    AbstractObjectMapKindPtr implementation;
    
    // Pointer to a default instance of the given lattice type.
    // AbstractObjectMap::get() can return copies of this object to indicate that
    // there is no mapping for a given AbstractObject.
    LatticePtr         defaultLat;
    
    // Different states of the set of mappings represented by this AbstractObject Map
    // between denotes a specific set implemented in implementation.
    // full and empty denote the set of all or no mappings and implementation may be NULL
    typedef enum {full, empty, between} state;
    state mapState;
    std::string state2Str(state s) { return (s==full?"full":(s==empty?"empty":(s==between?"between":"???"))); }

    // The composer and analysis within which this map is being maintained
    Composer* comp;
    ComposedAnalysis* analysis;
    
    public:
    AbstractObjectMap(const AbstractObjectMap& that);
    // Create a copy of that object but using the given implementation. The implementation object
    // is incorporated directly without making any additional copies.
    AbstractObjectMap(const AbstractObjectMap& that, AbstractObjectMapKindPtr implementation);
    AbstractObjectMap(LatticePtr defaultLat_, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);
    ~AbstractObjectMap();

    protected:
    // Initializes the AOM implementation that will be used inside this AbstractObjectMap. 
    // The choice of implementation will be based on the semantics that are implemented
    // by the key provided as the argument, which are assumed to be implemented by all
    // other keys that may ever be provided.
    void initImplementation(AbstractObjectPtr key);
    
    public:
    // Add a new memory object --> lattice pair to the frontier.
    // Return true if this causes the map to change and false otherwise.
    // It is assumed that the given Lattice is now owned by the AbstractObjectMap and can be modified and deleted by it.
    bool insert(AbstractObjectPtr key, LatticePtr val);
    
    // Removes the key matching the argument from the frontier.
    // Return true if this causes the map to change and false otherwise.
    bool remove(AbstractObjectPtr key);
    
    // Get all x-frontier for a given abstract memory object
    LatticePtr get(AbstractObjectPtr key);
        
    // Set this Lattice object to represent the set of all possible execution prefixes.
    // Return true if this causes the object to change and false otherwise.
    bool setToFull();
    
    // Set this Lattice object to represent the of no execution prefixes (empty set).
    // Return true if this causes the object to change and false otherwise.
    bool setToEmpty();
    
    // Set all the information associated Lattice object with this MemLocObjectPtr to full.
    // Return true if this causes the object to change and false otherwise.
    bool setMLValueToFull(MemLocObjectPtr ml);

    // Returns whether this lattice denotes the set of all possible execution prefixes.
    bool isFullLat();
    // Returns whether this lattice denotes the empty set.
    bool isEmptyLat();

    std::string str(std::string indent="") const;
    // Variant of the str method that can produce information specific to the current Part.
    // Useful since AbstractObjects can change from one Part to another.
    std::string strp(PartEdgePtr pedge, std::string indent="") const;
    
    // -----------------
    // Lattice methods
    // initializes this Lattice to its default state, if it is not already initialized
    void initialize();
    
    // returns a copy of this lattice
    Lattice* copy() const;
    
    // overwrites the state of this Lattice with that of that Lattice
    void copy(Lattice* thatL);
    
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
    Lattice* remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge);

    // Adds information about the MemLocObjects in newL to this Lattice, overwriting any information previously 
    //    maintained in this lattice about them.
    // Returns true if the Lattice state is modified and false otherwise.
    bool replaceML(Lattice* newL);
    
    // computes the meet of this and that and saves the result in this
    // returns true if this causes this to change and false otherwise
    bool meetUpdate(Lattice* that);
    
    // Identify keys that are must-equal to each other and merge their lattices
    // Return true if this causes the object to change and false otherwise.
    bool compressMustEq();

    // Remove all mappings with dead keys from this map.
    // Return true if this causes the object to change and false otherwise.
    bool compressDead();
    
    bool finiteLattice();
    
    bool operator==(Lattice* that);
  }; // class AbstractObjectMap

  extern AbstractObjectMapKindPtr NULLAbstractObjectMapKind;
  
  // The most generic AbstractObjectMap, which places no constraints on the 
  // relationships among the AbstractObjects used as keys
  class GenericAOM : public AbstractObjectMapKind {
    protected: 
    list<MapElement>   items;
    // True if all the lattices inserted into this map are finite
    // and false otherwise
    bool isFinite;

    public:
    //AbstractObjectMap() : isFinite(true), comp(NULL) {}
    GenericAOM(const GenericAOM& that, AbstractObjectMap* parent) : 
                                   AbstractObjectMapKind(parent),
                                   items (that.items),
                                   isFinite(that.isFinite)
    {}
    GenericAOM(const GenericAOM& that) : 
                                   AbstractObjectMapKind(that.parent),
                                   items (that.items),
                                   isFinite(that.isFinite)
    {}
    GenericAOM(AbstractObjectMap* parent) : AbstractObjectMapKind(parent), isFinite(true) {}
    ~GenericAOM() {}

    public:
    // Add a new memory object --> lattice pair to the frontier.
    // Return true if this causes the map to change and false otherwise.
    // It is assumed that the given Lattice is now owned by the AbstractObjectMap and can be modified and deleted by it.
    bool insert(AbstractObjectPtr key, LatticePtr val);
    
    // Removes the key matching the argument from the frontier.
    // Return true if this causes the map to change and false otherwise.
    bool remove(AbstractObjectPtr key);
    
    // Get all x-frontier for a given abstract memory object
    LatticePtr get(AbstractObjectPtr key);
    
    // Set all the information associated Lattice object with this MemLocObjectPtr to full.
    // Return true if this causes the object to change and false otherwise.
    // This function does nothing because it is a set of abstract objects rather than a map from some abstract objects
    // to others. 
    bool setMLValueToFull(MemLocObjectPtr ml);

    // Returns whether this lattice denotes the set of all possible execution prefixes.
    bool isFullLat();
    // Returns whether this lattice denotes the empty set.
    bool isEmptyLat();

    std::string str(std::string indent="") const;
    // Variant of the str method that can produce information specific to the current Part.
    // Useful since AbstractObjects can change from one Part to another.
    std::string strp(PartEdgePtr pedge, std::string indent="") const;
    
    // -----------------
    // Lattice methods
    // initializes this Lattice to its default state, if it is not already initialized
    void initialize();
    
    // returns a copy of this lattice
    AbstractObjectMapKindPtr copy() const;
    
    // overwrites the state of this Lattice with that of that Lattice
    void copy(AbstractObjectMapKindPtr thatL);
    
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
    AbstractObjectMapKindPtr remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge);

    // Adds information about the MemLocObjects in newL to this Lattice, overwriting any information previously 
    //    maintained in this lattice about them.
    // Returns true if the Lattice state is modified and false otherwise.
    bool replaceML(AbstractObjectMapKindPtr newL);
    
    // computes the meet of this and that and saves the result in this
    // returns true if this causes this to change and false otherwise
    bool meetUpdate(AbstractObjectMapKindPtr that);
    
    // Identify keys that are must-equal to each other and merge their lattices
    // Return true if this causes the object to change and false otherwise.
    bool compressMustEq();

    // Remove all mappings with dead keys from this map.
    // Return true if this causes the object to change and false otherwise.
    bool compressDead();
    
    bool finiteLattice();
    
    bool operator==(AbstractObjectMapKindPtr that);
  }; // class GenericAOM
  typedef boost::shared_ptr<GenericAOM> GenericAOMPtr;
  
  // AOM specialized to keys that inherit from AbstractObjectHierarchy.
  class HierarchicalAOM : public AbstractObjectMapKind {
    // The tree represents a mapping from objects that inherit from 
    // AbstractObjectHierarchy keys to LatticePtrs. Hierarchical objects are 
    // lists of comparable objects. When a new obj->value mapping is added to 
    // the AOM we iterate through obj's list of comparable objects 
    // (obj->getHierKey()), create one node in the tree for each sub-key, and 
    // place the value LatticePtr at the leaf node. Thus, if we see a LatticePtr 
    // at a leaf node, the hierarchical object to which this LatticePtr is 
    // mapped is the list of all the comparison objects at its Node and its 
    // parent Nodes.
    // [a]
    //  ->[b]
    //     ->[c]->val1
    //  ->[x]->val2
    // 
    // Denotes a map where [a,b,c] is mapped to LatticePtr val1 and [a,x] is mapped to val2.
    // 
    // The semantics of hierarchical objects is: if obj1->getHierKey() is a prefix 
    // of obj2->getHierKey(), then the set denoted by obj1 contains the set denoted by 
    // obj2. Otherwise, they are disjoint. 
    // 
    // insert(newObj, newVal)
    // - If the tree already contains newObj.getHierKey()
    //   Let oldObj be the AbstractObjectPtr that has this key
    //   - If oldObj.mustEqual(newKey), its Node's mapping is replaced by newVal
    //     In this case oldObj's Node cannot have children since it is known to be a singleton set
    //   - Else, it must be that oldObj.mayEqual(newKey) and its mapping is 
    //     updated with the union of newVal and oldVal.
    // - If the tree contains a prefix of newObj.getHierKey() (including an empty prefix)
    //   Add the mapping from newObj.getHierKey() to newVal to the tree deeper in the tree
    // 
    // get(obj)
    // - find the tree node denoted by obj.getHierKey()
    // - walk up the tree unioning the node's Lattice with those of its tree ancestors
    //
    // meet(obj1, obj2)
    
    public:
    class Node;
    typedef boost::shared_ptr<Node> NodePtr;
    class Node {
      friend std::ostream& operator<<(std::ostream& os, const NodePtr& node);
      friend class HierarchicalAOM;
      protected:
      // The sub-key that describes the object at this node (the full key is the
      // list of all the sub-keys at this node and above it in the tree)
      comparablePtr key;

      // The full key that fully defines this node (its full key is equal to the
      // concatenation of sub-keys along the path from the tree's root to this node).
      AbstractObjectHierarchy::hierKeyPtr fullKey;
      
      // All objects with the same key must denote the same set. However, when the set
      // they denote is a singleton, they all objects with this key are mustEqual to
      // each other. As such, we keep track of whether the set is a singleton so that
      // we can answer this question without explicitly calling mustEqual().
      bool isObjSingleton;
      
      // The value mapped to the object.
      LatticePtr val;
      
      // Records whether val is an original object given to this AOM by the user (true)
      // or whether it is a copy created by HierarchicalAOM as part of unioning
      // two LatticePtrs together (false). If it is a copy, we're sure that no other
      // object has access to it and thus, we're free to overwrite it to improve efficiency.
      bool originalVal;
      
      // Maps sub-keys to the Nodes that hold their obj->value mappings
      std::map<comparablePtr, NodePtr> subsets;
      
      public:
      Node();
      // Creates a sub-tree that holds the remaining portions of the key, from subKey until keyEnd
      // and places obj and val at the leaf of this sub-tree
      Node(comparablePtr myKey, std::list<comparablePtr>::const_iterator subKey, std::list<comparablePtr>::const_iterator keyEnd, 
           AbstractObjectHierarchy::hierKeyPtr fullKey, AbstractObjectPtr obj, LatticePtr val);
      Node(const NodePtr& that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);

      protected:
      // Returns whether the set denoted by thet given object is a singleton
      static bool isSingleton(AbstractObjectPtr obj);
      
      // Set the value at this node to full.
      // Return true if this causes the object to change and false otherwise.
      bool setValToFull();
      
      // Set the value at this node AND all of its sub-trees to full.
      // Return true if this causes the object to change and false otherwise.
      bool setSubTreeToFull();

      // Removes the given child node from the given parent node
      void remove(NodePtr child);
      
      // Union the values of this node and all of its children with the given lattice.
      // Return whether this causes lat to change.
      bool meetUpdate(LatticePtr lat);
      
      // Returns whether the object at this Node is definitely dead at the given PartEdge
      //bool mustDead(PartEdgePtr pedge);

      // Returns whether the fullKey at this node may be live at the given pedge
      bool isLive(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);
      
      // Returns whether the set that this node maps its key to is empty.
      // This is the case if all the values in its sub-tree denote the empty set
      bool isEmptyVal() const;

      SightStream& print(SightStream& s) const;
    }; // class Node
    static HierarchicalAOM::NodePtr NULLNode;
    
    protected:
    NodePtr tree;
    
    // True if all the lattices inserted into this map are finite
    // and false otherwise
    bool isFinite;
    
    public:
    HierarchicalAOM(const HierarchicalAOM& that, AbstractObjectMap* parent);
    HierarchicalAOM(const HierarchicalAOM& that);
    HierarchicalAOM(AbstractObjectMap* parent);

    public:
    // Add a new memory object --> lattice pair to the frontier.
    // Return true if this causes the map to change and false otherwise.
    // It is assumed that the given Lattice is now owned by the AbstractObjectMap and can be modified and deleted by it.
    bool insert(AbstractObjectPtr key, LatticePtr val, bool originalVal);
    bool insert(AbstractObjectPtr key, LatticePtr val)
    { return insert(key, val, false); }
    
    protected:
    // Recursive body of insert
    bool insert(NodePtr subTree,
                std::list<comparablePtr>::const_iterator subKey, std::list<comparablePtr>::const_iterator keyEnd,
                AbstractObjectHierarchy::hierKeyPtr fullKey, AbstractObjectPtr obj, LatticePtr val, bool originalVal);
    
    public:
    // Removes the key matching the argument from the frontier.
    // Return true if this causes the map to change and false otherwise.
    bool remove(AbstractObjectPtr key);
    
    // Returns the lattice object at the given sub-tree or an empty lattice if there isn't one
    LatticePtr getVal(NodePtr subTree);
    
    // Get all x-frontier for a given abstract memory object
    LatticePtr get(AbstractObjectPtr key);
    
    protected:
    // Return the Node that corresponds to the key of the given object and a boolean
    // that indicates whether the node corresponds to the full key (true) or a prefix
    // of the key (false).
    std::pair<std::pair<NodePtr, NodePtr>, bool> find(AbstractObjectHierarchyPtr obj);
    
    public:
    // Set all the information associated Lattice object with this MemLocObjectPtr to full.
    // Return true if this causes the object to change and false otherwise.
    // This function does nothing because it is a set of abstract objects rather than a map from some abstract objects
    // to others. 
    bool setMLValueToFull(MemLocObjectPtr ml);

    // Returns whether this lattice denotes the set of all possible execution prefixes.
    bool isFullLat();
    // Returns whether this lattice denotes the empty set.
    bool isEmptyLat();

    std::string str(std::string indent="") const;
    // Variant of the str method that can produce information specific to the current Part.
    // Useful since AbstractObjects can change from one Part to another.
    std::string strp(PartEdgePtr pedge, std::string indent="") const;
    
    // -----------------
    // Lattice methods
    // initializes this Lattice to its default state, if it is not already initialized
    void initialize();
    
    // returns a copy of this lattice
    AbstractObjectMapKindPtr copy() const;
    
    // overwrites the state of this Lattice with that of that Lattice
    void copy(AbstractObjectMapKindPtr thatL);
    
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
    AbstractObjectMapKindPtr remapML(const std::set<MLMapping>& ml2ml, PartEdgePtr fromPEdge);

    // Adds information about the MemLocObjects in newL to this Lattice, overwriting any information previously 
    //    maintained in this lattice about them.
    // Returns true if the Lattice state is modified and false otherwise.
    bool replaceML(AbstractObjectMapKindPtr newL);
    
    // Recursive body of replaceML
    bool replaceML(NodePtr thisST, NodePtr thatST);
    
    // computes the meet of this and that and saves the result in this
    // returns true if this causes this to change and false otherwise
    bool meetUpdate(AbstractObjectMapKindPtr that);
    
    // Recursive body of meetUpdate
    bool meetUpdate(NodePtr thisST, NodePtr thatST);
    
    // Identify keys that are must-equal to each other and merge their lattices
    // Return true if this causes the object to change and false otherwise.
    bool compressMustEq();
    
    bool finiteLattice();
    
    bool operator==(AbstractObjectMapKindPtr that);
  }; // class HierarchicalAOM
  typedef boost::shared_ptr<HierarchicalAOM> HierarchicalAOMPtr;
  
  std::ostream& operator<<(std::ostream& os, const HierarchicalAOM::NodePtr& node);
}; // namespace fuse;
