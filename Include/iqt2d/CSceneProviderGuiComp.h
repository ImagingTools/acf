#ifndef iqt2d_CSceneProviderGuiComp_included
#define iqt2d_CSceneProviderGuiComp_included


// Qt includes
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsSceneWheelEvent>


#include "iqtgui/TDesignerGuiCompBase.h"

#include "idoc/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"

#include "iqt2d/ISceneProvider.h"
#include "i2d/ISceneController.h"

#include "iqt2d/Generated/Ui_CSceneProviderGuiComp.h"


namespace iqt2d
{


class CSceneProviderGuiComp: 
			public iqtgui::TDesignerGuiCompBase<Ui::CSceneProviderGuiComp>,
			virtual public idoc::ICommandsProvider,
			virtual public ISceneProvider,
			virtual public i2d::ISceneController
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CSceneProviderGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CSceneProviderGuiComp);
		I_REGISTER_INTERFACE(idoc::ICommandsProvider);
		I_REGISTER_INTERFACE(ISceneProvider);
		I_REGISTER_INTERFACE(i2d::ISceneController);
		I_ASSIGN(m_allowWidgetResizeAttrPtr, "AllowWidgetResize", "Allow resize of QWidet object (should be disabled if this GUI size is managed by layout)", true, false)
		I_ASSIGN(m_sceneIdAttrPtr, "SceneId", "ID allowing identifying this scene", true, 0)
		I_ASSIGN(m_useAntialiasingAttrPtr, "UseAntialiasing", "Enables using of antialiasing", false, false)
		I_ASSIGN(m_fitModeAttrPtr, "FitMode", "Set fit automatic mode for the scene\n 0 - no fit (default)\n 1 - isotropic (reduction)\n 2 - isotropic\n 3 - anisotropic", false, 0)		
		I_ASSIGN(m_isotropyFactorAttrPtr, "IsotropyFactor", "Describe type of isotropic transformation: 0 - letterbox, 1 - full", true, 0)		
		I_ASSIGN(m_sceneControllerGuiCompPtr, "SceneController", "Scene controller", false, "SceneController");		
	I_END_COMPONENT;

	CSceneProviderGuiComp();

	double GetIsotropyFactor() const;
	void SetIsotropyFactor(double factor);

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqt2d::ISceneProvider)
	virtual int GetSceneId() const;
	virtual QGraphicsScene* GetScene() const;

	// reimplemented (i2d::ISceneController)
	virtual int GetSceneRestrictionFlags() const;
	virtual FitMode GetFitMode() const;
	virtual bool SetFitMode(FitMode fitMode);
	virtual bool IsFullScreenMode() const;
	virtual bool SetFullScreenMode(bool isFullScreen);
	virtual bool GetScale() const;
	virtual bool SetScale(int scaleMode = SM_SET, double value = 1.0);

public slots:
	void OnZoomIncrement();
	void OnZoomDecrement();
	void OnFitToView();
	void OnFitToShapes();
	void OnResetScale();
	void OnAutoFit(bool isAutoScale);

protected:
	virtual void OnResize(QResizeEvent* eventPtr);
	virtual void OnWheelEvent(QGraphicsSceneWheelEvent* eventPtr);
	virtual void OnKeyReleaseEvent(QKeyEvent* eventPtr);
	virtual void OnContextMenuEvent(QContextMenuEvent* eventPtr);
	virtual void OnMouseDoubleClickEvent(QMouseEvent* eventPtr);
	virtual void OnMouseMoveEvent(QMouseEvent* eventPtr);

	void CreateContextMenu();
	void ScaleView(double scaleFactor);
	void SetFittedScale(FitMode mode);

	iqtgui::CHierarchicalCommand& GetCommandsRootRef();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

signals:
	void zoomChanged(double);

private:
	QGraphicsScene* m_scenePtr;
	
	FitMode m_fitMode;

	bool m_isFullScreenMode;
	bool m_isZoomIgnored;

	iqtgui::CHierarchicalCommand m_editorCommand;
	iqtgui::CHierarchicalCommand m_fitToViewCommand;
	iqtgui::CHierarchicalCommand m_resetZoomCommand;

	I_REF(iqtgui::IGuiObject, m_sceneControllerGuiCompPtr)

	I_ATTR(bool, m_allowWidgetResizeAttrPtr);
	I_ATTR(int, m_sceneIdAttrPtr);
	I_ATTR(bool, m_useAntialiasingAttrPtr);
	I_ATTR(int, m_fitModeAttrPtr);
	I_ATTR(double, m_isotropyFactorAttrPtr);

	QWidget* m_savedParentWidgetPtr;
	QMatrix m_savedViewTransform;

	double m_isotropyFactor;
};


// inline protected methods

inline iqtgui::CHierarchicalCommand& CSceneProviderGuiComp::GetCommandsRootRef()
{
	return m_editorCommand;
}


} // namespace iqt2d


#endif // !iqt2d_CSceneProviderGuiComp_included


