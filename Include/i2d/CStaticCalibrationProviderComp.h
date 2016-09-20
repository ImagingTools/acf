#ifndef i2d_CStaticCalibrationProviderComp_included
#define i2d_CStaticCalibrationProviderComp_included


// ACF includes
#include <icomp/CComponentBase.h>

#include <imod/CMultiModelBridgeBase.h>

#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>


namespace i2d
{


/**
	Implementation of a static calibration provider.
	This component uses a staticaly defined calibration object (set via component reference) to provide the calibration instance.
	If the undelaying calibration object will be changed, this object will also fire a change notification event.
*/
class CStaticCalibrationProviderComp:
			public icomp::CComponentBase,
			public ICalibrationProvider,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStaticCalibrationProviderComp);
		I_REGISTER_INTERFACE(ICalibrationProvider);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_calibrationCompPtr, "Calibration", "Static calibration object", true, "Calibration");
		I_ASSIGN_TO(m_calibrationModelCompPtr, m_calibrationCompPtr, false);
	I_END_COMPONENT;

	CStaticCalibrationProviderComp();

	// reimplemented (i2d::ICalibrationProvider)
	virtual const ICalibration2d* GetCalibration() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(ICalibration2d, m_calibrationCompPtr);
	I_REF(imod::IModel, m_calibrationModelCompPtr);
};


} // namespace i2d


#endif // !i2d_CStaticCalibrationProviderComp_included


