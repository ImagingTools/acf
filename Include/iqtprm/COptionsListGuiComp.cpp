#include "iqtprm/COptionsListGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


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
		iprm::IParamsManager* selectionParamPtr = reinterpret_cast<iprm::IParamsManager*>(selectedItemPtr->data(0, DR_OBJECT_PTR).toUInt());

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
	
	iprm::IParamsManager* objectPtr = GetObjectPtr();
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

void COptionsListGuiComp::CreateOptionsTree(const iprm::IParamsManager* paramsManagerPtr, QTreeWidgetItem* parentItemPtr)
{
	if (paramsManagerPtr != NULL){
		int selectedOptionIndex = paramsManagerPtr->GetSelectedOptionIndex();

		for (int optionIndex = 0; optionIndex < paramsManagerPtr->GetParamsSetsCount(); optionIndex++){
			QString optionName = iqt::GetQString(paramsManagerPtr->GetParamsSetName(optionIndex));

			QTreeWidgetItem* itemPtr = new QTreeWidgetItem(OptionsList);
			itemPtr->setText(0, optionName);
			itemPtr->setData(0, DR_SELECTION_INDEX, optionIndex);
			itemPtr->setData(0, DR_OBJECT_PTR, quintptr(paramsManagerPtr));

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
	}
}


} // namespace iqtprm


