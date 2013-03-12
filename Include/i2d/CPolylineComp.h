#ifndef i2d_CPolylineComp_included
#define i2d_CPolylineComp_included


// ACF includes
#include "i2d/TObject2dCompWrap.h"
#include "i2d/CPolyline.h"


namespace i2d
{


/**
	Implementation of a poly-line as a component.
	It gives the possibility to define a list of polyline points via component attributes.
*/
class CPolylineComp: public TObject2dCompWrap<CPolyline>
{
public:
	typedef TObject2dCompWrap<CPolyline> BaseClass;

	I_BEGIN_COMPONENT(CPolylineComp);
		I_REGISTER_INTERFACE(CPolyline);
		I_REGISTER_INTERFACE(CPolygon);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN_MULTI_0(m_xAttrPtr, "X", "X-Positions of the poly-line points", true);
		I_ASSIGN_MULTI_0(m_yAttrPtr, "Y", "Y-Positions of the poly-line points", true);
		I_ASSIGN(m_isClosedAttrPtr, "IsClosed", "Indicate if poly-line should be initially closed", true, true);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIATTR(double, m_xAttrPtr);
	I_MULTIATTR(double, m_yAttrPtr);
	I_ATTR(bool, m_isClosedAttrPtr);
};


} // namespace i2d


#endif // !i2d_CPolylineComp_included
