#include "iqtgui/CDialogGuiComp.h"


// Qt includes
#include <QIcon>
#include <QApplication>


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
			dialogPtr->setWindowTitle(*m_dialogTitleAttrPtr);
		}
		else{
			dialogPtr->setWindowTitle(QCoreApplication::applicationName());
		}

		if (m_dialogIconPathAttrPtr.IsValid()){
			dialogPtr->setWindowIcon(QIcon(*m_dialogIconPathAttrPtr));
		}
		else{
			dialogPtr->setWindowIcon(QApplication::windowIcon());
		}
	}

	return dialogPtr.PopPtr();
}


} // namespace iqtgui


