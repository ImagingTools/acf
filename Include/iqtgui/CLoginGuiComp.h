#ifndef iqtgui_CLoginGuiComp_included
#define iqtgui_CLoginGuiComp_included


#include "isec/IUserLogin.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "iqtgui/Generated/ui_CLoginGuiComp.h"


namespace iqtgui
{


class CLoginGuiComp: public TDesignerGuiCompBase<Ui::CLoginGuiComp, QWidget>
{
public:
	typedef TDesignerGuiCompBase<Ui::CLoginGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CLoginGuiComp);
		I_ASSIGN(m_userLoginCompPtr, "UserLogin", "User login object", true, "UserLogin");
	I_END_COMPONENT;

protected:
	void UpdateGui();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	I_REF(isec::IUserLogin, m_userLoginCompPtr);

protected slots:
	void on_LoginButton_clicked();
	void on_LogoutButton_clicked();
};


} // namespace iqtgui


#endif // !iqtgui_CLoginGuiComp_included

