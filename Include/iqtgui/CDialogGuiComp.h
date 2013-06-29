#ifndef iqtgui_CDialogGuiComp_included
#define iqtgui_CDialogGuiComp_included


// ACF includes
#include "ibase/ICommandsProvider.h"

#include "iqtgui/IDialog.h"
#include "iqtgui/CGuiComponentDialog.h"
#include "iqtgui/CHierarchicalCommand.h"


namespace iqtgui
{


/**
	Dialog based represenation of any UI-Component.
	This component provides also a menu command and can be integrated into a consumer of ibase::ICommandsProvider interface.
*/
class CDialogGuiComp:
			public QObject,
			public icomp::CComponentBase,
			virtual public iqtgui::IDialog,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CDialogGuiComp);
		I_REGISTER_INTERFACE(iqtgui::IDialog);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_guiCompPtr, "Gui", "UI to show in the dialog", true, "Gui");
		I_ASSIGN(m_dialogTitleAttrPtr, "DialogTitle", "Title for the dialog", false, "DialogTitle");
		I_ASSIGN(m_dialogIconPathAttrPtr, "DialogIconPath", "Icon path for the dialog", false, "IconPath");
		I_ASSIGN(m_menuNameAttrPtr, "MenuName", "Name of the menu for the action group", true, "MenuName");
		I_ASSIGN(m_menuDescriptionAttrPtr, "MenuDescription", "Description for the action group", true, "MenuDescription");
		I_ASSIGN(m_rootMenuNameAttrPtr, "RootMenu", "Name of the root command", true, "RootMenu");
		I_ASSIGN(m_initialDialogSizeAttrPtr, "DialogSize", "Initial size of the dialog. The value is proportion of dialog size to desktop size", false, 0.5);
	I_END_COMPONENT;

	// reimplemented (iqtgui::IDialog)
	virtual int ExecuteDialog(IGuiObject* parentPtr);

	// reimpemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	virtual iqtgui::CGuiComponentDialog* CreateComponentDialog(int buttons, IGuiObject* parentPtr) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

protected Q_SLOTS:
	void OnCommandActivated();

private:
	void SetInitialDialogSize(QDialog& dialog) const;

private:
	I_REF(iqtgui::IGuiObject, m_guiCompPtr);
	I_ATTR(QString, m_dialogTitleAttrPtr);
	I_ATTR(QString, m_dialogIconPathAttrPtr);
	I_ATTR(QString, m_menuNameAttrPtr);
	I_ATTR(QString, m_menuDescriptionAttrPtr);
	I_ATTR(QString, m_rootMenuNameAttrPtr);
	I_ATTR(double, m_initialDialogSizeAttrPtr);

	iqtgui::CHierarchicalCommand m_rootCommand;
	iqtgui::CHierarchicalCommand m_rootMenuCommand;
	iqtgui::CHierarchicalCommand m_dialogCommand;
};


} // namespace iqtgui


#endif // !iqtgui_CDialogGuiComp_included

