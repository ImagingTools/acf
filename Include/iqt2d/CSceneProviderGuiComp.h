#ifndef iqt2d_CSceneProviderGuiComp_included
#define iqt2d_CSceneProviderGuiComp_included


// Qt includes
#include <QGraphicsView>
#include <QGraphicsScene>


#include "iqtgui/TDesignerGuiCompBase.h"

#include "idoc/ICommandsProvider.h"

#include "iqtgui/CHierarchicalCommand.h"

#include "iqt2d/ISceneProvider.h"
#include "iqt2d/ISceneController.h"
#include "iqt2d/ISceneRestrictions.h"

#include "iqt2d/Generated/Ui_CSceneProviderGuiComp.h"


namespace iqt2d
{


class CSceneProviderGuiComp: 
			public iqtgui::TDesignerGuiCompBase<Ui::CSceneProviderGuiComp>,
			virtual public idoc::ICommandsProvider,
			virtual public ISceneProvider,
			virtual public ISceneRestrictions
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CSceneProviderGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CSceneProviderGuiComp);
		I_REGISTER_INTERFACE(idoc::ICommandsProvider);
		I_REGISTER_INTERFACE(ISceneProvider);
		I_REGISTER_INTERFACE(ISceneRestrictions);
		I_ASSIGN(m_allowWidgetResizeAttrPtr, "AllowWidgetResize", "Allow resize of QWidet object (should be disabled if this GUI size is managed by layout)", true, false)
		I_ASSIGN(m_sceneIdAttrPtr, "SceneId", "ID allowing identifying this scene", true, 0)
		I_ASSIGN(m_useAntialiasingAttrPtr, "UseAntialiasing", "Enables using of antialiasing", false, false)
		I_ASSIGN(m_fitModeAttrPtr, "FitMode", "Set fit mode for the scene\n 0 - no fit\n 1 - scale contents to fit view area", false, false)		
		I_ASSIGN(m_sceneControllerCompPtr, "SceneController", "Scene controller", false, "SceneController");		
	I_END_COMPONENT;

	enum FitMode{
		NoFit,
		ScaleToFit
	};

	CSceneProviderGuiComp();

	void SetFitMode(FitMode fitMode);
	void SetFullScreenMode(bool isFullScreen);

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqt2d::ISceneProvider)
	virtual int GetSceneId() const;
	virtual QGraphicsScene* GetScene() const;

	// reimplemented (iqt2d::ISceneRestrictions)
	virtual int GetSceneRestrictionsFlags() const;

public slots:
	void SetZoom(double scaleFactor);
	void SetZoom(const QString& zoomString);
	void OnZoomIncrement();
	void OnZoomDecrement();
	void SwitchFullScreen();
	void OnFitToView();
	void OnFitToShapes();
	void OnResetScale();
	void OnAutoFit(bool isAutoScale);

protected:
	virtual void OnResize(QResizeEvent* event);
	virtual void OnWheelEvent(QGraphicsSceneWheelEvent* event);
	virtual void OnKeyReleaseEvent(QKeyEvent* event);
	virtual void OnContextMenuEvent(QContextMenuEvent* event);
	virtual void OnMouseDoubleClickEvent(QMouseEvent* event);
	virtual void OnMouseMoveEvent(QMouseEvent* event);

	void CreateContextMenu();
	void ScaleView(double scaleFactor);

	iqtgui::CHierarchicalCommand& GetCommandsRootRef();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* obj, QEvent* event);

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

	I_REF(iqt2d::ISceneController, m_sceneControllerCompPtr)

	I_ATTR(bool, m_allowWidgetResizeAttrPtr);
	I_ATTR(int, m_sceneIdAttrPtr);
	I_ATTR(bool, m_useAntialiasingAttrPtr);
	I_ATTR(int, m_fitModeAttrPtr);

	QWidget* m_savedParentWidgetPtr;
	QMatrix m_savedViewTransform;
};


// inline protected methods

inline iqtgui::CHierarchicalCommand& CSceneProviderGuiComp::GetCommandsRootRef()
{
	return m_editorCommand;
}


} // namespace iqt2d


#endif // !iqt2d_CSceneProviderGuiComp_included


