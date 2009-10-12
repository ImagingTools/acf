#include "icmpstr/CNote.h"


#include "istd/TChangeNotifier.h"

#include "iser/CArchiveTag.h"


namespace icmpstr
{


// public methods

// reimplemented (IComponentNote)

istd::CString CNote::GetText() const
{
	return m_noteText;
}


void CNote::SetText(const istd::CString& noteText)
{
	if (noteText != m_noteText){
		istd::CChangeNotifier notifier(this);

		m_noteText = noteText;
	}
}


i2d::CVector2d CNote::GetPosition() const
{
	return m_position;
}


void CNote::SetPosition(const i2d::CVector2d& position)
{
	if (position != m_position){
		istd::CChangeNotifier notifier(this);

		m_position = position;
	}
}


// reimplemented (iser::ISerializable)

bool CNote::Serialize(iser::IArchive& archive)
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


