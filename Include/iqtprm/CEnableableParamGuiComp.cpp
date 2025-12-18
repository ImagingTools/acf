#include <iqtprm/CEnableableParamGuiComp.h>


namespace iqtprm
{

// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CEnableableParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::IEnableableParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		
		EnableableCheck->setChecked(objectPtr->IsEnabled());	
		EnableableCheck->setEnabled(objectPtr->IsEnablingAllowed());
		if (m_enableableToolTipAttrPtr.IsValid())
			EnableableCheck->setToolTip(*m_enableableToolTipAttrPtr);
	}
}


void CEnableableParamGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iprm::IEnableableParam* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	UpdateBlocker updateBlocker(const_cast<CEnableableParamGuiComp*>(this));

	bool isEnabled = EnableableCheck->isChecked();
	if (isEnabled != objectPtr->IsEnabled()){
		objectPtr->SetEnabled(isEnabled);
	}
}


void CEnableableParamGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	iprm::IEnableableParam* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		EnableableCheck->setChecked(objectPtr->IsEnabled());
		EnableableCheck->setEnabled(objectPtr->IsEnablingAllowed());		
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CEnableableParamGuiComp::OnGuiCreated()
{
	if (m_enableableLabelAttrPtr.IsValid()){
		EnableableCheck->setText(*m_enableableLabelAttrPtr);
	}

	BaseClass::OnGuiCreated();
}

void CEnableableParamGuiComp::OnGuiRetranslate()
{
	if (m_enableableLabelAttrPtr.IsValid()){
		EnableableCheck->setText(*m_enableableLabelAttrPtr);
	}
	if (m_enableableToolTipAttrPtr.IsValid())
		EnableableCheck->setToolTip(*m_enableableToolTipAttrPtr);
}

// protected slots

void CEnableableParamGuiComp::on_EnableableCheck_stateChanged(int /*state*/)
{
	DoUpdateModel();
}

} // namespace iqtprm


