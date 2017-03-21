#include <imath/CSampledFunction2d.h>


// STL includes
#include <cstring>

// ACF includes
#include <istd/CChangeNotifier.h>


namespace imath
{


CSampledFunction2d::CSampledFunction2d()
{
}


CSampledFunction2d::CSampledFunction2d(const CSampledFunction2d& function2d)
:	m_samplesContainer(function2d.m_samplesContainer)
{
}


CSampledFunction2d::CSampledFunction2d(const imath::ISampledFunction2d& function2d)
{
	istd::CIndex2d size;
	size[0] = function2d.GetGridSize(0);
	size[1] = function2d.GetGridSize(1);

	m_samplesContainer.SetSizes(size);

	for (		SamplesContainer::Iterator index = m_samplesContainer.Begin();
				index != m_samplesContainer.End();
				++index){
		*index = function2d.GetValueAt(index)[0];
	}
}


CSampledFunction2d::CSampledFunction2d(const istd::CIndex2d& sizes, double defaultValue)
{
	CreateGrid2d(sizes, defaultValue);
}


void CSampledFunction2d::Reset()
{
	m_samplesContainer.Reset();
}


bool CSampledFunction2d::CreateGrid2d(const istd::CIndex2d& sizes, double defaultValue)
{
	if (!m_samplesContainer.SetSizes(sizes)){
		return false;
	}

	for (		SamplesContainer::Iterator index = m_samplesContainer.Begin();
				index != m_samplesContainer.End();
				++index){
		*index = defaultValue;
	}


	return true;
}


// reimplemented (imath::ISampledFunction2d)

bool CSampledFunction2d::CreateFunction(double* dataPtr, const ArgumentType& sizes)
{
	CreateGrid2d(sizes);

	SamplesContainer::Iterator beginIter = m_samplesContainer.Begin();
	double* functionDataPtr = &(*beginIter);
	
	std::memcpy(functionDataPtr, dataPtr, sizeof(double) * sizes[0] * sizes[1]);

	return true;
}


int CSampledFunction2d::GetTotalSamplesCount() const
{
	return m_samplesContainer.GetSize(0) * m_samplesContainer.GetSize(1);
}


istd::CRange CSampledFunction2d::GetLogicalRange(int dimensionIndex) const
{
	return istd::CRange(0, m_samplesContainer.GetSize(dimensionIndex));
}


int CSampledFunction2d::GetGridSize(int dimensionIndex) const
{
	return m_samplesContainer.GetSize(dimensionIndex);
}


istd::CRange CSampledFunction2d::GetResultValueRange(int /*dimensionIndex*/, int /*resultDimension*/) const
{
	return istd::CRange(0, 1.0);
}


// reimplemented (TIMathFunction)

bool CSampledFunction2d::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	ArgumentType boundary = m_samplesContainer.GetSizes();

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


