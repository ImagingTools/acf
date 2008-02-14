#ifndef iwin_CWinCriticalSection_included
#define iwin_CWinCriticalSection_included


#include <windows.h>

#include "isys/ICriticalSection.h"

#include "iwin/iwin.h"


namespace iwin{


class CWinCriticalSection: public isys::ICriticalSection
{
public:
	CWinCriticalSection();
	~CWinCriticalSection();

	// reimplemented (isys::ICriticalSection)
	virtual void Enter();
	virtual bool EnterIfFree();
	virtual void Leave();

private:
	CRITICAL_SECTION m_section;
};


} // namespace iwin


#endif //!iwin_CWinCriticalSection_included


