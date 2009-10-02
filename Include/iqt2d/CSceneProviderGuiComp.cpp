#include "iqt2d/CSceneProviderGuiComp.h"


#include <QApplication>
#include <QKeyEvent>
#include <QMenu>
#include <QVBoxLayout>


namespace iqt2d
{


// public methods

CSceneProviderGuiComp::CSceneProviderGuiComp()
:	m_isFullScreenMode(true),
	m_isZoomIgnored(false),
	m_fitToViewCommand("&Fit Contents To View"),
	m_resetZoomCommand("&Reset Zoom"),
	m_savedParentWidgetPtr(NULL),
	m_isotropyFactor(0)
{
	m_scenePtr = new QGraphicsScene;

	m_scenePtr->setFocus();
}


CSceneProviderGuiComp::FitMode CSceneProviderGuiComp::GetFitMode() const
{
	return m_fitMode;
}


void CSceneProviderGuiComp::SetFitMode(FitMode mode)
{
	if (mode != m_fitMode){
		m_fitMode = mode;

		if (m_fitMode != FM_NONE){
			SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

			SetFittedScale(m_fitMode);
		}
		else{
			SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
			SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		}
	}
}


bool CSceneProviderGuiComp::IsFullScreenMode() const
{
	return m_isFullScreenMode;
}


void CSceneProviderGuiComp::SetFullScreenMode(bool fullScreenMode)
{
	if (fullScreenMode != m_isFullScreenMode){
		m_isFullScreenMode = fullScreenMode;

		if (m_isFullScreenMode != CompleteFrame->isFullScreen()){
			if (m_isFullScreenMode){
				m_savedParentWidgetPtr = CompleteFrame->parentWidget();
				m_savedViewTransform = SceneView->matrix();
				CompleteFrame->setParent(NULL);

				BottomFrame->setVisible(false);
				CompleteFrame->showFullScreen();

				SetFittedScale(m_fitMode != FM_NONE? m_fitMode: FM_ISOTROPIC);
			}
			else{
				if (m_savedParentWidgetPtr != NULL){
					CompleteFrame->setParent(m_savedParentWidgetPtr);
					m_savedParentWidgetPtr->layout()->addWidget(CompleteFrame);
				}
				
				m_savedParentWidgetPtr = NULL;

				BottomFrame->setVisible(m_sceneControllerGuiCompPtr.IsValid());

				CompleteFrame->showNormal();

				SceneView->setMatrix(m_savedViewTransform);
			}
		}
	}
}


double CSceneProviderGuiComp::GetIsotropyFactor() const
{
	return m_isotropyFactor;
}


void CSceneProviderGuiComp::SetIsotropyFactor(double factor)
{
	if (factor != m_isotropyFactor){
		m_isotropyFactor = factor;

		if ((m_fitMode == FM_ISOTROPIC_REDUCTION) || (m_fitMode == FM_ISOTROPIC)){
			SetFittedScale(m_fitMode);
		}
	}
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
	if (m_fitMode != FM_NONE){
		return SR_ALL;
	}

	return SR_NONE;
}


// public slots

void CSceneProviderGuiComp::SetZoom(double scaleFactor)
{
	if (m_isZoomIgnored || (m_fitMode != FM_NONE)){
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


void CSceneProviderGuiComp::OnFitToView()
{
	SetFittedScale(FM_ISOTROPIC);
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
		SetFitMode(FM_ISOTROPIC);
	}
	else{
		SetFitMode(FM_NONE);
	}

	m_fitToViewCommand.setEnabled(!isAutoScale);
	m_resetZoomCommand.setEnabled(!isAutoScale);
}


// protected methods

void CSceneProviderGuiComp::OnResize(QResizeEvent* /*eventPtr*/)
{
	if (m_fitMode != FM_NONE){
		SetFittedScale(m_fitMode);
	}
}


void CSceneProviderGuiComp::OnWheelEvent(QGraphicsSceneWheelEvent* eventPtr)
{
	ScaleView(pow((double)2, eventPtr->delta() / 240.0));
}


void CSceneProviderGuiComp::OnMouseDoubleClickEvent(QMouseEvent* /*eventPtr*/)
{
	SetFullScreenMode(!IsFullScreenMode());
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
		SetFullScreenMode(false);
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


void CSceneProviderGuiComp::SetFittedScale(FitMode mode)
{
	if ((mode < FM_ISOTROPIC_REDUCTION) || (mode > FM_ANISOTROPIC)){
		return;
	}

	QRectF sceneRect = SceneView->sceneRect();
	if ((sceneRect.width() < I_BIG_EPSILON) || (sceneRect.height() < I_BIG_EPSILON)){
		return;
	}

	double scaleX = SceneView->width() / sceneRect.width();
	double scaleY = SceneView->height() / sceneRect.height();

	if (mode <= FM_ISOTROPIC){
		double newScale = istd::Min(scaleX, scaleY) * (1 - m_isotropyFactor) + istd::Max(scaleX, scaleY) * m_isotropyFactor;

		if ((mode == FM_ISOTROPIC_REDUCTION) && newScale >= 1){
			newScale = 1;
		}

		scaleX = newScale;
		scaleY = newScale;
	}

	QMatrix fitMatrix;
	fitMatrix.scale(scaleX, scaleY);
	fitMatrix.translate(-sceneRect.left(), -sceneRect.top());

	SceneView->setMatrix(fitMatrix);
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

	m_isotropyFactor = *m_isotropyFactorAttrPtr;

	if (m_fitModeAttrPtr.IsValid()){
		SetFitMode(FitMode(*m_fitModeAttrPtr));
	}

	m_scenePtr->installEventFilter(this);

	QColor backgroundColor = SceneView->backgroundBrush().color();
	QString backgroundColorString = QString("background-color: rgb(%1,%2,%3)").arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());

	// try to place the scene controller:
	if (m_sceneControllerGuiCompPtr.IsValid()){
		m_sceneControllerGuiCompPtr->CreateGui(ControllerFrame);
		QPalette viewPalette = SceneView->palette();
		viewPalette.setColor(QPalette::Window, SceneView->backgroundBrush().color());

		BottomFrame->setStyleSheet(backgroundColorString);
		ControllerFrame->setStyleSheet(backgroundColorString);
		GetWidget()->setStyleSheet(backgroundColorString);
		m_sceneControllerGuiCompPtr->GetWidget()->setStyleSheet(backgroundColorString);

		BottomFrame->setVisible(true);
	}
	else{
		BottomFrame->setVisible(false);
	}
} 


void CSceneProviderGuiComp::OnGuiDestroyed()
{
	if (m_sceneControllerGuiCompPtr.IsValid()){
		m_sceneControllerGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (QObject)

bool CSceneProviderGuiComp::eventFilter(QObject* obj, QEvent* eventPtr)
{
	if (!IsGuiCreated()){
		return QObject::eventFilter(obj, eventPtr);
	}

	if (obj != SceneView && obj != m_scenePtr){
		return QObject::eventFilter(obj, eventPtr);
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
	}

	return QObject::eventFilter(obj, eventPtr);
}


} // namespace iqt2d




