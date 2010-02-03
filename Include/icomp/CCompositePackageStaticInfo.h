#ifndef icomp_CCompositePackageStaticInfo_included
#define icomp_CCompositePackageStaticInfo_included


#include "istd/TCascadedMap.h"
#include "istd/TDelPtr.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CComponentStaticInfoBase.h"
#include "icomp/CCompositeComponentStaticInfo.h"


namespace icomp
{


class CCompositePackageStaticInfo: public icomp::CComponentStaticInfoBase
{
public:
	CCompositePackageStaticInfo(
				const std::string& packageId,
				const Ids& componentIds,
				const icomp::IComponentEnvironmentManager* managerPtr);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual Ids GetSubcomponentIds() const;
	virtual const icomp::IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const;
	virtual bool RegisterInterfaceExtractor(const istd::CClassInfo& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const icomp::IAttributeStaticInfo* attributeInfoPtr);
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);

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
	const icomp::IComponentEnvironmentManager& m_envManager;
};


}//namespace icomp


#endif // !icomp_CCompositePackageStaticInfo_included


