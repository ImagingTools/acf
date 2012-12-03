#ifndef i2d_CPolygonComp_included
#define i2d_CPolygonComp_included


// ACF includes
#include "i2d/TObject2dCompWrap.h"
#include "i2d/CPolygon.h"


namespace i2d
{


/**
	Implementation of a polygon as a component.
	It gives the possibility to define a polygonal model via component attributes.
*/
class CPolygonComp: public TObject2dCompWrap<CPolygon>
{
public:
	typedef TObject2dCompWrap<CPolygon> BaseClass;

	I_BEGIN_COMPONENT(CPolygonComp);
		I_REGISTER_INTERFACE(CPolygon);
		I_ASSIGN_MULTI_0(m_xAttrPtr, "X", "X-Positions of the polygon points", true);
		I_ASSIGN_MULTI_0(m_yAttrPtr, "Y", "Y-Positions of the polygon points", true);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIATTR(double, m_xAttrPtr);
	I_MULTIATTR(double, m_yAttrPtr);
};


} // namespace i2d


#endif // !i2d_CPolygonComp_included
