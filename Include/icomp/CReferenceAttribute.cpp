#include "icomp/CReferenceAttribute.h"


namespace icomp
{


// reimplemented (iser::IObject)

QByteArray CReferenceAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

QByteArray CReferenceAttribute::GetTypeName()
{
	return "Reference";
}


} // namespace icomp


