#include "icmpstr/CComponentNote.h"


#include "istd/TChangeNotifier.h"

#include "iser/CArchiveTag.h"


namespace icmpstr
{


// public methods

void CComponentNote::SetText(const istd::CString& noteText)
{
	m_noteText = noteText;
}


void CComponentNote::SetPosition(const i2d::CVector2d& position)
{
	m_position = position;
}


// reimplemented (IComponentNote)

istd::CString CComponentNote::GetText() const
{
	return m_noteText;
}


i2d::CVector2d CComponentNote::GetPosition() const
{
	return m_position;
}


// reimplemented (iser::ISerializable)

bool CComponentNote::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag textTag("NoteText", "Note text");
	static iser::CArchiveTag positionTag("NotePosition", "Note position");
	
	bool retVal = archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_noteText);
	retVal = retVal && archive.EndTag(textTag);

	retVal = retVal && archive.BeginTag(positionTag);
	retVal = retVal && m_position.Serialize(archive);
	retVal = retVal && archive.EndTag(positionTag);

	return retVal;
}


} // namespace icmpstr


