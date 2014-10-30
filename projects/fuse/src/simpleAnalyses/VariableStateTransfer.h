#ifndef _VARIABLESTATETRANSFER_H
#define _VARIABLESTATETRANSFER_H

#include "latticeFull.h"
#include "abstract_object_map.h"
#include "compose.h"
#include <vector>


namespace fuse {
template <class LatticeType, class AnalysisType>
class VariableStateTransfer : public DFTransferVisitor
{
  typedef boost::shared_ptr<LatticeType> LatticePtr;
//protected:
  public:
  bool modified;
  void updateModified(bool latModified) { modified = latModified || modified; }

  const debugLevel& dLevel;
  std::string dLevelStr;
  
  // A pointer to a default example lattice that can be duplicated
  // via defaultLat->copy() to make more instances of this Lattice type.
  LatticePtr defaultLat;
  Composer* composer;
  AnalysisType* analysis;

  AbstractObjectMap* prodLat;

  // Returns a Lattice object that corresponds to the memory location denoted by sgn in the current part
  virtual LatticePtr getLattice(SgExpression *sgn) {
    assert(sgn);
    // MemLocObjectPtrPair p = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MemLocObjectPtr p = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    if(dLevel()>=1) dbg << "VariableStateTransfer::getLattice() p="<<p->str("&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
    
    return getLattice(AbstractObjectPtr(p));
  }
  
  // Returns a Lattice object that corresponds to the memory location denoted by the given operand of sgn 
  // in the current part
  virtual LatticePtr getLatticeOperand(SgNode *sgn, SgExpression* operand) {
    scope s("VariableStateTransfer::getLatticeOperand()", scope::medium, attrGE(dLevelStr, 1));
    if(dLevel()>=1) {
      dbg << "sgn="<<SgNode2Str(sgn)<<endl;
      dbg << "operand="<<SgNode2Str(operand)<<endl;
    }
    
    assert(sgn);
    MemLocObjectPtr p = composer->OperandExpr2MemLoc(sgn, operand, part->inEdgeFromAny(), analysis);
    if(dLevel()>=1) dbg << "p="<<(p? p->str("&nbsp;&nbsp;&nbsp;&nbsp;"): "NULL")<<endl;
    return getLattice(p);
  }
    
  LatticePtr getLattice(const AbstractObjectPtr o) {
    LatticePtr l = boost::dynamic_pointer_cast<LatticeType>(prodLat->get(o));
    if(dLevel()>=1) dbg << "getLattice(o="<<o->strp(part->inEdgeFromAny(), "")<<", l="<<l->strp(part->inEdgeFromAny(), "")<<endl;
    assert(l);
    return l;
  }
  
  // Adds prodLat a mapping of the memory location denoted by sgn in the current part to lat. 
  // Returns true if this causes prodLat to change and false otherwise.
  void setLattice(SgNode *sgn, LatticePtr lat) {
    assert(sgn);
    // MemLocObjectPtrPair p = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MemLocObjectPtr p = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    if(dLevel()>=1) dbg << "setLattice() edge="<<part->inEdgeFromAny()->str()<<" p="<<p->strp(part->inEdgeFromAny(), "&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
    
    setLattice(p, lat);
  }
  
  // Adds prodLat a mapping of the memory location denoted by the given operand of node sgn in the current part to lat. 
  // Returns true if this causes prodLat to change and false otherwise.
  void setLatticeOperand(SgNode *sgn, SgExpression* operand, LatticePtr lat) {
    assert(sgn);
    // MemLocObjectPtrPair p = composer->OperandExpr2MemLoc(sgn, operand, part->inEdgeFromAny(), analysis);
    MemLocObjectPtr p = composer->OperandExpr2MemLoc(sgn, operand, part->inEdgeFromAny(), analysis);
    if(dLevel()>=1) dbg << "setLatticeOperand() p="<<p->strp(part->inEdgeFromAny(), "&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
    
    setLattice(p, lat);
  }
   
  void setLattice(const AbstractObjectPtr o, LatticePtr lat) {
    //if(dLevel()>=1) dbg << "setLattice(o="<<o->strp(part->inEdgeFromAny(), "")<<", lat="<<lat->strp(part->inEdgeFromAny(), "")<<endl;
    updateModified(prodLat->insert(o, lat));
    //if(dLevel()>=1) dbg << "&nbsp;&nbsp;&nbsp;prodLat="<<prodLat->str("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")<<endl;
  }

  bool getLattices(SgBinaryOp *sgn, LatticePtr &arg1Lat, LatticePtr &arg2Lat/*, LatticePtr &resLat*/) {
    arg1Lat = getLatticeOperand(sgn, sgn->get_lhs_operand());
    arg2Lat = getLatticeOperand(sgn, sgn->get_rhs_operand());
    /*resLat  = getLattice(sgn);*/

    //if(dLevel()>=1) dbg << "transfer B, resLat="<<resLat<<"\n";

    return (arg1Lat && arg2Lat/* && resLat*/);
  }
  
  bool getLattices(SgUnaryOp *sgn, LatticePtr &arg1Lat/*, LatticePtr &arg2Lat, LatticePtr &resLat*/) {
    arg1Lat = getLatticeOperand(sgn, sgn->get_operand());
    //resLat = getLattice(sgn);

    // Unary Update
    /*if(isSgMinusMinusOp(sgn) || isSgPlusPlusOp(sgn)) {
      // GB: This will not work for general lattices
      arg2Lat = (LatticePtr)(new LatticeType(1, part->inEdgeFromAny()));
    }*/
    //if(dLevel()>=1)  {
    //dbg << "res="<<res.str()<<" arg1="<<arg1.str()<<" arg1Lat="<<arg1Lat<<", arg2Lat="<<arg2Lat<<"\n";
    //dbg << "transfer B, resLat="<<resLat<<"\n";
    // }
    
    return (arg1Lat/* && arg2Lat && resLat*/);
  }

public:
  VariableStateTransfer(NodeState& state, std::map<PartEdgePtr, std::vector<Lattice*> >& dfInfo, 
                        // A pointer to a default example lattice that can be duplicated
                        // via defaultLat->copy() to make more instances of this Lattice type.
                        LatticePtr defaultLat,
                        Composer* composer, AnalysisType* analysis, PartPtr part, CFGNode cn, 
                        const debugLevel& dLevel, std::string dLevelStr) : 
    DFTransferVisitor(part, cn, state, dfInfo), 
    modified(false),
    dLevel(dLevel), dLevelStr(dLevelStr),
    defaultLat(defaultLat), 
    composer(composer), analysis(analysis)
  {
    //if(dLevel()>=1) dbg << "transfer A prodLat="<<prodLat<<"="<<prodLat->str("    ")<<"\n";
    // Make sure that all the lattices are initialized
    /*conVariableStateTransferst std::vector<Lattice*>& lattices = prodLat->getLattices();
    for(std::vector<Lattice*>::const_iterator it = lattices.begin(); it!=lattices.end(); it++)
      (dynamic_cast<LatticeType *>(*it))->initialize();*/
    assert(dfInfo.size()==1);
//    assert(dfInfo[NULLPartEdge].size()==2);
    //#SA: Incoming dfInfo is associated with inEdgeFromAny/outEdgeToAny
    PartEdgePtr wildCardPartEdge = ((ComposedAnalysis*)analysis)->getDirection()==ComposedAnalysis::fw? part->inEdgeFromAny() : part->outEdgeToAny();
    assert(dfInfo[wildCardPartEdge][0]);
    Lattice *l = dfInfo[wildCardPartEdge][0];
    prodLat = (dynamic_cast<AbstractObjectMap*>(l));
    assert(prodLat);
    
    // Adjust the edge of prodLat to correspond to the node's outgoing information 
    /*assert(prodLat->getPartEdge()->target());
    prodLat->setPartEdge(prodLat->getPartEdge()->target()->inEdgeFromAny());*/
  }

  void visit(SgAssignOp *sgn)
  {
    LatticePtr lhsLat, rhsLat;//, resLat;
    getLattices(sgn, lhsLat, rhsLat);//, resLat);
                
    if(dLevel()>=1)  {
      //dbg << "resLat=\n"; { indent ind; dbg << resLat->str("")<<"\n";}
      dbg << "lhsLat=\n"; { indent ind; dbg << lhsLat->str("")<<"\n";}
      dbg << "rhsLat=\n"; { indent ind; dbg << rhsLat->str("")<<"\n"; }
    }
    
    // Copy the lattice of the right-hand-side to both the left-hand-side variable and to the assignment expression itself
    // We only need to copy rhsLat once since it is a fresh object greated by prodLat->get()
    setLattice(sgn, rhsLat);
    setLatticeOperand(sgn, sgn->get_lhs_operand(), rhsLat);
    modified = true;
  }

  void visit(SgAssignInitializer *sgn)
  {
    LatticePtr asgnLat = getLatticeOperand(sgn, sgn->get_operand());
    LatticePtr resLat  = getLattice(sgn);
    
    if(dLevel()>=1) {
      dbg << "asgnLat="; { indent ind; dbg << asgnLat->str("")<<"\n"; }
      dbg << "resLat=";  { indent ind; dbg << resLat->str("") <<"\n"; }
    }

    setLattice(sgn, asgnLat); modified = true;
  }

  // XXX: Right now, we take the meet of all of the elements of the
  // initializer. This could be enhanced with an improved memory
  // abstraction to treat each element individually.
  void visit(SgAggregateInitializer *sgn)
  {
    LatticePtr res = getLattice(sgn);
    SgExpressionPtrList &inits = sgn->get_initializers()->get_expressions();
    if (inits.size() > 0) {
      LatticePtr initsCopy(dynamic_cast<LatticeType*>(getLattice(inits[0])->copy()));
      //res->copy(getLattice(inits[0]).get());
      modified = true;
      for (size_t i = 1; i < inits.size(); ++i)
        updateModified(initsCopy->meetUpdate(getLatticeOperand(sgn, inits[i]).get()));
        //res->meetUpdate(getLattice(inits[i]).get());
      setLattice(sgn, initsCopy);
    }
    else
        setLattice(sgn, res);
  }

  // XXX: This needs to be handled by an inter-procedural analysis
  // XXX: Right now, we take the meet of all of the elements of the
  // initializer. This could be enhanced with an improved memory
  // abstraction to treat each element individually.
  void visit(SgConstructorInitializer *sgn)
  {
    LatticePtr res = getLattice(sgn);
    SgExpressionPtrList &inits = sgn->get_args()->get_expressions();
    if (inits.size() > 0) {
      LatticePtr initsCopy(dynamic_cast<LatticeType*>(getLattice(inits[0])->copy()));
      //res->copy(getLattice(inits[0]).get());
      modified = true;
      for (size_t i = 1; i < inits.size(); ++i)
        updateModified(initsCopy->meetUpdate(getLatticeOperand(sgn, inits[i]).get()));
        //res->meetUpdate(getLattice(inits[i]).get());
      setLattice(sgn, initsCopy);
    }
    else
        setLattice(sgn, res);
  }

  // XXX: I don't even know what this is - Phil
  void visit(SgDesignatedInitializer *sgn)
  { }

  void visit(SgInitializedName *initName)
  {
    //if(dLevel()>=1) dbg << "visit(SgInitializedName *initName)"<<endl;
    indent ind(attrGE(dLevelStr, 1));
    LatticePtr initLat;
    if(initName->get_initializer()) {
      initLat = getLatticeOperand(initName, initName->get_initializer());
      if(dLevel()>=1) dbg << "initializer exists: "<<initLat->str("    ")<<"\n";
    // If there was no initializer, var's lattice is set to the default lattice 
    } else {
        boost::shared_ptr<Lattice> initLat2(defaultLat->copy());
      initLat = boost::dynamic_pointer_cast<LatticeType>(initLat2);
      initLat->setToEmpty();
     if(dLevel()>=1) dbg << "no initializer: "<<initLat->str("    ")<<"\n";
    }
    setLattice(initName, initLat);
    modified = true;
  }
  
/*  void visit(SgVariableDeclaration *decl)
  {
    if(dLevel()>=1) dbg << "visit(SgVariableDeclaration *decl)"<<endl;
    for(SgInitializedNamePtrList::iterator it=decl->get_variables().begin(); it!=decl->get_variables().end(); it++)
      visit(*it);
  }*/

  void visit(SgBinaryOp *sgn) {
    LatticePtr lhs, rhs;//, res;
    getLattices(sgn, lhs, rhs);//, res);
    LatticePtr lhsCopy(dynamic_cast<LatticeType*>(lhs->copy()));
    //res->copy(lhs.get());
    updateModified(lhsCopy->meetUpdate(rhs.get()));
    //res->meetUpdate(rhs.get());
    setLattice(sgn, lhsCopy);
    //setLattice(sgn, res);
    modified = true;
  }
  
  // These BinaryOps are no-ops because they don't update state and we don't
  // need to set their expression objects to their values since we can access
  // their values at their uses
  void visit(SgDotExp *sgn) { }
  void visit(SgArrowExp *sgn) { }
  
  // GB 2013-03-12: So we actually need this? It just gets a lattice at p and then sets it to itself!
  void visit(SgPntrArrRefExp *sgn) {
    if(dLevel()>=1) dbg << "<b>VariableStrateTransfer::visit(SgPntrArrRefExp *sgn)" << endl;
    // Copy data from the memory location identified by the array index expression to the
    // expression object of the SgPntrArrRefExp.
    // MemLocObjectPtrPair p = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    MemLocObjectPtr p = composer->Expr2MemLoc(sgn, part->inEdgeFromAny(), analysis);
    LatticePtr dataLat;
    // If this is a top-level array access expression
    // if(isSgPntrArrRefExp (sgn) && 
    //    (!isSgPntrArrRefExp (sgn->get_parent()) || !isSgPntrArrRefExp (isSgPntrArrRefExp (sgn->get_parent())->get_lhs_operand())))
    // {
    //   assert(p.mem);
    //   if(dLevel()>=1) dbg << "Getting "<<p.mem->str("")<<endl;
    //   dataLat = getLattice(AbstractObjectPtr(p.mem));
    // } else {
    //   if(dLevel()>=1) dbg << "Getting "<<p.expr->str("")<<endl;
    //   dataLat = getLattice(AbstractObjectPtr(p.expr));
    // }
    if(dLevel()>=1) dbg << "Getting p="<<p->str("")<<endl;
    dataLat = getLattice(AbstractObjectPtr(p));
    if(dLevel()>=1) {
      dbg << "Setting p="<<p->str("")<<endl;
      dbg << "to lat="<<dataLat->str("")<<endl;
    }
    setLattice(AbstractObjectPtr(p), dataLat);
    modified = true;
    if(dLevel()>=1) dbg << "</b>"<<endl;
  }

  /*void visit(SgCompoundAssignOp *sgn) {
    LatticePtr lhs, rhs;//, res;
    getLattices(sgn, lhs, rhs);//, res);
    
    LatticePtr lhsCopy(dynamic_cast<LatticeType*>(lhs->copy()));
    updateModified(lhsCopy->meetUpdate(rhs.get()));
    //updateModified(lhs->meetUpdate(rhs.get()));
    setLatticeOperand(sgn, sgn->get_lhs_operand(), lhsCopy);
    //setLattice(sgn->get_lhs_operand(), lhs);
    
    LatticePtr lhsCopy2(dynamic_cast<LatticeType*>(lhs->copy()));
    setLattice(sgn, lhsCopy2);
    //setLattice(sgn, lhs);
    modified = true;
  }*/

  void visit(SgCommaOpExp *sgn)
  {
    LatticePtr lhsLat, rhsLat;//, resLat;
    getLattices(sgn, lhsLat, rhsLat);//, resLat);
    
    setLattice(sgn, rhsLat);
    modified = true;
  }

  void visit(SgConditionalExp *sgn)
  {
    LatticePtr condLat  = getLatticeOperand(sgn, sgn->get_conditional_exp()),
               trueLat  = getLatticeOperand(sgn, sgn->get_true_exp()),
               falseLat = getLatticeOperand(sgn, sgn->get_false_exp());/*,
               resLat   = getLattice(sgn);*/
    
    LatticePtr condLatCopy(dynamic_cast<LatticeType*>(condLat->copy()));
    //resLat->copy(condLat.get());
    updateModified(condLatCopy->meetUpdate(trueLat.get()));
    //resLat->meetUpdate(trueLat.get());
    updateModified(condLatCopy->meetUpdate(falseLat.get()));
    //resLat->meetUpdate(falseLat.get());
    setLattice(sgn, condLatCopy);
    //setLattice(sgn, resLat);
    modified = true;
  }

  void visit(SgScopeOp *)
  {
    // Documentation says this is no longer used, so explicitly fail if we see it
    assert(0);
  }
};

}; //namespace fuse
#endif
