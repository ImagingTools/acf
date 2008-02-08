#include "iqt/iqt.h"


namespace iqt
{


QString GetQString(const istd::CString& string)
{
	return QString::fromStdWString(string);
}


istd::CString GetQString(const QString& string)
{
	return istd::CString(string.toStdWString());
}


} // namespace iqt


