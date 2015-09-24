#ifndef i2d_CAnnulusComp_included
#define i2d_CAnnulusComp_included


// ACF includes
#include "i2d/TObject2dCompWrap.h"
#include "i2d/CAnnulus.h"


namespace i2d
{


/**
	Implementation of an annulus as a component.
	It gives the possibility to define the annulus model via component attributes.
*/
class CAnnulusComp: public TObject2dCompWrap<CAnnulus>
{
public:
	typedef TObject2dCompWrap<CAnnulus> BaseClass;

	I_BEGIN_COMPONENT(CAnnulusComp);
		I_REGISTER_INTERFACE(i2d::CAnnulus);
		I_REGISTER_INTERFACE(CPosition2d);
		I_ASSIGN(m_centerXAttrPtr, "X", "X-Position of the annulus center", true, 0);
		I_ASSIGN(m_centerYAttrPtr, "Y", "X-Position of the annulus center", true, 0);
		I_ASSIGN(m_innerRadiusAttrPtr, "InnerRadius", "Inner radius of the annulus", true, 0);
		I_ASSIGN(m_outerRadiusAttrPtr, "OuterRadius", "Outer radius of the annulus", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

	// reimplemented (istd::IChangeable)
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	I_ATTR(double, m_centerXAttrPtr);
	I_ATTR(double, m_centerYAttrPtr);
	I_ATTR(double, m_innerRadiusAttrPtr);
	I_ATTR(double, m_outerRadiusAttrPtr);
};


} // namespace i2d


#endif // !i2d_CAnnulusComp_included


