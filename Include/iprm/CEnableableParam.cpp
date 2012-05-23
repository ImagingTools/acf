#include "iprm/CEnableableParam.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


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


void CEnableableParam::SetEnabled(bool isEnabled)
{
	if (IsEnablingAllowed() && (isEnabled != m_isEnabled)){
		istd::CChangeNotifier changePtr(this);
	
		m_isEnabled = isEnabled;
	}
}


// reimplemented (iser::ISerializable)

bool CEnableableParam::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag enabledTag("IsEnabled", "Object enabled state");

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


} // namespace iprm


