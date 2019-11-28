#include <iview/CConsoleGui.h>


// STL includes
#include <cmath>

// Qt includes
#include <QtCore/QDebug>
#include <QtGui/QWheelEvent>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFrame>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStatusBar>
#else
#include <QtGui/QFrame>
#include <QtGui/QToolButton>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QScrollBar>
#include <QtGui/QStatusBar>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iwidgets/CWidgetUpdateBlocker.h>
#include <iview/IInteractiveShape.h>
#include <iview/CInteractiveShapeBase.h>


namespace iview
{


CConsoleGui::CConsoleGui(QWidget* parent)
:	BaseClass(parent),
	m_commands("&View", 100),
	m_gridVisibleCommand("Show Grid", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_ONOFF, CGI_CALIBRATION),
	m_rulerVisibleCommand("Show Ruler", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_ONOFF, CGI_CALIBRATION),
	m_distanceToolCommand("Distance", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_ONOFF, CGI_CALIBRATION),
	m_gridInMmVisibleCommand("Grid in Millimeter", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_ONOFF, CGI_CALIBRATION),
	m_scrollVisibleCommand("Show Scrollbars", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_ONOFF, CGI_VIEW_CONTROL),
	m_zoomInCommand("Zoom In", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR, CGI_ZOOM),
	m_zoomOutCommand("Zoom Out", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR, CGI_ZOOM),
	m_zoomResetCommand("Reset Zoom", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR, CGI_ZOOM),
	m_zoomToFitCommand("Fit Contents To View", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_ONOFF, CGI_ZOOM),
	m_pointsSelectCommand("Selection Mode", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_EXCLUSIVE | ibase::ICommand::CF_ONOFF, CGI_SHAPE_EDITOR),
	m_pointsMoveCommand("Modify Mode", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_EXCLUSIVE | ibase::ICommand::CF_ONOFF, CGI_SHAPE_EDITOR),
	m_pointsAddCommand("Add Points", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_EXCLUSIVE | ibase::ICommand::CF_ONOFF, CGI_SHAPE_EDITOR),
	m_pointsSubCommand("Remove Points", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_EXCLUSIVE | ibase::ICommand::CF_ONOFF, CGI_SHAPE_EDITOR),
	m_shapeStatusInfoPtr(NULL),
	m_isFullScreenMode(false),
	m_isViewMaximized(false)
{
	m_viewWidget = new QWidget(this);

	m_viewPtr = new iview::CViewport(this, m_viewWidget);

	m_mainLayoutPtr = new QVBoxLayout(m_viewWidget);
	m_centerLayoutPtr = new QGridLayout(m_viewWidget);

	m_verticalScrollbarPtr = new QScrollBar(Qt::Vertical, m_viewWidget);
	m_verticalScrollbarPtr->setTracking(false);
	m_horizontalScrollbarPtr = new QScrollBar(Qt::Horizontal, m_viewWidget);
	m_horizontalScrollbarPtr->setTracking(false);

	// main layout
	m_mainLayoutPtr->addLayout(m_centerLayoutPtr);
	m_mainLayoutPtr->setMargin(0);

	m_centerLayoutPtr->addWidget(m_viewPtr, 0, 0);
	m_centerLayoutPtr->addWidget(m_verticalScrollbarPtr, 0, 1);
	m_centerLayoutPtr->addWidget(m_horizontalScrollbarPtr, 1, 0);
	m_centerLayoutPtr->setMargin(0);
	m_centerLayoutPtr->setSpacing(1);

	QSizePolicy emptyPolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored);
	QSizePolicy buttonPolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QSizePolicy expandingPolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	m_viewPtr->setSizePolicy(expandingPolicy);

	//	languageChange();
	resize(QSize(604, 480).expandedTo(minimumSizeHint()));

	UpdateEditModeButtons();
	UpdateButtonsState();
	UpdateComponentsPosition();
	UpdateCommands();

	ConnectSignalSlots();

	m_viewPtr->installEventFilter(this);
	m_viewWidget->installEventFilter(this);

	//Add widget view wrapper - to persist layout when switching from full screen
	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->setMargin(0);
	m_viewWidget->setParent(this);
	vLayout->addWidget(m_viewWidget);
}


void CConsoleGui::SetShapeStatusInfo(IShapeStatusInfo* shapeStatusInfoPtr)
{
	m_shapeStatusInfoPtr = shapeStatusInfoPtr;
}


bool CConsoleGui::IsFullScreenMode() const
{
	return m_isFullScreenMode;
}


void CConsoleGui::SetFullScreenMode(bool fullScreenMode)
{
	if (!IsFullScreenAllowed()){
		if (!m_isFullScreenMode){
			return;
		}

		// force fullscreen to be off
		fullScreenMode = false;
	}

	if (fullScreenMode != m_isFullScreenMode){
		m_isFullScreenMode = fullScreenMode;

		if (m_isFullScreenMode){
			m_horizontalScrollbarPtr->setVisible(false);
			m_verticalScrollbarPtr->setVisible(false);

			m_savedTransform = m_viewPtr->GetTransform();
			m_isViewMaximized = isMaximized();

			layout()->removeWidget(m_viewWidget);

			m_viewWidget->setParent(NULL);
			Qt::WindowFlags flags = m_viewWidget->windowFlags();
			m_viewWidget->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
			m_viewWidget->showFullScreen();

			// center image on the screen
			m_viewPtr->SetZoom(iview::CViewBase::ZM_FIT);
			m_viewPtr->Update();
		}
		else{
			Qt::WindowFlags flags = m_viewWidget->windowFlags();
			m_viewWidget->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);

			m_viewWidget->showNormal();

			m_viewWidget->setParent(this);
			layout()->addWidget(m_viewWidget);

			if (m_isViewMaximized){
				showMaximized();
			}

			m_viewPtr->SetTransform(m_savedTransform);
			m_viewPtr->Update();
		}
	}
}


void CConsoleGui::RetranslateGui()
{
	UpdateCommands();
}


// reimplemented (iview::CConsoleBase)

void CConsoleGui::UpdateCursorInfo(const QString& infoText)
{
	if (m_shapeStatusInfoPtr != NULL){
		istd::CChangeNotifier statusNotifier(m_shapeStatusInfoPtr);

		m_shapeStatusInfoPtr->SetInfoText(infoText);
	}
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CConsoleGui::GetCommands() const
{
	return &m_rootCommands;
}


// public slots

void CConsoleGui::OnZoomIn()
{
	m_viewPtr->SetZoom(iview::CViewBase::ZM_ZOOM_IN);

	UpdateZoomInOutState();
}


void CConsoleGui::OnZoomOut()
{
	m_viewPtr->SetZoom(iview::CViewBase::ZM_ZOOM_OUT);

	UpdateZoomInOutState();
}


void CConsoleGui::OnZoomReset()
{
	m_viewPtr->SetZoom(iview::CViewBase::ZM_RESET);

	UpdateZoomInOutState();
}


void CConsoleGui::OnZoomToFit(bool state)
{
	SetZoomToFit(state);

	UpdateZoomInOutState();
}


void CConsoleGui::OnFitContentsToView()
{
	m_viewPtr->SetZoom(iview::CViewBase::ZM_FIT);

	UpdateZoomInOutState();
}


void CConsoleGui::OnPointsNone()
{
	m_viewPtr->SetEditMode(iview::ISelectable::EM_NONE);
}


void CConsoleGui::OnPoinsMove()
{
	m_viewPtr->SetEditMode(iview::ISelectable::EM_MOVE);
}


void CConsoleGui::OnPointsAdd()
{
	m_viewPtr->SetEditMode(iview::ISelectable::EM_ADD);
}


void CConsoleGui::OnPointsSub()
{
	m_viewPtr->SetEditMode(iview::ISelectable::EM_REMOVE);
}


void CConsoleGui::OnShowScrollbars(bool state)
{
	SetScrollbarsVisible(state);
}


void CConsoleGui::OnShowGrid(bool state)
{
	SetGridVisible(state);

	m_viewPtr->Update();
}


void CConsoleGui::OnShowRuler(bool state)
{
	SetRulerVisible(state);
}


void CConsoleGui::OnActivateDistanceTool(bool state)
{
	SetDistanceToolActive(state);
}


void CConsoleGui::OnShowGridInMm(bool state)
{
	SetGridInMm(state);

	m_viewPtr->Update();
}


void CConsoleGui::OnHScrollbarChanged(int newPos)
{
	if (m_horizontalScrollbarPtr->updatesEnabled()){
		if (newPos < 0){
			newPos = 0;
		}
		else if (newPos > m_horizontalScrollbarPtr->maximum()){
			newPos = m_horizontalScrollbarPtr->maximum();
		}
		i2d::CRect bbox = m_viewPtr->GetBoundingBox();
		i2d::CRect clientRect = m_viewPtr->GetClientRect();
		i2d::CRect wholeRect = bbox.GetUnion(clientRect);
		i2d::CAffine2d transform = m_viewPtr->GetTransform();
		i2d::CVector2d position = transform.GetTranslation();
		position.SetX(position.GetX() + clientRect.GetLeft() - wholeRect.GetLeft() - newPos);
		transform.SetTranslation(position);
		m_viewPtr->SetTransform(transform);
		m_viewPtr->Update();
	}
}


void CConsoleGui::OnVScrollbarChanged(int newPos)
{
	if (m_verticalScrollbarPtr->updatesEnabled()){
		if (newPos < 0){
			newPos = 0;
		}
		else if (newPos > m_verticalScrollbarPtr->maximum()){
			newPos = m_verticalScrollbarPtr->maximum();
		}
		i2d::CRect bbox = m_viewPtr->GetBoundingBox();
		i2d::CRect clientRect = m_viewPtr->GetClientRect();
		i2d::CRect wholeRect = bbox.GetUnion(clientRect);
		i2d::CAffine2d transform = m_viewPtr->GetTransform();
		i2d::CVector2d position = transform.GetTranslation();
		position.SetY(position.GetY() + clientRect.GetTop() - wholeRect.GetTop() - newPos);
		transform.SetTranslation(position);
		m_viewPtr->SetTransform(transform);
		m_viewPtr->Update();
	}
}


void CConsoleGui::UpdateView()
{
	BaseClass::UpdateView();

	int selectedShapesCount = GetView().GetSelectedShapesCount();

	bool isSelected = (selectedShapesCount > 0);
	m_pointsSelectCommand.setEnabled(isSelected);
	m_pointsMoveCommand.setEnabled(isSelected);
	m_pointsAddCommand.setEnabled(isSelected);
	m_pointsSubCommand.setEnabled(isSelected);
}


// protected methods

void CConsoleGui::UpdateZoomInOutState()
{
	if (IsZoomToFit()){
		m_zoomInCommand.SetEnabled(false);
		m_zoomOutCommand.SetEnabled(false);
	}
	else{
		const i2d::CAffine2d& transform = m_viewPtr->GetTransform();
		double actualScale = transform.GetDeformMatrix().GetFrobeniusNorm() / ::sqrt(2.0);
		m_zoomInCommand.SetEnabled(actualScale < 100);
		m_zoomOutCommand.SetEnabled(actualScale > 0.01);
	}
}


void CConsoleGui::UpdateScrollbarsValues()
{
	iwidgets::CWidgetUpdateBlocker hScrollBlocker(m_horizontalScrollbarPtr);
	iwidgets::CWidgetUpdateBlocker vScrollBlocker(m_verticalScrollbarPtr);

	i2d::CRect bbox = m_viewPtr->GetBoundingBox();
	i2d::CRect clientRect = m_viewPtr->GetClientRect();
	i2d::CRect wholeRect = bbox.GetUnion(clientRect);
	if (!wholeRect.IsEmpty()){
		const int minX = 0;
		const int maxX = wholeRect.GetWidth() - clientRect.GetWidth();

		if (maxX > 0){
			int posX = clientRect.GetLeft() - wholeRect.GetLeft();
			if (posX < 0){
				posX = 0;
			}
			else if (posX > maxX){
				posX = maxX;
			}
			int pageX = clientRect.GetWidth();

			m_horizontalScrollbarPtr->setEnabled(true);
			m_horizontalScrollbarPtr->setMinimum(minX);
			m_horizontalScrollbarPtr->setMaximum(maxX);
			m_horizontalScrollbarPtr->setValue(posX);
			m_horizontalScrollbarPtr->setSingleStep(pageX / 8 + 1);
			m_horizontalScrollbarPtr->setPageStep(pageX);
		}
		else{
			m_horizontalScrollbarPtr->setEnabled(false);
			m_horizontalScrollbarPtr->setMinimum(0);
			m_horizontalScrollbarPtr->setMaximum(0);
			m_horizontalScrollbarPtr->setValue(0);
			m_horizontalScrollbarPtr->setSingleStep(1);
			m_horizontalScrollbarPtr->setPageStep(1);
		}

		const int minY = 0;
		const int maxY = wholeRect.GetHeight() - clientRect.GetHeight();
		if (maxY > 0){
			int posY = clientRect.GetTop() - wholeRect.GetTop();
			if (posY < 0){
				posY = 0;
			}
			else if (posY > maxY){
				posY = maxY;
			}

			int pageY = clientRect.GetHeight();

			m_verticalScrollbarPtr->setEnabled(true);
			m_verticalScrollbarPtr->setMinimum(minY);
			m_verticalScrollbarPtr->setMaximum(maxY);
			m_verticalScrollbarPtr->setValue(posY);
			m_verticalScrollbarPtr->setSingleStep(pageY / 8 + 1);
			m_verticalScrollbarPtr->setPageStep(pageY);
		}
		else{
			m_verticalScrollbarPtr->setEnabled(false);
			m_verticalScrollbarPtr->setMinimum(0);
			m_verticalScrollbarPtr->setMaximum(0);
			m_verticalScrollbarPtr->setValue(0);
			m_verticalScrollbarPtr->setSingleStep(1);
			m_verticalScrollbarPtr->setPageStep(1);
		}

		//Set scrollbar visibility in full screen mode
		if(IsFullScreenMode()){
			m_horizontalScrollbarPtr->setVisible(m_horizontalScrollbarPtr->isEnabled());
			m_verticalScrollbarPtr->setVisible(m_verticalScrollbarPtr->isEnabled());
		}
	}
}


bool CConsoleGui::OnWheelEvent(QWheelEvent* eventPtr)
{
	if ((m_viewPtr == NULL) || IsZoomToFit()){
		return true;
	}

	static const double mouseWheelZoomStep = 0.25;
	static const double minZoomScale = 0.01;
	static const double maxZoomScale = 250;

	iview::CScreenTransform transform = m_viewPtr->GetTransform();
	const double actualScale = transform.GetDeformMatrix().GetFrobeniusNorm();
	const double factor = mouseWheelZoomStep * eventPtr->delta() / 120.0;

	if (		((actualScale < minZoomScale) && (factor < 0)) || 
				((actualScale > maxZoomScale) && (factor > 0))){
		return true;
	}

	const double scale = pow(2.0, factor);
	const istd::CIndex2d screenPos = iqt::GetCIndex2d(eventPtr->pos());
	const i2d::CVector2d logPos = transform.GetClientPosition(screenPos);

	i2d::CAffine2d zoomTransform;
	zoomTransform.Reset(logPos * (1 - scale), 0, scale);

	transform.Apply(zoomTransform);

	m_viewPtr->SetTransform(transform);

	UpdateZoomInOutState();

	return true;
}


bool CConsoleGui::OnMouseDoubleClickEvent(QEvent* /*eventPtr*/)
{
	SetFullScreenMode(!IsFullScreenMode());

	return true;
}


bool CConsoleGui::OnKeyPressEvent(QKeyEvent* eventPtr)
{
	switch(eventPtr->key()){

	case Qt::Key_Escape:
		if (IsFullScreenMode()){
			SetFullScreenMode(false);

			return true;
		}
		break;
	}

	return false;
}


// reimplemented (iview::CConsoleBase)

void CConsoleGui::UpdateEditModeButtons()
{
	int mode = m_viewPtr->GetEditMode();

	m_pointsSelectCommand.setChecked(mode == iview::ISelectable::EM_NONE);
	m_pointsMoveCommand.setChecked(mode == iview::ISelectable::EM_MOVE);
	m_pointsAddCommand.setChecked(mode == iview::ISelectable::EM_ADD);
	m_pointsSubCommand.setChecked(mode == iview::ISelectable::EM_REMOVE);
}


void CConsoleGui::UpdateButtonsState()
{
	UpdateZoomInOutState();

	const iview::IViewLayer& gridLayer = m_viewPtr->GetCalibrationLayer();
	bool isGridLayerActive = !gridLayer.GetBoundingBox().IsEmpty();

	m_zoomToFitCommand.setChecked(IsZoomToFit());
	m_zoomToFitCommand.SetEnabled(IsBackgroundActive());
	m_zoomResetCommand.SetEnabled(!IsZoomToFit());

	m_scrollVisibleCommand.SetEnabled(!IsZoomToFit());
	m_scrollVisibleCommand.setChecked(AreScrollbarsVisible());
	m_gridVisibleCommand.SetEnabled(isGridLayerActive);
	m_gridVisibleCommand.setChecked(IsGridVisible());
	m_rulerVisibleCommand.setChecked(IsRulerVisible());
	m_distanceToolCommand.setChecked(IsDistanceToolActive());
	m_gridInMmVisibleCommand.setChecked(IsGridInMm());
}


void CConsoleGui::UpdateComponentsPosition()
{
	iwidgets::CWidgetUpdateBlocker frameBlocker(this);
	iwidgets::CWidgetUpdateBlocker viewBlocker(m_viewPtr);

	// scroll bars
	if (m_isFullScreenMode){
		UpdateScrollbarsValues();
	}
	else{
		bool isScrollHVisible = false;
		bool isScrollVVisible = false;

		if (AreScrollbarsVisible()){
			bool isZoomToFit = IsZoomToFit();
			FitMode fitMode = GetFitMode();
			isScrollHVisible = !isZoomToFit || (fitMode == FM_VERTICAL);
			isScrollVVisible = !isZoomToFit || (fitMode == FM_HORIZONTAL);

			UpdateScrollbarsValues();
		}

		m_horizontalScrollbarPtr->setVisible(isScrollHVisible);
		m_verticalScrollbarPtr->setVisible(isScrollVVisible);
	}


	//	TODO: Implement rulers and mm button if m_isRulerVisible == true!

	// buttons
	bool areButtonsVisible = IsButtonsPanelVisible();

	// TODO: Implement vertical buttons panel if m_isButtonsPanelVertical is true;
	bool zoomVisible = areButtonsVisible && AreZoomsVisible();
	m_zoomInCommand.setVisible(zoomVisible);
	m_zoomOutCommand.setVisible(zoomVisible);
	m_zoomResetCommand.setVisible(zoomVisible);
	m_zoomToFitCommand.setVisible(zoomVisible && IsZoomToFitVisible());

	bool polyVisible = areButtonsVisible && ArePolylineButtonsVisible();
	m_pointsSelectCommand.setVisible(polyVisible);
	m_pointsMoveCommand.setVisible(polyVisible);
	m_pointsAddCommand.setVisible(polyVisible);
	m_pointsSubCommand.setVisible(polyVisible);

	m_scrollVisibleCommand.setVisible(areButtonsVisible && IsScrollbarsButtonVisible());
	m_gridVisibleCommand.setVisible(areButtonsVisible && IsGridButtonVisible());
	m_rulerVisibleCommand.setVisible(areButtonsVisible && IsRulerButtonVisible());
	m_gridInMmVisibleCommand.setVisible(areButtonsVisible && IsMmButtonVisible());
}


void CConsoleGui::UpdateCommands()
{
	static const istd::IChangeable::ChangeSet commandsChangeSet(IDisplay::CS_CONSOLE, ibase::ICommandsProvider::CF_COMMANDS);
	istd::CChangeNotifier commandsNotifier(this, &commandsChangeSet);
	Q_UNUSED(commandsNotifier);

	m_rootCommands.ResetChilds();
	m_commands.ResetChilds();
	
	m_commands.SetVisuals(tr("&View"), tr("&View"), tr("&View"));
	m_rootCommands.InsertChild(&m_commands);

	// zoom commands
	if (AreZoomsVisible()){
		m_zoomInCommand.SetVisuals(tr("Zoom In"), tr("Zoom In"), tr("Zoom In"), QIcon(":/Icons/ZoomIn"));
		m_commands.InsertChild(&m_zoomInCommand);

		m_zoomOutCommand.SetVisuals(tr("Zoom Out"), tr("Zoom Out"), tr("Zoom Out"), QIcon(":/Icons/ZoomOut"));
		m_commands.InsertChild(&m_zoomOutCommand);

		m_zoomResetCommand.SetVisuals(tr("Reset Zoom"), tr("Reset Zoom"), tr("Reset Zoom"), QIcon(":/Icons/ZoomReset"));
		m_commands.InsertChild(&m_zoomResetCommand);
	}

	if (IsZoomToFitVisible()){
		m_zoomToFitCommand.SetVisuals(tr("Fit Contents To View"), tr("Fit To View"), tr("Zoom to fit content area"), QIcon(":/Icons/ZoomToFit"));
		m_commands.InsertChild(&m_zoomToFitCommand);
	}

	// points commands
	if (ArePolylineButtonsVisible()){
		m_pointsSelectCommand.SetVisuals(tr("Selection Mode"), tr("Selection"), tr("Object Selection Mode"), QIcon(":/Icons/PointsNone"));
		m_commands.InsertChild(&m_pointsSelectCommand);

		m_pointsMoveCommand.SetVisuals(tr("Modify Mode"), tr("Modify"), tr("Object Modification Mode"), QIcon(":/Icons/PointsMove"));
		m_commands.InsertChild(&m_pointsMoveCommand);

		m_pointsAddCommand.SetVisuals(tr("Add Points"), tr("Add"), tr("Point Adding Mode"), QIcon(":/Icons/PointsAdd"));
		m_commands.InsertChild(&m_pointsAddCommand);

		m_pointsSubCommand.SetVisuals(tr("Remove Points"), tr("Remove"), tr("Point Removing Mode"), QIcon(":/Icons/PointsSub"));
		m_commands.InsertChild(&m_pointsSubCommand);
	}

	// components visibility commands
	if (IsGridButtonVisible()){
		m_gridVisibleCommand.SetVisuals(tr("Show Grid"), tr("Show Grid"), tr("Show/Hide Grid"), QIcon(":/Icons/Grid"));
		m_gridVisibleCommand.setChecked(IsGridVisible());
		m_commands.InsertChild(&m_gridVisibleCommand);
	}

	if (IsRulerButtonVisible()){
		m_rulerVisibleCommand.SetVisuals(tr("Show Ruler"), tr("Show Ruler"), tr("Show/Hide Ruler"), QIcon(":/Icons/ShowRuler"));
		m_rulerVisibleCommand.setChecked(IsRulerVisible());
		m_commands.InsertChild(&m_rulerVisibleCommand);
	}

	if (IsDistanceButtonVisible()){
		m_distanceToolCommand.SetVisuals(tr("Distance"), tr("Distance"), tr("Activate distance measurement tool"), QIcon(":/Icons/Ruler"));
		m_distanceToolCommand.setChecked(IsDistanceToolActive());
		m_commands.InsertChild(&m_distanceToolCommand);
	}

	if (IsMmButtonVisible()){
		m_gridInMmVisibleCommand.SetVisuals(tr("Grid in Millimeter"), tr("Millimeter"), tr("Show grid in millimeters"), QIcon(":/Icons/LogicalUnit"));
		m_gridInMmVisibleCommand.setChecked(IsGridInMm());
		m_commands.InsertChild(&m_gridInMmVisibleCommand);
	}

	if (IsScrollbarsButtonVisible()){
		m_scrollVisibleCommand.SetVisuals(tr("Show Scrollbars"), tr("Show Scrollbars"), tr("Show/Hide Scrollbars"), QIcon(":/Icons/ShowScrollbar"));
		m_scrollVisibleCommand.setChecked(AreScrollbarsVisible());
		m_commands.InsertChild(&m_scrollVisibleCommand);
	}
}


bool CConsoleGui::OnSelectChange(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state)
{
	Q_EMIT selectionChanged(view, position, shape, state);

	return false;
}


bool CConsoleGui::OnViewMouseButton(
			const iview::IShapeView& view,
			const istd::CIndex2d& position,
			Qt::MouseButton buttonType,
			bool state,
			const iview::IInteractiveShape* shapePtr)
{
	Q_EMIT mouseClicked(view, position, buttonType, state, shapePtr);

	return false;
}


void CConsoleGui::OnBoundingBoxChanged()
{
	UpdateScrollbarsValues();
}


// reimplemented (QObject)

bool CConsoleGui::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if (sourcePtr != m_viewPtr && (sourcePtr != m_viewWidget)){
		return BaseClass::eventFilter(sourcePtr, eventPtr);
	}

	int eventType = eventPtr->type();

	switch(eventType){
	case QEvent::MouseButtonDblClick:
		if (OnMouseDoubleClickEvent(eventPtr)){
			return true;
		}
		break;

	case QEvent::Wheel:
		if (OnWheelEvent(dynamic_cast<QWheelEvent*>(eventPtr))){
			return true;
		}
		break;

	case QEvent::KeyPress:
	{
		QKeyEvent* keyEventPtr = dynamic_cast<QKeyEvent*>(eventPtr);
		Q_ASSERT(keyEventPtr != NULL);

		OnKeyPressEvent(keyEventPtr);
		break;
	}

	default:
		break;
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// private members

bool CConsoleGui::ConnectSignalSlots()
{
	bool retVal = true;

	retVal = connect(&m_zoomInCommand, SIGNAL(triggered()), this, SLOT(OnZoomIn())) && retVal;
	retVal = connect(&m_zoomOutCommand, SIGNAL(triggered()), this, SLOT(OnZoomOut())) && retVal;
	retVal = connect(&m_zoomResetCommand, SIGNAL(triggered()), this, SLOT(OnZoomReset())) && retVal;
	retVal = connect(&m_zoomToFitCommand, SIGNAL(toggled(bool)), this, SLOT(OnZoomToFit(bool))) && retVal;
	
	retVal = connect(&m_pointsSelectCommand, SIGNAL(triggered()), this, SLOT(OnPointsNone())) && retVal;
	retVal = connect(&m_pointsMoveCommand, SIGNAL(triggered()), this, SLOT(OnPoinsMove())) && retVal;
	retVal = connect(&m_pointsAddCommand, SIGNAL(triggered()), this, SLOT(OnPointsAdd())) && retVal;
	retVal = connect(&m_pointsSubCommand, SIGNAL(triggered()), this, SLOT(OnPointsSub())) && retVal;

	retVal = connect(&m_scrollVisibleCommand, SIGNAL(toggled(bool)), this, SLOT(OnShowScrollbars(bool))) && retVal;
	retVal = connect(&m_gridVisibleCommand, SIGNAL(toggled(bool)), this, SLOT(OnShowGrid(bool))) && retVal;
	retVal = connect(&m_rulerVisibleCommand, SIGNAL(toggled(bool)), this, SLOT(OnShowRuler(bool))) && retVal;
	retVal = connect(&m_distanceToolCommand, SIGNAL(toggled(bool)), this, SLOT(OnActivateDistanceTool(bool))) && retVal;
	retVal = connect(&m_gridInMmVisibleCommand, SIGNAL(toggled(bool)), this, SLOT(OnShowGridInMm(bool))) && retVal;
	
	retVal = connect(m_horizontalScrollbarPtr, SIGNAL(valueChanged(int)), this, SLOT(OnHScrollbarChanged(int))) && retVal;
	retVal = connect(m_verticalScrollbarPtr, SIGNAL(valueChanged(int)), this, SLOT(OnVScrollbarChanged(int))) && retVal;
	retVal = connect(m_horizontalScrollbarPtr, SIGNAL(sliderMoved(int)), this, SLOT(OnHScrollbarChanged(int))) && retVal;
	retVal = connect(m_verticalScrollbarPtr, SIGNAL(sliderMoved(int)), this, SLOT(OnVScrollbarChanged(int))) && retVal;

	retVal = connect(m_viewPtr, SIGNAL(ShapesChanged()), this, SLOT(UpdateView()), Qt::QueuedConnection) && retVal;

	return retVal;
}


} // namespace iview


