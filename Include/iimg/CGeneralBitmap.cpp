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
	m_pixelFormat(PF_UNKNOWN)
{
}


CGeneralBitmap::CGeneralBitmap(const CGeneralBitmap& bitmap)
{
	if (!bitmap.m_buffer.IsToRelase()){
		// copy structure refering to external buffer
		m_buffer.SetPtr(bitmap.m_buffer.GetPtr(), false);
		m_size = bitmap.m_size;
		m_linesDifference = bitmap.m_linesDifference;
		m_pixelFormat = bitmap.m_pixelFormat;
	}
	else{
		// copy structure refering to external buffer
		if (CGeneralBitmap::CreateBitmap(bitmap.m_size, bitmap.GetPixelBitsCount(), bitmap.GetComponentsCount(), bitmap.m_pixelFormat)){
			int linesSize = (GetPixelBitsCount() * m_size.GetX() + 7) >> 3;

			for (int y = 0; y < m_size.GetY(); ++y){
				const void* sourceLinePtr = bitmap.GetLinePtr(y);
				void* destLinePtr = CGeneralBitmap::GetLinePtr(y);

				std::memcpy(destLinePtr, sourceLinePtr, linesSize);
			}
		}
	}
}


// reimplemented (iimg::IBitmap)

bool CGeneralBitmap::IsFormatSupported(PixelFormat pixelFormat) const
{
	return pixelFormat != PF_MONO && pixelFormat != PF_RGB24;
}


IBitmap::PixelFormat CGeneralBitmap::GetPixelFormat() const
{
	return m_pixelFormat;
}


bool CGeneralBitmap::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, int, int)
{
	if (!IsFormatSupported(pixelFormat))
		return false;

	return CreateBitmap(size, GetPixelBitsCount(pixelFormat), GetComponentsCount(pixelFormat), pixelFormat);
}


bool CGeneralBitmap::CreateBitmap(PixelFormat pixelFormat, const istd::CIndex2d& size, void* dataPtr, bool releaseFlag, int linesDifference)
{
	if (!IsFormatSupported(pixelFormat))
		return false;

	return CreateBitmap(size, dataPtr, releaseFlag, linesDifference, GetPixelBitsCount(pixelFormat), GetComponentsCount(pixelFormat), pixelFormat);
}


// reimplemented (iimg::IRasterImage)

void CGeneralBitmap::ResetImage()
{
	istd::CChangeNotifier notifier(this);

	m_size.Reset();
	m_buffer.Reset();
	m_linesDifference = 0;
}


void CGeneralBitmap::ClearImage()
{
	if (m_size.IsSizeEmpty() || !m_buffer.IsValid()){
		return;
	}

	istd::CChangeNotifier notifier(this);

	// we have to do this line by line because line addresses are not obligatory plain.
	int lineSize = GetLineBytesCount();
	for (int y = 0; y < m_size.GetY(); y ++){
		quint8* lineBufferPtr = (quint8*)GetLinePtr(y);

		std::memset(lineBufferPtr, 0, lineSize); 
	}
}


// reimplemented (istd::IChangeable)

int CGeneralBitmap::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CGeneralBitmap::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
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

			return BaseClass::CopyFrom(object, mode);
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


CGeneralBitmap& CGeneralBitmap::operator=(const CGeneralBitmap& bitmap)
{
	istd::CChangeNotifier notifier(this);

	if (!bitmap.m_buffer.IsToRelase()){
		// copy structure refering to external buffer
		m_buffer.SetPtr(bitmap.m_buffer.GetPtr(), false);
		m_size = bitmap.m_size;
		m_linesDifference = bitmap.m_linesDifference;
		m_pixelFormat = bitmap.m_pixelFormat;
	}
	else{
		// copy structure refering to external buffer
		if (CGeneralBitmap::CreateBitmap(bitmap.m_size, bitmap.GetPixelBitsCount(), bitmap.GetComponentsCount(), bitmap.m_pixelFormat)){
			int linesSize = (GetPixelBitsCount() * m_size.GetX() + 7) >> 3;

			for (int y = 0; y < m_size.GetY(); ++y){
				const void* sourceLinePtr = bitmap.GetLinePtr(y);
				void* destLinePtr = CGeneralBitmap::GetLinePtr(y);

				std::memcpy(destLinePtr, sourceLinePtr, linesSize);
			}
		}
	}
	return *this;
}


bool CGeneralBitmap::operator==(const CGeneralBitmap& bitmap) const
{
	if (m_size != bitmap.m_size){
		return false;
	}

	if (m_pixelFormat != bitmap.m_pixelFormat){
		return false;
	}

	if (GetPixelBitsCount() != bitmap.GetPixelBitsCount()){
		return false;
	}

	if (m_buffer.IsValid() && bitmap.m_buffer.IsValid() && (m_buffer.GetPtr() != bitmap.m_buffer.GetPtr())){
		int lineBytes = (GetPixelBitsCount() * m_size.GetX() + 7) >> 3;

		for (int y = 0; y < m_size.GetY(); ++y){
			if (std::memcmp(GetLinePtr(y), bitmap.GetLinePtr(y), lineBytes) != 0){
				return false;
			}
		}
	}

	return true;
}


bool CGeneralBitmap::operator!=(const CGeneralBitmap& bitmap) const
{
	return !operator==(bitmap);
}


// protected methods

bool CGeneralBitmap::CreateBitmap(
			const istd::CIndex2d& size,
			int pixelBitsCount,
			int componentsCount,
			PixelFormat pixelFormat)
{
	if ((size.GetX() < 0) || (size.GetY() < 0) || (componentsCount <= 0) || (pixelBitsCount <= 0) || (pixelBitsCount % (componentsCount * 8) != 0))
		return false;

	if (m_buffer.IsToRelase() && size == m_size && pixelBitsCount == GetPixelBitsCount() && componentsCount == GetComponentsCount() && pixelFormat == m_pixelFormat)
		return true;	// nothing to do

	int linesDifference = (pixelBitsCount * size.GetX() + 7) >> 3;
	Q_ASSERT(linesDifference >= 0);
	int bufferSize = linesDifference * size.GetY();

	istd::CChangeNotifier notifier(this);

	m_size = size;
	m_pixelFormat = pixelFormat;
	m_linesDifference = linesDifference;

	if (bufferSize > 0){
		try {
			uint8_t* buff = new quint8[bufferSize];
			m_buffer.SetPtr(buff, true);
		}
		catch (...)
		{
			ResetData();
		}

		return m_buffer.IsValid();
	}
	else{
		m_buffer.Reset();
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
	if (!IsFormatSupported(pixelFormat))
		return false;

	if (		(size.GetX() < 0) ||
				(size.GetY() < 0) ||
				(componentsCount <= 0) ||
				(pixelBitsCount <= 0) ||
				(pixelBitsCount % (componentsCount * 8) != 0)){
		return false;
	}

	istd::CChangeNotifier notifier(this);

	m_size = size;
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


