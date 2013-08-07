#ifndef iqt2d_CSceneConnectorGuiComp_included
#define iqt2d_CSceneConnectorGuiComp_included


// ACF includes
#include "ibase/ICommandsProvider.h"
#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"
#include "iqt2d/IViewProvider.h"
#include "iqt2d/IViewExtender.h"

#include "iqt2d/Generated/ui_CSceneConnectorGuiComp.h"


namespace iqt2d
{


class CSceneConnectorGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CSceneConnectorGuiComp, QWidget>,
			virtual public ibase::ICommandsProvider
{
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CSceneConnectorGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CSceneConnectorGuiComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_sceneGuiCompPtr, "SceneGui", "Gui providing scene information", true, "SceneGui");
		I_ASSIGN_TO(m_sceneCommandsCompPtr, m_sceneGuiCompPtr, false);
		I_ASSIGN_TO(m_sceneProviderCompPtr, m_sceneGuiCompPtr, true);
		I_ASSIGN(m_extenderGuiCompPtr, "ExtenderGui", "Gui providing parameters displayed on scene", true, "ExtenderGui");
		I_ASSIGN_TO(m_extenderCommandsCompPtr, m_extenderGuiCompPtr, false);
		I_ASSIGN_TO(m_extenderCompPtr, m_extenderGuiCompPtr, true);
	I_END_COMPONENT;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iqtgui::IGuiObject, m_sceneGuiCompPtr);
	I_REF(ibase::ICommandsProvider, m_sceneCommandsCompPtr);
	I_REF(IViewProvider, m_sceneProviderCompPtr);
	I_REF(iqtgui::IGuiObject, m_extenderGuiCompPtr);
	I_REF(ibase::ICommandsProvider, m_extenderCommandsCompPtr);
	I_REF(IViewExtender, m_extenderCompPtr);

	mutable iqtgui::CHierarchicalCommand m_commands;
};


} // namespace iqt2d


#endif // !iqt2d_CSceneConnectorGuiComp_included


