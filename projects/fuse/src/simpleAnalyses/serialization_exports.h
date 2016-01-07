#ifndef _SERIALIZATION_EXPORTS_H
#define _SERIALIZATION_EXPORTS_H

#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_GUID(fuse::MPICommValueKind, "mpicommvaluekind")
BOOST_CLASS_EXPORT_GUID(fuse::MPICommValueDefaultKind, "mpicommvaluedefaultkind")
BOOST_CLASS_EXPORT_GUID(fuse::ConcreteValue, "concretevalue")
BOOST_CLASS_EXPORT_GUID(fuse::IntegerConcreteValue, "integerconcretevalue")
BOOST_CLASS_EXPORT_GUID(fuse::LongIntConcreteValue, "longintconcretevalue")
BOOST_CLASS_EXPORT_GUID(fuse::LongLongIntConcreteValue, "longlongintconcretevalue")
BOOST_CLASS_EXPORT_GUID(fuse::MPICommValueConcreteKind, "mpicommvalueconcretekind")
BOOST_CLASS_EXPORT_GUID(fuse::MPICommValueUnknownKind, "mpicommvalueunknownkind")

BOOST_CLASS_EXPORT_GUID(fuse::Lattice, "lattice")
BOOST_CLASS_EXPORT_GUID(fuse::FiniteLattice, "finitelattice")
BOOST_CLASS_EXPORT_GUID(fuse::AbstractObject, "abstractobject")
BOOST_CLASS_EXPORT_GUID(fuse::ValueObject, "valueobject")
BOOST_CLASS_EXPORT_GUID(fuse::MPICommValueObject, "mpicommvalueobject")
BOOST_CLASS_EXPORT_GUID(fuse::CompSharedPtr<fuse::ConcreteValue>, "compsharedptrconcretevalue")
#endif
