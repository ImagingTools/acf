#ifndef icomp_CReferenceAttribute_included
#define icomp_CReferenceAttribute_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iattr/TAttribute.h>
#include <icomp/IAttributeStaticInfo.h>


namespace icomp
{


/**
	Special attribute used to store referenced component ID.
*/
class CReferenceAttribute: public iattr::CIdAttribute
{
public:
	typedef iattr::CIdAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = iattr::IAttributeMetaInfo::AF_OBLIGATORY | iattr::IAttributeMetaInfo::AF_SINGLE | IAttributeStaticInfo::AF_REFERENCE,
		DAF_OPTIONAL = iattr::IAttributeMetaInfo::AF_NULLABLE | iattr::IAttributeMetaInfo::AF_SINGLE | IAttributeStaticInfo::AF_REFERENCE
	};

	CReferenceAttribute();
	CReferenceAttribute(const CReferenceAttribute& attribute);
	explicit CReferenceAttribute(const QByteArray& value);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// static methods
	static QByteArray GetTypeName();
};


} // namespace icomp


#endif // !icomp_CReferenceAttribute_included


