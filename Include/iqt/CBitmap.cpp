#include "iqt/CBitmap.h"


// STL includes
#include <cstring>

// QT includes
#include <QVector>
#include <QColor>

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


// reimplemented (iimg::IBitmap)

int CBitmap::GetPixelFormat() const
{
	return CalcFromQtFormat(m_image.format());
}


bool CBitmap::CreateBitmap(int pixelFormat, const istd::CIndex2d& size)
{
	istd::CChangeNotifier changePtr(this);

	QImage::Format imageFormat = CalcQtFormat(pixelFormat);
	if (imageFormat != QImage::Format_Invalid){
		QImage image(size.GetX(), size.GetY(), imageFormat);

		m_externalBuffer.Reset();

		return SetQImage(image);
	}

	return false;
}

bool CBitmap::CreateBitmap(int pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference)
{
	istd::CChangeNotifier changePtr(this);

	QImage::Format imageFormat = CalcQtFormat(pixelFormat);
	if (imageFormat != QImage::Format_Invalid){
		QImage image((I_BYTE*)dataPtr, size.GetX(), size.GetY(), imageFormat);
		if ((linesDifference != 0) && (linesDifference != image.scanLine(1) - image.scanLine(0))){
			return false;	// requested format doesnt fit to internal Qt bitmap representation
		}

		m_externalBuffer.SetPtr((I_BYTE*)dataPtr, releaseFlag);

		return SetQImage(image);
	}

	return false;
}


int CBitmap::GetLinesDifference() const
{
	return m_image.scanLine(1) - m_image.scanLine(0);
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
		return 4;

	default:
		return 0;
	}
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
				int lineBytesCount = istd::Min(GetLineBytesCount(), bitmapPtr->GetLineBytesCount());
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

QImage::Format CBitmap::CalcQtFormat(int pixelFormat) const
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


int CBitmap::CalcFromQtFormat(QImage::Format imageFormat) const
{
	switch (imageFormat){
	case QImage::Format_RGB32:
		return PF_RGB;

	case QImage::Format_ARGB32:
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


} // namespace iqt


