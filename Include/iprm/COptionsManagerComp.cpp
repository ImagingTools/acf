#include "iprm/COptionsManagerComp.h"


namespace iprm
{


// protected methods

// reimplemented (icomp::CComponentBase)

void COptionsManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetFixedOptionsList(m_fixedOptionsListCompPtr.GetPtr());
	SetSelectionSerialized(*m_serializeSelectionAttrPtr);
}


void COptionsManagerComp::OnComponentDestroyed()
{
	SetFixedOptionsList(NULL);

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


