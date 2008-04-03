#include "icomp/CFileRegistriesManagerBase.h"


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/IFileSystem.h"

#include "icomp/CRegistry.h"


namespace icomp
{


const IRegistry* CFileRegistriesManagerBase::GetRegistryFromFile(const istd::CString& path) const
{
	istd::CString correctedPath = path;

	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr != NULL){
		correctedPath = fileSystemPtr->GetNormalizedPath(correctedPath);
	}

	RegistriesMap::const_iterator iter = m_registriesMap.find(correctedPath);

	if (iter != m_registriesMap.end()){
		return iter->second.GetPtr();
	}

	istd::TDelPtr<IRegistry>& mapValue = m_registriesMap[correctedPath];
	iser::IArchive* archivePtr = CreateArchive(correctedPath);
	if (archivePtr != NULL){
		istd::TDelPtr<IRegistry> newRegistryPtr(new CRegistry(&m_componentsFactory));
		if (newRegistryPtr->Serialize(*archivePtr)){
			mapValue.TakeOver(newRegistryPtr);

			return mapValue.GetPtr();
		}
	}

	return NULL;
}


// reimplemented (icomp::IRegistriesManager)

const IRegistry* CFileRegistriesManagerBase::GetRegistry(
			const std::string& packageId,
			const std::string& componentId,
			const IRegistry* contextPtr) const
{
	istd::CString path;

	if (contextPtr != NULL){
		InvRegistriesMap::const_iterator contextIter = m_invRegistriesMap.find(contextPtr);

		I_ASSERT(contextIter != m_invRegistriesMap.end());

		if (contextIter != m_invRegistriesMap.end()){
			path = contextIter->second + istd::CString("/");
		}
	}

	if (!packageId.empty()){
		path += istd::CString(packageId) + istd::CString("/");
	}

	path += istd::CString(componentId);

	return GetRegistryFromFile(path);
}


// protected methods

CFileRegistriesManagerBase::CFileRegistriesManagerBase(const IComponentStaticInfo* factoryPtr)
:	m_componentsFactory(*factoryPtr)
{
	I_ASSERT(factoryPtr != NULL);
}


}//namespace icomp


