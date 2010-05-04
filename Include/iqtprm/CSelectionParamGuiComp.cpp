#include "iqtprm/CSelectionParamGuiComp.h"


#include "iqt/CSignalBlocker.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void CSelectionParamGuiComp::UpdateModel() const
{
	if (!IsGuiCreated()){
		return;
	}
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CSelectionParamGuiComp*>(this));

		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
		int switchesCount = m_comboBoxes.GetCount();
		for (		int switchIndex = 0;
					(selectionPtr != NULL) && switchIndex < switchesCount;
					++switchIndex){
			const QComboBox* switchBoxPtr = m_comboBoxes.GetAt(switchIndex);
			selectionPtr->SetSelectedOptionIndex(switchBoxPtr->currentIndex());

			selectionPtr = selectionPtr->GetActiveSubselection();
		}
	}
}


void CSelectionParamGuiComp::UpdateEditor(int /*updateFlags*/)
{
	if (!IsGuiCreated()){
		return;
	}

	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker block(this);

	m_comboBoxes.Reset();

	for (		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
				selectionPtr != NULL;
				selectionPtr = selectionPtr->GetActiveSubselection()){
		QComboBox* switchBoxPtr = new QComboBox(SelectionFrame);
		m_comboBoxes.PushBack(switchBoxPtr);
		QLayout* layoutPtr = SelectionFrame->layout();
		if (layoutPtr != NULL){
			layoutPtr->addWidget(switchBoxPtr);
		}

		QObject::connect(switchBoxPtr, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectionChanged(int)));

		int optionsCont = selectionPtr->GetOptionsCount();
	
		for (int i = 0; i < optionsCont; ++i){
			const istd::CString& name = selectionPtr->GetOptionName(i);

			switchBoxPtr->addItem(iqt::GetQString(name));
		}

		int selectedIndex = selectionPtr->GetSelectedOptionIndex();

		if (selectedIndex >= 0){
			switchBoxPtr->setCurrentIndex(selectedIndex);
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSelectionParamGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_optionsLabelAttrPtr.IsValid()){
		SelectionLabel->setText(iqt::GetQString(*m_optionsLabelAttrPtr));
	}
}


void CSelectionParamGuiComp::OnGuiDestroyed()
{
	m_comboBoxes.Reset();

	BaseClass::OnGuiDestroyed();
}


// protected slots

void CSelectionParamGuiComp::OnSelectionChanged(int /*index*/)
{
	UpdateModel();
}


} // namespace iqtprm


