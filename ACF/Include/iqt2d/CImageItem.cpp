#include "iqt2d/CImageItem.h"


#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "iqt/IQImageProvider.h"


namespace iqt2d
{


// public methods

CImageItem::CImageItem() 
{
	m_backgroundPixmap = QPixmap(16, 16);
		
	QPainter p(&m_backgroundPixmap);
	p.fillRect(0, 0, 8, 8, QBrush(qRgb(200,200,200)));
	p.fillRect(0, 8, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 0, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 8, 8, 8, QBrush(qRgb(200,200,200)));
}


void CImageItem::SetBitmap(const iimg::IBitmap& bitmap)
{
	const iqt::IQImageProvider* providerPtr = dynamic_cast<const iqt::IQImageProvider*>(&bitmap);
	if (providerPtr != NULL){
		m_bitmap = QPixmap::fromImage(providerPtr->GetQImage(), Qt::AutoColor);
	}
	else{
		m_bitmap = QPixmap();
	}

	setRect(0, 0, m_bitmap.width(), m_bitmap.height());

	update();
}


// reimplemented (QGraphicsRectItem)

void CImageItem::paint(QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
	QRect imageRect(QPoint(0, 0), m_bitmap.size());
	if (imageRect.isEmpty()){
		return;
	}

	p->save();
	p->setMatrixEnabled(false);

	QRect viewportRect = p->matrix().mapRect(imageRect);
	p->drawTiledPixmap(viewportRect, m_backgroundPixmap);
	p->setPen(QPen(Qt::black));
	viewportRect.adjust(0, 0, -1, -1);
	p->drawRect(viewportRect);
	p->restore();

	p->drawPixmap(option->exposedRect, m_bitmap, option->exposedRect);
}


} // namespace iqt2d


