#include <i2d/CStaticCalibrationProviderComp.h>


namespace i2d
{


CStaticCalibrationProviderComp::CStaticCalibrationProviderComp()
:	imod::CMultiModelBridgeBase(this)
{
}


// reimplemented (i2d::ICalibrationProvider)

const ICalibration2d* CStaticCalibrationProviderComp::GetCalibration() const
{
	if (m_calibrationCompPtr.IsValid()){
		return m_calibrationCompPtr.GetPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CStaticCalibrationProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_calibrationModelCompPtr.IsValid()){
		m_calibrationModelCompPtr->AttachObserver(this);
	}
}


void CStaticCalibrationProviderComp::OnComponentDestroyed()
{
	EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace i2d


