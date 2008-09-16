#ifndef iqt2d_CSceneProviderComp_included
#define iqt2d_CSceneProviderComp_included


#include <QGraphicsView>
#include <QGraphicsScene>


#include "imod/TSingleModelObserverBase.h"
#include "imod/IModel.h"

#include "iqt/TGuiComponentBase.h"
#include "iqt/TGuiObserverWrap.h"

#include "idoc/ICommandsProvider.h"

#include "iqt/CHierarchicalCommand.h"

#include "iqt2d/ISceneProvider.h"


namespace iqt2d
{


class CSceneProviderComp: 
			public iqt::TGuiComponentBase<QGraphicsView>,
			virtual public idoc::ICommandsProvider,
			virtual public ISceneProvider
{
	Q_OBJECT

public:
	typedef iqt::TGuiComponentBase<QGraphicsView> BaseClass;

	I_BEGIN_COMPONENT(CSceneProviderComp);
		I_REGISTER_INTERFACE(idoc::ICommandsProvider);
		I_REGISTER_INTERFACE(ISceneProvider);
		I_ASSIGN(m_allowWidgetResizeAttrPtr, "AllowWidgetResize", "Allow resize of QWidet object (should be disabled if this GUI size is managed by layout)", true, false);
		I_ASSIGN(m_sceneIdAttrPtr, "SceneId", "ID allowing identifying this scene", true, 0);
		I_ASSIGN(m_useAntialiasingAttrPtr, "UseAntialiasing", "Enables using of antialiasing", false, false);
	I_END_COMPONENT;

	enum FitMode{
		NoFit,
		ScaleToFit
	};

	CSceneProviderComp();

	void SetFitMode(FitMode fitMode);
	void SetFullScreenMode(bool isFullScreen);

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqt2d::ISceneProvider)
	virtual int GetSceneId() const;
	virtual QGraphicsScene* GetScene() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

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

	iqt::CHierarchicalCommand& GetCommandsRootRef();

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* obj, QEvent* event);

signals:
	void zoomChanged(double);

private:
	QGraphicsScene* m_scenePtr;
	
	FitMode m_fitMode;

	bool m_isFullScreenMode;
	bool m_isZoomIgnored;

	iqt::CHierarchicalCommand m_editorCommand;
	iqt::CHierarchicalCommand m_fitToViewCommand;
	iqt::CHierarchicalCommand m_resetZoomCommand;

	I_ATTR(bool, m_allowWidgetResizeAttrPtr);
	I_ATTR(int, m_sceneIdAttrPtr);
	I_ATTR(bool, m_useAntialiasingAttrPtr);
};


// inline protected methods

inline iqt::CHierarchicalCommand& CSceneProviderComp::GetCommandsRootRef()
{
	return m_editorCommand;
}


} // namespace iqt2d


#endif // !iqt2d_CSceneProviderComp_included


