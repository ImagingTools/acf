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

void CChangeDelegator::OnBeginChanges()
{
	if (m_slavePtr != NULL){
		m_slavePtr->BeginChanges(GetDelegatedChanges());
	}
}


void CChangeDelegator::OnEndChanges(const ChangeSet& changeSet)
{
	if ((m_slavePtr != NULL) && !changeSet.IsEmpty()){
		m_slavePtr->EndChanges(GetDelegatedChanges());
	}
}


} // namespace istd

