#ifndef iqt_CSignalBlocker_included
#define iqt_CSignalBlocker_included


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <iqt/iqt.h>


namespace iqt
{


/**
	Simple helper class blocking Qt signals of specified QObject.
*/
class CSignalBlocker
{
public:

	/**
		Constructor.
		
		\param [in,out]	objectPtr	if non-null, pointer to the object to be blocked/unblocked.
		\param	blockChilds		 	(Optional) true to block the object's childs.
	*/
	CSignalBlocker(QObject* objectPtr, bool blockChilds = false);

	/**
		Destructor.
	*/
	virtual ~CSignalBlocker();

private:
	/**
		Block/unblock all childs.
		
		\param [in,out]	objectPtr	If non-null, pointer to the object to be blocked/unblocked.
		\param	block			 	true to block, false to unblock.
	*/
	void BlockChilds(QObject* objectPtr, bool block);

private:
	QObject* m_objectPtr;
	bool m_blockChilds;
};


} // namespace iqt


#endif // !iqt_CSignalBlocker_included

