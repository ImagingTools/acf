#ifndef iqt2d_CSceneConnectorGuiComp_included
#define iqt2d_CSceneConnectorGuiComp_included


#include "iqt/IGuiObject.h"
#include "iqt/TDesignerGuiCompBase.h"

#include "iqt2d/ISceneProvider.h"
#include "iqt2d/ISceneExtender.h"

#include "iqt2d/Generated/ui_CSceneConnectorGuiComp.h"


namespace iqt2d
{


class CSceneConnectorGuiComp: public iqt::TDesignerGuiCompBase<Ui::CSceneConnectorGuiComp, QWidget>
{
public:
	typedef iqt::TDesignerGuiCompBase<Ui::CSceneConnectorGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CSceneConnectorGuiComp)
		I_ASSIGN(m_sceneGuiCompPtr, "SceneGui", "Gui providing scene information", true, "SceneGui");
		I_ASSIGN(m_sceneProviderCompPtr, "SceneGui", "Gui providing scene information", true, "SceneGui");
		I_ASSIGN(m_extenderGuiCompPtr, "ExtenderGui", "Gui providing parameters displayed on scene", true, "ExtenderGui");
		I_ASSIGN(m_extenderCompPtr, "ExtenderGui", "Gui providing parameters displayed on scene", true, "ExtenderGui");
	I_END_COMPONENT

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(iqt::IGuiObject, m_sceneGuiCompPtr);
	I_REF(ISceneProvider, m_sceneProviderCompPtr);
	I_REF(iqt::IGuiObject, m_extenderGuiCompPtr);
	I_REF(ISceneExtender, m_extenderCompPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CSceneConnectorGuiComp_included


