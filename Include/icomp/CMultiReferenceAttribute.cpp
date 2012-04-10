#include "icomp/CMultiReferenceAttribute.h"


namespace icomp
{


// reimplemented (iser::IObject)

QByteArray CMultiReferenceAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

QByteArray CMultiReferenceAttribute::GetTypeName()
{
	return "Reference[]";
}


} // namespace icomp


