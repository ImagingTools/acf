#ifndef iview_CConsoleGui_included
#define iview_CConsoleGui_included


// ACF includes
#include "iqtgui/CHierarchicalCommand.h"

#include "ibase/ICommandsProvider.h"

#include "iview/CConsoleBase.h"
#include "iview/CViewport.h"


// Foirward declarations
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
	Q_PROPERTY(bool GridInMm READ IsGridInMm WRITE SetGridInMm)
	Q_PROPERTY(bool ShowButtonPanel READ IsButtonsPanelVisible WRITE SetButtonsPanelVisible)
	Q_PROPERTY(bool ButtonPanelVertical READ IsButtonsPanelVertical WRITE SetButtonsPanelVertical)
	Q_PROPERTY(bool ScrollbarsVisible READ AreScrollbarsVisible WRITE SetScrollbarsVisible)
	Q_PROPERTY(bool StatusVisible READ IsStatusVisible WRITE SetStatusVisible)	
	Q_PROPERTY(bool ZoomButtonsVisible READ AreZoomsVisible WRITE SetZoomsVisible)	
	Q_PROPERTY(bool ZoomToFitButtonsVisible READ IsZoomToFitVisible WRITE SetZoomToFitVisible)	
	Q_PROPERTY(bool PolylineButtonsVisible READ ArePolylineButtonsVisible WRITE SetPolylineButtonsVisible)	
	Q_PROPERTY(bool UserModeButtonsVisible READ AreUserModeButtonsVisible WRITE SetUserModeButtonsVisible)	
	Q_PROPERTY(bool StatusButtonVisible READ IsStatusButtonVisible WRITE SetStatusButtonVisible)	
	Q_PROPERTY(bool ScrollbarsButtonVisible READ IsScrollbarsButtonVisible WRITE SetScrollbarsButtonVisible)	
	Q_PROPERTY(bool GridButtonVisible READ IsGridButtonVisible WRITE SetGridButtonVisible)	
	Q_PROPERTY(bool RulerButtonVisible READ IsRulerButtonVisible WRITE SetRulerButtonVisible)	

	Q_PROPERTY(bool MmButtonVisible READ IsMmButtonVisible WRITE SetMmButtonVisible)	
	Q_PROPERTY(bool PixelPositionVisible READ IsPixelPositionVisible WRITE SetPixelPositionVisible)	
	Q_PROPERTY(bool PixelValueVisible READ IsPixelValueVisible WRITE SetPixelValueVisible)	
	Q_PROPERTY(bool MmPositionVisible READ IsMmPositionVisible WRITE SetMmPositionVisible)	
	Q_PROPERTY(FitMode FitMode READ GetFitMode WRITE SetFitMode)

public:
	typedef iview::CConsoleBase BaseClass;

	enum CommandGroupId
	{
		CGI_ZOOM = ibase::ICommand::GI_USER + 1,
		CGI_SHAPE_EDITOR = ibase::ICommand::GI_USER + 2,
		CGI_CALIBRATION = ibase::ICommand::GI_USER + 3,
		CGI_VIEW_CONTROL = ibase::ICommand::GI_USER + 4
	};

	explicit CConsoleGui(QWidget* parent = NULL);

	// reimplemented (iview::CConsoleBase)
	virtual const CViewport& GetView() const;
	virtual CViewport& GetViewRef();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

public Q_SLOTS:
	virtual void OnZoomIn();
	virtual void OnZoomOut();
	virtual void OnZoomReset();
	virtual void OnZoomToFit(bool state);
	virtual void OnFitContentstoView();
	virtual void OnPointsNone();
	virtual void OnPoinsMove();
	virtual void OnPointsAdd();
	virtual void OnPointsSub();
	virtual void OnShowScrollbars(bool state);
	virtual void OnShowStatus(bool state);
	virtual void OnShowGrid(bool state);
	virtual void OnShowRuler(bool state);
	virtual void OnShowGridInMm(bool state);
	virtual void OnHScrollbarChanged(int newPos);
	virtual void OnVScrollbarChanged(int newPos);

Q_SIGNALS:
	bool selectionChanged(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state);
	bool mouseClicked(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr);

protected:
	void UpdateZoomInOutState();
	void UpdateScrollbarsValues();

	// reimplemented (iview::CConsoleBase)
	virtual void UpdateCursorInfo(const BaseClass::CursorInfo& info);
	virtual void UpdateEditModeButtons();
	virtual void UpdateButtonsState();
	virtual void UpdateComponentsPosition();
	virtual void UpdateCommands();
	virtual void SetStatusText(const QString& message);

	// events
	virtual bool OnSelectChange(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state);
	virtual bool OnMouseButton(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr);
	virtual void OnBoundingBoxChanged();

	// reimplemented Qt (QWidget)
	virtual void wheelEvent(QWheelEvent* event);

	iview::CViewport* m_viewPtr;

private:
	bool ConnectSignalSlots();

private:
	QVBoxLayout* m_mainLayoutPtr;
	QGridLayout* m_centerLayoutPtr;
	QHBoxLayout* m_statusLayoutPtr;

	QScrollBar* m_verticalScrollbarPtr;
	QScrollBar* m_horizontalScrollbarPtr;

	QStatusBar* m_statusBarPtr;
	QLabel* m_positionLabelPtr;
	QLabel* m_positionMmLabelPtr;
	QLabel* m_colorLabelPtr;

	// commands
	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_commands;

	iqtgui::CHierarchicalCommand m_gridVisibleCommand;
	iqtgui::CHierarchicalCommand m_rulerVisibleCommand;
	iqtgui::CHierarchicalCommand m_gridInMmVisibleCommand;
	iqtgui::CHierarchicalCommand m_scrollVisibleCommand;
	iqtgui::CHierarchicalCommand m_statusVisibleCommand;

	iqtgui::CHierarchicalCommand m_zoomInCommand;
	iqtgui::CHierarchicalCommand m_zoomOutCommand;
	iqtgui::CHierarchicalCommand m_zoomResetCommand;
	iqtgui::CHierarchicalCommand m_zoomToFitCommand;

	iqtgui::CHierarchicalCommand m_pointsSelectCommand;
	iqtgui::CHierarchicalCommand m_pointsMoveCommand;
	iqtgui::CHierarchicalCommand m_pointsAddCommand;
	iqtgui::CHierarchicalCommand m_pointsSubCommand;
};


// inline methods

// view access

inline const CViewport& CConsoleGui::GetView() const
{
	I_ASSERT(m_viewPtr != NULL);

	return *m_viewPtr;
}


inline CViewport& CConsoleGui::GetViewRef()
{
	I_ASSERT(m_viewPtr != NULL);

	return *m_viewPtr;
}


} // namespace iview


#endif // !iview_CConsoleGui_included



