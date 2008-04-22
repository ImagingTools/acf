#include "iqt2d/CImageItem.h"


#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>


namespace iqt2d
{


// public methods

CImageItem::CImageItem() 
{
	addToGroup(&m_frameItem);
	addToGroup(&m_imageItem);
}


CImageItem::~CImageItem()
{ 
}


void CImageItem::SetImage(const QImage& image)
{
	m_imageItem.SetImage(image);

	m_frameItem.setRect(m_imageItem.rect());
}


int CImageItem::GetWidth() const
{
	return m_frameItem.rect().width();
}


int CImageItem::GetHeight() const
{
	return m_frameItem.rect().height();
}


// public methods of embedded class ImageItem

void CImageItem::ImageItem::SetImage(const QImage& image)
{
	setRect(0, 0, image.width(), image.height());
	m_pixmap = QPixmap::fromImage(image, Qt::AutoColor);

	setZValue(1);

	update();
}

// protected methods of embedded class ImageItem

void CImageItem::ImageItem::paint(QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
	p->drawPixmap(option->exposedRect, m_pixmap, option->exposedRect);
}


int CImageItem::ImageItem::GetWidth() const
{
	return m_pixmap.width();
}


int CImageItem::ImageItem::GetHeight() const
{
	return m_pixmap.height();
}


// protected methods of embedded class ImageFrame

// reimplemented (QGraphicsItem)

void CImageItem::ImageFrame::paint(QPainter* p, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	QGraphicsScene* scenePtr = scene();
	if (scenePtr == NULL){
		return;
	}

	if (m_backgroundPixmap.isNull()){
		CreateBackgroundPixmap();
	}

	p->save();
	QRectF sceneRect = scenePtr->sceneRect();
	sceneRect.setWidth(sceneRect.width() * p->matrix().m11());
	sceneRect.setHeight(sceneRect.height() * p->matrix().m11());

	p->scale(1.0 / p->matrix().m11(), 1.0 / p->matrix().m11());
	p->drawTiledPixmap(sceneRect, m_backgroundPixmap);
	p->restore();
	
	QList<QGraphicsView*> parentViews = scenePtr->views();
	if (parentViews.count() == 0){
		return;
	}

	QGraphicsView* viewPtr = parentViews.at(0);
	
	sceneRect = scenePtr->sceneRect();
	QRectF viewRect = viewPtr->rect();

	if (viewRect.contains(sceneRect)){
		p->setPen(QPen(Qt::black));		

		p->drawRect(rect());
	}
}


// private methods of embedded class ImageFrame

void CImageItem::ImageFrame::CreateBackgroundPixmap()
{
	m_backgroundPixmap = QPixmap(16, 16);
		
	QPainter p(&m_backgroundPixmap);
	p.fillRect(0, 0, 8, 8, QBrush(qRgb(200,200,200)));
	p.fillRect(0, 8, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 0, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 8, 8, 8, QBrush(qRgb(200,200,200)));
}


} // namespace iqt2d


