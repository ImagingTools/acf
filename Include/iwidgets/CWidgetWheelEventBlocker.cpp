#include <iwidgets/CWidgetWheelEventBlocker.h>

// Qt includes
#include <QtCore/QEvent>
#include <QtWidgets/QAbstractSpinBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QAbstractScrollArea>

// Acf includes
#include <istd/CChangeNotifier.h>
#include <istd/CChangeGroup.h>


namespace iwidgets
{


// public methods

CWidgetWheelEventBlocker::CWidgetWheelEventBlocker(int flags, QObject* parentPtr):
	QObject(parentPtr),
	m_processingFlags(flags)
{

}


int CWidgetWheelEventBlocker::GetProcessingFlags() const
{
	return m_processingFlags;
}


const CWidgetWheelEventBlocker::ObjectsPtrsList CWidgetWheelEventBlocker::GetWatchedWidgets() const
{
	return m_watchingWidgetPtrsList;
}


void CWidgetWheelEventBlocker::SetProcessingFlags(int flags)
{
	if (m_processingFlags != flags){
		istd::CChangeNotifier changeNotifier(this);

		m_processingFlags = flags;
	}
}


void CWidgetWheelEventBlocker::AddProcessingFlag(ProcessingFlags flag)
{
	if (!(m_processingFlags & flag)){
		istd::CChangeNotifier changeNotifier(this);

		m_processingFlags |= flag;
	}
}


void CWidgetWheelEventBlocker::AcquireChildScrollArea(QObject& parentObject)
{
	for (QAbstractScrollArea* childScrollAreaPtr : parentObject.findChildren<QAbstractScrollArea*>(QString(), Qt::FindChildrenRecursively)){
		AddSupportedChildWidgets(*childScrollAreaPtr);
	}
	QAbstractScrollArea* scrollAreaPtr = dynamic_cast<QAbstractScrollArea*>(&parentObject);
	if (scrollAreaPtr != nullptr){
		AddSupportedChildWidgets(*scrollAreaPtr);
	}
}

bool CWidgetWheelEventBlocker::AddChildrenWidgetsFromParent(QObject& parentObject)
{
	return AddSupportedChildWidgets(parentObject);
}


bool CWidgetWheelEventBlocker::AddWidgets(ObjectsPtrsList& widgetPtrsList)
{
	istd::CChangeGroup groupChanges(this);

	bool retVal = false;
	for (QObject* povidedWidgetPtr : widgetPtrsList){
		bool widgetAdded = false;
		if (m_processingFlags & PF_COMBO_BOXES){
			QComboBox* providedComboBoxPtr = dynamic_cast<QComboBox*>(povidedWidgetPtr);
			if (providedComboBoxPtr != nullptr){
				AcquireWidget(*providedComboBoxPtr);
				retVal = true;
				widgetAdded = true;
			}
		}

		if (!widgetAdded && (m_processingFlags & PF_SPIN_BOXES)){
			QAbstractSpinBox* providedSpinBoxPtr = dynamic_cast<QAbstractSpinBox*>(povidedWidgetPtr);
			if (providedSpinBoxPtr != nullptr){
				AcquireWidget(*providedSpinBoxPtr);
				retVal = true;
			}
		}
	}
	return retVal;
}


// reimplemented (QObject)

bool CWidgetWheelEventBlocker::eventFilter(QObject* objectPtr, QEvent* eventPtr)
{
	if (m_processingFlags & PF_COLLECT_CHILDREN){
		QWidget* widgetPtr = dynamic_cast<QWidget*>(objectPtr);
		if (widgetPtr != nullptr){
			QChildEvent* childEventPtr = dynamic_cast<QChildEvent*>(eventPtr);
			if (childEventPtr != nullptr){
				QObject* addedChildWidgetPtr = childEventPtr->child();
				if (addedChildWidgetPtr != nullptr){
					AddSupportedChildWidgets(*addedChildWidgetPtr);
				}
			}
		}
	}

	if (m_watchingWidgetPtrsList.contains(dynamic_cast<QWidget*>(objectPtr))){
		if (m_processingFlags & PF_COMBO_BOXES){
			QComboBox* comboBoxPtr = dynamic_cast<QComboBox*>(objectPtr);
			if (comboBoxPtr != nullptr && eventPtr->type() == QEvent::Wheel){
				return true;
			}
		}

		if (m_processingFlags & PF_SPIN_BOXES){
			QAbstractSpinBox* spinBoxPtr = dynamic_cast<QAbstractSpinBox*>(objectPtr);
			if (spinBoxPtr != nullptr){
				if (eventPtr->type() == QEvent::Show){
					spinBoxPtr->setFocusPolicy(Qt::StrongFocus);
				}
				else if (eventPtr->type() == QEvent::Wheel){
					if (!spinBoxPtr->hasFocus()){
						return true;
					}
				}
			}
		}
	}
	return BaseClass::eventFilter(objectPtr, eventPtr);
}


int CWidgetWheelEventBlocker::GetSupportedOperations() const
{
	return (BaseClass2::SO_OBSERVE | BaseClass2::SO_RESET);
}


bool CWidgetWheelEventBlocker::ResetData(CompatibilityMode mode)
{
	istd::CChangeNotifier changeNotifier(this);

	m_watchingWidgetPtrsList.clear();
	m_processingFlags = PF_NONE;
	return true;
}


bool CWidgetWheelEventBlocker::AddSupportedChildWidgets(QObject& parentObject)
{
	istd::CChangeGroup groupChanges(this);

	bool retVal = false;
	if (!m_watchingWidgetPtrsList.contains(&parentObject) && m_processingFlags & PF_COLLECT_CHILDREN){
		parentObject.installEventFilter(this);
		for (QObject* childObjectPtr : parentObject.findChildren<QObject*>(QString(), Qt::FindChildrenRecursively)){
			m_watchingWidgetPtrsList << childObjectPtr;
			AddSupportedChildWidgets(*childObjectPtr);
		}
	}

	if (m_processingFlags & PF_COMBO_BOXES){
		for (QComboBox* childComboBoxPtr : parentObject.findChildren<QComboBox*>(QString(), Qt::FindChildrenRecursively)){
			AcquireWidget(*childComboBoxPtr);
			retVal = true;
		}
	}

	if (m_processingFlags & PF_SPIN_BOXES){
		for (QAbstractSpinBox* childSpinBoxPtr : parentObject.findChildren<QAbstractSpinBox*>(QString(), Qt::FindChildrenRecursively)){
			AcquireWidget(*childSpinBoxPtr);
			retVal = true;
		}
	}
	return retVal;
}


void CWidgetWheelEventBlocker::AcquireWidget(QObject& widget)
{
	if (!m_watchingWidgetPtrsList.contains(&widget)){
		istd::CChangeNotifier changeNotifier(this);

		widget.installEventFilter(this);
		m_watchingWidgetPtrsList << &widget;
	}
}


} // namespace iwidgets

