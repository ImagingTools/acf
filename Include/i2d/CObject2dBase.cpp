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


// reimplemented (istd::IChangeable)

bool CObject2dBase::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	bool retVal = true;

	const CObject2dBase* object2dPtr = dynamic_cast<const CObject2dBase*>(&object);
	if (object2dPtr != NULL){
		switch (mode){
		case CM_STRICT:
			if (m_calibrationPtr != object2dPtr->m_calibrationPtr){
				return false;
			}
			break;

		case CM_WITH_REFS:
			m_calibrationPtr = object2dPtr->m_calibrationPtr;
			break;

		case CM_CONVERT:
			if (object2dPtr->m_calibrationPtr != NULL){
				retVal = Transform(*object2dPtr->m_calibrationPtr) && retVal;
			}

			if (m_calibrationPtr != NULL){
				retVal = InvTransform(*m_calibrationPtr) && retVal;
			}

			break;

		default:
			break;
		}
	}

	return retVal;
}


} // namespace i2d


