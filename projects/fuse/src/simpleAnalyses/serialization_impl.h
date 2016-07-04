#ifndef _SERIALIZATION_IMPL_H
#define _SERIALIZATION_IMPL_H

#include "mpi_comm_analysis.h"
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/set.hpp>

namespace fuse {

#define serializationDebugLevel 0
#if serializationDebugLevel==0
#define DISABLE_SIGHT
#endif

  /**********************
   * MPICommValueObject *
   **********************/
  template<class Archive>
  void save_construct_data(Archive& ar, const MPICommValueObject* mvo_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("save_construct_data(MPICommValueObject*)", scope::medium),
                    3, serializationDebugLevel)
    MPICommValueKindPtr kind = mvo_p->getKind();
    ar & kind;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, MPICommValueObject* mvo_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("load_construct_data(MPICommValueObject*)", scope::medium),
                    3, serializationDebugLevel)
    MPICommValueKindPtr kind;
    ar & kind;
    ::new(mvo_p) MPICommValueObject(kind, NULLPartEdge);
  }

  template<class Archive>
  void MPICommValueObject::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("MPICommValueObject::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & kind;

    SIGHT_VERB(dbg << "kind=" << kind->str() << ", addr=" << kind.get() << ", use_count=" << kind.use_count() << endl,
               3, serializationDebugLevel)
  }

  /********************
   * MPICommValueKind *
   ********************/
  template<class Archive>
  void MPICommValueKind::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("MPICommValueKind::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & kind;
  }

  /***************************
   * MPICommValueDefaultKind *
   ***************************/
  template<class Archive>
  void MPICommValueDefaultKind::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("MPICommValueDefaultKind::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & boost::serialization::base_object<MPICommValueKind>(*this);
  }

  /****************************
   * MPICommValueConcreteKind *
   ****************************/
  template<class Archive>
  void save_construct_data(Archive& ar, const MPICommValueConcreteKind* mck_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("save_construct_data(MPICommValueConcreteKind*)", scope::medium),
                    3, serializationDebugLevel)
    set<ConcreteValuePtr> cvals = mck_p->getConcreteValuePtrSet();
    ar & cvals;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, MPICommValueConcreteKind* mck_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("load_construct_data(MPICommValueConcreteKind*)", scope::medium),
                    3, serializationDebugLevel)
    set<ConcreteValuePtr> cvals;
    ar & cvals;
    ::new(mck_p) MPICommValueConcreteKind(cvals);
  }
  
  template<class Archive>
  void MPICommValueConcreteKind::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("MPICommValueConcreteKind::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & boost::serialization::base_object<MPICommValueKind>(*this);
    ar & concreteValues;
  }

  /***************************
   * MPICommValueUnknownKind *
   ***************************/
  template<class Archive>
  void MPICommValueUnknownKind::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("MPICommValueUnknownKind::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & boost::serialization::base_object<MPICommValueKind>(*this);
  }

  /*****************
   * ConcreteValue *
   *****************/
  template<class Archive>
  void ConcreteValue::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("ConcreteValue::serialize()", scope::medium),
                    3, serializationDebugLevel)
  }

  /************************
   * IntegerConcreteValue *
   ************************/
  template<class Archive>
  void save_construct_data(Archive& ar, const IntegerConcreteValue* icv_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("save_construct_data(IntegerConcreteValue*)", scope::medium),
                    3, serializationDebugLevel)
    int value = icv_p->get_value();
    ar & value;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, IntegerConcreteValue* icv_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("load_construct_data(IntegerConcreteValue*)", scope::medium),
                    3, serializationDebugLevel)
    int value;
    ar & value;
    ::new(icv_p) IntegerConcreteValue(value);
  }

  template<class Archive>
  void IntegerConcreteValue::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("IntegerConcreteValue::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & boost::serialization::base_object<ConcreteValue>(*this);
    int value = get_value();
    ar & value;
  }

  /************************
   * LongIntConcreteValue *
   ************************/
  template<class Archive>
  void save_construct_data(Archive& ar, const LongIntConcreteValue* icv_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("save_construct_data(LongIntConcreteValue*)", scope::medium),
                    3, serializationDebugLevel)
    long value = icv_p->get_value();
    ar & value;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, LongIntConcreteValue* icv_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("load_construct_data(LongIntConcreteValue*)", scope::medium),
                    3, serializationDebugLevel)
    long value;
    ar & value;
    ::new(icv_p) LongIntConcreteValue(value);
  }

  template<class Archive>
  void LongIntConcreteValue::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("LongIntConcreteValue::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & boost::serialization::base_object<ConcreteValue>(*this);
    long value = get_value();
    ar & value;
  }

  /****************************
   * LongLongIntConcreteValue *
   ****************************/
  template<class Archive>
  void save_construct_data(Archive& ar, const LongLongIntConcreteValue* icv_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("save_construct_data(LongLongIntConcreteValue*)", scope::medium),
                    3, serializationDebugLevel)
    long long value = icv_p->get_value();
    ar & value;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, LongLongIntConcreteValue* icv_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("load_construct_data(LongLongIntConcreteValue*)", scope::medium),
                    3, serializationDebugLevel)
    long long value;
    ar & value;
    ::new(icv_p) LongLongIntConcreteValue(value);
  }

  template<class Archive>
  void LongLongIntConcreteValue::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("LongLongIntConcreteValue::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & boost::serialization::base_object<ConcreteValue>(*this);
    long long value = get_value();
    ar & value;
  }

  /***********************
   * StringConcreteValue *
   ***********************/
  template<class Archive>
  void save_construct_data(Archive& ar, const StringConcreteValue* scv_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("save_construct_data(StringConcreteValue*)", scope::medium),
                    3, serializationDebugLevel)
    string value = scv_p->get_value();
    ar & value;
  }

  template<class Archive>
  void load_construct_data(Archive& ar, StringConcreteValue* scv_p, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("load_construct_data(StringConcreteValue*)", scope::medium),
                    3, serializationDebugLevel)
    string value;
    ar & value;
    ::new(scv_p) StringConcreteValue(value);
  }

  template<class Archive>
  void StringConcreteValue::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("StringConcreteValue::serialize()", scope::medium),
                    3, serializationDebugLevel)
    ar & boost::serialization::base_object<ConcreteValue>(*this);
    string value = get_value();
    ar & value;
  }

  /*****************
   * CompSharedPtr *
   *****************/
  template<class Type> template<class Archive>
  void CompSharedPtr<Type>::serialize(Archive& ar, const unsigned int version) {
    SIGHT_VERB_DECL(scope, ("CompSharedPtr::serialize", scope::medium),
                    3, serializationDebugLevel)
    ar & ptr;

    SIGHT_VERB(dbg << "ptr=" << ptr->str() << ", addr=" << ptr.get() << ", use_count=" << ptr.use_count() << endl,
               3, serializationDebugLevel)

  }
};
#endif
