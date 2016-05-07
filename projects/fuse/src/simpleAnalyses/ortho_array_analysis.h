#ifndef _ORTHO_ARRAY_ANALYSIS_H
#define _ORTHO_ARRAY_ANALYSIS_H

#include "compose.h"

namespace fuse {
  class ArrayAnalysis;

  // Memory object wrapping the information about array
  class ArrayML : public MemLocObject
  {
  protected:
    // The parent array reference
    SgNode *array_ref;
    // memory object for the top level array object
    MemRegionObjectPtr region;
    // list of value objects for the subscripts
    IntersectValueObjectPtr iv;
    
    // to represent other memory objects that are not array
    MemLocObjectPtr nonArrayML;
        
    // useful to distinguish this object from other memory objects
    typedef enum {empty, array, notarray, full} MType;
    MType mtype;
    
  public:
    ArrayML(SgNode* sgn, MemRegionObjectPtr arrayML, IntersectValueObjectPtr iv);
    ArrayML(SgNode* sgn, MemLocObjectPtr nonarrayML);
    ArrayML(const ArrayML& that);

    MType getMType() const;
    MemLocObjectPtr getNonArrayML() const;
    MemRegionObjectPtr getArrayMR() const;
    IntersectValueObjectPtr getArrayIndexValue() const; 

    // Copy this object and return a pointer to it
    MemLocObjectPtr copyML() const;

    bool mayEqualML(MemLocObjectPtr that, PartEdgePtr pedge);
    bool mustEqualML(MemLocObjectPtr that, PartEdgePtr pedge);
    
    // Returns whether the two abstract objects denote the same set of concrete objects
    bool equalSetML(MemLocObjectPtr that, PartEdgePtr pedge);
    
    // Returns whether this abstract object denotes a non-strict subset (the sets may be equal) of the set denoted
    // by the given abstract object.
    bool subSetML(MemLocObjectPtr that, PartEdgePtr pedge);
    
    bool isLiveML(PartEdgePtr pedge);
    
    // Computes the meet of this and that and saves the result in this
    // returns true if this causes this to change and false otherwise
    bool meetUpdateML(MemLocObjectPtr that, PartEdgePtr pedge);
    
    // Returns whether this AbstractObject denotes the set of all possible execution prefixes.
    bool isFullML(PartEdgePtr pedge);
    // Returns whether this AbstractObject denotes the empty set.
    bool isEmptyML(PartEdgePtr pedge);
    
    // Set this object to represent the set of all possible MemLocs
    // Return true if this causes the object to change and false otherwise.
    bool setToFull();
    // Set this Lattice object to represent the empty set of MemLocs.
    // Return true if this causes the object to change and false otherwise.
    bool setToEmpty();

    // pretty print
    std::string str(std::string indent="") const;
  };
  typedef boost::shared_ptr<ArrayML> ArrayMLPtr;  

  class ArrayAnalysis : public UndirDataflow {

    // Returns a shared pointer to a freshly-allocated copy of this ComposedAnalysis object
    ComposedAnalysisPtr copy() { return boost::make_shared<ArrayAnalysis>(); }
    
    // The genInitLattice, genInitFact and transfer functions are not implemented since this 
    // is not a dataflow analysis.
   
    // Maps the given SgNode to an implementation of the MemLocObject abstraction.
    // Variant of Expr2Val where Part field is ignored since it makes no difference for the syntactic analysis.
    MemLocObjectPtr  Expr2MemLoc(SgNode* n, PartEdgePtr pedge);
    bool implementsExpr2MemLoc() { return true; }
  
    // pretty print for the object
    std::string str(std::string indent="") const
    { return "ArrayAnalysis"; }
  };
}; // end namespace
#endif
