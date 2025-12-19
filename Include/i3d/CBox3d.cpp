#include <i3d/CBox3d.h>


// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace i3d
{


// static constants
static const iser::CArchiveTag s_minTag("Min", "Minimum corner", iser::CArchiveTag::TT_BRANCH);
static const iser::CArchiveTag s_maxTag("Max", "Maximum corner", iser::CArchiveTag::TT_BRANCH);


CVector3d CBox3d::GetClosestPoint(const CVector3d& point) const
{
	if (IsEmpty()){
		return point;
	}
	
	// Clamp point to box bounds
	CVector3d result;
	result.SetX(qMax(m_min.GetX(), qMin(point.GetX(), m_max.GetX())));
	result.SetY(qMax(m_min.GetY(), qMin(point.GetY(), m_max.GetY())));
	result.SetZ(qMax(m_min.GetZ(), qMin(point.GetZ(), m_max.GetZ())));
	
	return result;
}


bool CBox3d::Serialize(iser::IArchive& archive)
{
	bool retVal = true;
	
	retVal = retVal && archive.BeginTag(s_minTag);
	retVal = retVal && m_min.Serialize(archive);
	retVal = retVal && archive.EndTag(s_minTag);
	
	retVal = retVal && archive.BeginTag(s_maxTag);
	retVal = retVal && m_max.Serialize(archive);
	retVal = retVal && archive.EndTag(s_maxTag);
	
	return retVal;
}


} // namespace i3d
