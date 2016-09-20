#include <icomp/CMultiReferenceAttribute.h>


namespace icomp
{


// public methods

CMultiReferenceAttribute::CMultiReferenceAttribute()
{
}


CMultiReferenceAttribute::CMultiReferenceAttribute(const CMultiReferenceAttribute& attribute)
	:BaseClass(attribute)
{
}


CMultiReferenceAttribute::CMultiReferenceAttribute(int elementsCount, QByteArray* valuesPtr)
	:BaseClass(elementsCount, valuesPtr)
{
}


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


