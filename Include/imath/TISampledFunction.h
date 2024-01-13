#pragma once


// ACF includes
#include <istd/IChangeable.h>
#include <istd/TIndex.h>
#include <istd/TRange.h>
#include <imath/TIMathFunction.h>
#include <imath/TVector.h>


namespace imath
{


/**
	Interface for a resampled function with a regular grid design.
*/
template <int ArgumentDimensions, int ResultDimensions>
class TISampledFunction: 
			virtual public imath::TIMathFunction<
						istd::TIndex<ArgumentDimensions>, 
						TVector<ResultDimensions> >,
			virtual public istd::IChangeable
{
public:
	typedef imath::TIMathFunction<
				istd::TIndex<ArgumentDimensions>, 
				imath::TVector<ResultDimensions> > BaseClass;
	typedef typename BaseClass::ArgumentType ArgumentType;
	typedef typename BaseClass::ResultType ResultType;
	typedef istd::TIndex<ArgumentDimensions> SampleIndex;

	/**
		Create function from input data.
	*/
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes) = 0;

	/**
		Get number of samples stored in this container.
	*/
	virtual int GetTotalSamplesCount() const = 0;

	/**
		Get number of samples for specified dimension.
	*/
	virtual int GetGridSize(int dimensionIndex) const = 0;

	/**
		Get sample value at the given index.
	*/
	virtual double GetSampleAt(const SampleIndex& index) const = 0;

	/**
		Get logical range for the specified dimension.
		Logical range defines boundaries of the function domain in the given dimension.
	*/
	virtual istd::CRange GetLogicalRange(int dimensionIndex) const = 0;

	/**
		Get the range (boundary) of possible result values for the given axis.
		Optionally some result dimension can be also specified.
		\param	dimensionIndex	index of argument dimension.
		\param	resultDimension	optional index of result dimension, it can be negative if all dimensions are meaned.
		\return	range of possible values, or invalid range if it is unknown.
	*/
	virtual istd::CRange GetResultValueRange(int dimensionIndex, int resultDimension = -1) const = 0;

	/**
		Get the number of argument components.
	*/
	virtual int GetArgumentDimensionality() const;

	/**
		Get the number of result components.
	*/
	virtual int GetResultDimensionality() const;
};


template <int ArgumentDimensions, int ResultDimensions>
inline int TISampledFunction<ArgumentDimensions, ResultDimensions>::GetArgumentDimensionality() const
{
	return ArgumentDimensions;
}
	

template <int ArgumentDimensions, int ResultDimensions>
inline int TISampledFunction<ArgumentDimensions, ResultDimensions>::GetResultDimensionality() const
{
	return ResultDimensions;
}


// standard templatization

typedef TISampledFunction<1, 1> ISampledFunction;
typedef TISampledFunction<2, 1> ISampledFunction2d;


} // namespace imath


