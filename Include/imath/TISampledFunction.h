#ifndef imath_TISampledFunction_included
#define imath_TISampledFunction_included


#include "istd/IChangeable.h"
#include "istd/TIndex.h"
#include "istd/CRange.h"

#include "imath/TIMathFunction.h"
#include "imath/TVector.h"


namespace imath
{

/**
	Interface for a resampled function with a regular grid design.
*/
template <int InputDimensions, int OutputDimensions>
class TISampledFunction: 
			virtual public imath::TIMathFunction<
						istd::TIndex<InputDimensions>, 
						imath::TVector<OutputDimensions> >,
			virtual public istd::IChangeable
{
public:
	typedef imath::TIMathFunction<
				istd::TIndex<InputDimensions>, 
				imath::TVector<OutputDimensions> > BaseClass;

	typedef istd::TIndex<InputDimensions> ElementIndex;
	typedef imath::TVector<OutputDimensions> ResultValue;

	/**
		Create function from input data.
	*/
	virtual bool CreateFunction(double* dataPtr, int width, int height) = 0;

	/**
		Get number of samples stored in this container.
	*/
	virtual int GetSamplesCount() const = 0;

	/**
		Get function definition intervall for the given axis.
	*/
	virtual istd::CRange GetIntervalRange(int dimensionIndex) const = 0;

	/**
		Get function value range for the given axis.
	*/
	virtual istd::CRange GetValueRange(int dimensionIndex) const = 0;

	/**
		Get the number of argument components.
	*/
	virtual int GetArgumentDimensionality() const;

	/**
		Get the number of result components.
	*/
	virtual int GetResultDimensionality() const;
};


template <int InputDimensions, int OutputDimensions>
inline int TISampledFunction<InputDimensions, OutputDimensions>::GetArgumentDimensionality() const
{
	return InputDimensions;
}
	

template <int InputDimensions, int OutputDimensions>
inline int TISampledFunction<InputDimensions, OutputDimensions>::GetResultDimensionality() const
{
	return OutputDimensions;
}


// standard templatization

typedef TISampledFunction<1, 1> ISampledFunction;
typedef TISampledFunction<2, 1> ISampledFunction2d;


} // namespace imath


#endif // !imath_TISampledFunction_included


