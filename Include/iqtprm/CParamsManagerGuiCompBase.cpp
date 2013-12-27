#include "iqtprm/CParamsManagerGuiCompBase.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CChangeDelegator.h"
#include "iprm/IParamsSet.h"
#include "iprm/IOptionsManager.h"
#include "iqtgui/CItemDelegate.h"
#include "iqtgui/CWidgetUpdateBlocker.h"
#include "iview/IShapeView.h"
#include "iqt2d/IViewProvider.h"


namespace iqtprm
{


// public methods

CParamsManagerGuiCompBase::CParamsManagerGuiCompBase()
	:m_lastConnectedModelPtr(NULL)
{
	SetUpdateFilter(~istd::IChangeable::CF_DELEGATED);
}


// reimplemented (iqt2d::IViewExtender)

void CParamsManagerGuiCompBase::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(providerPtr != NULL);

	m_connectedSceneFlags[providerPtr] = flags;

	iqt2d::IViewExtender* extenderPtr = GetCurrentViewExtenderPtr();
	if (extenderPtr != NULL){
		extenderPtr->AddItemsToScene(providerPtr, flags);
	}
}


void CParamsManagerGuiCompBase::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

	iqt2d::IViewExtender* extenderPtr = GetCurrentViewExtenderPtr();
	if (extenderPtr != NULL){
		extenderPtr->RemoveItemsFromScene(providerPtr);
	}

	m_connectedSceneFlags.remove(providerPtr);
}


// protected slots

void CParamsManagerGuiCompBase::on_AddButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		objectPtr->InsertParamsSet(-1, selectedIndex);

		// auto select newly created element
		if (objectPtr->GetParamsSetsCount() == 1){
			objectPtr->SetSelectedOptionIndex(0);
		}
		else {
			objectPtr->SetSelectedOptionIndex(selectedIndex);
		}
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
			// successfully removed
			if (objectPtr->RemoveParamsSet(selectedIndex)){
				// auto select element next to removed
				int count = objectPtr->GetParamsSetsCount();
				if (count > 0){
					if (selectedIndex < count){
						objectPtr->SetSelectedOptionIndex(selectedIndex);
					}
					else{
						objectPtr->SetSelectedOptionIndex(count-1);
					}
				}
			} else {	// not removed by any reason
				UpdateParamsView(selectedIndex);
			}
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

	const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();

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

		if (objectPtr->SetParamsSetName(setIndex, item->text(0)) == false){
			// if not set: restore old name
			item->setText(0, objectPtr->GetParamsSetName(setIndex));
		}

		iprm::IOptionsManager* optionsManagerPtr = dynamic_cast<iprm::IOptionsManager*>(objectPtr);
		if ((optionsManagerPtr != NULL) && (item->flags() & Qt::ItemIsUserCheckable)){
			optionsManagerPtr->SetOptionEnabled(setIndex, item->checkState(0));
		}
	}
}


void CParamsManagerGuiCompBase::on_ParamsComboBox_currentIndexChanged(int /*index*/)
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	int selectedIndex = GetSelectedIndex();

	ParamsComboBox->setEditable(selectedIndex >= 0);

	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr == NULL){
		return;
	}

	const iprm::IOptionsList* constraintsPtr = selectionPtr->GetSelectionConstraints();

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


void CParamsManagerGuiCompBase::on_ParamsComboBox_editTextChanged(const QString& text)
{
	if (IsUpdateBlocked()){
		return;
	}

	int selectedIndex = ParamsComboBox->currentIndex();
	if (selectedIndex >= 0){
		UpdateBlocker updateBlocker(this);

		iprm::IParamsManager* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			int setIndex = ParamsComboBox->itemData(selectedIndex, Qt::UserRole).toInt();

			if (objectPtr->SetParamsSetName(setIndex, text)){
				ParamsComboBox->setItemText(selectedIndex, text);
			}
		}
	}
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


iqt2d::IViewExtender* CParamsManagerGuiCompBase::GetCurrentViewExtenderPtr() const
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return NULL;
	}

	int selectedParamsSetIndex = objectPtr->GetSelectedOptionIndex();
	if (selectedParamsSetIndex < 0){
		return NULL;
	}

	iprm::IParamsSet* paramSetPtr = objectPtr->GetParamsSet(selectedParamsSetIndex);
	if (paramSetPtr == NULL){
		return NULL;
	}

	return dynamic_cast<iqt2d::IViewExtender*>(GetEditorGuiPtr(paramSetPtr));
}


// protected methods

void CParamsManagerGuiCompBase::UpdateActions()
{
	Q_ASSERT(IsGuiCreated());

	int flags = 0;
	int prevFlags = 0;
	int nextFlags = 0;

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int currentIndex = objectPtr->GetSelectedOptionIndex();

		flags = objectPtr->GetIndexOperationFlags(currentIndex);

		if (currentIndex >= 0){
			if (currentIndex >= 1){
				prevFlags = objectPtr->GetIndexOperationFlags(currentIndex - 1);
			}

			if (currentIndex < objectPtr->GetParamsSetsCount() - 1){
				nextFlags = objectPtr->GetIndexOperationFlags(currentIndex + 1);
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
	ParamsTree->clear();

	int selectedIndex = -1;	

	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int setsCount = objectPtr->GetParamsSetsCount();

		const iprm::IOptionsList* paramsListPtr = NULL;
		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
		if (selectionPtr != NULL){
			selectedIndex = selectionPtr->GetSelectedOptionIndex();
		
			paramsListPtr = selectionPtr->GetSelectionConstraints(); 
		}

		for (int paramSetIndex = 0; paramSetIndex < setsCount; ++paramSetIndex){
			int flags = objectPtr->GetIndexOperationFlags(paramSetIndex);

			Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
			if ((flags & iprm::IParamsManager::MF_SUPPORT_RENAME) != 0){
				itemFlags |= Qt::ItemIsEditable;
			}

			if (*m_supportEnablingAttrPtr && (paramsListPtr != NULL) && (flags & iprm::IOptionsManager::OOF_SUPPORT_ENABLING) != 0){
				itemFlags |= Qt::ItemIsUserCheckable;
			}

			QString name = objectPtr->GetParamsSetName(paramSetIndex);
			QTreeWidgetItem* paramsSetItemPtr = new QTreeWidgetItem();
			paramsSetItemPtr->setFlags(itemFlags);

			paramsSetItemPtr->setText(0, name);
			paramsSetItemPtr->setData(0, Qt::UserRole, paramSetIndex);
			if (*m_supportEnablingAttrPtr && (paramsListPtr != NULL) && (flags & iprm::IOptionsManager::OOF_SUPPORT_ENABLING) != 0){
				paramsSetItemPtr->setCheckState(0, paramsListPtr->IsOptionEnabled(paramSetIndex) ? Qt::Checked : Qt::Unchecked);
			}
			
			iprm::IParamsSet* paramsSetPtr = objectPtr->GetParamsSet(paramSetIndex);
			if (paramsSetPtr != NULL){
				QByteArray id = paramsSetPtr->GetFactoryId();
				if (m_factoryIconIndexMap.contains(id)){
					int iconIndex = m_factoryIconIndexMap[id];						
					QIcon icon = m_stateIconsMap.value(iconIndex);
					paramsSetItemPtr->setIcon(0, icon);						
				}
			}
			

			ParamsTree->addTopLevelItem(paramsSetItemPtr);

			paramsSetItemPtr->setSelected(paramSetIndex == selectedIndex);	
		}
	}

	UpdateParamsView(selectedIndex);
}


void CParamsManagerGuiCompBase::UpdateComboBox()
{
	ParamsComboBox->clear();

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
			if ((flags & iprm::IParamsManager::MF_SUPPORT_RENAME) != 0){
				itemFlags |= Qt::ItemIsEditable;
			}

			QString name = objectPtr->GetParamsSetName(paramSetIndex);
			QIcon icon;
			iprm::IParamsSet* paramsSetPtr = objectPtr->GetParamsSet(paramSetIndex);
			if (paramsSetPtr != NULL){
				QByteArray id = paramsSetPtr->GetFactoryId();
				if (m_factoryIconIndexMap.contains(id)){
					int iconIndex = m_factoryIconIndexMap[id];						
					icon = m_stateIconsMap.value(iconIndex);
				}
			}

			ParamsComboBox->addItem(icon, name, paramSetIndex);
		}
	}

	ParamsComboBox->setCurrentIndex(selectedIndex);
	ParamsComboBox->setEditable(selectedIndex >= 0);
	ParamsComboBox->setEnabled(ParamsComboBox->count() > 0);

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

				bool setReadOnly = ((objectPtr->GetIndexOperationFlags(selectedIndex) & iprm::IParamsManager::MF_SUPPORT_EDIT) == 0);
				imod::IModelEditor* modelEditorPtr = CompCastPtr<imod::IModelEditor>(paramsSetObserverPtr);
				if (modelEditorPtr != NULL){
					modelEditorPtr->SetReadOnly(setReadOnly);
				}
			}
		}

		ParamsFrame->setVisible(paramsFrameVisible);

		ParamsFrame->setUpdatesEnabled(true);
	}

	RemoveButton->setEnabled(selectedIndex >= 0);

	if (modelPtr == NULL){
		ParamsFrame->hide();
	}

	UpdateActions();
}


int CParamsManagerGuiCompBase::GetSelectedIndex() const
{
	Q_ASSERT(IsGuiCreated());

	int retVal = -1;

	if (*m_comboBoxViewAttrPtr){
		int index = ParamsComboBox->currentIndex();
		if (index < 0){
			return -1;
		}

		retVal = ParamsComboBox->itemData(index).toInt();

		return retVal;
	}

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

		const iprm::IOptionsList* typeConstraintsPtr = objectPtr->GetParamsTypeConstraints();
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
	ParamsTree->setItemDelegate(new iqtgui::CItemDelegate(20, this));
}


void CParamsManagerGuiCompBase::OnGuiModelDetached()
{
	EnsureParamsGuiDetached();

	BaseClass::OnGuiModelDetached();
}


void CParamsManagerGuiCompBase::UpdateGui(int updateFlags)
{
	Q_ASSERT(IsGuiCreated());

	bool forceUpdate =
				((updateFlags & iprm::IOptionsList::CF_OPTIONS_CHANGED) != 0) ||
				((updateFlags & iprm::IParamsManager::CF_SET_INSERTED) != 0) ||
				((updateFlags & iprm::IParamsManager::CF_SET_REMOVED) != 0);

	if (!forceUpdate && ((updateFlags & CF_INIT_EDITOR) == 0) && (updateFlags & istd::IChangeable::CF_DELEGATED)){
		return;
	}

	if (*m_comboBoxViewAttrPtr){
		UpdateComboBox();
	}
	else{
		UpdateTree();
	}
}


// reimplemented (iqtgui::CComponentBase)

void CParamsManagerGuiCompBase::OnGuiCreated()
{
	ParamsFrame->setVisible(false);

	BaseClass::OnGuiCreated();

	if (*m_comboBoxViewAttrPtr){
		ParamsTree->hide();
		horizontalSpacer->changeSize(0,0);
		AddButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
		RemoveButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	} else {
		ParamsComboBox->hide();
	}

	// initialize state icons map:
	if (m_iconProviderCompPtr.IsValid()){
		for (int fileState = 0; fileState < m_iconProviderCompPtr->GetIconCount(); fileState++){
			QPixmap pixmap = m_iconProviderCompPtr->GetIcon(fileState).pixmap(QSize(64, 64),QIcon::Normal, QIcon::On);
			m_stateIconsMap[fileState] = QIcon(pixmap);
		}
	}

	ButtonsFrame->setVisible(*m_allowAddRemoveAttrPtr);
}


// reimplemented (imod::CSingleModelObserverBase)

void CParamsManagerGuiCompBase::BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((updateFlags & iprm::ISelectionParam::CF_SELECTION_CHANGED) != 0){
		DetachCurrentExtender();
	}

	BaseClass::BeforeUpdate(modelPtr, updateFlags, updateParamsPtr);
}


void CParamsManagerGuiCompBase::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);

	if ((updateFlags & iprm::ISelectionParam::CF_SELECTION_CHANGED) != 0){
		AttachCurrentExtender();
	}
}


// private methods

void CParamsManagerGuiCompBase::AttachCurrentExtender()
{
	DetachCurrentExtender();

	iqt2d::IViewExtender* extenderPtr = GetCurrentViewExtenderPtr();
	if (extenderPtr != NULL){
		for (		ConnectedSceneFlags::const_iterator sceneIter = m_connectedSceneFlags.begin();
			sceneIter != m_connectedSceneFlags.end();
			++sceneIter){
				iqt2d::IViewProvider* providerPtr = sceneIter.key();
				Q_ASSERT(providerPtr != NULL);

				extenderPtr->AddItemsToScene(providerPtr, sceneIter.value());
		}
	}
}


void CParamsManagerGuiCompBase::DetachCurrentExtender()
{
	iqt2d::IViewExtender* extenderPtr = GetCurrentViewExtenderPtr();
	if (extenderPtr != NULL){
		for (		ConnectedSceneFlags::const_iterator sceneIter = m_connectedSceneFlags.begin();
					sceneIter != m_connectedSceneFlags.end();
					++sceneIter){
			iqt2d::IViewProvider* providerPtr = sceneIter.key();
			Q_ASSERT(providerPtr != NULL);

			extenderPtr->RemoveItemsFromScene(providerPtr);
		}
	}
}


} // namespace iqtprm


