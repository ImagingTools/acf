#ifndef iqt_CTreeWidgetFilter_included
#define iqt_CTreeWidgetFilter_included


#include <QTreeWidget>

#include "iqt/iqt.h"


namespace iqt
{


/**
	Provide some corrections in \c QTreeWidget object.
	Objects of this class are automatically registered by slave objects and should not be deleted manually.
	Please create instance of this class on heap.
*/
class CTreeWidgetFilter: public QObject
{
public:
	typedef QObject BaseClass;

	/**
		Constructor.
		\param	slavePtr	pointer to filtered slave tree object.
							It cannot be NULL.
	*/
	CTreeWidgetFilter(QTreeWidget* slavePtr);

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* obj, QEvent* event);

private:
	QTreeWidget& m_slave;
};


} // namespace iqt


#endif //!iqt_CTreeWidgetFilter_included


