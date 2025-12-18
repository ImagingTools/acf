#ifndef iqt_included
#define iqt_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSize>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtCore/QLine>
#include <QtCore/QLineF>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>

// ACF includes
#include <istd/CIndex2d.h>

#include <i2d/CVector2d.h>
#include <i2d/CRectangle.h>
#include <i2d/CLine2d.h>
#include <i2d/CRect.h>

#include <iqt/ITranslationManager.h>


/**
	This namespace contains non-UI utility classes based on Qt.
*/
namespace iqt
{


extern QSize GetQSize(const istd::CIndex2d& size);
extern istd::CIndex2d GetCIndex2d(const QSize& size);
extern QPoint GetQPoint(const istd::CIndex2d& position);
extern QPointF GetQPoint(const i2d::CVector2d& position);
extern istd::CIndex2d GetCIndex2d(const QPoint& position);
extern QRectF GetQRectF(const i2d::CRectangle& rect);
extern i2d::CRectangle GetCRectangle(const QRectF& rect);
extern i2d::CLine2d GetCLine2d(const QLine& line);
extern QLine GetQLine(const i2d::CLine2d& line);
extern i2d::CLine2d GetCLine2d(const QLineF& line);
extern QLineF GetQLineF(const i2d::CLine2d& line);
extern QRect GetQRect(const i2d::CRect& rect);
extern i2d::CRect GetCRect(const QRect& rect);

/**
	Function to get phrase tranlate.
	Check all connect translation manager
	and return translate when finds it.
	If not found, returns the original phrase.
*/
QByteArray GetTranslation(const iqt::ITranslationManager* translationManagerPtr, const QByteArray& phrase, const QByteArray& languageId, const QByteArray& context);

} // namespace iqt


#endif // !iqt_included


