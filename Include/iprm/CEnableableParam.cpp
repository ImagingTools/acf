#include <iprm/CEnableableParam.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iprm
{


// static constants
static istd::IChangeable::ChangeSet s_enabledChange(iprm::IEnableableParam::CF_ENABLED, "Enabled");
static istd::IChangeable::ChangeSet s_disabledChange(iprm::IEnableableParam::CF_ENABLED, "Disabled");


CEnableableParam::CEnableableParam(bool isEnabled)
:	m_isEnabled(isEnabled)
{
}


// pseudo-reimplemented (iprm::IEnableableParam)

bool CEnableableParam::IsEnabled() const
{
	return m_isEnabled;
}


bool CEnableableParam::IsEnablingAllowed() const
{
	return true;
}


bool CEnableableParam::SetEnabled(bool isEnabled)
{
	if (!IsEnablingAllowed()){
		return false;
	}

	if (isEnabled != m_isEnabled){
		istd::CChangeNotifier notifier(this, isEnabled? &s_enabledChange: &s_disabledChange);

		m_isEnabled = isEnabled;
	}

	return true;
}


// reimplemented (iser::ISerializable)

bool CEnableableParam::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag enabledTag("IsEnabled", "Object enabled state", iser::CArchiveTag::TT_LEAF);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	bool isEnabled = m_isEnabled;

	bool retVal = archive.BeginTag(enabledTag);
	retVal = retVal && archive.Process(isEnabled);
	retVal = retVal && archive.EndTag(enabledTag);

	if (!isStoring){
		SetEnabled(isEnabled);
	}

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CEnableableParam::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const IEnableableParam* sourceParamPtr = dynamic_cast<const IEnableableParam*>(&object);
	if (sourceParamPtr != NULL){
		bool sourceValue = sourceParamPtr->IsEnabled();
		if (m_isEnabled != sourceValue){
			istd::CChangeNotifier notifier(this);

			m_isEnabled = sourceValue;
		}

		return true;
	}

	return false;
}


istd::IChangeable* CEnableableParam::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::TDelPtr<CEnableableParam> clonedPtr(new CEnableableParam);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr.PopPtr();
	}

	return nullptr;
}


} // namespace iprm


