#include <i3d/CSphere.h>


// ACF includes
#include <i3d/CLine3d.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace i3d
{


// static constants
static const iser::CArchiveTag s_centerTag("Center", "Center position", iser::CArchiveTag::TT_BRANCH);
static const iser::CArchiveTag s_radiusTag("Radius", "Radius", iser::CArchiveTag::TT_LEAF);


bool CSphere::Intersects(const CLine3d& line) const
{
	// Get closest point on line to sphere center
	CVector3d closest = line.GetClosestPoint(m_center);
	double distSq = (closest - m_center).GetLengthSq();
	
	return distSq <= m_radius * m_radius;
}


bool CSphere::Serialize(iser::IArchive& archive)
{
	bool retVal = true;
	
	retVal = retVal && archive.BeginTag(s_centerTag);
	retVal = retVal && m_center.Serialize(archive);
	retVal = retVal && archive.EndTag(s_centerTag);
	
	retVal = retVal && archive.BeginTag(s_radiusTag);
	retVal = retVal && archive.Process(m_radius);
	retVal = retVal && archive.EndTag(s_radiusTag);
	
	return retVal;
}


} // namespace i3d
