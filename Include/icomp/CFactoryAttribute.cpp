#include <icomp/CFactoryAttribute.h>


namespace icomp
{


// public methods

CFactoryAttribute::CFactoryAttribute()
	:BaseClass()
{
}


CFactoryAttribute::CFactoryAttribute(const CFactoryAttribute& attribute)
	:BaseClass(attribute)

{
}


CFactoryAttribute::CFactoryAttribute(const QByteArray& value)
	:BaseClass(value)
{
}


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


