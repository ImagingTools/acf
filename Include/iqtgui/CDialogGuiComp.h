#ifndef iqtgui_CDialogGuiComp_included
#define iqtgui_CDialogGuiComp_included


// ACF includes
#include <ibase/ICommandsProvider.h>
#include <imod/TModelWrap.h>
#include <iqtgui/IDialog.h>
#include <iqtgui/CGuiComponentDialog.h>
#include <iqtgui/CHierarchicalCommand.h>


namespace iqtgui
{


class CDialogGuiCompAttr: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CDialogGuiCompAttr);
		I_ASSIGN(m_initialDialogSizeAttrPtr, "DialogSize", "Initial size of the dialog. The value is proportion of dialog size to desktop size", false, 0.5);
		I_ASSIGN(m_initialDialogSizeXAttrPtr, "DialogSizeX", "Initial size of the dialog. The value is proportion of dialog size to desktop width", false, 0.5);
		I_ASSIGN(m_initialDialogSizeYAttrPtr, "DialogSizeY", "Initial size of the dialog. The value is proportion of dialog size to desktop height", false, 0.5);
	I_END_COMPONENT;

protected:
	I_ATTR(double, m_initialDialogSizeAttrPtr);
	I_ATTR(double, m_initialDialogSizeXAttrPtr);
	I_ATTR(double, m_initialDialogSizeYAttrPtr);
};


/**
	Dialog based representation of any UI-Component.
	This component provides also a menu command and can be integrated into a consumer of ibase::ICommandsProvider interface.
*/
class CDialogGuiComp:
			public QObject,
			public CDialogGuiCompAttr,
			virtual public iqtgui::IDialog
{
	Q_OBJECT
public:
	typedef CDialogGuiCompAttr BaseClass;
	typedef QObject BaseClass2;

	I_BEGIN_COMPONENT(CDialogGuiComp);
		I_REGISTER_INTERFACE(iqtgui::IDialog);
		I_REGISTER_SUBELEMENT(Command);
		I_REGISTER_SUBELEMENT_INTERFACE(Command, ibase::ICommandsProvider, GetCommandsProvider);
		I_REGISTER_SUBELEMENT_INTERFACE(Command, imod::IModel, GetCommandsProvider);
		I_REGISTER_SUBELEMENT_INTERFACE(Command, istd::IChangeable, GetCommandsProvider);
		I_ASSIGN(m_guiCompPtr, "Gui", "UI to show in the dialog", true, "Gui");
		I_ASSIGN(m_dialogTitleAttrPtr, "DialogTitle", "Title for the dialog", false, "DialogTitle");
		I_ASSIGN(m_dialogIconPathAttrPtr, "DialogIconPath", "Icon path for the dialog", false, "IconPath");
		I_ASSIGN(m_menuNameAttrPtr, "MenuName", "Name of the menu for the action group", true, "MenuName");
		I_ASSIGN(m_menuDescriptionAttrPtr, "MenuDescription", "Description for the action group", true, "");
		I_ASSIGN(m_rootMenuNameAttrPtr, "RootMenu", "Name of the root command", true, "");
		I_ASSIGN(m_isModalAttrPtr, "IsModal", "Modality of the dialog", true, true);
		I_ASSIGN(m_dialogButtonsAttrPtr, "DialogButtons", "Dialog buttons. See QDialogButtonBox::StandardButton for details", true, QDialogButtonBox::Ok);
		I_ASSIGN(m_defaultButtonAttrPtr, "DefaultDialogButton", "Default dialog button.See Qt::StandardButton for details", true, 0);
		I_ASSIGN(m_defaultButtonPropertyAttrPtr, "DefaultDialogButtonProperty", "Key of the boolean property that will be set to TRUE in context of the default button. Can be used for defining custom style of the primary action of the dialog", true, "DefaultButton");
		I_ASSIGN(m_windowFlagsAttrPtr, "WindowFlags", "Window flags is used to specify various window-system properties for the widget. See Qt::WindowFlags for details", true, 134295555);
	I_END_COMPONENT;

	CDialogGuiComp();

	// reimplemented (iqtgui::IDialog)
	virtual int ExecuteDialog(IGuiObject* parentPtr);

protected:
	virtual iqtgui::CGuiComponentDialog* CreateComponentDialog(int buttons, IGuiObject* parentPtr) const;
	virtual void OnRetranslate();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

protected Q_SLOTS:
	void OnCommandActivated();

private:
	class CommandsProvider: virtual public ibase::ICommandsProvider
	{
	public:
		CommandsProvider();

		void SetParent(CDialogGuiComp* parentPtr);

		// reimplemented (ibase::ICommandsProvider)
		virtual const ibase::IHierarchicalCommand* GetCommands() const;

	private:
		CDialogGuiComp* m_parentPtr;
	};


	template <class InterfaceType>
	static InterfaceType* GetCommandsProvider(CDialogGuiComp& parent)
	{
		return &parent.m_commandsProvider;
	}

private:
	I_REF(iqtgui::IGuiObject, m_guiCompPtr);
	I_TEXTATTR(m_dialogTitleAttrPtr);
	I_ATTR(QString, m_dialogIconPathAttrPtr);
	I_TEXTATTR(m_menuNameAttrPtr);
	I_TEXTATTR(m_menuDescriptionAttrPtr);
	I_TEXTATTR(m_rootMenuNameAttrPtr);
	I_ATTR(bool, m_isModalAttrPtr);
	I_ATTR(int, m_dialogButtonsAttrPtr);
	I_ATTR(int, m_defaultButtonAttrPtr);
	I_ATTR(QByteArray, m_defaultButtonPropertyAttrPtr);
	I_ATTR(int, m_windowFlagsAttrPtr);

	iqtgui::CHierarchicalCommand m_rootCommand;
	iqtgui::CHierarchicalCommand m_rootMenuCommand;
	iqtgui::CHierarchicalCommand m_dialogCommand;

	imod::TModelWrap<CommandsProvider> m_commandsProvider;

	QDialog* m_dialogPtr;
};


} // namespace iqtgui


#endif // !iqtgui_CDialogGuiComp_included

