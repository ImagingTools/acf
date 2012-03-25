#include "iview/CNoneCalibration.h"


// ACF includes
#include "i2d/CAffine2d.h"


namespace iview
{


// public methods

// reimplemented (i2d::ITransformation2d)

int CNoneCalibration::GetTransformationFlags() const
{
	return TF_FORWARD | TF_INJECTIVE | TF_SURJECTIVE | TF_PRESERVE_NULL | TF_PRESERVE_DISTANCE | TF_PRESERVE_ANGLE;
}


bool CNoneCalibration::GetDistance(
			const i2d::CVector2d& origPos1,
			const i2d::CVector2d& origPos2,
			double& result,
			i2d::ITransformation2d::ExactnessMode /*mode*/) const
{
	result = origPos1.GetDistance(origPos2);
	
	return true;
}


bool CNoneCalibration::GetLocalTransform(const i2d::CVector2d& /*logPosition*/, i2d::CAffine2d& result, ExactnessMode /*mode*/) const
{
	result = i2d::CAffine2d::GetIdentity();
	
	return true;
}


bool CNoneCalibration::GetLocalInvTransform(const i2d::CVector2d& /*viewPosition*/, i2d::CAffine2d& result, ExactnessMode /*mode*/) const
{
	result = i2d::CAffine2d::GetIdentity();
	
	return true;
}


// reimplemented (i2d::ITransformation2d)

bool CNoneCalibration::GetPositionAt(const i2d::CVector2d& viewPosition, i2d::CVector2d& result, ExactnessMode /*mode*/) const
{
	result = viewPosition;
	
	return true;
}


bool CNoneCalibration::GetInvPositionAt(const i2d::CVector2d& logPosition, i2d::CVector2d& result, ExactnessMode /*mode*/) const
{
	result = logPosition;

	return true;
}

	
const i2d::ITransformation2d* CNoneCalibration::CreateCombinedTransformation(const i2d::ITransformation2d& /*transform*/) const
{
	return new CNoneCalibration();
}


// reimplemented (imath::TISurjectFunction)

bool CNoneCalibration::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetInvPositionAt(argument, result);
}


i2d::CVector2d CNoneCalibration::GetInvValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d result;

	GetInvPositionAt(argument, result);

	return result;
}


// reimplemented (imath::TIMathFunction)

bool CNoneCalibration::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetPositionAt(argument, result);
}


i2d::CVector2d CNoneCalibration::GetValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d result;
	
	GetPositionAt(argument, result);

	return result;
}


// reimplemented (iser::ISerializable)

bool CNoneCalibration::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


// static methods

const CNoneCalibration& CNoneCalibration::GetInstance()
{
	return s_defaultInstance;
}


// static attributes

CNoneCalibration CNoneCalibration::s_defaultInstance;


} // namespace iview


