#include "iqt2d/CImageViewComp.h"


#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QMenu>


#include "iqt/CBitmap.h"


namespace iqt2d
{


// public methods

CImageViewComp::CImageViewComp()
	:m_isFullScreenMode(true),
	m_isZoomIgnored(false)
{
	m_scenePtr = new QGraphicsScene;

	m_scenePtr->setFocus();

	CreateContextMenu();
}


CImageViewComp::~CImageViewComp() 
{
}


void CImageViewComp::SetFitMode(FitMode mode)
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


void CImageViewComp::SetFullScreenMode(bool fullScreenMode)
{
	m_isFullScreenMode = fullScreenMode;
}


// public slots

void CImageViewComp::SetZoom(double scaleFactor)
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

	QMatrix scaleMatrix;
	scaleMatrix.scale(scaleFactor, scaleFactor);

	viewPtr->setMatrix(scaleMatrix);

	m_isZoomIgnored = true;

	emit zoomChanged(scaleFactor);

	m_isZoomIgnored = false;

}


void CImageViewComp::SetZoom(const QString& zoomString)
{
	SetZoom(zoomString.toInt() / 100.0);
}


void CImageViewComp::OnZoomIncrement()
{
	 ScaleView(pow((double)2, 0.5));  
}


void CImageViewComp::OnZoomDecrement()
{
	ScaleView(pow((double)2, -0.5));  
}


void CImageViewComp::SwitchFullScreen()
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


void CImageViewComp::OnFitToView()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

//	viewPtr->fitInView(&m_imageItem, Qt::KeepAspectRatio);

	double scaleX = viewPtr->width()  / (double)(m_imageItem.GetWidth());
	double scaleY = viewPtr->height() / (double)(m_imageItem.GetHeight());
	
	double newScale = (scaleX >=scaleY) ? scaleY : scaleX;

	QMatrix scaleMatrix;
	scaleMatrix.scale(newScale, newScale);	
		
	viewPtr->setMatrix(scaleMatrix);
}


void CImageViewComp::OnFitToImage()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	viewPtr->showNormal();
	double r = viewPtr->matrix().m11();
	viewPtr->resize(m_imageItem.GetWidth() * r, m_imageItem.GetHeight() * r);
}


// reimplemented (iqt::TGuiObserverWrap)

void CImageViewComp::UpdateModel() const
{
}


void CImageViewComp::UpdateEditor()
{
	if (IsGuiCreated()){
		iqt::CBitmap* bitmapPtr = dynamic_cast<iqt::CBitmap*>(GetObjectPtr());

		if (bitmapPtr != NULL){
			m_imageItem.SetImage(*bitmapPtr);

			InvalidateScene();
		}
	}
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CImageViewComp::GetCommands() const
{
	return &m_editorCommand;
}


// protected methods

void CImageViewComp::OnResize(QResizeEvent* /*event*/)
{
	if (m_fitMode == ScaleToFit){
		OnFitToView();
	}
}


void CImageViewComp::OnWheelEvent(QGraphicsSceneWheelEvent* event)
{
	ScaleView(pow((double)2, event->delta() / 240.0));
}


void CImageViewComp::OnMouseDoubleClickEvent(QMouseEvent* /*event*/)
{
	if (m_isFullScreenMode){
		SwitchFullScreen();
	}
}


void CImageViewComp::OnMouseMoveEvent(QMouseEvent* /*event*/)
{
}


void CImageViewComp::OnKeyReleaseEvent(QKeyEvent* event)
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


void CImageViewComp::OnContextMenuEvent(QContextMenuEvent* /*event*/)
{
}


// reimplemented (iqt::CGuiComponentBase)

void CImageViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}
	
	viewPtr->setScene(m_scenePtr);
	viewPtr->setMouseTracking(true);
	viewPtr->setDragMode(QGraphicsView::ScrollHandDrag);
//	viewPtr->setCacheMode(QGraphicsView::CacheBackground);

	m_scenePtr->addItem(&m_imageItem);

	viewPtr->installEventFilter(this);
	m_scenePtr->installEventFilter(this);
	
	m_scenePtr->setBackgroundBrush(QBrush(Qt::lightGray));
}


// reimplemented (QObject)

bool CImageViewComp::eventFilter(QObject* obj, QEvent* event)
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
	}

	return false;
}


// private methods

void CImageViewComp::InvalidateScene()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}
	
	viewPtr->centerOn(&m_imageItem);
}


void CImageViewComp::CreateContextMenu()
{
	iqt::CHierarchicalCommand* imageMenuPtr = new iqt::CHierarchicalCommand("&Image");

	iqt::CHierarchicalCommand* fitToViewCommandPtr = new iqt::CHierarchicalCommand("&Fit Image To View");
	connect(fitToViewCommandPtr, SIGNAL( activated()), this, SLOT(OnFitToView()));
	imageMenuPtr->InsertChild(fitToViewCommandPtr, true);

	iqt::CHierarchicalCommand* fitToImageCommandPtr = new iqt::CHierarchicalCommand("&Fit View To Image");
	connect(fitToImageCommandPtr, SIGNAL( activated()), this, SLOT(OnFitToImage()));
	imageMenuPtr->InsertChild(fitToImageCommandPtr, true);

	m_editorCommand.InsertChild(imageMenuPtr, true);
}



void CImageViewComp::ScaleView(double scaleFactor)
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	QMatrix sceneMatrix = viewPtr->matrix();
	QMatrix scaleMatrix;
	scaleMatrix.scale(scaleFactor, scaleFactor);

	sceneMatrix *= scaleMatrix;
	
	SetZoom(sceneMatrix.m11());
}


} // namespace iqt2d