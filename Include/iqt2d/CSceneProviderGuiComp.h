#ifndef iqt2d_CSceneProviderGuiComp_included
#define iqt2d_CSceneProviderGuiComp_included


// Qt includes
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsSceneWheelEvent>


// ACF includes
#include "i2d/ISceneController.h"

#include "ibase/ICommandsProvider.h"

#include "iqtgui/IDropConsumer.h"
#include "iqtgui/TDesignerGuiCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqtdoc/IPrintable.h"

#include "iqt2d/ISceneProvider.h"
#include "iqt2d/Generated/Ui_CSceneProviderGuiComp.h"


namespace iqt2d
{


class CSceneProviderGuiComp: 
			public iqtgui::TDesignerGuiCompBase<Ui::CSceneProviderGuiComp>,
			virtual public iqtdoc::IPrintable,
			virtual public ibase::ICommandsProvider,
			virtual public ISceneProvider,
			virtual public i2d::ISceneController
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CSceneProviderGuiComp> BaseClass;

	enum BackgroundMode
	{
		/**
			Standard window backround.
		*/
		BM_NORMAL,

		/**
			Filled with solid color.
		*/
		BM_SOLID,

		/**
			Grid of horizontal and vertical lines.
		*/
		BM_GRID,

		/**
			Checkerboard pattern.
		*/
		BM_CHECKERBOARD,

		/**
			Dot grid.
		*/
		BM_DOT_GRID
	};

	I_BEGIN_COMPONENT(CSceneProviderGuiComp);
		I_REGISTER_INTERFACE(iqtdoc::IPrintable);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(ISceneProvider);
		I_REGISTER_INTERFACE(i2d::ISceneController);
		I_ASSIGN(m_allowWidgetResizeAttrPtr, "AllowWidgetResize", "Allow resize of QWidet object (should be disabled if this GUI size is managed by layout)", true, false);
		I_ASSIGN(m_sceneIdAttrPtr, "SceneId", "ID allowing identifying this scene", true, 0);
		I_ASSIGN(m_useAntialiasingAttrPtr, "UseAntialiasing", "Enables using of antialiasing", false, false);
		I_ASSIGN(m_fitModeAttrPtr, "FitMode", "Set fit automatic mode for the scene\n 0 - no fit (default)\n 1 - isotropic (reduction)\n 2 - isotropic\n 3 - anisotropic", false, 0);
		I_ASSIGN(m_isotropyFactorAttrPtr, "IsotropyFactor", "Describe type of isotropic transformation: 0 - letterbox, 1 - full", true, 0);
		I_ASSIGN(m_sceneControllerGuiCompPtr, "SceneController", "Scene controller", false, "SceneController");
		I_ASSIGN_MULTI_0(m_dropConsumersCompPtr, "DropConsumers", "List of consumers for the drop event", false);
		I_ASSIGN(m_backgroundModeAttrPtr, "BackgroundMode", "Mode of background drawing:\n 0 - normal window\n 1 - solid color\n 2 - grid\n 3 - checkerboard\n 4 - dot grid", true, 0);	
		I_ASSIGN(m_gridSizeAttrPtr, "GridSize", "Size of grid, it is used also for background", true, 20);	
		I_ASSIGN(m_isAlignmentEnabledAttrPtr, "IsAlignmentEnabled", "If true, grid alignment will be enabled", true, false);	
		I_ASSIGN(m_sceneWidthAttrPtr, "SceneWidth", "Logical with of scene", false, 1000);	
		I_ASSIGN(m_sceneHeightAttrPtr, "SceneHeight", "Logical height of scene", false, 1000);	
	I_END_COMPONENT;

	CSceneProviderGuiComp();

	double GetIsotropyFactor() const;
	void SetIsotropyFactor(double factor);

	// reimplemented (iqtdoc::IPrintable)
	virtual void Print(QPrinter* printerPtr) const;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqt2d::ISceneProvider)
	virtual int GetSceneId() const;
	virtual QGraphicsScene* GetScene() const;
	virtual bool GetSceneAlignment(double& distance) const;
	virtual int GetSceneFlags() const;

	// reimplemented (i2d::ISceneController)
	virtual int GetSceneRestrictionFlags() const;
	virtual FitMode GetFitMode() const;
	virtual bool SetFitMode(FitMode fitMode);
	virtual bool IsFullScreenMode() const;
	virtual bool SetFullScreenMode(bool isFullScreen);
	virtual double GetScale() const;
	virtual bool SetScale(int scaleMode = SM_SET, double value = 1.0);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

public slots:
	void OnZoomIncrement();
	void OnZoomDecrement();
	void OnFitToView();
	void OnFitToShapes();
	void OnResetScale();
	void OnAutoFit(bool isAutoScale);

protected:
	/**
		Implementation of QGraphicsScene with own background drawing to provide rastered backgrounds.
	*/
	class CScene: public QGraphicsScene
	{
	public:
		typedef QGraphicsScene BaseClass;

		CScene(CSceneProviderGuiComp* parentPtr);

	protected:
		void DrawGrid(QPainter* painter, const QRectF& rect, bool useDot = false);

		// reimplemented (QGraphicsScene)
		virtual void drawBackground(QPainter* painter, const QRectF & rect);
		virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * eventPtr);
		virtual void dropEvent(QGraphicsSceneDragDropEvent * eventPtr);
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * eventPtr);

	private:
		CSceneProviderGuiComp& m_parent;
	};

	virtual void OnResize(QResizeEvent* eventPtr);
	virtual void OnWheelEvent(QGraphicsSceneWheelEvent* eventPtr);
	virtual void OnKeyReleaseEvent(QKeyEvent* eventPtr);
	virtual void OnContextMenuEvent(QContextMenuEvent* eventPtr);
	virtual void OnMouseDoubleClickEvent(QEvent* eventPtr);
	virtual void OnMouseMoveEvent(QMouseEvent* eventPtr);

	void CreateContextMenu();
	void ScaleView(double scaleFactor);
	void SetFittedScale(FitMode mode);

	iqtgui::CHierarchicalCommand& GetCommandsRootRef();

	bool HasDropConsumerForFormat(const QStringList& formats) const;
	void DelegateDropEvent(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);

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

	I_REF(iqtgui::IGuiObject, m_sceneControllerGuiCompPtr);
	I_MULTIREF(iqtgui::IDropConsumer, m_dropConsumersCompPtr);

	I_ATTR(bool, m_allowWidgetResizeAttrPtr);
	I_ATTR(int, m_sceneIdAttrPtr);
	I_ATTR(bool, m_useAntialiasingAttrPtr);
	I_ATTR(int, m_fitModeAttrPtr);
	I_ATTR(double, m_isotropyFactorAttrPtr);
	I_ATTR(int, m_backgroundModeAttrPtr);
	I_ATTR(double, m_gridSizeAttrPtr);
	I_ATTR(bool, m_isAlignmentEnabledAttrPtr);
	I_ATTR(double, m_sceneWidthAttrPtr);
	I_ATTR(double, m_sceneHeightAttrPtr);

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


