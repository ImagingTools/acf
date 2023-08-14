#pragma once


// ACF includes
#include <i2d/TObject2dCompWrap.h>


// RTVision includes
#include <i2d/CParallelogram.h>


namespace i2d
{


/**
	Implementation of a parallelogram as a component.
	It gives the possibility to define a parallelogram model via component attributes.
*/
class CParallelogramComp: public i2d::TObject2dCompWrap<i2d::CParallelogram>
{
public:
	typedef i2d::TObject2dCompWrap<i2d::CParallelogram> BaseClass;

	I_BEGIN_COMPONENT(CParallelogramComp);
		I_REGISTER_INTERFACE(CParallelogram);
		I_REGISTER_INTERFACE(IObject2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_xAttrPtr, "X", "X-Position of the parallelogramm", true, 0);
		I_ASSIGN(m_yAttrPtr, "Y", "Y-Position of the parallelogramm", true, 0);
		I_ASSIGN(m_widthAttrPtr, "Width", "Width of the parallelogramm", true, 200);
		I_ASSIGN(m_heightAttrPtr, "Height", "Height of the parallelogramm", true, 200);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(double, m_xAttrPtr);
	I_ATTR(double, m_yAttrPtr);
	I_ATTR(double, m_widthAttrPtr);
	I_ATTR(double, m_heightAttrPtr);
};


} // namespace i2d


