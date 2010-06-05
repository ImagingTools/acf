#include "iqtprm/CParamsManagerGuiComp.h"


#include "istd/TChangeNotifier.h"
#include "istd/CChangeDelegator.h"

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


// reimplemented (imod::IModelEditor)

void CParamsManagerGuiComp::UpdateModel() const
{
}


void CParamsManagerGuiComp::UpdateEditor(int updateFlags)
{
	// if the set was removed, the model was already detached from all observers,
	// so we must reset our temporary model pointer:
	if ((updateFlags & iprm::IParamsManager::CF_SET_REMOVED) != 0 && (updateFlags & istd::CChangeDelegator::CF_DELEGATED) != 0){
		m_lastConnectedModelPtr = NULL;
	}
	
	if (IsGuiCreated()){
		UpdateBlocker updateBlocker(this);

		UpdateTree();
	}
}


// protected slots

void CParamsManagerGuiComp::on_AddButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		I_ASSERT(selectedIndex < objectPtr->GetSetsCount());

		objectPtr->InsertSet(selectedIndex);
	}
}


void CParamsManagerGuiComp::on_RemoveButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		I_ASSERT(selectedIndex < objectPtr->GetSetsCount());

		if (selectedIndex >= 0){
			objectPtr->RemoveSet(selectedIndex);
		}
	}
}


void CParamsManagerGuiComp::on_ParamsTree_itemSelectionChanged()
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	UpdateActions();

	int selectedIndex = GetSelectedIndex();

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr == NULL){
		return;
	}

	const iprm::ISelectionConstraints* constraintsPtr = selectionPtr->GetConstraints();

	if (		(constraintsPtr != NULL) &&
				(selectedIndex < constraintsPtr->GetOptionsCount()) &&
				(selectedIndex != selectionPtr->GetSelectedOptionIndex())){
		selectionPtr->SetSelectedOptionIndex(selectedIndex);

		return;
	}

	if (selectedIndex < 0){
		UpdateParamsView(-1);
	}
}


void CParamsManagerGuiComp::on_ParamsTree_itemChanged(QTreeWidgetItem* item, int column)
{
	if (IsUpdateBlocked()){
		return;
	}

	if (column != 0){
		return;
	}

	UpdateBlocker updateBlocker(this);

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int setIndex = item->data(0, Qt::UserRole).toInt();

		objectPtr->SetSetName(setIndex, iqt::GetCString(item->text(0)));
	}
}


// protected methods

void CParamsManagerGuiComp::UpdateActions()
{
	I_ASSERT(IsGuiCreated());

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int flags = objectPtr->GetManagerFlags();

		AddButton->setEnabled((flags & iprm::IParamsManager::MF_NO_INSERT) == 0);
		RemoveButton->setEnabled((flags & iprm::IParamsManager::MF_NO_DELETE) == 0);
	}
}


void CParamsManagerGuiComp::UpdateTree()
{
	if (!IsGuiCreated()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	ParamsTree->clear();

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int flags = objectPtr->GetManagerFlags();

		Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		if ((flags & iprm::IParamsManager::MF_NAME_FIXED) == 0){
			itemFlags |= Qt::ItemIsEditable;
		}

		int setsCount = objectPtr->GetSetsCount();

		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
		int selectedIndex = -1;
		if (selectionPtr != NULL){
			selectedIndex = selectionPtr->GetSelectedOptionIndex();
		}

		for (int paramSetIndex = 0; paramSetIndex < setsCount; ++paramSetIndex){
			const istd::CString& name = objectPtr->GetSetName(paramSetIndex);
			QTreeWidgetItem* paramsSetItemPtr = new QTreeWidgetItem();
			paramsSetItemPtr->setText(0, iqt::GetQString(name));
			paramsSetItemPtr->setData(0, Qt::UserRole, paramSetIndex);
			paramsSetItemPtr->setFlags(itemFlags);
			ParamsTree->addTopLevelItem(paramsSetItemPtr);

			paramsSetItemPtr->setSelected(paramSetIndex == selectedIndex);	
		}

		UpdateParamsView(selectedIndex);
	}
}


void CParamsManagerGuiComp::UpdateParamsView(int selectedIndex)
{
	EnsureParamsGuiDetached();

	bool paramsFrameVisible = false;

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && (selectedIndex >= 0)){
		I_ASSERT(selectedIndex < objectPtr->GetSetsCount());

		if (m_paramsObserverCompPtr.IsValid()){
			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetParamsSet(selectedIndex));

			if (modelPtr != NULL){
				I_ASSERT(!modelPtr->IsAttached(m_paramsObserverCompPtr.GetPtr()));
					
				if (modelPtr->AttachObserver(m_paramsObserverCompPtr.GetPtr())){
					m_lastConnectedModelPtr = modelPtr;

					paramsFrameVisible = true;
				}
			}
		}
	}

	ParamsFrame->setVisible(paramsFrameVisible);

	RemoveButton->setEnabled(selectedIndex >= 0);
}


int CParamsManagerGuiComp::GetSelectedIndex() const
{
	I_ASSERT(IsGuiCreated());

	int retVal = -1;
	QList<QTreeWidgetItem*> items = ParamsTree->selectedItems();
	if (!items.empty()){
		QTreeWidgetItem* itemPtr = items.first();

		if (itemPtr != NULL){
			retVal = itemPtr->data(0, Qt::UserRole).toInt();
		}
	}

	return retVal;
}


void CParamsManagerGuiComp::EnsureParamsGuiDetached()
{
	if ((m_lastConnectedModelPtr != NULL) && m_paramsObserverCompPtr.IsValid()){
		if (m_lastConnectedModelPtr->IsAttached(m_paramsObserverCompPtr.GetPtr())){
			m_lastConnectedModelPtr->DetachObserver(m_paramsObserverCompPtr.GetPtr());
		}
	}

	m_lastConnectedModelPtr = NULL;
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CParamsManagerGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int flags = objectPtr->GetManagerFlags();
		if ((flags & iprm::IParamsManager::MF_COUNT_FIXED) != 0){
			AddButton->setVisible(false);
			RemoveButton->setVisible(false);
		}
	}

	if (m_paramsGuiCompPtr.IsValid()){
		m_paramsGuiCompPtr->CreateGui(ParamsFrame);
	}

	ParamsTree->setItemDelegate(new iqtgui::CItemDelegate());
}


void CParamsManagerGuiComp::OnGuiModelDetached()
{
	if (m_paramsGuiCompPtr.IsValid()){
		m_paramsGuiCompPtr->DestroyGui();
	}

	EnsureParamsGuiDetached();

	BaseClass::OnGuiModelDetached();
}


} // namespace iqtprm


