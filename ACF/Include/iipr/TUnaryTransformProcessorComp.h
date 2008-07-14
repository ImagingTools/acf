#ifndef iipr_TUnaryTransformProcessorComp_included
#define iipr_TUnaryTransformProcessorComp_included


#include <functional>


#include "iimg/TBitmapIterator.h"

#include "iipr/TImageProcessorCompBase.h"


namespace iipr
{
	

template <typename ArgumentType>
class InvertFunction: public std::unary_function<ArgumentType, ArgumentType>
{
public:
	InvertFunction()
	{
		m_maxValue = std::numeric_limits<ArgumentType>::max();
	}

	inline ArgumentType operator()(const ArgumentType& value)
	{
		return (m_maxValue - value);
	}

private:
	ArgumentType m_maxValue;
};


/**
	Implementation of general morphological operator.
*/
template <typename PixelType, class Predicate> 
class TUnaryTransformProcessorComp: public iipr::TImageProcessorCompBase<istd::IPolymorphic>
{
public:
	typedef iipr::TImageProcessorCompBase<istd::IPolymorphic> BaseClass;

protected:		
	// reimplemented (iipr::TImageProcessorCompBase<istd::IPolymorphic>)
	virtual bool ProcessImage(
				const istd::IPolymorphic* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	Predicate m_predicate;
};


// reimplemented (iipr::TImageProcessorCompBase<istd::IPolymorphic>)

template <typename PixelType, class Predicate> 
bool TUnaryTransformProcessorComp<PixelType, Predicate>::ProcessImage(
			const istd::IPolymorphic* /*paramsPtr*/,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	bool retVal = true;
	
	iimg::TBitmapIterator<PixelType> inputIterator(&inputImage);
	iimg::TBitmapIterator<PixelType> outputIterator(&outputImage);

	while (inputIterator.IsValid()){
		*outputIterator = m_predicate(*inputIterator);

		++inputIterator;
		++outputIterator;
	}

	return retVal;
}


typedef TUnaryTransformProcessorComp<I_BYTE, InvertFunction<I_BYTE> > CInvertOperatorComp;


} // namespace iipr


#endif // !iipr_TUnaryTransformProcessorComp_included


