#include "istd/CChangeGroup.h"


// ACF includes
#include "istd/IChangeable.h"


namespace istd
{


bool CChangeGroup::IsValid() const
{
	return m_changeablePtr != NULL;
}


void CChangeGroup::Reset()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(m_changeSet);

		m_changeablePtr = NULL;
	}
}


// private methods

CChangeGroup::CChangeGroup(const CChangeGroup& /*group*/)
:	m_changeSet(istd::IChangeable::GetNoChanges())
{
}


} // namespace istd


