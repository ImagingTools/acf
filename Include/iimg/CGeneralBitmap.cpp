#include "iimg/CGeneralBitmap.h"


#include <memory.h>

#include "istd/TChangeNotifier.h"



namespace iimg
{


CGeneralBitmap::CGeneralBitmap()
:	m_linesDifference(0), m_pixelBitsCount(0), m_componentsCount(0)
{
}


// reimplemented (iimg::IBitmap)

bool CGeneralBitmap::CreateBitmap(const istd::CIndex2d& size, int pixelBitsCount, int componentsCount)
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

	int bufferSize = m_linesDifference * size.GetY();
	if (bufferSize > 0){
		m_buffer.SetPtr(new I_BYTE[bufferSize], true);

		return m_buffer.IsValid();
	}
	else{
		m_buffer.Reset();

		return true;
	}
}


bool CGeneralBitmap::CreateBitmap(const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference, int pixelBitsCount, int componentsCount)
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

	if (linesDifference != 0){
		m_linesDifference = linesDifference;
	}
	else{
		m_linesDifference = (pixelBitsCount * size.GetX() + 7) >> 3;
	}

	m_buffer.SetPtr((I_BYTE*)dataPtr, releaseFlag);

	return true;
}


int CGeneralBitmap::GetLinesDifference() const
{
	return m_linesDifference;
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


bool CGeneralBitmap::CopyImageFrom(const IRasterImage& image)
{
	const IBitmap* bitmapPtr = dynamic_cast<const IBitmap*>(&image);
	if (bitmapPtr != NULL){
		istd::CIndex2d size = bitmapPtr->GetImageSize();
		if (CreateBitmap(size, bitmapPtr->GetPixelBitsCount(), bitmapPtr->GetComponentsCount())){
			int lineBytesCount = istd::Min(GetLineBytesCount(), bitmapPtr->GetLineBytesCount());
			for (int y = 0; y < size.GetY(); ++y){
				::memcpy(GetLinePtr(y), bitmapPtr->GetLinePtr(y), lineBytesCount);
			}

			return true;
		}
	}

	return false;
}


} // namespace iimg


