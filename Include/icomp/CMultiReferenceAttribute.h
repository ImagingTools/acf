#ifndef icomp_CMultiReferenceAttribute_included
#define icomp_CMultiReferenceAttribute_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include "icomp/TMultiAttribute.h"


namespace icomp
{


/**
	Special attribute used to store list of referenced component ID's.
*/
class CMultiReferenceAttribute: public CIdListAttribute
{
public:
	typedef CIdListAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_REFERENCE,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_REFERENCE
	};

	CMultiReferenceAttribute(){}
	CMultiReferenceAttribute(const CMultiReferenceAttribute& attribute):BaseClass(attribute){}
	CMultiReferenceAttribute(int elementsCount, QByteArray* valuesPtr):BaseClass(elementsCount, valuesPtr){}

	// reimplemented (iser::IObject)
	QByteArray GetFactoryId() const;

	// static methods
	static QByteArray GetTypeName();
};


} // namespace icomp


#endif // icomp_CMultiReferenceAttribute_included


