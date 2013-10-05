#include "i2d/CCalibration2dProxyComp.h"


namespace i2d
{


// public methods

CCalibration2dProxyComp::CCalibration2dProxyComp()
	:m_isCalibrationCalculated(false)
{
}

// reimplemented (ICalibration2d)

const imath::IUnitInfo* CCalibration2dProxyComp::GetArgumentUnitInfo() const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetArgumentUnitInfo();
	}

	return NULL;
}


const imath::IUnitInfo* CCalibration2dProxyComp::GetResultUnitInfo() const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetResultUnitInfo();
	}

	return NULL;
}

const ICalibration2d* CCalibration2dProxyComp::CreateCombinedCalibration(const ICalibration2d& calibration) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->CreateCombinedCalibration(calibration);
	}

	return NULL;
}


// reimplemented (ITransformation2d)

int CCalibration2dProxyComp::GetTransformationFlags() const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetTransformationFlags();
	}

	return EM_NONE;
}


bool CCalibration2dProxyComp::GetDistance(
			const CVector2d& origPos1,
			const CVector2d& origPos2,
			double& result,
			ExactnessMode mode) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetDistance(origPos1, origPos2, result, mode);
	}

	return false;
}


bool CCalibration2dProxyComp::GetPositionAt(
			const CVector2d& origPosition,
			CVector2d& result,
			ExactnessMode mode) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetPositionAt(origPosition, result, mode);
	}

	return false;
}


bool CCalibration2dProxyComp::GetInvPositionAt(
			const CVector2d& transfPosition,
			CVector2d& result,
			ExactnessMode mode) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetInvPositionAt(transfPosition, result, mode);
	}

	return false;
}


bool CCalibration2dProxyComp::GetLocalTransform(
			const CVector2d& origPosition,
			CAffine2d& result,
			ExactnessMode mode) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetLocalTransform(origPosition, result, mode);
	}

	return false;
}


bool CCalibration2dProxyComp::GetLocalInvTransform(
				const CVector2d& transfPosition,
				CAffine2d& result,
				ExactnessMode mode) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetLocalInvTransform(transfPosition, result, mode);
	}

	return false;
}

// reimplemented (imath::TISurjectFunction)

bool CCalibration2dProxyComp::GetInvValueAt(const CVector2d& argument, CVector2d& result) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetInvValueAt(argument, result);
	}

	return false;
}


CVector2d CCalibration2dProxyComp::GetInvValueAt(const CVector2d& argument) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetInvValueAt(argument);
	}

	return CVector2d();
}


// reimplemented (imath::TIMathFunction)

bool CCalibration2dProxyComp::GetValueAt(const CVector2d& argument, CVector2d& result) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetValueAt(argument, result);
	}

	return false;
}


CVector2d CCalibration2dProxyComp::GetValueAt(const CVector2d& argument) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetValueAt(argument);
	}

	return CVector2d();
}


// reimplemented (iser::ISerializable)

bool CCalibration2dProxyComp::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


// reimplemented (imod::IObserver)

void CCalibration2dProxyComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	m_isCalibrationCalculated = false;

	BaseClass2::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}



// protected methods

// reimplemented (icomp::CComponentBase)

void CCalibration2dProxyComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_calibrationProviderModelCompPtr.IsValid()){
		m_calibrationProviderModelCompPtr->AttachObserver(this);
	}
}


void CCalibration2dProxyComp::OnComponentDestroyed()
{
	EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


// private methods

const ICalibration2d* CCalibration2dProxyComp::GetCalibrationPtr() const
{
	EnsureWorkingCalibrationUpdated();

	return m_workingCalibrationPtr.GetPtr();
}


void CCalibration2dProxyComp::EnsureWorkingCalibrationUpdated() const
{
	if (!m_isCalibrationCalculated && m_calibrationProviderCompPtr.IsValid()){
		m_isCalibrationCalculated = true;

		const i2d::ICalibration2d* calibrationPtr = m_calibrationProviderCompPtr->GetCalibration();
		if (calibrationPtr != NULL){
			m_workingCalibrationPtr.SetCastedOrRemove(calibrationPtr->CloneMe(), true);
		}
		else{
			m_workingCalibrationPtr.SetPtr(&s_defaultTransform, false);
		}
	}
}


// private static attributes

i2d::CAffineTransformation2d CCalibration2dProxyComp::s_defaultTransform(
			i2d::CAffine2d(i2d::CMatrix2d(1, 0, 0, 1)));




} // namespace i2d


