#include "istd/CChangeNotifier.h"


// ACF includes
#include "istd/IChangeable.h"


namespace istd
{


bool CChangeNotifier::IsValid() const
{
	return m_changeablePtr != NULL;
}


void CChangeNotifier::Reset()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(m_changeSet);

		m_changeablePtr = NULL;
	}
}


void CChangeNotifier::Abort()
{
	if (m_changeablePtr != NULL){
		m_changeablePtr->EndChanges(IChangeable::GetNoChanges());

		m_changeablePtr = NULL;
	}
}


// private methods

CChangeNotifier::CChangeNotifier(const CChangeNotifier& /*notifier*/)
:	m_changeSet(istd::IChangeable::GetNoChanges())
{
}


} // namespace istd


