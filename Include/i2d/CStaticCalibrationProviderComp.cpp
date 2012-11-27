#include "i2d/CStaticCalibrationProviderComp.h"


namespace i2d
{		


// public methods

// reimplemented (ICalibrationProvider)

const ICalibration2d* CStaticCalibrationProviderComp::GetCalibration() const
{
	if (m_calibrationCompPtr.IsValid()){
		return m_calibrationCompPtr.GetPtr();
	}

	return NULL;
}


} // namespace i2d


