#include "iqt/CTreeWidgetFilter.h"


#include <QEvent>
#include <QMouseEvent>


namespace iqt
{


CTreeWidgetFilter::CTreeWidgetFilter(QTreeWidget* slavePtr)
:	QObject(slavePtr),
	m_slave(*slavePtr)
{
	I_ASSERT(slavePtr != NULL);

	m_slave.viewport()->installEventFilter(this);
}



// protected methods

// reimplemented (QObject)

bool CTreeWidgetFilter::eventFilter(QObject* obj, QEvent* event)
{
	switch (event->type()){
	case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
			if (mouseEvent != NULL){
				QTreeWidgetItem* foundPtr = m_slave.itemAt(mouseEvent->pos());
				if (foundPtr == NULL){
					m_slave.clearSelection();

					return true;
				}
			}
		}

		return false;

	default:
		return BaseClass::eventFilter(obj, event);
	}
}


} // namespace iqt


