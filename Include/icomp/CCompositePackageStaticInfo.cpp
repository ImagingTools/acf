#include "icomp/CCompositePackageStaticInfo.h"


#include "icomp/CComponentAddress.h"


namespace icomp
{


CCompositePackageStaticInfo::CCompositePackageStaticInfo(
			const std::string& packageId,
			const Ids& componentIds,
			const icomp::IComponentEnvironmentManager* managerPtr)

:	m_packageId(packageId),
	m_envManager(*managerPtr)
{
	I_ASSERT(managerPtr != NULL);

	for (		Ids::const_iterator iter = componentIds.begin();
				iter != componentIds.end();
				++iter){
		m_subcomponentInfos.InsertLocal(*iter, ComponentInfo());
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

int CCompositePackageStaticInfo::GetComponentType() const
{
	return CT_NONE;
}


icomp::IComponent* CCompositePackageStaticInfo::CreateComponent() const
{
	return NULL;
}


const icomp::IComponentStaticInfo::InterfaceExtractors& CCompositePackageStaticInfo::GetInterfaceExtractors() const
{
	static InterfaceExtractors empty;

	return empty;
}


const icomp::IComponentStaticInfo::AttributeInfos& CCompositePackageStaticInfo::GetAttributeInfos() const
{
	static AttributeInfos empty;

	return empty;
}


icomp::IComponentStaticInfo::Ids CCompositePackageStaticInfo::GetSubcomponentIds() const
{
	Ids retVal;

	m_subcomponentInfos.GetKeys(retVal);

	return retVal;
}


const icomp::IComponentStaticInfo* CCompositePackageStaticInfo::GetSubcomponentInfo(const std::string& subcomponentId) const
{
	ComponentInfo* infoPtr = m_subcomponentInfos.FindLocalElement(subcomponentId);
	if (infoPtr != NULL){
		if (!infoPtr->isInitialized){
			icomp::CComponentAddress address(m_packageId, subcomponentId);

			const icomp::IRegistry* registryPtr = m_envManager.GetRegistry(address);
			if (registryPtr != NULL){
				infoPtr->componentInfoPtr.SetPtr(new icomp::CCompositeComponentStaticInfo(*registryPtr, m_envManager));
			}

			infoPtr->isInitialized = true;
		}

		return infoPtr->componentInfoPtr.GetPtr();
	}

	return NULL;

}


bool CCompositePackageStaticInfo::RegisterInterfaceExtractor(const istd::CClassInfo& /*interfaceId*/, InterfaceExtractorPtr /*extractorPtr*/)
{
	return false;
}


bool CCompositePackageStaticInfo::RegisterAttributeInfo(const std::string& /*attributeId*/, const icomp::IAttributeStaticInfo* /*attributeInfoPtr*/)
{
	return false;
}


bool CCompositePackageStaticInfo::RegisterSubcomponentInfo(const std::string& /*subcomponentId*/, const IComponentStaticInfo* /*componentInfoPtr*/)
{
	return false;
}


const istd::CString& CCompositePackageStaticInfo::GetDescription() const
{
	return istd::CString::GetEmpty();
}


const istd::CString& CCompositePackageStaticInfo::GetKeywords() const
{
	return istd::CString::GetEmpty();
}


}//namespace icomp


