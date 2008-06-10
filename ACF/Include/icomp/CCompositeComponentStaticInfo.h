#ifndef icomp_CCompositeComponentStaticInfo_included
#define icomp_CCompositeComponentStaticInfo_included


#include "istd/TCascadedMap.h"
#include "istd/TDelPtr.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistriesManager.h"


namespace icomp
{


class CCompositeComponentStaticInfo: public IComponentStaticInfo
{
public:
	CCompositeComponentStaticInfo(const IRegistry* m_registryPtr);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const;
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual Ids GetSubcomponentIds() const;
	virtual const IComponentStaticInfo* GetSubcomponent(const std::string& subcomponentId) const;
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);
	virtual istd::CString GetDescription() const;
	virtual istd::CString GetKeywords() const;

private:
	const IRegistry& m_registry;
};


}//namespace icomp


#endif //!icomp_CCompositeComponentStaticInfo_included


