#ifndef i2d_CTubePolylineComp_included
#define i2d_CTubePolylineComp_included


// ACF includes
#include <i2d/TObject2dCompWrap.h>
#include <i2d/CTubePolyline.h>


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
		I_ASSIGN(m_defaultAXPtr, "Default_A_X", "Default x coordinate for first line point", false, 40);
		I_ASSIGN(m_defaultAYPtr, "Default_A_Y", "Default y coordinate for first line point", false, 40);
		I_ASSIGN(m_defaultBXPtr, "Default_B_X", "Default x coordinate for second line point", false, 120);
		I_ASSIGN(m_defaultBYPtr, "Default_B_Y", "Default y coordinate for second line point", false, 40);
	I_END_COMPONENT;

	// reimplemented (i2d::CPolygon)
	virtual bool InsertNode(const i2d::CVector2d& node) override;
	virtual bool InsertNode(int index, const i2d::CVector2d& node) override;

protected:
	// reimplemented (i2d::CPolygon)
	virtual void SetNodesCount(int count) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(double, m_minTubeRangeValueAttrPtr);
	I_ATTR(double, m_maxTubeRangeValueAttrPtr);
	I_ATTR(int, m_defaultAXPtr);
	I_ATTR(int, m_defaultAYPtr);
	I_ATTR(int, m_defaultBXPtr);
	I_ATTR(int, m_defaultBYPtr);

	istd::CRange m_defaultTubeRange;
};


} // namespace i2d


#endif // !i2d_CTubePolylineComp_included


