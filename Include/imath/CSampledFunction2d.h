#ifndef imath_CSampledFunction2d_included
#define imath_CSampledFunction2d_included


#include "istd/TArray.h"

#include "imath/TISampledFunction.h"


namespace imath
{


/**
	Implementation of the resampled 2D-function.
*/
class CSampledFunction2d: virtual public ISampledFunction2d
{
public:
	CSampledFunction2d(int width = -1, int height = -1, double defaultValue = 0);

	void Reset();
	void Create(int width, int height, double defaultValue = 0);
	void SetSampleValue(const ArgumentType& index, double value);
	
	// reimplemented (imath::ISampledFunction2d)
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes);
	virtual int GetSamplesCount() const;
	virtual int GetGridSize(int dimensionIndex) const;
	virtual istd::CRange GetLogicalRange(int dimensionIndex) const;
	virtual istd::CRange GetResultValueRange(int dimensionIndex, int resultDimension = -1) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;

private:
	typedef istd::TArray<double, 2> SamplesContainer;

	SamplesContainer m_samplesContainer;
};


} // namespace imath


#endif // !imath_CSampledFunction2d_included


