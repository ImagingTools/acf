#include "iqt2d/CImageItem.h"


#include <QStyleOptionGraphicsItem>
#include <QPainter>


namespace iqt2d
{


CImageItem::CImageItem() 
{
	addToGroup(&m_imageItem);
}


CImageItem::~CImageItem()
{ 
}


void CImageItem::SetImage(const QImage& image)
{
	m_imageItem.SetImage(image);

	m_frameItem.setRect(m_imageItem.boundingRect().adjusted(0.5,0.5,-0.5,-0.5));
}


int CImageItem::GetWidth() const
{
	return m_imageItem.GetWidth();
}


int CImageItem::GetHeight() const
{
	return m_imageItem.GetHeight();
}


void CImageItem::ImageItem::SetImage(const QImage& image)
{
	setRect(0, 0, image.width(), image.height());
	m_pixmap = QPixmap::fromImage(image, Qt::AutoColor);

	setZValue(0);

	update();
}


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


void CImageItem::ImageFrame::paint(QPainter* p, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	p->setPen(QPen(Qt::darkGreen));		

	p->drawRect(rect());
}


} // namespace iqt2d

