#ifndef iipr_CExtremumCaliperProcessorComp_included
#define iipr_CExtremumCaliperProcessorComp_included


#include "istd/CString.h"

#include "icomp/CComponentBase.h"

#include "iipr/CExtremumCaliperProcessor.h"


namespace iipr
{


class CExtremumCaliperProcessorComp: public icomp::CComponentBase, public CExtremumCaliperProcessor
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CExtremumCaliperProcessor BaseClass2;

	I_BEGIN_COMPONENT(CExtremumCaliperProcessorComp);
		I_REGISTER_INTERFACE(ICaliperProcessor);
		I_ASSIGN(m_caliperParamsIdAttrPtr, "CaliperParamsId", "ID of caliper parameters in parameter set", true, "CaliperParams");
	I_END_COMPONENT

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_ATTR(istd::CString, m_caliperParamsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CExtremumCaliperProcessorComp_included


