#include "iipr/CLineProjectionProcessorComp.h"


namespace iipr
{


// reimplemented (icomp::IComponent)

void CLineProjectionProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_lineParamIdAttrPtr.IsValid());	// isObligatory was set 'true'
	SetLineParamId((*m_lineParamIdAttrPtr).ToString());
}


} // namespace iipr

