#include <iwidgets/CTreeWidgetFilter.h>


// Qt includes
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>


namespace iwidgets
{


CTreeWidgetFilter::CTreeWidgetFilter(QTreeWidget* slavePtr)
:	QObject(slavePtr),
	m_slave(*slavePtr)
{
	Q_ASSERT(slavePtr != NULL);

	m_slave.viewport()->installEventFilter(this);
}



// protected methods

// reimplemented (QObject)

bool CTreeWidgetFilter::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if (eventPtr->type() == QEvent::MouseButtonPress){
		QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(eventPtr);
		if (mouseEvent != NULL){
			QTreeWidgetItem* foundPtr = m_slave.itemAt(mouseEvent->pos());
			if (foundPtr == NULL){
				m_slave.clearSelection();
			}
		}
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


} // namespace iwidgets


