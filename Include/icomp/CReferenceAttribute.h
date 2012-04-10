#ifndef icomp_CReferenceAttribute_included
#define icomp_CReferenceAttribute_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include "icomp/TAttribute.h"


namespace icomp
{


/**
	Special attribute used to store referenced component ID.
*/
class CReferenceAttribute: public CIdAttribute
{
public:
	typedef CIdAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_SINGLE | IAttributeStaticInfo::AF_REFERENCE,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_SINGLE | IAttributeStaticInfo::AF_REFERENCE
	};

	CReferenceAttribute(){}
	CReferenceAttribute(const CReferenceAttribute& attribute):BaseClass(attribute){}
	explicit CReferenceAttribute(const QByteArray& value):BaseClass(value){}

	// reimplemented (iser::IObject)
	QByteArray GetFactoryId() const;

	// static methods
	static QByteArray GetTypeName();
};


} // namespace icomp


#endif // !icomp_CReferenceAttribute_included


