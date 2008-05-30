#include "iqt/CSelectionParamGuiComp.h"


namespace iqt
{


// reimplemented (imod::IModelEditor)

void CSelectionParamGuiComp::UpdateModel() const
{
	iprm::ISelectionParam* objectPtr = GetObjectPtr();
	if (IsGuiCreated() && (objectPtr != NULL)){
		objectPtr->SetSelectedOptionIndex(SelectionCB->currentIndex());
	}
}


void CSelectionParamGuiComp::UpdateEditor()
{
	if (!IsGuiCreated()){
		return;
	}

	SelectionCB->clear();

	iprm::ISelectionParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int optionsCont = objectPtr->GetOptionsCount();

		for (int i = 0; i < optionsCont; ++i){
			const istd::CString& name = objectPtr->GetOptionName(i);

			SelectionCB->addItem(iqt::GetQString(name));
		}

		int selectedIndex = objectPtr->GetSelectedOptionIndex();

		if (selectedIndex >= 0){
			SelectionCB->setCurrentIndex(selectedIndex);
		}
		else if (optionsCont > 0){
			SelectionCB->setCurrentIndex(0);
		}
	}
}


// protected slots

void CSelectionParamGuiComp::on_SelectionCB_currentIndexChanged(int index)
{
	iprm::ISelectionParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->SetSelectedOptionIndex(index);
	}
}


// reimplemented (iqt::TGuiObserverWrap)

void CSelectionParamGuiComp::OnGuiModelShown()
{
	BaseClass::OnGuiModelShown();

	UpdateEditor();
}


} // namespace iqt


