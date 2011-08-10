#ifndef icmpstr_CRegistryConsistInfoComp_included
#define icmpstr_CRegistryConsistInfoComp_included


// QT includes
#include <QHash>
#include <QIcon>

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CComponentBase.h"

#include "icmpstr/IRegistryConsistInfo.h"
#include "icmpstr/IExternalMetaInfoManager.h"


namespace icmpstr
{


class CRegistryConsistInfoComp:
			public QObject,
			public icomp::CComponentBase,
			virtual public IRegistryConsistInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	enum MessageId
	{
		MI_COMPONENT_INACTIVE = 0x819f0,
		MI_NO_ELEMENT_INFO,
		MI_BAD_ATTRIBUTE_TYPE,
		MI_UNDEF_ATTRIBUTE,
		MI_COMPOSITE_FOUND,
		MI_WRONG_INTERFACE,
		MI_COMPONENT_NOT_FOUND,
		MI_REF_NOT_RESOLVED
	};

	I_BEGIN_COMPONENT(CRegistryConsistInfoComp);
		I_REGISTER_INTERFACE(IRegistryConsistInfo);
		I_ASSIGN(m_envManagerCompPtr, "EnvironmentManager", "Allows access to component environment information", true, "EnvironmentManager");
		I_ASSIGN_TO(m_externalMetaInfoManagerCompPtr, m_envManagerCompPtr, false);
	I_END_COMPONENT;

	// reimplemented (icmpstr::IRegistryConsistInfo)
	virtual icomp::IRegistry::Ids GetCompatibleElements(
				const icomp::IComponentStaticInfo::Ids& interfaceNames,
				const icomp::IRegistry& registry,
				int queryFlags) const;
	virtual bool IsRegistryValid(
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	virtual bool IsElementValid(
				const std::string& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	virtual bool IsElementWithInfoValid(
				const std::string& elementName,
				const icomp::IRegistry::ElementInfo& elementInfo,
				const icomp::IComponentStaticInfo* metaInfoPtr,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	virtual bool IsAttributeValid(
				const std::string& attributeName,
				const std::string& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	virtual QIcon GetComponentIcon(const icomp::CComponentAddress& address) const;

protected:
	icomp::IRegistry::Ids GetCompatibleSubcomponents(
				const std::string& elementId,
				const icomp::IComponentStaticInfo& elementStaticInfo,
				const icomp::IComponentStaticInfo::Ids& interfaceNames) const;
	istd::CString GetAddressName(const icomp::CComponentAddress& address) const;
	bool CheckAttributeCompatibility(
				const iser::IObject& attribute,
				const icomp::IAttributeStaticInfo& attributeMetaInfo,
				const std::string& attributeName,
				const std::string& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	bool CheckPointedElementCompatibility(
				const std::string& pointedElementName,
				const icomp::IComponentStaticInfo::Ids& interfaceNames,
				const std::string& attributeName,
				const std::string& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	bool CheckPointedElementInfoCompatibility(
				const std::string& pointedElementName,
				const icomp::IComponentStaticInfo* pointedMetaInfoPtr,
				const icomp::IComponentStaticInfo::Ids& interfaceNames,
				const std::string& attributeName,
				const std::string& elementName,
				bool ignoreUndef,
				ibase::IMessageConsumer* reasonConsumerPtr) const;

private:
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(IExternalMetaInfoManager, m_externalMetaInfoManagerCompPtr);

	typedef std::map<icomp::CComponentAddress, QIcon> IconCache;
	mutable IconCache m_iconCache;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryConsistInfoComp_included


