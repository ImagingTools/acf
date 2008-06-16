#include "CChannelAddressParamComp.h"


namespace imebase
{


// reimplemented (icomp::IComponent)

void CChannelAddressParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_mailhausAccessCompPtr.IsValid()){
		m_mailhausAccessCompPtr->CreateSelectionTree(*this);
	}
}

	
void CChannelAddressParamComp::OnComponentDestroyed()
{
	ResetNodes();

	BaseClass::OnComponentDestroyed();
}


} // namespace imebase


