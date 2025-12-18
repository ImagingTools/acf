#include <i2d/CTubePolyline.h>


namespace i2d
{


// reimplemented (i2d::IObject2d)

bool CTubePolyline::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPt)
{
	int nodesCount = GetNodesCount();
	if (nodesCount < 2){
		return false;
	}

	for (int i = 0; i < nodesCount; ++i){
		const i2d::CVector2d& knee = GetKneeVector(i);
		const i2d::CVector2d& pos = GetNodePos(i);
		const istd::CRange& range = GetTNodeData(i).GetTubeRange();

		double newMin;
		if (!transformation.GetDistance(pos, pos - knee * range.GetMinValue(), newMin)){
			return false;
		}

		double newMax;
		if (!transformation.GetDistance(pos, pos - knee * range.GetMaxValue(), newMax)){
			return false;
		}

		double minSign = (range.GetMinValue() > 0) - (range.GetMinValue() < 0);
		double maxSign = (range.GetMaxValue() > 0) - (range.GetMaxValue() < 0);
		GetTNodeDataRef(i).SetTubeRange(istd::CRange(minSign*newMin, maxSign*newMax));
	}

	return BaseClass::Transform(transformation, mode, errorFactorPt);
}


} // namespace i2d


