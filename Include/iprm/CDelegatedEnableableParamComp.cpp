#include <iprm/CDelegatedEnableableParamComp.h>


namespace iprm
{



// public methods

CDelegatedEnableableParamComp::CDelegatedEnableableParamComp()
	:m_updateBridge(this, imod::CModelUpdateBridge::UF_SOURCE)
{
}


// protected methods

// reimplemented (iprm::IEnableableParam)

bool CDelegatedEnableableParamComp::IsEnabled() const
{
	if (m_baseEnableableParamCompPtr.IsValid()){
		return m_baseEnableableParamCompPtr->IsEnabled();
	}

	return false;
}


bool CDelegatedEnableableParamComp::IsEnablingAllowed() const
{
	if (m_baseEnableableParamCompPtr.IsValid()){
		return m_baseEnableableParamCompPtr->IsEnablingAllowed();
	}

	return false;
}


bool CDelegatedEnableableParamComp::SetEnabled(bool isEnabled)
{
	if (m_baseEnableableParamCompPtr.IsValid()){
		return m_baseEnableableParamCompPtr->SetEnabled(isEnabled);
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CDelegatedEnableableParamComp::Serialize(iser::IArchive& archive)
{
	if (m_baseEnableableParamCompPtr.IsValid()){
		return m_baseEnableableParamCompPtr->Serialize(archive);
	}

	// Serialization in this case is optional:
	return true;
}


// reimplemented (istd::IChangeable)

bool CDelegatedEnableableParamComp::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	if (m_baseEnableableParamCompPtr.IsValid()){
		return m_baseEnableableParamCompPtr->CopyFrom(object, mode);
	}

	// Copying to a non-existing object is OK:
	return true;
}


// reimplemented (icomp::CComponentBase)

void CDelegatedEnableableParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_baseEnableableParamModelCompPtr.IsValid()){
		m_baseEnableableParamModelCompPtr->AttachObserver(&m_updateBridge);
	}
}


void CDelegatedEnableableParamComp::OnComponentDestroyed()
{
	if (m_baseEnableableParamModelCompPtr.IsValid() && m_baseEnableableParamModelCompPtr->IsAttached(&m_updateBridge)){
		m_baseEnableableParamModelCompPtr->DetachObserver(&m_updateBridge);
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


