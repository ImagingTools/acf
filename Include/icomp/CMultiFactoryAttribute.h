#ifndef icomp_CMultiFactoryAttribute_included
#define icomp_CMultiFactoryAttribute_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <icomp/CMultiReferenceAttribute.h>


namespace icomp
{


/**
	Special attribute used to store list of referenced component ID's.
*/
class CMultiFactoryAttribute: public CMultiReferenceAttribute
{
public:
	typedef CMultiReferenceAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_FACTORY,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_MULTIPLE | IAttributeStaticInfo::AF_FACTORY
	};

	CMultiFactoryAttribute();
	CMultiFactoryAttribute(const CMultiFactoryAttribute& attribute);
	CMultiFactoryAttribute(int elementsCount, QByteArray* valuesPtr);

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// static methods
	static QByteArray GetTypeName();
};


} // namespace icomp


#endif // icomp_CMultiFactoryAttribute_included


