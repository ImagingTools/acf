#include "imath/CAkimaInterpolator.h"


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/istd.h"
#include "imath/CSplineSegmentFunction.h"


namespace imath
{


CAkimaInterpolator::CAkimaInterpolator()
{
}


CAkimaInterpolator::CAkimaInterpolator(double* positions, double* values, int nodesCount)
{
	SetNodes(positions, values, nodesCount);
}


void CAkimaInterpolator::SetNodes(double* positions, double* values, int nodesCount)
{
	m_nodes.clear();

	if (nodesCount >= 3){
		// help storage objects
		QVector<double> xe(nodesCount + 4, 0);
		QVector<double> ye(nodesCount + 4, 0);
		QVector<double> s(nodesCount + 3, 0);
		
		// copy values and positions to help array
		for (int i = 0; i < nodesCount; i++){
			xe[i + 2] = positions[i];
			ye[i + 2] = values[i];
		}

		// extrapolate x values on left and right
		xe[1] = xe[2] + xe[3] - xe[4];
		xe[0] = xe[1] + xe[2] - xe[3];
		xe[nodesCount + 2] = xe[nodesCount + 1] + xe[nodesCount] - xe[nodesCount - 1];
		xe[nodesCount + 3] = xe[nodesCount + 2] + xe[nodesCount + 1] - xe[nodesCount];  

		// calculate slopes
		for (int i = 2; i < nodesCount + 1; ++i){
			s[i] = (ye[i + 1] - ye[i]) / (xe[i + 1] - xe[i]);
		}

		// extrapolate x values and slopes on left and right
		ye[1] = (xe[2] - xe[1]) * (s[3] - 2 * s[2]) + ye[2];
		s[1] = (ye[2] - ye[1]) / (xe[2] - xe[1]);
		ye[0] = (xe[1] - xe[0]) * (s[2] - 2 * s[1]) + ye[1]; 
		s[0] = (ye[1] - ye[0]) / (xe[1] - xe[0]);
		ye[nodesCount + 2] = (2 * s[nodesCount] - s[nodesCount - 1]) * (xe[nodesCount + 2] - xe[nodesCount + 1]) + ye[nodesCount + 1];
		s[nodesCount + 1] = (ye[nodesCount + 2] - ye[nodesCount + 1]) / (xe[nodesCount + 2] - xe[nodesCount + 1]);
		ye[nodesCount + 3] = (2 * s[nodesCount + 1] - s[nodesCount]) * (xe[nodesCount + 3] - xe[nodesCount + 2]) + ye[nodesCount + 2];
		s[nodesCount + 2] = (ye[nodesCount + 3] - ye[nodesCount + 2]) / (xe[nodesCount + 3] - xe[nodesCount + 2]);

		// Calculation of all polynomial coefficients
		for (int i = 0; i < nodesCount; ++i){
			double prevPrevSlope = s[i];
			double prevSlope = s[i + 1];
			double nextSlope = s[i + 2];
			double nextNextSlope = s[i + 3];

			double prevSlopeDiff = qAbs(prevPrevSlope - prevSlope);
			double nextSlopeDiff = qAbs(nextSlope - nextNextSlope);
			double slopeDiffSum = prevSlopeDiff + nextSlopeDiff;

			Node& node = m_nodes[positions[i]];
			node.value = values[i];
			node.derivative = (slopeDiffSum > I_BIG_EPSILON)?
						(nextSlopeDiff * prevSlope + prevSlopeDiff * nextSlope) / slopeDiffSum:
						0.5 * (prevSlope + nextSlope);
		}
	}
	else if (nodesCount == 2){
		double derivative = (values[1] - values[0]) / (positions[1] - positions[0]);

		Node& firstNode = m_nodes[positions[0]];
		firstNode.value = values[0];
		firstNode.derivative = derivative;

		Node& lastNode = m_nodes[positions[1]];
		lastNode.value = values[1];
		lastNode.derivative = derivative;
	}
	else if (nodesCount == 1){
		Node& firstNode = m_nodes[positions[0]];
		firstNode.value = values[0];
		firstNode.derivative = 0;
	}
}


// reimplemented (imath::TIMathFunction<double, double>)

bool CAkimaInterpolator::GetValueAt(const double& argument, double& result) const
{
	int nodesCount = m_nodes.size();
	if (nodesCount >= 1){
		Nodes::ConstIterator nextIter = m_nodes.lowerBound(argument);
		if ((nodesCount >= 2) && (nextIter != m_nodes.constEnd())){
			double nextPosition = nextIter.key();
			Q_ASSERT(nextPosition >= argument);
			const Node& nextNode = nextIter.value();

			if (nextIter != m_nodes.constBegin()){
				// interpolation in segment
				Nodes::ConstIterator prevIter = nextIter - 1;

				double prevPosition = prevIter.key();
				Q_ASSERT(prevPosition <= argument);
				const Node& prevNode = prevIter.value();

				double nodeDiff = (nextPosition - prevPosition);
				double alpha = (argument - prevPosition) / nodeDiff;

				double leftValueFactor = CSplineSegmentFunction::GetValueKernelAt(alpha);
				double rightValueFactor = CSplineSegmentFunction::GetValueKernelAt(1 - alpha);
				double leftDerivativeFactor = CSplineSegmentFunction::GetDerivativeKernelAt(alpha) * nodeDiff;
				double rightDerivativeFactor = -CSplineSegmentFunction::GetDerivativeKernelAt(1 - alpha) * nodeDiff;

				result =	prevNode.value * leftValueFactor +
							prevNode.derivative * leftDerivativeFactor +
							nextNode.value * rightValueFactor +
							nextNode.derivative * rightDerivativeFactor;

				return true;
			}
			else{
				// extrapolation at the begin
				result = nextNode.value + nextNode.derivative * (argument - nextPosition);

				return true;
			}
		}
		else{
			// extrapolation at the end
			Nodes::ConstIterator lastIter = m_nodes.constEnd() - 1;
			double lastPosition = lastIter.key();
			Q_ASSERT(lastPosition <= argument);
			const Node& lastNode = lastIter.value();

			result = lastNode.value + lastNode.derivative * (argument - lastPosition);

			return true;
		}
	}
	else{
		return false;
	}
}


double CAkimaInterpolator::GetValueAt(const double& argument) const
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


