#ifndef icomp_CCompositePackageStaticInfo_included
#define icomp_CCompositePackageStaticInfo_included


#include "istd/TCascadedMap.h"
#include "istd/TDelPtr.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/CCompositeComponentStaticInfo.h"


namespace icomp
{


class CCompositePackageStaticInfo: public icomp::IComponentStaticInfo
{
public:
	CCompositePackageStaticInfo(
				const std::string& packageId,
				const Ids& componentIds,
				const icomp::IRegistriesManager* registriesManagerPtr);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual icomp::IComponent* CreateComponent() const;
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual Ids GetSubcomponentIds() const;
	virtual const icomp::IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const;
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const icomp::IAttributeStaticInfo* attributeInfoPtr);
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);
	virtual istd::CString GetDescription() const;
	virtual istd::CString GetKeywords() const;

private:
	struct ComponentInfo
	{
		ComponentInfo():isInitialized(false){}

		istd::TDelPtr<icomp::CCompositeComponentStaticInfo> componentInfoPtr;
		bool isInitialized;
	};

	typedef istd::TCascadedMap< std::string, ComponentInfo > SubcomponentInfos;

	mutable SubcomponentInfos m_subcomponentInfos;
	std::string m_packageId;
	const icomp::IRegistriesManager& m_registriesManager;
};


}//namespace icomp


#endif // !icomp_CCompositePackageStaticInfo_included


