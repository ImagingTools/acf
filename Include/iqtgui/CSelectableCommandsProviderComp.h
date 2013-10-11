#ifndef iqtgui_CSelectableCommandsProviderComp_included
#define iqtgui_CSelectableCommandsProviderComp_included



// ACF includes
#include "imod/TSingleModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "ibase/ICommandsProvider.h"
#include "iprm/ISelectionParam.h"
#include "iqtgui/IIconProvider.h"
#include "iqtgui/CHierarchicalCommand.h"


namespace iqtgui
{


/**
	Selectable commands provider component.
	Menu commands are generated from the selection constraints of the observered selection object.
*/
class CSelectableCommandsProviderComp:
			public QObject,
			public icomp::CComponentBase,
			protected imod::TSingleModelObserverBase<iprm::ISelectionParam>,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<iprm::ISelectionParam> BaseClass2;
	
	I_BEGIN_COMPONENT(CSelectableCommandsProviderComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_commandSelectionCompPtr, "CommandsSelection", "Commands selector and contstraints provider", true, "CommandsSelection");
		I_ASSIGN_TO(m_commandSelectionModelCompPtr, m_commandSelectionCompPtr, true);
		I_ASSIGN(m_actionIconsProviderCompPtr, "ActionIcons", "Icons for the region actions", false, "ActionIcons");
		I_ASSIGN(m_noSelectionCommandAttrPtr, "UnselectActionName", "The name of the unselect action", false, "UnselectActionName");
		I_ASSIGN(m_menuNameAttrPtr, "MenuName", "Name of the menu for the action group", true, "MenuName");
		I_ASSIGN(m_menuDescriptionAttrPtr, "MenuDescription", "Description for the action group", true, "MenuDescription");
		I_ASSIGN(m_rootMenuNameAttrPtr, "RootMenu", "Name of the root command", true, "RootMenu");
		I_ASSIGN(m_showInToolBarAttrPtr, "ShowInToolBar", "If enabled, the action will be shown in the application's tool bar", false, false);
	I_END_COMPONENT;

	CSelectableCommandsProviderComp();

	// reimpemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// reimpemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimpemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private Q_SLOTS:
	void OnCommandActivated();

protected:
	I_REF(iprm::ISelectionParam, m_commandSelectionCompPtr);
	I_REF(imod::IModel, m_commandSelectionModelCompPtr);
	I_REF(iqtgui::IIconProvider, m_actionIconsProviderCompPtr);
	I_ATTR(QString, m_noSelectionCommandAttrPtr);
	I_ATTR(QString, m_menuNameAttrPtr);
	I_ATTR(QString, m_menuDescriptionAttrPtr);
	I_ATTR(QString, m_rootMenuNameAttrPtr);
	I_ATTR(bool, m_showInToolBarAttrPtr);

	iqtgui::CHierarchicalCommand m_rootMenuCommand;
	iqtgui::CHierarchicalCommand m_mainMenuCommand;
	iqtgui::CHierarchicalCommand m_commandsList;
};


} // namespace iqtgui


#endif // !iqtgui_CSelectableCommandsProviderComp_included


