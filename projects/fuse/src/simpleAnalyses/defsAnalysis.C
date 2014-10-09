#include "defsAnalysis.h"
#include "abstract_object.h"
#include "stx_analysis.h"
#include <ostream>

namespace fuse {

/************************
 ***** DefsAnalysis *****
 ************************/

void DefsAnalysis::addDef(SgExpression* expr, PartEdgePtr pedge) {
  MemLocObjectPtr ml = composer->Expr2MemLoc(expr, pedge, cdip);
  cout << "    DefsAnalysis::addDef(expr="<<SgNode2Str(expr)<<")"<<endl;
  cout << "        ml="<<ml->str()<<endl;
  MemRegionObjectPtr mr = ml->getRegion();
  if(ml->getRegion()->isConcrete()) {
    set<SgNode*> defSyms = ml->getRegion()->getConcrete();
    for(set<SgNode*>::iterator d=defSyms.begin(); d!=defSyms.end(); d++)
      if(isSgVariableSymbol(*d)) {
        cout << "        Adding def "<<SgNode2Str(isSgVariableSymbol(*d))<<" : "<<vIDMap.variableId(isSgVariableSymbol(*d)).toString()<<endl;
        defs.insert(vIDMap.variableId(isSgVariableSymbol(*d)));
      }
  }
}

void DefsAnalysis::operator()(const CFGNode& n) {
  std::set<PartEdgePtr> refined;
  cout << "n="<<CFGNode2Str(n)<<endl;
  collectRefinedEdges(composer, refined, n.outEdges());

  for(std::set<PartEdgePtr>::iterator r=refined.begin(); r!=refined.end(); r++) {
    set<MemLocObjectPtr> defs;
    if(isSgBinaryOp(n.getNode())) {
      if(isSgAssignOp(n.getNode()) || isSgCompoundAssignOp(n.getNode()))
        addDef(isSgBinaryOp(n.getNode())->get_lhs_operand(), *r);
    } else if(isSgUnaryOp(n.getNode())) {
      if(isSgPlusPlusOp(n.getNode()) || isSgMinusMinusOp(n.getNode()))
        addDef(isSgUnaryOp(n.getNode())->get_operand(), *r);
    }
  }
}

void DefsAnalysis::runAnalysis(SgNode* target) {
  if(isSgExprStatement(target)) {
    runAnalysis(isSgExprStatement(target)->get_expression());
  } else if(isSgReturnStmt(target)) {
    runAnalysis(isSgReturnStmt(target)->get_expression());
  } else if(isSgIfStmt(target)) {
    runAnalysis(isSgIfStmt(target)->get_conditional());
  } else if(isSgCastExp(target)) {
    runAnalysis(isSgCastExp(target)->get_operand());
  } else if(isSgBasicBlock(target)) {
    // Recursively call toString on the first statement in the block
    const SgStatementPtrList & stmt = isSgBasicBlock(target)->get_statements();
    if(stmt.size()>0) runAnalysis(*stmt.begin());
  } else if(isSgFunctionDeclaration(target) || isSgFunctionDefinition(target) || isSgPragmaDeclaration(target)) {
  } else if(isSgExpression(target)) {
    if(!(isSgVarRefExp(target))) {
      cout << "target="<<SgNode2Str(target)<<endl;
      /*CFGNode start;
      if(isSgInitializedName(target)) start = CFGNode(target,  1);
      else if(isSgBinaryOp(target)) start = CFGNode(target,  2);
      else if(isSgUnaryOp(target)) {
        if(isSgCastExp(target)) start = CFGNode(target,  0);
        else if(isSgAddressOfOp(target) || isSgPointerDerefExp(target) || isSgPlusPlusOp(target) || isSgMinusMinusOp(target)) start = CFGNode(target,  1);
        else                   start = CFGNode(target,  2);
      }
      else if(isSgValueExp(target))    start = CFGNode(target,  1);
      else                        start = CFGNode(target,  0);*/
      /*CFGNode start = cfgBeginningOfConstruct(target);
      std::vector<CFGEdge> startOut = start.outEdges();
      ROSE_ASSERT(startOut.size()>0);
      std::vector<CFGEdge> startOutIn = startOut.begin()->target().inEdges();
      ROSE_ASSERT(startOutIn.size()>0);
      start = startOutIn.begin()->source();

      CFGNode end = cfgEndOfConstruct(target);
      std::vector<CFGEdge> endIn = end.inEdges();
      ROSE_ASSERT(endIn.size()>0);
      std::vector<CFGEdge> endInOut = endIn.begin()->target().outEdges();
      ROSE_ASSERT(endInOut.size()>0);
      end = endInOut.begin()->target();*/
//      cout << "start="<<CFGNode2Str(start)<<", end="<<CFGNode2Str(end)<<endl;
//      mapPath(*this, start, start);

      StxPartPtr start = StxPart::create(cfgBeginningOfConstruct(target), NULL);
      cout << "Initial start="<<start.str()<<endl;
      std::list<StxPartEdgePtr> startOut = start->outStxEdges();
      ROSE_ASSERT(startOut.size()>0);
      cout << "    start->out="<<(*startOut.begin())->str()<<endl;
/*      std::list<StxPartEdgePtr> startOutIn = (*startOut.begin())->stxTarget()->inStxEdges();
      ROSE_ASSERT(startOutIn.size()>0);
      start = (*startOutIn.begin())->source();*/
      start = (*startOut.begin())->stxTarget();
      cout << "    Final start="<<start.str()<<endl;

      StxPartPtr end = StxPart::create(cfgEndOfConstruct(target), NULL);
      cout << "Initial end="<<end.str()<<endl;
      std::list<StxPartEdgePtr> endIn = end->inStxEdges();
      ROSE_ASSERT(endIn.size()>0);
      cout << "    end->in="<<(*endIn.begin())->str()<<endl;
      std::list<StxPartEdgePtr> endInOut = (*endIn.begin())->stxSource()->outStxEdges();
      ROSE_ASSERT(endInOut.size()>0);
      end = (*endInOut.begin())->target();
      cout << "    Final end="<<end.str()<<endl;

      cout << "start="<<start.str()<<", end="<<end.str()<<endl;
      std::set<CFGNode> startNodes = start->CFGNodes(); assert(startNodes.size()==1);
      std::set<CFGNode> endNodes   = end->CFGNodes();   assert(endNodes.size()==1);
      mapPath(*this, *startNodes.begin(), *endNodes.begin());
    }
  }
}

/******************************
 ***** FuseRDAstAttribute *****
 ******************************/

// Maps labels in the AST to a reference to the set of defs at the label.
std::map<Label, VariableIdMapping::VariableIdSet> FuseRDAstAttribute::label2defsMap;
// Alternate representation that keeps the same data a set of pairs
std::set<std::pair<Label, VariableId> > FuseRDAstAttribute::label2defsSet;

// All the variables that were defined
std::set<VariableId> FuseRDAstAttribute::allDefs;

// Maps each variableId to all of its definition labels
std::map<VariableId, LabelSet> FuseRDAstAttribute::def2labels;

// All the labels that were annotated
LabelSet FuseRDAstAttribute::allLabelsSet;

FuseRDAstAttribute::FuseRDAstAttribute(Composer* composer, checkDataflowInfoPass* cdip, VariableIdMapping& vIDMap, Label label, Labeler& labeler) : 
                           composer(composer), cdip(cdip), vIDMap(vIDMap) {
  target = labeler.getNode(label);
  DefsAnalysis analysis(composer, cdip, vIDMap, defs);
  analysis.runAnalysis(labeler.getNode(label));
  label2defsMap[label] = defs;
  for(VariableIdMapping::VariableIdSet::iterator d=defs.begin(); d!=defs.end(); d++) {
    label2defsSet.insert(make_pair(label, *d));
    allDefs.insert(*d);
    def2labels[*d].insert(label);
  }
  allLabelsSet.insert(label);
}

FuseRDAstAttribute::~FuseRDAstAttribute() {}

string FuseRDAstAttribute::toString() {
  ostringstream s;
  s << "[FuseRDAstAttribute: "<<SgNode2Str(target)<<": ";
  for(VariableIdMapping::VariableIdSet::iterator d=defs.begin(); d!=defs.end(); d++) {
    if(d!=defs.begin()) s << ",";
    s << vIDMap.getSymbol(*d)->get_name().getString();
  }
  s << "]";
  return s.str();
}

void FuseRDAstAttribute::placeLabeler(Composer* composer, checkDataflowInfoPass* cdip, VariableIdMapping& vIDMap, Labeler& labeler) {
  for(Labeler::iterator i=labeler.begin(); i!=labeler.end(); ++i) {
    SgNode* node=labeler.getNode(*i);
    ROSE_ASSERT(node);
    cout << "================================"<<endl;
    cout << "Placing label at "<<SgNode2Str(node)<<endl;
    //node->setAttribute("fuse_cp_above", new ValueASTAttribute(node, composer, cdip, above, "fuse_cp_above"));
    node->setAttribute("fuse_rd", new FuseRDAstAttribute(composer, cdip, vIDMap, *i, labeler));
  }
}

}; // namespace fuse
