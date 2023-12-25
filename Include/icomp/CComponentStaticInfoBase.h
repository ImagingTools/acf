#ifndef icomp_CComponentStaticInfoBase_included
#define icomp_CComponentStaticInfoBase_included


// STL includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// ACF includes
#include <icomp/IComponentStaticInfo.h>


namespace icomp
{


class CComponentStaticInfoBase: virtual public IComponentStaticInfo
{
public:
	// reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const override;
	virtual const IAttributeStaticInfo* GetAttributeInfo(const QByteArray& attributeId) const override;
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const QByteArray& embeddedId) const override;
	virtual const QString& GetDescription() const override;
	virtual const QString& GetKeywords() const override;

	//	reimplemented (icomp::IElementStaticInfo)
	virtual Ids GetMetaIds(int metaGroupId) const override;
	virtual const IElementStaticInfo* GetSubelementInfo(const QByteArray& subcomponentId) const override;

	//	reimplemented (iattr::IAttributesMetaInfoProvider)
	virtual iattr::IAttributesProvider::AttributeIds GetAttributeMetaIds() const override;
	virtual const iattr::IAttributeMetaInfo* GetAttributeMetaInfo(const QByteArray& attributeId) const override;
};


} // namespace icomp


#endif // !icomp_CComponentStaticInfoBase_included


