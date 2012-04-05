#ifndef icmpstr_CElementSelectionInfoManagerBase_included
#define icmpstr_CElementSelectionInfoManagerBase_included


// ACF includes
#include "istd/TDelPtr.h"
#include "istd/TOptDelPtr.h"
#include "istd/CClassInfo.h"

#include "imod/TSingleModelObserverBase.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IRegistryElement.h"

#include "icmpstr/IAttributeSelectionObserver.h"
#include "icmpstr/IElementSelectionInfo.h"
#include "icmpstr/IRegistryConsistInfo.h"


namespace icmpstr
{


class CElementSelectionInfoManagerBase: public imod::TSingleModelObserverBase<IElementSelectionInfo>
{
public:
	typedef imod::TSingleModelObserverBase<IElementSelectionInfo> BaseClass;

	CElementSelectionInfoManagerBase();

	icomp::IRegistry* GetRegistry() const;
	QStringList GetExportAliases(const std::string& attributeName) const;
	const icomp::IComponentStaticInfo* GetComponentMetaInfo(const icomp::CComponentAddress& address) const;
	const iser::IObject* GetAttributeObject(const std::string& attributeId, const icomp::IRegistry::ElementInfo& elementInfo) const;
	const icomp::IAttributeStaticInfo* GetAttributeStaticInfo(const std::string& attributeId, const icomp::IRegistry::ElementInfo& elementInfo) const;

	// abstract methods
	virtual const icomp::IComponentEnvironmentManager* GetMetaInfoManagerPtr() const = 0;
	virtual const icmpstr::IRegistryConsistInfo* GetConsistencyInfoPtr() const = 0;

protected:
	void UpdateAddressToMetaInfoMap();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	struct AttrInfo
	{
		istd::TPointerBase<const icomp::IRegistryElement::AttributeInfo> infoPtr;
		istd::TPointerBase<const icomp::IAttributeStaticInfo> staticInfoPtr;
	};

	typedef QMap<std::string, AttrInfo> ElementIdToAttrInfoMap;
	typedef QMap<std::string, ElementIdToAttrInfoMap> AttrInfosMap;

	typedef QMap<icomp::CComponentAddress, istd::TOptDelPtr<const icomp::IComponentStaticInfo> > AddressToInfoMap;

	AddressToInfoMap m_adressToMetaInfoMap;
};


} // namespace icmpstr


#endif // !icmpstr_CElementSelectionInfoManagerBase_included

