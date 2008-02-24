#ifndef iqt_included
#define iqt_included


#include <QString>

#include "istd/CString.h"


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


} // namespace iqt


#endif // !iqt_included


