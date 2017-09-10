#include <iimg/CBitmap.h>


// include STL
#include <cstring>

// Qt includes
#include <QtCore/QVector>
#include <QtCore/QMutexLocker>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>


namespace iimg
{


// global functions

template <	typename PixelType,
			typename WorkingType>
bool ConvertToGrayImage(const IBitmap& inputBitmap, CBitmap& outputBitmap)
{
	istd::CIndex2d size = inputBitmap.GetImageSize();

	if (outputBitmap.CreateBitmap(IBitmap::PF_GRAY, size)){
		// do not copy empty image
		if (size.IsSizeEmpty()){	
			return true;
		}

		PixelType minValue = *(const PixelType*)inputBitmap.GetLinePtr(0);
		PixelType maxValue = minValue;

		for (int y = 0; y < size.GetY(); ++y){
			const PixelType* inputLinePtr = (const PixelType*)inputBitmap.GetLinePtr(y);

			for (int x = 0; x < size.GetX(); ++x){
				PixelType value = inputLinePtr[x];

				if (value < minValue){
					minValue = value;
				}

				if (value > maxValue){
					maxValue = value;
				}
			}
		}

		if (maxValue > minValue){
			for (int y = 0; y < size.GetY(); ++y){
				const PixelType* inputLinePtr = (const PixelType*)inputBitmap.GetLinePtr(y);
				quint8* outputLinePtr = (quint8*)outputBitmap.GetLinePtr(y);

				for (int x = 0; x < size.GetX(); ++x){
					WorkingType value = inputLinePtr[x];

					outputLinePtr[x] = quint8(255 * (value - minValue) / (maxValue - minValue));
				}
			}
		}
		else{
			outputBitmap.ClearImage();
		}

		return true;
	}

	return false;
}


CBitmap::CBitmap()
{
}


CBitmap::CBitmap(const CBitmap& bitmap)
:	BaseClass(bitmap), 
	m_image(bitmap.m_image.copy()),
	m_externalBuffer(NULL)
{
}


CBitmap::CBitmap(const QImage& image)
{
	SetQImage(image);
}


// reimplemented (iimg::IQImageProvider)

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


bool CBitmap::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, int /*pixelBitsCount*/, int /*componentsCount*/)
{
	if (size.IsSizeEmpty()){
		ResetImage();

		return true;
	}

	QImage::Format imageFormat = CalcQtFormat(pixelFormat);
	QSize imageSize(size.GetX(), size.GetY());

	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	// check if we need to recreate the image
	if (!m_externalBuffer.IsValid() && (m_image.format() == imageFormat) && (m_image.size() == imageSize)){
		m_image.setDotsPerMeterX(1000);
		m_image.setDotsPerMeterY(1000);

		return true;
	}

	m_externalBuffer.Reset();

	// else recreate the image
	if (imageFormat != QImage::Format_Invalid){
		QImage image(imageSize, imageFormat);

		Q_ASSERT(!image.isNull());
		if (image.isNull()){
			return false;
		}

		image.setDotsPerMeterX(1000);
		image.setDotsPerMeterY(1000);

		Q_ASSERT(!image.isNull());
		if (image.isNull()){
			return false;
		}

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
		
		Q_ASSERT(!image.isNull());
		if (image.isNull()){
			return false;
		}

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
	Q_ASSERT(positionY >= 0);
	Q_ASSERT(positionY < GetImageSize().GetY());

	return m_image.scanLine(positionY);
}


void* CBitmap::GetLinePtr(int positionY)
{
	Q_ASSERT(positionY >= 0);
	Q_ASSERT(positionY < GetImageSize().GetY());

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


void CBitmap::ClearImage()
{
	m_image.fill(0);
}


istd::CIndex2d CBitmap::GetImageSize() const
{
	return m_image.size();
}


int CBitmap::GetComponentsCount() const
{
	switch (m_image.format()){
	case QImage::Format_Indexed8:
	case QImage::Format_Mono:
	case QImage::Format_MonoLSB:
#if QT_VERSION >= 0x050500
	case QImage::Format_Grayscale8:
	case QImage::Format_Alpha8:
#endif
		return 1;

	case QImage::Format_RGB16:
	case QImage::Format_RGB555:
	case QImage::Format_RGB444:
	case QImage::Format_ARGB4444_Premultiplied:
		return 2;

	case QImage::Format_ARGB8565_Premultiplied:
	case QImage::Format_RGB666:
	case QImage::Format_ARGB6666_Premultiplied:
	case QImage::Format_RGB888:
	case QImage::Format_ARGB8555_Premultiplied:
		return 3;

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
		QRgb pixel = m_image.pixel(position.GetX(), position.GetY());

		rgbValue[2] = qRed(pixel) / 255.0;
		rgbValue[1] = qGreen(pixel) / 255.0;
		rgbValue[0] = qBlue(pixel) / 255.0;

		return rgbValue;
	}

	return BaseClass::GetColorAt(position);
}


// reimplemented (istd::IChangeable)

int CBitmap::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CBitmap::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	const CBitmap* bitmapImplPtr = dynamic_cast<const CBitmap*>(&object);

	if (bitmapImplPtr != NULL){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_externalBuffer.Reset();

		m_image = bitmapImplPtr->GetQImage().copy();

		return true;
	}
	else{
		const IBitmap* bitmapPtr = dynamic_cast<const IBitmap*>(&object);
		if (bitmapPtr != NULL){
			istd::CChangeNotifier notifier(this);
			Q_UNUSED(notifier);

			istd::CIndex2d size = bitmapPtr->GetImageSize();

			iimg::IBitmap::PixelFormat pixelFormat = bitmapPtr->GetPixelFormat();
			if (IsFormatSupported(pixelFormat)){
				if (CreateBitmap(pixelFormat, size)){
					// do not copy empty image
					if (size.IsSizeEmpty()){
						return true;
					}

					int lineBytesCount = qMin(GetLineBytesCount(), bitmapPtr->GetLineBytesCount());
					Q_ASSERT(lineBytesCount >= 0);
					if (lineBytesCount <= 0){
						return false;
					}

					for (int y = 0; y < size.GetY(); ++y){
						std::memcpy(GetLinePtr(y), bitmapPtr->GetLinePtr(y), lineBytesCount);
					}

					return true;
				}
			}
			else if (mode == CM_CONVERT){
				switch (pixelFormat){
				case PF_FLOAT32:
					return ConvertToGrayImage<float, float>(*bitmapPtr, *this);

				case PF_FLOAT64:
					return ConvertToGrayImage<double, double>(*bitmapPtr, *this);

				case PF_GRAY16:
					return ConvertToGrayImage<quint16, quint32>(*bitmapPtr, *this);

				case PF_GRAY32:
					return ConvertToGrayImage<quint32, quint64>(*bitmapPtr, *this);

				default:
					break;
				}
			}
		}
	}

	return false;
}


istd::IChangeable* CBitmap::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<CBitmap> clonePtr(new CBitmap);

	if (clonePtr->CopyFrom(*this, mode)){
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
#if QT_VERSION >= 0x050500
			return QImage::Format_Grayscale8;
#else
			return QImage::Format_Indexed8;
#endif

		case PF_MONO:
			return QImage::Format_Mono;

		// just pixel mapped formats
		case PF_GRAY16:
			return QImage::Format_RGB16;

		case PF_GRAY32:
			return QImage::Format_ARGB32;

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
#if QT_VERSION >= 0x050500
	case QImage::Format_Grayscale8:
	case QImage::Format_Alpha8:
#endif
		return PF_GRAY;

	case QImage::Format_Mono:
		return PF_MONO;

	// just pixel mapped formats
	case QImage::Format_RGB16:
	case QImage::Format_RGB555:
	case QImage::Format_RGB444:
	case QImage::Format_ARGB4444_Premultiplied:
		return PF_GRAY16;

	default:
		return PF_UNKNOWN;
	}
}


bool CBitmap::SetQImage(const QImage& image)
{
	m_image = image;

	if (m_image.isNull()){
		return false;
	}

	if (m_image.format() == QImage::Format_Indexed8){
		QMutexLocker lock(&s_colorTableLock);
		if (s_colorTableGray.isEmpty()){
			s_colorTableGray.resize(256);

			for (int colorIndex = 0; colorIndex < 256; ++colorIndex){
				s_colorTableGray[colorIndex] = ::qRgb(colorIndex, colorIndex, colorIndex);
			}
		}

		m_image.setColorTable(s_colorTableGray);
	}

	return !(m_image.isNull());
}


// static members

QVector<QRgb> CBitmap::s_colorTableGray;
QMutex CBitmap::s_colorTableLock;


} // namespace iimg


