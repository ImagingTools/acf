#ifndef i2d_CTubePolylineComp_included
#define i2d_CTubePolylineComp_included


// ACF includes
#include "i2d/TObject2dCompWrap.h"
#include "i2d/CTubePolyline.h"


namespace i2d
{


/**
	Component definition of the tube region based on a polyline.
*/
class CTubePolylineComp: public TObject2dCompWrap<CTubePolyline>
{
public:
	typedef TObject2dCompWrap<CTubePolyline> BaseClass;

	I_BEGIN_COMPONENT(CTubePolylineComp);
		I_REGISTER_INTERFACE(CTubePolyline);
		I_ASSIGN(m_minTubeRangeValueAttrPtr, "DefaultLeftTubeRange", "Default left tube range value", false, -10);
		I_ASSIGN(m_maxTubeRangeValueAttrPtr, "DefaultRightTubeRange", "Default right tube range value", false, 10);
	I_END_COMPONENT;

	// reimplemented (i2d::CPolygon)
	virtual bool InsertNode(const i2d::CVector2d& node);
	virtual bool InsertNode(int index, const i2d::CVector2d& node);

protected:
	// reimplemented (i2d::CPolygon)
	virtual void SetNodesCount(int count);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_minTubeRangeValueAttrPtr);
	I_ATTR(double, m_maxTubeRangeValueAttrPtr);

	istd::CRange m_defaultTubeRange;
};


} // namespace i2d


#endif // !i2d_CTubePolylineComp_included


