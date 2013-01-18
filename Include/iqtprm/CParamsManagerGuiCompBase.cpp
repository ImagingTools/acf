#include "iqtprm/CParamsManagerGuiCompBase.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CChangeDelegator.h"

#include "iprm/IParamsSet.h"
#include "iprm/ISelectionConstraints.h"

#include "iqtgui/CItemDelegate.h"
#include "iqtgui/CWidgetUpdateBlocker.h"


namespace iqtprm
{


// public methods

CParamsManagerGuiCompBase::CParamsManagerGuiCompBase()
	:m_lastConnectedModelPtr(NULL)
{
}


// protected slots

void CParamsManagerGuiCompBase::on_AddButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		objectPtr->InsertParamsSet(-1, selectedIndex);
	}
}


void CParamsManagerGuiCompBase::on_RemoveButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		EnsureParamsGuiDetached();

		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if (selectedIndex >= 0){
			objectPtr->RemoveParamsSet(selectedIndex);
		}
	}
}


void CParamsManagerGuiCompBase::on_UpButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if (selectedIndex < 1){
			return;
		}

		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SwapParamsSet(selectedIndex, selectedIndex - 1);
		objectPtr->SetSelectedOptionIndex(selectedIndex - 1);
	}
}


void CParamsManagerGuiCompBase::on_DownButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if ((selectedIndex < 0) || (selectedIndex >= objectPtr->GetParamsSetsCount() - 1)){
			return;
		}

		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SwapParamsSet(selectedIndex, selectedIndex + 1);
		objectPtr->SetSelectedOptionIndex(selectedIndex + 1);
	}
}


void CParamsManagerGuiCompBase::on_ParamsTree_itemSelectionChanged()
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


void CParamsManagerGuiCompBase::on_ParamsTree_itemChanged(QTreeWidgetItem* item, int column)
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

void CParamsManagerGuiCompBase::UpdateActions()
{
	Q_ASSERT(IsGuiCreated());

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

	AddButton->setEnabled((flags & iprm::IParamsManager::MF_SUPPORT_INSERT) != 0);
	RemoveButton->setEnabled((flags & iprm::IParamsManager::MF_SUPPORT_DELETE) != 0);
	UpButton->setEnabled(
				((prevFlags & iprm::IParamsManager::MF_SUPPORT_SWAP) != 0) &&
				((flags & iprm::IParamsManager::MF_SUPPORT_SWAP) != 0));
	DownButton->setEnabled(
				((flags & iprm::IParamsManager::MF_SUPPORT_SWAP) != 0) &&
				((nextFlags & iprm::IParamsManager::MF_SUPPORT_SWAP) != 0));
}


void CParamsManagerGuiCompBase::UpdateTree()
{
	UpdateBlocker updateBlocker(this);

	ParamsTree->clear();

	int selectedIndex = -1;

	int iconsCount = 0;
	if (m_iconProviderCompPtr.IsValid()){
		iconsCount = m_iconProviderCompPtr->GetIconCount();
	}

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
			if ((flags & iprm::IParamsManager::MF_SUPPORT_RENAME) != 0){
				itemFlags |= Qt::ItemIsEditable;
			}

			QString name = objectPtr->GetParamsSetName(paramSetIndex);
			QTreeWidgetItem* paramsSetItemPtr = new QTreeWidgetItem();
			paramsSetItemPtr->setText(0, name);
			paramsSetItemPtr->setData(0, Qt::UserRole, paramSetIndex);
			paramsSetItemPtr->setFlags(itemFlags);

			if (iconsCount > 0){
				iprm::IParamsSet* paramsSetPtr = objectPtr->GetParamsSet(paramSetIndex);
				if (paramsSetPtr != NULL){
					QByteArray id = paramsSetPtr->GetFactoryId();
					if (m_factoryIconIndexMap.contains(id)){
						int iconIndex = m_factoryIconIndexMap[id];
						if (iconIndex < iconsCount){
							QIcon icon = m_iconProviderCompPtr->GetIcon(iconIndex);
							paramsSetItemPtr->setIcon(0, icon);
						}
					}
				}
			}

			ParamsTree->addTopLevelItem(paramsSetItemPtr);

			paramsSetItemPtr->setSelected(paramSetIndex == selectedIndex);	
		}
	}

	UpdateParamsView(selectedIndex);
}


void CParamsManagerGuiCompBase::UpdateParamsView(int selectedIndex)
{
	imod::IModel* modelPtr = NULL;
	imod::IObserver* paramsSetObserverPtr = NULL;

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && (selectedIndex >= 0)){
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		paramsSetObserverPtr = GetObserverPtr(objectPtr->GetParamsSet(selectedIndex));

		if (paramsSetObserverPtr != NULL){
			modelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetParamsSet(selectedIndex));
		}
	}

	if (modelPtr != m_lastConnectedModelPtr){
		iqtgui::CWidgetUpdateBlocker blocker(ParamsFrame);

		EnsureParamsGuiDetached();

		bool paramsFrameVisible = false;

		if (modelPtr != NULL){
			Q_ASSERT(!modelPtr->IsAttached(paramsSetObserverPtr));
				
			if (modelPtr->AttachObserver(paramsSetObserverPtr)){
				m_lastConnectedModelPtr = modelPtr;
				m_lastObserverPtr = paramsSetObserverPtr;

				paramsFrameVisible = true;
			}
		}

		ParamsFrame->setVisible(paramsFrameVisible);

		ParamsFrame->setUpdatesEnabled(true);
	}

	RemoveButton->setEnabled(selectedIndex >= 0);

	UpdateActions();
}


int CParamsManagerGuiCompBase::GetSelectedIndex() const
{
	Q_ASSERT(IsGuiCreated());

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


void CParamsManagerGuiCompBase::EnsureParamsGuiDetached()
{
	if ((m_lastObserverPtr != NULL) && (m_lastConnectedModelPtr != NULL)){
		if (m_lastObserverPtr->IsModelAttached(m_lastConnectedModelPtr)){
			m_lastConnectedModelPtr->DetachObserver(m_lastObserverPtr);
		}
	}

	m_lastConnectedModelPtr = NULL;
	m_lastObserverPtr = NULL;
}


QByteArray CParamsManagerGuiCompBase::GetSelectedParamsSetTypeId() const
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if ((objectPtr != NULL) && objectPtr->GetSelectedOptionIndex() >= 0){
		iprm::IParamsSet* paramsSetPtr = objectPtr->GetParamsSet(objectPtr->GetSelectedOptionIndex());
		if (paramsSetPtr != NULL){
			return paramsSetPtr->GetFactoryId();
		}
	}

	return QByteArray();
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CParamsManagerGuiCompBase::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	bool areAddRemoveButtonsNeeded = false;
	bool areUpDownButtonsNeeded = false;

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int flags = objectPtr->GetIndexOperationFlags();

		if (*m_allowAddRemoveAttrPtr){
			areAddRemoveButtonsNeeded = ((flags & iprm::IParamsManager::MF_COUNT_FIXED) == 0);
		}

		if (*m_allowUpDownAttrPtr){
			areUpDownButtonsNeeded = ((flags & iprm::IParamsManager::MF_SUPPORT_SWAP) != 0);
		}

		m_startVariableMenus.clear();
		m_factoryIconIndexMap.clear();

		int iconsCount = 0;
		if (m_iconProviderCompPtr.IsValid()){
			iconsCount = m_iconProviderCompPtr->GetIconCount();
		}

		const iprm::ISelectionConstraints* typeConstraintsPtr = objectPtr->GetParamsTypeConstraints();
		if (typeConstraintsPtr != NULL){
			int typesCount = typeConstraintsPtr->GetOptionsCount();

			if (typesCount > 1){
				Q_ASSERT(typeConstraintsPtr != NULL);

				for (int i = 0; i < typesCount; ++i){
					// translate
					QString typeName = typeConstraintsPtr->GetOptionName(i);

					QAction* action = m_startVariableMenus.addAction(typeName);

					// store original type index
					action->setData(i);

					// update icon
					if (iconsCount > i){
						QIcon icon = m_iconProviderCompPtr->GetIcon(i);
						action->setIcon(icon);
					}

					m_factoryIconIndexMap[typeConstraintsPtr->GetOptionId(i)] = i;
				}

				AddButton->setMenu(&m_startVariableMenus);
				QObject::connect(&m_startVariableMenus, SIGNAL(triggered(QAction*)), this, SLOT(OnAddMenuOptionClicked(QAction*)));
			}
		}
	}

	AddRemoveButtonsFrame->setVisible(areAddRemoveButtonsNeeded);
	UpDownButtonsFrame->setVisible(areUpDownButtonsNeeded);

	ButtonsFrame->setVisible(areAddRemoveButtonsNeeded && areUpDownButtonsNeeded);
	ParamsTree->setItemDelegate(new iqtgui::CItemDelegate());
}


void CParamsManagerGuiCompBase::OnGuiModelDetached()
{
	EnsureParamsGuiDetached();

	BaseClass::OnGuiModelDetached();
}


void CParamsManagerGuiCompBase::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	UpdateTree();
}


// reimplemented (iqtgui::CComponentBase)

void CParamsManagerGuiCompBase::OnGuiCreated()
{
	ParamsFrame->setVisible(false);

	BaseClass::OnGuiCreated();

	ButtonsFrame->setVisible(*m_allowAddRemoveAttrPtr);
}


void CParamsManagerGuiCompBase::OnAddMenuOptionClicked(QAction* action)
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int typeIndex = action->data().toInt();

		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		objectPtr->InsertParamsSet(typeIndex, selectedIndex);
	}
}


} // namespace iqtprm


