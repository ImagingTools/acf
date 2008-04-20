#include "iqt2d/CImageViewComp.h"


#include <QApplication>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMenu>


#include "iqt/CBitmap.h"


namespace iqt2d
{


CImageViewComp::CImageViewComp()
{
	m_isFullScreenMode = true;

	m_backgroundPixmap = QPixmap(16,16);
		
	QPainter p(&m_backgroundPixmap);
	p.fillRect(0, 0, 8, 8, QBrush(qRgb(200,200,200)));
	p.fillRect(0, 8, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 0, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 8, 8, 8, QBrush(qRgb(200,200,200)));

	m_isZoomIgnored = false;
}


CImageViewComp::~CImageViewComp() 
{
}


void CImageViewComp::CreateContextMenu()
{
	m_contextMenu = new QMenu(GetWidget());
	m_contextMenu->setTitle(tr("View Properties"));

	QAction* actionFitInView = new QAction(tr("&Fit Image In View"), m_contextMenu);
	connect(actionFitInView, SIGNAL( activated()), this, SLOT(onFitInView()));
	m_contextMenu->addAction(actionFitInView);

	QAction* actionFitToImage = new QAction(tr("&Fit View To Image"), m_contextMenu);
	connect(actionFitToImage, SIGNAL(activated()), this, SLOT(onFitToImage()));
	m_contextMenu->addAction(actionFitToImage);
}


// reimplemented (iqt::TGuiObserverWrap)

void CImageViewComp::UpdateModel() const
{
}


void CImageViewComp::UpdateEditor()
{
	iqt::CBitmap* bitmapPtr = dynamic_cast<iqt::CBitmap*>(GetObjectPtr());

	if (bitmapPtr != NULL){
		m_scene.setSceneRect(0, 0, bitmapPtr->width(), bitmapPtr->height());

		m_imageItem.SetImage(*bitmapPtr);
	}
}


// protected methods

// reimplemented (iqt::CGuiComponentBase)

void CImageViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}
	
	viewPtr->setScene(&m_scene);
	viewPtr->setMouseTracking(true);
	viewPtr->setDragMode(QGraphicsView::ScrollHandDrag);
	viewPtr->setBackgroundBrush(QBrush(m_backgroundPixmap));

	CreateContextMenu();
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

		onFitInView();
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


void CImageViewComp::ScaleView(qreal scaleFactor)
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	QMatrix sceneMatrix = viewPtr->matrix();
	QMatrix scaleMatrix;
	scaleMatrix.scale(scaleFactor,scaleFactor);

	sceneMatrix *= scaleMatrix;
	
	SetZoom(sceneMatrix.m11());
}


void CImageViewComp::onIncrementZoom()
{
	 ScaleView(pow((double)2, 0.5));  
}


void CImageViewComp::onDecrementZoom()
{
	ScaleView(pow((double)2, -0.5));  
}


void CImageViewComp::SetZoom(qreal scaleFactor)
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
	scaleMatrix.scale(scaleFactor,scaleFactor);

	viewPtr->setMatrix(scaleMatrix);

	m_isZoomIgnored = true;

	emit zoomChanged(scaleFactor);

	m_isZoomIgnored = false;

}


void CImageViewComp::SetZoom(const QString & s)
{
	SetZoom(s.toInt()/100.0);
}


void CImageViewComp::switchFullScreen()
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


void CImageViewComp::onFitInView()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	qreal scaleX = (qreal)viewPtr->width()  / (qreal)(m_imageItem.GetWidth());
	qreal scaleY = (qreal)viewPtr->height() / (qreal)(m_imageItem.GetHeight());
	
	qreal s = (scaleX >=scaleY) ? scaleY : scaleX;

	s -= 0.01;

	QMatrix scaleMatrix;
	scaleMatrix.scale(s,s);	
		
	viewPtr->setMatrix(scaleMatrix);
}


void CImageViewComp::onFitToImage()
{
	QGraphicsView* viewPtr = GetQtWidget();
	I_ASSERT(viewPtr != NULL);
	if (viewPtr == NULL){
		return;
	}

	viewPtr->showNormal();
	qreal r = viewPtr->matrix().m11();
	viewPtr->resize(m_imageItem.GetWidth() * r, m_imageItem.GetHeight() * r);
}

/*
void CImageViewComp::resizeEvent(QResizeEvent* event)
{
	QGraphicsView::resizeEvent(event);

	if (m_fitMode == ScaleToFit){
		onFitInView();
	}
}


void CImageViewComp::wheelEvent(QWheelEvent* event)
{
	QGraphicsView::wheelEvent(event);

	ScaleView(pow((double)2, -event->delta() / 240.0));
}


void CImageViewComp::mouseDoubleClickEvent(QMouseEvent * event)
{
	QGraphicsView::mouseDoubleClickEvent(event);

	if (m_isFullScreenMode){
		switchFullScreen();
	}
}


void CImageViewComp::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);
}


void CImageViewComp::mouseReleaseEvent(QMouseEvent* event)
{
	QGraphicsView::mouseReleaseEvent(event);
}


void CImageViewComp::keyReleaseEvent(QKeyEvent* event)
{
	QGraphicsView::keyPressEvent(event);

	switch(event->key()){
		case Qt::Key_Plus:
			onIncrementZoom();
			break;
		case Qt::Key_Minus:
			onDecrementZoom();
			break;
		case Qt::Key_Escape:
			if (parentWidget() == NULL){
				QWidget* w = window();
				w->showNormal();
			}
			break;
	}
}


void CImageViewComp::contextMenuEvent(QContextMenuEvent* event)
{
	if (m_imageItem == NULL){
		return;
	}

	QGraphicsView::contextMenuEvent(event);
	m_contextMenu->exec(QCursor::pos());
}
*/


// private methods

void CImageViewComp::InvalidateScene()
{
	m_scene.setSceneRect(0,0, m_imageItem.GetWidth(), m_imageItem.GetHeight());
	
//	centerOn(m_imageItem);
}
	

} // namespace iqt2d