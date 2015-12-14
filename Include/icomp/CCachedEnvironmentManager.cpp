#include "icomp/CCachedEnvironmentManager.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace icomp
{


// static constants
static const iser::CArchiveTag s_packagesTag("Packages", "Set of known packages", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_packageTag("Package", "Single package description", iser::CArchiveTag::TT_GROUP, &s_packagesTag);
static const iser::CArchiveTag s_packageIdTag("PackageId", "Name of package", iser::CArchiveTag::TT_LEAF, &s_packageTag);
static const iser::CArchiveTag s_componentsTag("Components", "List of components", iser::CArchiveTag::TT_MULTIPLE, &s_packageTag);
static const iser::CArchiveTag s_componentTag("Component", "Single component description", iser::CArchiveTag::TT_GROUP, &s_componentsTag);
static const iser::CArchiveTag s_componentIdTag("ComponentId", "Name of component", iser::CArchiveTag::TT_LEAF, &s_componentTag);
static const iser::CArchiveTag s_registryDataTag("Registry", "Registry data", iser::CArchiveTag::TT_GROUP, &s_componentTag);


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

		RegisterEmbeddedComponentInfo(packageId, packageInfo.staticInfoPtr.GetPtr());
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
	bool retVal = true;

	bool isStoring = archive.IsStoring();
	if (isStoring){
		int packagesCount = m_packagesMap.size();

		retVal = retVal && archive.BeginMultiTag(s_packagesTag, s_packageTag, packagesCount);
		for (		PackagesMap::Iterator packageIter = m_packagesMap.begin();
					packageIter != m_packagesMap.end();
					++packageIter){
			retVal = retVal && archive.BeginTag(s_packageTag);

			retVal = retVal && archive.BeginTag(s_packageIdTag);
			QByteArray packageId = packageIter.key();
			retVal = retVal && archive.Process(packageId);
			retVal = retVal && archive.EndTag(s_packageIdTag);

			const PackageInfo& packageInfo = packageIter.value();
			int componentsCount = packageInfo.registriesMap.size();

			retVal = retVal && archive.BeginMultiTag(s_componentsTag, s_componentTag, componentsCount);
			for (		RegistriesMap::ConstIterator componentIter = packageInfo.registriesMap.constBegin();
						componentIter != packageInfo.registriesMap.constEnd();
						++componentIter){
				retVal = retVal && archive.BeginTag(s_componentTag);

				retVal = retVal && archive.BeginTag(s_componentIdTag);
				QByteArray componentId = componentIter.key();
				retVal = retVal && archive.Process(componentId);
				retVal = retVal && archive.EndTag(s_componentIdTag);

				retVal = retVal && archive.BeginTag(s_registryDataTag);
				Registry& registry = const_cast<Registry&>(componentIter.value());
				retVal = retVal && registry.Serialize(archive);
				retVal = retVal && archive.EndTag(s_registryDataTag);

				retVal = retVal && archive.EndTag(s_componentTag);
			}
			retVal = retVal && archive.EndTag(s_componentsTag);

			retVal = retVal && archive.EndTag(s_packageTag);
		}
		retVal = retVal && archive.EndTag(s_packagesTag);
	}
	else{
		int packagesCount = 0;
		retVal = retVal && archive.BeginMultiTag(s_packagesTag, s_packageTag, packagesCount);

		if (!retVal){
			return false;
		}

		for (int packageIndex = 0; packageIndex < packagesCount; ++packageIndex){
			retVal = retVal && archive.BeginTag(s_packageTag);

			QByteArray packageId;
			retVal = retVal && archive.BeginTag(s_packageIdTag);
			retVal = retVal && archive.Process(packageId);
			retVal = retVal && archive.EndTag(s_packageIdTag);

			PackageInfo& packageInfo = m_packagesMap[packageId];

			int componentsCount = 0;
			retVal = retVal && archive.BeginMultiTag(s_componentsTag, s_componentTag, componentsCount);

			if (!retVal){
				return false;
			}

			for (int componentIndex = 0; componentIndex < componentsCount; ++componentIndex){
				retVal = retVal && archive.BeginTag(s_componentTag);

				QByteArray componentId;
				retVal = retVal && archive.BeginTag(s_componentIdTag);
				retVal = retVal && archive.Process(componentId);
				retVal = retVal && archive.EndTag(s_componentIdTag);

				if (!retVal){
					return false;
				}

				Registry& registry = packageInfo.registriesMap[componentId];
				retVal = retVal && archive.BeginTag(s_registryDataTag);
				retVal = retVal && registry.Serialize(archive);
				retVal = retVal && archive.EndTag(s_registryDataTag);

				retVal = retVal && archive.EndTag(s_componentTag);
			}
			retVal = retVal && archive.EndTag(s_componentsTag);

			retVal = retVal && archive.EndTag(s_packageTag);
		}
		retVal = retVal && archive.EndTag(s_packagesTag);

		InitializeStaticInfo();
	}

	return retVal;
}


} // namespace icomp


