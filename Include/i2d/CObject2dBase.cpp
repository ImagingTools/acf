#include "i2d/CObject2dBase.h"


namespace i2d
{


CObject2dBase::CObject2dBase()
:	m_calibrationPtr(NULL)
{
}


CObject2dBase::CObject2dBase(const CObject2dBase& object2d)
:	m_calibrationPtr(object2d.m_calibrationPtr)
{
}


void CObject2dBase::SetCalibration(const ICalibration2d* calibrationPtr)
{
	m_calibrationPtr = calibrationPtr;
}


// reimplemented (i2d::ICalibrationProvider)

const ICalibration2d* CObject2dBase::GetCalibration() const
{
	return m_calibrationPtr;
}


// reimplemented (i2d::IObject2d)

bool CObject2dBase::Transform(
			const ITransformation2d& /*transformation*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	return false;
}


bool CObject2dBase::InvTransform(
			const ITransformation2d& /*transformation*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	return false;
}


bool CObject2dBase::GetTransformed(
			const ITransformation2d& /*transformation*/,
			IObject2d& /*result*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/) const
{
	return false;
}


bool CObject2dBase::GetInvTransformed(
			const ITransformation2d& /*transformation*/,
			IObject2d& /*result*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/) const
{
	return false;
}


} // namespace i2d


