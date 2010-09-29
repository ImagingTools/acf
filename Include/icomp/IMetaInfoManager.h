#ifndef icomp_IPackagesManager_included
#define icomp_IPackagesManager_included


#include "istd/IPolymorphic.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/CComponentAddress.h"


namespace icomp
{


class IMetaInfoManager: virtual public istd::IPolymorphic
{
public:
	enum ComponentTypeFlag
	{
		CTF_REAL = 1 << IComponentStaticInfo::CT_REAL,
		CTF_COMPOSITE = 1 << IComponentStaticInfo::CT_COMPOSITE,
		CTF_ALL = CTF_REAL | CTF_COMPOSITE
	};

	typedef std::set<CComponentAddress> ComponentAddresses;

	/**
		Get addresses of known components.
	*/
	virtual ComponentAddresses GetComponentAddresses(int typeFlag = CTF_ALL) const = 0;

	/**
		Get access to static info of some component.
		\param	address	component address.
		\return	static info of component or NULL, if no component type for specified address was found.
	*/
	virtual const IComponentStaticInfo* GetComponentMetaInfo(const CComponentAddress& address) const = 0;
	
	/**
		Get access to static info of a package.
		\param	package ID.
		\return	static info of the package or NULL, if no package for specified ID was found.
	*/
	virtual const IComponentStaticInfo* GetPackageMetaInfo(const std::string& packageId) const = 0;

};


}//namespace icomp


#endif // !icomp_IPackagesManager_included


