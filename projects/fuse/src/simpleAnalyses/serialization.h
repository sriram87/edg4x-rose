#ifndef _SERIALIZATION_H
#define _SERIALIZATION_H

/*****************************************
 * Author: Sriram Aananthakrishnan, 2015 *
 *****************************************/

#include "compose.h"
#include "mpi_comm_analysis.h"
#include <boost/serialization/set.hpp>
#include <boost/serialization/base_object.hpp>

// BOOST Serialization methods for Object serialization
// Non-intrusive functions for serializing FUSE objects

namespace boost {
  namespace serialization {

    /***********
     * Lattice *
     ***********/
    template<class Archive>
    void save_construct_data(Archive& ar, const fuse::Lattice* lat_p, const unsigned int version) {
      // nothing to save
    }

    template<class Archive>
    void load_construct_data(Archive& ar, fuse::Lattice* lat_p, const unsigned int version) {
      //::new(lat_p)fuse::Lattice(fuse::NULLPartEdge);
    }

    template<class Archive>
    void serialize(Archive& ar, fuse::Lattice& lat, const unsigned int version) {
    }

    /*****************
     * FiniteLattice *
     *****************/
    template<class Archive>
    void save_construct_data(Archive& ar, const fuse::FiniteLattice* lat_p, const unsigned int version) {
      // nothing to save
    }

    template<class Archive>
    void load_construct_data(Archive& ar, fuse::FiniteLattice* lat_p, const unsigned int version) {
      //::new(lat_p)fuse::FiniteLattice(fuse::NULLPartEdge);
    }

    template<class Archive>
    void serialize(Archive& ar, fuse::FiniteLattice& lat, const unsigned int version) {
      boost::serialization::base_object<fuse::Lattice>(lat);
      // nothing to save
    }

    /******************
     * AbstractObject *
     ******************/
    template<class Archive>
    void serialize(Archive& ar, fuse::AbstractObject& ao, const unsigned int version) {
      // nothing to save
    }


    /***************
     * ValueObject *
     ***************/
    template<class Archive>
    void serialize(Archive& ar, fuse::ValueObject& vo, const unsigned int version) {
      boost::serialization::base_object<fuse::AbstractObject>(vo);
      // nothing to save
    }


    /**********************
     * MPICommValueObject *
     **********************/
    template<class Archive>
    void save_construct_data(Archive& ar, const fuse::MPICommValueObject* mvo_p, const unsigned int version) {
      fuse::MPICommValueKind* kind = mvo_p->getKind().get();
      ar << kind;
    }

    template<class Archive>
    void load_construct_data(Archive& ar, fuse::MPICommValueObject* mvo_p, const unsigned int version) {
      fuse::MPICommValueKind* kind;
      ar >> kind;
      fuse::MPICommValueKindPtr kind_p = boost::shared_ptr<fuse::MPICommValueKind>(kind);
      ::new(mvo_p)fuse::MPICommValueObject(fuse::NULLPartEdge, kind_p);
    }

    template<class Archive>
    void serialize(Archive& ar, fuse::MPICommValueObject& mvo, const unsigned int version) {
      // boost::serialization::base_object<fuse::Lattice>(mvo);
      // boost::serialization::base_object<fuse::FiniteLattice>(mvo);
      // boost::serialization::base_object<fuse::ValueObject>(mvo);
      fuse::MPICommValueKind* kind_p = mvo.getKind().get();
      ar & kind_p;
    }

    /********************
     * MPICommValueKind *
     ********************/
    template<class Archive>
    void save_construct_data(Archive& ar, fuse::MPICommValueKind* kind_p, const unsigned int version) {
      fuse::MPICommValueKind::ValueKind k = kind_p->getKindType();
      ar << k;
    }

    template<class Archive>
    void load_construct_data(Archive& ar, fuse::MPICommValueKind* kind_p, const unsigned int version) {
      fuse::MPICommValueKind::ValueKind k;
      ar >> k;
      //::new(kind_p)fuse::MPICommValueKind(k);
    }

    template<class Archive>
    void serialize(Archive& ar, fuse::MPICommValueKind& kind, const unsigned int version) {
      fuse::MPICommValueKind::ValueKind k = kind.getKindType();
      ar & k;      
    }

    /***************************
     * MPICommValueDefaultKind *
     ***************************/
    template<class Archive>
    void serialize(Archive& ar, fuse::MPICommValueDefaultKind& kind, const unsigned int version) {
      boost::serialization::base_object<fuse::MPICommValueKind>(kind);
      // nothing to save otherwise
    }

    /*****************
     * ConcreteValue *
     *****************/
    template<class Archive>
    void serialize(Archive& ar, fuse::ConcreteValue& cval, const unsigned int version) {
      // nothing to save
    }

    /************************
     * IntegerConcreteValue *
     ************************/
    template<class Archive>
    void save_construct_data(Archive& ar, const fuse::IntegerConcreteValue* ival_p, const unsigned int version) {
      int value = ival_p->get_value();
      ar << value;
    }

    template<class Archive>
    void load_construct_data(Archive& ar, fuse::IntegerConcreteValue* ival_p, const unsigned int version) {
      int value;
      ar >> value;
      ::new(ival_p)fuse::IntegerConcreteValue(value);
    }

    template<class Archive>
    void serialize(Archive& ar, fuse::IntegerConcreteValue& ival, const unsigned int version) {
      boost::serialization::base_object<fuse::ConcreteValue>(ival);
      int value = ival.get_value();
      ar & value;
    }

    /****************************
     * MPICommValueConcreteKind *
     ****************************/
    template<class Archive>
    void save_construct_data(Archive& ar, const fuse::MPICommValueConcreteKind* mvo_p, const unsigned int version) {
      std::set<fuse::ConcreteValuePtr> cvals = mvo_p->getConcreteValuePtrSet();
      ar << cvals;
    }

    template<class Archive>
    void load_construct_data(Archive& ar, fuse::MPICommValueConcreteKind* mvo_p, const unsigned int version) {
      std::set<fuse::ConcreteValuePtr> cvals;
      ar >> cvals;
      ::new(mvo_p)fuse::MPICommValueConcreteKind(cvals);
    }

    template<class Archive>
    void serialize(Archive& ar, fuse::MPICommValueConcreteKind& kind, const unsigned int version) {
      boost::serialization::base_object<fuse::MPICommValueKind>(kind);
      std::set<fuse::ConcreteValuePtr> cvals = kind.getConcreteValuePtrSet();
      ar & cvals;
    }

    /***************************
     * MPICommValueUnknownKind *
     ***************************/
    template<class Archive>
    void serialize(Archive& ar, fuse::MPICommValueUnknownKind& kind, const unsigned int version) {
      boost::serialization::base_object<fuse::MPICommValueKind>(kind);
    }

  } // end namespace serialization
} // end namespace boost

#endif
