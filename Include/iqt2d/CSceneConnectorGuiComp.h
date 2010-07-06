#ifndef iqt2d_CSceneConnectorGuiComp_included
#define iqt2d_CSceneConnectorGuiComp_included


#include "ibase/ICommandsProvider.h"

#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqt2d/ISceneProvider.h"
#include "iqt2d/ISceneExtender.h"
#include "iqt2d/Generated/ui_CSceneConnectorGuiComp.h"


namespace iqt2d
{


class CSceneConnectorGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CSceneConnectorGuiComp, QWidget>,
			virtual public ibase::ICommandsProvider
{
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CSceneConnectorGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CSceneConnectorGuiComp)
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_sceneGuiCompPtr, "SceneGui", "Gui providing scene information", true, "SceneGui");
		I_ASSIGN(m_sceneCommandsCompPtr, "SceneGui", "Gui providing scene information", false, "SceneGui");
		I_ASSIGN(m_sceneProviderCompPtr, "SceneGui", "Gui providing scene information", true, "SceneGui");
		I_ASSIGN(m_extenderGuiCompPtr, "ExtenderGui", "Gui providing parameters displayed on scene", true, "ExtenderGui");
		I_ASSIGN(m_extenderCommandsCompPtr, "ExtenderGui", "Gui providing parameters displayed on scene", false, "ExtenderGui");
		I_ASSIGN(m_extenderCompPtr, "ExtenderGui", "Gui providing parameters displayed on scene", true, "ExtenderGui");
	I_END_COMPONENT

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_sceneGuiCompPtr);
	I_REF(ibase::ICommandsProvider, m_sceneCommandsCompPtr);
	I_REF(ISceneProvider, m_sceneProviderCompPtr);
	I_REF(iqtgui::IGuiObject, m_extenderGuiCompPtr);
	I_REF(ibase::ICommandsProvider, m_extenderCommandsCompPtr);
	I_REF(ISceneExtender, m_extenderCompPtr);

	mutable iqtgui::CHierarchicalCommand m_commands;
};


} // namespace iqt2d


#endif // !iqt2d_CSceneConnectorGuiComp_included


