#include "icomp/CMultiFactoryAttribute.h"


namespace icomp
{


// reimplemented (iser::IObject)

QByteArray CMultiFactoryAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

QByteArray CMultiFactoryAttribute::GetTypeName()
{
	return "Factory[]";
}


} // namespace icomp


