#ifndef imath_CSampledFunction2d_included
#define imath_CSampledFunction2d_included


#include <istd/TArray.h>
#include <istd/CIndex2d.h>

#include <imath/TISampledFunction.h>


namespace imath
{


/**
	Implementation of the resampled 2D-function.
*/
class CSampledFunction2d: virtual public ISampledFunction2d
{
public:
	CSampledFunction2d();
	CSampledFunction2d(const CSampledFunction2d& function2d);
	CSampledFunction2d(const imath::ISampledFunction2d& function2d);
	CSampledFunction2d(const istd::CIndex2d& size, double defaultValue = 0);

	void Reset();
	bool CreateGrid2d(const istd::CIndex2d& size, double defaultValue = 0);
	istd::CIndex2d GetGridSize2d() const;
	double GetSampleValue(const istd::CIndex2d& index) const;
	void SetSampleValue(const istd::CIndex2d& index, double value);
	
	// reimplemented (imath::ISampledFunction2d)
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes);
	virtual int GetTotalSamplesCount() const;
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


// public inline methods

inline istd::CIndex2d CSampledFunction2d::GetGridSize2d() const
{
	return m_samplesContainer.GetSizes();
}


inline double CSampledFunction2d::GetSampleValue(const istd::CIndex2d& index) const
{
	Q_ASSERT(index.IsInside(m_samplesContainer.GetSizes()));

	return m_samplesContainer.GetAt(index);
}


inline void CSampledFunction2d::SetSampleValue(const istd::CIndex2d& index, double value)
{
	Q_ASSERT(index.IsInside(m_samplesContainer.GetSizes()));

	m_samplesContainer.SetAt(index, value);
}


} // namespace imath


#endif // !imath_CSampledFunction2d_included


