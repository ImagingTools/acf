#include "iqt/CBitmap.h"


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QVector>
#include <QtGui/QColor>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TDelPtr.h"


namespace iqt
{


CBitmap::CBitmap()
{
}


CBitmap::CBitmap(const CBitmap& bitmap)
:	BaseClass(bitmap), m_image(bitmap.m_image)
{
}


CBitmap::CBitmap(const QImage& image)
{
	SetQImage(image);
}


// reimplemented (iqt::IQImageProvider)

const QImage& CBitmap::GetQImage() const
{
	return m_image;
}


bool CBitmap::CopyImageFrom(const QImage& image)
{
	istd::CChangeNotifier changePtr(this);

	m_externalBuffer.Reset();

	return SetQImage(image);
}


// reimplemented (ibase::IObjectSnap)

bool CBitmap::GetSnap(const istd::IChangeable& data, iimg::IBitmap& objectSnap, const istd::CIndex2d& size) const
{
	const iqt::CBitmap* bitmapPtr = dynamic_cast<const iqt::CBitmap*>(&data);
	if (bitmapPtr == NULL){
		return false;
	}

	iqt::CBitmap snapBitmap;
	snapBitmap.CopyImageFrom(bitmapPtr->GetQImage().scaled(size.GetX(), size.GetY(), Qt::KeepAspectRatioByExpanding));

	objectSnap.CopyFrom(snapBitmap);

	return true;
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CBitmap::GetCalibration() const
{
	EnsureLogTransformCalculated();

	return &m_logTransform;
}


// reimplemented (iimg::IBitmap)

bool CBitmap::IsFormatSupported(PixelFormat pixelFormat) const
{
	switch (pixelFormat){
		case PF_MONO:
		case PF_GRAY:
		case PF_RGB:
		case PF_RGBA:
			return true;
		default:
			return false;
	}

	return false;
}


iimg::IBitmap::PixelFormat CBitmap::GetPixelFormat() const
{
	return CalcFromQtFormat(m_image.format());
}


bool CBitmap::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size)
{
	istd::CChangeNotifier changePtr(this);

	QImage::Format imageFormat = CalcQtFormat(pixelFormat);
	if (imageFormat != QImage::Format_Invalid){
		QImage image(size.GetX(), size.GetY(), imageFormat);

		image.setDotsPerMeterX(1000);
		image.setDotsPerMeterY(1000);

		m_externalBuffer.Reset();

		return SetQImage(image);
	}

	return false;
}


bool CBitmap::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference)
{
	istd::CChangeNotifier changePtr(this);

	QImage::Format imageFormat = CalcQtFormat(pixelFormat);
	if (imageFormat != QImage::Format_Invalid){
		QImage image((quint8*)dataPtr, size.GetX(), size.GetY(), imageFormat);
		if ((linesDifference != 0) && (linesDifference != image.scanLine(1) - image.scanLine(0))){
			return false;	// requested format doesnt fit to internal Qt bitmap representation
		}

		image.setDotsPerMeterX(1000);
		image.setDotsPerMeterY(1000);

		m_externalBuffer.SetPtr((quint8*)dataPtr, releaseFlag);

		return SetQImage(image);
	}

	return false;
}


int CBitmap::GetLinesDifference() const
{
	return m_image.bytesPerLine();
}


int CBitmap::GetPixelsDifference() const
{
	return GetComponentsCount();
}


int CBitmap::GetPixelBitsCount() const
{
	return GetComponentsCount() * 8;
}


const void* CBitmap::GetLinePtr(int positionY) const
{
	I_ASSERT(positionY >= 0);
	I_ASSERT(positionY < GetImageSize().GetY());

	return m_image.scanLine(positionY);
}


void* CBitmap::GetLinePtr(int positionY)
{
	I_ASSERT(positionY >= 0);
	I_ASSERT(positionY < GetImageSize().GetY());

	return m_image.scanLine(positionY);
}


// reimplemented (iimg::IRasterImage)

void CBitmap::ResetImage()
{
	istd::CChangeNotifier changePtr(this);

	m_image = QImage();

	m_image.setDotsPerMeterX(1000);
	m_image.setDotsPerMeterY(1000);

	m_externalBuffer.Reset();
}


istd::CIndex2d CBitmap::GetImageSize() const
{
	return GetCIndex2d(m_image.size());
}


int CBitmap::GetComponentsCount() const
{
	switch (m_image.format()){
	case QImage::Format_Indexed8:
		return 1;

	case QImage::Format_RGB32:
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied:
		return 4;

	default:
		return 0;
	}
}

icmm::CVarColor CBitmap::GetColorAt(const istd::CIndex2d& position) const
{
	PixelFormat pixelFormat = GetPixelFormat();
	if (pixelFormat == PF_RGB){
		icmm::CVarColor rgbValue(3);
		QColor color(m_image.pixel(position.GetX(), position.GetY()));

		rgbValue[0] = color.red() / 255.0;
		rgbValue[1] = color.green() / 255.0;
		rgbValue[2] = color.blue() / 255.0;

		return rgbValue;
	}

	return BaseClass::GetColorAt(position);
}


// reimplemented (istd::IChangeable)

int CBitmap::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CBitmap::CopyFrom(const istd::IChangeable& object)
{
	const CBitmap* bitmapPtr = dynamic_cast<const CBitmap*>(&object);

	if (bitmapPtr != NULL){
		istd::CChangeNotifier notifier(this);

		m_image = bitmapPtr->GetQImage();

		return true;
	}
	else{
		const IBitmap* bitmapPtr = dynamic_cast<const IBitmap*>(&object);
		if (bitmapPtr != NULL){
			istd::CChangeNotifier notifier(this);
			istd::CIndex2d size = bitmapPtr->GetImageSize();
			if (CreateBitmap(bitmapPtr->GetPixelFormat(), size)){
				int lineBytesCount = qMin(GetLineBytesCount(), bitmapPtr->GetLineBytesCount());
				for (int y = 0; y < size.GetY(); ++y){
					std::memcpy(GetLinePtr(y), bitmapPtr->GetLinePtr(y), lineBytesCount);
				}

				return true;
			}
		}
	}

	return false;
}


istd::IChangeable* CBitmap::CloneMe() const
{
	istd::TDelPtr<CBitmap> clonePtr(new CBitmap);

	if (clonePtr->CopyFrom(*this)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// protected methods

QImage::Format CBitmap::CalcQtFormat(PixelFormat pixelFormat) const
{
	switch (pixelFormat){
		case PF_RGB:
			return QImage::Format_RGB32;

		case PF_RGBA:
			return QImage::Format_ARGB32;

		case PF_GRAY:
			return QImage::Format_Indexed8;

		default:
			return QImage::Format_Invalid;
	}
}


iimg::IBitmap::PixelFormat CBitmap::CalcFromQtFormat(QImage::Format imageFormat) const
{
	switch (imageFormat){
	case QImage::Format_RGB32:
		return PF_RGB;

	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied:
		return PF_RGBA;

	case QImage::Format_Indexed8:
		return PF_GRAY;

	default:
		return PF_UNKNOWN;
	}
}


bool CBitmap::SetQImage(const QImage& image)
{
	m_image = image;

	if (m_image.format() == QImage::Format_Indexed8){
		QVector<QRgb> colorTable(256);

		for (int colorIndex = 0; colorIndex < 256; ++colorIndex){
			colorTable[colorIndex] = ::qRgb(colorIndex, colorIndex, colorIndex);
		}

		m_image.setColorTable(colorTable);
	}

	return true;
}


// private methods

void CBitmap::EnsureLogTransformCalculated() const 
{
	int pixelPerMmX = m_image.dotsPerMeterX() / 1000;
	int pixelPerMmY = m_image.dotsPerMeterY() / 1000;

	i2d::CVector2d scale(1.0 / pixelPerMmX, 1.0 / pixelPerMmY);

	i2d::CVector2d center(GetImageSize().GetX() * 0.5 * scale.GetX(), GetImageSize().GetY() * 0.5 * scale.GetY());
	
	m_logTransform.Reset(-center, 0.0, scale);
}


} // namespace iqt


