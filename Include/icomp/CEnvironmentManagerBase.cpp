#include "icomp/CEnvironmentManagerBase.h"


namespace icomp
{


// reimplemented (icomp::IRegistryLoader)

const IRegistry* CEnvironmentManagerBase::GetRegistryFromFile(const istd::CString& /*path*/) const
{
	return NULL;
}


// reimplemented (icomp::IComponentEnvironmentManager)

bool CEnvironmentManagerBase::ConfigureEnvironment(const istd::CString& /*configFilePath*/)
{
	return false;
}


istd::CString CEnvironmentManagerBase::GetConfigFilePath() const
{
	return istd::CString::GetEmpty();
}


// reimplemented (icomp::IRegistriesManager)

const IRegistry* CEnvironmentManagerBase::GetRegistry(const CComponentAddress& /*address*/) const
{
	return NULL;
}


istd::CString CEnvironmentManagerBase::GetPackageDirPath(const std::string& /*packageId*/) const
{
	return istd::CString::GetEmpty();
}


// reimplemented (icomp::IMetaInfoManager)

CEnvironmentManagerBase::ComponentAddresses CEnvironmentManagerBase::GetComponentAddresses(int typeFlag) const
{
	ComponentAddresses retVal;

	Ids packageIds = GetSubcomponentIds();
	for (		Ids::const_iterator packageIter = packageIds.begin();
				packageIter != packageIds.end();
				++packageIter){
		const std::string packageName = *packageIter;

		const IComponentStaticInfo* packageInfoPtr = GetSubcomponentInfo(packageName);
		if (packageInfoPtr != NULL){
			Ids componentIds = packageInfoPtr->GetSubcomponentIds();

			icomp::CComponentAddress address;
			address.SetPackageId(packageName);

			for (		Ids::const_iterator componentIter = componentIds.begin();
						componentIter != componentIds.end();
						++componentIter){
				const std::string componentName = *componentIter;

				const IComponentStaticInfo* componentInfoPtr = GetSubcomponentInfo(componentName);
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


istd::CString CEnvironmentManagerBase::GetComponentInfoPath(const icomp::CComponentAddress& /*address*/) const
{
	return istd::CString::GetEmpty();
}


const IComponentStaticInfo* CEnvironmentManagerBase::GetComponentMetaInfo(const CComponentAddress& address) const
{
	const icomp::IComponentStaticInfo* packageInfoPtr = GetSubcomponentInfo(address.GetPackageId());
	if (packageInfoPtr != NULL){
		return packageInfoPtr->GetSubcomponentInfo(address.GetComponentId());
	}

	return NULL;
}


} // namespace icomp


