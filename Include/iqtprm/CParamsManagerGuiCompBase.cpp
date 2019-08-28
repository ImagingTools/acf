#include <iqtprm/CParamsManagerGuiCompBase.h>


// Qt includes
#include <QtGui/QStandardItemModel>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/IParamsSet.h>
#include <iprm/IOptionsManager.h>
#include <iwidgets/CItemDelegate.h>
#include <iwidgets/CWidgetUpdateBlocker.h>
#include <iview/IShapeView.h>
#include <iqt2d/IViewProvider.h>


namespace iqtprm
{


// public methods

CParamsManagerGuiCompBase::CParamsManagerGuiCompBase()
:	m_lastConnectedModelPtr(NULL),
	m_lastObserverPtr(NULL)
{
	static const istd::IChangeable::ChangeSet changeMask(
				iprm::ISelectionParam::CF_SELECTION_CHANGED,
				iprm::IOptionsList::CF_OPTIONS_CHANGED,
				iprm::IParamsManager::CF_SET_INSERTED,
				iprm::IParamsManager::CF_SET_REMOVED,
				iprm::IParamsManager::CF_SET_NAME_CHANGED,
				CF_INIT_EDITOR);
	SetObservedIds(changeMask);

	QObject::connect(&m_startVariableMenus, SIGNAL(triggered(QAction*)), this, SLOT(OnAddMenuOptionClicked(QAction*)));
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
	InsertNewParamsSet();
}


void CParamsManagerGuiCompBase::on_RemoveButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		EnsureParamsGuiDetached();

		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if (selectedIndex >= 0){
			istd::CChangeNotifier notifier(objectPtr);
			Q_UNUSED(notifier);

			// successfully removed
			if (objectPtr->RemoveParamsSet(selectedIndex)){
				// select element before the removed
				objectPtr->SetSelectedOptionIndex(selectedIndex - 1);
			}
			else{	// not removed by any reason
				UpdateParamsView(selectedIndex);
			}
		}
	}
}


void CParamsManagerGuiCompBase::on_UpButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if (selectedIndex < 1){
			return;
		}

		istd::CChangeNotifier notifier(objectPtr);
		Q_UNUSED(notifier);

		objectPtr->SwapParamsSet(selectedIndex, selectedIndex - 1);
		objectPtr->SetSelectedOptionIndex(selectedIndex - 1);
	}
}


void CParamsManagerGuiCompBase::on_DownButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int selectedIndex = GetSelectedIndex();
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		if ((selectedIndex < 0) || (selectedIndex >= objectPtr->GetParamsSetsCount() - 1)){
			return;
		}

		istd::CChangeNotifier notifier(objectPtr);
		Q_UNUSED(notifier);

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

	iprm::ISelectionParam* selectionPtr = GetObservedObject();
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

	iprm::IParamsManager* objectPtr = GetObservedObject();
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

	ParamsComboBox->setEditable(selectedIndex >= 0 && *m_comboBoxEditableAttrPtr);

	iprm::ISelectionParam* selectionPtr = GetObservedObject();
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

		iprm::IParamsManager* objectPtr = GetObservedObject();
		if (objectPtr != NULL){
			int setIndex = ParamsComboBox->itemData(selectedIndex, Qt::UserRole).toInt();

			if (objectPtr->SetParamsSetName(setIndex, text)){
				ParamsComboBox->setItemText(selectedIndex, text);
			}
		}
	}
}


void CParamsManagerGuiCompBase::on_LoadParamsButton_clicked()
{
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->LoadFromFile(*objectPtr);
	}
}


void CParamsManagerGuiCompBase::on_SaveParamsButton_clicked()
{
	const iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->SaveToFile(*objectPtr);
	}
}


void CParamsManagerGuiCompBase::OnAddMenuOptionClicked(QAction* action)
{
	Q_ASSERT(action != NULL);

	int typeIndex = action->data().toInt();

	InsertNewParamsSet(typeIndex);
}


iqt2d::IViewExtender* CParamsManagerGuiCompBase::GetCurrentViewExtenderPtr() const
{
	iprm::IParamsManager* objectPtr = GetObservedObject();
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

	iqtgui::IGuiObject* parameterEditorPtr = GetEditorGuiPtr(paramSetPtr);

	iqt2d::IViewExtender* viewExtenderPtr = dynamic_cast<iqt2d::IViewExtender*>(parameterEditorPtr);
	if (viewExtenderPtr == NULL){
		viewExtenderPtr = CompCastPtr<iqt2d::IViewExtender>(parameterEditorPtr);
	}

	return viewExtenderPtr;
}


// protected methods

void CParamsManagerGuiCompBase::UpdateActions()
{
	Q_ASSERT(IsGuiCreated());

	int flags = 0;
	int prevFlags = 0;
	int nextFlags = 0;

	iprm::IParamsManager* objectPtr = GetObservedObject();
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

	AddButton->setEnabled((flags & iprm::IParamsManager::MF_COUNT_FIXED) == 0);
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

	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int setsCount = objectPtr->GetParamsSetsCount();

		const iprm::IOptionsList* paramsListPtr = objectPtr->GetSelectionConstraints();
		selectedIndex = objectPtr->GetSelectedOptionIndex();

		for (int paramSetIndex = 0; paramSetIndex < setsCount; ++paramSetIndex){
			int flags = objectPtr->GetIndexOperationFlags(paramSetIndex);

			bool isOptionEnabled = (paramsListPtr == NULL) ? true : paramsListPtr->IsOptionEnabled(paramSetIndex);

			Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
			if ((flags & iprm::IParamsManager::MF_SUPPORT_RENAME) != 0){
				itemFlags |= Qt::ItemIsEditable;
			}

			if (!isOptionEnabled){
				itemFlags &= ~Qt::ItemIsEnabled;
			}

			if (*m_supportEnablingAttrPtr && (paramsListPtr != NULL) && (flags & iprm::IOptionsManager::OOF_SUPPORT_ENABLING) != 0){
				itemFlags |= Qt::ItemIsUserCheckable;
			}

			// for inactive items, no operations are allowed
			if (flags & iprm::IParamsManager::MF_INACTIVE){
				itemFlags = 0;
			}

			QString name = objectPtr->GetParamsSetName(paramSetIndex);
			QTreeWidgetItem* paramsSetItemPtr = new QTreeWidgetItem();
			paramsSetItemPtr->setFlags(itemFlags);

			paramsSetItemPtr->setText(0, name);
			paramsSetItemPtr->setData(0, Qt::UserRole, paramSetIndex);
			if (*m_supportEnablingAttrPtr && (paramsListPtr != NULL) && (flags & iprm::IOptionsManager::OOF_SUPPORT_ENABLING) != 0){
				paramsSetItemPtr->setCheckState(0, isOptionEnabled ? Qt::Checked : Qt::Unchecked);
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

	ParamsTree->setCurrentItem(ParamsTree->topLevelItem(selectedIndex));

	UpdateParamsView(selectedIndex);
}


void CParamsManagerGuiCompBase::UpdateComboBox()
{
	ParamsComboBox->clear();

	int selectedIndex = -1;
	int setsCount = 0;
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		setsCount = objectPtr->GetParamsSetsCount();

		const iprm::IOptionsList* paramsListPtr = objectPtr->GetSelectionConstraints();
		selectedIndex = objectPtr->GetSelectedOptionIndex();

		for (int paramSetIndex = 0; paramSetIndex < setsCount; ++paramSetIndex){
			int flags = objectPtr->GetIndexOperationFlags(paramSetIndex);
			bool isOptionEnabled = (paramsListPtr == NULL) ? true : paramsListPtr->IsOptionEnabled(paramSetIndex);

			Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
			if ((flags & iprm::IParamsManager::MF_SUPPORT_RENAME) != 0){
				itemFlags |= Qt::ItemIsEditable;
			}

			// for inactive items, no operations are allowed
			if (flags & iprm::IParamsManager::MF_INACTIVE){
				itemFlags = 0;
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

			if (		*m_supportEnablingAttrPtr && 
						(flags & iprm::IOptionsManager::OOF_SUPPORT_ENABLING) != 0){
				ParamsComboBox->setItemData(paramSetIndex, isOptionEnabled ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
			}

			ParamsComboBox->addItem(icon, name, paramSetIndex);

			QStandardItemModel* itemModelPtr = qobject_cast<QStandardItemModel*>(ParamsComboBox->model());
			if (itemModelPtr != NULL){
				QStandardItem* newItemPtr = itemModelPtr->item(ParamsComboBox->count() - 1);

				newItemPtr->setFlags(itemFlags);
			}
		}
	}

	ParamsComboBox->setCurrentIndex(selectedIndex);
	ParamsComboBox->setEditable(selectedIndex >= 0 && *m_comboBoxEditableAttrPtr);
	ParamsComboBox->setEnabled(ParamsComboBox->count() > 0);

	UpdateParamsView(selectedIndex);
}


void CParamsManagerGuiCompBase::UpdateParamsView(int selectedIndex)
{
	iwidgets::CWidgetUpdateBlocker blocker(GetQtWidget());

	imod::IModel* modelPtr = NULL;
	imod::IObserver* paramsSetObserverPtr = NULL;

	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL && (selectedIndex >= 0)){
		Q_ASSERT(selectedIndex < objectPtr->GetParamsSetsCount());

		paramsSetObserverPtr = GetObserverPtr(objectPtr->GetParamsSet(selectedIndex));

		if (paramsSetObserverPtr != NULL){
			modelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetParamsSet(selectedIndex));
		}
	}

	bool reattachParams = 
				(modelPtr != m_lastConnectedModelPtr) ||
				(m_lastObserverPtr && !(m_lastObserverPtr->IsModelAttached(modelPtr)));
	if (reattachParams){
		EnsureParamsGuiDetached();

		if (modelPtr != NULL){
			Q_ASSERT(!modelPtr->IsAttached(paramsSetObserverPtr));
				
			if (modelPtr->AttachObserver(paramsSetObserverPtr)){
				m_lastConnectedModelPtr = modelPtr;
				m_lastObserverPtr = paramsSetObserverPtr;

				bool setReadOnly = ((objectPtr->GetIndexOperationFlags(selectedIndex) & iprm::IParamsManager::MF_SUPPORT_EDIT) == 0);
				imod::IModelEditor* modelEditorPtr = CompCastPtr<imod::IModelEditor>(paramsSetObserverPtr);
				if (modelEditorPtr != NULL){
					modelEditorPtr->SetReadOnly(setReadOnly);
				}
			}
		}
	}

	RemoveButton->setEnabled(selectedIndex >= 0);

	bool paramsVisible = (m_lastConnectedModelPtr != NULL);
	if (paramsVisible != ParamsFrame->isVisible()){
		ParamsFrame->setVisible(paramsVisible);
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
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if ((objectPtr != NULL) && objectPtr->GetSelectedOptionIndex() >= 0){
		iprm::IParamsSet* paramsSetPtr = objectPtr->GetParamsSet(objectPtr->GetSelectedOptionIndex());
		if (paramsSetPtr != NULL){
			return paramsSetPtr->GetFactoryId();
		}
	}

	return QByteArray();
}


void CParamsManagerGuiCompBase::InsertNewParamsSet(int typeIndex)
{
	iprm::IParamsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int insertIndex = GetSelectedIndex();
		Q_ASSERT(insertIndex < objectPtr->GetParamsSetsCount());

		int flags = objectPtr->GetIndexOperationFlags(insertIndex);
		if ((flags & iprm::IParamsManager::MF_SUPPORT_INSERT) == 0){ // Fixed parameter set is currently selected
			insertIndex = -1;
		}
		else{
			++insertIndex;
		}

		istd::CChangeNotifier notifier(objectPtr);
		Q_UNUSED(notifier);

		int newIndex = objectPtr->InsertParamsSet(typeIndex, insertIndex);
		if (newIndex >= 0){
			// select newly created element
			objectPtr->SetSelectedOptionIndex(newIndex);
		}
	}
}



// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CParamsManagerGuiCompBase::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	bool areAddRemoveButtonsNeeded = false;
	bool areUpDownButtonsNeeded = false;

	iprm::IParamsManager* objectPtr = GetObservedObject();
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

			if (typesCount > 0){
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
			}
		}
	}

	AddRemoveButtonsFrame->setVisible(areAddRemoveButtonsNeeded);
	UpDownButtonsFrame->setVisible(areUpDownButtonsNeeded);

	ButtonsFrame->setVisible(areAddRemoveButtonsNeeded || areUpDownButtonsNeeded);
}


void CParamsManagerGuiCompBase::OnGuiModelDetached()
{
	EnsureParamsGuiDetached();

	BaseClass::OnGuiModelDetached();
}


void CParamsManagerGuiCompBase::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

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

	bool isLoadAvailable = false;
	bool isSaveAvailable = false;

	if (m_paramsLoaderCompPtr.IsValid()){
		isLoadAvailable = m_paramsLoaderCompPtr->IsOperationSupported(NULL, NULL, ifile::IFilePersistence::QF_LOAD, true);
		isSaveAvailable = m_paramsLoaderCompPtr->IsOperationSupported(NULL, NULL, ifile::IFilePersistence::QF_SAVE, true);
	}

	LoadParamsButton->setVisible(isLoadAvailable);
	SaveParamsButton->setVisible(isSaveAvailable);
}


// reimplemented (imod::CSingleModelObserverBase)

void CParamsManagerGuiCompBase::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::AfterUpdate(modelPtr, changeSet);

	if (changeSet.Contains(iprm::ISelectionParam::CF_SELECTION_CHANGED)){
		DetachCurrentExtender();
		AttachCurrentExtender();
	}
}


// private methods

void CParamsManagerGuiCompBase::AttachCurrentExtender()
{
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


