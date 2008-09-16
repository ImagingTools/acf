#ifndef iwin_CCriticalSection_included
#define iwin_CCriticalSection_included


#include "iqt/iqt.h"

#include "isys/ICriticalSection.h"

#include <QMutex>


namespace iqt
{


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
	QMutex m_section;
};


} // namespace iqt


#endif // !iwin_CCriticalSection_included


