#include "i2d/CLabel.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/TDelPtr.h"
#include "iser/CArchiveTag.h"


namespace i2d
{


// public methods

CLabel::CLabel(const i2d::CVector2d& position, const QString& labelText)
:	BaseClass(position), 
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
	static iser::CArchiveTag textTag("Text", "Text of label", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CLabel::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CLabel::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CLabel* labelPtr = dynamic_cast<const CLabel*>(&object);

	if (labelPtr != NULL){
		istd::CChangeNotifier notifier(this);
		
		SetPosition(labelPtr->GetPosition());
		SetText(labelPtr->GetText());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeable* CLabel::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CLabel> clonePtr(new CLabel);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


} // namespace i2d


