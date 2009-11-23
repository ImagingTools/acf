#ifndef gmgstd_CSimpleHistogram_included
#define gmgstd_CSimpleHistogram_included


// STL includes
#include <vector>

#include "istd/CRange.h"

#include "imath/TISampledFunction.h"


namespace imath
{


/**
	Describe model of normal (non-cumulative) histogram.
*/
class CNormalHistogram: public ISampledFunction
{
public:
	CNormalHistogram();

	const istd::CRange& GetValueRange() const;
	void SetValueRange(const istd::CRange& range);

	/**
		Remove all elements from this histogram.
	*/
	void Reset();

	int GetElementsCount() const;
	void SetElementsCount(int count);

	void ClearAllElements();
	int GetElement(int index) const;
	void SetElement(int index, int value);
	void IncreaseElement(int index);

	bool AddValue(double value);
	int GetQuantileIndex(double quantile) const;
	double GetAproxQuantileValue(double quantile) const;

	// reimplemented (imath::TISampledFunction)
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes);
	virtual int GetSamplesCount() const;
	virtual int GetGridSize(int dimensionIndex) const;
	virtual istd::CRange GetLogicalRange(int dimensionIndex) const;
	virtual istd::CRange GetResultValueRange(int dimensionIndex, int resultDimension = -1) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;

protected:
	void CalcElementsSum() const;

private:
	istd::CRange m_valueRange;
	typedef std::vector<int> Elements;
	Elements m_elements;

	mutable int m_elementsSum;
};


// inline methods

inline const istd::CRange& CNormalHistogram::GetValueRange() const
{
	return m_valueRange;
}


inline void CNormalHistogram::SetValueRange(const istd::CRange& range)
{
	m_valueRange = range;
}


inline void CNormalHistogram::Reset()
{
	m_elements.clear();
	m_elementsSum = 0;
}


inline int CNormalHistogram::GetElementsCount() const
{
	return int(m_elements.size());
}


inline void CNormalHistogram::ClearAllElements()
{
	for (Elements::iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter){
		*iter = 0;
	}

	m_elementsSum = 0;
}


inline int CNormalHistogram::GetElement(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetElementsCount());

	return m_elements[index];
}


} // namespace imath


#endif // !gmgstd_CSimpleHistogram_included


