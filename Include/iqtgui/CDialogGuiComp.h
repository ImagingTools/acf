#ifndef iqtgui_CDialogGuiComp_included
#define iqtgui_CDialogGuiComp_included


// ACF includes
#include "iqtgui/IDialog.h"
#include "iqtgui/CGuiComponentDialog.h"


namespace iqtgui
{


/**
	Dialog based represenation of any UI-Component.
*/
class CDialogGuiComp: public icomp::CComponentBase, virtual public iqtgui::IDialog
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CDialogGuiComp)
		I_REGISTER_INTERFACE(iqtgui::IDialog);
		I_ASSIGN(m_guiCompPtr, "Gui", "UI to show in the dialog", true, "Gui");
		I_ASSIGN(m_dialogTitleAttrPtr, "DialogTitle", "Title for the dialog", false, "DialogTitle");
		I_ASSIGN(m_dialogIconPathAttrPtr, "DialogIconPath", "Icon path for the dialog", false, "IconPath");
	I_END_COMPONENT

	// reimplemented (iqtgui::IDialog)
	virtual void Execute();

protected:
	virtual iqtgui::CGuiComponentDialog* CreateComponentDialog(int buttons) const;

private:
	I_REF(iqtgui::IGuiObject, m_guiCompPtr);
	I_ATTR(istd::CString, m_dialogTitleAttrPtr);
	I_ATTR(istd::CString, m_dialogIconPathAttrPtr);
};


} // namespace iqtgui


#endif // !iqtgui_CDialogGuiComp_included

