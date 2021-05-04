#ifndef iview_CConsoleGui_included
#define iview_CConsoleGui_included


// ACF includes
#include <iqtgui/CHierarchicalCommand.h>
#include <ibase/ICommandsProvider.h>
#include <iview/CConsoleBase.h>
#include <iview/CViewport.h>
#include <iview/IShapeStatusInfo.h>


// Forward declarations
class QVBoxLayout;
class QGridLayout;
class QHBoxLayout;
class QScrollBar;
class QStatusBar;
class QLabel;


namespace iview
{


class CConsoleGui:
			public iview::CConsoleBase,
			public ibase::ICommandsProvider
{
	Q_OBJECT
	Q_ENUMS(FitMode)
	
	Q_PROPERTY(bool ZoomToFit READ IsZoomToFit WRITE SetZoomToFit)
	Q_PROPERTY(bool GridVisible READ IsGridVisible WRITE SetGridVisible)
	Q_PROPERTY(bool RulerVisible READ IsRulerVisible WRITE SetRulerVisible)
	Q_PROPERTY(bool DistanceMeasureToolActive READ IsDistanceMeasureToolActive WRITE SetDistanceMeasureToolActive)
	Q_PROPERTY(bool PointMeasureToolActive READ IsPointMeasureToolActive WRITE SetPointMeasureToolActive)
	Q_PROPERTY(bool GridInMm READ IsGridInMm WRITE SetGridInMm)
	Q_PROPERTY(bool ShowButtonPanel READ IsButtonsPanelVisible WRITE SetButtonsPanelVisible)
	Q_PROPERTY(bool ButtonPanelVertical READ IsButtonsPanelVertical WRITE SetButtonsPanelVertical)
	Q_PROPERTY(bool ScrollbarsVisible READ AreScrollbarsVisible WRITE SetScrollbarsVisible)
	Q_PROPERTY(bool ZoomButtonsVisible READ AreZoomsVisible WRITE SetZoomsVisible)	
	Q_PROPERTY(bool ZoomToFitButtonsVisible READ IsZoomToFitVisible WRITE SetZoomToFitVisible)	
	Q_PROPERTY(bool PolylineButtonsVisible READ ArePolylineButtonsVisible WRITE SetPolylineButtonsVisible)	
	Q_PROPERTY(bool UserModeButtonsVisible READ AreUserModeButtonsVisible WRITE SetUserModeButtonsVisible)	
	Q_PROPERTY(bool ScrollbarsButtonVisible READ IsScrollbarsButtonVisible WRITE SetScrollbarsButtonVisible)	
	Q_PROPERTY(bool GridButtonVisible READ IsGridButtonVisible WRITE SetGridButtonVisible)	
	Q_PROPERTY(bool RulerButtonVisible READ IsRulerButtonVisible WRITE SetRulerButtonVisible)	
	Q_PROPERTY(bool MmButtonVisible READ IsMmButtonVisible WRITE SetMmButtonVisible)	
	Q_PROPERTY(FitMode FitMode READ GetFitMode WRITE SetFitMode)

public:
	typedef iview::CConsoleBase BaseClass;

	enum CommandGroupId
	{
		CGI_ZOOM = ibase::ICommand::GI_USER + 1,
		CGI_SHAPE_EDITOR,
		CGI_CALIBRATION,
		CGI_VIEW_CONTROL,
		CGI_SHAPE_TOOLS
	};

	explicit CConsoleGui(QWidget* parent = NULL);

	/**
		Set status info object for the console shapes.
	*/
	void SetShapeStatusInfo(IShapeStatusInfo* shapeStatusInfoPtr);

	bool IsFullScreenMode() const;
	void SetFullScreenMode(bool isFullScreen);

	void RetranslateGui();

	// reimplemented (iview::CConsoleBase)
	virtual const CViewport& GetView() const;
	virtual CViewport& GetViewRef();
	virtual void UpdateCursorInfo(const QString& infoText);

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

public Q_SLOTS:
	void OnZoomIn();
	void OnZoomOut();
	void OnZoomReset();
	void OnZoomToFit(bool state);
	void OnFitContentsToView();
	void OnPointsNone();
	void OnPoinsMove();
	void OnPointsAdd();
	void OnPointsSub();
	void OnShowScrollbars(bool state);
	void OnShowGrid(bool state);
	void OnShowRuler(bool state);
	void OnActivateDistanceMeasureTool(bool state);
	void OnActivatePointMeasureTool(bool state);
	void OnShowGridInMm(bool state);
	void OnHScrollbarChanged(int newPos);
	void OnVScrollbarChanged(int newPos);
	void UpdateView();
	void OnStopFullScreen();

Q_SIGNALS:
	bool selectionChanged(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state);
	bool mouseClicked(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr);

protected:
	void UpdateZoomInOutState();
	void UpdateScrollbarsValues();

	// events
	virtual bool OnWheelEvent(QWheelEvent* eventPtr);
	virtual bool OnMouseDoubleClickEvent(QEvent* eventPtr);
	virtual bool OnKeyPressEvent(QKeyEvent* eventPtr);

	// reimplemented (iview::CConsoleBase)
	virtual void UpdateEditModeButtons();
	virtual void UpdateButtonsState();
	virtual void UpdateComponentsPosition();
	virtual void UpdateCommands();
	virtual bool OnSelectChange(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state);
	virtual bool OnViewMouseButton(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr);
	virtual void OnBoundingBoxChanged();
	
	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);	

	iview::CViewport* m_viewPtr;

private:
	bool ConnectSignalSlots();

	QVBoxLayout* m_mainLayoutPtr;
	QGridLayout* m_centerLayoutPtr;

	QScrollBar* m_verticalScrollbarPtr;
	QScrollBar* m_horizontalScrollbarPtr;

	// commands
	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_commands;

	iqtgui::CHierarchicalCommand m_gridVisibleCommand;
	iqtgui::CHierarchicalCommand m_rulerVisibleCommand;
	iqtgui::CHierarchicalCommand m_distanceMeasureToolCommand;
	iqtgui::CHierarchicalCommand m_pointMeasureToolCommand;
	iqtgui::CHierarchicalCommand m_gridInMmVisibleCommand;
	iqtgui::CHierarchicalCommand m_scrollVisibleCommand;

	iqtgui::CHierarchicalCommand m_zoomInCommand;
	iqtgui::CHierarchicalCommand m_zoomOutCommand;
	iqtgui::CHierarchicalCommand m_zoomResetCommand;
	iqtgui::CHierarchicalCommand m_zoomToFitCommand;

	iqtgui::CHierarchicalCommand m_pointsSelectCommand;
	iqtgui::CHierarchicalCommand m_pointsMoveCommand;
	iqtgui::CHierarchicalCommand m_pointsAddCommand;
	iqtgui::CHierarchicalCommand m_pointsSubCommand;

	IShapeStatusInfo* m_shapeStatusInfoPtr;

	bool m_isFullScreenMode;
	bool m_isViewMaximized;
	QWidget* m_viewWidget;	
	iview::CScreenTransform m_savedTransform;	
};


// inline methods

// view access

inline const CViewport& CConsoleGui::GetView() const
{
	Q_ASSERT(m_viewPtr != NULL);

	return *m_viewPtr;
}


inline CViewport& CConsoleGui::GetViewRef()
{
	Q_ASSERT(m_viewPtr != NULL);

	return *m_viewPtr;
}


} // namespace iview


#endif // !iview_CConsoleGui_included



