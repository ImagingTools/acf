#ifndef iproc_TSyncProcessorCompBase_included
#define iproc_TSyncProcessorCompBase_included


#include "ibase/TMessageProducerWrap.h"

#include "iproc/TSyncProcessorWrap.h"

#include "icomp/CComponentBase.h"

#include "iwin/CTimer.h"


namespace iproc
{


/**
	Simple definition macro for a syncronious processor.
*/
template <class ProcessorInterface>
class TSyncProcessorCompBase:	public ibase::TMessageProducerWrap<icomp::CComponentBase>,
								public iproc::TSyncProcessorWrap<ProcessorInterface>
{
public:
	typedef ibase::TMessageProducerWrap<icomp::CComponentBase> BaseClass;
	typedef iproc::TSyncProcessorWrap<ProcessorInterface> BaseClass2;

	I_BEGIN_BASE_COMPONENT(TSyncProcessorCompBase)
		I_REGISTER_INTERFACE(ProcessorInterface)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
		I_ASSIGN(m_preprocessorCompPtr, "Preprocessor", "Pre-processing operator", false, "Preprocessor");
	I_END_COMPONENT

protected:
	typedef ProcessorInterface ProcessorInterfaceType;
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
	I_TREF(ProcessorInterfaceType, m_preprocessorCompPtr);
};


} // namespace iproc


#endif // !iproc_TSyncProcessorCompBase_included

