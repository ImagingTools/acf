#ifndef i2d_CStaticCalibrationProviderComp_included
#define i2d_CStaticCalibrationProviderComp_included


// ACF includes
#include "icomp/CComponentBase.h"
#include "i2d/ICalibration2d.h"
#include "i2d/ICalibrationProvider.h"


namespace i2d
{		


/**
	Implementation of a annulus as a component.
	It gives the possibility to define a annulus segment model via component attributes.
*/
class CStaticCalibrationProviderComp: public icomp::CComponentBase, public ICalibrationProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStaticCalibrationProviderComp);
		I_REGISTER_INTERFACE(ICalibrationProvider);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Static calibration object", true, "Calibration");
	I_END_COMPONENT;

protected:
	// reimplemented (ICalibrationProvider)
	virtual const ICalibration2d* GetCalibration() const;

private:
	I_REF(ICalibration2d, m_calibrationCompPtr);
};


} // namespace i2d


#endif // !i2d_CStaticCalibrationProviderComp_included


