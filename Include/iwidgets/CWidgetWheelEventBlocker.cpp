#include <iwidgets/CWidgetWheelEventBlocker.h>

// Qt includes
#include <QtCore/QEvent>
#include <QtWidgets/QAbstractSpinBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QAbstractScrollArea>
#include <QtWidgets/QLayoutItem>

// Acf includes
#include <istd/CChangeNotifier.h>
#include <istd/CChangeGroup.h>


namespace iwidgets
{


// public methods

CWidgetWheelEventBlocker::CWidgetWheelEventBlocker(QWidget* parentPtr, int flags)
	:QObject(parentPtr),
	m_processingFlags(flags)
{
	AddSupportedChildWidgets(*parentPtr);
}


// protected methods

void CWidgetWheelEventBlocker::FilterWidgets(ObjectPtrList& widgetPtrsList)
{
	for (QObject* povidedWidgetPtr : widgetPtrsList){
		if (m_processingFlags & AW_COMBO_BOXES){
			QComboBox* providedComboBoxPtr = dynamic_cast<QComboBox*>(povidedWidgetPtr);
			if (providedComboBoxPtr != nullptr){
				AcquireWidget(*providedComboBoxPtr);
			}
		}

		if (m_processingFlags & AW_SPIN_BOXES){
			QAbstractSpinBox* providedSpinBoxPtr = dynamic_cast<QAbstractSpinBox*>(povidedWidgetPtr);
			if (providedSpinBoxPtr != nullptr){
				AcquireWidget(*providedSpinBoxPtr);
			}
		}
	}
}


// reimplemented (QObject)

bool CWidgetWheelEventBlocker::eventFilter(QObject* objectPtr, QEvent* eventPtr)
{
	int eventType = eventPtr->type();
	if (eventType != QEvent::Wheel && eventType != QEvent::ChildAdded){
		return QObject::eventFilter(objectPtr, eventPtr);
	}

	if (eventType == QEvent::ChildAdded){
		QList<QWidget*> children = objectPtr->findChildren<QWidget*>(QString(), Qt::FindChildrenRecursively);
		for (QWidget* childPtr: children){
			AddSupportedChildWidgets(*childPtr);
		}
	}
	else if (eventType == QEvent::Wheel){
		if (m_filteredWidgets.contains(dynamic_cast<QWidget*>(objectPtr))){
			if (m_processingFlags & AW_COMBO_BOXES){
				const QComboBox* comboBoxPtr = dynamic_cast<const QComboBox*>(objectPtr);
				if (comboBoxPtr != nullptr){
					return true;
				}
			}

			if (m_processingFlags & AW_SPIN_BOXES){
				const QAbstractSpinBox* spinBoxPtr = dynamic_cast<const QAbstractSpinBox*>(objectPtr);
				if (spinBoxPtr != nullptr){
					return true;
				}
			}
		}
	}

	return QObject::eventFilter(objectPtr, eventPtr);
}


void CWidgetWheelEventBlocker::AddSupportedChildWidgets(QWidget& parentObject)
{
	QComboBox* comboBoxPtr = dynamic_cast<QComboBox*>(&parentObject);
	QAbstractSpinBox* spinBoxPtr = dynamic_cast<QAbstractSpinBox*>(&parentObject);
	if (comboBoxPtr != nullptr){
		if (m_processingFlags & AW_COMBO_BOXES){
			AcquireWidget(*comboBoxPtr);
		}
	}
	else if(spinBoxPtr != nullptr){
		if (m_processingFlags & AW_SPIN_BOXES){
			AcquireWidget(*spinBoxPtr);
		}
	}
	else {
		// required for the child-added event
		parentObject.installEventFilter(this);

		if (m_processingFlags & AW_COMBO_BOXES){
			for (QComboBox* childComboBoxPtr : parentObject.findChildren<QComboBox*>(QString(), Qt::FindChildrenRecursively)){
				AcquireWidget(*childComboBoxPtr);
			}
		}

		if (m_processingFlags & AW_SPIN_BOXES){
			for (QAbstractSpinBox* childSpinBoxPtr : parentObject.findChildren<QAbstractSpinBox*>(QString(), Qt::FindChildrenRecursively)){
				AcquireWidget(*childSpinBoxPtr);
			}
		}
	}
}


void CWidgetWheelEventBlocker::AcquireWidget(QWidget& widget)
{
	widget.installEventFilter(this);
	m_filteredWidgets.insert(&widget);
}


} // namespace iwidgets


