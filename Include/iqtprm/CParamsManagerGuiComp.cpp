#include "iqtprm/CParamsManagerGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"

#include "iqt/CSignalBlocker.h"


namespace iqtprm
{


CParamsManagerGuiComp::CParamsManagerGuiComp()
:	m_lastConnectedModelPtr(NULL),
	m_managerObserver(this)
{
}


// reimplemented (imod::IModelEditor)

void CParamsManagerGuiComp::UpdateModel() const
{
}


void CParamsManagerGuiComp::UpdateEditor()
{
	if (IsGuiCreated() && !IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		const iprm::ISelectionParam* selectionPtr = GetObjectPtr();
		if (selectionPtr != NULL){
			int selected = selectionPtr->GetSelectedOptionIndex();

			iqt::CSignalBlocker signalBlocker(ParamsTree);

			if (selected < ParamsTree->topLevelItemCount()){
				ParamsTree->clearSelection();

				QTreeWidgetItem* selectedItemPtr = ParamsTree->topLevelItem(selected);
				if ((selectedItemPtr != NULL) && !selectedItemPtr->isSelected()){
					selectedItemPtr->setSelected(true);

					UpdateParamsView(selected);
				}
			}
		}
	}
}


// protected slots

void CParamsManagerGuiComp::on_AddButton_clicked()
{
	istd::TChangeNotifier<iprm::IParamsManager> objectPtr(m_paramsManagerCompPtr.GetPtr(), CF_PARAMS_ADD);
	if (objectPtr.IsValid()){
		int selectedIndex = GetSelectedIndex();
		I_ASSERT(selectedIndex < objectPtr->GetSetsCount());

		objectPtr->InsertSet(selectedIndex);
	}
}


void CParamsManagerGuiComp::on_RemoveButton_clicked()
{
	istd::TChangeNotifier<iprm::IParamsManager> objectPtr(m_paramsManagerCompPtr.GetPtr(), CF_PARAMS_REMOVE);
	if (objectPtr.IsValid()){
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

	UpdateParamsView(selectedIndex);

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (		(selectedIndex >= 0) &&
				(selectionPtr != NULL) &&
				(selectedIndex < selectionPtr->GetOptionsCount()) &&
				(selectedIndex != selectionPtr->GetSelectedOptionIndex())){
		selectionPtr->SetSelectedOptionIndex(selectedIndex);
	}
}


// protected methods

void CParamsManagerGuiComp::UpdateActions()
{
	I_ASSERT(IsGuiCreated());

	if (m_paramsManagerCompPtr.IsValid()){
		int flags = m_paramsManagerCompPtr->GetManagerFlags();

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

	if (m_paramsManagerCompPtr.IsValid()){
		int flags = m_paramsManagerCompPtr->GetManagerFlags();

		Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		if ((flags & iprm::IParamsManager::MF_NAME_FIXED) == 0){
			itemFlags |= Qt::ItemIsEditable;
		}

		int setsCount = m_paramsManagerCompPtr->GetSetsCount();

		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
		int selectedIndex = -1;
		if (selectionPtr != NULL){
			selectedIndex = selectionPtr->GetSelectedOptionIndex();
		}

		for (int i = 0; i < setsCount; ++i){
			const istd::CString& name = m_paramsManagerCompPtr->GetSetName(i);
			QTreeWidgetItem* paramsSetItemPtr = new QTreeWidgetItem();
			paramsSetItemPtr->setText(0, iqt::GetQString(name));
			paramsSetItemPtr->setData(0, Qt::UserRole, i);
			paramsSetItemPtr->setFlags(itemFlags);
			ParamsTree->addTopLevelItem(paramsSetItemPtr);

			paramsSetItemPtr->setSelected(i == selectedIndex);
		}

		UpdateParamsView(selectedIndex);
	}
}


void CParamsManagerGuiComp::UpdateParamsView(int selectedIndex)
{
	iqt::CSignalBlocker blocker(ParamsFrame);

	EnsureParamsGuiDetached();

	if (m_paramsManagerCompPtr.IsValid() && (selectedIndex >= 0)){
		I_ASSERT(selectedIndex < m_paramsManagerCompPtr->GetSetsCount());

		if (m_paramsObserverCompPtr.IsValid()){
			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(m_paramsManagerCompPtr->GetParamsSet(selectedIndex));
			if ((modelPtr != NULL) && modelPtr->AttachObserver(m_paramsObserverCompPtr.GetPtr())){
				m_lastConnectedModelPtr = modelPtr;
			}
		}
	}
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
		m_lastConnectedModelPtr->DetachObserver(m_paramsObserverCompPtr.GetPtr());
	}

	m_lastConnectedModelPtr = NULL;
}


// reimplemented (iqt::TGuiObserverWrap)

void CParamsManagerGuiComp::OnGuiModelShown()
{
	UpdateTree();
}


void CParamsManagerGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	if (m_paramsManagerCompPtr.IsValid()){
		int flags = m_paramsManagerCompPtr->GetManagerFlags();
		if ((flags & iprm::IParamsManager::MF_COUNT_FIXED) != 0){
			AddButton->setVisible(false);
			RemoveButton->setVisible(false);
		}
	}

	if (m_paramsGuiCompPtr.IsValid()){
		m_paramsGuiCompPtr->CreateGui(ParamsFrame);
		ParamsFrame->setVisible(true);
	}
	else{
		ParamsFrame->setVisible(false);
	}
}


void CParamsManagerGuiComp::OnGuiModelDetached()
{
	if (m_paramsGuiCompPtr.IsValid()){
		m_paramsGuiCompPtr->DestroyGui();
	}

	EnsureParamsGuiDetached();

	BaseClass::OnGuiModelAttached();
}


// reimplemented (icomp::IComponent)

void CParamsManagerGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_paramsManagerModelCompPtr.IsValid()){
		m_paramsManagerModelCompPtr->AttachObserver(&m_managerObserver);
	}
}


void CParamsManagerGuiComp::OnComponentDestroyed()
{
	if (m_paramsManagerModelCompPtr.IsValid() && m_paramsManagerModelCompPtr->IsAttached(&m_managerObserver)){
		m_paramsManagerModelCompPtr->DetachObserver(&m_managerObserver);
	}

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class ManagerObserver

CParamsManagerGuiComp::ManagerObserver::ManagerObserver(CParamsManagerGuiComp* parentPtr)
:	m_parent(*parentPtr)
{
}


// protected methods of embedded class ManagerObserver

void CParamsManagerGuiComp::ManagerObserver::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.UpdateTree();
}


} // namespace iqtprm


