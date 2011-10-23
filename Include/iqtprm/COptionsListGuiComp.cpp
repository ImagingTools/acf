#include "iqtprm/COptionsListGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/ISelectionConstraints.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void COptionsListGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	QList<QTreeWidgetItem*> selectedItems = OptionsList->selectedItems();
	if (selectedItems.isEmpty()){
		iprm::ISelectionParam* objectPtr = GetObjectPtr();
		I_ASSERT(objectPtr != NULL);

		objectPtr->SetSelectedOptionIndex(iprm::ISelectionParam::NO_SELECTION);
	}
	else{
		QTreeWidgetItem* selectedItemPtr = selectedItems[0];

		int selectedIndex = selectedItemPtr->data(0, DR_SELECTION_INDEX).toInt();
		iprm::ISelectionParam* selectionParamPtr = reinterpret_cast<iprm::ISelectionParam*>(selectedItemPtr->data(0, DR_SELECTION_PARAM).toUInt());

		I_ASSERT(selectionParamPtr != NULL);

		selectionParamPtr->SetSelectedOptionIndex(selectedIndex);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void COptionsListGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	OptionsList->clear();
	
	iprm::ISelectionParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		CreateOptionsTree(objectPtr);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void COptionsListGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(OptionsList, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectionChanged()));
}


void COptionsListGuiComp::OnGuiDestroyed()
{
	BaseClass::OnGuiDestroyed();
}


// protected slots

void COptionsListGuiComp::OnSelectionChanged()
{
	if (!IsUpdateBlocked() && IsModelAttached()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


// private methods

void COptionsListGuiComp::CreateOptionsTree(const iprm::ISelectionParam* selectionParamPtr, QTreeWidgetItem* parentItemPtr)
{
	const iprm::ISelectionConstraints* selectionContraints = selectionParamPtr->GetSelectionConstraints();
	if (selectionParamPtr != NULL){
		int selectedOptionIndex = selectionParamPtr->GetSelectedOptionIndex();

		for (int optionIndex = 0; optionIndex < selectionContraints->GetOptionsCount(); optionIndex++){
			QString optionName = iqt::GetQString(selectionContraints->GetOptionName(optionIndex));

			QTreeWidgetItem* itemPtr = new QTreeWidgetItem(OptionsList);
			itemPtr->setText(0, optionName);
			itemPtr->setData(0, DR_SELECTION_INDEX, optionIndex);
			itemPtr->setData(0, DR_SELECTION_PARAM, quintptr(selectionParamPtr));

			if (parentItemPtr != NULL){
				parentItemPtr->addChild(itemPtr);
			}
			else{
				OptionsList->addTopLevelItem(itemPtr);
			}

			if (selectedOptionIndex == optionIndex){
				itemPtr->setSelected(true);
			}
		}

		iprm::ISelectionParam* subSelectionPtr = selectionParamPtr->GetActiveSubselection();
		if (subSelectionPtr != NULL){
			CreateOptionsTree(subSelectionPtr, parentItemPtr);
		}
	}
}


} // namespace iqtprm


