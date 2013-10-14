#include "i2d/CCalibration2dProxyComp.h"


// ACF includes
#include "imod/IModel.h"
#include "i2d/CAffine2d.h"
#include "i2d/CAffineTransformation2d.h"


namespace i2d
{


// public methods

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

	istd::TDelPtr<ICalibration2d> retVal;
	retVal.SetCastedOrRemove(calibration.CloneMe());

	return retVal.PopPtr();
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

	result = origPos1.GetDistance(origPos2);

	return true;
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

	result = origPosition;

	return true;
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

	result = transfPosition;

	return true;
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

	result = CAffine2d::GetIdentity();

	return true;
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

	result = CAffine2d::GetIdentity();

	return true;
}

// reimplemented (imath::TISurjectFunction)

bool CCalibration2dProxyComp::GetInvValueAt(const CVector2d& argument, CVector2d& result) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetInvValueAt(argument, result);
	}

	result = argument;

	return true;
}


CVector2d CCalibration2dProxyComp::GetInvValueAt(const CVector2d& argument) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetInvValueAt(argument);
	}

	return argument;
}


// reimplemented (imath::TIMathFunction)

bool CCalibration2dProxyComp::GetValueAt(const CVector2d& argument, CVector2d& result) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetValueAt(argument, result);
	}

	result = argument;

	return true;
}


CVector2d CCalibration2dProxyComp::GetValueAt(const CVector2d& argument) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetValueAt(argument);
	}

	return argument;
}


// reimplemented (iser::ISerializable)

bool CCalibration2dProxyComp::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


// reimplemented (istd::IChangeable)

istd::IChangeable* CCalibration2dProxyComp::CloneMe(CompatibilityMode mode) const
{
	const ICalibration2d* calibrationPtr = GetCalibrationPtr();
	if (calibrationPtr != NULL){
		return calibrationPtr->CloneMe(mode);
	}

	return new i2d::CAffineTransformation2d(i2d::CAffine2d::GetIdentity());
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
	if (m_calibrationProviderCompPtr.IsValid()){
		return m_calibrationProviderCompPtr->GetCalibration();
	}

	return NULL;
}


} // namespace i2d


