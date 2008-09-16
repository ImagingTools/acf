#include "iqt/CCriticalSection.h"


namespace iqt
{


CCriticalSection::CCriticalSection()
{
}


CCriticalSection::~CCriticalSection()
{
}


// reimplemented (isys::ICriticalSection)

void CCriticalSection::Enter()
{
	m_section.lock();
}


bool CCriticalSection::EnterIfFree()
{
	return m_section.tryLock();
}


void CCriticalSection::Leave()
{
	m_section.unlock();
}


} // namespace iqt

