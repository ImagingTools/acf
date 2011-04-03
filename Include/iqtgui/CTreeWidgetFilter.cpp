#include "iqtgui/CTreeWidgetFilter.h"


// Qt includes
#include <QEvent>
#include <QMouseEvent>


namespace iqtgui
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

bool CTreeWidgetFilter::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	switch (eventPtr->type()){
	case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(eventPtr);
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
		return BaseClass::eventFilter(sourcePtr, eventPtr);
	}
}


} // namespace iqtgui


