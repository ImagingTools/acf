#include "icomp/CFileRegistriesManagerBase.h"


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/IFileSystem.h"

#include "icomp/CRegistry.h"


namespace icomp
{


// reimplemented (icomp::IRegistriesManager)

const IRegistry* CFileRegistriesManagerBase::GetRegistry(
			const ::std::string& packageId,
			const ::std::string& factoryId,
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

	path += istd::CString(factoryId);

	isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
	if (fileSystemPtr != NULL){
		path = fileSystemPtr->GetNormalizedPath(path);
	}

	RegistriesMap::const_iterator iter = m_registriesMap.find(path);

	if (iter != m_registriesMap.end()){
		return iter->second.GetPtr();
	}

	istd::TDelPtr<IRegistry>& mapValue = m_registriesMap[path];
	iser::IArchive* archivePtr = CreateArchive(path);
	if (archivePtr != NULL){
		istd::TDelPtr<IRegistry> newRegistryPtr(new CRegistry(&m_componentsFactory));
		if (newRegistryPtr->Serialize(*archivePtr)){
			mapValue.TakeOver(newRegistryPtr);

			return mapValue.GetPtr();
		}
	}

	return NULL;
}


// protected methods

CFileRegistriesManagerBase::CFileRegistriesManagerBase(const IComponentStaticInfo* factoryPtr)
:	m_componentsFactory(*factoryPtr)
{
	I_ASSERT(factoryPtr != NULL);
}


}//namespace icomp


