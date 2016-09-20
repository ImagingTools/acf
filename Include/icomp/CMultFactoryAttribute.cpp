#include <icomp/CMultiFactoryAttribute.h>


namespace icomp
{


// public methods

CMultiFactoryAttribute::CMultiFactoryAttribute()
{
}


CMultiFactoryAttribute::CMultiFactoryAttribute(const CMultiFactoryAttribute& attribute)
	:BaseClass(attribute)
{
}


CMultiFactoryAttribute::CMultiFactoryAttribute(int elementsCount, QByteArray* valuesPtr)
	:BaseClass(elementsCount, valuesPtr)
{
}


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


