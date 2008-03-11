#include "iqt/CGuiComponentBase.h"


#include <QLayout>
#include <QEvent>


namespace iqt
{


CGuiComponentBase::CGuiComponentBase()
:	m_widgetPtr(NULL)
{
}


// reimplemented (iqt::IGuiObject)

bool CGuiComponentBase::IsGuiCreated() const
{
	return (m_widgetPtr != NULL);
}


bool CGuiComponentBase::CreateGui(QWidget* parentPtr)
{
	if (m_widgetPtr == NULL){
		m_widgetPtr = InitWidgetToParent(parentPtr);

		if (m_widgetPtr != NULL){
			if (parentPtr != NULL){
				QLayout* layoutPtr = parentPtr->layout();

				if (layoutPtr != NULL){
					layoutPtr->addWidget(m_widgetPtr);
				}
			}

			m_widgetPtr->installEventFilter(this);

			metaObject()->connectSlotsByName(m_widgetPtr);

			OnGuiCreated();

			return true;
		}
	}

	return false;
}


bool CGuiComponentBase::DestroyGui()
{
	if (m_widgetPtr != NULL){
		OnGuiDestroyed();

		m_widgetPtr->deleteLater();

		m_widgetPtr = NULL;
	}

	return true;
}


void CGuiComponentBase::OnTryClose(bool* /*ignoredPtr*/)
{
}


// protected methods

void CGuiComponentBase::OnGuiShown()
{
}


void CGuiComponentBase::OnGuiHidden()
{
}


bool CGuiComponentBase::OnKeyPressed(QKeyEvent* /*event*/)
{
	return false;
}


void CGuiComponentBase::OnRetranslate()
{
}


void CGuiComponentBase::OnGuiCreated()
{
}


void CGuiComponentBase::OnGuiDestroyed()
{
}


// reimplemented (QObject)

bool CGuiComponentBase::eventFilter(QObject* obj, QEvent* event)
{
	QWidget* widgetPtr = GetWidget();
	if (obj != widgetPtr || widgetPtr == NULL){
		return false;
	}

	switch (event->type()){
		case QEvent::Close:
			bool ignoreClose;
			OnTryClose(&ignoreClose);
			if (ignoreClose){
				event->ignore();

				return true;
			}

		case QEvent::Show:
			OnGuiShown();
			break;

		case QEvent::Hide:
			OnGuiHidden();
			break;

		case QEvent::LanguageChange:
			OnRetranslate();
			break;
	}

	return false;
}


} // namespace iqt

