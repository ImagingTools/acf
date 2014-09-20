#include "iqt2d/CExtendedConsoleGuiComp.h"


namespace iqt2d
{


// protected methods

// reimplemented (CGuiComponentBase)

void CExtendedConsoleGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_viewProviderGuiCompPtr.IsValid()){
		m_viewProviderGuiCompPtr->CreateGui(ViewFrame);
	}

	if (m_toolBarCompPtr.IsValid()){
		m_toolBarCompPtr->CreateGui(CommandsFrame);

		if (*m_isFloatingToolbarAttr){
			CommandsFrame->setParent(ViewFrame);
			CommandsFrame->setFrameShape(QFrame::StyledPanel);
			CommandsFrame->setAutoFillBackground(true);
		}
	}
	else
		CommandsFrame->hide();

	if (m_statusInfoGuiCompPtr.IsValid()){
		m_statusInfoGuiCompPtr->CreateGui(InfoFrame);
	}
	else
		InfoFrame->hide();
}


void CExtendedConsoleGuiComp::OnGuiDestroyed()
{
	if (m_viewProviderGuiCompPtr.IsValid()){
		m_viewProviderGuiCompPtr->DestroyGui();
	}

	if (m_toolBarCompPtr.IsValid()){
		m_toolBarCompPtr->DestroyGui();
	}

	if (m_statusInfoGuiCompPtr.IsValid()){
		m_statusInfoGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (QObject)

bool CExtendedConsoleGuiComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if (eventPtr->type() == QEvent::Resize || eventPtr->type() == QEvent::Show){
		if (CommandsFrame->isVisible() && *m_isFloatingToolbarAttr){
			CommandsFrame->adjustSize();
			CommandsFrame->setMaximumWidth(ViewFrame->width());
		}
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


} // namespace iqt2d


