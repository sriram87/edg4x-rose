#ifndef _SERIALIZATION_IMPL_H
#define _SERIALIZATION_IMPL_H

#include "mpi_comm_analysis.h"
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/set.hpp>

namespace fuse {

  DEBUG_LEVEL(serializationDebugLevel, 1);

  /**********************
   * MPICommValueObject *
   **********************/
  template<class Archive>
  void save_construct_data(Archive& ar, const MPICommValueObject* mvo_p, const unsigned int version) {
    scope reg("save_construct_data(MPICommValueObject*)", scope::medium, attrGE("serializationDebugLevel", 3));
    MPICommValueKindPtr kind = mvo_p->getKind();
    ar & kind;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, MPICommValueObject* mvo_p, const unsigned int version) {
    scope reg("load_construct_data(MPICommValueObject*)", scope::medium, attrGE("serializationDebugLevel", 3));
    MPICommValueKindPtr kind;
    ar & kind;
    ::new(mvo_p) MPICommValueObject(kind, NULLPartEdge);
  }

  template<class Archive>
  void MPICommValueObject::serialize(Archive& ar, const unsigned int version) {
    scope reg("MPICommValueObject::serialize()", scope::medium, attrGE("serializationDebugLevel", 3));
    ar & kind;
    if(serializationDebugLevel() >= 3) {
      dbg << "kind=" << kind->str() << ", addr=" << kind.get() << ", use_count=" << kind.use_count() << endl;
    }
  }

  /********************
   * MPICommValueKind *
   ********************/
  template<class Archive>
  void MPICommValueKind::serialize(Archive& ar, const unsigned int version) {
    scope reg("MPICommValueKind::serialize()", scope::medium, attrGE("serializationDebugLevel", 3));
    ar & kind;
  }

  /***************************
   * MPICommValueDefaultKind *
   ***************************/
  template<class Archive>
  void MPICommValueDefaultKind::serialize(Archive& ar, const unsigned int version) {
    scope reg("MPICommValueDefaultKind::serialize()", scope::medium, attrGE("serializationDebugLevel", 3));
    ar & boost::serialization::base_object<MPICommValueKind>(*this);
  }

  /****************************
   * MPICommValueConcreteKind *
   ****************************/
  template<class Archive>
  void save_construct_data(Archive& ar, const MPICommValueConcreteKind* mck_p, const unsigned int version) {
    scope reg("save_construct_data(MPICommValueConcreteKind*)", scope::medium, attrGE("serializationDebugLevel", 3));
    set<ConcreteValuePtr> cvals = mck_p->getConcreteValuePtrSet();
    ar & cvals;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, MPICommValueConcreteKind* mck_p, const unsigned int version) {
    scope reg("load_construct_data(MPICommValueConcreteKind*)", scope::medium, attrGE("serializationDebugLevel", 3));
    set<ConcreteValuePtr> cvals;
    ar & cvals;
    ::new(mck_p) MPICommValueConcreteKind(cvals);
  }
  
  template<class Archive>
  void MPICommValueConcreteKind::serialize(Archive& ar, const unsigned int version) {
    scope reg("MPICommValueConcreteKind::serialize()", scope::medium, attrGE("serializationDebugLevel", 3));
    ar & boost::serialization::base_object<MPICommValueKind>(*this);
    ar & concreteValues;
  }

  /***************************
   * MPICommValueUnknownKind *
   ***************************/
  template<class Archive>
  void MPICommValueUnknownKind::serialize(Archive& ar, const unsigned int version) {
    scope reg("MPICommValueUnknownKind::serialize()", scope::medium, attrGE("serializationDebugLevel", 3));
    ar & boost::serialization::base_object<MPICommValueKind>(*this);
  }

  template<class Archive>
  void ConcreteValue::serialize(Archive& ar, const unsigned int version) {
    scope reg("ConcreteValue::serialize()", scope::medium, attrGE("serializationDebugLevel", 3));
  }

  template<class Archive>
  void save_construct_data(Archive& ar, const IntegerConcreteValue* icv_p, const unsigned int version) {
    scope reg("save_construct_data(IntegerConcreteValue*)", scope::medium, attrGE("serializationDebugLevel", 3));
    int value = icv_p->get_value();
    ar & value;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, IntegerConcreteValue* icv_p, const unsigned int version) {
    scope reg("load_construct_data(IntegerConcreteValue*)", scope::medium, attrGE("serializationDebugLevel", 3));
    int value;
    ar & value;
    ::new(icv_p) IntegerConcreteValue(value);
  }

  template<class Archive>
  void IntegerConcreteValue::serialize(Archive& ar, const unsigned int version) {
    scope reg("IntegerConcreteValue::serialize()", scope::medium, attrGE("serializationDebugLevel", 3));
    ar & boost::serialization::base_object<ConcreteValue>(*this);
    int value = get_value();
    ar & value;
  }

  template<class Type> template<class Archive>
  void CompSharedPtr<Type>::serialize(Archive& ar, const unsigned int version) {
    scope reg("CompSharedPtr::serialize", scope::medium, attrGE("serializationDebugLevel", 3));
    ar & ptr;
    if(serializationDebugLevel() >= 3) {
      dbg << "ptr=" << ptr->str() << ", addr=" << ptr.get() << ", use_count=" << ptr.use_count() << endl;
    }
  }
};
#endif
