#include "i2d/CPolygonExtractor.h"


namespace i2d
{


i2d::CPolygon CPolygonExtractor::CreatePolygon(const i2d::CAnnulus& annulus, int innerNodesCount, int outerNodesCount)
{
	Q_ASSERT(innerNodesCount > 0);
	Q_ASSERT(outerNodesCount > 0);

	i2d::CPolygon result;
	result.Clear();

	double smallRadius = annulus.GetInnerRadius();
	double bigRadius = annulus.GetOuterRadius();
	const i2d::CVector2d& center = annulus.GetPosition();

	double step = I_2PI / innerNodesCount;

	double angle = 0.0;
	int index;
	for (index = 0; index <= innerNodesCount; ++index){
		i2d::CVector2d point;
		point.Init(angle, smallRadius);
		result.InsertNode(point + center);
		angle += step;
	}

	step = I_2PI / outerNodesCount;

	angle = 0.0;
	for (index = 0; index <= outerNodesCount; ++index){
		i2d::CVector2d point;
		point.Init(angle, bigRadius);
		result.InsertNode(point + center);
		angle -= step;
	}

	return result;
}


}// namespace i2d


