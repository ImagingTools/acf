#ifndef ibase_CInstantiatorComp_included
#define ibase_CInstantiatorComp_included


#include "icomp/CComponentBase.h"


namespace ibase
{


/**
	This help component simply instanciate set of dependent components during its instantiation.
*/
class CInstantiatorComp: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CInstantiatorComp)
		I_ASSIGN_MULTI_0(m_instancesAttrPtr, "Instances", "List of instances will be created during instantiator is created", false)
		I_ASSIGN(m_freeComponentCompPtr, "FreeComponent", "Free component initialization", false, "FreeComponent")
	I_END_COMPONENT

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(void*, m_instancesAttrPtr);
	I_REF(void*, m_freeComponentCompPtr);
};


} // namespace ibase


#endif // !ibase_CInstantiatorComp_included


