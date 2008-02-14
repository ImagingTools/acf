#include "iwin/CWinCriticalSection.h"


extern "C" BOOL _stdcall TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);	// TODO: try to find other work around


namespace iwin{


CWinCriticalSection::CWinCriticalSection()
{
	::InitializeCriticalSection(&m_section);
}


CWinCriticalSection::~CWinCriticalSection()
{
	::DeleteCriticalSection(&m_section);
}


// reimplemented (isys::ICriticalSection)

void CWinCriticalSection::Enter()
{
	::EnterCriticalSection(&m_section);
}


bool CWinCriticalSection::EnterIfFree()
{
	BOOL status = ::TryEnterCriticalSection(&m_section);

	return status == TRUE;
}


void CWinCriticalSection::Leave()
{
	::LeaveCriticalSection(&m_section);
}


} // namespace qwin


