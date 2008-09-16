#include "iqt/CSelectionParamGuiComp.h"


#include "iqt/CSignalBlocker.h"


namespace iqt
{


// reimplemented (imod::IModelEditor)

void CSelectionParamGuiComp::UpdateModel() const
{
	if (!IsGuiCreated()){
		return;
	}

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


void CSelectionParamGuiComp::UpdateEditor()
{
	if (!IsGuiCreated()){
		return;
	}

	iqt::CSignalBlocker blocker(SelectionFrame);

	m_comboBoxes.Reset();

	for (		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
				selectionPtr != NULL;
				selectionPtr = selectionPtr->GetActiveSubselection()){
		QComboBox* switchBoxPtr = new QComboBox(NULL);
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


// protected slots

void CSelectionParamGuiComp::OnSelectionChanged(int /*index*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(this);

		UpdateModel();
		UpdateEditor();
	}
}


// reimplemented (iqt::TGuiObserverWrap)

void CSelectionParamGuiComp::OnGuiModelShown()
{
	BaseClass::OnGuiModelShown();

	UpdateEditor();
}


} // namespace iqt


