#ifndef iqt_included
#define iqt_included


#include <QString>
#include <QStringList>
#include <QSize>
#include <QPoint>

#include "istd/CString.h"
#include "istd/CIndex2d.h"

#include "i2d/CVector2d.h"


/**
\defgroup iqt Qt Basics
\ingroup ACF
@{
*/

/** 
@} 
*/


/**
	This namespace contains standard classes based on Qt.
*/
namespace iqt
{


extern QString GetQString(const istd::CString& string);
extern istd::CString GetCString(const QString& string);
extern QStringList GetQStringList(const istd::CStringList& stringList);
extern istd::CStringList GetCStringList(const QStringList& stringList);
extern QSize GetQSize(const istd::CIndex2d& size);
extern istd::CIndex2d GetCIndex2d(const QSize& size);
extern QPoint GetQPoint(const istd::CIndex2d& position);
extern istd::CIndex2d GetCIndex2d(const QPoint& position);
extern QPointF GetQPointF(const i2d::CVector2d& position);
extern i2d::CVector2d GetCVector2d(const QPointF& position);


} // namespace iqt


#endif // !iqt_included


