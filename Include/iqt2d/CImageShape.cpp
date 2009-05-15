#include "iqt2d/CImageShape.h"


#include <QStyleOptionGraphicsItem>
#include <QPainter>


#include "istd/TDelPtr.h"

#include "iqt/IQImageProvider.h"
#include "iqt/CBitmap.h"


namespace iqt2d
{


// public methods

CImageShape::CImageShape(const icmm::IColorTransformation* colorTransformationPtr)
:	m_isBackgroundGridUsed(false),
	m_colorTransformationPtr(colorTransformationPtr)
{
	m_backgroundPixmap = QPixmap(16, 16);
		
	QPainter p(&m_backgroundPixmap);
	p.fillRect(0, 0, 8, 8, QBrush(qRgb(200,200,200)));
	p.fillRect(0, 8, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 0, 8, 8, QBrush(Qt::white));
	p.fillRect(8, 8, 8, 8, QBrush(qRgb(200,200,200)));
}


// reimplemented (imod::IObserver)

void CImageShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	const iqt::IQImageProvider* providerPtr = dynamic_cast<const iqt::IQImageProvider*>(GetObjectPtr());
	istd::TDelPtr<iqt::CBitmap> qtBitmapPtr;
	if (providerPtr == NULL){
		qtBitmapPtr.SetPtr(new iqt::CBitmap);
		providerPtr = qtBitmapPtr.GetPtr();
		iimg::IBitmap* bitmapPtr = GetObjectPtr();

		qtBitmapPtr->CopyImageFrom(*bitmapPtr);
	}

	QImage image = providerPtr->GetQImage();
	if (m_colorTransformationPtr != NULL){
		SetLookupTableToImage(image, *m_colorTransformationPtr);
	}
		
	m_bitmap = QPixmap::fromImage(image, Qt::AutoColor);

	update();
}


// reimplemented (QGraphicsRectItem)

QRectF CImageShape::boundingRect() const
{
	QRect imageRect(QPoint(0, 0), m_bitmap.size());

	return QRectF(imageRect);
}


void CImageShape::paint(QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
	QRect imageRect(QPoint(0, 0), m_bitmap.size());
	if (imageRect.isEmpty()){
		return;
	}

	if (m_isBackgroundGridUsed){
		p->save();
		p->setMatrixEnabled(false);

		QRect viewportRect = p->matrix().mapRect(imageRect);
		p->drawTiledPixmap(viewportRect, m_backgroundPixmap);
		p->setPen(QPen(Qt::black));
		viewportRect.adjust(0, 0, -1, -1);
		p->drawRect(viewportRect);
		p->restore();
	}

	p->drawPixmap(option->exposedRect, m_bitmap, option->exposedRect);
}


// private methods

void CImageShape::SetLookupTableToImage(QImage& image, const icmm::IColorTransformation& colorTransformation)
{
	if (image.isGrayscale()){
		QVector<QRgb> rgbTable;
		for (int colorIndex = 0; colorIndex < 256; colorIndex++){
			icmm::CVarColor argumentColor;	
			argumentColor.SetElementsCount(1);
			argumentColor.SetElement(0, colorIndex / 255.0);

			icmm::CVarColor result = colorTransformation.GetValueAt(argumentColor);
			if (result.GetElementsCount() == 3){
				rgbTable.append(qRgb(result[0] * 255, result[1] * 255, result[2] * 255));
			}
			else{
				rgbTable.append(qRgb(colorIndex, colorIndex, colorIndex));
			}
		}

		image.setNumColors(256);
		image.setColorTable(rgbTable);
	}
}



} // namespace iqt2d


