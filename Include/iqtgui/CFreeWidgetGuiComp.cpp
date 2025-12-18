#include <iqtgui/CFreeWidgetGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QVBoxLayout>
#else
#include <QtGui/QVBoxLayout>
#endif


namespace iqtgui
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CFreeWidgetGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_slaveGuiCompPtr.IsValid()){
		m_mainWindowPtr.SetPtr(new QMainWindow);

		if (m_mainWindowPtr.IsValid()){
			QVBoxLayout* layoutPtr = new QVBoxLayout(m_mainWindowPtr.GetPtr());
			m_mainWindowPtr->setLayout(layoutPtr);
			m_mainWindowPtr->setWindowTitle(*m_titleAttrPtr);

			if (m_slaveGuiCompPtr->CreateGui(m_mainWindowPtr.GetPtr())){
				m_mainWindowPtr->setCentralWidget(m_slaveGuiCompPtr->GetWidget());
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


