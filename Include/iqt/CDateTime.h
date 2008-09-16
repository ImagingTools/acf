#ifndef iqt_CDateTime_included
#define iqt_CDateTime_included


#include <QDateTime>

#include "isys/CDateTimeBase.h"


namespace iqt
{


/**
	Implementation of isys::IDateTime interface based on Qt.
*/
class CDateTime: public isys::CDateTimeBase, public QDateTime
{
public:
	CDateTime();
	CDateTime(const CDateTime& time);
	CDateTime(const QDateTime& time);

	// reimplemented (isys::IDateTime)
	virtual int GetComponent(int component) const;
	virtual void SetComponent(int component, int value);
	virtual bool SetCurrentTime();
	virtual double ToCTime() const;
	virtual void FromCTime(double ctime);
};


} // namespace iqt


#endif // !iqt_CDateTime_included


