#include <iimg/CGeneralBitmap.h>


// STL includes
#include <cstring>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>


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


IBitmap::PixelFormat CGeneralBitmap::GetPixelFormat() const
{
	return m_pixelFormat;
}


bool CGeneralBitmap::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, int pixelBitsCount, int componentsCount)
{
	switch(pixelFormat){
	case PF_GRAY:
		componentsCount = 1;
		pixelBitsCount = 8;
		break;

	case PF_RGB:
	case PF_RGBA:
		componentsCount = 4;
		pixelBitsCount = 32;
		break;

	case PF_GRAY16:
		componentsCount = 1;
		pixelBitsCount = 16;
		break;

	case PF_GRAY32:
		componentsCount = 1;
		pixelBitsCount = 32;
		break;

	case PF_FLOAT32:
		componentsCount = 1;
		pixelBitsCount = 32;
		break;

	case PF_FLOAT64:
		componentsCount = 1;
		pixelBitsCount = 64;
		break;

	case PF_USER:
		break;

	default:
		return false;
	}

	// Unknown pixel format and missing user format specification:
	if ((componentsCount <= 0) || (pixelBitsCount <= 0)){
		return false;
	}

	return CreateBitmap(size, pixelBitsCount, componentsCount, pixelFormat);
}


bool CGeneralBitmap::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference)
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
	Q_ASSERT(positionY >= 0);
	Q_ASSERT(positionY < m_size.GetY());

	return m_buffer.GetPtr() + m_linesDifference * positionY;
}


void* CGeneralBitmap::GetLinePtr(int positionY)
{
	Q_ASSERT(positionY >= 0);
	Q_ASSERT(positionY < m_size.GetY());

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


void CGeneralBitmap::ClearImage()
{
	if (m_size.IsSizeEmpty() || !m_buffer.IsValid()){
		return;
	}

	// we have to do this line by line because line addresses are not obligatory plain.
	int lineSize = GetLineBytesCount();
	for (int y = 0; y < m_size.GetY(); y ++){
		quint8* lineBufferPtr = (quint8*)GetLinePtr(y);
		std::memset(lineBufferPtr, 0, lineSize); 
	}
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


bool CGeneralBitmap::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const IBitmap* bitmapPtr = dynamic_cast<const IBitmap*>(&object);
	if (bitmapPtr != NULL){
		istd::CChangeNotifier notifier(this);

		istd::CIndex2d size = bitmapPtr->GetImageSize();
		if (CreateBitmap(bitmapPtr->GetPixelFormat(), size)){
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

	return false;
}


istd::IChangeable* CGeneralBitmap::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<CGeneralBitmap> clonePtr(new CGeneralBitmap);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// protected methods

bool CGeneralBitmap::CreateBitmap(
			const istd::CIndex2d& size,
			int pixelBitsCount,
			int componentsCount,
			PixelFormat pixelFormat)
{
	if (		(size.GetX() < 0) ||
				(size.GetY() < 0) ||
				(componentsCount <= 0) ||
				(pixelBitsCount <= 0) ||
				(pixelBitsCount % (componentsCount * 8) != 0)){
		return false;
	}

	if ((size == m_size) && (pixelBitsCount == m_pixelBitsCount) && (componentsCount == m_componentsCount) && (pixelFormat = m_pixelFormat)){
		return true;	// nothing to do
	}

	int oldBufferSize = m_linesDifference * size.GetY();

	int linesDifference = (pixelBitsCount * size.GetX() + 7) >> 3;
	Q_ASSERT(linesDifference >= 0);
	int bufferSize = linesDifference * size.GetY();

	istd::CChangeNotifier notifier(this);

	m_size = size;
	m_pixelBitsCount = pixelBitsCount;
	m_componentsCount = componentsCount;
	m_pixelFormat = pixelFormat;
	m_linesDifference = linesDifference;

	if (oldBufferSize != bufferSize){
		if (bufferSize > 0){
			m_buffer.SetPtr(new quint8[bufferSize], true);

			return m_buffer.IsValid();
		}
		else{
			m_buffer.Reset();
		}
	}

	return true;
}


bool CGeneralBitmap::CreateBitmap(
			const istd::CIndex2d& size,
			void* dataPtr,
			bool releaseFlag,
			int linesDifference,
			int pixelBitsCount,
			int componentsCount,
			PixelFormat pixelFormat)
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


