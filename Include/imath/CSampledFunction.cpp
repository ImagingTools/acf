#include <imath/CSampledFunction.h>


// ACF includes
#include <istd/CChangeNotifier.h>


namespace imath
{


CSampledFunction::CSampledFunction()
	:m_interpolatorPtr(nullptr)
{
}


CSampledFunction::CSampledFunction(const CSampledFunction& function)
	:m_samplesContainer(function.m_samplesContainer),
	m_logicalRange(function.GetLogicalRange(0)),
	m_interpolatorPtr(function.m_interpolatorPtr)
{
}


CSampledFunction::CSampledFunction(const imath::ISampledFunction& function)
	:m_logicalRange(function.GetLogicalRange(0)),
	m_interpolatorPtr(nullptr)
{
	Q_ASSERT(function.GetArgumentDimensionality() == 1);
	Q_ASSERT(function.GetResultDimensionality() == 1);

	int size = function.GetGridSize(0);

	m_samplesContainer.resize(size);

	for (int i = 0; i < m_samplesContainer.size(); ++i){
		m_samplesContainer[i] = function.GetValueAt(Index(i))[0];
	}
}


CSampledFunction::CSampledFunction(int size, double defaultValue)
	:m_logicalRange(0, size)
{
	Initialize(size, defaultValue);
}


void CSampledFunction::Reset()
{
	m_samplesContainer.clear();
}


bool CSampledFunction::Initialize(int size, double defaultValue)
{
	m_samplesContainer.resize(size);
	
	for (int i = 0; i < m_samplesContainer.size(); ++i){
		m_samplesContainer[i] = defaultValue;
	}

	return true;
}


void CSampledFunction::SetInterpolator(ISampledFunctionInterpolator* interpolatorPtr)
{
	m_interpolatorPtr = interpolatorPtr;

	if (m_interpolatorPtr != nullptr){
		m_interpolatorPtr->InitFromFunction(*this);
	}
}


// reimplemented (imath::ISampledFunction)

bool CSampledFunction::CreateFunction(double* dataPtr, const ArgumentType& sizes)
{;
	Initialize(sizes[0]);

	SamplesContainer::iterator beginIter = m_samplesContainer.begin();
	double* functionDataPtr = &(*beginIter);
	
	std::memcpy(functionDataPtr, dataPtr, sizeof(double) * sizes[0] * sizes[1]);

	return true;
}


int CSampledFunction::GetTotalSamplesCount() const
{
	return int(m_samplesContainer.size());
}


double CSampledFunction::GetSampleAt(const SampleIndex& index) const
{
	return GetSampleValue(index[0]);
}


istd::CRange CSampledFunction::GetLogicalRange(int dimensionIndex) const
{
	Q_ASSERT(dimensionIndex == 0);

	return m_logicalRange;
}


int CSampledFunction::GetGridSize(int dimensionIndex) const
{
	Q_ASSERT(dimensionIndex == 0);

	return int(m_samplesContainer.size());
}


istd::CRange CSampledFunction::GetResultValueRange(int /*dimensionIndex*/, int /*resultDimension*/) const
{
	return istd::CRange(0, 1.0);
}


// reimplemented (TIMathFunction)

bool CSampledFunction::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	double x = argument[0];

	if (!m_logicalRange.Contains(x)){
		return false;
	}

	if (m_interpolatorPtr != nullptr){
		double resultValue = 0;
		bool retVal = m_interpolatorPtr->GetValueAt(x, resultValue);
		if (retVal){
			result.SetElement(0, resultValue);

			return true;
		}

		return false;
	}

	istd::CIntRange sampleRange(0, int(m_samplesContainer.size()));

	int sampleIndex = sampleRange.GetValueFromAlpha(m_logicalRange.GetAlphaFromValue(x));

	if ((sampleIndex >= 0) && (sampleIndex < int(m_samplesContainer.size()))){
		result.SetElement(0, m_samplesContainer[sampleIndex]);
		
		return true;
	}

	return false;
}


CSampledFunction::ResultType CSampledFunction::GetValueAt(const ArgumentType& argument) const
{
	ResultType result;

	GetValueAt(argument, result);

	return result;
}


} // namespace imath


