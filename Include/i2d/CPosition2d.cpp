#include "i2d/CPosition2d.h"


#include "istd/TChangeNotifier.h"

#include "i2d/CRectangle.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{	


CPosition2d::CPosition2d()
:	m_position(0, 0)
{
}


CPosition2d::CPosition2d(const CVector2d& position)
:	m_position(position)
{
}


void CPosition2d::SetPosition(const CVector2d& position)
{
	if (position != m_position){
		istd::CChangeNotifier notifier(this, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		m_position = position;
	}
}


// reimplemented (IObject2d)

CVector2d CPosition2d::GetCenter() const
{
	return m_position;
}


void CPosition2d::MoveCenterTo(const CVector2d& position)
{
	SetPosition(position);
}


// reimplemented (iser::ISerializable)

bool CPosition2d::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag centerTag("Center", "Center position");
	retVal = retVal && archive.BeginTag(centerTag);
	retVal = retVal && m_position.Serialize(archive);
	retVal = retVal && archive.EndTag(centerTag);

	return retVal;
}


} // namespace i2d


