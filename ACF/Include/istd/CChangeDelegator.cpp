#include "istd/CChangeDelegator.h"


namespace istd
{


// public methods

CChangeDelegator::CChangeDelegator()
{
	m_slavePtr = NULL;
}


CChangeDelegator::CChangeDelegator(IChangeable* slavePtr)
:	m_slavePtr(slavePtr)
{
}


// reimplemented (istd::IChangeable)

void CChangeDelegator::BeginChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (m_slavePtr != NULL){
		m_slavePtr->BeginChanges(changeFlags | CF_DELEGATED, changeParamsPtr);
	}
}


void CChangeDelegator::EndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	if (m_slavePtr != NULL){
		m_slavePtr->EndChanges(changeFlags | CF_DELEGATED, changeParamsPtr);
	}
}


} // namespace istd

