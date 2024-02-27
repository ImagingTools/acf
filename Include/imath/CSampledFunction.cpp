#include <imath/CSampledFunction.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>


namespace imath
{


CSampledFunction::CSampledFunction()
{
}


CSampledFunction::CSampledFunction(const CSampledFunction& function)
	:m_samplesContainer(function.m_samplesContainer),
	m_logicalRange(function.GetLogicalRange(0))
{
}


CSampledFunction::CSampledFunction(const imath::ISampledFunction& function)
	:m_logicalRange(function.GetLogicalRange(0))
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


bool CSampledFunction::operator==(const CSampledFunction& other) const
{
	bool retVal = m_samplesContainer == other.m_samplesContainer;
	retVal = retVal && m_logicalRange == other.m_logicalRange;

	return retVal;
}


void CSampledFunction::Reset()
{
	if (!m_samplesContainer.empty()){
		istd::CChangeNotifier notifier(this);

		m_samplesContainer.clear();
	}
}


bool CSampledFunction::Initialize(int size, double defaultValue)
{
	istd::CChangeNotifier notifier(this);

	m_samplesContainer.resize(size);
	
	for (int i = 0; i < m_samplesContainer.size(); ++i){
		m_samplesContainer[i] = defaultValue;
	}

	return true;
}


// reimplemented (imath::ISampledFunction)

bool CSampledFunction::CreateFunction(double* dataPtr, const ArgumentType& sizes)
{
	istd::CChangeNotifier notifier(this);

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


// reimplemented (istd::IChangeable)

int CSampledFunction::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY | SO_RESET;
}


bool CSampledFunction::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSampledFunction* objectPtr = dynamic_cast<const CSampledFunction*>(&object);

	if (objectPtr != nullptr){
		istd::CChangeNotifier notifier(this);

		*this = *objectPtr;

		return true;
	}

	return false;
}


bool CSampledFunction::IsEqual(const IChangeable& object) const
{
	const CSampledFunction* objectPtr = dynamic_cast<const CSampledFunction*>(&object);

	if (objectPtr != nullptr){
		return *this == *objectPtr;
	}

	return false;
}


istd::IChangeable* CSampledFunction::CloneMe(CompatibilityMode /*mode*/) const
{
	istd::TDelPtr<CSampledFunction> clonePtr(new CSampledFunction());

	if (clonePtr->CopyFrom(*this)){
		return clonePtr.PopPtr();
	}

	return nullptr;
}


bool CSampledFunction::ResetData(CompatibilityMode /*mode*/)
{
	Reset();

	return true;
}


} // namespace imath


