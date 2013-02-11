#include "iview/CImageShape.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "istd/TDelPtr.h"

#include "imod/IModel.h"

#include "icmm/CRgbToHsvTranformation.h"
#include "icmm/CRgb.h"
#include "icmm/CHsv.h"

#include "iimg/CBitmap.h"

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

bool CImageShape::OnAttached(imod::IModel* modelPtr)
{
	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(modelPtr);
	if (bitmapPtr != NULL){
		return BaseClass::OnAttached(modelPtr);
	}

	return false;
}


void CImageShape::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	const iimg::IQImageProvider* providerPtr = dynamic_cast<const iimg::IQImageProvider*>(modelPtr);
	istd::TDelPtr<iimg::CBitmap> qtBitmapPtr;
	if (providerPtr == NULL){
		qtBitmapPtr.SetPtr(new iimg::CBitmap);
		providerPtr = qtBitmapPtr.GetPtr();
		iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(modelPtr);
		Q_ASSERT(bitmapPtr != NULL);

		qtBitmapPtr->CopyFrom(*bitmapPtr);
	}

	QImage image = providerPtr->GetQImage();
	if (m_colorTransformationPtr != NULL){
		SetLookupTableToImage(image, *m_colorTransformationPtr);
	}

	m_pixmap = QPixmap::fromImage(image, Qt::AutoColor);

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// reimplemented (iview::CShapeBase)

i2d::CRect CImageShape::CalcBoundingBox() const
{
	i2d::CRect boundingBox = i2d::CRect::GetEmpty();

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const iview::CScreenTransform& transform = GetViewToScreenTransform();

		const iimg::IBitmap& model = *dynamic_cast<const iimg::IBitmap*>(modelPtr);
		Q_ASSERT(&model != NULL);

		ibase::CSize size = model.GetImageSize();

		istd::CIndex2d corners[4];

		corners[0] = transform.GetScreenPosition(i2d::CVector2d(0, 0));
		corners[1] = transform.GetScreenPosition(i2d::CVector2d(size.GetX(), 0));
		corners[2] = transform.GetScreenPosition(i2d::CVector2d(0, size.GetY()));
		corners[3] = transform.GetScreenPosition(i2d::CVector2d(size.GetX(), size.GetY()));

		boundingBox = i2d::CRect(corners[0], corners[0]);
		boundingBox.Union(corners[1]);
		boundingBox.Union(corners[2]);
		boundingBox.Union(corners[3]);
	}

	return boundingBox;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CImageShape::IsTouched(istd::CIndex2d position) const
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const iview::CScreenTransform& transform = GetViewToScreenTransform();

		const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(modelPtr);
		Q_ASSERT(bitmapPtr != NULL);

		ibase::CSize size = bitmapPtr->GetImageSize();

		istd::CIndex2d bitmapPosition = transform.GetClientPosition(position).ToIndex2d();
		if (		(bitmapPosition.GetX() >= 0) &&
					(bitmapPosition.GetY() >= 0) &&
					(bitmapPosition.GetX() < size.GetX()) &&
					(bitmapPosition.GetY() < size.GetY())){
			return TS_INACTIVE;
		}
	}

	return TS_NONE;
}


QString CImageShape::GetShapeDescriptionAt(istd::CIndex2d position) const
{
	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const iview::CScreenTransform& transform = GetViewToScreenTransform();

		const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(modelPtr);
		Q_ASSERT(bitmapPtr != NULL);

		ibase::CSize size = bitmapPtr->GetImageSize();

		istd::CIndex2d bitmapPosition = transform.GetClientPosition(position).ToIndex2d();
		if (		(bitmapPosition.GetX() >= 0) &&
					(bitmapPosition.GetY() >= 0) &&
					(bitmapPosition.GetX() < size.GetX()) &&
					(bitmapPosition.GetY() < size.GetY())){
			int pixelMode = bitmapPtr->GetPixelFormat();

			icmm::CVarColor pixelValue = bitmapPtr->GetColorAt(bitmapPosition);
			
			switch (pixelMode){
			case iimg::IBitmap::PF_GRAY:
				return QObject::tr("Gray value %1%").arg(int(pixelValue[0] * 100));

			case iimg::IBitmap::PF_RGB:{
				icmm::CRgbToHsvTranformation rgbToHsvTransformation;
				icmm::CRgb rgb(pixelValue[2], pixelValue[1], pixelValue[0]);
				icmm::CHsv hsv;
				
				icmm::CVarColor rgbColor(rgb);
				icmm::CVarColor hsvColor(hsv.GetElementsCount());
				rgbToHsvTransformation.GetValueAt(rgbColor, hsvColor);
				hsv = hsvColor;

				return QObject::tr("  Red:%1% (%2), Green:%3% (%4), Blue:%5% (%6) Hue: %7°, Saturation: %8, Value: %9")
							.arg(int(rgb.GetElement(icmm::CRgb::CI_RED)* 100))
							.arg(int(rgb.GetElement(icmm::CRgb::CI_RED)* 255))
							.arg(int(rgb.GetElement(icmm::CRgb::CI_BLUE)* 100))
							.arg(int(rgb.GetElement(icmm::CRgb::CI_BLUE)* 255))
							.arg(int(rgb.GetElement(icmm::CRgb::CI_GREEN)* 100))
							.arg(int(rgb.GetElement(icmm::CRgb::CI_GREEN)* 255))
							.arg(int(hsv.GetElement(icmm::CHsv::CI_HUE)))
							.arg(int(hsv.GetElement(icmm::CHsv::CI_SATURATION)* 255))
							.arg(int(hsv.GetElement(icmm::CHsv::CI_VALUE)* 255));
			}

			case iimg::IBitmap::PF_RGBA:
				return QObject::tr("RGBA value %1%, %2%, %3%, %4%")
							.arg(int(pixelValue[3] * 100))
							.arg(int(pixelValue[2] * 100))
							.arg(int(pixelValue[1] * 100))
							.arg(int(pixelValue[0] * 100));
			}
		}
	}

	return "";
}


// protected methods

void CImageShape::DrawBitmap(
			QPainter& painter,
			const iimg::IBitmap& bitmap,
			const i2d::CRect& bitmapArea,
			const i2d::CAffine2d& destTransform) const
{
	const iimg::CBitmap* bitmapPtr = dynamic_cast<const iimg::CBitmap*>(&bitmap);

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


