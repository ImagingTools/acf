#pragma once


// ACF includes
#include <ibase/ICommandsProvider.h>
#include <iqtgui/IGuiObject.h>
#include <iqtgui/TDesignerGuiCompBase.h>
#include <iqtgui/CHierarchicalCommand.h>
#include <iqt2d/IViewProvider.h>
#include <iqt2d/IViewExtender.h>
#include <iqtgui/TRestorableGuiWrap.h>
#include <GeneratedFiles/iqt2d/ui_CSceneConnectorGuiComp.h>


namespace iqt2d
{


class CSceneConnectorGuiComp:
			public iqtgui::TRestorableGuiWrap<iqtgui::TDesignerGuiCompBase<Ui::CSceneConnectorGuiComp, QWidget>>,
			virtual public ibase::ICommandsProvider
{
public:
	typedef iqtgui::TRestorableGuiWrap<iqtgui::TDesignerGuiCompBase<Ui::CSceneConnectorGuiComp, QWidget>> BaseClass;

	I_BEGIN_COMPONENT(CSceneConnectorGuiComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_sceneGuiCompPtr, "SceneGui", "GUI providing scene information", true, "SceneGui");
		I_ASSIGN_TO(m_sceneCommandsCompPtr, m_sceneGuiCompPtr, false);
		I_ASSIGN_TO(m_sceneProviderCompPtr, m_sceneGuiCompPtr, true);
		I_ASSIGN(m_extenderGuiCompPtr, "ExtenderGui", "Gui providing parameters displayed on scene", false, "ExtenderGui");
		I_ASSIGN_TO(m_extenderCommandsCompPtr, m_extenderGuiCompPtr, false);
		I_ASSIGN_TO(m_extenderCompPtr, m_extenderGuiCompPtr, true);
		I_ASSIGN(m_settingsKeyAttrPtr, "SettingsKey", "Key for saving/restoring of the layout information in the registry", false, "SettingsKey");
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const override;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	// reimplemented (TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings) override;
	virtual void OnSaveSettings(QSettings& settings) const override;

private:
	QString GetSettingsKey() const;

	I_REF(iqtgui::IGuiObject, m_sceneGuiCompPtr);
	I_REF(ibase::ICommandsProvider, m_sceneCommandsCompPtr);
	I_REF(IViewProvider, m_sceneProviderCompPtr);
	I_REF(iqtgui::IGuiObject, m_extenderGuiCompPtr);
	I_REF(ibase::ICommandsProvider, m_extenderCommandsCompPtr);
	I_REF(IViewExtender, m_extenderCompPtr);
	I_ATTR(QString, m_settingsKeyAttrPtr);

	mutable iqtgui::CHierarchicalCommand m_commands;
};


} // namespace iqt2d


