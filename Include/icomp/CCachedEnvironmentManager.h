#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <istd/TSmartPtr.h>
#include <iser/ISerializable.h>
#include <iser/TCopySerializedWrap.h>
#include <icomp/CRegistry.h>
#include <icomp/CCompositePackageStaticInfo.h>
#include <icomp/CEnvironmentManagerBase.h>


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
	virtual const icomp::IRegistry* GetRegistry(const icomp::CComponentAddress& address, const icomp::IRegistry* contextRegistryPtr = NULL) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	typedef iser::TCopySerializedWrap<CRegistry> Registry;
	typedef QMap<QByteArray, istd::TSmartPtr<Registry>> RegistriesMap;

	struct PackageInfo
	{
		istd::TDelPtr<CCompositePackageStaticInfo> staticInfoPtr;
		RegistriesMap registriesMap;
	};
	typedef QMap<QByteArray, PackageInfo> PackagesMap;

	PackagesMap m_packagesMap;
};


} // namespace icomp


