#include "iimg/CBitmapBase.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "ibase/CSize.h"


namespace iimg
{


bool CBitmapBase::CopyBitmapRegion(const iimg::IBitmap& sourceBitmap, const i2d::CRectangle& area)
{
	istd::CIndex2d sourceImageSize = sourceBitmap.GetImageSize();
	i2d::CRectangle bitmapAoi;

	bitmapAoi.SetLeft(::floor(area.GetLeft()));
	bitmapAoi.SetRight(::ceil(area.GetRight()));
	bitmapAoi.SetTop(::floor(area.GetTop()));
	bitmapAoi.SetBottom(::ceil(area.GetBottom()));

	bitmapAoi = bitmapAoi.GetIntersection(i2d::CRectangle(sourceImageSize));
	if (bitmapAoi.IsEmpty()){
		return false;
	}

	if (!CreateBitmap(sourceBitmap.GetPixelFormat(), ibase::CSize((int)bitmapAoi.GetWidth(), (int)bitmapAoi.GetHeight()))){
		return false;
	}

	int top  = (int)bitmapAoi.GetTop();
	int left = (int)bitmapAoi.GetLeft();

	istd::CIndex2d size = GetImageSize();
	int lineBytesCount = GetLineBytesCount();
	for (int y = 0; y < size.GetY(); ++y){
		quint8* destLinePtr = (quint8*)GetLinePtr(y);
		quint8* sourceLinePtr = ((quint8*)sourceBitmap.GetLinePtr(y + top)) + left;

		std::memcpy(destLinePtr, sourceLinePtr, lineBytesCount);
	}

	return true;
}


// reimplemented (iimg::IBitmap)

int CBitmapBase::GetLineBytesCount() const
{
	istd::CIndex2d size = GetImageSize();
	int bytesPerPixel = ((GetPixelBitsCount() + 7) / 8);

	return bytesPerPixel * size.GetX();
}


int CBitmapBase::GetComponentBitsCount(int /*componentIndex*/) const
{
	I_ASSERT(GetComponentsCount() > 0);

	return GetPixelBitsCount() / GetComponentsCount();
}


// reimplemented (iimg::IRasterImage)

bool CBitmapBase::IsEmpty() const
{
	istd::CIndex2d size = GetImageSize();

	return (size.GetX() <= 0) || (size.GetY() <= 0);
}


icmm::CVarColor CBitmapBase::GetColorAt(const istd::CIndex2d& position) const
{
	I_ASSERT(position.IsValid());
	I_ASSERT(position.IsInside(GetImageSize()));

	int componentsCount = GetComponentsCount();

	icmm::CVarColor retVal(componentsCount);

	int byteOffsetX = (GetPixelBitsCount() * position.GetX()) >> 3;

	quint8* pixelPtr = (quint8*)GetLinePtr(position.GetY());
	I_ASSERT(pixelPtr != NULL);
	pixelPtr += byteOffsetX;

	for (int i = 0; i < componentsCount; ++i){
		quint8 componentValue;
		if (GetComponentBitsCount(i) == 8){
			componentValue = pixelPtr[i];
		}
		else{
			componentValue = 0;
		}

		retVal.SetElement(i, componentValue / 255.0);
	}

	return retVal;
}


bool CBitmapBase::SetColorAt(const istd::CIndex2d& position, const icmm::CVarColor& color)
{
	I_ASSERT(position.IsValid());
	I_ASSERT(position.IsInside(GetImageSize()));

	int componentsCount = GetComponentsCount();

	int byteOffsetX = (GetPixelBitsCount() * position.GetX()) >> 3;

	quint8* pixelPtr = (quint8*)GetLinePtr(position.GetY());
	I_ASSERT(pixelPtr != NULL);
	pixelPtr += byteOffsetX;

	int commonComponentsCount = qMin(color.GetElementsCount(), componentsCount);
	for (int i = 0; i < commonComponentsCount; ++i){
		if (GetComponentBitsCount(i) != 8){
			return false;
		}

		int componentValue = int(color.GetElement(i) * 255);
		if (componentValue >= 0){
			if (componentValue <= 255){
				pixelPtr[i] = quint8(componentValue);
			}
			else{
				pixelPtr[i] = 255;
			}
		}
		else{
			pixelPtr[i] = 0;
		}
	}

	for (int i = commonComponentsCount; i < componentsCount; ++i){
		if (GetComponentBitsCount(i) != 8){
			return false;
		}

		pixelPtr[i] = 0;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CBitmapBase::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	bool isStoring = archive.IsStoring();

	static iser::CArchiveTag headerTag("BitmapHeader", "Header of bitmap");
	retVal = retVal && archive.BeginTag(headerTag);

	istd::CIndex2d size;
	int pixelFormat = PF_UNKNOWN;

	if (isStoring){
		size = GetImageSize();
		pixelFormat =  GetPixelFormat();
	}

	static iser::CArchiveTag sizeTag("Size", "Size of bitmap");
	retVal = retVal && archive.BeginTag(sizeTag);

	static iser::CArchiveTag sizeXTag("X", "Bitmap width");
	retVal = retVal && archive.BeginTag(sizeXTag);
	retVal = retVal && archive.Process(size[0]);
	retVal = retVal && archive.EndTag(sizeXTag);

	static iser::CArchiveTag sizeYTag("Y", "Bitmap height");
	retVal = retVal && archive.BeginTag(sizeYTag);
	retVal = retVal && archive.Process(size[1]);
	retVal = retVal && archive.EndTag(sizeYTag);

	retVal = retVal && archive.EndTag(sizeTag);

	static iser::CArchiveTag pixelFormatTag("PixelFormat", "Pixel format");
	retVal = retVal && archive.BeginTag(pixelFormatTag);
	retVal = retVal && archive.Process(pixelFormat);
	retVal = retVal && archive.EndTag(pixelFormatTag);

	retVal = retVal && archive.EndTag(headerTag);

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	if (!isStoring){
		if (!size.IsZero()){
			if (!CreateBitmap(pixelFormat, size)){
				return false;
			}
		}
	}

	static iser::CArchiveTag dataTag("BitmapData", "Bitmap data section");
	retVal = retVal && archive.BeginTag(dataTag);

	int lineBytesCount = GetLineBytesCount();

	for (int lineIndex = 0; lineIndex < size.GetY(); ++lineIndex){
		void* linePtr = GetLinePtr(lineIndex);

		static iser::CArchiveTag lineTag("Line", "Single bitmap line");
		retVal = retVal && archive.BeginTag(lineTag);
		retVal = retVal && archive.ProcessData(linePtr, lineBytesCount);
		retVal = retVal && archive.EndTag(lineTag);
	}

	retVal = retVal && archive.EndTag(dataTag);

	return retVal;
}


} // namespace iimg


