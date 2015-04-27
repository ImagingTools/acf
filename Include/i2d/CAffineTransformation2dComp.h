#ifndef i2d_CAffineTransformation2dComp_included
#define i2d_CAffineTransformation2dComp_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "i2d/TObject2dCompWrap.h"
#include "i2d/CAffineTransformation2d.h"


namespace i2d
{


/**
	Attributable component for affine 2D-transformation.
*/
class CAffineTransformation2dComp: public TObject2dCompWrap<CAffineTransformation2d>
{
public:
	typedef TObject2dCompWrap<CAffineTransformation2d> BaseClass;

	I_BEGIN_COMPONENT(CAffineTransformation2dComp);
		I_REGISTER_INTERFACE(ICalibration2d);
		I_REGISTER_INTERFACE(ITransformation2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(i2d::CAffineTransformation2d);
		I_ASSIGN(m_m00AttrPtr, "M00", "Matrix value at the position (0, 0)", false, 1.0);
		I_ASSIGN(m_m10AttrPtr, "M10", "Matrix value at the position (1, 0)", false, 0);
		I_ASSIGN(m_m01AttrPtr, "M01", "Matrix value at the position (0, 1)", false, 0);
		I_ASSIGN(m_m11AttrPtr, "M11", "Matrix value at the position (1, 1)", false, 1.0);
		I_ASSIGN(m_translationXAttrPtr, "TranslationX", "Translation in X-direction", false, 0.0);
		I_ASSIGN(m_translationYAttrPtr, "TranslationY", "Translation in Y-direction", false, 0.0);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(double, m_m00AttrPtr);
	I_ATTR(double, m_m10AttrPtr);
	I_ATTR(double, m_m01AttrPtr);
	I_ATTR(double, m_m11AttrPtr);
	I_ATTR(double, m_translationXAttrPtr);
	I_ATTR(double, m_translationYAttrPtr);
};


} // namespace i2d


#endif // !i2d_CAffineTransformation2dComp_included


