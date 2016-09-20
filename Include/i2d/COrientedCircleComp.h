#ifndef i2d_COrientedCircleComp_included
#define i2d_COrientedCircleComp_included


// ACF includes
#include <i2d/TObject2dCompWrap.h>
#include <i2d/COrientedCircle.h>


namespace i2d
{


/**
	Implementation of a circle as a component.
	It gives the possibility to define a circle model via component attributes.
*/
class COrientedCircleComp: public TObject2dCompWrap<COrientedCircle>
{
public:
	typedef TObject2dCompWrap<COrientedCircle> BaseClass;

	I_BEGIN_COMPONENT(COrientedCircleComp);
		I_REGISTER_INTERFACE(COrientedCircle);
		I_REGISTER_INTERFACE(CPosition2d);
		I_ASSIGN(m_centerXAttrPtr, "X", "X-Position of the circle center", true, 0);
		I_ASSIGN(m_centerYAttrPtr, "Y", "Y-Position of the circle center", true, 0);
		I_ASSIGN(m_radiusAttrPtr, "Radius", "Radius of the circle", true, 0);
		I_ASSIGN(m_isOrientedOutsideAttrPtr, "IsOrientedOutside", "Flag indicating that this circle is oriented to outside", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_centerXAttrPtr);
	I_ATTR(double, m_centerYAttrPtr);
	I_ATTR(double, m_radiusAttrPtr);
	I_ATTR(bool, m_isOrientedOutsideAttrPtr);
};


} // namespace i2d


#endif // !i2d_COrientedCircleComp_included


