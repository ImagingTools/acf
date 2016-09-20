#include <iview/CScreenTransform.h>


namespace iview
{


void CScreenTransform::GetApply(const i2d::CAffine2d& transform, CScreenTransform& result) const
{
	BaseClass::GetApply(transform, result);

	result.CalculateInvert();
}


void CScreenTransform::ApplyLeft(const i2d::CAffine2d& transform)
{
	BaseClass::ApplyLeft(transform);

	CalculateInvert();
}


} // namespace iview

