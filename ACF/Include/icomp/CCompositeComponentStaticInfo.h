#ifndef icomp_CCompositeComponentStaticInfo_included
#define icomp_CCompositeComponentStaticInfo_included


#include "icomp/CBaseComponentStaticInfo.h"


namespace icomp
{


class IRegistry;


class CCompositeComponentStaticInfo: public CBaseComponentStaticInfo
{
public:
	CCompositeComponentStaticInfo(const IRegistry& registry);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const;
};


}//namespace icomp


#endif //!icomp_CCompositeComponentStaticInfo_included


