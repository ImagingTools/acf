#include "imath/CNormalHistogram.h"


#include "istd/TChangeNotifier.h"


namespace imath
{


CNormalHistogram::CNormalHistogram()
:	m_elementsSum(0)
{
}


void CNormalHistogram::SetElementsCount(int count)
{
	istd::CChangeNotifier notifier(this);

	if (count < GetElementsCount()){
		m_elements.resize(count, 0);

		CalcElementsSum();
	}
	else{
		m_elements.resize(count, 0);
	}
}


void CNormalHistogram::SetElement(int index, int value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetElementsCount());

	istd::CChangeNotifier notifier(this);

	m_elementsSum -= m_elements[index];
	m_elements[index] = value;
	m_elementsSum += value;
}


void CNormalHistogram::IncreaseElement(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetElementsCount());

	istd::CChangeNotifier notifier(this);

	++m_elements[index];
	++m_elementsSum;
}


bool CNormalHistogram::AddValue(double value)
{
	double alpha = m_valueRange.GetAlphaFromValue(value);
	if ((alpha < 0) || (alpha > 1) || m_elements.empty()){
		return false;
	}

	int elementIndex = int(alpha * (m_elements.size() - I_BIG_EPSILON));
	I_ASSERT(elementIndex < GetElementsCount());

	IncreaseElement(elementIndex);

	return true;
};


int CNormalHistogram::GetQuantileIndex(double quantile) const
{
	double sumThreshold = quantile * m_elementsSum;

	if (sumThreshold <= 0){
		return -1;
	}

	int sum = 0;
	int elementsCount = int(m_elements.size());
	for (int i = 0; i < elementsCount; ++i){
		sum += m_elements[i];

		if (sum > sumThreshold){
			return i;
		}
	}

	return elementsCount;
}


double CNormalHistogram::GetAproxQuantileValue(double quantile) const
{
	double sumThreshold = quantile * m_elementsSum;

	if (sumThreshold <= 0){
		return m_valueRange.GetMinValue();
	}

	int sum = 0;
	int elementsCount = int(m_elements.size());
	for (int i = 0; i < elementsCount; ++i){
		int element = m_elements[i];

		sum += element;

		if (sum > sumThreshold){
			double finePos;
			if (i > 0){
				int prevSum = sum - element;
				finePos = i + (sumThreshold - prevSum / (element)) - 0.5;
			}
			else{
				finePos = sum / sumThreshold;
			}

			return m_valueRange.GetValueFromAlpha(finePos / GetElementsCount());
		}
	}

	return m_valueRange.GetMaxValue();
}


// reimplemented (imath::TISampledFunction)

bool CNormalHistogram::CreateFunction(double* dataPtr, const ArgumentType& sizes)
{
	m_elementsSum = 0;

	int elementsCount = sizes[0];
	m_elements.resize(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		int element = int(dataPtr[i]);

		m_elements[i] = element;
		m_elementsSum += element;
	}

	return true;
}


int CNormalHistogram::GetSamplesCount() const
{
	return GetElementsCount();
}


int CNormalHistogram::GetGridSize(int dimensionIndex) const
{
	if (dimensionIndex == 0){
		return GetElementsCount();
	}

	return 0;
}


istd::CRange CNormalHistogram::GetLogicalRange(int dimensionIndex) const
{
	if (dimensionIndex == 0){
		return m_valueRange;
	}

	return istd::CRange::GetNull();
}


istd::CRange CNormalHistogram::GetResultValueRange(int /*dimensionIndex*/, int /*resultDimension*/) const
{
	return istd::CRange::GetInvalid();
}


// reimplemented (imath::TIMathFunction)

bool CNormalHistogram::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	int index = argument[0];
	if ((index >= 0) && index < GetElementsCount()){
		result[0] = GetElement(index);

		return true;
	}
	else{
		return false;
	}
}


CNormalHistogram::ResultType CNormalHistogram::GetValueAt(const ArgumentType& argument) const
{
	TVector<1> retVal;

	int index = argument[0];
	if ((index >= 0) && index < GetElementsCount()){
		retVal[0] = GetElement(index);
	}
	else{
		retVal[0] = 0;
	}

	return retVal;
}


// protected methods

void CNormalHistogram::CalcElementsSum() const
{
	m_elementsSum = 0;

	for (Elements::const_iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter){
		m_elementsSum += *iter;
	}
}


} // namespace imath


