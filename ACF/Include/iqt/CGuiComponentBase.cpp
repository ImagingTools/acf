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

void CGuiComponentBase::AttachTo(QWidget* parentWidgetPtr, int layoutMargin)
{
	I_ASSERT(m_widgetPtr != NULL);

	// TODO: implement right hide/show logic to prevent the "flickering" after widget creation.

	if (m_widgetPtr != NULL){
		bool parentCreated = parentWidgetPtr->testAttribute(Qt::WA_WState_Created);
		bool thisCreated = m_widgetPtr->testAttribute(Qt::WA_WState_Created);
		bool thisHidden = m_widgetPtr->isHidden();

		QLayout* parentLayoutPtr = parentWidgetPtr->layout();
		if (parentLayoutPtr == NULL){
			parentLayoutPtr = new QGridLayout(parentWidgetPtr);
		}

		if (layoutMargin >= 0){
			parentLayoutPtr->setMargin(layoutMargin);
		}

		if (thisHidden && parentCreated && thisCreated){
			m_widgetPtr->setVisible(true);
		}

		parentLayoutPtr->addWidget(m_widgetPtr);
	}
}


void CGuiComponentBase::DetachFrom(QWidget* parentWidgetPtr)
{
	I_ASSERT(m_widgetPtr != NULL);

	if (m_widgetPtr != NULL){
		QLayout* parentLayoutPtr = parentWidgetPtr->layout();
		if (parentLayoutPtr != NULL){
			parentLayoutPtr->removeWidget(m_widgetPtr);
			
			m_widgetPtr->setHidden(true);
		}
	}
}


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
			break;

		case QEvent::Show:
			if (!m_isGuiShown){
				m_isGuiShown = true;
				OnGuiShown();
			}
			break;

		case QEvent::Hide:
			if (m_isGuiShown){
				m_isGuiShown = false;
				OnGuiHidden();
			}
			break;

		case QEvent::LanguageChange:
			OnRetranslate();
			break;
	}

	return false;
}


} // namespace iqt

