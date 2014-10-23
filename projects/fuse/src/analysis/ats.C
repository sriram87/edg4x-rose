#include "sage3basic.h"
#include "partitions.h"
#include "abstract_object.h"
#include "compose.h"
#include <boostGraphCFG.h>
#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include "ats.h"

#include "sight.h"
#include "sight_verbosity.h"
using namespace std;
using namespace sight;
using namespace boost;

namespace fuse {

#define atsDebugLevel 0

/* ###########################
   ##### SSAMemLocObject #####
   ########################### */

SSAMemLocObject::SSAMemLocObject(const SSAMemLocObject& that) : MemLocObject(that)
{
  baseML = that.baseML;
  loc = that.loc;
  access = that.access;
}

// pretty print
string SSAMemLocObject::str(string indent) const {
  ostringstream oss;
  oss << "[SSAMemLocObject: access="<<accessType2Str(access)<", baseML="<< baseML->str(indent+"    ")<<endl;
  oss << indent << "    loc=" << loc->str(indent+"    ")<<"]";
  return oss.str();
}

// copy this object and return a pointer to it
MemLocObjectPtr SSAMemLocObject::copyML() const
{ return boost::make_shared<SSAMemLocObject>(*this); }

bool SSAMemLocObject::mayEqual(MemLocObjectPtr that_arg, PartEdgePtr pedge_arg, Composer* comp, ComposedAnalysis* analysis) {
  SSAMemLocObjectPtr that = boost::dynamic_pointer_cast<SSAMemLocObject>(that_arg);
  assert(that);
  CallCtxSensPartEdgePtr pedge = dynamicConstPtrCast<CallCtxSensPartEdge>(pedge_arg);
  assert(pedge);

  // If these MemLocs were defined at the same SSA node and have the same type of access (use, def, usedef)
  if(loc == that->loc && access == that->access)
    return baseML->mayEqual(that->baseML, pedge->getParent(), comp, analysis);
  // Otherwise, they may not be equal
  else
    return false;
}

bool SSAMemLocObject::mustEqual(MemLocObjectPtr that_arg, PartEdgePtr pedge_arg, Composer* comp, ComposedAnalysis* analysis) {
  SSAMemLocObjectPtr that = boost::dynamic_pointer_cast<SSAMemLocObject>(that_arg);
  assert(that);
  CallCtxSensPartEdgePtr pedge = dynamicConstPtrCast<CallCtxSensPartEdge>(pedge_arg);
  assert(pedge);

  // If these MemLocs were defined at the same SSA node and have the same type of access (use, def, usedef)
  if(loc == that->loc && access == that->access)
    return baseML->mustEqual(that->baseML, pedge->getParent(), comp, analysis);
  // Otherwise, they are not must-equal
  else
    return false;
}

// Returns whether the two abstract objects denote the same set of concrete objects
bool SSAMemLocObject::equalSet(MemLocObjectPtr that_arg, PartEdgePtr pedge_arg, Composer* comp, ComposedAnalysis* analysis) {
  SSAMemLocObjectPtr that = boost::dynamic_pointer_cast<SSAMemLocObject>(that_arg);
  assert(that);
  CallCtxSensPartEdgePtr pedge = dynamicConstPtrCast<CallCtxSensPartEdge>(pedge_arg);
  assert(pedge);

  // If these MemLocs were defined at the same SSA node and have the same type of access (use, def, usedef)
  if(loc == that->loc && access == that->access)
    return baseML->equalSet(that->baseML, pedge->getParent(), comp, analysis);
  // Otherwise, their sets must not be equal
  else
    return false;
}

// Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
// by the given abstract object.
bool SSAMemLocObject::subSet(MemLocObjectPtr that_arg, PartEdgePtr pedge_arg, Composer* comp, ComposedAnalysis* analysis) {
  SSAMemLocObjectPtr that = boost::dynamic_pointer_cast<SSAMemLocObject>(that_arg);
  assert(that);
  CallCtxSensPartEdgePtr pedge = dynamicConstPtrCast<CallCtxSensPartEdge>(pedge_arg);
  assert(pedge);

  // If these MemLocs were defined at the same SSA node and have the same type of access (use, def, usedef)
  if(loc == that->loc && access == that->access)
    return baseML->subSet(that->baseML, pedge->getParent(), comp, analysis);
  // Otherwise, this is not a subset of that
  else
    return false;
}

bool SSAMemLocObject::isLive(PartEdgePtr pedge_arg, Composer* comp, ComposedAnalysis* analysis) {
  CallCtxSensPartEdgePtr pedge = dynamicConstPtrCast<CallCtxSensPartEdge>(pedge_arg);
  assert(pedge);

  // SSAMemLocs are live at exactly the same Parts where the base MemLoc was live since SSA does not affect liveness or scope
  return ml->isLive(pedge->getParent(), comp, analysis);
}

// Computes the meet of this and that and saves the result in this
// returns true if this causes this to change and false otherwise
bool SSAMemLocObject::meetUpdate(MemLocObjectPtr that_arg, PartEdgePtr pedge_arg, Composer* comp, ComposedAnalysis* analysis) {
  SSAMemLocObjectPtr that = boost::dynamic_pointer_cast<SSAMemLocObject>(that_arg);
  assert(that);
  CallCtxSensPartEdgePtr pedge = dynamicConstPtrCast<CallCtxSensPartEdge>(pedge_arg);
  assert(pedge);

  bool modified = false;
  // Meet the MemLoc but don't modify the definition of the location
  modified = baseML->meetUpdate(that->baseML, pedge->getParent(), comp, analysis) || modified;
  return modified;
}

// Returns whether this AbstractObject denotes the set of all possible execution prefixes.
bool SSAMemLocObject::isFull(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  return baseML->isFull(pedge->getParent(), comp, analysis);
}

// Returns whether this AbstractObject denotes the empty set.
bool SSAMemLocObject::isEmpty(PartEdgePtr pedge, Composer* comp, ComposedAnalysis* analysis) {
  return baseML->isEmpty(pedge->getParent(), comp, analysis);
}

// Set this object to represent the set of all possible MemLocs
// Return true if this causes the object to change and false otherwise.
bool SSAMemLocObject::setToFull() {
  bool modified = false;
  assert(0);
  // WE CANNOT IMPLEMENT THIS BECAUSE SETTING baseML TO FULL WOULD CHANGE IT, WHICH WOULD BE BAD FOR
  // ALL OF ITS OTHER USERS. AS SUCH, WE NEED AbstractObjects TO IMPLEMENT A COPY FUNCTIONALITY.
  //modified = ml->setToFull(comp, baseML, pedge->getParent(), ccsa) || modified;
  return modified;
}
// Set this Lattice object to represent the empty set of MemLocs.
// Return true if this causes the object to change and false otherwise.
bool SSAMemLocObject::setToEmpty() {
  assert(0);
}

// Returns whether all instances of this class form a hierarchy. Every instance of the same
// class created by the same analysis must return the same value from this method!
bool SSAMemLocObject::isHierarchy() const {
  return baseML->isHierarchy();
}

// Returns a key that uniquely identifies this particular AbstractObject in the
// set hierarchy.
const AbstractObjectHierarchy::hierKeyPtr& SSAMemLocObject::getHierKey() const {
  AbstractObjectHierarchyPtr hierBaseML = boost::dynamic_pointer_cast<AbstractObjectHierarchy>(baseML);
  ROSE_ASSERT(hierBaseML);
  return hierBaseML->getHierKey();
}

/********************
 ***** ATSGraph *****
 ********************/

ATSGraph::ATSGraph(Composer* comp, ComposedAnalysis* analysis) : comp(comp), analysis(analysis)
{
  // The following two variables are used to record the nodes traversed.
  nodesToVertices.clear();
  set<PartPtr> nodesProcessed;
  set<PartPtr> startStates = comp->GetStartAStates(analysis);

  /*cout << "startStates="<<endl;
  BOOST_FOREACH (const PartPtr& s, startStates) {
    cout << "    "<<s->str()<<endl;
  }*/

  BOOST_FOREACH (const PartPtr& s, startStates) {
    buildCFG(s, nodesToVertices, nodesProcessed);
  }

  //cout << "ATSGraph::ATSGraph #dominanceFrontiers="<<dominanceFrontiers.size()<<endl;
  SIGHT_VERB_IF(2, atsDebugLevel)
  scope s(txt()<<"nodesToVertices(#"<<nodesToVertices.size()<<")");
  for(map<PartPtr, Vertex>::iterator i=nodesToVertices.begin(); i!=nodesToVertices.end(); i++)
    dbg << "    "<<i->second<<": "<<i->first->str()<<endl;
  SIGHT_VERB_FI()
}

void ATSGraph::buildCFG(PartPtr node,
                        std::map<PartPtr, Vertex>& nodesAdded,
                        std::set<PartPtr>& nodesProcessed)
{
  //scope a("buildCFG");
  //dbg << "node="<<node->str()<<endl;
  // Each node is processed exactly once
  if (nodesProcessed.count(node) > 0) return;
  nodesProcessed.insert(node);

  std::map<PartPtr, Vertex>::iterator iter;
  bool inserted;
  Vertex from, to;

  // Add the source node.
  const PartPtr& src = node;

  boost::tie(iter, inserted) = nodesAdded.insert(std::make_pair(src, Vertex()));
  if (inserted)
  {
    from = add_vertex(*this);
    (*this)[from] = src;
    iter->second = from;
  }
  else
  {
    from = iter->second;
  }

  list<PartEdgePtr> outEdges = node->outEdges();
  BOOST_FOREACH (const PartEdgePtr& pedge, outEdges)
  {
    // For each out edge, add the target node.
    PartPtr tar = pedge->target();

    boost::tie(iter, inserted) = nodesAdded.insert(std::make_pair(tar, Vertex()));
    if (inserted)
    {
      to = add_vertex(*this);
      (*this)[to] = tar;
      iter->second = to;
    }
    else
    {
      to = iter->second;
    }

    // Add the edge.
    Edge edge = add_edge(from, to, *this).first;
    (*this)[edge] = pedge;

    // Build the CFG recursively.
    buildCFG(tar, nodesAdded, nodesProcessed);
  }
}

// Return the anchor Parts of a given function
std::set<PartPtr> ATSGraph::GetStartAStates()
{ return comp->GetStartAStates(analysis); }

// There may be multiple terminal points in the application (multiple calls to exit(), returns from main(), etc.)
std::set<PartPtr> ATSGraph::GetEndAStates()
{ return comp->GetEndAStates(analysis); }

ATSGraph::Vertex ATSGraph::getVertexForNode(PartPtr node) const
{
  std::map<PartPtr, Vertex>::const_iterator vertexIter = nodesToVertices.find(node);
  if (vertexIter == nodesToVertices.end())
    return GraphTraits::null_vertex();
  else
  {
    ROSE_ASSERT((*this)[vertexIter->second] == node);
    return vertexIter->second;
  }
}

set<PartPtr> ATSGraph::calculateIteratedDominanceFrontier(const vector<PartPtr>& startNodes)
{
  set<PartPtr> result;
  set<PartPtr> visitedNodes;
  vector<PartPtr> worklist;

  worklist.insert(worklist.end(), startNodes.begin(), startNodes.end());

  while (!worklist.empty())
  {
    PartPtr currentNode = worklist.back();
    worklist.pop_back();
    visitedNodes.insert(currentNode);

    //Get the dominance frontier of the node and add it to the results
    ROSE_ASSERT(dominanceFrontiers.count(currentNode) != 0);
    const set<PartPtr>& dominanceFrontier = dominanceFrontiers.find(currentNode)->second;

    //Add all the children to the result and to the worklist
    BOOST_FOREACH(PartPtr dfNode, dominanceFrontier)
    {
      if (visitedNodes.count(dfNode) > 0)
        continue;

      result.insert(dfNode);
      worklist.push_back(dfNode);
    }
  }

  return result;
}

void ATSGraph::showDominatorTree() {
  ostringstream dot;

  dot << "digraph DominatorTree {"<<endl;

  typedef graph_traits<ATSGraph>::vertex_iterator vertex_iter;
  for(std::pair<vertex_iter, vertex_iter> vp = vertices(*this); vp.first != vp.second; ++vp.first)
    dot << "node"<<*(vp.first)<<" [label=\""<<(*this)[*(vp.first)]->str()<<"\"];"<<endl;

  for(std::map<Vertex, Vertex>::iterator d=dominatorTree.begin(); d!=dominatorTree.end(); d++)
    dot << "node"<<d->second << " -> node"<<d->first<<";"<<endl;

  dot << "}"<<endl;

  sight::structure::graph g(dot.str());
}

void ATSGraph::showDominanceFrontier() {
  ostringstream dot;

  dot << "digraph DominatorTree {"<<endl;
  typedef graph_traits<ATSGraph>::vertex_iterator vertex_iter;
  for(std::pair<vertex_iter, vertex_iter> vp = vertices(*this); vp.first != vp.second; ++vp.first)
    dot << "node"<<*(vp.first)<<" [label=\""<<(*this)[*(vp.first)]->str()<<"\"];"<<endl;

  dbg << "showDominanceFrontier:"<<endl;
  for(map<PartPtr, std::set<PartPtr> >::const_iterator n=dominanceFrontiers.begin(); n!=dominanceFrontiers.end(); n++) {
    dbg << "n="<<n->first->str()<<endl;
    BOOST_FOREACH(PartPtr f, n->second) { dbg << "    :"<<f->str()<<endl; }
    vector<PartPtr> cur; cur.push_back(n->first);
    set<PartPtr> front = calculateIteratedDominanceFrontier(cur);
    BOOST_FOREACH(PartPtr f, front) { dbg << "    ~"<<f->str()<<endl; }
    for(std::set<PartPtr>::const_iterator i=front.begin(); i!=front.end(); i++)
      dot << "node"<<nodesToVertices[n->first]<<" -> node"<<nodesToVertices[*i]<<endl;
  }

  dot << "}"<<endl;

  sight::structure::graph g(dot.str());
}

std::set<SSAMemLocObjectPtr> ATSGraph::emptySSAMemLocObjectSet;

// Return the set of defs at this part
const std::set<SSAMemLocObjectPtr>& ATSGraph::getDefs(PartPtr part) const {
  std::map<PartPtr, std::set<SSAMemLocObjectPtr> >::const_iterator i=defs.find(part);
  if(i!=defs.end()) return i->second;
  else              return emptySSAMemLocObjectSet;
}

// Return the set of uses at this part
const std::set<SSAMemLocObjectPtr>& ATSGraph::getUses(PartPtr part) const {
  std::map<PartPtr, std::set<SSAMemLocObjectPtr> >::const_iterator i=uses.find(part);
  if(i!=uses.end()) return i->second;
  else              return emptySSAMemLocObjectSet;
}

// Collects all the defs and uses at each Part and stores them into defs and uses
void ATSGraph::collectDefsUses() {
  set<PartPtr> startStates = comp->GetStartAStates(analysis);
  for(fw_partEdgeIterator state(startStates); !state.isEnd(); state++) {
    PartPtr part = state.getPart();
    set<SSAMemLocObjectPtr>& partDefs = defs[part];
    set<SSAMemLocObjectPtr>& partUses = uses[part];

    set<CFGNode> cfgNodes = part->CFGNodes();
    for(set<CFGNode>::iterator cn=cfgNodes.begin(); cn!=cfgNodes.end(); cn++) {
      SgNode* sgn = cn->getNode();
      if(SgBinaryOp* binOp = isSgBinaryOp(sgn)) {
        if(isSgAssignOp(sgn) || isSgCompoundAssignOp(sgn))
          partDefs.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(binOp->get_lhs_operand(), state.getPartEdge(), analysis), part, SSAMemLocObject::def));

        if(!isSgAssignOp(sgn))
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(binOp->get_lhs_operand(), state.getPartEdge(), analysis), part, SSAMemLocObject::use));

        partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(binOp->get_rhs_operand(), state.getPartEdge(), analysis), SSAMemLocObject::use));
        partDefs.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(binOp, state.getPartEdge(), analysis), part, SSAMemLocObject::def));

        // !!!! Should add a use of the memory location of the LHS
      } else if(SgUnaryOp* unOp = isSgUnaryOp(sgn)) {
        if(isSgMinusMinusOp(sgn) || isSgPlusPlusOp(sgn))
          partDefs.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(unOp->get_operand(), state.getPartEdge(), analysis), part, SSAMemLocObject::def));

        partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(unOp->get_operand(), state.getPartEdge(), analysis), SSAMemLocObject::use));
        partDefs.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(unOp, state.getPartEdge(), analysis), part, SSAMemLocObject::def));
      } else if(SgInitializedName* iname = isSgInitializedName(sgn)) {
        if(iname->get_initializer())
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(iname->get_initializer(), state.getPartEdge(), analysis), SSAMemLocObject::use));
        partDefs.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(iname, state.getPartEdge(), analysis), part, SSAMemLocObject::def));
      } else if(SgIfStmt* ifStmt = isSgIfStmt(sgn)) {
        if(SgExprStatement* exprStmt = isSgExprStatement(ifStmt->get_conditional()))
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(exprStmt->get_expression(), state.getPartEdge(), analysis), SSAMemLocObject::use));
      } else if(SgWhileStmt* whileStmt = isSgWhileStmt(sgn)) {
        if(SgExprStatement* exprStmt = isSgExprStatement(whileStmt->get_condition()))
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(exprStmt->get_expression(), state.getPartEdge(), analysis), SSAMemLocObject::use));
      } else if(SgDoWhileStmt* doWhileStmt = isSgDoWhileStmt(sgn)) {
        if(SgExprStatement* exprStmt = isSgExprStatement(doWhileStmt->get_condition()))
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(exprStmt->get_expression(), state.getPartEdge(), analysis), SSAMemLocObject::use));
      } else if(SgForStatement* forStmt = isSgForStatement(sgn)) {
        if(SgExprStatement* exprStmt = isSgExprStatement(forStmt->get_test()))
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(exprStmt, state.getPartEdge(), analysis), SSAMemLocObject::use));
        if(SgExprStatement* exprStmt = isSgExprStatement(forStmt->get_increment()))
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(exprStmt, state.getPartEdge(), analysis), SSAMemLocObject::use));
      } else if(SgSwitchStatement* switchStmt = isSgSwitchStatement(sgn)) {
        if(SgExprStatement* exprStmt = isSgExprStatement(switchStmt->get_item_selector()))
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(exprStmt->get_expression(), state.getPartEdge(), analysis), SSAMemLocObject::use));
      } else if(SgCallExpression* call = isSgCallExpression(sgn)) {
        if(!isSgFunctionCallExp(call->get_function()))
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(call->get_function(), state.getPartEdge(), analysis), SSAMemLocObject::use));
        SgExprListExp* args = call->get_args();
        for(SgExpressionPtrList::iterator a=args->get_expressions().begin(); a!=args->get_expressions().end(); a++)
          partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(*a, state.getPartEdge(), analysis), SSAMemLocObject::use));
        partDefs.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(call, state.getPartEdge(), analysis), part, SSAMemLocObject::def));
      } else if(SgAssignInitializer* init = isSgAssignInitializer(sgn)) {
        partUses.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(init->get_operand(), state.getPartEdge(), analysis), SSAMemLocObject::use));
        partDefs.insert(boost::make_shared<SSAMemLocObject>(comp->Expr2MemLoc(init, state.getPartEdge(), analysis), part, SSAMemLocObject::def));
      }
    }
  }
}

void ATSGraph::showDefsUses() {
  dbg << "<u>defs</u>"<<endl;
  dbg << "<table border=1>";
  for(map<PartPtr, set<SSAMemLocObjectPtr> >::iterator d=defs.begin(); d!=defs.end(); d++) {
    dbg << "<tr><td>"<<d->first->str()<<"</td><td>";
    for(set<SSAMemLocObjectPtr>::iterator i=d->second.begin(); i!=d->second.end(); i++)
      dbg << (*i)->str()<<endl;
    dbg << "</td></tr>";
  }
  dbg << "</table>";

  dbg <<endl;
  dbg << "<u>uses</u>"<<endl;
  dbg << "<table border=1>";
  for(map<PartPtr, set<SSAMemLocObjectPtr> >::iterator u=uses.begin(); u!=uses.end(); u++) {
    dbg << "<tr><td>"<<u->first->str()<<"</td><td>";
    for(set<SSAMemLocObjectPtr>::iterator i=u->second.begin(); i!=u->second.end(); i++)
      dbg << (*i)->str()<<endl;
    dbg << "</td></tr>";
  }
  dbg << "</table>";
}

// Finds all the Parts where phi nodes should be placed and identifies the MemLocs they must define
void ATSGraph::placePhiNodes() {
  SIGHT_VERB_DECL(scope, ("placePhiNodes"), 2, atsDebugLevel)
  // Iterate over all the parts that are on the dominance frontier of some other parts
  for(map<PartPtr, set<PartPtr> >::iterator i=domFrontierOf.begin(); i!=domFrontierOf.end(); i++) {
    SIGHT_VERB_DECL(scope, (txt()<<"Part "<<i->first->str()), 2, atsDebugLevel)
    // We will place a Phi node at i->first that re-defines all the MemLocs defined at the nodes
    // that have i->first on their dominance frontier

    // Iterate over all the Parts that have i->first on their dominance frontiers
    for(set<PartPtr>::const_iterator j=i->second.begin(); j!=i->second.end(); j++) {
      SIGHT_VERB_DECL(scope, (txt()<<"Frontier "<<(*j)->str()<<" #"<<defs[*j].size()), 2, atsDebugLevel)
      // Iterate over all the MemLocs defined at Part j
      for(set<SSAMemLocObjectPtr>::iterator d=defs[*j].begin(); d!=defs[*j].end(); d++) {
        SIGHT_VERB(dbg << "Live="<<(((*d)->baseML->isLive(i->first.get()->inEdgeFromAny(), comp, analysis)))<<" def="<<(*d)->str()<<endl, 2, atsDebugLevel)
        // If this def is live at i->first, add it as a phi definition at that Part
        if(((*d)->baseML->isLive(i->first.get()->inEdgeFromAny(), comp, analysis))) {
          // Insert into phiDefs[i->first] if no other node in the set denotes the same set as *d
          bool foundEqSet=false;
          for(set<SSAMemLocObjectPtr>::iterator ml=phiMLs[i->first].begin(); ml!=phiMLs[i->first].end(); ml++) {
            if(ml->baseML->equalSet(*d, i->first.get()->inEdgeFromAny(), comp, analysis)) {
              foundEqSet = true;
              break;
            }
          }
          SSAMemLocObjectPtr phiAccess = boost::make_shared<SSAMemLocObjectPtr>(*d, i->first, SSAMemLocObject::usedef);
          addDefUseLink(phiAccess, *d);
          if(!foundEqSet)
            phiMLs[i->first].insert(phiAccess);
        }
      }
    }
  }
}

// Relaces all instances of string search in subject with replace
// From http://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-of-a-string-with-another-string
void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void ATSGraph::showPhiNodes() {
  /*dbg << "<u>phiDefs</u>"<<endl;
  dbg << "<table>";
  for(map<PartPtr, set<MemLocObjectPtr> >::iterator d=phiDefs.begin(); d!=phiDefs.end(); d++) {
    dbg << "<tr><td>"<<d->first->str()<<"</td></td>";
    for(set<MemLocObjectPtr>::iterator i=d->second.begin(); i!=d->second.end(); i++)
      dbg << (*i)->str()<<endl;
    dbg << "</tr>";
  }
  dbg << "</table>";*/

  // First, assign unique nodeIDs to all parts in the ATS and emit node descriptions for all parts
  ostringstream dot;
  set<PartPtr> startStates = comp->GetStartAStates(analysis);

  std::map<PartPtr, int> partID;
  int clusterCount=0;
  dot << "digraph PhiNodes {"<<endl;
  {
  set<PartPtr> visited;
  set<PartPtr> worklist = startStates;
  //for(fw_partEdgeIterator state(startStates); !state.isEnd(); state++) {
  while(worklist.size()>0) {
    //PartPtr part = state.getPart();
    PartPtr part = *worklist.begin();
    worklist.erase(part);
    if(visited.find(part) != visited.end()) continue;
    visited.insert(part);
    list<PartEdgePtr> out = part->outEdges();
    for(list<PartEdgePtr>::iterator i=out.begin(); i!=out.end(); i++) worklist.insert((*i)->target());

    int ID = partID.size();

    // If this is a phi node
    if(phiDefs.find(part) != phiDefs.end()) {
      dot << "subgraph cluster_"<<(clusterCount++)<<" {"<<endl;
      dot << "  style=filled;"<<endl;
      dot << "  color=lightgrey;"<<endl;
      dot << "  node"<<ID<<" [label=\""<<part->str()<<"\" color=red];"<<endl;
      int j=0;
      for(set<SSAMemLocObjectPtr> i=phiMLs[part].begin(); i!=phiMLs[part].end(); ++i, ++j){
        string mlLabel = i->baseML->str();
        // Replace all the line breaks in mlLabel with explicit text "\n";
        ReplaceStringInPlace(mlLabel, "\n", "\\n");
        dot << "  node"<<ID<<"_"<<j<<" [label=\""<<mlLabel<<"\" border=0];"<<endl;
        if(j==0)
          dot << "node"<<ID<<" -> node"<<ID<<"_"<<j<<" [style=invis];"<<endl;
        else
          dot << "node"<<ID<<"_"<<(j-1)<<" -> node"<<ID<<"_"<<j<<" [style=invis];"<<endl;
      }
      dot << "}"<<endl;
    }
    else
      dot << "node"<<ID<<" [label=\""<<part->str()<<"\"];"<<endl;
    partID[part] = ID;
  }
  }
  // Second, add transition system edges
  {
  set<PartPtr> visited;
  set<PartPtr> worklist = startStates;
  //for(fw_partEdgeIterator state(startStates); !state.isEnd(); state++) {
  while(worklist.size()>0) {
    //PartPtr part = state.getPart();
    PartPtr part = *worklist.begin();
    worklist.erase(part);
    if(visited.find(part) != visited.end()) continue;
    visited.insert(part);
    list<PartEdgePtr> out = part->outEdges();
    for(list<PartEdgePtr>::iterator i=out.begin(); i!=out.end(); i++) worklist.insert((*i)->target());

    //list<PartEdgePtr> out = part->outEdges();
    for(list<PartEdgePtr>::iterator e=out.begin(); e!=out.end(); e++) {
      dot << "node"<<partID[part]<<" -> node"<<partID[(*e)->target()]<<";"<<endl;
      //dbg << part->str()<<"/"<<(*e)->source()->str()<<"/#"<<partID[part]<<" -> "<<(*e)->target()->str()<<"/#"<<partID[(*e)->target()]<<endl;
    }
  }
  }
  dot << "}"<<endl;

  sight::structure::graph g(dot.str());
}

void ATSGraph::addDefUseLink(SSAMemLocObjectPtr use, SSAMemLocObjectPtr def) {
  use2def[use].insert(def);
  def2use[def].insert(use);
}

// Returns the SSA uses for the given def
const std::set<SSAMemLocObjectPtr>& getUses(SSAMemLocObjectPtr def) const {
  map<SSAMemLocObjectPtr, set<SSAMemLocObjectPtr> >::const_iterator i=def2use.find(def);
  ROSE_ASSERT(i != def2use.end());
  return i->second;
}

// Returns the SSA defs for the given use
const std::set<SSAMemLocObjectPtr>& getDefs(SSAMemLocObjectPtr use) const {
  map<SSAMemLocObjectPtr, set<SSAMemLocObjectPtr> >::const_iterator i=use2def.find(use);
  ROSE_ASSERT(i != use2def.end());
  return i->second;
}

// Computes the mapping from MemLoc uses to their defs
void ATSGraph::computeUse2Def() {
  SIGHT_VERB_DECL(scope, ("computeUse2Def"), 2, atsDebugLevel)

  // Maps each Part The set of MemLocs that were used later than the Part and that
  // we're keeping track of to identify their definition points.
  map<PartPtr, set<SSAMemLocObjectPtr> > liveUses;

  // Iterate over the ATS backwards looking for uses and propagating them to their defs
  set<PartPtr> endStates = comp->GetEndAStates(analysis);

  for(bw_partEdgeIterator state(endStates); !state.isEnd(); state++) {
    PartPtr part = state.getPart();
    SIGHT_VERB_DECL(scope, (txt()<<"part "<<part->str()), 2, atsDebugLevel)

    // The liveUses set we'll propagate to this part's predecessors
    set<SSAMemLocObjectPtr> predLiveUses;

    // Initialize predLiveUses to be the set assigned to this node and erase the liveUses
    // mapping for this part
    if(liveUses.find(part) != liveUses.end()) {
      predLiveUses = liveUses[part];
      liveUses.erase(part);
    }

    // Remove from liveUses any that have been defined at the current part
    for(set<SSAMemLocObjectPtr>::iterator u=predLiveUses.begin(); u!=predLiveUses.end(); ) {
      bool curErased=false;
      if(defs.find(part)!=defs.end()) {
        for(set<SSAMemLocObjectPtr>::iterator d=defs[part].begin(); d!=defs[part].end(); d++) {
          // If the current def definitely writes to the current use
          if((*d)->baseML->mustEqual((*u)->baseML, state.getPartEdge(), comp, analysis)) {
            // Connect the use of u to this def
            addDefUseLink(*u, *d);
            SIGHT_VERB_IF(2, atsDebugLevel)
            dbg << "must definition: "<<(*d)->str()<<endl;
            dbg << "   use="<<(*u)->str()<<endl;
            SIGHT_VERB_FI()

            // Erase  the current element of liveUses and advance the iterator u
            predLiveUses.erase(u++);
            curErased=true;
            break;

          // Else, ff the current def possibly writes to the current use
          } else if((*d)->baseML->mustEqual((*u)->baseML, state.getPartEdge(), comp, analysis)) {
            // Connect the use of u to this def
            addDefUseLink(*u, *d);
            SIGHT_VERB_IF(2, atsDebugLevel)
            dbg << "may definition: "<<(*d)->str()<<endl;
            dbg << "   use="<<(*u)->str()<<endl;
            SIGHT_VERB_FI()

            // Do not remove this use from predLiveUses since this may not be the most recent def of *u
          }
        }
      }

      if(phiMLs.find(part)!=phiMLs.end()) {
        SIGHT_VERB(dbg << "Phi Node"<<endl, 2, atsDebugLevel)
        for(set<SSAMemLocObjectPtr>::iterator ml=phiMLs[i->first].begin(); ml!=phiMLs[i->first].end(); ml++) {
          SIGHT_VERB(dbg << "phiAccess="<<(*ml)->str()<<endl, 2, atsDebugLevel)

          // If the current def definitely writes to the current use
          if((*ml)->baseML->mustEqual((*u)->baseML, state.getPartEdge(), comp, analysis)) {
            // Connect the use of u to this def
            addDefUseLink(*u, *ml);
            SIGHT_VERB_IF(2, atsDebugLevel)
            dbg << "must phi definition: "<<(*ml)->str()<<endl;
            dbg << "   use="<<(*u)->str()<<endl;
            SIGHT_VERB_FI()

            // Erase  the current element of liveUses and advance the iterator u
            predLiveUses.erase(u++);
            curErased=true;
            break;

          // Else, if the current def possibly writes to the current use
          } else if((*ml)->baseML->mustEqual((*u)->baseML, state.getPartEdge(), comp, analysis)) {
            // Connect the use of u to this def
            addDefUseLink(*u, *ml);
            SIGHT_VERB_IF(2, atsDebugLevel)
            dbg << "may phi definition: "<<(*ml)->str()<<endl;
            dbg << "   use="<<(*u)->str()<<endl;
            SIGHT_VERB_FI()

            // Do not remove this use from predLiveUses since this may not be the most recent def of *u
          }
        }
      }

      // If we have not yet advanced the iterator u, do so now
      if(!curErased) u++;
    }

    // Remove from predLiveUses all the MemLocs that are dead at this Part
    for(set<SSAMemLocObjectPtr>::iterator u=predLiveUses.begin(); u!=predLiveUses.end(); ) {
      if(!((*u)->ml->isLive(state.getPartEdge(), comp, analysis))) {
        SIGHT_VERB(dbg << "erasing dead "<<(*u)->str()<<endl, 2, atsDebugLevel)
        predLiveUses.erase(u++);
      } else
        ++u;
    }

    // Add the uses at the current part into predLiveUses
    for(set<SSAMemLocObjectPtr>::iterator u=uses[part].begin(); u!=uses[part].end(); u++)
      predLiveUses.insert(*u);

    // If this is a phi node, add all the defs at this node as uses
    /*if(phiDefs.find(part)!=phiDefs.end()) {
      for(set<MemLocObjectPtr>::iterator d=phiDefs[part].begin(); d!=phiDefs[part].end(); d++) {
        predLiveUses.insert(boost::make_shared<SSAMemLocObject>(*d, part, SSAMemLocObject::use));
      }
    }*/

    SIGHT_VERB_IF(2, atsDebugLevel)
    { scope s("predLiveUses");
    for(set<SSAMemLocObjectPtr>::iterator u=predLiveUses.begin(); u!=predLiveUses.end(); u++) {
      dbg << (*u)->str()<<endl;
    } }
    SIGHT_VERB_FI()

    // Pass predLiveUses to all of this part's predecessors
    list<PartEdgePtr> in = part->inEdges();
    { SIGHT_VERB_DECL(scope, ("predecessors"), 2, atsDebugLevel)
    for(list<PartEdgePtr>::iterator e=in.begin(); e!=in.end(); e++) {
      SIGHT_VERB(dbg << "pred "<<(*e)->source()->str()<<endl, 2, atsDebugLevel)
      liveUses[(*e)->source()] = predLiveUses;
    }}
  }
}

void ATSGraph::showUse2Def() {
  ostringstream dot;

  for(map<SSAMemLocObjectPtr, set<SSAMemLocObjectPtr>  >::iterator i=use2def.begin(); i!=use2def.end(); i++) {
    scope s(i->first->str());
    for(set<SSAMemLocObjectPtr>::iterator j=i->second.begin(); j!=i->second.end(); j++)
      dbg << "    "<<(*j)->str()<<endl;
  }

  dot << "digraph Use2Def {"<<endl;

  // First, assign unique nodeIDs to all parts in the ATS and emit node descriptions for all parts
  set<PartPtr> startStates = comp->GetStartAStates(analysis);
  std::map<PartPtr, int> partID;
  for(fw_partEdgeIterator state(startStates); !state.isEnd(); state++) {
    PartPtr part = state.getPart();
    int ID=partID.size();
    dot << "node"<<ID<<" [label=\""<<part->str()<<"\"";
    if(phiDefs.find(part) != phiDefs.end())
      dot << " color=red"<<endl;
    dot << "];"<<endl;
    partID[part] = ID;
  }

  // Next, add transition system edges
  /*for(fw_partEdgeIterator state(startStates); !state.isEnd(); state++) {
    PartPtr src = state.getPartEdge()->source();
    PartPtr tgt = state.getPartEdge()->target();
    dot << "node"<<partID[src]<<" -> node"<<partID[tgt]<<";"<<endl;
  }*/
  {
    set<PartPtr> visited;
    for(fw_partEdgeIterator state(startStates); !state.isEnd(); state++) {
      PartPtr part = state.getPart();
      list<PartEdgePtr> out = part->outEdges();
      if(visited.find(part) != visited.end()) continue;
      visited.insert(part);

      for(list<PartEdgePtr>::iterator e=out.begin(); e!=out.end(); e++) {
        dot << "node"<<partID[part]<<" -> node"<<partID[(*e)->target()]<<";"<<endl;
      }
    }
  }

  // Finally, add edges for def-use relations
  for(map<SSAMemLocObjectPtr, set<SSAMemLocObjectPtr> >::iterator i=use2def.begin(); i!=use2def.end(); ++i) {
    for(set<SSAMemLocObjectPtr>::iterator j=i->second.begin(); j!=i->second.end(); ++j) {
      string mlLabel = (*j)->str();
      // Replace all the line breaks in mlLabel with explicit text "\n";
      ReplaceStringInPlace(mlLabel, "\n", "\\n");
      dot << "node"<<partID[(*j)->loc]<<" -> node"<<partID[i->first->loc]<<" [label=\""<<mlLabel<<"\" color=blue];"<<endl;
    }
  }

  dot << "}"<<endl;

  sight::structure::graph g(dot.str());
}

void ATSGraph::buildSSA() {
  getDominatorTree();
  SIGHT_VERB_IF(2, atsDebugLevel)
  scope s("Dominator Tree");
  showDominatorTree();
  SIGHT_VERB_FI()

  calculateDominanceFrontiers();
  SIGHT_VERB_IF(2, atsDebugLevel)
  scope s("Dominance Frontier");
  showDominanceFrontier();
  SIGHT_VERB_FI()

  collectDefsUses();
  SIGHT_VERB_IF(2, atsDebugLevel)
  scope s("DefsUses");
  showDefsUses();
  SIGHT_VERB_FI()

  placePhiNodes();
  SIGHT_VERB_IF(2, atsDebugLevel)
  scope s("Phi Nodes");
  showPhiNodes();
  SIGHT_VERB_FI()

  computeUse2Def();
  SIGHT_VERB_IF(2, atsDebugLevel)
  scope s("Use2Def");
  showUse2Def();
  SIGHT_VERB_FI()
}

}; // namespace fuse
