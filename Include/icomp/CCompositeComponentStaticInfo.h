#ifndef icomp_CCompositeComponentStaticInfo_included
#define icomp_CCompositeComponentStaticInfo_included


// STL includes
#include <QtCore/QMap>


// ACF includes
#include "istd/TDelPtr.h"

#include "icomp/IAttributeStaticInfo.h"
#include "icomp/IRegistry.h"
#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


class IComponentEnvironmentManager;
class IRegistriesManager;


class CCompositeComponentStaticInfo: public CBaseComponentStaticInfo
{
public:
	CCompositeComponentStaticInfo(
				const IRegistry& registry,
				const icomp::IComponentEnvironmentManager& manager,
				const IComponentStaticInfo* parentPtr = NULL);

	// reimplemented (icomp::IRealComponentStaticInfo)
	virtual IComponent* CreateComponent() const;

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const;
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const std::string& embeddedId) const;
	virtual const QString& GetDescription() const;
	virtual const QString& GetKeywords() const;

protected:
	/**
		Get the element info for the given element ID. Method works recurive for the complex ID's.
	*/
	const IRegistry::ElementInfo* GetElementInfoFromRegistry(
				const IRegistry& registry,
				const std::string& elementId,
				const icomp::IRegistriesManager& manager) const;

	class AttrAsOptionalDelegator: virtual public IAttributeStaticInfo
	{
	public:
		AttrAsOptionalDelegator(
					const IAttributeStaticInfo* slavePtr,
					const iser::IObject* defaultValuePtr);

		virtual const std::string& GetAttributeDescription() const;
		virtual const iser::IObject* GetAttributeDefaultValue() const;
		virtual std::string GetAttributeTypeName() const;
		virtual IElementStaticInfo::Ids GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const;
		virtual int GetAttributeFlags() const;

	private:
		const IAttributeStaticInfo& m_slave;
		const iser::IObject* m_defaultValuePtr;
	};

private:
	typedef istd::TDelPtr<IAttributeStaticInfo> AttrMetaInfoPtr;
	typedef QMap<const IAttributeStaticInfo*, AttrMetaInfoPtr> AttrReplacers;
	AttrReplacers m_attrReplacers;

	typedef istd::TDelPtr<icomp::CCompositeComponentStaticInfo> ComponentInfoPtr;
	typedef QMap<std::string, ComponentInfoPtr> EmbeddedComponentInfos;
	mutable EmbeddedComponentInfos m_embeddedComponentInfos;

	QString m_description;
	QString m_keywords;
	int m_category;
};


} // namespace icomp


#endif // !icomp_CCompositeComponentStaticInfo_included


