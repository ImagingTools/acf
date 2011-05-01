#include "iqt2d/CImageShape.h"


// Qt includes
#include <QStyleOptionGraphicsItem>
#include <QPainter>


// ACF includes
#include "istd/TDelPtr.h"

#include "iqt/IQImageProvider.h"
#include "iqt/CBitmap.h"


namespace iqt2d
{


// public methods

CImageShape::CImageShape(const icmm::IColorTransformation* colorTransformationPtr)
:	m_isFrameVisible(false),
	m_positionMode(PM_CORNER_LT),
	m_colorTransformationPtr(colorTransformationPtr)
{
}


// reimplemented (imod::IObserver)

void CImageShape::BeforeUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	prepareGeometryChange();
}


void CImageShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	const iqt::IQImageProvider* providerPtr = dynamic_cast<const iqt::IQImageProvider*>(GetObjectPtr());
	istd::TDelPtr<iqt::CBitmap> qtBitmapPtr;
	if (providerPtr == NULL){
		qtBitmapPtr.SetPtr(new iqt::CBitmap);
		providerPtr = qtBitmapPtr.GetPtr();
		iimg::IBitmap* bitmapPtr = GetObjectPtr();

		qtBitmapPtr->CopyFrom(*bitmapPtr);
	}

	QImage image = providerPtr->GetQImage();
	if (m_colorTransformationPtr != NULL){
		SetLookupTableToImage(image, *m_colorTransformationPtr);
	}

	m_bitmap = QPixmap::fromImage(image, Qt::AutoColor);

	update();
}


bool CImageShape::OnDetached(imod::IModel* modelPtr)
{
	m_bitmap = QPixmap();

	update();

	return BaseClass::OnDetached(modelPtr);
}



// reimplemented (QGraphicsRectItem)

QRectF CImageShape::boundingRect() const
{
	QRectF imageRect(QPoint(0, 0), m_bitmap.size());
	if (imageRect.isEmpty()){
		return QRectF();
	}

	QRectF retVal(imageRect);

	if (m_positionMode == PM_CENTER){
		QPointF center = retVal.center();
		retVal.adjust(-center.x(), -center.y(), -center.x(), -center.y());
	}

	return retVal;
}


void CImageShape::paint(QPainter* p, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	QRectF imageRect = boundingRect();
	if (imageRect.isEmpty()){
		return;
	}

	p->drawPixmap(imageRect.topLeft(), m_bitmap);

	if (m_isFrameVisible){
		p->setPen(QPen(Qt::black, 2));
		imageRect.adjust(-1, -1, 1, 1);
		p->drawRect(imageRect);
	}
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


