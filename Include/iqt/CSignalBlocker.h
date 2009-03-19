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
	CSignalBlocker(QObject* objectPtr, bool blockChilds = false);
	virtual ~CSignalBlocker();

private:
	void BlockChilds(QObject* objectPtr, bool block);

private:
	QObject* m_objectPtr;
	bool m_blockChilds;
};


} // namespace iqt


#endif // !iqt_CSignalBlocker_included

