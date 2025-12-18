#pragma once


// ACF includes
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
	explicit CSampledFunction2d(const imath::ISampledFunction2d& function2d);
	explicit CSampledFunction2d(const istd::CIndex2d& size, double defaultValue = 0);

	void Reset();
	bool CreateGrid2d(const istd::CIndex2d& size, double defaultValue = 0);
	istd::CIndex2d GetGridSize2d() const;
	double GetSampleValue(const istd::CIndex2d& index) const;
	void SetSampleValue(const istd::CIndex2d& index, double value);
	
	// reimplemented (imath::ISampledFunction2d)
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes) override;
	virtual int GetTotalSamplesCount() const override;
	virtual int GetGridSize(int dimensionIndex) const override;
	virtual double GetSampleAt(const SampleIndex& index) const override;
	virtual istd::CRange GetLogicalRange(int dimensionIndex) const override;
	virtual istd::CRange GetResultValueRange(int dimensionIndex, int resultDimension = -1) const override;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const override;
	virtual ResultType GetValueAt(const ArgumentType& argument) const override;

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


