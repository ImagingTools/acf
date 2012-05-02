#ifndef icmpstr_CSceneProviderGuiComp_included
#define icmpstr_CSceneProviderGuiComp_included


// Qt includes
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneResizeEvent>
#include <QtGui/QGraphicsSceneWheelEvent>

// ACF includes
#include "i2d/ISceneController.h"

#include "ibase/ICommandsProvider.h"

#include "iqtgui/IDropConsumer.h"
#include "iqtgui/TDesignerGuiCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "icmpstr/ISceneProvider.h"
#include "icmpstr/Generated/ui_CSceneProviderGuiComp.h"


namespace icmpstr
{


class CSceneProviderGuiComp: 
			public iqtgui::TDesignerGuiCompBase<Ui::CSceneProviderGuiComp>,
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
		BM_DOT_GRID,

		/**
			Transparent background.
		*/
		BM_TRANSPARENT
	};

	I_BEGIN_COMPONENT(CSceneProviderGuiComp);
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
		I_ASSIGN(m_backgroundModeAttrPtr, "BackgroundMode", "Mode of background drawing:\n 0 - normal window\n 1 - solid color\n 2 - grid\n 3 - checkerboard\n 4 - dot grid\n 5 - transparent", true, 0);	
		I_ASSIGN(m_gridSizeAttrPtr, "GridSize", "Size of grid, it is used also for background", true, 20);	
		I_ASSIGN(m_isAlignmentEnabledAttrPtr, "IsAlignmentEnabled", "If true, grid alignment will be enabled", true, false);	
		I_ASSIGN(m_sceneWidthAttrPtr, "SceneWidth", "Logical with of scene", false, 1000);	
		I_ASSIGN(m_sceneHeightAttrPtr, "SceneHeight", "Logical height of scene", false, 1000);	
		I_ASSIGN(m_allowSelectAllAttrPtr, "AllowSelectAll", "If true, command 'Select All' will be enabled", true, false);	
	I_END_COMPONENT;

	CSceneProviderGuiComp();

	double GetIsotropyFactor() const;
	void SetIsotropyFactor(double factor);

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icmpstr::ISceneProvider)
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
		void DrawGrid(QPainter& painter, const QRectF& rect, bool useDot = false);

		// reimplemented (QGraphicsScene)
		virtual void drawBackground(QPainter* painter, const QRectF & rect);
		virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * eventPtr);
		virtual void dropEvent(QGraphicsSceneDragDropEvent * eventPtr);
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * eventPtr);

	private:
		CSceneProviderGuiComp& m_parent;
	};

	virtual bool OnResize(QResizeEvent* eventPtr);
	virtual bool OnWheelEvent(QGraphicsSceneWheelEvent* eventPtr);
	virtual bool OnKeyReleaseEvent(QKeyEvent* eventPtr);
	virtual bool OnMouseDoubleClickEvent(QEvent* eventPtr);

	void ScaleView(double scaleFactor);
	void SetFittedScale(FitMode mode);

	iqtgui::CHierarchicalCommand& GetCommandsRootRef();

	bool HasDropConsumerForFormat(const QStringList& formats) const;
	void DelegateDropEvent(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	void ResetScene();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

Q_SIGNALS:
	void zoomChanged(double);

protected Q_SLOTS:
	void OnPrint();
	void OnZoomIncrement();
	void OnZoomDecrement();
	void OnFitToView();
	void OnFitToShapes();
	void OnResetScale();
	void OnAutoFit(bool isAutoScale);
	void OnSelectAllShapes();

private:
	istd::TDelPtr<QGraphicsScene> m_scenePtr;
	
	FitMode m_fitMode;

	bool m_isFullScreenMode;
	bool m_isZoomIgnored;

	iqtgui::CHierarchicalCommand m_commands;
	iqtgui::CHierarchicalCommand m_fileMenu;
	iqtgui::CHierarchicalCommand m_printCommand;
	iqtgui::CHierarchicalCommand m_editMenu;
	iqtgui::CHierarchicalCommand m_selectAllCommand;
	iqtgui::CHierarchicalCommand m_viewMenu;
	iqtgui::CHierarchicalCommand m_autoFitToViewCommand;
	iqtgui::CHierarchicalCommand m_fitToViewCommand;
	iqtgui::CHierarchicalCommand m_fitToImageCommand;
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
	I_ATTR(bool, m_allowSelectAllAttrPtr);

	QWidget* m_savedParentWidgetPtr;
	QMatrix m_savedViewTransform;

	double m_isotropyFactor;
};


// inline protected methods

inline iqtgui::CHierarchicalCommand& CSceneProviderGuiComp::GetCommandsRootRef()
{
	return m_commands;
}


} // namespace icmpstr


#endif // !icmpstr_CSceneProviderGuiComp_included


