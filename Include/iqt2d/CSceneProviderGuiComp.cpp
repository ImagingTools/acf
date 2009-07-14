#include "iqt2d/CSceneProviderGuiComp.h"


#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>


namespace iqt2d
{


// public methods

CSceneProviderGuiComp::CSceneProviderGuiComp()
:	m_isFullScreenMode(true),
	m_isZoomIgnored(false),
	m_fitToViewCommand("&Fit Contents To View"),
	m_resetZoomCommand("&Reset Zoom"),
	m_savedParentWidgetPtr(NULL)
{
	m_scenePtr = new QGraphicsScene;

	m_scenePtr->setFocus();
}


void CSceneProviderGuiComp::SetFitMode(FitMode mode)
{
	m_fitMode = mode;
	if (m_fitMode == ScaleToFit){
		SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		OnFitToView();
	}
	else{
		SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}


void CSceneProviderGuiComp::SetFullScreenMode(bool fullScreenMode)
{
	m_isFullScreenMode = fullScreenMode;
}


// public slots

void CSceneProviderGuiComp::SetZoom(double scaleFactor)
{
	if (m_isZoomIgnored){
		return;
	}

	if (m_fitMode == ScaleToFit){
		return;
	}

	QMatrix currentMatrix = SceneView->matrix();
	double currentScale = sqrt(currentMatrix.det());

	QMatrix newMatrix;
	newMatrix.scale(scaleFactor / currentScale, scaleFactor/ currentScale);

	currentMatrix *= newMatrix;

	SceneView->setMatrix(currentMatrix);

	m_isZoomIgnored = true;

	emit zoomChanged(scaleFactor);

	m_isZoomIgnored = false;
}


void CSceneProviderGuiComp::SetZoom(const QString& zoomString)
{
	SetZoom(zoomString.toInt() / 100.0);
}


void CSceneProviderGuiComp::OnZoomIncrement()
{
	 ScaleView(pow((double)2, 0.5));  
}


void CSceneProviderGuiComp::OnZoomDecrement()
{
	ScaleView(pow((double)2, -0.5));  
}


void CSceneProviderGuiComp::SwitchFullScreen()
{
	if (SceneView->isFullScreen()){
		if (m_savedParentWidgetPtr != NULL){
			SceneView->setParent(m_savedParentWidgetPtr);
			m_savedParentWidgetPtr->layout()->addWidget(SceneView);
		}
		
		m_savedParentWidgetPtr = NULL;

		SceneView->showNormal();

		SceneView->setMatrix(m_savedViewTransform);
	}

	else{
		m_savedParentWidgetPtr = SceneView->parentWidget();
		m_savedViewTransform = SceneView->matrix();
		SceneView->setParent(NULL);

		SceneView->showFullScreen();

		OnFitToView();
	}
}


void CSceneProviderGuiComp::OnFitToView()
{
	QRectF sceneRect = SceneView->sceneRect();
	double scaleX = SceneView->width() / sceneRect.width();
	double scaleY = SceneView->height() / sceneRect.height();

	double newScale = istd::Min(scaleX, scaleY);

	QMatrix fitMatrix;
	fitMatrix.scale(newScale, newScale);
	fitMatrix.translate(-sceneRect.left(), -sceneRect.top());

	SceneView->setMatrix(fitMatrix);
}


void CSceneProviderGuiComp::OnFitToShapes()
{
	SceneView->showNormal();

	double r = SceneView->matrix().m11();
	QRectF sceneRect = SceneView->sceneRect();
	SceneView->resize(int(sceneRect.width() * r), int(sceneRect.height() * r));
}


void CSceneProviderGuiComp::OnResetScale()
{
	SetZoom(1.0);
}


void CSceneProviderGuiComp::OnAutoFit(bool isAutoScale)
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

const idoc::IHierarchicalCommand* CSceneProviderGuiComp::GetCommands() const
{
	return &m_editorCommand;
}


// reimplemented (iqt2d::ISceneProvider)

int CSceneProviderGuiComp::GetSceneId() const
{
	I_ASSERT(m_sceneIdAttrPtr.IsValid());

	return *m_sceneIdAttrPtr;
}


QGraphicsScene* CSceneProviderGuiComp::GetScene() const
{
	return m_scenePtr;
}


// reimplemented (iqt2d::ISceneRestrictions)

int CSceneProviderGuiComp::GetSceneRestrictionsFlags() const
{
	return SR_NONE;
}


// protected methods

void CSceneProviderGuiComp::OnResize(QResizeEvent* /*eventPtr*/)
{
	if (m_fitMode == ScaleToFit){
		OnFitToView();
	}
}


void CSceneProviderGuiComp::OnWheelEvent(QGraphicsSceneWheelEvent* eventPtr)
{
	ScaleView(pow((double)2, eventPtr->delta() / 240.0));
}


void CSceneProviderGuiComp::OnMouseDoubleClickEvent(QMouseEvent* /*eventPtr*/)
{
	if (m_isFullScreenMode){
		SwitchFullScreen();
	}
}


void CSceneProviderGuiComp::OnMouseMoveEvent(QMouseEvent* /*eventPtr*/)
{
}


void CSceneProviderGuiComp::OnKeyReleaseEvent(QKeyEvent* eventPtr)
{

	switch(eventPtr->key()){
		case Qt::Key_Plus:
			OnZoomIncrement();
			break;
		case Qt::Key_Minus:
			OnZoomDecrement();
			break;
		case Qt::Key_Escape:
			if (SceneView->isFullScreen()){
				SwitchFullScreen();
			}
			break;
	}
}


void CSceneProviderGuiComp::OnContextMenuEvent(QContextMenuEvent* /*eventPtr*/)
{
}


void CSceneProviderGuiComp::CreateContextMenu()
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
		iqtgui::CHierarchicalCommand* fitToImageCommandPtr = new iqtgui::CHierarchicalCommand("&Fit View To Contents");
		connect(fitToImageCommandPtr, SIGNAL( activated()), this, SLOT(OnFitToShapes()));
		imageMenuPtr->InsertChild(fitToImageCommandPtr, true);
	}

	connect(&m_resetZoomCommand, SIGNAL( activated()), this, SLOT(OnResetScale()));
	imageMenuPtr->InsertChild(&m_resetZoomCommand);

	m_editorCommand.InsertChild(imageMenuPtr, true);
}


void CSceneProviderGuiComp::ScaleView(double scaleFactor)
{
	QMatrix sceneMatrix = SceneView->matrix();

	double currentScale = sqrt(sceneMatrix.det());

	SetZoom(scaleFactor * currentScale);
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSceneProviderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CreateContextMenu();
	
	SceneView->setScene(m_scenePtr);
	SceneView->setMouseTracking(true);
	SceneView->setDragMode(QGraphicsView::ScrollHandDrag);
	SceneView->setBackgroundBrush(QBrush(QColor(128, 128, 128)));
	SceneView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	SceneView->installEventFilter(this);
	
	if (m_useAntialiasingAttrPtr.IsValid() && *m_useAntialiasingAttrPtr){
		SceneView->setRenderHints(QPainter::Antialiasing);
	}

	if (m_fitModeAttrPtr.IsValid()){
		SetFitMode(FitMode(*m_fitModeAttrPtr));
	}

	m_scenePtr->installEventFilter(this);

	QColor backgroundColor = SceneView->backgroundBrush().color();
	QString backgroundColorString = QString("background-color: rgb(%1,%2,%3)").arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());

	// try to place the scene controller:
	if (m_sceneControllerCompPtr.IsValid()){
		iqtgui::IGuiObject* guiPtr = dynamic_cast<iqtgui::IGuiObject*>(m_sceneControllerCompPtr.GetPtr());
		if (guiPtr != NULL){
			guiPtr->CreateGui(ControllerFrame);
			QPalette viewPalette = SceneView->palette();
			viewPalette.setColor(QPalette::Window, SceneView->backgroundBrush().color());

			ControllerFrame->setStyleSheet(backgroundColorString);
			GetWidget()->setStyleSheet(backgroundColorString);
			guiPtr->GetWidget()->setStyleSheet(backgroundColorString);
		}

		m_sceneControllerCompPtr->RegisterSceneProvider(this);
	}
} 


void CSceneProviderGuiComp::OnGuiDestroyed()
{
	if (m_sceneControllerCompPtr.IsValid()){
		iqtgui::IGuiObject* guiPtr = dynamic_cast<iqtgui::IGuiObject*>(m_sceneControllerCompPtr.GetPtr());
		if (guiPtr != NULL){
			guiPtr->DestroyGui();
		}
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (QObject)

bool CSceneProviderGuiComp::eventFilter(QObject* obj, QEvent* eventPtr)
{
	if (!IsGuiCreated()){
		return false;
	}

	if (obj != SceneView && obj != m_scenePtr){
		return false;
	}

	switch(eventPtr->type()){
		case QEvent::MouseButtonDblClick:
		case QEvent::GraphicsSceneMouseDoubleClick:
			OnMouseDoubleClickEvent(dynamic_cast<QMouseEvent*>(eventPtr));
			break;
		case QEvent::KeyRelease:
			OnKeyReleaseEvent(dynamic_cast<QKeyEvent*>(eventPtr));
			break;

		case QEvent::Resize:
			OnResize(dynamic_cast<QResizeEvent*>(eventPtr));
			break;

		case QEvent::GraphicsSceneWheel:
			OnWheelEvent(dynamic_cast<QGraphicsSceneWheelEvent*>(eventPtr));
			break;

		default:
			break;
	}

	return false;
}


} // namespace iqt2d




