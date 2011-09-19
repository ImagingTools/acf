#include "icomp/CEnvironmentManagerBase.h"


#include "icomp/IRegistry.h"


namespace icomp
{


// reimplemented (icomp::IComponentEnvironmentManager)

istd::CString CEnvironmentManagerBase::GetConfigFilePath() const
{
	return istd::CString::GetEmpty();
}


// reimplemented (icomp::IPackagesManager)

bool CEnvironmentManagerBase::LoadPackages(const istd::CString& /*configFilePath*/)
{
	return false;
}


int CEnvironmentManagerBase::GetPackageType(const std::string& /*packageId*/) const
{
	return PT_UNKNOWN;
}


istd::CString CEnvironmentManagerBase::GetPackagePath(const std::string& /*packageId*/) const
{
	return istd::CString::GetEmpty();
}


// reimplemented (icomp::IRegistriesManager)

const IRegistry* CEnvironmentManagerBase::GetRegistry(const CComponentAddress& address, const IRegistry* contextRegistryPtr) const
{
	if ((contextRegistryPtr != NULL) && address.GetPackageId().empty()){
		return contextRegistryPtr->GetEmbeddedRegistry(address.GetComponentId());
	}

	return NULL;
}


// reimplemented (icomp::IMetaInfoManager)

CEnvironmentManagerBase::ComponentAddresses CEnvironmentManagerBase::GetComponentAddresses(int typeFlag) const
{
	ComponentAddresses retVal;

	Ids packageIds = GetMetaIds(MGI_EMBEDDED_COMPONENTS);
	for (		Ids::const_iterator packageIter = packageIds.begin();
				packageIter != packageIds.end();
				++packageIter){
		const std::string packageName = *packageIter;

		const IComponentStaticInfo* packageInfoPtr = GetEmbeddedComponentInfo(packageName);
		if (packageInfoPtr != NULL){
			Ids componentIds = packageInfoPtr->GetMetaIds(MGI_EMBEDDED_COMPONENTS);

			icomp::CComponentAddress address;
			address.SetPackageId(packageName);

			for (		Ids::const_iterator componentIter = componentIds.begin();
						componentIter != componentIds.end();
						++componentIter){
				const std::string componentName = *componentIter;

				const IComponentStaticInfo* componentInfoPtr = GetEmbeddedComponentInfo(componentName);
				if (componentInfoPtr != NULL){
					int componentType = componentInfoPtr->GetComponentType();

					if ((typeFlag & (1 << componentType)) != 0){
						address.SetComponentId(*componentIter);
					}

					retVal.insert(address);
				}
			}
		}
	}

	return retVal;
}


const IComponentStaticInfo* CEnvironmentManagerBase::GetComponentMetaInfo(const CComponentAddress& address) const
{
	const icomp::IComponentStaticInfo* packageInfoPtr = GetEmbeddedComponentInfo(address.GetPackageId());
	if (packageInfoPtr != NULL){
		return packageInfoPtr->GetEmbeddedComponentInfo(address.GetComponentId());
	}

	return NULL;
}


const IComponentStaticInfo* CEnvironmentManagerBase::GetPackageMetaInfo(const std::string& packageId) const
{
	return GetEmbeddedComponentInfo(packageId);
}



} // namespace icomp


