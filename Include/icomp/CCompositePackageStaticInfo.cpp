#include "icomp/CCompositePackageStaticInfo.h"


#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CComponentAddress.h"


namespace icomp
{


CCompositePackageStaticInfo::CCompositePackageStaticInfo(
			const std::string& packageId,
			const icomp::IComponentEnvironmentManager* managerPtr)

:	m_packageId(packageId),
	m_envManager(*managerPtr)
{
	I_ASSERT(managerPtr != NULL);
}


void CCompositePackageStaticInfo::RegisterEmbeddedComponent(const std::string& componentId)
{
	m_embeddedComponentInfos[componentId];
}


//	reimplemented (icomp::IComponentStaticInfo)

const IComponentStaticInfo* CCompositePackageStaticInfo::GetEmbeddedComponentInfo(const std::string& embeddedId) const
{
	EmbeddedComponentInfos::iterator infoIter = m_embeddedComponentInfos.find(embeddedId);
	if (infoIter != m_embeddedComponentInfos.end()){
		ComponentInfo& info = infoIter.value();

		if (!info.isInitialized){
			info.isInitialized = true;

			icomp::CComponentAddress address(m_packageId, embeddedId);

			const icomp::IRegistry* registryPtr = m_envManager.GetRegistry(address);
			if (registryPtr != NULL){
				info.componentInfoPtr.SetPtr(new icomp::CCompositeComponentStaticInfo(*registryPtr, m_envManager, this));
			}
		}

		return info.componentInfoPtr.GetPtr();
	}

	return NULL;
}


IElementStaticInfo::Ids CCompositePackageStaticInfo::GetMetaIds(int metaGroupId) const
{
	Ids retVal;

	if (metaGroupId == MGI_EMBEDDED_COMPONENTS){
		for (		EmbeddedComponentInfos::const_iterator iter = m_embeddedComponentInfos.begin();
					iter != m_embeddedComponentInfos.end();
					++iter){
			retVal.insert(iter.key());
		}
	}

	return retVal;
}


} // namespace icomp


