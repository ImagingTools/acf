#include "iqtprm/CParamsManagerGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CChangeDelegator.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"

#include "iprm/IParamsSet.h"
#include "iprm/ISelectionConstraints.h"

#include "iqtgui/CItemDelegate.h"


namespace iqtprm
{


// public methods

CParamsManagerGuiComp::CParamsManagerGuiComp()
:	m_lastConnectedModelPtr(NULL)
{
}


// protected slots

void CParamsManagerGuiComp::on_AddButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		I_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		objectPtr->InsertParamsSet(selectedIndex);
	}
}


void CParamsManagerGuiComp::on_RemoveButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		EnsureParamsGuiDetached();

		int selectedIndex = GetSelectedIndex();
		I_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if (selectedIndex >= 0){
			objectPtr->RemoveParamsSet(selectedIndex);
		}
	}
}


void CParamsManagerGuiComp::on_UpButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		I_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if (selectedIndex < 1){
			return;
		}

		iprm::IParamsSet* originalParamsPtr = objectPtr->GetParamsSet(selectedIndex);
		if (originalParamsPtr == NULL){
			return;
		}

		iser::CMemoryWriteArchive storeArchive;
		if (!originalParamsPtr->Serialize(storeArchive)){
			return;
		}

		QString name = objectPtr->GetParamsSetName(selectedIndex);

		istd::CChangeNotifier notifier(objectPtr);

		int newIndex = objectPtr->InsertParamsSet(selectedIndex - 1);
		if (newIndex < 0){
			return;
		}

		int indexToRemove = newIndex;

		iprm::IParamsSet* newParamsPtr = objectPtr->GetParamsSet(newIndex);
		I_ASSERT(newParamsPtr != NULL);	// insert returned success, it cannot be NULL!

		if (		(selectedIndex + 1 < objectPtr->GetParamsSetsCount()) &&
					((objectPtr->GetIndexOperationFlags(selectedIndex + 1) & iprm::IParamsManager::MF_NO_DELETE) == 0)){
			iser::CMemoryReadArchive restoreArchive(storeArchive);
			if (newParamsPtr->Serialize(restoreArchive)){
				objectPtr->SetParamsSetName(newIndex, name);
				indexToRemove = selectedIndex + 1;
			}
		}

		I_ASSERT(indexToRemove < objectPtr->GetParamsSetsCount());

		objectPtr->RemoveParamsSet(indexToRemove);

		objectPtr->SetSelectedOptionIndex(newIndex);
	}
}


void CParamsManagerGuiComp::on_DownButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		I_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if ((selectedIndex < 0) || (selectedIndex >= objectPtr->GetParamsSetsCount() - 1)){
			return;
		}

		iprm::IParamsSet* originalParamsPtr = objectPtr->GetParamsSet(selectedIndex);
		if (originalParamsPtr == NULL){
			return;
		}

		iser::CMemoryWriteArchive storeArchive;
		if (!originalParamsPtr->Serialize(storeArchive)){
			return;
		}

		QString name = objectPtr->GetParamsSetName(selectedIndex);

		istd::CChangeNotifier notifier(objectPtr);

		int newIndex = objectPtr->InsertParamsSet(selectedIndex + 2);
		if (newIndex < 0){
			return;
		}

		int indexToRemove = newIndex;

		iprm::IParamsSet* newParamsPtr = objectPtr->GetParamsSet(newIndex);
		I_ASSERT(newParamsPtr != NULL);	// insert returned success, it cannot be NULL!

		if ((objectPtr->GetIndexOperationFlags(selectedIndex) & iprm::IParamsManager::MF_NO_DELETE) == 0){
			iser::CMemoryReadArchive restoreArchive(storeArchive);
			if (newParamsPtr->Serialize(restoreArchive)){
				objectPtr->SetParamsSetName(newIndex, name);
				indexToRemove = selectedIndex;
			}
		}

		I_ASSERT(indexToRemove < objectPtr->GetParamsSetsCount());

		objectPtr->RemoveParamsSet(indexToRemove);
		--newIndex;	// correction becouse item for lower index was removed

		objectPtr->SetSelectedOptionIndex(newIndex);
	}
}


void CParamsManagerGuiComp::on_ParamsTree_itemSelectionChanged()
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	int selectedIndex = GetSelectedIndex();

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr == NULL){
		return;
	}

	const iprm::ISelectionConstraints* constraintsPtr = selectionPtr->GetSelectionConstraints();

	if (		(constraintsPtr != NULL) &&
				(selectedIndex < constraintsPtr->GetOptionsCount()) &&
				(selectedIndex != selectionPtr->GetSelectedOptionIndex())){
		if (selectionPtr->SetSelectedOptionIndex(selectedIndex)){
			UpdateParamsView(selectedIndex);
		}

		return;
	}

	if (selectedIndex < 0){
		UpdateParamsView(-1);
	}
}


void CParamsManagerGuiComp::on_ParamsTree_itemChanged(QTreeWidgetItem* item, int column)
{
	if (column != 0){
		return;
	}

	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int setIndex = item->data(0, Qt::UserRole).toInt();

		objectPtr->SetParamsSetName(setIndex, item->text(0));
	}
}


// protected methods

void CParamsManagerGuiComp::UpdateActions()
{
	I_ASSERT(IsGuiCreated());

	int flags = -1;
	int prevFlags = -1;
	int nextFlags = -1;

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int currentIndex = objectPtr->GetSelectedOptionIndex();

		flags = objectPtr->GetIndexOperationFlags(currentIndex);

		if (currentIndex >= 0){
			if (currentIndex >= 1){
				prevFlags = objectPtr->GetIndexOperationFlags(currentIndex - 1);
			}

			if (currentIndex < objectPtr->GetParamsSetsCount() - 1){
				nextFlags = objectPtr->GetIndexOperationFlags(currentIndex + 2);
			}
		}
	}

	AddButton->setEnabled((flags & iprm::IParamsManager::MF_NO_INSERT) == 0);
	RemoveButton->setEnabled((flags & iprm::IParamsManager::MF_NO_DELETE) == 0);
	UpButton->setEnabled(
				((prevFlags & iprm::IParamsManager::MF_NO_DELETE) == 0) &&
				((flags & iprm::IParamsManager::MF_NO_INSERT) == 0));
	DownButton->setEnabled(
				((flags & iprm::IParamsManager::MF_NO_DELETE) == 0) &&
				((nextFlags & iprm::IParamsManager::MF_NO_INSERT) == 0));
}


void CParamsManagerGuiComp::UpdateTree()
{
	UpdateBlocker updateBlocker(this);

	ParamsTree->clear();

	int selectedIndex = -1;

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int setsCount = objectPtr->GetParamsSetsCount();

		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
		if (selectionPtr != NULL){
			selectedIndex = selectionPtr->GetSelectedOptionIndex();
		}

		for (int paramSetIndex = 0; paramSetIndex < setsCount; ++paramSetIndex){
			int flags = objectPtr->GetIndexOperationFlags(paramSetIndex);

			Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
			if ((flags & iprm::IParamsManager::MF_NAME_FIXED) == 0){
				itemFlags |= Qt::ItemIsEditable;
			}

			const QString& name = objectPtr->GetParamsSetName(paramSetIndex);
			QTreeWidgetItem* paramsSetItemPtr = new QTreeWidgetItem();
			paramsSetItemPtr->setText(0, name);
			paramsSetItemPtr->setData(0, Qt::UserRole, paramSetIndex);
			paramsSetItemPtr->setFlags(itemFlags);
			ParamsTree->addTopLevelItem(paramsSetItemPtr);

			paramsSetItemPtr->setSelected(paramSetIndex == selectedIndex);	
		}
	}

	UpdateParamsView(selectedIndex);
}


void CParamsManagerGuiComp::UpdateParamsView(int selectedIndex)
{
	imod::IModel* modelPtr = NULL;

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && (selectedIndex >= 0)){
		I_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if (m_paramsObserverCompPtr.IsValid()){
			modelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetParamsSet(selectedIndex));
		}
	}

	if (modelPtr != m_lastConnectedModelPtr){
		EnsureParamsGuiDetached();

		bool paramsFrameVisible = false;

		if (modelPtr != NULL){
			I_ASSERT(!modelPtr->IsAttached(m_paramsObserverCompPtr.GetPtr()));
				
			if (modelPtr->AttachObserver(m_paramsObserverCompPtr.GetPtr())){
				m_lastConnectedModelPtr = modelPtr;

				paramsFrameVisible = true;
			}
		}

		ParamsFrame->setVisible(paramsFrameVisible);
	}

	RemoveButton->setEnabled(selectedIndex >= 0);

	UpdateActions();
}


int CParamsManagerGuiComp::GetSelectedIndex() const
{
	I_ASSERT(IsGuiCreated());

	int retVal = -1;
	QList<QTreeWidgetItem*> items = ParamsTree->selectedItems();
	if (!items.isEmpty()){
		QTreeWidgetItem* itemPtr = items.first();

		if (itemPtr != NULL){
			retVal = itemPtr->data(0, Qt::UserRole).toInt();
		}
	}

	return retVal;
}


void CParamsManagerGuiComp::EnsureParamsGuiDetached()
{
	if (m_paramsObserverCompPtr.IsValid() && (m_lastConnectedModelPtr != NULL)){
		if (m_paramsObserverCompPtr->IsModelAttached(m_lastConnectedModelPtr)){
			m_lastConnectedModelPtr->DetachObserver(m_paramsObserverCompPtr.GetPtr());
		}
	}

	m_lastConnectedModelPtr = NULL;
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CParamsManagerGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	bool areButtonsNeeded = false;

	if (*m_allowAddRemoveAttrPtr){
		iprm::IParamsManager* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			int flags = objectPtr->GetIndexOperationFlags();

			areButtonsNeeded = ((flags & iprm::IParamsManager::MF_COUNT_FIXED) == 0);
		}
	}

	ButtonsFrame->setVisible(areButtonsNeeded);
	ParamsTree->setItemDelegate(new iqtgui::CItemDelegate());
}


void CParamsManagerGuiComp::OnGuiModelDetached()
{
	EnsureParamsGuiDetached();

	BaseClass::OnGuiModelDetached();
}


void CParamsManagerGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	UpdateTree();
}


// reimplemented (iqtgui::CComponentBase)

void CParamsManagerGuiComp::OnGuiCreated()
{
	ParamsFrame->setVisible(false);

	BaseClass::OnGuiCreated();

	if (m_paramsGuiCompPtr.IsValid()){
		m_paramsGuiCompPtr->CreateGui(ParamsFrame);
	}

	ButtonsFrame->setVisible(*m_allowAddRemoveAttrPtr);
}


void CParamsManagerGuiComp::OnGuiDestroyed()
{
	if (m_paramsGuiCompPtr.IsValid()){
		m_paramsGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtprm


