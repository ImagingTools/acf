#include "iqt/CParamsManagerGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iprm/IParamsSet.h"


namespace iqt
{


CParamsManagerGuiComp::CParamsManagerGuiComp()
:	m_lastConnectedModelPtr(NULL)
{
}


// reimplemented (imod::IModelEditor)

void CParamsManagerGuiComp::UpdateModel() const
{
}


void CParamsManagerGuiComp::UpdateEditor()
{
	UpdateTree();
}


// protected slots

void CParamsManagerGuiComp::on_AddButton_clicked()
{
	istd::TChangeNotifier<iprm::IParamsManager> objectPtr(GetObjectPtr(), CF_PARAMS_ADD);
	if (objectPtr.IsValid()){
		int selectedIndex = GetSelectedIndex();
		I_ASSERT(selectedIndex < objectPtr->GetSetsCount());

		objectPtr->InsertSet(selectedIndex);
	}
}


void CParamsManagerGuiComp::on_RemoveButton_clicked()
{
	istd::TChangeNotifier<iprm::IParamsManager> objectPtr(GetObjectPtr(), CF_PARAMS_REMOVE);
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
	UpdateActions();

	if (!m_paramsObserverCompPtr.IsValid()){
		return;
	}

	EnsureParamsGuiDetached();

	int selectedIndex = GetSelectedIndex();
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if ((objectPtr != NULL) && (selectedIndex >= 0)){
		I_ASSERT(selectedIndex < objectPtr->GetSetsCount());

		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetParamsSet(selectedIndex));
		if (modelPtr->AttachObserver(m_paramsObserverCompPtr.GetPtr())){
			m_lastConnectedModelPtr = modelPtr;
		}

		if (		(selectedIndex >= 0) &&
					m_selectionParamCompPtr.IsValid() &&
					(selectedIndex < m_selectionParamCompPtr->GetOptionsCount()) &&
					(selectedIndex != m_selectionParamCompPtr->GetSelectedOptionIndex())){
			m_selectionParamCompPtr->SetSelectedOptionIndex(selectedIndex);
		}
	}
}


// protected methods

void CParamsManagerGuiComp::UpdateActions()
{
	I_ASSERT(IsGuiCreated());

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int flags = objectPtr->GetManagerFlags();

		AddButton->setVisible((flags & iprm::IParamsManager::MF_NO_INSERT) == 0);
		RemoveButton->setVisible((flags & iprm::IParamsManager::MF_NO_DELETE) == 0);
	}
}


void CParamsManagerGuiComp::UpdateTree()
{
	I_ASSERT(IsGuiCreated());

	ParamsTree->clear();

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int flags = objectPtr->GetManagerFlags();

		Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		if ((flags & iprm::IParamsManager::MF_NAME_FIXED) == 0){
			itemFlags |= Qt::ItemIsEditable;
		}

		int setsCount = objectPtr->GetSetsCount();

		int selectedIndex = -1;
		if (m_selectionParamCompPtr.IsValid()){
			selectedIndex = m_selectionParamCompPtr->GetSelectedOptionIndex();
		}

		for (int i = 0; i < setsCount; ++i){
			const istd::CString& name = objectPtr->GetSetName(i);
			QTreeWidgetItem* paramsSetItemPtr = new QTreeWidgetItem();
			paramsSetItemPtr->setText(0, iqt::GetQString(name));
			paramsSetItemPtr->setData(0, Qt::UserRole, i);
			paramsSetItemPtr->setFlags(itemFlags);
			ParamsTree->addTopLevelItem(paramsSetItemPtr);

			if (i == selectedIndex){
				paramsSetItemPtr->setSelected(true);
			}
		}
	}
}


int CParamsManagerGuiComp::GetSelectedIndex() const
{
	I_ASSERT(IsGuiCreated());

	int retVal = -1;
	QTreeWidgetItem* itemPtr = ParamsTree->currentItem();
	if (itemPtr != NULL){
		retVal = itemPtr->data(0, Qt::UserRole).toInt();
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


} // namespace iqt


