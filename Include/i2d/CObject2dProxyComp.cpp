#include <i2d/CObject2dProxyComp.h>


// ACF includes
#include <i2d/CRectangle.h>


namespace i2d
{


// public methods

// reimplemented (i2d::IObject2d)

CVector2d CObject2dProxyComp::GetCenter() const
{
	const i2d::IObject2d* objectPtr = GetObject2d();
	if (objectPtr != NULL){
		return objectPtr->GetCenter();
	}

	return CVector2d();
}


void CObject2dProxyComp::MoveCenterTo(const CVector2d& /*position*/)
{
}


CRectangle CObject2dProxyComp::GetBoundingBox() const
{
	const i2d::IObject2d* objectPtr = GetObject2d();
	if (objectPtr != NULL){
		return objectPtr->GetBoundingBox();
	}

	return CRectangle(0, 0, 0, 0);
}


void CObject2dProxyComp::SetCalibration(const ICalibration2d* /*calibrationPtr*/, bool /*releaseFlag*/)
{
}


bool CObject2dProxyComp::Transform(
			const ITransformation2d& /*transformation*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	return false;
}


bool CObject2dProxyComp::InvTransform(
			const ITransformation2d& /*transformation*/,
			ITransformation2d::ExactnessMode /*mode*/,
			double* /*errorFactorPtr*/)
{
	return false;
}


bool CObject2dProxyComp::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	const i2d::IObject2d* objectPtr = GetObject2d();
	if (objectPtr != NULL){
		return objectPtr->GetTransformed(transformation, result, mode, errorFactorPtr);
	}

	return false;
}


bool CObject2dProxyComp::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	const i2d::IObject2d* objectPtr = GetObject2d();
	if (objectPtr != NULL){
		return objectPtr->GetInvTransformed(transformation, result, mode, errorFactorPtr);
	}

	return false;
}


// reimplemented (i2d::IClibrationProvider)

const ICalibration2d* CObject2dProxyComp::GetCalibration() const
{
	const i2d::IObject2d* objectPtr = GetObject2d();
	if (objectPtr != NULL){
		return objectPtr->GetCalibration();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CObject2dProxyComp::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


// reimplemented (istd::IChangeable)

istd::IChangeableUniquePtr CObject2dProxyComp::CloneMe(CompatibilityMode mode) const
{
	const i2d::IObject2d* objectPtr = GetObject2d();
	if (objectPtr != NULL){
		return objectPtr->CloneMe(mode);
	}

	return istd::IChangeableUniquePtr();
}


// private methods

const i2d::IObject2d* CObject2dProxyComp::GetObject2d() const
{
	if (m_object2dProviderCompPtr.IsValid()){
		return m_object2dProviderCompPtr->GetObject2d();
	}

	return NULL;
}


} // namespace i2d


