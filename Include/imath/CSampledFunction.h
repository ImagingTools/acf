#pragma once


// ACF includes
#include <istd/TArray.h>
#include <istd/TIndex.h>
#include <imath/TISampledFunction.h>


namespace imath
{


/**
	Implementation of the resampled 1D-function.
*/
class CSampledFunction: virtual public ISampledFunction
{
public:
	typedef istd::TIndex<1> Index;

	CSampledFunction();
	CSampledFunction(const CSampledFunction& function);
	explicit CSampledFunction(const imath::ISampledFunction& function);
	explicit CSampledFunction(int size, double defaultValue = 0);

	bool operator==(const CSampledFunction& other) const;

	void Reset();
	bool Initialize(int size, double defaultValue = 0);
	double GetSampleValue(int index) const;
	void SetSampleValue(int index, double value);
	void SetLogicalRange(const istd::CRange& logicalRange);
	double GetSamplingStep() const;
	double GetSampleCoordinate(int index) const;
	
	// reimplemented (imath::ISampledFunction)
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes) override;
	virtual int GetTotalSamplesCount() const override;
	virtual double GetSampleAt(const SampleIndex& index) const override;
	virtual int GetGridSize(int dimensionIndex) const override;
	virtual istd::CRange GetLogicalRange(int dimensionIndex) const override;
	virtual istd::CRange GetResultValueRange(int dimensionIndex, int resultDimension = -1) const override;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const override;
	virtual ResultType GetValueAt(const ArgumentType& argument) const override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	typedef std::vector<double> SamplesContainer;
	SamplesContainer m_samplesContainer;
	istd::CRange m_logicalRange;
};


// public inline methods

inline double CSampledFunction::GetSampleValue(int index) const
{
	Q_ASSERT(index>= 0 && index < m_samplesContainer.size());

	return m_samplesContainer[index];
}


inline void CSampledFunction::SetSampleValue(int index, double value)
{
	Q_ASSERT(index >= 0 && index < m_samplesContainer.size());

	m_samplesContainer[index] = value;
}


inline void CSampledFunction::SetLogicalRange(const istd::CRange& logicalRange)
{
	m_logicalRange = logicalRange;
}


inline double CSampledFunction::GetSamplingStep() const
{
	if (m_samplesContainer.size() >= 2){
		return m_logicalRange.GetLength() / (m_samplesContainer.size() - 1);
	}

	return 0;
}


inline double CSampledFunction::GetSampleCoordinate(int index) const
{
	Q_ASSERT(m_samplesContainer.size() > 0);
	Q_ASSERT(0 <= index);
	Q_ASSERT(index < m_samplesContainer.size());

	return m_logicalRange.GetValueFromAlpha(double(index) / double(m_samplesContainer.size() - 1));
}


} // namespace imath


