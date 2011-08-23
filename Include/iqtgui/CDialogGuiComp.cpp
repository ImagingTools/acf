#include "iqtgui/CDialogGuiComp.h"


// Qt includes
#include <QIcon>


namespace iqtgui
{


// public methods

// reimplemented (iqtgui::IDialog)

void CDialogGuiComp::ExecuteDialog(IGuiObject* parentPtr)
{
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr(CreateComponentDialog(QDialogButtonBox::Close, parentPtr));
	if (dialogPtr.IsValid()){
		dialogPtr->exec();
	}
}


// protected methods

iqtgui::CGuiComponentDialog* CDialogGuiComp::CreateComponentDialog(int buttons, IGuiObject* parentPtr) const
{
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr;

	if (m_guiCompPtr.IsValid()){
		QWidget* parentWidgetPtr = (parentPtr != NULL)? parentPtr->GetWidget(): NULL;

		dialogPtr.SetPtr(
					new iqtgui::CGuiComponentDialog(
								m_guiCompPtr.GetPtr(),
								buttons,
								true,
								parentWidgetPtr));

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


