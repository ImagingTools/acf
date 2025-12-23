#include <i3d/CLine3d.h>


// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace i3d
{


// static constants
static const iser::CArchiveTag s_point1Tag("Point1", "First point", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_point2Tag("Point2", "Second point", iser::CArchiveTag::TT_LEAF);


double CLine3d::GetClosestParameter(const CVector3d& point) const
{
	CVector3d direction = m_point2 - m_point1;
	double lengthSq = direction.GetLength2();
	
	if (lengthSq < I_BIG_EPSILON){
		return 0.0;
	}
	
	CVector3d toPoint = point - m_point1;
	double t = toPoint.GetDotProduct(direction) / lengthSq;
	
	return t;
}


CVector3d CLine3d::GetClosestPoint(const CVector3d& point) const
{
	double t = GetClosestParameter(point);
	
	// Clamp to segment bounds
	if (t < 0.0){
		t = 0.0;
	}
	else if (t > 1.0){
		t = 1.0;
	}
	
	return GetPointAt(t);
}


double CLine3d::GetDistanceSq(const CVector3d& point) const
{
	CVector3d closest = GetClosestPoint(point);
	return (point - closest).GetLength2();
}


double CLine3d::GetDistance(const CVector3d& point) const
{
	return qSqrt(GetDistanceSq(point));
}


bool CLine3d::Serialize(iser::IArchive& archive)
{
	bool retVal = true;
	
	retVal = retVal && archive.BeginTag(s_point1Tag);
	retVal = retVal && m_point1.Serialize(archive);
	retVal = retVal && archive.EndTag(s_point1Tag);
	
	retVal = retVal && archive.BeginTag(s_point2Tag);
	retVal = retVal && m_point2.Serialize(archive);
	retVal = retVal && archive.EndTag(s_point2Tag);
	
	return retVal;
}


} // namespace i3d
