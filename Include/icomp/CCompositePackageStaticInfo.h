#ifndef icomp_CCompositePackageStaticInfo_included
#define icomp_CCompositePackageStaticInfo_included


#include <map>

#include "istd/TDelPtr.h"

#include "iser/ISerializable.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CPackageStaticInfo.h"
#include "icomp/CCompositeComponentStaticInfo.h"


namespace icomp
{


/**
	Implementation of package static info designing to provide composite component information 'on demand'.
*/
class CCompositePackageStaticInfo: public CPackageStaticInfo
{
public:
	typedef CPackageStaticInfo BaseClass;

	CCompositePackageStaticInfo(
				const std::string& packageId,
				const Ids& componentIds,
				const icomp::IComponentEnvironmentManager* managerPtr);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const std::string& embeddedId) const;
	virtual Ids GetMetaIds(int metaGroupId) const;

private:
	struct ComponentInfo
	{
		ComponentInfo():isInitialized(false){}

		istd::TDelPtr<icomp::CCompositeComponentStaticInfo> componentInfoPtr;
		bool isInitialized;
	};

	typedef std::map<std::string, ComponentInfo> SubcomponentInfos;
	mutable SubcomponentInfos m_subcomponentInfos;

	std::string m_packageId;
	const icomp::IComponentEnvironmentManager& m_envManager;
};


}//namespace icomp


#endif // !icomp_CCompositePackageStaticInfo_included


