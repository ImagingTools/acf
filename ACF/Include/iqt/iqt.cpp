#include "iqt/iqt.h"

#include <QStringList>


namespace iqt
{


QString GetQString(const istd::CString& string)
{
#ifdef NO_QT_STL_SUPPORT
	return QString::fromUtf16((const I_WORD*)string.c_str());
#else
	return QString::fromStdWString(string);
#endif
}


istd::CString GetCString(const QString& string)
{
#ifdef NO_QT_STL_SUPPORT
	return istd::CString((const wchar_t*)string.utf16());
#else
	return istd::CString(string.toStdWString());
#endif
}


QStringList GetQStringList(const istd::CStringList& stringList)
{
	QStringList output;

	for (		istd::CStringList::const_iterator begin = stringList.begin(); 
				begin != stringList.end(); 
				begin++){
		
		output.push_back(GetQString(*begin));
	}

	return output;
}


istd::CStringList GetCStringList(const QStringList& stringList)
{
	istd::CStringList output;

	for (		QStringList::const_iterator begin = stringList.begin(); 
				begin != stringList.end(); 
				begin++){

		output.push_back(GetCString(*begin));
	}

	return output;
}


QSize GetQSize(const istd::CIndex2d& size)
{
	return QSize(size.GetX(), size.GetY());
}


istd::CIndex2d GetCIndex2d(const QSize& size)
{
	return istd::CIndex2d(size.width(), size.height());
}


QPoint GetQPoint(const istd::CIndex2d& position)
{
	return QPoint(position.GetX(), position.GetY());
}


istd::CIndex2d GetCIndex2d(const QPoint& position)
{
	return istd::CIndex2d(position.x(), position.y());
}


QPointF GetQPointF(const i2d::CVector2d& position)
{
	return QPointF(position.GetX(), position.GetY());
}


i2d::CVector2d GetCVector2d(const QPointF& position)
{
	return i2d::CVector2d(position.x(), position.y());
}


QRectF GetQRectF(const i2d::CRectangle& rect)
{
	return QRectF(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
}


i2d::CRectangle GetCRectangle(const QRectF& rect)
{
	return i2d::CRectangle(rect.left(), rect.top(), rect.width(), rect.height());
}


i2d::CLine2d GetCLine2d(const QLine& line)
{
	return i2d::CLine2d(line.x1(), line.y1(), line.x2(), line.y2());
}


QLine GetQLine(const i2d::CLine2d& line)
{
	return QLine(line.GetPoint1().GetX(), line.GetPoint1().GetY(), line.GetPoint2().GetX(), line.GetPoint2().GetY());
}


i2d::CLine2d GetCLine2d(const QLineF& line)
{
	return i2d::CLine2d(line.x1(), line.y1(), line.x2(), line.y2());
}


QLineF GetQLineF(const i2d::CLine2d& line)
{
	return QLineF(line.GetPoint1().GetX(), line.GetPoint1().GetY(), line.GetPoint2().GetX(), line.GetPoint2().GetY());
}



} // namespace iqt

