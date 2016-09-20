#include <i2d/CNoneCalibration2d.h>


// ACF includes
#include <i2d/CAffine2d.h>


namespace i2d
{


// public methods

// reimplemented (i2d::ICalibration2d)

const i2d::CRectangle* CNoneCalibration2d::GetArgumentArea() const
{
	return NULL;
}


const i2d::CRectangle* CNoneCalibration2d::GetResultArea() const
{
	return NULL;
}


const imath::IUnitInfo* CNoneCalibration2d::GetArgumentUnitInfo() const
{
	return NULL;
}


const imath::IUnitInfo* CNoneCalibration2d::GetResultUnitInfo() const
{
	return NULL;
}


const i2d::ICalibration2d* CNoneCalibration2d::CreateCombinedCalibration(const i2d::ITransformation2d& /*transformation*/) const
{
	return new CNoneCalibration2d();
}


// reimplemented (i2d::ITransformation2d)

int CNoneCalibration2d::GetTransformationFlags() const
{
	return TF_FORWARD | TF_INJECTIVE | TF_SURJECTIVE | TF_PRESERVE_NULL | TF_PRESERVE_DISTANCE | TF_PRESERVE_ANGLE | TF_SUPPORT_COMBINE;
}


bool CNoneCalibration2d::GetDistance(
			const i2d::CVector2d& origPos1,
			const i2d::CVector2d& origPos2,
			double& result,
			i2d::ITransformation2d::ExactnessMode /*mode*/) const
{
	result = origPos1.GetDistance(origPos2);
	
	return true;
}


bool CNoneCalibration2d::GetLocalTransform(const i2d::CVector2d& /*logPosition*/, i2d::CAffine2d& result, ExactnessMode /*mode*/) const
{
	result = i2d::CAffine2d::GetIdentity();
	
	return true;
}


bool CNoneCalibration2d::GetLocalInvTransform(const i2d::CVector2d& /*viewPosition*/, i2d::CAffine2d& result, ExactnessMode /*mode*/) const
{
	result = i2d::CAffine2d::GetIdentity();
	
	return true;
}


// reimplemented (i2d::ITransformation2d)

bool CNoneCalibration2d::GetPositionAt(const i2d::CVector2d& viewPosition, i2d::CVector2d& result, ExactnessMode /*mode*/) const
{
	result = viewPosition;
	
	return true;
}


bool CNoneCalibration2d::GetInvPositionAt(const i2d::CVector2d& logPosition, i2d::CVector2d& result, ExactnessMode /*mode*/) const
{
	result = logPosition;

	return true;
}

	
// reimplemented (imath::TISurjectFunction)

bool CNoneCalibration2d::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetInvPositionAt(argument, result);
}


i2d::CVector2d CNoneCalibration2d::GetInvValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d result;

	GetInvPositionAt(argument, result);

	return result;
}


// reimplemented (imath::TIMathFunction)

bool CNoneCalibration2d::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetPositionAt(argument, result);
}


i2d::CVector2d CNoneCalibration2d::GetValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d result;
	
	GetPositionAt(argument, result);

	return result;
}


// reimplemented (iser::ISerializable)

bool CNoneCalibration2d::Serialize(iser::IArchive& /*archive*/)
{
	return true;
}


// static methods

const CNoneCalibration2d& CNoneCalibration2d::GetInstance()
{
	return s_defaultInstance;
}


// static attributes

CNoneCalibration2d CNoneCalibration2d::s_defaultInstance;


} // namespace i2d


