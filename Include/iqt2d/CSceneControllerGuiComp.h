#ifndef iqt2d_CSceneControllerGuiComp_included
#define iqt2d_CSceneControllerGuiComp_included


#include "iqtgui/TDesignerGuiCompBase.h"


#include "iqt2d/ISceneController.h"
#include "iqt2d/ISceneRestrictions.h"
#include "iqt2d/Generated/ui_CSceneControllerGuiComp.h"


namespace iqt2d
{


class CSceneControllerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CSceneControllerGuiComp>,
			virtual public iqt2d::ISceneController	
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CSceneControllerGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CSceneControllerGuiComp);
		I_REGISTER_INTERFACE(iqt2d::ISceneController);
		I_ASSIGN(m_sceneRestrictionsCompPtr, "SceneRestrictions", "Scene manipulation restrictions", false, "SceneRestrictions");		
	I_END_COMPONENT;

	CSceneControllerGuiComp();

	// reimplemented (iqt2d::ISceneController)
	virtual void RegisterSceneProvider(const ISceneProvider* sceneProviderPtr);

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
	const iqt2d::ISceneProvider* m_sceneProviderPtr;
	
	I_REF(iqt2d::ISceneRestrictions, m_sceneRestrictionsCompPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CSceneControllerGuiComp_included


