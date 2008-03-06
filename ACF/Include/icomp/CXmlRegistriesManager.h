#ifndef icomp_CXmlRegistriesManager_included
#define icomp_CXmlRegistriesManager_included


#include "icomp/CFileRegistriesManagerBase.h"


namespace icomp
{


class CXmlRegistriesManager: public CFileRegistriesManagerBase
{
protected:
	typedef CFileRegistriesManagerBase BaseClass;

	/**
		Constructor.
		\param	factoryPtr	pointer to main static info object used to factorize real components.
	*/
	CXmlRegistriesManager(const IComponentStaticInfo* factoryPtr);

	virtual iser::IArchive* CreateArchive(const istd::CString& path) const;
};


}//namespace icomp


#endif // !icomp_CXmlRegistriesManager_included


