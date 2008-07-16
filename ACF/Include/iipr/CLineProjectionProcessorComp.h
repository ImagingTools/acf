#ifndef iipr_CLineProjectionProcessorComp_included
#define iipr_CLineProjectionProcessorComp_included


#include "istd/CString.h"

#include "icomp/CComponentBase.h"

#include "iipr/CLineProjectionProcessor.h"


namespace iipr
{


class CLineProjectionProcessorComp: public icomp::CComponentBase, public CLineProjectionProcessor
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CLineProjectionProcessor BaseClass2;

	I_BEGIN_COMPONENT(CLineProjectionProcessorComp);
		I_REGISTER_INTERFACE(ILineProjectionProcessor);
		I_REGISTER_INTERFACE(IProjectionConstraints);
		I_ASSIGN(m_lineParamIdAttrPtr, "LineParamId", "ID of line parameter in parameter set", true, "LineParam");
	I_END_COMPONENT

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_ATTR(istd::CString, m_lineParamIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CLineProjectionProcessorComp_included


