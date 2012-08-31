#include "iqtprm/CEnableableParamGuiComp.h"


namespace iqtprm
{


// public methods

// reimplemented (imod::IModelEditor)

void CEnableableParamGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iprm::IEnableableParam* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	UpdateBlocker updateBlocker(const_cast<CEnableableParamGuiComp*>(this));

	bool isEnabled = EnableableCheck->isChecked();
	if (isEnabled != objectPtr->IsEnabled()){
		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetEnabled(isEnabled);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CEnableableParamGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::IEnableableParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		
		EnableableCheck->setChecked(objectPtr->IsEnabled());	
		EnableableCheck->setEnabled(objectPtr->IsEnablingAllowed());
	}
}


void CEnableableParamGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	iprm::IEnableableParam* objectPtr = GetObjectPtr();
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
}

// protected slots

void CEnableableParamGuiComp::on_EnableableCheck_stateChanged(int /*state*/)
{
	DoUpdateModel();
}

} // namespace iqtprm


