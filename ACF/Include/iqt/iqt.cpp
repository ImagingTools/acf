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


} // namespace iqt

