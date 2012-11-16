#include "i2d/COrientedCircle.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{	


COrientedCircle::COrientedCircle()
:	m_orientedOutside(false)
{
}


COrientedCircle::COrientedCircle(double radius, const CVector2d& center, bool orientatedOutside)
:	BaseClass(radius, center), m_orientedOutside(orientatedOutside)
{
}


void COrientedCircle::SetOrientedOutside(bool orientatedOutside)
{
	if (orientatedOutside != m_orientedOutside){
		istd::CChangeNotifier notifier(this);

		m_orientedOutside = orientatedOutside;
	}
}


bool COrientedCircle::operator==(const COrientedCircle& ref) const
{
	return BaseClass::operator==(ref) && (ref.m_orientedOutside == m_orientedOutside);
}


bool COrientedCircle::operator!=(const COrientedCircle& ref) const
{
	return BaseClass::operator!=(ref) || (ref.m_orientedOutside != m_orientedOutside);
}


// reimplemented (iser::ISerializable)

bool COrientedCircle::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag orientatedOutsideTag("IsOrientedOutside", "Flag if this circle is oriented inside or outside");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | CF_MODEL);

	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(orientatedOutsideTag);
	retVal = retVal && archive.Process(m_orientedOutside);
	retVal = retVal && archive.EndTag(orientatedOutsideTag);

	return retVal;
}


} // namespace i2d

