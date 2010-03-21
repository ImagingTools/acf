#include "iqtgui/CDialogGuiComp.h"


// Qt includes
#include <QIcon>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IDialog)

void CDialogGuiComp::Execute()
{
	if (!m_dialogPtr.IsValid()){
		return;
	}

	m_dialogPtr->exec();
}


// reimplemented (icomp::IComponent)

void CDialogGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_guiCompPtr.IsValid()){
		m_dialogPtr.SetPtr(
					new iqtgui::CGuiComponentDialog(
								m_guiCompPtr.GetPtr(),
								QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
								true));

		if (m_dialogTitleAttrPtr.IsValid()){
			m_dialogPtr->setWindowTitle(iqt::GetQString(*m_dialogTitleAttrPtr));
		}

		if (m_dialogIconPathAttrPtr.IsValid()){
			m_dialogPtr->setWindowIcon(QIcon(iqt::GetQString(*m_dialogIconPathAttrPtr)));
		}
	}
}


void CDialogGuiComp::OnComponentDestroyed()
{
	m_dialogPtr.Reset();

	BaseClass::OnComponentDestroyed();
}


} // namespace iqtgui


