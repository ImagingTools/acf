#include "i2d/CLabel.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace i2d
{


CLabel::CLabel(const i2d::CVector2d& position, const QString& labelText)
	:BaseClass(position), 
	m_text(labelText)
{
}


void CLabel::SetText(const QString& labelText)
{
	m_text = labelText;
}


// reimplemented (iser::ISerializable)

bool CLabel::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag textTag("Text", "Text of label");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

	bool retVal = true;

	retVal = retVal && BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	return retVal;
}


} // namespace i2d


