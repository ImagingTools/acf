#include "iqtgui/CDialogGuiComp.h"


// Qt includes
#include <QIcon>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IDialog)

void CDialogGuiComp::Execute()
{
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr(CreateComponentDialog());
	if (dialogPtr.IsValid()){
		dialogPtr->exec();
	}
}


// protected methods

iqtgui::CGuiComponentDialog* CDialogGuiComp::CreateComponentDialog() const
{
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr;

	if (m_guiCompPtr.IsValid()){
		dialogPtr.SetPtr(
					new iqtgui::CGuiComponentDialog(
								m_guiCompPtr.GetPtr(),
								QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
								true));

		if (m_dialogTitleAttrPtr.IsValid()){
			dialogPtr->setWindowTitle(iqt::GetQString(*m_dialogTitleAttrPtr));
		}

		if (m_dialogIconPathAttrPtr.IsValid()){
			dialogPtr->setWindowIcon(QIcon(iqt::GetQString(*m_dialogIconPathAttrPtr)));
		}
	}

	return dialogPtr.PopPtr();
}


} // namespace iqtgui


