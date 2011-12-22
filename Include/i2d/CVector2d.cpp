#include "i2d/CVector2d.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{


void CVector2d::Init(double angle, double length)
{
	SetX(::cos(angle) * length);
	SetY(::sin(angle) * length);
}


CVector2d CVector2d::GetOrthogonal() const
{
	return CVector2d(GetY(), -GetX());
}


istd::CIndex2d CVector2d::ToIndex2d() const
{
	return istd::CIndex2d(int(GetX()), int(GetY()));
}


CVector2d CVector2d::GetNormalized(double length) const
{
	CVector2d retVal;

    if (GetNormalized(retVal, length)){
		return retVal;
    }

	return CVector2d(length, 0.0);
}


} // namespace i2d


