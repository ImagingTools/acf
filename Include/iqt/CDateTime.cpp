#include "iqt/CDateTime.h"


namespace iqt
{


CDateTime::CDateTime()
{
	*this = currentDateTime();
}


CDateTime::CDateTime(const CDateTime& time)
:	QDateTime(time)
{
}


CDateTime::CDateTime(const QDateTime& time)
:	QDateTime(time)
{
}


// reimplemented (isys::IDateTime)

int CDateTime::GetComponent(int component) const
{
	switch (component){
	case TC_YEAR:
		return date().year();

	case TC_MONTH:
		return date().month();

	case TC_DAY:
		return date().day();

	case TC_HOUR:
		return time().hour();

	case TC_MINUTE:
		return time().minute();

	case TC_SECOND:
		return time().second();

	case TC_MICROSECOND:
		return time().msec();

	default:
		I_CRITICAL();

		return 0;
	};
}


void CDateTime::SetComponent(int component, int value)
{
	switch (component){
	case TC_YEAR:
		{
			QDate date = this->date();
			setDate(QDate(value, date.month(), date.day()));
		}
		break;

	case TC_MONTH:
		{
			QDate date = this->date();
			setDate(QDate(date.year(), value, date.day()));
		}
		break;

	case TC_DAY:
		{
			QDate date = this->date();
			setDate(QDate(date.year(), date.month(), value));
		}
		break;

	case TC_HOUR:
		{
			QTime time = this->time();
			setTime(QTime(value, time.minute(), time.second(), time.msec()));
		}
		break;

	case TC_MINUTE:
		{
			QTime time = this->time();
			setTime(QTime(time.hour(), value, time.second(), time.msec()));
		}
		break;

	case TC_SECOND:
		{
			QTime time = this->time();
			setTime(QTime(time.hour(), time.minute(), value, time.msec()));
		}
		break;

	case TC_MICROSECOND:
		{
			QTime time = this->time();
			setTime(QTime(time.hour(), time.minute(), time.second(), value));
		}
		break;

	default:
		I_CRITICAL();

		break;
	};
}


bool CDateTime::SetCurrentTime()
{
	*this = currentDateTime();

	return true;
}


double CDateTime::ToCTime() const
{
	return toTime_t() + (time().msec() * 0.001);
}


void CDateTime::FromCTime(double ctime)
{
	unsigned ctimeValue = unsigned(ctime);
	setTime_t(uint(ctime));
	QDateTime::operator=(addMSecs(qint64((ctime - ctimeValue) * 1000)));
}


} // namespace iqt


