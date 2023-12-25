#ifndef i2d_CTubePolyline_included
#define i2d_CTubePolyline_included


// ACF includes
#include <i2d/TDataNodePolyline.h>
#include <i2d/CTubeNode.h>


namespace i2d
{


/**
	Definition of tube region based on polyline.
*/
class CTubePolyline: public TDataNodePolyline<CTubeNode>
{
public:
	typedef TDataNodePolyline<CTubeNode> BaseClass;

	// reimplemented (i2d::IObject2d)
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
};


} // namespace i2d


#endif // !i2d_CTubePolyline_included



