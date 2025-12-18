#ifndef icomp_CFactoryAttribute_included
#define icomp_CFactoryAttribute_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <icomp/CReferenceAttribute.h>


namespace icomp
{


/**
	Special attribute used to store referenced component ID.
*/
class CFactoryAttribute: public CReferenceAttribute
{
public:
	typedef CReferenceAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_SINGLE | IAttributeStaticInfo::AF_FACTORY,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_SINGLE | IAttributeStaticInfo::AF_FACTORY
	};

	CFactoryAttribute();
	CFactoryAttribute(const CFactoryAttribute& attribute);
	explicit CFactoryAttribute(const QByteArray& value);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// static methods
	static QByteArray GetTypeName();
};


} // namespace icomp


#endif // !icomp_CFactoryAttribute_included


