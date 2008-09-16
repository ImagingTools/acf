#include "iwin/CCriticalSection.h"


extern "C" BOOL _stdcall TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);	// TODO: try to find other work around


namespace iwin
{


CCriticalSection::CCriticalSection()
{
	::InitializeCriticalSection(&m_section);
}


CCriticalSection::~CCriticalSection()
{
	::DeleteCriticalSection(&m_section);
}


// reimplemented (isys::ICriticalSection)

void CCriticalSection::Enter()
{
	::EnterCriticalSection(&m_section);
}


bool CCriticalSection::EnterIfFree()
{
	BOOL status = ::TryEnterCriticalSection(&m_section);

	return status == TRUE;
}


void CCriticalSection::Leave()
{
	::LeaveCriticalSection(&m_section);
}


} // namespace iwin


