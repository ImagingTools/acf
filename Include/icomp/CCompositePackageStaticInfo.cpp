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
		const std::string& id = *iter;

		m_subcomponentInfos[id];
	}
}


//	reimplemented (icomp::IComponentStaticInfo)

const IComponentStaticInfo* CCompositePackageStaticInfo::GetEmbeddedComponentInfo(const std::string& embeddedId) const
{
	SubcomponentInfos::iterator infoIter = m_subcomponentInfos.find(embeddedId);
	if (infoIter != m_subcomponentInfos.end()){
		ComponentInfo& info = infoIter->second;

		if (!info.isInitialized){
			icomp::CComponentAddress address(m_packageId, embeddedId);

			const icomp::IRegistry* registryPtr = m_envManager.GetRegistry(address);
			if (registryPtr != NULL){
				info.componentInfoPtr.SetPtr(new icomp::CCompositeComponentStaticInfo(*registryPtr, m_envManager, this));
			}

			info.isInitialized = true;
		}

		return info.componentInfoPtr.GetPtr();
	}

	return NULL;

}


IComponentStaticInfo::Ids CCompositePackageStaticInfo::GetMetaIds(int metaGroupId) const
{
	Ids retVal;

	if (metaGroupId == MGI_EMBEDDED_COMPONENTS){
		for (		SubcomponentInfos::const_iterator iter = m_subcomponentInfos.begin();
					iter != m_subcomponentInfos.end();
					++iter){
			retVal.insert(iter->first);
		}
	}

	return retVal;
}


}//namespace icomp


