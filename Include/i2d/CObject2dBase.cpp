#include "i2d/CObject2dBase.h"


#include "istd/TChangeNotifier.h"


namespace i2d
{


CObject2dBase::CObject2dBase()
:	m_isUndoAllowed(true)
{
}


CObject2dBase::CObject2dBase(const CObject2dBase& object2d)
:	m_calibrationPtr(object2d.m_calibrationPtr),
	m_isUndoAllowed(true)
{
}


void CObject2dBase::StartTransform()
{
	istd::CChangeNotifier changePtr(this, i2d::IObject2d::CF_OBJECT_START_TRANSFORM);

	m_isUndoAllowed = false;
}


void CObject2dBase::FinishTransform()
{
	m_isUndoAllowed = true;

	istd::CChangeNotifier changePtr(this, i2d::IObject2d::CF_OBJECT_END_TRANSFORM);
}


bool CObject2dBase::IsUndoAllowed() const
{
	return m_isUndoAllowed;
}


// reimplemented (i2d::ICalibrationProvider)

const ICalibration2d* CObject2dBase::GetCalibration() const
{
	return m_calibrationPtr.GetPtr();
}


// reimplemented (i2d::IObject2d)

void CObject2dBase::SetCalibration(const ICalibration2d* calibrationPtr, bool releaseFlag)
{
	m_calibrationPtr.SetPtr(calibrationPtr, releaseFlag);
}


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
			if (m_calibrationPtr.GetPtr() != object2dPtr->m_calibrationPtr.GetPtr()){
				return false;
			}
			break;

		case CM_WITH_REFS:
			if (!object2dPtr->m_calibrationPtr.IsValid() || !object2dPtr->m_calibrationPtr.IsToRelase()){
				m_calibrationPtr = object2dPtr->m_calibrationPtr;
			}
			break;

		case CM_CONVERT:
			if (object2dPtr->m_calibrationPtr.IsValid()){
				retVal = Transform(*object2dPtr->m_calibrationPtr) && retVal;
			}

			if (m_calibrationPtr.IsValid()){
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


