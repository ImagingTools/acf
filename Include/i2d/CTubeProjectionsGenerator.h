#ifndef i2d_CTubeProjectionsGenerator_included
#define i2d_CTubeProjectionsGenerator_included


#include "i2d/CTubePolyline.h"


namespace i2d
{


/**
	Helper class for generation of list of projection lines in the given tube region.
*/
class CTubeProjectionsGenerator
{
public:
	typedef QVector<CLine2d> ProjectionLines;

	/**
		Create list of projections for the given tube region.
		\param tube	Input tube region
		\param projectionsCount Number of projections to be generated
		\param projections	List of generated projection lines.
	*/
	static bool CreateNodes(const i2d::CTubePolyline& tube, int projectionsCount, ProjectionLines& projections);
};


} // namespace i2d


#endif // !i2d_CTubeProjectionsGenerator_included


