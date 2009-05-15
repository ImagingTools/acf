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
	void SetSampleValue(const ElementIndex& index, double value);
	
	// reimplemented (ISampledFunction2d)
	virtual bool CreateFunction(double* dataPtr, int width, int height);
	virtual int GetSamplesCount() const;
	virtual istd::CRange GetIntervalRange(int dimensionIndex) const;
	virtual istd::CRange GetValueRange(int dimensionIndex) const;

	// reimplemented (TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;

private:
	typedef istd::TArray<double, 2> SamplesContainer;

	SamplesContainer m_samplesContainer;
};


} // namespace imath


#endif // !imath_CSampledFunction2d_included


