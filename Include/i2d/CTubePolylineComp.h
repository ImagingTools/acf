#ifndef i2d_CTubePolylineComp_included
#define i2d_CTubePolylineComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "i2d/TDataNodePolyline.h"
#include "i2d/CTubeNode.h"


namespace i2d
{


typedef TDataNodePolyline<CTubeNode> CTubePolyline;


class CTubePolylineComp:
			public icomp::CComponentBase,
			public CTubePolyline
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CTubePolyline BaseClass2;

	I_BEGIN_COMPONENT(CTubePolylineComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IObject2d);
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



