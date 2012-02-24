#ifndef iqt_included
#define iqt_included


// Qt includes
#include <QString>
#include <QStringList>
#include <QSize>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QLine>
#include <QLineF>
#include <QFileInfo>
#include <QDateTime>


// ACF includes
#include "istd/CIndex2d.h"

#include "isys/CFileInfo.h"

#include "i2d/CVector2d.h"
#include "i2d/CRectangle.h"
#include "i2d/CLine2d.h"
#include "i2d/CRect.h"


/**
	This namespace contains standard classes based on Qt.
*/
namespace iqt
{


extern QSize GetQSize(const istd::CIndex2d& size);
extern istd::CIndex2d GetCIndex2d(const QSize& size);
extern QPoint GetQPoint(const istd::CIndex2d& position);
extern istd::CIndex2d GetCIndex2d(const QPoint& position);
extern QPointF GetQPointF(const i2d::CVector2d& position);
extern i2d::CVector2d GetCVector2d(const QPointF& position);
extern QRectF GetQRectF(const i2d::CRectangle& rect);
extern i2d::CRectangle GetCRectangle(const QRectF& rect);
extern i2d::CLine2d GetCLine2d(const QLine& line);
extern QLine GetQLine(const i2d::CLine2d& line);
extern i2d::CLine2d GetCLine2d(const QLineF& line);
extern QLineF GetQLineF(const i2d::CLine2d& line);
extern isys::CFileInfo GetCFileInfo(const QFileInfo& fileInfo);
extern QFileInfo GetQFileInfo(const isys::CFileInfo& fileInfo);
extern QDateTime GetQDateTime(const isys::IDateTime& dateTime);
extern isys::CSimpleDateTime GetCSimpleDateTime(const QDateTime& dateTime);
extern QRect GetQRect(const i2d::CRect& rect);
extern i2d::CRect GetCRect(const QRect& rect);


} // namespace iqt


#endif // !iqt_included


