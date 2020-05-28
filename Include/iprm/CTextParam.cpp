#include <iprm/CTextParam.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iprm
{


// reimplemented (iprm::ITextParam)

QString CTextParam::GetText() const
{
	return m_text;
}


void CTextParam::SetText(const QString& text)
{
	if (m_text != text){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_text = text;
	}
}


bool CTextParam::IsReadOnly() const
{
	return false;
}


// reimplemented (iser::ISerializable)

bool CTextParam::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag textTag("Text", "Text", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	bool retVal = archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CTextParam::GetSupportedOperations() const
{
	return SO_CLONE | SO_COPY | SO_COMPARE | SO_RESET;
}


bool CTextParam::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const ITextParam* sourcePtr = dynamic_cast<const ITextParam*>(&object);
	if (sourcePtr != NULL){
		SetText(sourcePtr->GetText());

		return true;
	}

	return false;
}


bool CTextParam::IsEqual(const IChangeable& object) const
{
	const ITextParam* sourcePtr = dynamic_cast<const ITextParam*>(&object);
	if (sourcePtr != NULL){
		return (m_text == sourcePtr->GetText());
	}

	return false;
}


istd::IChangeable* CTextParam::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::TDelPtr<CTextParam> clonePtr(new CTextParam);
	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


bool CTextParam::ResetData(CompatibilityMode /*mode*/)
{
	if (!m_text.isEmpty()){
		istd::CChangeNotifier changeNotifier(this);

		m_text.clear();
	}

	return true;
}


} // namespace iprm


