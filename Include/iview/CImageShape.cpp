#include "iview/CImageShape.h"


// Qt includes
#include <QPainter>


// ACF includes
#include "istd/TDelPtr.h"

#include "imod/IModel.h"

#include "iimg/IBitmap.h"

#include "iqt/CBitmap.h"

#include "iview/CScreenTransform.h"


namespace iview
{


// public methods

CImageShape::CImageShape(const icmm::IColorTransformation* colorTransformationPtr)
:	m_colorTransformationPtr(colorTransformationPtr)
{
}


// reimplemented (iview::IVisualizable)

void CImageShape::Draw(QPainter& drawContext) const
{
	const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(GetModelPtr());
	if (IsDisplayConnected() && bitmapPtr != NULL){
		ibase::CSize bitmapSize = bitmapPtr->GetImageSize();
		if (!bitmapSize.IsNull()){
			i2d::CRect bitmapArea(istd::CIndex2d(0, 0), bitmapSize);

			i2d::CAffine2d destTransform = GetViewToScreenTransform();

			i2d::CMatrix2d& deform = destTransform.GetDeformMatrixRef();

			i2d::CMatrix2d::ColumnVector column0; 
			i2d::CMatrix2d::ColumnVector column1;

			deform.GetColumnVector(0, column0);
			deform.GetColumnVector(1, column1);

			column0 *= bitmapArea.GetRight();
			column1 *= bitmapArea.GetBottom();

			deform.SetColumnVector(0, column0);
			deform.SetColumnVector(1, column1);

			DrawBitmap(drawContext, *bitmapPtr, bitmapArea, destTransform);
		}
	}
}


// reimplemented (imod::IObserver)

void CImageShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	const iqt::IQImageProvider* providerPtr = dynamic_cast<const iqt::IQImageProvider*>(GetModelPtr());
	istd::TDelPtr<iqt::CBitmap> qtBitmapPtr;
	if (providerPtr == NULL){
		qtBitmapPtr.SetPtr(new iqt::CBitmap);
		providerPtr = qtBitmapPtr.GetPtr();
		iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(GetModelPtr());
		I_ASSERT(bitmapPtr != NULL);

		qtBitmapPtr->CopyFrom(*bitmapPtr);
	}

	QImage image = providerPtr->GetQImage();
	if (m_colorTransformationPtr != NULL){
		SetLookupTableToImage(image, *m_colorTransformationPtr);
	}

	m_pixmap = QPixmap::fromImage(image, Qt::AutoColor);
}


// reimplemented (iview::CInteractiveShapeBase)

void CImageShape::CalcBoundingBox(i2d::CRect& result) const
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const iview::CScreenTransform& transform = GetViewToScreenTransform();

		const iimg::IBitmap& model = *dynamic_cast<const iimg::IBitmap*>(modelPtr);
		I_ASSERT(&model != NULL);

		ibase::CSize size = model.GetImageSize();

		istd::CIndex2d corners[4];

		corners[0] = transform.GetScreenPosition(i2d::CVector2d(0, 0));
		corners[1] = transform.GetScreenPosition(i2d::CVector2d(size.GetX(), 0));
		corners[2] = transform.GetScreenPosition(i2d::CVector2d(0, size.GetY()));
		corners[3] = transform.GetScreenPosition(i2d::CVector2d(size.GetX(), size.GetY()));

		result = i2d::CRect(corners[0], corners[0]);
		result.Union(corners[1]);
		result.Union(corners[2]);
		result.Union(corners[3]);
	}
	else{
		result.Reset();
	}
}


// protected methods

void CImageShape::DrawBitmap(
			QPainter& painter,
			const iimg::IBitmap& bitmap,
			const i2d::CRect& bitmapArea,
			const i2d::CAffine2d& destTransform) const
{
	const iqt::CBitmap* bitmapPtr = dynamic_cast<const iqt::CBitmap*>(&bitmap);

	if (bitmapPtr != NULL){
		const i2d::CMatrix2d& deform = destTransform.GetDeformMatrix();
		const i2d::CVector2d& pos = destTransform.GetTranslation();

		QMatrix matrix(	deform.GetAt(0, 0) / double(bitmapArea.GetWidth()),
						deform.GetAt(1, 0) / double(bitmapArea.GetWidth()),
						deform.GetAt(0, 1) / double(bitmapArea.GetHeight()),
						deform.GetAt(1, 1) / double(bitmapArea.GetHeight()),
						pos.GetX(),
						pos.GetY());
		
		painter.setMatrix(matrix);

		painter.drawPixmap(0, 0, m_pixmap, bitmapArea.GetLeft(), bitmapArea.GetTop(), bitmapArea.GetRight(), bitmapArea.GetBottom());

		painter.setMatrixEnabled(false);
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


} // namespace iview


