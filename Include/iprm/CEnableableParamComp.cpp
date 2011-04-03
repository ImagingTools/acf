#include "iprm/CEnableableParamComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (istd::IEnableable)

bool CEnableableParamComp::IsEnabled() const
{
	return m_isEnabled;
}


bool CEnableableParamComp::IsEnablingAllowed() const
{
	return true;
}


void CEnableableParamComp::SetEnabled(bool isEnabled)
{
	if (isEnabled != m_isEnabled){
		istd::CChangeNotifier notifier(this);

		m_isEnabled = isEnabled;
	}
}


// reimplemented (iser::ISerializable)

bool CEnableableParamComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag isEnabledTag("IsEnabled", "Flag if element is enabled");
	retVal = retVal && archive.BeginTag(isEnabledTag);
	retVal = retVal && archive.Process(m_isEnabled);
	retVal = retVal && archive.EndTag(isEnabledTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CEnableableParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultIsEnabledAttrPtr.IsValid()){
		m_isEnabled = *m_defaultIsEnabledAttrPtr;
	}
}


} // namespace iprm


