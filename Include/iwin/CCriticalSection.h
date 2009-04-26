#ifndef iwin_CCriticalSection_included
#define iwin_CCriticalSection_included


#include "isys/ICriticalSection.h"

#include "iwin/iwin.h"

#include <windows.h>


namespace iwin
{


/**
	Realization of critical section on Windows.

	\ingroup Windows
*/
class CCriticalSection: virtual public isys::ICriticalSection
{
public:
	CCriticalSection();
	~CCriticalSection();

	// reimplemented (isys::ICriticalSection)
	virtual void Enter();
	virtual bool EnterIfFree();
	virtual void Leave();

private:
	CRITICAL_SECTION m_section;
};


} // namespace iwin


#endif // !iwin_CCriticalSection_included


