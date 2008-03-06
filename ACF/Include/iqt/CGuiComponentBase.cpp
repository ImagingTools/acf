#include "iqt/CGuiComponentBase.h"


#include <QLayout>


namespace iqt
{


CGuiComponentBase::CGuiComponentBase(const icomp::IComponentContext* contextPtr)
:	BaseClass(contextPtr), m_widgetPtr(NULL)
{
}


// reimplemented (iqt::IGuiObject)

bool CGuiComponentBase::IsGuiCreated() const
{
	return (m_widgetPtr != NULL);
}


bool CGuiComponentBase::CreateGui(QWidget* parentPtr, bool addToLayout)
{
	if (m_widgetPtr == NULL){
		m_widgetPtr = CreateWidget(parentPtr);

		if (m_widgetPtr != NULL){
			if (addToLayout && (parentPtr != NULL)){
				QLayout* layoutPtr = parentPtr->layout();

				if (layoutPtr != NULL){
					layoutPtr->addWidget(m_widgetPtr);
				}
			}

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


} // namespace iqt


