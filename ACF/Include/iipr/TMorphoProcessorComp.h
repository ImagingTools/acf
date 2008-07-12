#ifndef iipr_TMorphoProcessorComp_included
#define iipr_TMorphoProcessorComp_included


#include <functional>


#include "iimg/TBitmapIterator.h"

#include "iipr/TImageProcessorCompBase.h"
#include "iipr/CRectangularFilterParams.h"


namespace iipr
{
	

class MinimumFunction: public std::binary_function<I_BYTE, I_BYTE, I_BYTE>
{
public:
	inline I_BYTE operator()(const I_BYTE& value1, const I_BYTE& value2)
	{
		return istd::Min(value1, value2);
	}
};


class MaximumFunction: public std::binary_function<I_BYTE, I_BYTE, I_BYTE>
{
public:
	inline I_BYTE operator()(const I_BYTE& value1, const I_BYTE& value2)
	{
		return istd::Max(value1, value2);
	}
};


/**
	Implementation of general morphological operator.
*/
template <typename PixelType, class CompareFunction> 
class TMorphoProcessorComp: public iipr::TImageProcessorCompBase<iipr::CRectangularFilterParams>
{
public:
	typedef iipr::TImageProcessorCompBase<iipr::CRectangularFilterParams> BaseClass;

protected:		
	// reimplemented (iipr::TImageProcessorCompBase<iipr::CRectangularFilterParams>)
	virtual bool ProcessImage(
				const iipr::CRectangularFilterParams* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	CompareFunction m_predicate;
};


// reimplemented (iipr::TImageProcessorCompBase<iipr::CRectangularFilterParams>)

template <typename PixelType, class CompareFunction> 
bool TMorphoProcessorComp<PixelType, CompareFunction>::ProcessImage(
			const iipr::CRectangularFilterParams* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (paramsPtr == NULL){
		return false;
	}

	bool retVal = true;
	
	int kernelWidth = paramsPtr->GetSize().GetX();
	int kernelHalfWidth = kernelWidth / 2;
	int imageWidth = inputImage.GetImageSize().GetX();
	
	int kernelHeight = paramsPtr->GetSize().GetY();
	int kernelHalfHeight = kernelHeight / 2;
	int imageHeight = inputImage.GetImageSize().GetY();

	for (int y = 0; y < imageHeight; y++){
		iimg::TBitmapIterator<I_BYTE> inputIterator(&inputImage);
		iimg::TBitmapIterator<I_BYTE> outputIterator(&outputImage);

		inputIterator.MoveTo(kernelHalfWidth, y);
		outputIterator.MoveTo(kernelHalfWidth, y);

		for (int x = kernelHalfWidth; x < imageWidth - kernelHalfWidth; x++){
			iimg::TBitmapIterator<I_BYTE> kernelIterator = inputIterator;
			PixelType outputValue = *kernelIterator;

			kernelIterator.x -= kernelHalfWidth;

			for (int mx = -kernelHalfWidth; mx <= kernelHalfWidth; mx++){
				PixelType kernelValue = *kernelIterator;

				outputValue  = m_predicate(outputValue, kernelValue);

				++kernelIterator.x;
			}

			*outputIterator = outputValue;
			++inputIterator.x;
			++outputIterator.x;
		}
	}

	iimg::CGeneralBitmap tempBitmap;
	tempBitmap.CopyImageFrom(outputImage);

	for (int x = 0; x < imageWidth; x++){
		iimg::TBitmapIterator<PixelType> inputIterator(&tempBitmap);
		iimg::TBitmapIterator<PixelType> outputIterator(&outputImage);

		inputIterator.MoveTo(x, kernelHalfHeight);
		outputIterator.MoveTo(x, kernelHalfHeight);

		for (int y = kernelHalfHeight; y < imageHeight - kernelHalfHeight; y++){
			iimg::TBitmapIterator<PixelType> kernelIterator = inputIterator;
		
			PixelType outputValue = *kernelIterator;

			kernelIterator.y -= kernelHalfHeight;

			for (int kernelIndex = -kernelHalfHeight; kernelIndex <= kernelHalfHeight; kernelIndex++){
				PixelType kernelValue = *kernelIterator;

				outputValue  = m_predicate(outputValue, kernelValue);

				++kernelIterator.y;
			}

			*outputIterator = outputValue;
			++inputIterator.y;
			++outputIterator.y;
		}
	}

	return retVal;
}


typedef TMorphoProcessorComp<I_BYTE, MaximumFunction> CMorphoMaxOperatorComp;
typedef TMorphoProcessorComp<I_BYTE, MinimumFunction> CMorphoMinOperatorComp;


} // namespace iipr


#endif // !iipr_TMorphoProcessorComp_included


