#ifndef icmpstr_CRegistryConsistInfoComp_included
#define icmpstr_CRegistryConsistInfoComp_included


// Qt includes
#include <QtCore/QHash>
#include <QtGui/QIcon>

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
				const icomp::IElementStaticInfo::Ids& interfaceNames,
				const icomp::IRegistry& registry,
				int queryFlags) const;
	virtual bool IsRegistryValid(
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	virtual bool IsElementValid(
				const QByteArray& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	virtual bool IsElementWithInfoValid(
				const QByteArray& elementName,
				const icomp::IRegistry::ElementInfo& elementInfo,
				const icomp::IComponentStaticInfo* metaInfoPtr,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	virtual bool IsAttributeValid(
				const QByteArray& attributeName,
				const QByteArray& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	virtual QIcon GetComponentIcon(const icomp::CComponentAddress& address) const;

protected:
	/**
		Get list of compatible element ID's.
		It include also pointed element.
	*/
	icomp::IRegistry::Ids GetCompatibleIds(
				const QByteArray& elementId,
				const icomp::IElementStaticInfo& elementStaticInfo,
				const icomp::IElementStaticInfo::Ids& interfaceNames,
				bool subcomponentsFlag) const;
	bool CheckAttributeCompatibility(
				const iser::IObject& attribute,
				const icomp::IAttributeStaticInfo& attributeMetaInfo,
				const QByteArray& attributeName,
				const QByteArray& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	bool CheckPointedElementCompatibility(
				const QByteArray& pointedElementName,
				const icomp::IElementStaticInfo::Ids& interfaceNames,
				const QByteArray& attributeName,
				const QByteArray& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				ibase::IMessageConsumer* reasonConsumerPtr) const;
	bool CheckPointedElementInfoCompatibility(
				const QByteArray& pointedElementName,
				const icomp::IElementStaticInfo* pointedMetaInfoPtr,
				const icomp::IElementStaticInfo::Ids& interfaceNames,
				const QByteArray& attributeName,
				const QByteArray& elementName,
				bool ignoreUndef,
				ibase::IMessageConsumer* reasonConsumerPtr) const;

private:
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(IExternalMetaInfoManager, m_externalMetaInfoManagerCompPtr);

	typedef QMap<icomp::CComponentAddress, QIcon> IconCache;
	mutable IconCache m_iconCache;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryConsistInfoComp_included


