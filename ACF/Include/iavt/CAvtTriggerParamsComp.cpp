#include "iavt/CAvtTriggerParamsComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iavt
{


CAvtTriggerParamsComp::CAvtTriggerParamsComp()
	:m_isTriggerEnabled(false),
	m_triggerMode(EdgeMode),
	m_triggerPolarity(RisingPolarity)
{
}


// reimplemented (iavt::IAvtTriggerParams)

bool CAvtTriggerParamsComp::IsTriggerEnabled() const
{
	return m_isTriggerEnabled;
}


void CAvtTriggerParamsComp::SetTriggerEnabled(bool isEnabled)
{
	if (m_isTriggerEnabled != isEnabled){
		istd::CChangeNotifier changePtr(this);

		m_isTriggerEnabled = isEnabled;
	}
}


int CAvtTriggerParamsComp::GetTriggerMode() const
{
	return m_triggerMode;
}


void CAvtTriggerParamsComp::SetTriggerMode(int triggerMode)
{
	if (m_triggerMode!= triggerMode){
		istd::CChangeNotifier changePtr(this);

		m_triggerMode = triggerMode;
	}
}


int CAvtTriggerParamsComp::GetTriggerPolarity() const
{
	return m_triggerPolarity;
}


void CAvtTriggerParamsComp::SetTriggerPolarity(int triggerPolarity)
{
	if (m_triggerPolarity!= triggerPolarity){
		istd::CChangeNotifier changePtr(this);

		m_triggerPolarity = triggerPolarity;
	}
}


// reimplemented (iser::ISerializable)

bool CAvtTriggerParamsComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag isTriggerEnabledTag("TriggerEnabled", "Is external trigger enabled");

	bool retVal = archive.BeginTag(isTriggerEnabledTag);
	retVal = retVal && archive.Process(m_isTriggerEnabled);
	retVal = retVal && archive.EndTag(isTriggerEnabledTag);

	static iser::CArchiveTag bottomTag("TriggerMode", "Trigger mode");
	retVal = retVal && archive.BeginTag(bottomTag);
	retVal = retVal && archive.Process(m_triggerMode);
	retVal = retVal && archive.EndTag(bottomTag);

	static iser::CArchiveTag leftTag("TriggerPolarity", "Trigger polarity");
	retVal = retVal && archive.BeginTag(leftTag);
	retVal = retVal && archive.Process(m_triggerPolarity);
	retVal = retVal && archive.EndTag(leftTag);

	return retVal;
}


// reimplemented (icomp::IComponent)

void CAvtTriggerParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_isTriggerEnabledAttrPtr.IsValid()){
		m_isTriggerEnabled = *m_isTriggerEnabledAttrPtr;
	}

	if (m_triggerModeAttrPtr.IsValid()){
		m_triggerMode = *m_triggerModeAttrPtr;
	}

	if (m_triggerPolarityAttrPtr.IsValid()){
		m_triggerPolarity = *m_triggerPolarityAttrPtr;
	}
}


} // namespace iavt


