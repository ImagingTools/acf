#include "idoc/CTextDocumentComp.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "istd/CChangeNotifier.h"


namespace idoc
{


// public methods

// reimplemented (idoc::ITextDocument)

QString CTextDocumentComp::GetText() const
{
	return m_text;
}


void CTextDocumentComp::SetText(const QString& text)
{
	if (m_text != text){
		istd::CChangeNotifier changePtr(this);

		m_text = text;
	}
}


// reimplemented (iser::ISerializable)

bool CTextDocumentComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag textTag("Text", "Text", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CTextDocumentComp::OnComponentCreated()
{
	if (m_defaultTextAttrPtr.IsValid()){
		m_text = *m_defaultTextAttrPtr;
	}
}


} // namespace idoc


