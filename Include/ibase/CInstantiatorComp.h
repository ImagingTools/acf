#ifndef ibase_CInstantiatorComp_included
#define ibase_CInstantiatorComp_included


#include <icomp/CComponentBase.h>


namespace ibase
{


/**
	This help component simply instantiate a set of dependent components during its instantiation.
	\deprecated
*/
class CInstantiatorComp: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CInstantiatorComp);
		I_ASSIGN_MULTI_0(m_instancesAttrPtr, "Instances", "List of instances will be created during instantiator is created", false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(istd::IPolymorphic, m_instancesAttrPtr);
};


} // namespace ibase


#endif // !ibase_CInstantiatorComp_included


