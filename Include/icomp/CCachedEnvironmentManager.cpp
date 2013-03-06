#include "icomp/CCachedEnvironmentManager.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icomp
{


bool CCachedEnvironmentManager::AddComposedComponent(
			const icomp::CComponentAddress& address,
			const icomp::IRegistry& registry)
{
	PackageInfo& packageInfo = m_packagesMap[address.GetPackageId()];

	return packageInfo.registriesMap[address.GetComponentId()].CopyFrom(registry);
}


void CCachedEnvironmentManager::InitializeStaticInfo()
{
	for (		PackagesMap::Iterator packageIter = m_packagesMap.begin();
				packageIter != m_packagesMap.end();
				++packageIter){
		const QByteArray& packageId = packageIter.key();
		PackageInfo& packageInfo = packageIter.value();

		if (!packageInfo.staticInfoPtr.IsValid()){
			packageInfo.staticInfoPtr.SetPtr(new CCompositePackageStaticInfo(packageId, this));
		}

		for (		RegistriesMap::ConstIterator componentIter = packageInfo.registriesMap.constBegin();
					componentIter != packageInfo.registriesMap.constEnd();
					++componentIter){
			const QByteArray& componentId = componentIter.key();

			packageInfo.staticInfoPtr->RegisterEmbeddedComponent(componentId);
		}
	}
}


// reimplemented (icomp::IRegistriesManager)

const icomp::IRegistry* CCachedEnvironmentManager::GetRegistry(
			const icomp::CComponentAddress& address,
			const icomp::IRegistry* contextRegistryPtr) const
{
	PackagesMap::ConstIterator packageIter = m_packagesMap.constFind(address.GetPackageId());
	if (packageIter != m_packagesMap.constEnd()){
		RegistriesMap::ConstIterator registryIter = packageIter->registriesMap.constFind(address.GetComponentId());
		if (registryIter != packageIter->registriesMap.constEnd()){
			const icomp::IRegistry& registry = *registryIter;

			return &registry;
		}
		else{
			return NULL;
		}
	}

	return BaseClass::GetRegistry(address, contextRegistryPtr);
}


// reimplemented (iser::ISerializable)

bool CCachedEnvironmentManager::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag packagesTag("Packages", "Set of known packages");
	static iser::CArchiveTag packageTag("Package", "Single package description");
	static iser::CArchiveTag packageIdTag("PackageId", "Name of package");
	static iser::CArchiveTag componentsTag("Components", "List of components");
	static iser::CArchiveTag componentTag("Component", "Single component description");
	static iser::CArchiveTag componentIdTag("ComponentId", "Name of component");
	static iser::CArchiveTag registryDataTag("Registry", "Registry data");

	bool retVal = true;

	bool isStoring = archive.IsStoring();
	if (isStoring){
		int packagesCount = m_packagesMap.size();

		retVal = retVal && archive.BeginMultiTag(packagesTag, packageTag, packagesCount);
		for (		PackagesMap::Iterator packageIter = m_packagesMap.begin();
					packageIter != m_packagesMap.end();
					++packageIter){
			retVal = retVal && archive.BeginTag(packageTag);

			retVal = retVal && archive.BeginTag(packageIdTag);
			QByteArray packageId = packageIter.key();
			retVal = retVal && archive.Process(packageId);
			retVal = retVal && archive.EndTag(packageIdTag);

			const PackageInfo& packageInfo = packageIter.value();
			int componentsCount = packageInfo.registriesMap.size();

			retVal = retVal && archive.BeginMultiTag(componentsTag, componentTag, componentsCount);
			for (		RegistriesMap::ConstIterator componentIter = packageInfo.registriesMap.constBegin();
						componentIter != packageInfo.registriesMap.constEnd();
						++componentIter){
				retVal = retVal && archive.BeginTag(componentTag);

				retVal = retVal && archive.BeginTag(componentIdTag);
				QByteArray componentId = componentIter.key();
				retVal = retVal && archive.Process(componentId);
				retVal = retVal && archive.EndTag(componentIdTag);

				retVal = retVal && archive.BeginTag(registryDataTag);
				CRegistry& registry = const_cast<CRegistry&>(componentIter.value());
				retVal = retVal && registry.Serialize(archive);
				retVal = retVal && archive.EndTag(registryDataTag);

				retVal = retVal && archive.EndTag(componentTag);
			}
			retVal = retVal && archive.EndTag(componentsTag);

			retVal = retVal && archive.EndTag(packageTag);
		}
		retVal = retVal && archive.EndTag(packagesTag);
	}
	else{
		int packagesCount = 0;
		retVal = retVal && archive.BeginMultiTag(packagesTag, packageTag, packagesCount);

		if (!retVal){
			return false;
		}

		for (int packageIndex = 0; packageIndex < packagesCount; ++packagesCount){
			retVal = retVal && archive.BeginTag(packageTag);

			QByteArray packageId;
			retVal = retVal && archive.BeginTag(packageIdTag);
			retVal = retVal && archive.Process(packageId);
			retVal = retVal && archive.EndTag(packageIdTag);

			PackageInfo& packageInfo = m_packagesMap[packageId];

			int componentsCount = 0;
			retVal = retVal && archive.BeginMultiTag(componentsTag, componentTag, componentsCount);

			if (!retVal){
				return false;
			}

			for (int componentIndex = 0; componentIndex < componentsCount; ++componentIndex){
				retVal = retVal && archive.BeginTag(componentTag);

				QByteArray componentId;
				retVal = retVal && archive.BeginTag(componentIdTag);
				retVal = retVal && archive.Process(componentId);
				retVal = retVal && archive.EndTag(componentIdTag);

				if (!retVal){
					return false;
				}

				CRegistry& registry = packageInfo.registriesMap[componentId];
				retVal = retVal && archive.BeginTag(registryDataTag);
				retVal = retVal && registry.Serialize(archive);
				retVal = retVal && archive.EndTag(registryDataTag);

				retVal = retVal && archive.EndTag(componentTag);
			}
			retVal = retVal && archive.EndTag(componentsTag);

			retVal = retVal && archive.EndTag(packageTag);
		}
		retVal = retVal && archive.EndTag(packagesTag);

		InitializeStaticInfo();
	}

	return retVal;
}


} // namespace icomp


