#include "iprm/COptionsManagerComp.h"


namespace iprm
{


// protected methods

// reimplemented (icomp::CComponentBase)

void COptionsManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetSlaveSelectionConstraints(m_slaveSelectionConstraintsCompPtr.GetPtr());
}


void COptionsManagerComp::OnComponentDestroyed()
{
	SetSlaveSelectionConstraints(NULL);

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


