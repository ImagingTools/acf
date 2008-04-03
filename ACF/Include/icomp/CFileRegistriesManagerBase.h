#ifndef icomp_CFileRegistriesManagerBase_included
#define icomp_CFileRegistriesManagerBase_included


#include <string>
#include <map>

#include "istd/IPolymorphic.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"

#include "icomp/IRegistry.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


class CFileRegistriesManagerBase: virtual public IRegistriesManager
{
public:
	const IRegistry* GetRegistryFromFile(const istd::CString& path) const;

	// reimplemented (icomp::IRegistriesManager)
	virtual const IRegistry* GetRegistry(const std::string& packageId, const std::string& componentId, const IRegistry* contextPtr) const;

protected:
	/**
		Constructor.
		\param	factoryPtr	pointer to main static info object used to factorize real components.
	*/
	CFileRegistriesManagerBase(const IComponentStaticInfo* factoryPtr);

	// abstract methods
	/**
		Create archive for specified path.
		\param	path		path to registry file without extension.
	*/
	virtual iser::IArchive* CreateArchive(const istd::CString& path) const = 0;

private:
	typedef std::map<istd::CString, istd::TDelPtr<IRegistry> > RegistriesMap;
	typedef std::map<const IRegistry*, istd::CString> InvRegistriesMap;

	mutable RegistriesMap m_registriesMap;
	mutable InvRegistriesMap m_invRegistriesMap;

	const IComponentStaticInfo& m_componentsFactory;
};


}//namespace icomp


#endif // !icomp_CFileRegistriesManagerBase_included


