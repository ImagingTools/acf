#include <iimg/CBitmapBase.h>


// STL includes
#include <cstring>

// Qt includes
#include <QtCore/qmath.h>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <ibase/CSize.h>


namespace iimg
{


// reimplemented (i2d::IObject2d)

i2d::CVector2d CBitmapBase::GetCenter() const
{
	istd::CIndex2d imageSize = GetImageSize();

	return i2d::CVector2d(imageSize.GetX() * 0.5, imageSize.GetY() * 0.5);
}


void CBitmapBase::MoveCenterTo(const i2d::CVector2d& /*position*/)
{
}


i2d::CRectangle CBitmapBase::GetBoundingBox() const
{
	istd::CIndex2d imageSize = GetImageSize();

	return i2d::CRectangle(0, 0, imageSize.GetX(), imageSize.GetY());
}


// reimplemented (iimg::IBitmap)

bool CBitmapBase::CreateImageFromRegion(const iimg::IBitmap& sourceBitmap, const i2d::CRect& region)
{
	istd::CIndex2d sourceImageSize = sourceBitmap.GetImageSize();

	i2d::CRect usedRegion = region;
	usedRegion.Intersection(i2d::CRect(sourceImageSize));
	if (usedRegion.IsEmpty()){
		return false;
	}

	if (!CreateBitmap(sourceBitmap.GetPixelFormat(), usedRegion.GetSize())){
		return false;
	}

	int top  = usedRegion.GetTop();
	int left = usedRegion.GetLeft();

	int bytesPerPixel = ((GetPixelBitsCount() + 7) / 8);

	istd::CIndex2d size = GetImageSize();
	int lineBytesCount = GetLineBytesCount();
	for (int y = 0; y < size.GetY(); ++y){
		quint8* destLinePtr = (quint8*)GetLinePtr(y);
		quint8* sourceLinePtr = ((quint8*)sourceBitmap.GetLinePtr(y + top)) + left * bytesPerPixel;

		std::memcpy(destLinePtr, sourceLinePtr, lineBytesCount);
	}

	return true;
}


int CBitmapBase::GetLineBytesCount() const
{
	istd::CIndex2d size = GetImageSize();
	int bytesPerPixel = ((GetPixelBitsCount() + 7) / 8);

	return bytesPerPixel * size.GetX();
}


int CBitmapBase::GetComponentBitsCount(int /*componentIndex*/) const
{
	int componentsCount = GetComponentsCount();

	if (componentsCount > 0){
		return GetPixelBitsCount() / componentsCount;
	}
	else{
		return 0;
	}
}


// reimplemented (iimg::IRasterImage)

bool CBitmapBase::IsEmpty() const
{
	istd::CIndex2d size = GetImageSize();

	return (size.GetX() <= 0) || (size.GetY() <= 0);
}


icmm::CVarColor CBitmapBase::GetColorAt(const istd::CIndex2d& position) const
{
	Q_ASSERT(position.IsValid());
	Q_ASSERT(position.IsInside(GetImageSize()));

	int componentsCount = GetComponentsCount();

	icmm::CVarColor retVal(componentsCount);

	int byteOffsetX = (GetPixelBitsCount() * position.GetX()) >> 3;

	quint8* pixelPtr = (quint8*)GetLinePtr(position.GetY());
	Q_ASSERT(pixelPtr != NULL);
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
	Q_ASSERT(position.IsValid());
	Q_ASSERT(position.IsInside(GetImageSize()));

	int componentsCount = GetComponentsCount();

	int byteOffsetX = (GetPixelBitsCount() * position.GetX()) >> 3;

	quint8* pixelPtr = (quint8*)GetLinePtr(position.GetY());
	Q_ASSERT(pixelPtr != NULL);
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
	static iser::CArchiveTag headerTag("BitmapHeader", "Header of bitmap", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag sizeTag("Size", "Size of bitmap", iser::CArchiveTag::TT_GROUP, &headerTag);
	static iser::CArchiveTag sizeXTag("X", "Bitmap width", iser::CArchiveTag::TT_LEAF, &sizeTag);
	static iser::CArchiveTag sizeYTag("Y", "Bitmap height", iser::CArchiveTag::TT_LEAF, &sizeTag);
	static iser::CArchiveTag pixelFormatTag("PixelFormat", "Pixel format", iser::CArchiveTag::TT_LEAF, &headerTag);
	static iser::CArchiveTag dataTag("BitmapData", "Bitmap data section", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag lineTag("Line", "Single bitmap line", iser::CArchiveTag::TT_GROUP, &dataTag);

	bool isStoring = archive.IsStoring();

	bool retVal = true;

	retVal = retVal && archive.BeginTag(headerTag);

	istd::CIndex2d size;
	int pixelFormat = PF_UNKNOWN;

	if (isStoring){
		size = GetImageSize();
		pixelFormat = GetPixelFormat();
	}

	retVal = retVal && archive.BeginTag(sizeTag);

	retVal = retVal && archive.BeginTag(sizeXTag);
	retVal = retVal && archive.Process(size[0]);
	retVal = retVal && archive.EndTag(sizeXTag);

	retVal = retVal && archive.BeginTag(sizeYTag);
	retVal = retVal && archive.Process(size[1]);
	retVal = retVal && archive.EndTag(sizeYTag);

	retVal = retVal && archive.EndTag(sizeTag);

	retVal = retVal && archive.BeginTag(pixelFormatTag);
	retVal = retVal && archive.Process(pixelFormat);
	retVal = retVal && archive.EndTag(pixelFormatTag);

	retVal = retVal && archive.EndTag(headerTag);

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	if (!isStoring){
		if (!size.IsZero()){
			if (!CreateBitmap(PixelFormat(pixelFormat), size)){
				return false;
			}
		}
	}

	retVal = retVal && archive.BeginTag(dataTag);

	int lineBytesCount = GetLineBytesCount();

	for (int lineIndex = 0; lineIndex < size.GetY(); ++lineIndex){
		void* linePtr = GetLinePtr(lineIndex);

		retVal = retVal && archive.BeginTag(lineTag);
		retVal = retVal && archive.ProcessData(linePtr, lineBytesCount);
		retVal = retVal && archive.EndTag(lineTag);
	}

	retVal = retVal && archive.EndTag(dataTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CBitmapBase::ResetData(CompatibilityMode /*mode*/)
{
	ResetImage();

	return true;
}


} // namespace iimg


