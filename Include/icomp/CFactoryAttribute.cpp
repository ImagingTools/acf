#include "icomp/CFactoryAttribute.h"


namespace icomp
{


// reimplemented (iser::IObject)

QByteArray CFactoryAttribute::GetFactoryId() const
{
	return GetTypeName();
}


// static methods

QByteArray CFactoryAttribute::GetTypeName()
{
	return "Factory";
}


} // namespace icomp


