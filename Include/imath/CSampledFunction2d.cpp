#include "imath/CSampledFunction2d.h"

#include "istd/TChangeNotifier.h"


namespace imath
{


CSampledFunction2d::CSampledFunction2d(int width, int height, double defaultValue)
{
	Create(width, height, defaultValue);
}


void CSampledFunction2d::Reset()
{
	m_samplesContainer.Reset();
}


void CSampledFunction2d::Create(int width, int height, double defaultValue)
{
	Reset();

	if (width > 0 && height > 0){
		m_samplesContainer.SetSize(0, width);
		m_samplesContainer.SetSize(1, height);

		SamplesContainer::Iterator beginIter = m_samplesContainer.Begin();
		double* dataPtr = &(*beginIter);

		memset(dataPtr, 0, sizeof(double) * width * height);

		if (defaultValue != 0){
			for (		SamplesContainer::Iterator index = m_samplesContainer.Begin();
						index != m_samplesContainer.End();
						index++){
					*index = defaultValue;
			}
		}
	}
}


void CSampledFunction2d::SetSampleValue(const ElementIndex& index, double value)
{
	I_IF_DEBUG(
		ElementIndex boundary;
		boundary.SetAt(0, m_samplesContainer.GetSize(0));
		boundary.SetAt(1, m_samplesContainer.GetSize(1));)
	I_ASSERT(index.IsInside(boundary));

	m_samplesContainer.SetAt(index, value);
}


// reimplemented (ISampledFunction2d)

bool CSampledFunction2d::CreateFunction(double* dataPtr, int width, int height)
{
	Create(width, height);

	SamplesContainer::Iterator beginIter = m_samplesContainer.Begin();
	double* functionDataPtr = &(*beginIter);
	
	memcpy(functionDataPtr, dataPtr, sizeof(double) * width * height);

	return true;
}


int CSampledFunction2d::GetSamplesCount() const
{
	return m_samplesContainer.GetSize(0) * m_samplesContainer.GetSize(1);
}


istd::CRange CSampledFunction2d::GetIntervalRange(int dimensionIndex) const
{
	return istd::CRange(0, m_samplesContainer.GetSize(dimensionIndex));
}


istd::CRange CSampledFunction2d::GetValueRange(int /*dimensionIndex*/) const
{
	return istd::CRange(0, 1.0);
}


// reimplemented (TIMathFunction)

bool CSampledFunction2d::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	ElementIndex boundary;
	boundary.SetAt(0, m_samplesContainer.GetSize(0));
	boundary.SetAt(1, m_samplesContainer.GetSize(1));

	if (argument.IsInside(boundary)){
		result.SetElement(0, m_samplesContainer.GetAt(argument));
		
		return true;
	}

	return false;
}


CSampledFunction2d::ResultType CSampledFunction2d::GetValueAt(const ArgumentType& argument) const
{
	ResultType result;

	GetValueAt(argument, result);

	return result;
}



} // namespace imath


