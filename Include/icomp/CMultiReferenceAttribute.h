#ifndef icomp_CMultiReferenceAttribute_included
#define icomp_CMultiReferenceAttribute_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iattr/TMultiAttribute.h>
#include <icomp/IAttributeStaticInfo.h>


namespace icomp
{


/**
	Special attribute used to store list of referenced component ID's.
*/
class CMultiReferenceAttribute: public iattr::CIdListAttribute
{
public:
	typedef iattr::CIdListAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = iattr::IAttributeMetaInfo::AF_OBLIGATORY | iattr::IAttributeMetaInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_REFERENCE,
		DAF_OPTIONAL = iattr::IAttributeMetaInfo::AF_NULLABLE | iattr::IAttributeMetaInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_REFERENCE
	};

	CMultiReferenceAttribute();
	CMultiReferenceAttribute(const CMultiReferenceAttribute& attribute);
	CMultiReferenceAttribute(int elementsCount, QByteArray* valuesPtr);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// static methods
	static QByteArray GetTypeName();
};


} // namespace icomp


#endif // icomp_CMultiReferenceAttribute_included


