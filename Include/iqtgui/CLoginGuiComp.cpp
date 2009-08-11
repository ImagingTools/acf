#include "iqtgui/CLoginGuiComp.h"


// Qt includes
#include <QMessageBox>


namespace iqtgui
{


// protected methods

void CLoginGuiComp::UpdateGui()
{
	if (m_userLoginCompPtr.IsValid()){
		std::string loggedUser = m_userLoginCompPtr->GetLoggedUser();
		if (!loggedUser.empty()){
			UserNameEdit->setText(loggedUser.c_str());
			UserNameEdit->setEnabled(false);
			PasswordFrame->setVisible(false);
			LogoutButton->setVisible(true);
			LoginButton->setVisible(false);
		}
		else{
			UserNameEdit->setEnabled(true);
			PasswordFrame->setVisible(true);
			LogoutButton->setVisible(false);
			LoginButton->setVisible(true);
		}

		LoginFrame->setVisible(true);
	}
	else{
		LoginFrame->setVisible(false);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CLoginGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	UpdateGui();
}


// protected slots

void CLoginGuiComp::on_LoginButton_clicked()
{
	if (m_userLoginCompPtr.IsValid()){
		if (!m_userLoginCompPtr->LoginUser(UserNameEdit->text().toStdString(), iqt::GetCString(PasswordEdit->text()))){
			QMessageBox::warning(GetWidget(), tr("Login Error") , tr("Wrong password or user name"));
		}

		UpdateGui();
	}
}


void CLoginGuiComp::on_LogoutButton_clicked()
{
	if (m_userLoginCompPtr.IsValid()){
		m_userLoginCompPtr->LogoutUser();

		UpdateGui();
	}
}


} // namespace iqtgui

