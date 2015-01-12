#include "sage3basic.h"
#include "compose.h"
#include "tight_composer.h"
#include "const_prop_analysis.h"
#include "live_dead_analysis.h"
#include "call_context_sensitivity_analysis.h"
#include "dead_path_elim_analysis.h"
#include "printAnalysisStates.h"
#include "pointsToAnalysis.h"
#include "virtualMethodAnalysis.h"
#include "dynamicMonitor.h"
//#include "constantAnalysis.h"
//#include "valueNumbering.h"
#include "defsAnalysis.h"
#include "analysis_tester.h"
#include <vector>
#include <ctype.h>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>
#include "sageBuilder.h"
#include "sageInterface.h"
#include "sight.h"
using namespace sight;
#include <sys/time.h>

using namespace std;
using namespace fuse;
using namespace boost::xpressive;
using namespace SageBuilder;
using namespace SageInterface;
//using namespace scc_private;

// Regex expressions for the composition command, defined globally so that they can be used inside main 
// (where they're initialized) as well as inside output_nested_results()
sregex composer, lcComposer, lpComposer, tComposer,
       analysis, noAnalysis, cpAnalysis, ldAnalysis, ccsAnalysis, dpAnalysis, ptAnalysis,
                 vmAnalysis, spcpAnalysis, spvnAnalysis,
                 ssacpAnalysis, ssaldAnalysis, ssaccsAnalysis, ssadpAnalysis, ssaptAnalysis,
                 ssavmAnalysis, ssaspcpAnalysis, ssaspvnAnalysis,
                 dynMonitor,
       analysisList, compSpec;

// Displays nested results to std::cout with indenting
struct output_nested_results
{
  typedef enum {looseSeq, loosePar, tight, unknown} composerType;
  
  int tabs_;
  composerType parentComposerType;
  list<ComposedAnalysis*>& parentSubAnalyses;
  Composer** parentComposer; 
  checkDataflowInfoPass* cdip;
      
  output_nested_results(int tabs, composerType& parentComposerType, list<ComposedAnalysis*>& parentSubAnalyses, Composer** parentComposer, checkDataflowInfoPass* cdip)
      : tabs_( tabs ), parentComposerType(parentComposerType), parentSubAnalyses(parentSubAnalyses), parentComposer(parentComposer), cdip(cdip)
  {
  }

  string composerType2Str(composerType type)
  { return (type==looseSeq? "looseSeq": (type==loosePar? "loosePar": (type==tight? "tight": (type==unknown? "unknown": "???")))); }

  template< typename BidiIterT >
  void operator ()( match_results< BidiIterT > const &what ) 
  {
    //scope s("operator ()");
    // first, do some indenting
    typedef typename std::iterator_traits< BidiIterT >::value_type char_type;
    //char_type space_ch = char_type(' ');

    string match = what[0];
    
    // If this term is an analysis rather than a composer
    smatch subWhat;
    //dbg << "match="<<match<<", parentComposerType="<<composerType2Str(parentComposerType)<<", parentComposer="<<parentComposer<<endl;
    if(regex_match(match, subWhat, analysis)) {
      /*cout << "analysis match="<<match<<", smatch="<<subWhat<<endl;
      cout << "regex_match(match, subWhat, cpAnalysis)="<<regex_match(match, subWhat, cpAnalysis)<<endl;
      cout << "regex_match(match, subWhat, ssacpAnalysis)="<<regex_match(match, subWhat, ssacpAnalysis)<<endl;*/
      // Create the selected analysis and add it to the parent's sub-analysis list
           if(regex_match(match, subWhat, cpAnalysis))      {  /*dbg << "CP"<<endl;*/ parentSubAnalyses.push_back(new ConstantPropagationAnalysis(/*useSSA*/false)); }
      else if(regex_match(match, subWhat, ssacpAnalysis))   {  /*dbg << "CP"<<endl;*/ parentSubAnalyses.push_back(new ConstantPropagationAnalysis(/*useSSA*/true)); }
      else if(regex_match(match, subWhat, ldAnalysis))      { parentSubAnalyses.push_back(new LiveDeadMemAnalysis()); }
      else if(regex_match(match, subWhat, ssaldAnalysis))   { parentSubAnalyses.push_back(new LiveDeadMemAnalysis()); }
      else if(regex_match(match, subWhat, dpAnalysis))      { parentSubAnalyses.push_back(new DeadPathElimAnalysis(false)); }
      else if(regex_match(match, subWhat, ssadpAnalysis))   { parentSubAnalyses.push_back(new DeadPathElimAnalysis(true)); }
      else if(regex_match(match, subWhat, ccsAnalysis))  {
        parentSubAnalyses.push_back(new CallContextSensitivityAnalysis(1, CallContextSensitivityAnalysis::callSite, /*trackBase2RefinedPartEdgeMapping*/ true));
        
/*        list<ComposedAnalysis*> mySubAnalyses;
        composerType myComposerType = unknown;
        output_nested_results ons(tabs_ + 1, myComposerType, mySubAnalyses, NULL, NULL);
        std::for_each(
            subWhat.nested_results().begin(),
            subWhat.nested_results().end(),
            ons);*/
      }
      else if(regex_match(match, subWhat, ssaccsAnalysis))  { parentSubAnalyses.push_back(new CallContextSensitivityAnalysis(1, CallContextSensitivityAnalysis::callSite, /*trackBase2RefinedPartEdgeMapping*/ true)); }
      else if(regex_match(match, subWhat, ptAnalysis))      { parentSubAnalyses.push_back(new PointsToAnalysis(/*useSSA*/false)); }
      else if(regex_match(match, subWhat, ssaptAnalysis))   { parentSubAnalyses.push_back(new PointsToAnalysis(/*useSSA*/true)); }
      else if(regex_match(match, subWhat, vmAnalysis))      { parentSubAnalyses.push_back(new VirtualMethodAnalysis(/*useSSA*/false)); }
      else if(regex_match(match, subWhat, ssavmAnalysis))   { parentSubAnalyses.push_back(new VirtualMethodAnalysis(/*useSSA*/true)); }
      else if(regex_match(match, subWhat, dynMonitor))      { parentSubAnalyses.push_back(new DynamicMonitor()); }

      //else if(regex_match(match, subWhat, spcpAnalysis)) { parentSubAnalyses.push_back(new SparseConstantAnalysis()); }
      //else if(regex_match(match, subWhat, spvnAnalysis)) { parentSubAnalyses.push_back(new SparseValueNumbering()); }

    // Otherwise, if this is a composer, create the analyses in its sub-analysis list and then create the composer
    } else if(regex_match(match, subWhat, lcComposer)) {
      //std::fill_n( std::ostream_iterator<char_type>( std::cout ), tabs_ * 4, space_ch ); cout << "LOOSE SEQUENTIAL\n"<<endl;
      parentComposerType = looseSeq;
    } else if(regex_match(match, subWhat, lpComposer)) {
      //std::fill_n( std::ostream_iterator<char_type>( std::cout ), tabs_ * 4, space_ch ); cout << "LOOSE PARALLEL\n"<<endl;
      parentComposerType = loosePar;
    } else if(regex_match(match, subWhat, tComposer)) {
      parentComposerType = tight;
    // Finally, if this is a list of analyses for a given parent composer
    } else if(parentComposerType != unknown) {
      //cout << "other match="<<match<<endl;
      
      //assert(parentComposerType != unknown);
      list<ComposedAnalysis*> mySubAnalyses;
      composerType myComposerType = unknown;
      
      // Output any nested matches
      output_nested_results ons(tabs_ + 1, myComposerType, mySubAnalyses, NULL, NULL);
      std::for_each(
          what.nested_results().begin(),
          what.nested_results().end(),
          ons);
      // std::fill_n( std::ostream_iterator<char_type>( std::cout ), tabs_ * 4, space_ch );
      /*dbg << "#mySubAnalyses="<<mySubAnalyses.size()<<endl;
      for(list<ComposedAnalysis*>::iterator i=mySubAnalyses.begin(); i!=mySubAnalyses.end(); i++)
      { dbg << "    "<<(*i)->str()<<endl; }*/
      
      if(parentComposerType == looseSeq) {
//dbg << "ChainComposer"<<endl;
        ChainComposer* cc = new ChainComposer(mySubAnalyses, cdip, true);
        // Until ChainComposer is made to be a ComposedAnalysis, we cannot add it to the parentSubAnalyses list. This means that 
        // LooseComposer can only be used at the outer-most level of composition
        // !!!parentSubAnalyses.push_back(cc);
        if(parentComposer) *parentComposer = cc;
      } else if(parentComposerType == loosePar) {
//dbg << "LooseParallelComposer"<<endl;
        LooseParallelComposer* lp = new LooseParallelComposer(mySubAnalyses);
        parentSubAnalyses.push_back(lp);
        if(parentComposer) *parentComposer = lp;
      } else if(parentComposerType == tight) {
//dbg << "TightComposer parentComposer="<<parentComposer<<endl;
        TightComposer* t = new TightComposer(mySubAnalyses, /*trackBase2RefinedPartEdgeMapping*/ false, /*useSSA*/ false);
        parentSubAnalyses.push_back(t);
        if(parentComposer) *parentComposer = t;
      }
    } else {
      //dbg << "Other"<<endl;
      // Output any nested matches
      list<ComposedAnalysis*> mySubAnalyses;
      composerType myComposerType = unknown;
      output_nested_results ons(tabs_ + 1, myComposerType, parentSubAnalyses, NULL, NULL);
      //output_nested_results ons(tabs_ + 1, myComposerType, mySubAnalyses, NULL, NULL);
      //output_nested_results ons(tabs_ + 1, parentComposerType, parentSubAnalyses, parentComposer, NULL);
      std::for_each(
          what.nested_results().begin(),
          what.nested_results().end(),
          ons);
    }
  }
};

#include "midend/abstractLayer/VariableIdMapping.h"
#include "midend/abstractLayer/CPAstAttributeInterface.h"
#include "midend/abstractLayer/AstAnnotator.h"
#include "midend/abstractLayer/Labeler.h"
#include "stx_analysis.h"
VariableIdMapping vIDMap;

#define valAttrDebugLevel 0

class ValueASTAttribute: public CPAstAttributeInterface {
  protected:
  std::set<PartEdgePtr> refinedEdges;
  Composer* composer;
  checkDataflowInfoPass* cdip;
  public:
  typedef enum {above, below} dirT;
  dirT dir;
  std::string label;
  std::list<SgVariableSymbol *> allInScopeVars;
  CFGNode cn;

  // Returns a list of all the variables declared in the given scope
  std::list<SgVariableSymbol*> getAllVarSymbolsInScope(SgScopeStatement *scope) {
    list<SgVariableSymbol*> vars;
    SgVariableSymbol* var = scope->first_variable_symbol();
    while(var) {
      vars.push_back(var);
      var = scope->next_variable_symbol();
    }
    return vars;
  }

  // Returns a list of all the variables declared in the scopes that contain this node
  std::list<SgVariableSymbol*> getAllVarSymbols(SgNode *n) {
    n = n->get_parent();
    list<SgVariableSymbol*> allVars;
    while(n) {
      if(SgScopeStatement* scope = isSgScopeStatement(n)) {
        list<SgVariableSymbol*> scopeVars = getAllVarSymbolsInScope(scope);
        for(list<SgVariableSymbol*>::iterator v=scopeVars.begin(); v!=scopeVars.end(); v++) {
          allVars.push_back(*v);
        }
      }
      n = n->get_parent();
    }
    return allVars;
  }

  ValueASTAttribute(SgNode* n, Composer* composer, checkDataflowInfoPass* cdip, dirT dir, std::string label): 
        composer(composer), cdip(cdip), dir(dir), label(label) {
    SIGHT_VERB_DECL(scope, (txt()<<"ValueASTAttribute("<<SgNode2Str(n)<<")"), 1, valAttrDebugLevel)

    if(isSgFunctionDefinition(n)) return;
 
    // NOTE: this is a temporary hack where we assume the appropriate index for the CFGNode
    //       that represents SgNode n. In the future we should change Expr2* to accept CFGNodes
    if(isSgInitializedName(n)) cn = CFGNode(n, 1);
    else if(isSgBinaryOp(n))   cn = CFGNode(n, 2);
    else if(isSgUnaryOp(n)) {
      if(isSgCastExp(n))       cn = CFGNode(n, 0);
      else if(isSgAddressOfOp(n) || isSgPointerDerefExp(n) || isSgPlusPlusOp(n) || isSgMinusMinusOp(n)) cn = CFGNode(n, 1);
      else                     cn = CFGNode(n, 2);
    }
    else if(isSgValueExp(n))   cn = CFGNode(n, 1);
    else if(isSgFunctionCallExp(n)) cn = CFGNode(n, 2);
    else if(isSgWhileStmt(n))  cn = CFGNode(n, 1);
    else                       cn = CFGNode(n, 0);

 
    // Collect the PartEdges (computed by the given composer) that refine the incoming or
    // outgoing edges of the given SgNode
    //collectRefinedEdges(composer, refinedEdges, (dir==above? cn.inEdges(): cn.outEdges()));
    if(dir==above) collectIncomingRefinedEdges(composer, refinedEdges, cn);
    else           collectOutgoingRefinedEdges(composer, refinedEdges, cn);

    allInScopeVars = getAllVarSymbols(n);
    SIGHT_VERB_IF(1, valAttrDebugLevel)
      scope s("allInScopeVars");
      for(std::list<SgVariableSymbol *>::iterator v=allInScopeVars.begin(); v!=allInScopeVars.end(); ++v) {
        dbg << SgNode2Str(*v)<<endl;
      }
    SIGHT_VERB_FI()
  }
 
  // Apply Expr2Value for the given expression to all the edges in refinedEdges and return
  // the union of the resulting ValueObjects
  ValueObjectPtr Expr2Val(SgExpression* expr) { 
    ValueObjectPtr val;

    SIGHT_VERB(dbg << "Expr2Val("<<SgNode2Str(expr)<<") #refinedEdges="<<refinedEdges.size()<<endl, 1, valAttrDebugLevel)
    for(std::set<PartEdgePtr>::iterator r=refinedEdges.begin(); r!=refinedEdges.end(); r++) {
      SIGHT_VERB(dbg << "    edge="<<(*r)->str()<<endl, 1, valAttrDebugLevel)
      ValueObjectPtr edgeVal = composer->Expr2Val(expr, *r, cdip);
      if(val==NULLValueObject) val = edgeVal;
      else                     val->meetUpdate(edgeVal, *r, composer, NULL);
    }
    SIGHT_VERB(dbg << "Expr2Val returning val="<<val.get()<<endl, 1, valAttrDebugLevel)

    return val;
  }

  bool isConstantInteger(SgVarRefExp* ref) {
    SIGHT_VERB_DECL(scope, (txt()<<"isConstantInteger(ref="<<SgNode2Str(ref)<<")"), 1, valAttrDebugLevel)
    ValueObjectPtr val = Expr2Val(ref);
    SIGHT_VERB(dbg << "isConstantInteger() val="<<val.get()<<endl, 1, valAttrDebugLevel)
    if(!val) return false;
    SIGHT_VERB(dbg << "isConstantInteger() val="<<val->str()<<endl, 1, valAttrDebugLevel)
    if(val->isConcrete() && isStrictIntegerType(val->getConcreteType())) {
      std::set<boost::shared_ptr<SgValueExp> > cVals = val->getConcreteValue();
      if(cVals.size()==1) return true;
    }
    return false;
  }
  
  bool isConstantInteger(VariableId varId) {
    SIGHT_VERB_DECL(scope, (txt()<<"isConstantInteger(varID="<<SgNode2Str(vIDMap.getSymbol(varId))<<")"), 1, valAttrDebugLevel)
    ValueObjectPtr val = Expr2Val(buildVarRefExp(isSgVariableSymbol(vIDMap.getSymbol(varId))));
    SIGHT_VERB(dbg << "isConstantInteger() val="<<val.get()<<endl, 1, valAttrDebugLevel)
    if(!val) return false;
    SIGHT_VERB(dbg << "isConstantInteger() val(concrete="<<val->isConcrete()<<")="<<val->str()<<endl, 1, valAttrDebugLevel)
    if(val->isConcrete() && isStrictIntegerType(val->getConcreteType())) {
      std::set<boost::shared_ptr<SgValueExp> > cVals = val->getConcreteValue();
      if(cVals.size()==1) return true;
    }
    return false;
  }
  
  ConstantInteger getConstantInteger(SgVarRefExp* ref) {
    SIGHT_VERB(dbg << "getConstantInteger("<<SgNode2Str(ref)<<")"<<endl, 1, valAttrDebugLevel)
    ValueObjectPtr val = Expr2Val(ref);
    assert(val);
    if(val->isConcrete() && isStrictIntegerType(val->getConcreteType())) {
      SIGHT_VERB(dbg << "val="<<val->str()<<endl, 1, valAttrDebugLevel)
      std::set<boost::shared_ptr<SgValueExp> > cVals = val->getConcreteValue();
      if(cVals.size()==1) return getIntegerConstantValue((*cVals.begin()).get());
    }
    ROSE_ASSERT(0);
  }

  ConstantInteger getConstantInteger(VariableId varId) {
    ValueObjectPtr val = Expr2Val(buildVarRefExp(isSgVariableSymbol(vIDMap.getSymbol(varId))));
    if(val->isConcrete() && isStrictIntegerType(val->getConcreteType())) {
      std::set<boost::shared_ptr<SgValueExp> > cVals = val->getConcreteValue();
      if(cVals.size()==1) return getIntegerConstantValue((*cVals.begin()).get());
    }
    ROSE_ASSERT(0);
  }

  ~ValueASTAttribute() {}

  string toString() {
    if(cn.getNode()==NULL) return "";

    if(isSgExprStatement(cn.getNode())) {
      ValueASTAttribute exprLabel(isSgExprStatement(cn.getNode())->get_expression(), composer, cdip, dir, label);
      return exprLabel.toString();
    } else if(isSgReturnStmt(cn.getNode())) {
      ValueASTAttribute exprLabel(isSgReturnStmt(cn.getNode())->get_expression(), composer, cdip, dir, label);
      return exprLabel.toString();
    } else if(isSgIfStmt(cn.getNode())) {
      ValueASTAttribute exprLabel(isSgIfStmt(cn.getNode())->get_conditional(), composer, cdip, dir, label);
      return exprLabel.toString();
    } else if(isSgCastExp(cn.getNode())) {
      ValueASTAttribute exprLabel(isSgCastExp(cn.getNode())->get_operand(), composer, cdip, dir, label);
      return exprLabel.toString();
    } else if(isSgBasicBlock(cn.getNode())) {
      // Recursively call toString on the first statement in the block
      const SgStatementPtrList & stmt = isSgBasicBlock(cn.getNode())->get_statements();
      if(stmt.size()==0) return "[]";
      ValueASTAttribute exprLabel(*stmt.begin(), composer, cdip, dir, label);
      return exprLabel.toString();
    } else if(isSgFunctionDeclaration(cn.getNode()) || isSgFunctionDefinition(cn.getNode()) || isSgPragmaDeclaration(cn.getNode())) {
      return "[]";
    }
    
    ostringstream s;
    {
    SIGHT_VERB_DECL(scope, (txt()<<"ValueASTAttribute::toString("<<CFGNode2Str(cn)<<")"), 1, valAttrDebugLevel)
    s << "["<<label<<" : "<<SgNode2Str(cn.getNode())<<": ";
    //cout << CFGNode2Str(cn) << ": "<<endl;;
    int numConstants=0;
    for(list<SgVariableSymbol *>::iterator v=allInScopeVars.begin(); v!=allInScopeVars.end(); v++) {
      if(v!=allInScopeVars.begin()) { s << ", "; }

      SgVarRefExp* ref = buildVarRefExp(*v);
      SIGHT_VERB(dbg << "    "<<ref->unparseToString() << " = ", 1, valAttrDebugLevel)
      s << ref->unparseToString()<<"=";
      if(isConstantInteger(ref)) {
        //if(numConstants>0) { s << ", "; }
        s << getConstantInteger(ref);
        SIGHT_VERB(dbg << getConstantInteger(ref)<<endl, 1, valAttrDebugLevel)
        //s << getConstantInteger(ref);
        //cout << "        "<<ref->unparseToString() << "=" << getConstantInteger(ref)<<endl;;
        numConstants++;
      } else {
        SIGHT_VERB(dbg << "?"<<endl, 1, valAttrDebugLevel)
        s << "?";
      }
      
      delete ref;
    }
    s << "]";
    }
    //cout << endl;
    return s.str();
  }

  static void place(Composer* composer, checkDataflowInfoPass* cdip) {
    RoseAst ast(getProject());
    for(RoseAst::iterator i=ast.begin(); i!=ast.end();++i) {
      if(isSgExpression(*i)) {
        //(*i)->setAttribute("fuse_cp_above", new ValueASTAttribute(*i, composer, cdip, above, "fuse_cp_above"));
        (*i)->setAttribute("fuse_cp_below", new ValueASTAttribute(*i, composer, cdip, below, "fuse_cp_below"));
      }
    }
  }

  static void placeLabeler(Composer* composer, checkDataflowInfoPass* cdip, Labeler& labeler) {
    for(Labeler::iterator i=labeler.begin();i!=labeler.end();++i) {
      SgNode* node=labeler.getNode(*i);
      ROSE_ASSERT(node);
      //node->setAttribute("fuse_cp_above", new ValueASTAttribute(node, composer, cdip, above, "fuse_cp_above"));
      node->setAttribute("fuse_cp_below", new ValueASTAttribute(node, composer, cdip, below, "fuse_cp_below"));
    }
  }

  static void show(Composer* composer, VariableIdMapping& vIDMap) {
    RoseAst ast(getProject());
    for(RoseAst::iterator i=ast.begin(); i!=ast.end();++i) {
      /*if(SgExprStatement* stmt = isSgExprStatement(*i)) {
        cout << "i="<<SgNode2Str(*i)<<endl;
        ValueASTAttribute* above = (ValueASTAttribute*)stmt->get_expression()->getAttribute("fuse_cp_above");
        cout << "    above="<<above<<endl;
       // ValueASTAttribute* below = stmt->get_expression()->getAttribute("fuse_cp_aft");

        // Determine whether any of the variables in the current scope are constants above
        // this statement
        SgScopeStatement* scope = getEnclosingProcedure(stmt);
        cout << "    scope="<<SgNode2Str(scope)<<endl;
        SgVariableSymbol *cur = scope->first_variable_symbol();
        while(cur) {
          cout << "    cur="<<SgNode2Str(cur)<<", isConstantInteger="<<above->isConstantInteger(vIDMap.variableId(cur))<<endl;
          if(above->isConstantInteger(vIDMap.variableId(cur)))
            cout << "Variable "<<cur->unparseToString()<<"="<<above->getConstantInteger(vIDMap.variableId(cur))<<" at "<<SgNode2Str(stmt)<<endl;
          cur = scope->next_variable_symbol();
        }
      }*/
      if(SgVarRefExp* ref = isSgVarRefExp(*i)) {
        if(isStrictIntegerType(ref->get_type())) {
          //cout << "ref="<<SgNode2Str(ref)<<endl;
          ValueASTAttribute* above = (ValueASTAttribute*)(ref->getAttribute("fuse_cp_above"));
          //cout << "    above="<<above<<endl;
          //cout << "    isConstantInteger="<<above->isConstantInteger(ref)<<endl; cout.flush();
          if(above->isConstantInteger(ref))
            cout << SgNode2Str(ref)<<": Value="<<above->getConstantInteger(ref)<<endl;
        }
      }
    }
  }
}; // class ValueASTAttribute

template<typename T>
void printAttributes(Labeler* labeler, VariableIdMapping* vim, string attributeName) {
   long labelNum=labeler->numberOfLabels();
   for(long i=0;i<labelNum;++i) {
     Label lab=i;
     SgNode* node=labeler->getNode(i);
     cout<<"@Label "<<lab<<":";
     T* attr=dynamic_cast<T*>(node->getAttribute(attributeName));
     if(node)
       cout<<attr->toString()<<endl; // the attribute is casted to also allow to call other functions here
     else
       cout<<" none.";
     cout<<endl;
   }
}


int main(int argc, char** argv)
{
  FuseInit(argc, argv);
  
  printf("========== S T A R T ==========\n");
  
  Rose_STL_Container<string> args = CommandlineProcessing::generateArgListFromArgcArgv(argc, argv);
  // Strip the dataflow analysis options
  
  // Run the front end
  struct timeval start, end;
  gettimeofday(&start, NULL);

  SgProject* project = frontend(argc, argv);

  gettimeofday(&end, NULL);
  cout << "  Front End Elapsed="<<((end.tv_sec*1000000+end.tv_usec) -
                                               (start.tv_sec*1000000+start.tv_usec))/1000000.0<<"s"<<endl;

  //generatePDF(*project);

#if 0
  // DQ (6/1/2014): Added generation of graphs to support debugging.
     generateDOTforMultipleFile(*project);

  // Output an optional graph of the AST (the whole graph, of bounded complexity, when active)
     const int MAX_NUMBER_OF_IR_NODES_TO_GRAPH_FOR_WHOLE_GRAPH = 10000;
     generateAstGraph(project,MAX_NUMBER_OF_IR_NODES_TO_GRAPH_FOR_WHOLE_GRAPH,"");
#endif

  printf("Frontend done\n");fflush(stdout);
  
  string fuseCmd = "";
  Rose_STL_Container<string> dataflowoptions = CommandlineProcessing::generateOptionList(args, "-fuse:");
  //std::vector<std::string>  dataflowoptions = project->get_originalCommandLineArgumentList();
  if(dataflowoptions.size()>1) { cout << "ERROR: too many -dataflow options!"; exit(-1); }
  // If the caller provided a description of the composition structure on the command line, set fuseCmd to it
  else if(dataflowoptions.size()==1) {
    fuseCmd = dataflowoptions[0];
  // Otherwise, read it from the fuse pragmas in the source
  } else {
    Rose_STL_Container<SgNode*> pragmas = NodeQuery::querySubTree(project, V_SgPragma);
    for(Rose_STL_Container<SgNode*>::iterator p=pragmas.begin(); p!=pragmas.end(); p++) {
      SgPragma* pragma = isSgPragma(*p);
      assert(pragma);

      //cout << "pragma: "<< pragma->get_pragma() << endl;
      sregex pragmaLine = *_s >> as_xpr("fuse") >> *_s >> (s1=+~_n);
      smatch what;

      if(regex_match(pragma->get_pragma(), what, pragmaLine)) {
        assert(what.size()==2);
        fuseCmd.append(what[1]);
      }
    }
  }

  lcComposer = as_xpr(icase("loosechain")) | icase("lc");
  lpComposer = as_xpr(icase("loosepar"))   | icase("lp");
  tComposer  = as_xpr(icase("tight"))      | icase("t");
  composer = by_ref(lcComposer) | by_ref(lpComposer) | by_ref(tComposer);
  //composer = as_xpr(icase("loosechain")) | icase("lc") | icase("loosepar") | icase("lp");
  
  noAnalysis   = as_xpr(icase("noanalysis"))                        | icase("none");
  cpAnalysis   = as_xpr(icase("constantpropagationanalysis"))       | icase("constprop")   | icase("cp");
  ldAnalysis   = as_xpr(icase("livedeadmemanalysis"))               | icase("livedead")    | icase("ld");
  ccsAnalysis  = as_xpr(icase("callctxsensanalysis"))               | icase("callctxsens") | icase("ccs");
  dpAnalysis   = as_xpr(icase("deadpathelimanalysis"))              | icase("deadpath")    | icase("dp");
  ptAnalysis   = as_xpr(icase("pointstoanalysis"))                  | icase("pointsto")    | icase("pt");
  vmAnalysis   = as_xpr(icase("virtualmethodanalysis"))             | icase("virtualmem")  | icase("vm");
  spcpAnalysis = as_xpr(icase("sparseconstantpropagationanalysis")) | icase("spconstprop") | icase("spcp");
  spvnAnalysis = as_xpr(icase("sparsevaluenumberinganalysis"))      | icase("spvalnum")    | icase("spvn");
  ssacpAnalysis   = as_xpr(icase("SSA:constantpropagationanalysis"))       | icase("SSA:constprop")   | icase("SSA:cp");
  ssaldAnalysis   = as_xpr(icase("SSA:livedeadmemanalysis"))               | icase("SSA:livedead")    | icase("SSA:ld");
  ssaccsAnalysis  = as_xpr(icase("SSA:callctxsensanalysis"))               | icase("SSA:callctxsens") | icase("SSA:ccs");
  ssadpAnalysis   = as_xpr(icase("SSA:deadpathelimanalysis"))              | icase("SSA:deadpath")    | icase("SSA:dp");
  ssaptAnalysis   = as_xpr(icase("SSA:pointstoanalysis"))                  | icase("SSA:pointsto")    | icase("SSA:pt");
  ssavmAnalysis   = as_xpr(icase("SSA:virtualmethodanalysis"))             | icase("SSA:virtualmem")  | icase("SSA:vm");
  ssaspcpAnalysis = as_xpr(icase("SSA:sparseconstantpropagationanalysis")) | icase("SSA:spconstprop") | icase("SSA:spcp");
  ssaspvnAnalysis = as_xpr(icase("SSA:sparsevaluenumberinganalysis"))      | icase("SSA:spvalnum")    | icase("SSA:spvn");
  dynMonitor      = as_xpr(icase("dynamicmonitor"))                        | icase("dynmon")          | icase("dm");
  analysis = by_ref(noAnalysis)   |
             by_ref(cpAnalysis)   | by_ref(ldAnalysis) | by_ref(ccsAnalysis) |
             by_ref(dpAnalysis)   | by_ref(ptAnalysis) | by_ref(vmAnalysis)  |
             by_ref(spcpAnalysis) | by_ref(spvnAnalysis) |
             by_ref(ssacpAnalysis)   | by_ref(ssaldAnalysis) | by_ref(ssaccsAnalysis) |
             by_ref(ssadpAnalysis)   | by_ref(ssaptAnalysis) | by_ref(ssavmAnalysis)  |
             by_ref(ssaspcpAnalysis) | by_ref(ssaspvnAnalysis) |
             by_ref(dynMonitor);
  analysisList = '(' >> *_s >> (by_ref(analysis) | by_ref(compSpec)) >> *_s >> (!('(' >> *_s >>  +_w   >> *_s >> ')') ) >>
        *(*_s >> "," >> *_s >> (by_ref(analysis) | by_ref(compSpec)) >> *_s >> (!('(' >> *_s >>  +_w   >> *_s >> ')') ) ) >> *_s >> ')';
  compSpec = *_s >> by_ref(composer) >> *_s >> analysisList >> *_s;

/*
  // Remove leading spaces
  unsigned int startNonSpace=0;
  while(startNonSpace<fuseCmd.length() && (fuseCmd[startNonSpace]==' ' || fuseCmd[startNonSpace]=='\t')) startNonSpace++;
  if(startNonSpace>0) fuseCmd.erase(0, startNonSpace);

  // Remove trailing spaces
  unsigned int endNonSpace=fuseCmd.length()-1;
  while(endNonSpace>=0 && (fuseCmd[endNonSpace]==' ' || fuseCmd[endNonSpace]=='\t')) endNonSpace--;
  if(endNonSpace<fuseCmd.length()-1) fuseCmd.erase(endNonSpace+1);*/

  cout << "fuseCmd = \""<<fuseCmd<<"\"\n";
  
  // If this is a command for the compositional framework
  if(fuseCmd.size()>0) {
    smatch what;
    if(regex_match(fuseCmd, what, compSpec)) {
      //cout << "MATCH composer\n";
      list<ComposedAnalysis*>  mySubAnalyses;
      Composer* rootComposer = NULL;
      output_nested_results::composerType rootComposerType = output_nested_results::unknown;
      
      checkDataflowInfoPass* cdip = new checkDataflowInfoPass();
      
      output_nested_results ons(0, rootComposerType, mySubAnalyses, &rootComposer, cdip);
      std::for_each(what.nested_results().begin(),
                    what.nested_results().end(),
                    ons);
      assert(rootComposer!=NULL);
      
      struct timeval start, end;
      gettimeofday(&start, NULL);
      
      ((ChainComposer*)rootComposer)->runAnalysis();
      
/*      DynamicMonitor dynMon(rootComposer);
      dynMon.runAnalysis();*/

      gettimeofday(&end, NULL);
      cout << "Elapsed="<<((end.tv_sec*1000000+end.tv_usec) - 
                           (start.tv_sec*1000000+start.tv_usec))/1000000.0<<"s"<<endl;
  
      VariableIdMapping vIDMap;
      vIDMap.computeVariableSymbolMapping(getProject());
//      ValueASTAttribute::place(rootComposer, cdip);
//      ValueASTAttribute::show(rootComposer, vIDMap);
      Labeler labeler(getProject());
//      labeler.createLabels(getProject());
      ValueASTAttribute::placeLabeler(rootComposer, cdip, labeler);
      FuseRDAstAttribute::placeLabeler(rootComposer, cdip, vIDMap, labeler);

      AstAnnotator ara(&labeler);
      ara.annotateAstAttributesAsCommentsBeforeStatements(getProject(), "fuse_cp_below");
      ara.annotateAstAttributesAsCommentsBeforeStatements(getProject(), "fuse_rd");
//      printAttributes<ValueASTAttribute>(&labeler, &vIDMap, "fuse_cp_above");

/*      ValueASTAttribute::placeLabeler(rootComposer, cdip, labeler);
      AstAnnotator ara(&labeler);
      ara.annotateAstAttributesAsCommentsBeforeStatements(getProject(), "fuse_cp_below");
      FuseRDAstAttribute::placeLabeler(rootComposer, cdip, vIDMap, labeler);
      printAttributes<FuseRDAstAttribute>(&labeler, &vIDMap, "fuse_rd");*/
      
      //cout << "rootComposer="<<rootComposer<<" cdip->getNumErrors()="<<cdip->getNumErrors()<<endl;
      if(cdip) {
        if(cdip->getNumErrors() > 0) cout << cdip->getNumErrors() << " Errors Reported!"<<endl;
        else                         cout << "PASS"<<endl;
      }

    } else
      cout << "ERROR: cannot parse the fuse pragma\n";
  }
  
  printf("==========  E  N  D  ==========\n");
  
  return backend (project);
  //return 0;
}

