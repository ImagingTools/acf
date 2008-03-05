#ifndef iwin_CCriticalSection_included
#define iwin_CCriticalSection_included


#include <windows.h>

#include "isys/ICriticalSection.h"

#include "iwin/iwin.h"


namespace iwin{


class CCriticalSection: public isys::ICriticalSection
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


