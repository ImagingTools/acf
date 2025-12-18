#ifndef icomp_IMetaInfoManager_included
#define icomp_IMetaInfoManager_included


// ACF includes
#include <icomp/IComponentStaticInfo.h>
#include <icomp/IComponentListProvider.h>


namespace icomp
{


class IMetaInfoManager: virtual public IComponentListProvider
{
public:
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
	virtual const IComponentStaticInfo* GetPackageMetaInfo(const QByteArray& packageId) const = 0;
};


} // namespace icomp


#endif // !icomp_IMetaInfoManager_included


