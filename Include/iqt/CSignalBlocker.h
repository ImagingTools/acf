#ifndef iqt_CSignalBlocker_included
#define iqt_CSignalBlocker_included


#include "iqt/iqt.h"


#include <QObject>


namespace iqt
{


/**
	Simple helper class blocking Qt signals of specified QObject.
*/
class CSignalBlocker
{
public:
	CSignalBlocker(QObject* objectPtr);
	virtual ~CSignalBlocker();

private:
	QObject* m_objectPtr;
};


} // namespace iqt


#endif // !iqt_CSignalBlocker_included

