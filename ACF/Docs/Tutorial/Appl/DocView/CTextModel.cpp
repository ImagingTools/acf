#include "CTextModel.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "istd/TChangeNotifier.h"


CTextModel::CTextModel()
:	m_text("Hello World")
{
}


istd::CString CTextModel::GetText() const
{
	return m_text;
}


void CTextModel::SetText(const istd::CString& text)
{
	if (m_text != text){
		istd::CChangeNotifier changePtr(this);

		m_text = text;
	}
}


// reimplemented (iser::ISerializable)

bool CTextModel::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag textTag("Text", "Text");
	bool retVal = archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	return retVal;
}
