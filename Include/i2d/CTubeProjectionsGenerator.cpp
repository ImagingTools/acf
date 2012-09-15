#include "i2d/CTubeProjectionsGenerator.h"


namespace i2d
{


bool CTubeProjectionsGenerator::CreateNodes(const i2d::CTubePolyline& tube, int projectionsCount, ProjectionLines& projections)
{
	int nodesCount = tube.GetNodesCount();

	if ((nodesCount <= 1) || (projectionsCount <= 0)){
		return false;
	}

	double tubeLength = tube.GetLength();

	double step = tubeLength / projectionsCount;
	i2d::CVector2d prevNode = tube.GetNode(0);
	i2d::CVector2d kneeVector = tube.GetKneeVector(0);
	const CTubeNode* tubeNodePtr = dynamic_cast<const CTubeNode*>(&tube.GetNodeData(0));
	if (tubeNodePtr == NULL){
		return false;
	}
	istd::CRange prevAoiRange = tubeNodePtr->GetTubeRange();
	i2d::CLine2d frontSegmentLine(
				prevNode - kneeVector * prevAoiRange.GetMinValue(),
				prevNode - kneeVector * prevAoiRange.GetMaxValue());
	
	i2d::CLine2d middleSegmentLine;
	i2d::CLine2d endSegmentLine;

	projections.push_back(frontSegmentLine);

	istd::CRange lengthRange(0, 0);

	int nextNodeInex = 0;
	for (int profileIndex = 0; profileIndex < projectionsCount; ++profileIndex){
		double lengthPos = step * (profileIndex + 1 - I_BIG_EPSILON);
		while (lengthPos > lengthRange.GetMaxValue()){
			nextNodeInex = (nextNodeInex + 1) % nodesCount;
			const i2d::CVector2d& nextNode = tube.GetNode(nextNodeInex);
			const CTubeNode* nextTubeDataPtr = dynamic_cast<const CTubeNode*>(&tube.GetNodeData(nextNodeInex));
			if (nextTubeDataPtr == NULL){
				return false;
			}
			const istd::CRange& nextAoiRange = nextTubeDataPtr->GetTubeRange();
			const i2d::CVector2d& nextKneeVector = tube.GetKneeVector(nextNodeInex);

			istd::CRange middleAoi(
							(prevAoiRange.GetMinValue() + nextAoiRange.GetMinValue()) * 0.5,
							(prevAoiRange.GetMaxValue() + nextAoiRange.GetMaxValue()) * 0.5);
			i2d::CVector2d orthogonalVector = (nextNode - prevNode).GetOrthogonal().GetNormalized();
			i2d::CVector2d middlePoint = (nextNode + prevNode) * 0.5;

			endSegmentLine = frontSegmentLine;
			middleSegmentLine = i2d::CLine2d(
							middlePoint - orthogonalVector * middleAoi.GetMinValue(),
							middlePoint - orthogonalVector * middleAoi.GetMaxValue());
			frontSegmentLine = i2d::CLine2d(
							nextNode - nextKneeVector * nextAoiRange.GetMinValue(),
							nextNode - nextKneeVector * nextAoiRange.GetMaxValue());

			double newLengthRangeMaxValue = lengthRange.GetMaxValue() + nextNode.GetDistance(prevNode);
			lengthRange.SetMinValue(lengthRange.GetMaxValue());
			lengthRange.SetMaxValue(newLengthRangeMaxValue);

			prevNode = nextNode;
			prevAoiRange = nextAoiRange;
		}

		double alpha = (lengthPos - lengthRange.GetMinValue()) / lengthRange.GetLength();
		if (alpha < 0.5){
			double prop1 = alpha * 2;
			double prop2 = 1 - prop1;
			i2d::CLine2d line(endSegmentLine.GetPoint1() * prop2 + middleSegmentLine.GetPoint1() * prop1,
							endSegmentLine.GetPoint2() * prop2 + middleSegmentLine.GetPoint2() * prop1);
			projections.push_back(line);
		}
		else{
			double prop1 = alpha * 2 - 1;
			double prop2 = 1 - prop1;
			i2d::CLine2d line(middleSegmentLine.GetPoint1() * prop2 + frontSegmentLine.GetPoint1() * prop1,
							middleSegmentLine.GetPoint2() * prop2 + frontSegmentLine.GetPoint2() * prop1);
			projections.push_back(line);
		}
	}

	return true;
}


} // namespace i2d


