#ifndef iwidgets_CTreeWidgetFilter_included
#define iwidgets_CTreeWidgetFilter_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QTreeWidget>
#else
#include <QtGui/QTreeWidget>
#endif

#include <iwidgets/iwidgets.h>


namespace iwidgets
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
	explicit CTreeWidgetFilter(QTreeWidget* slavePtr);

protected:
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr) override;

private:
	QTreeWidget& m_slave;
};


} // namespace iwidgets


#endif // !iwidgets_CTreeWidgetFilter_included


