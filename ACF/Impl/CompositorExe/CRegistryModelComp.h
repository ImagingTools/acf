#ifndef CRegistryModelComp_included
#define CRegistryModelComp_included


#include "icomp/CRegistry.h"
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"


class CRegistryModelComp: public icomp::TModelCompWrap< 
									icomp::TMakeComponentWrap<icomp::IRegistry, icomp::CRegistry> >
{
public:
	typedef icomp::TModelCompWrap< 
				icomp::TMakeComponentWrap<icomp::IRegistry, icomp::CRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CRegistryModelComp)
		I_ASSIGN(m_staticInfoCompPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo")
	I_END_COMPONENT

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_REF(icomp::IComponentStaticInfo, m_staticInfoCompPtr);
};


#endif // !CRegistryModelComp_included


