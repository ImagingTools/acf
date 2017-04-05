#include <iqtprm/COptionsManagerEditorComp.h>


// Qt includes
#include <QtCore/QUuid>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iprm/IParamsSet.h>
#include <iprm/IOptionsList.h>
#include <iprm/ISelectionParam.h>
#include <iqtgui/IMultiVisualStatusProvider.h>
#include <iwidgets/CItemDelegate.h>
#include <iwidgets/CWidgetUpdateBlocker.h>


namespace iqtprm
{


// public methods

COptionsManagerEditorComp::COptionsManagerEditorComp()
:	m_lastSelectedIndex(-1)
{
}


// protected slots

void COptionsManagerEditorComp::on_AddButton_clicked()
{
	static QString defaultOptionDescription;

#if QT_VERSION >= 0x040800
	QByteArray defaultOptionId = QUuid::createUuid().toByteArray();
#else
	QByteArray defaultOptionId = QUuid::createUuid().toString().toLatin1();
#endif


	iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QString defaultOptionName= CalculateNewDefaultName();

		objectPtr->InsertOption(
					defaultOptionName,
					defaultOptionId,
					defaultOptionDescription,
					m_lastSelectedIndex);
	}

	UpdateActions();
}


void COptionsManagerEditorComp::on_RemoveButton_clicked()
{
	iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		Q_ASSERT(m_lastSelectedIndex < objectPtr->GetOptionsCount());

		if (m_lastSelectedIndex >= 0){
			objectPtr->RemoveOption(m_lastSelectedIndex);
		}
	}

	UpdateActions();
}


void COptionsManagerEditorComp::on_UpButton_clicked()
{
	iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		Q_ASSERT(m_lastSelectedIndex < objectPtr->GetOptionsCount());

		if (m_lastSelectedIndex < 1){
			return;
		}

		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SwapOptions(m_lastSelectedIndex, m_lastSelectedIndex - 1);
		--m_lastSelectedIndex;
	}

	UpdateActions();
}


void COptionsManagerEditorComp::on_DownButton_clicked()
{
	iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		Q_ASSERT(m_lastSelectedIndex < objectPtr->GetOptionsCount());

		if ((m_lastSelectedIndex < 0) || (m_lastSelectedIndex >= objectPtr->GetOptionsCount() - 1)){
			return;
		}

		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SwapOptions(m_lastSelectedIndex, m_lastSelectedIndex + 1);
		++m_lastSelectedIndex;
	}

	UpdateActions();
}


void COptionsManagerEditorComp::on_OptionsList_itemSelectionChanged()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		m_lastSelectedIndex = GetSelectedIndex();

		iprm::ISelectionParam* selectionPtr = GetObservedObject();
		if (selectionPtr != NULL){
			static const istd::IChangeable::ChangeSet changeSet(iprm::ISelectionParam::CF_SELECTION_CHANGED);
			istd::CChangeNotifier selectionNotifier(selectionPtr, &changeSet);
			Q_UNUSED(selectionNotifier);

			selectionPtr->SetSelectedOptionIndex(m_lastSelectedIndex);
		}

		UpdateActions();
	}
}


void COptionsManagerEditorComp::on_OptionsList_itemChanged(QListWidgetItem* item)
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int optionIndex = item->data(Qt::UserRole).toInt();

		objectPtr->SetOptionName(optionIndex, item->text());
	}
}


void COptionsManagerEditorComp::on_LoadParamsButton_clicked()
{
	iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->LoadFromFile(*objectPtr);
	}
}


void COptionsManagerEditorComp::on_SaveParamsButton_clicked()
{
	const iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->SaveToFile(*objectPtr);
	}
}


// protected methods

void COptionsManagerEditorComp::UpdateActions()
{
	Q_ASSERT(IsGuiCreated());

	EnsureSelectedIndexUpdated();

	UpdateButtonsVisibility();
}


void COptionsManagerEditorComp::UpdateList()
{
	UpdateBlocker updateBlocker(this);

	int lastSelectedIndex = m_lastSelectedIndex;

	OptionsList->clear();
	
	iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int optionsCount = objectPtr->GetOptionsCount();

		for (int optionIndex = 0; optionIndex < optionsCount; ++optionIndex){

			Qt::ItemFlags itemFlags = objectPtr->IsOptionEnabled(optionIndex)? Qt::ItemIsEnabled | Qt::ItemIsSelectable : Qt::NoItemFlags;

			int operationFlags = objectPtr->GetOptionOperationFlags(optionIndex);
			if (objectPtr->IsOptionEnabled(optionIndex) && ((operationFlags & iprm::IOptionsManager::OOF_SUPPORT_RENAME) != 0)){
				itemFlags |= Qt::ItemIsEditable;
			}

			QString name = objectPtr->GetOptionName(optionIndex);
			QString description = objectPtr->GetOptionDescription(optionIndex);
			QListWidgetItem* optionItemPtr = new QListWidgetItem();
			optionItemPtr->setText(name);

			const iqtgui::IMultiVisualStatusProvider* visualStatusProviderPtr = dynamic_cast<const iqtgui::IMultiVisualStatusProvider*>(objectPtr);
			if (visualStatusProviderPtr != NULL){
				const iqtgui::IVisualStatus* visualStatusPtr = visualStatusProviderPtr->GetVisualStatus(optionIndex);
				if (visualStatusPtr != NULL){
					optionItemPtr->setIcon(visualStatusPtr->GetStatusIcon());
				}
			}

			optionItemPtr->setData(Qt::UserRole, optionIndex);
			optionItemPtr->setFlags(itemFlags);

			if (m_iconSizeAttrPtr.IsValid()){
				optionItemPtr->setSizeHint(QSize(optionItemPtr->sizeHint().width(), *m_iconSizeAttrPtr + 2));
			}

			OptionsList->addItem(optionItemPtr);

			if (itemFlags == Qt::ItemIsSelectable){
				optionItemPtr->setSelected(optionIndex == lastSelectedIndex);
			}
		}
	}
}


int COptionsManagerEditorComp::GetSelectedIndex() const
{
	Q_ASSERT(IsGuiCreated());

	int retVal = -1;

	QList<QListWidgetItem*> items = OptionsList->selectedItems();
	if (!items.isEmpty()){
		QListWidgetItem* itemPtr = items.first();

		if (itemPtr != NULL){
			retVal = itemPtr->data(Qt::UserRole).toInt();
		}
	}

	return retVal;
}


void COptionsManagerEditorComp::EnsureSelectedIndexUpdated() const
{
	iprm::ISelectionParam* selectionPtr = GetObservedObject();
	if (selectionPtr != NULL){
		m_lastSelectedIndex = selectionPtr->GetSelectedOptionIndex();
	}
}


// protected methods

QString COptionsManagerEditorComp::CalculateNewDefaultName() const
{
	Q_ASSERT(IsGuiCreated());

	QString defaultSetName = *m_defaultOptionNameAttrPtr;
	if (defaultSetName.contains("%1")){			
		for (int suffixIndex = 1; suffixIndex < 1000; ++suffixIndex){
			QString tmpName = defaultSetName;
			tmpName.replace(QString("%1"), QString::number(suffixIndex));
			if (OptionsList->findItems(tmpName, Qt::MatchExactly).isEmpty()){
				defaultSetName = tmpName;
				break;
			}
		}
	}

	return defaultSetName;
}


// reimplemented (iqtgui::TGuiObserverWrap)

void COptionsManagerEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::IOptionsManager* objectPtr = GetObservedObject();

	bool addRemoveVisible = (objectPtr != NULL) && *m_allowAddRemoveAttrPtr;
	bool upDownVisible = (objectPtr != NULL) && *m_allowUpDownAttrPtr;

	ButtonsFrame->setVisible(addRemoveVisible || upDownVisible);
	AddRemoveButtonsFrame->setVisible(addRemoveVisible);
	UpDownButtonsFrame->setVisible(upDownVisible);

	UpdateButtonsVisibility();

	OptionsList->setVisible(true);
}


void COptionsManagerEditorComp::OnGuiModelDetached()
{
	OptionsList->setVisible(false);

	ButtonsFrame->setVisible(false);

	BaseClass::OnGuiModelDetached();
}


void COptionsManagerEditorComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());
	
	UpdateList();
}


// reimplemented (iqtgui::CComponentBase)

void COptionsManagerEditorComp::OnGuiCreated()
{
	OptionsList->setVisible(false);

	AddRemoveButtonsFrame->setVisible(false);
	UpDownButtonsFrame->setVisible(false);

	ButtonsFrame->setVisible(*m_allowAddRemoveAttrPtr);

	bool isLoadAvailable = false;
	bool isSaveAvailable = false;

	if (m_paramsLoaderCompPtr.IsValid()){
		isLoadAvailable = m_paramsLoaderCompPtr->IsOperationSupported(NULL, NULL, ifile::IFilePersistence::QF_LOAD, true);
		isSaveAvailable = m_paramsLoaderCompPtr->IsOperationSupported(NULL, NULL, ifile::IFilePersistence::QF_SAVE, true);
	}

	LoadParamsButton->setVisible(isLoadAvailable);
	SaveParamsButton->setVisible(isSaveAvailable);

	BaseClass::OnGuiCreated();

	if (m_iconSizeAttrPtr.IsValid()){
		OptionsList->setIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));
		OptionsList->setUniformItemSizes(true);
	}

	AddButton->setEnabled(false);
	RemoveButton->setEnabled(false);
	UpButton->setEnabled(false);
	DownButton->setEnabled(false);

	UpdateActions();
}


void COptionsManagerEditorComp::UpdateButtonsVisibility()
{
	if (!IsGuiCreated()) {
		return;
	}
	iprm::IOptionsManager* objectPtr = GetObservedObject();
	if (objectPtr == NULL){
		return;
	}

	int operationFlags = objectPtr->GetOptionOperationFlags(m_lastSelectedIndex);

	bool isAddAllowed = ((operationFlags & iprm::IOptionsManager::OOF_SUPPORT_INSERT) != 0);
	bool isRemoveAllowed = (m_lastSelectedIndex >= 0) && ((operationFlags & iprm::IOptionsManager::OOF_SUPPORT_DELETE) != 0);
	bool isUpAllowed = (m_lastSelectedIndex > 0) && ((objectPtr->GetOptionOperationFlags(m_lastSelectedIndex - 1) & iprm::IOptionsManager::OOF_SUPPORT_SWAP) != 0);
	bool isDownAllowed = ((m_lastSelectedIndex >= 0) &&
						(m_lastSelectedIndex < objectPtr->GetOptionsCount() - 1)) &&
						((objectPtr->GetOptionOperationFlags(m_lastSelectedIndex + 1) & iprm::IOptionsManager::OOF_SUPPORT_SWAP) != 0);

	AddButton->setEnabled(isAddAllowed);
	RemoveButton->setEnabled(isRemoveAllowed);
	UpButton->setEnabled(isUpAllowed);
	DownButton->setEnabled(isDownAllowed);
}



} // namespace iqtprm


