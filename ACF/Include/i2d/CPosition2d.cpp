#include "i2d/CPosition2d.h"


#include "istd/TChangeNotifier.h"

#include "i2d/CRectangle.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{	


CPosition2d::CPosition2d()
:	m_center(0, 0)
{
}


CPosition2d::CPosition2d(const CVector2d& center)
:	m_center(center)
{
}


void CPosition2d::SetCenter(const CVector2d& center)
{
	if (center != m_center){
		istd::CChangeNotifier notifier(this);

		m_center = center;
	}
}


// reimplemented (IObject2d)

CVector2d CPosition2d::GetCenter() const
{
	return m_center;
}


CRectangle CPosition2d::GetBoundingBox() const
{
	return CRectangle(
				m_center.GetX(), 
				m_center.GetY(), 
				m_center.GetX(), 
				m_center.GetY()); 
}


// reimplemented (iser::ISerializable)

bool CPosition2d::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag centerTag("Center", "Center position");
	retVal = retVal && archive.BeginTag(centerTag);
	retVal = retVal && m_center.Serialize(archive);
	retVal = retVal && archive.EndTag(centerTag);

	return retVal;
}


} // namespace i2d


