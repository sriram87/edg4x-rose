#pragma once

#include <boost/function.hpp>
#include "cfgUtils.h"
#include "comp_shared_ptr.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include "abstract_object.h"

namespace fuse {

class SSAMemLocObject {
  public:
  MemLocObjectPtr baseML;
  PartPtr loc;

  typedef enum {def, use,
                usedef // Denotes both a def and a use. This is the case for the variables at phi nodes,
                       // which are guaranteed to have the same values at their use and def points
               } accessType;
  accessType access;

  SSAMemLocObject(MemLocObjectPtr baseML, PartPtr loc, accessType access) : baseML(baseML), loc(loc), access(access) {}

  std::string accessType2Str(accessType access) { return (access==def?"def":(access==use?"use":"???")); }

  // pretty print
  std::string str(std::string indent="") const;

  // copy this object and return a pointer to it
  MemLocObjectPtr copyML() const;

  bool mayEqual(MemLocObjectPtr that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);
  bool mustEqual(MemLocObjectPtr that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);

  // Returns whether the two abstract objects denote the same set of concrete objects
  bool equalSet(MemLocObjectPtr o, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);

  // Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
  // by the given abstract object.
  bool subSet(MemLocObjectPtr o, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);

  bool isLive(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);

  // Computes the meet of this and that and saves the result in this
  // returns true if this causes this to change and false otherwise
  bool meetUpdate(MemLocObjectPtr that, PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);

  // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
  bool isFull(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);
  // Returns whether this AbstractObject denotes the empty set.
  bool isEmpty(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis);

  // Set this object to represent the set of all possible MemLocs
  // Return true if this causes the object to change and false otherwise.
  bool setToFull();

  // Set this Lattice object to represent the empty set of MemLocs.
  // Return true if this causes the object to change and false otherwise.
  bool setToEmpty();

  // Returns whether all instances of this class form a hierarchy. Every instance of the same
  // class created by the same analysis must return the same value from this method!
  bool isHierarchy() const;
  // AbstractObjects that form a hierarchy must inherit from the AbstractObjectHierarchy class

  // Returns a key that uniquely identifies this particular AbstractObject in the
  // set hierarchy.
  const AbstractObject::hierKeyPtr& getHierKey() const;
}; // class SSAMemLocObject
typedef boost::shared_ptr<SSAMemLocObject> SSAMemLocObjectPtr;

class ATSGraph: public boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, PartPtr, PartEdgePtr>
{
  public:
  typedef boost::graph_traits<ATSGraph>   GraphTraits;
  typedef GraphTraits::vertex_descriptor  Vertex;
  typedef GraphTraits::edge_descriptor    Edge;

  typedef std::map<Vertex, Vertex>        VertexVertexMap;
  typedef std::set<Vertex>                Vertices;

  std::set<PartPtr> startNodes;

  //! A map from a CFG node to the corresponding vertex
  std::map<PartPtr, Vertex> nodesToVertices;

  //! The dominator tree of this CFG.
  mutable VertexVertexMap dominatorTree;

  //! The postdominator tree of this CFG.
  mutable VertexVertexMap postdominatorTree;

  std::map<PartPtr, PartPtr> iDominatorMap;
  std::map<PartPtr, PartPtr> iPostDominatorMap;

  // Maps each Part to the set of Parts on its dominance frontier
  std::map<PartPtr, std::set<PartPtr> > dominanceFrontiers;

  // Inverse of dominanceFrontiers that maps each Part to the set of Parts that have it on their dominance frontiers
  std::map<PartPtr, std::set<PartPtr> > domFrontierOf;

  Composer* comp;
  ComposedAnalysis* analysis;

  ATSGraph(Composer* comp, ComposedAnalysis* analysis);

  protected:
  void buildCFG(PartPtr node,
                std::map<PartPtr, Vertex>& nodesAdded,
                std::set<PartPtr>& nodesProcessed);

  public:
  // Return the anchor Parts of a given function
  std::set<PartPtr> GetStartAStates();

  // There may be multiple terminal points in the application (multiple calls to exit(), returns from main(), etc.)
  std::set<PartPtr> GetEndAStates();

  const ATSGraph::VertexVertexMap& getDominatorTree();// const;

  const ATSGraph::VertexVertexMap& getPostdominatorTree();// const;

  Vertex getVertexForNode(PartPtr node) const;

  std::set<PartPtr> calculateIteratedDominanceFrontier(const std::vector<PartPtr>& startNodes);

  void calculateDominanceFrontiers();

  void showDominatorTree();
  void showDominanceFrontier();

  protected:
  // Map each Part to the set of MemLocs defined and used at this Part
  std::map<PartPtr, std::set<SSAMemLocObjectPtr> > defs;
  std::map<PartPtr, std::set<SSAMemLocObjectPtr> > uses;
  static std::set<SSAMemLocObjectPtr> emptySSAMemLocObjectSet;

  public:
  // Return the set of defs at this part
  const std::set<SSAMemLocObjectPtr>& getDefs(PartPtr part) const;

  // Return the set of uses at this part
  const std::set<SSAMemLocObjectPtr>& getUses(PartPtr part) const;

  // Collects all the defs and uses at each Part and stores them into defs and uses
  void collectDefsUses();

  void showDefsUses();

  // Maps each Part where a phi node should be placed (it is on the dominance frontier of some Parts)
  // to the set of MemLocs that will be defined at this phi node.
  /* For each MemLoc we maintain
  // two SSAMemLocs, one for its use immediately before the Phi node (first element in the pair)
  // and another for its def immediately after (second element). Both SSAMemLocs wrap the same base MemLoc */
  std::map<PartPtr, std::set<SSAMemLocObjectPtr> > phiMLs;

  // Finds all the Parts where phi nodes should be placed and identifies the MemLocs they must define
  void placePhiNodes();

  void showPhiNodes();

  // Returns whether the given Part is a phi node
  bool isPhiNode(PartPtr part) const { return phiMLs.find(part) != phiMLs.end(); }

  // Returns the set of MemLocs defined at the given Part
  const std::set<SSAMemLocObjectPtr>& getDefsUsesAtPhiNode(PartPtr part) const {
    ROSE_ASSERT(isPhiNode(part));
    std::map<PartPtr, std::set<SSAMemLocObjectPtr> >::const_iterator i=phiMLs.find(part);
    ROSE_ASSERT(i!=phiMLs.end());
    return i->second;
  }

  protected:
  // Maps each use to its Static Single Assignment defs
  std::map<SSAMemLocObjectPtr, std::set<SSAMemLocObjectPtr> > use2def;
  // Maps each def to its Static Single Assignment uses
  std::map<SSAMemLocObjectPtr, std::set<SSAMemLocObjectPtr> > def2use;

  void addDefUseLink(SSAMemLocObjectPtr use, SSAMemLocObjectPtr def);

  public:
  // Returns the SSA uses for the given def
  const std::set<SSAMemLocObjectPtr>& getUses(SSAMemLocObjectPtr def) const;
  // Returns the SSA defs for the given use
  const std::set<SSAMemLocObjectPtr>& getDefs(SSAMemLocObjectPtr use) const;

  // Computes the mapping from MemLoc uses to their defs
  void computeUse2Def();

  void showUse2Def();

  // Creates all the look-aside data structures required to represent the ATS in SSA form
  void buildSSA();
}; // class ATSGraph

}; // namespace fuse
