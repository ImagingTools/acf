#include "iqt/iqt.h"

#include <QStringList>


namespace iqt
{


QString GetQString(const istd::CString& string)
{
	return QString::fromStdWString(string);
}


istd::CString GetCString(const QString& string)
{
	return istd::CString(string.toStdWString());
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


} // namespace iqt

