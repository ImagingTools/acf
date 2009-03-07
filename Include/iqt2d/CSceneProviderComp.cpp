#include "iqt2d/CSceneProviderComp.h"


#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QMenu>


namespace iqt2d
{


// public methods

CSceneProviderComp::CSceneProviderComp()
:	m_isFullScreenMode(true),
	m_isZoomIgnored(false),
	m_fitToViewCommand("&Fit Image To View"),
	m_resetZoomCommand("&Reset Zoom")
{
	m_scenePtr = new QGraphicsScene;

	m_scenePtr->setFocus();
}


void CSceneProviderComp::SetFitMode(FitMode mode)
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	m_fitMode = mode;
	if (m_fitMode == ScaleToFit){
		viewPtr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		viewPtr->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		OnFitToView();
	}
	else{
		viewPtr->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		viewPtr->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}


void CSceneProviderComp::SetFullScreenMode(bool fullScreenMode)
{
	m_isFullScreenMode = fullScreenMode;
}


// public slots

void CSceneProviderComp::SetZoom(double scaleFactor)
{
	if (m_isZoomIgnored){
		return;
	}

	if (m_fitMode == ScaleToFit){
		return;
	}

	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	QMatrix fitMatrix;
	fitMatrix.scale(scaleFactor, scaleFactor);

	viewPtr->setMatrix(fitMatrix);

	m_isZoomIgnored = true;

	emit zoomChanged(scaleFactor);

	m_isZoomIgnored = false;
}


void CSceneProviderComp::SetZoom(const QString& zoomString)
{
	SetZoom(zoomString.toInt() / 100.0);
}


void CSceneProviderComp::OnZoomIncrement()
{
	 ScaleView(pow((double)2, 0.5));  
}


void CSceneProviderComp::OnZoomDecrement()
{
	ScaleView(pow((double)2, -0.5));  
}


void CSceneProviderComp::SwitchFullScreen()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	if (viewPtr->isFullScreen()){
		viewPtr->showNormal();
	}

	else{
		viewPtr->showFullScreen();
	}
}


void CSceneProviderComp::OnFitToView()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	QRectF sceneRect = viewPtr->sceneRect();
	double scaleX = viewPtr->width() / sceneRect.width();
	double scaleY = viewPtr->height() / sceneRect.height();

	double newScale = istd::Min(scaleX, scaleY);

	QMatrix fitMatrix;
	fitMatrix.scale(newScale, newScale);
	fitMatrix.translate(-sceneRect.left(), -sceneRect.top());

	viewPtr->setMatrix(fitMatrix);
}


void CSceneProviderComp::OnFitToShapes()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	viewPtr->showNormal();
	double r = viewPtr->matrix().m11();
	QRectF sceneRect = viewPtr->sceneRect();
	viewPtr->resize(int(sceneRect.width() * r), int(sceneRect.height() * r));
}


void CSceneProviderComp::OnResetScale()
{
	SetZoom(1.0);
}


void CSceneProviderComp::OnAutoFit(bool isAutoScale)
{
	if (isAutoScale){
		SetFitMode(ScaleToFit);
	}
	else{
		SetFitMode(NoFit);
	}

	m_fitToViewCommand.setEnabled(!isAutoScale);
	m_resetZoomCommand.setEnabled(!isAutoScale);
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CSceneProviderComp::GetCommands() const
{
	return &m_editorCommand;
}


// reimplemented (iqt2d::ISceneProvider)

int CSceneProviderComp::GetSceneId() const
{
	I_ASSERT(m_sceneIdAttrPtr.IsValid());

	return *m_sceneIdAttrPtr;
}


QGraphicsScene* CSceneProviderComp::GetScene() const
{
	return m_scenePtr;
}


// reimplemented (icomp::IComponent)

void CSceneProviderComp::OnComponentCreated()
{
	CreateContextMenu();
}


// protected methods

void CSceneProviderComp::OnResize(QResizeEvent* /*event*/)
{
	if (m_fitMode == ScaleToFit){
		OnFitToView();
	}
}


void CSceneProviderComp::OnWheelEvent(QGraphicsSceneWheelEvent* event)
{
	ScaleView(pow((double)2, event->delta() / 240.0));
}


void CSceneProviderComp::OnMouseDoubleClickEvent(QMouseEvent* /*event*/)
{
	if (m_isFullScreenMode){
		SwitchFullScreen();
	}
}


void CSceneProviderComp::OnMouseMoveEvent(QMouseEvent* /*event*/)
{
}


void CSceneProviderComp::OnKeyReleaseEvent(QKeyEvent* event)
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	switch(event->key()){
		case Qt::Key_Plus:
			OnZoomIncrement();
			break;
		case Qt::Key_Minus:
			OnZoomDecrement();
			break;
		case Qt::Key_Escape:
			if (viewPtr->isFullScreen()){
				viewPtr->showNormal();
			}
			break;
	}
}


void CSceneProviderComp::OnContextMenuEvent(QContextMenuEvent* /*event*/)
{
}


void CSceneProviderComp::CreateContextMenu()
{
	iqtgui::CHierarchicalCommand* imageMenuPtr = new iqtgui::CHierarchicalCommand("&Image");

	iqtgui::CHierarchicalCommand* autoFitToViewCommandPtr = new iqtgui::CHierarchicalCommand("&Auto Fit");
	autoFitToViewCommandPtr->SetStaticFlags(iqtgui::CHierarchicalCommand::CF_ONOFF | 
											iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU);
	connect(autoFitToViewCommandPtr, SIGNAL(toggled(bool)), this, SLOT(OnAutoFit(bool)));
	imageMenuPtr->InsertChild(autoFitToViewCommandPtr, true);

	connect(&m_fitToViewCommand, SIGNAL( activated()), this, SLOT(OnFitToView()));
	imageMenuPtr->InsertChild(&m_fitToViewCommand);

	I_ASSERT(m_allowWidgetResizeAttrPtr.IsValid());	// this attribute is obligatory
	if (*m_allowWidgetResizeAttrPtr){
		iqtgui::CHierarchicalCommand* fitToImageCommandPtr = new iqtgui::CHierarchicalCommand("&Fit View To Image");
		connect(fitToImageCommandPtr, SIGNAL( activated()), this, SLOT(OnFitToShapes()));
		imageMenuPtr->InsertChild(fitToImageCommandPtr, true);
	}

	connect(&m_resetZoomCommand, SIGNAL( activated()), this, SLOT(OnResetScale()));
	imageMenuPtr->InsertChild(&m_resetZoomCommand);

	m_editorCommand.InsertChild(imageMenuPtr, true);
}


void CSceneProviderComp::ScaleView(double scaleFactor)
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	QMatrix sceneMatrix = viewPtr->matrix();
	QMatrix fitMatrix;
	fitMatrix.scale(scaleFactor, scaleFactor);

	sceneMatrix *= fitMatrix;
	
	SetZoom(sceneMatrix.m11());
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSceneProviderComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}
	
	viewPtr->setFrameStyle(QFrame::NoFrame);
	viewPtr->setScene(m_scenePtr);
	viewPtr->setMouseTracking(true);
	viewPtr->setDragMode(QGraphicsView::ScrollHandDrag);
	viewPtr->setCacheMode(QGraphicsView::CacheBackground);

	viewPtr->installEventFilter(this);
	m_scenePtr->installEventFilter(this);
	
	m_scenePtr->setBackgroundBrush(QBrush(QColor(193, 196, 180, 255)));

	if (m_useAntialiasingAttrPtr.IsValid() && *m_useAntialiasingAttrPtr){
		viewPtr->setRenderHints(QPainter::Antialiasing);
	}

	if (m_fitModeAttrPtr.IsValid()){
		SetFitMode(FitMode(*m_fitModeAttrPtr));
	}
}


// reimplemented (QObject)

bool CSceneProviderComp::eventFilter(QObject* obj, QEvent* event)
{
	if (!IsGuiCreated()){
		return false;
	}

	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return false;
	}

	if (obj != viewPtr && obj != m_scenePtr){
		return false;
	}

	switch(event->type()){
		case QEvent::MouseButtonDblClick:
		case QEvent::GraphicsSceneMouseDoubleClick:
			OnMouseDoubleClickEvent(dynamic_cast<QMouseEvent*>(event));
			break;
		case QEvent::KeyRelease:
			OnKeyReleaseEvent(dynamic_cast<QKeyEvent*>(event));
			break;

		case QEvent::Resize:
			OnResize(dynamic_cast<QResizeEvent*>(event));
			break;

		case QEvent::GraphicsSceneWheel:
			OnWheelEvent(dynamic_cast<QGraphicsSceneWheelEvent*>(event));
			break;

		default:
			break;
	}

	return false;
}


} // namespace iqt2d


