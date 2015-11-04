#include "iwidgets/CFocusDecorator.h"


namespace iwidgets
{


// public methods

CFocusDecorator::CFocusDecorator(QObject* parentPtr)
	:QObject(parentPtr)
{
}

CFocusDecorator::~CFocusDecorator()
{
	UnregisterAllWidgets();
}


bool CFocusDecorator::RegisterWidget(QWidget* widgetPtr, GraphicsEffectFactory* factoryPtr)
{
	if (widgetPtr == NULL){
		return false;
	}

	if (m_widgetEffectsMap.contains(widgetPtr)){
		return true;
	}

	widgetPtr->installEventFilter(this);

	m_widgetEffectsMap[widgetPtr] = factoryPtr;

	return true;
}


void CFocusDecorator::UnregisterWidget(QWidget* widgetPtr)
{
	if (widgetPtr == NULL){
		return;
	}

	Q_ASSERT(m_widgetEffectsMap.contains(widgetPtr));
	if (m_widgetEffectsMap.contains(widgetPtr)){
		widgetPtr->removeEventFilter(this);
	}

	m_widgetEffectsMap.remove(widgetPtr);

	widgetPtr->setGraphicsEffect(NULL);
}


void CFocusDecorator::UnregisterAllWidgets()
{
	while(!m_widgetEffectsMap.isEmpty()){
		QWidget* widgetPtr = m_widgetEffectsMap.begin().key();

		UnregisterWidget(widgetPtr);
	}
}


// protected methods

// reimplemented (QObject)

bool CFocusDecorator::eventFilter(QObject* objectPtr, QEvent* eventPtr)
{
	if (objectPtr != NULL && eventPtr != NULL){
		int eventType = eventPtr->type();
		switch (eventType){
			case QEvent::FocusIn:{
				QWidget* widgetPtr = dynamic_cast<QWidget*>(objectPtr);
				if (widgetPtr != NULL){
					GraphicsEffectFactory* factoryPtr = m_widgetEffectsMap.value(widgetPtr, NULL);
					if (factoryPtr != NULL){
						widgetPtr->setGraphicsEffect(factoryPtr->CreateInstance());
					}
				}
				break;
			}

			case QEvent::FocusOut:{
				QWidget* widgetPtr = dynamic_cast<QWidget*>(objectPtr);
				if (widgetPtr != NULL){
					widgetPtr->setGraphicsEffect(NULL);
				}
				break;
			}
		}
	}

	return QObject::eventFilter(objectPtr, eventPtr);
}


} // namespace iwidgets
