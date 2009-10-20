#ifndef iqt2d_CSceneControllerGuiComp_included
#define iqt2d_CSceneControllerGuiComp_included


#include "iqtgui/TDesignerGuiCompBase.h"


#include "i2d/ISceneController.h"
#include "iqt2d/ISceneProvider.h"
#include "iqt2d/Generated/ui_CSceneControllerGuiComp.h"


namespace iqt2d
{


class CSceneControllerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CSceneControllerGuiComp>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CSceneControllerGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CSceneControllerGuiComp);
		I_ASSIGN(m_sceneRestrictionsCompPtr, "SceneRestrictions", "Scene manipulation restrictions", false, "SceneRestrictions");
		I_ASSIGN(m_sceneProviderCompPtr, "SceneProvider", "Scene provider", true, "SceneProvider");
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected slots:
	void OnZoomIncrement();
	void OnZoomDecrement();
	void OnResetView();
	void OnRotateRight();
	void OnRotateLeft();

private:
	I_REF(i2d::ISceneController, m_sceneRestrictionsCompPtr);
	I_REF(iqt2d::ISceneProvider, m_sceneProviderCompPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CSceneControllerGuiComp_included


