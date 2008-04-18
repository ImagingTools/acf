#include "CTextModelComp.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "istd/TChangeNotifier.h"


istd::CString CTextModelComp::GetText() const
{
	return m_text;
}


void CTextModelComp::SetText(const istd::CString& text)
{
	if (m_text != text){
		istd::CChangeNotifier changePtr(this);

		m_text = text;
	}
}


// reimplemented (icomp::IComponent)

void CTextModelComp::OnComponentCreated()
{
	if (m_defaultTextAttrPtr.IsValid()){
		m_text = *m_defaultTextAttrPtr;
	}
}


// reimplemented (iser::ISerializable)

bool CTextModelComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag textTag("Text", "Text");
	bool retVal = archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	return retVal;
}


