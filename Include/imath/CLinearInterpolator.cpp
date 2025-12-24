#include <imath/CLinearInterpolator.h>


namespace imath
{


// public methods

CLinearInterpolator::CLinearInterpolator()
	:m_isExtrapolationEnabled(false)
{
}


CLinearInterpolator::CLinearInterpolator(double* positions, double* values, int nodesCount, bool isExtrapolationEnabled)
	:m_isExtrapolationEnabled(isExtrapolationEnabled)
{
	SetNodes(positions, values, nodesCount);
}


void CLinearInterpolator::SetNodes(double* positions, double* values, int nodesCount)
{
	m_nodes.clear();

	for (int nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++){
		m_nodes[positions[nodeIndex]] = values[nodeIndex];
	}
}


bool CLinearInterpolator::InitFromFunction(const ISampledFunction& function)
{
	m_nodes.clear();

	istd::CRange logicalRange = function.GetLogicalRange(0);
	istd::CIntRange sampleRange = istd::CIntRange(0, function.GetTotalSamplesCount());

	for (int sampleIndex = 0; sampleIndex < function.GetTotalSamplesCount(); sampleIndex++){
		double sampleAlpha = sampleRange.GetAlphaFromValue(sampleIndex);

		double logicalPosition = logicalRange.GetValueFromAlpha(sampleAlpha);

		m_nodes[logicalPosition] = function.GetSampleAt(istd::TIndex<1>(sampleIndex));
	}

	return true;
}


// reimplemented (imath::TIMathFunction<double, double>)

bool CLinearInterpolator::GetValueAt(const double& argument, double& result) const
{
	Nodes::ConstIterator nextIter = m_nodes.lowerBound(argument);
	const bool additionalCondition = m_isExtrapolationEnabled ? (m_nodes.count() == 1) : true;

	if (nextIter != m_nodes.constEnd()){
		double nextPosition = nextIter.key();
		Q_ASSERT(nextPosition >= argument);
		double nextValue = nextIter.value();

		if ((nextPosition == argument) || ((nextIter == m_nodes.constBegin()) && additionalCondition)){
			result = nextValue;

			return true;
		}

		if (m_isExtrapolationEnabled && (nextIter == m_nodes.constBegin())){
			//extrapolate x < x_first
			Nodes::ConstIterator nextNextIter = std::next(nextIter);
			double nextNextPosition = nextNextIter.key();
			double nextNextValue = nextNextIter.value();

			double nodeDiff = (nextNextPosition - nextPosition);
			Q_ASSERT(nodeDiff > 0);

			double slope = (nextNextValue - nextValue) / nodeDiff;
			result = nextValue - slope * (nextPosition - argument);

			return true;
		}


		Nodes::ConstIterator prevIter = std::prev(nextIter);

		double prevPosition = prevIter.key();
		Q_ASSERT(prevPosition <= argument);
		double prevValue = prevIter.value();

		// interpolation in segment
		double nodeDiff = (nextPosition - prevPosition);
		Q_ASSERT(nodeDiff > 0);

		double alpha = (argument - prevPosition) / nodeDiff;

		result = prevValue * (1 - alpha) + nextValue * alpha;

		return true;
	}

	else if (nextIter == m_nodes.constEnd() && additionalCondition){
		result = nextIter.value();

		return true;
	}

	else if (m_isExtrapolationEnabled){
		/*extrapolate x > last_x*/
		nextIter = std::prev(m_nodes.constEnd());

		double prevPosition = nextIter.key();
		Q_ASSERT(prevPosition <= argument);
		double prevValue = nextIter.value();

		Nodes::ConstIterator prevIter = std::prev(nextIter);
		double prevPrevPosition = prevIter.key();
		double prevPrevValue = prevIter.value();

		double nodeDiff = (prevPosition - prevPrevPosition);
		Q_ASSERT(nodeDiff > 0);

		double slope = (prevValue - prevPrevValue) / nodeDiff;
		result = prevValue + slope * (argument - prevPosition);

		return true;
	}

	return false;
}


double CLinearInterpolator::GetValueAt(const double& argument) const
{
	double retVal;
	if (GetValueAt(argument, retVal)){
		return retVal;
	}
	else{
		return 0;
	}
}


} // namespace imath


