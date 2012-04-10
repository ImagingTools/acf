#include "iimg/CGeneralBitmap.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TDelPtr.h"



namespace iimg
{


CGeneralBitmap::CGeneralBitmap()
:	m_linesDifference(0),
	m_pixelBitsCount(0),
	m_componentsCount(0),
	m_pixelFormat(PF_UNKNOWN)
{
}


// reimplemented (iimg::IBitmap)

bool CGeneralBitmap::IsFormatSupported(PixelFormat /*pixelFormat*/) const
{
	return true;
}


int CGeneralBitmap::GetPixelFormat() const
{
	return m_pixelFormat;
}


bool CGeneralBitmap::CreateBitmap(int pixelFormat, const istd::CIndex2d& size)
{
	switch(pixelFormat){
	case PF_GRAY:
		return CreateBitmap(size, 8, 1, pixelFormat);

	case PF_RGB:
	case PF_RGBA:
		return CreateBitmap(size, 32, 4, pixelFormat);

	case PF_GRAY16:
		return CreateBitmap(size, 16, 1, pixelFormat);

	case PF_GRAY32:
		return CreateBitmap(size, 32, 1, pixelFormat);

	default:
		return false;
	}
}


bool CGeneralBitmap::CreateBitmap(int pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference)
{
	switch(pixelFormat){
		case PF_GRAY:
			return CreateBitmap(size, dataPtr, releaseFlag, linesDifference, 8, 1, PF_GRAY);

		case PF_RGB:
		case PF_RGBA:
			return CreateBitmap(size, dataPtr, releaseFlag, linesDifference, 32, 4, pixelFormat);

		case PF_GRAY16:
			return CreateBitmap(size, dataPtr, releaseFlag, linesDifference, 16, 1, PF_GRAY16);

		case PF_GRAY32:
			return CreateBitmap(size, dataPtr, releaseFlag, linesDifference, 32, 1, PF_GRAY32);

		default:
			return false;
	}
}


int CGeneralBitmap::GetLinesDifference() const
{
	return m_linesDifference;
}


int CGeneralBitmap::GetPixelsDifference() const
{
	return m_pixelBitsCount >> 3;
}


int CGeneralBitmap::GetPixelBitsCount() const
{
	return m_pixelBitsCount;
}


const void* CGeneralBitmap::GetLinePtr(int positionY) const
{
	return m_buffer.GetPtr() + m_linesDifference * positionY;
}


void* CGeneralBitmap::GetLinePtr(int positionY)
{
	return m_buffer.GetPtr() + m_linesDifference * positionY;
}


// reimplemented (iimg::IRasterImage)

void CGeneralBitmap::ResetImage()
{
	m_size.Reset();
	m_buffer.Reset();
	m_linesDifference = 0;
	m_pixelBitsCount = 0;
	m_componentsCount = 0;
}


istd::CIndex2d CGeneralBitmap::GetImageSize() const
{
	return m_size;
}


int CGeneralBitmap::GetComponentsCount() const
{
	return m_componentsCount;
}


// reimplemented (istd::IChangeable)

int CGeneralBitmap::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CGeneralBitmap::CopyFrom(const istd::IChangeable& object)
{
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

	return false;
}


istd::IChangeable* CGeneralBitmap::CloneMe() const
{
	istd::TDelPtr<CGeneralBitmap> clonePtr(new CGeneralBitmap);

	if (clonePtr->CopyFrom(*this)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// protected methods

bool CGeneralBitmap::CreateBitmap(
			const istd::CIndex2d& size,
			int pixelBitsCount,
			int componentsCount,
			int pixelFormat)
{
	if (		(size.GetX() < 0) ||
				(size.GetY() < 0) ||
				(componentsCount <= 0) ||
				(pixelBitsCount <= 0) ||
				(pixelBitsCount % (componentsCount * 8) != 0)){
		return false;
	}

	istd::CChangeNotifier notifier(this);

	m_linesDifference = (pixelBitsCount * size.GetX() + 7) >> 3;
	I_ASSERT(m_linesDifference >= 0);

	m_size = size;
	m_pixelBitsCount = pixelBitsCount;
	m_componentsCount = componentsCount;
	m_pixelFormat = pixelFormat;

	int bufferSize = m_linesDifference * size.GetY();
	if (bufferSize > 0){
		m_buffer.SetPtr(new quint8[bufferSize], true);

		return m_buffer.IsValid();
	}
	else{
		m_buffer.Reset();

		return true;
	}
}


bool CGeneralBitmap::CreateBitmap(
			const istd::CIndex2d& size,
			void* dataPtr,
			bool releaseFlag,
			int linesDifference,
			int pixelBitsCount,
			int componentsCount,
			int pixelFormat)
{
	if (		(size.GetX() < 0) ||
				(size.GetY() < 0) ||
				(componentsCount <= 0) ||
				(pixelBitsCount <= 0) ||
				(pixelBitsCount % (componentsCount * 8) != 0)){
		return false;
	}

	istd::CChangeNotifier notifier(this);

	m_size = size;
	m_pixelBitsCount = pixelBitsCount;
	m_componentsCount = componentsCount;
	m_pixelFormat = pixelFormat;

	if (linesDifference != 0){
		m_linesDifference = linesDifference;
	}
	else{
		m_linesDifference = (pixelBitsCount * size.GetX() + 7) >> 3;
	}

	m_buffer.SetPtr((quint8*)dataPtr, releaseFlag);

	return true;
}


} // namespace iimg


