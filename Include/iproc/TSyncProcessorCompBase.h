#ifndef iproc_TSyncProcessorCompBase_included
#define iproc_TSyncProcessorCompBase_included


#include "ibase/TLoggerCompWrap.h"

#include "iproc/TSyncProcessorWrap.h"

#include "icomp/CComponentBase.h"


namespace iproc
{


/**
	Simple definition macro for a synchronous processor.
*/
template <class Base, class Interface = Base>
class TSyncProcessorCompBase:
			public ibase::CLoggerComponentBase,
			public iproc::TSyncProcessorWrap<Base>
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef iproc::TSyncProcessorWrap<Base> BaseClass2;
	typedef Interface InterfaceType;

	I_BEGIN_BASE_COMPONENT(TSyncProcessorCompBase)
		I_REGISTER_INTERFACE(Interface)
		I_REGISTER_INTERFACE(iproc::IProcessor)
		I_ASSIGN(m_paramsIdAttrPtr, "ParamsId", "ID of processor parameter", true, "ParamsId");
	I_END_COMPONENT

protected:
	I_ATTR(istd::CString, m_paramsIdAttrPtr);
};


// global typedefs

typedef TSyncProcessorCompBase<iproc::IProcessor> CSyncProcessorCompBase;


} // namespace iproc


#endif // !iproc_TSyncProcessorCompBase_included

