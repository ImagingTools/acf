#include <iview/CImageShape.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <istd/TDelPtr.h>
#include <imod/IModel.h>
#include <icmm/CVarColor.h>
#include <icmm/CRgbToHsvTranformation.h>
#include <icmm/CRgb.h>
#include <icmm/CHsv.h>
#include <iimg/CBitmap.h>
#include <iview/CScreenTransform.h>


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
	const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(GetObservedModel());
	if (IsDisplayConnected() && bitmapPtr != NULL){
		ibase::CSize bitmapSize = bitmapPtr->GetImageSize();
		if (!bitmapSize.IsNull()){
			i2d::CRect bitmapArea(istd::CIndex2d(0, 0), bitmapSize);

			i2d::CVector2d corners[3];
			corners[0] = GetScreenPosition(i2d::CVector2d(0, 0));
			corners[1] = GetScreenPosition(i2d::CVector2d(bitmapSize.GetX(), 0));
			corners[2] = GetScreenPosition(i2d::CVector2d(0, bitmapSize.GetY()));

			i2d::CMatrix2d destDeform(corners[1] - corners[0], corners[2] - corners[0]);
			i2d::CAffine2d destTransform(destDeform, corners[0]);

			DrawBitmap(drawContext, *bitmapPtr, bitmapArea, destTransform);
		}
	}
}


// reimplemented (imod::IObserver)

bool CImageShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(modelPtr);
	if (bitmapPtr != NULL){
		return BaseClass::OnModelAttached(modelPtr, changeMask);
	}

	return false;
}


void CImageShape::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
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

	if (m_colorTransformationPtr != NULL){
		QImage image = providerPtr->GetQImage().copy();
		SetLookupTableToImage(image, *m_colorTransformationPtr);
		m_pixmap = QPixmap::fromImage(image, Qt::AutoColor);
	}
	else{
		m_pixmap = QPixmap::fromImage(providerPtr->GetQImage(), Qt::AutoColor);
	}

	BaseClass::AfterUpdate(modelPtr, changeSet);
}


// reimplemented (iview::CShapeBase)

i2d::CRect CImageShape::CalcBoundingBox() const
{
	i2d::CRect boundingBox = i2d::CRect::GetEmpty();

	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const iimg::IBitmap& model = *dynamic_cast<const iimg::IBitmap*>(modelPtr);
		Q_ASSERT(&model != NULL);

		ibase::CSize size = model.GetImageSize();

		istd::CIndex2d corners[4];

		corners[0] = GetScreenPosition(i2d::CVector2d(0, 0)).ToIndex2d();
		corners[1] = GetScreenPosition(i2d::CVector2d(size.GetX(), 0)).ToIndex2d();
		corners[2] = GetScreenPosition(i2d::CVector2d(0, size.GetY())).ToIndex2d();
		corners[3] = GetScreenPosition(i2d::CVector2d(size.GetX(), size.GetY())).ToIndex2d();

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
	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(modelPtr);
		Q_ASSERT(bitmapPtr != NULL);

		ibase::CSize size = bitmapPtr->GetImageSize();

		istd::CIndex2d bitmapPosition = GetLogPosition(i2d::CVector2d(position)).ToIndex2d();
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
	QString retVal;

	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const iimg::IBitmap* bitmapPtr = dynamic_cast<const iimg::IBitmap*>(modelPtr);
		Q_ASSERT(bitmapPtr != NULL);

		ibase::CSize size = bitmapPtr->GetImageSize();

		istd::CIndex2d bitmapPosition = GetLogPosition(i2d::CVector2d(position)).ToIndex2d();
		if (		(bitmapPosition.GetX() >= 0) &&
					(bitmapPosition.GetY() >= 0) &&
					(bitmapPosition.GetX() < size.GetX()) &&
					(bitmapPosition.GetY() < size.GetY())){
			int pixelMode = bitmapPtr->GetPixelFormat();

			icmm::CVarColor pixelValue = bitmapPtr->GetColorAt(bitmapPosition);

			retVal = QObject::tr("[%1, %2] px").arg(bitmapPosition.GetX()).arg(bitmapPosition.GetY());

			QString pixelValueInfo;

			switch (pixelMode){
			case iimg::IBitmap::PF_GRAY:
				pixelValueInfo = QObject::tr("Gray value %1%").arg(int(pixelValue[0] * 100));
				break;

			case iimg::IBitmap::PF_RGB:
				{
					icmm::CRgbToHsvTranformation rgbToHsvTransformation;
					icmm::CRgb rgb(pixelValue[2], pixelValue[1], pixelValue[0]);
					icmm::CHsv hsv;

					icmm::CVarColor rgbColor(rgb);
					icmm::CVarColor hsvColor(hsv.GetElementsCount());
					rgbToHsvTransformation.GetValueAt(rgbColor, hsvColor);
					hsv = hsvColor;

					pixelValueInfo = QObject::tr("[RGB=(%1%,%3%,%5%)=(%2,%4,%6), HSV=(%7,%8,%9)]")
								.arg(int(rgb.GetElement(icmm::CRgb::CI_RED) * 100))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_RED) * 255))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_GREEN) * 100))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_GREEN) * 255))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_BLUE) * 100))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_BLUE) * 255))
								.arg(int(hsv.GetElement(icmm::CHsv::CI_HUE)))
								.arg(int(hsv.GetElement(icmm::CHsv::CI_SATURATION) * 255))
								.arg(int(hsv.GetElement(icmm::CHsv::CI_VALUE) * 255));
				}
				break;

			case iimg::IBitmap::PF_RGBA:
				{
					icmm::CRgbToHsvTranformation rgbToHsvTransformation;
					icmm::CRgb rgb(pixelValue[3], pixelValue[2], pixelValue[1]);
					icmm::CHsv hsv;
					double alpha = pixelValue[0];

					icmm::CVarColor rgbColor(rgb);
					icmm::CVarColor hsvColor(hsv.GetElementsCount());
					rgbToHsvTransformation.GetValueAt(rgbColor, hsvColor);
					hsv = hsvColor;

					pixelValueInfo = QObject::tr("[RGBA=(%1%,%3%,%5%,%7)=(%2,%4,%6,%8), HSV=(%9,%10,%11)]")
								.arg(int(rgb.GetElement(icmm::CRgb::CI_RED) * 100))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_RED) * 255))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_GREEN) * 100))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_GREEN) * 255))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_BLUE) * 100))
								.arg(int(rgb.GetElement(icmm::CRgb::CI_BLUE) * 255))
								.arg(int(alpha * 100))
								.arg(int(alpha * 255))
								.arg(int(hsv.GetElement(icmm::CHsv::CI_HUE)))
								.arg(int(hsv.GetElement(icmm::CHsv::CI_SATURATION) * 255))
								.arg(int(hsv.GetElement(icmm::CHsv::CI_VALUE) * 255));
				}
				break;

			default:
				break;
			}

			if (!pixelValueInfo.isEmpty()){
				retVal += QObject::tr(", ");
				retVal += pixelValueInfo;
			}
		}
	}

	return retVal;
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

		painter.drawPixmap(
			0,
			0,
			m_pixmap,
			bitmapArea.GetLeft(),
			bitmapArea.GetTop(),
			bitmapArea.GetRight(),
			bitmapArea.GetBottom());

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
#if QT_VERSION < 0x050000
		image.setNumColors(256);
#else
		image.setColorCount(256);
#endif
		image.setColorTable(rgbTable);
	}
}


} // namespace iview


