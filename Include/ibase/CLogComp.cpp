#include "ibase/CLogComp.h"


namespace ibase
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CLogComp::OnComponentCreated()
{
	if (m_maxMessageCountAttrPtr.IsValid()){
		SetMaxMessageCount(m_maxMessageCountAttrPtr->GetValue());
	}
}


} // namespace ibase


