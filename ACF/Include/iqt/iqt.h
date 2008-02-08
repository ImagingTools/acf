#ifndef iqt_included
#define iqt_included


#include <QString>

#include "istd/CString.h"


/**
\defgroup iqt
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
extern istd::CString GetQString(const QString& string);


} // namespace iqt


#endif // !iqt_included


