#ifndef istd_CGeneralTimeStamp_included
#define istd_CGeneralTimeStamp_included


// Qt includes
#include <QtCore/QObject>
#if QT_VERSION >= 0x040700
	#include <QtCore/QElapsedTimer>
#else
	#include <QtCore/QDateTime>
#endif

// ACF includes
#include "istd/ITimeStamp.h"


namespace istd
{


/**
	A general time stamp implementation.
*/
class CGeneralTimeStamp: virtual public ITimeStamp
{
public:
	CGeneralTimeStamp();

	double GetTimeTo(const CGeneralTimeStamp& timeStamp) const;

	// reimplemented (istd::ITimeStamp)
	virtual void Start(double elapsedTime = 0);
	virtual QDateTime GetStartTime() const;
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimeStamp& timeStamp) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

private:
#if QT_VERSION >= 0x040700
	QElapsedTimer m_timer;
#else
	QDateTime m_timer;
#endif

	double m_timeShift;

protected:
	// static member
	static ChangeSet s_startChangeSet;
};


} // namespace istd


#endif // !istd_CGeneralTimeStamp_included


