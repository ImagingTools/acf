#include "iqtgui/CFreeWidgetGuiComp.h"

#include <QMainWindow>
#include <QVBoxLayout>


namespace iqtgui
{


// reimplemented (icomp::IComponent)

void CFreeWidgetGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_slaveGuiCompPtr.IsValid()){
		m_mainWindowPtr.SetPtr(new QMainWindow);

		if (m_mainWindowPtr.IsValid()){
			new QVBoxLayout(m_mainWindowPtr.GetPtr());
			m_mainWindowPtr->setWindowTitle(iqt::GetQString(*m_titleAttrPtr));

			if (m_slaveGuiCompPtr->CreateGui(m_mainWindowPtr.GetPtr())){
				m_mainWindowPtr->show();
			}
		}
	}
}


void CFreeWidgetGuiComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_slaveGuiCompPtr.IsValid() && m_slaveGuiCompPtr->IsGuiCreated()){
		m_slaveGuiCompPtr->DestroyGui();
	}

	m_mainWindowPtr.Reset();
}


} // namespace iqtgui


