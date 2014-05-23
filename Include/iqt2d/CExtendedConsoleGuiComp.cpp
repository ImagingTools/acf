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


} // namespace iqt2d


