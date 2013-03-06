#ifndef icomp_CCachedEnvironmentManager_included
#define icomp_CCachedEnvironmentManager_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "iser/ISerializable.h"
#include "icomp/CRegistry.h"
#include "icomp/CCompositePackageStaticInfo.h"
#include "icomp/CEnvironmentManagerBase.h"


namespace icomp
{


/**
	Provide environment manager using stored registries.
*/
class CCachedEnvironmentManager:
			public CEnvironmentManagerBase,
			public iser::ISerializable
{
public:
	typedef CEnvironmentManagerBase BaseClass;

	/**
		Add single registry to this manager.
	*/
	bool AddComposedComponent(const icomp::CComponentAddress& address, const icomp::IRegistry& registry);

	/**
		Initialize all static info for registries.
	*/
	void InitializeStaticInfo();

	// reimplemented (icomp::IRegistriesManager)
	virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr = NULL) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef QMap<QByteArray, CRegistry> RegistriesMap;
	struct PackageInfo
	{
		istd::TDelPtr<CCompositePackageStaticInfo> staticInfoPtr;
		RegistriesMap registriesMap;
	};
	typedef QMap<QByteArray, PackageInfo> PackagesMap;

	PackagesMap m_packagesMap;
};


} // namespace icomp


#endif // !icomp_CCachedEnvironmentManager_included


