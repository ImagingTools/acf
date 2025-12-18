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
#include <istd/ITimeStamp.h>


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
	virtual void Start(double elapsedTime = 0) override;
	virtual QDateTime GetStartTime() const override;
	virtual double GetElapsed() const override;
	virtual double GetTimeTo(const ITimeStamp& timeStamp) const override;
	virtual void WaitTo(double time) const override;
	virtual double GetTimerResolution() const override;

protected:
#if QT_VERSION >= 0x040700
	QElapsedTimer m_timer;
#else
	QDateTime m_timer;
#endif

	double m_timeShift = 0;
};


} // namespace istd


#endif // !istd_CGeneralTimeStamp_included


