#include "iqtprm/COptionsManagerEditorComp.h"


// Qt includes
#include <QtCore/QUuid>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "iprm/IParamsSet.h"
#include "iprm/IOptionsList.h"
#include "iprm/ISelectionParam.h"
#include "iqtgui/IMultiVisualStatusProvider.h"
#include "iwidgets/CItemDelegate.h"
#include "iwidgets/CWidgetUpdateBlocker.h"


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
	static QString defaultOptionName= "Option";
	static QString defaultOptionDescription;

#if QT_VERSION >= 0x040800
	QByteArray defaultOptionId = QUuid::createUuid().toByteArray();
#else
	QByteArray defaultOptionId = QUuid::createUuid().toString().toLatin1();
#endif

	iprm::IOptionsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
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
	iprm::IOptionsManager* objectPtr = GetObjectPtr();
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
	iprm::IOptionsManager* objectPtr = GetObjectPtr();
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
	iprm::IOptionsManager* objectPtr = GetObjectPtr();
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

		iprm::ISelectionParam* selectionPtr = GetObjectPtr();
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

	iprm::IOptionsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int optionIndex = item->data(Qt::UserRole).toInt();

		objectPtr->SetOptionName(optionIndex, item->text());
	}
}


// protected methods

void COptionsManagerEditorComp::UpdateActions()
{
	Q_ASSERT(IsGuiCreated());

	EnsureSelectedIndexUpdated();

	iprm::IOptionsManager* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		AddButton->setEnabled(false);
		RemoveButton->setEnabled(false);
		UpButton->setEnabled(false);
		DownButton->setEnabled(false);

		return;
	}

	AddButton->setEnabled(true);
	RemoveButton->setEnabled(m_lastSelectedIndex >= 0);
	UpButton->setEnabled(m_lastSelectedIndex > 0);
	DownButton->setEnabled((m_lastSelectedIndex >= 0) && (m_lastSelectedIndex < objectPtr->GetOptionsCount() - 1));
}


void COptionsManagerEditorComp::UpdateList()
{
	UpdateBlocker updateBlocker(this);

	int lastSelectedIndex = m_lastSelectedIndex;

	OptionsList->clear();
	
	iprm::IOptionsList* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int optionsCount = objectPtr->GetOptionsCount();

		for (int optionIndex = 0; optionIndex < optionsCount; ++optionIndex){

			Qt::ItemFlags itemFlags = objectPtr->IsOptionEnabled(optionIndex) ? Qt::ItemIsEnabled | Qt::ItemIsSelectable : Qt::NoItemFlags;

			iprm::IOptionsManager* managerPtr = dynamic_cast<iprm::IOptionsManager*>(objectPtr);
			if (managerPtr != NULL){
				int operationFlags = managerPtr->GetOptionOperationFlags(optionIndex);
				if (objectPtr->IsOptionEnabled(optionIndex) && (operationFlags & iprm::IOptionsManager::OOF_SUPPORT_RENAME)){
					itemFlags |= Qt::ItemIsEditable;
				}
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
	iprm::ISelectionParam* selectionPtr = GetObjectPtr();
	if (selectionPtr != NULL){
		m_lastSelectedIndex = selectionPtr->GetSelectedOptionIndex();
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void COptionsManagerEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::IOptionsManager* objectPtr = GetObjectPtr();

	bool addRemoveVisible = (objectPtr != NULL) && *m_allowAddRemoveAttrPtr;
	bool upDownVisible = (objectPtr != NULL) && *m_allowUpDownAttrPtr;

	ButtonsFrame->setVisible(addRemoveVisible || upDownVisible);
	AddRemoveButtonsFrame->setVisible(addRemoveVisible);
	UpDownButtonsFrame->setVisible(upDownVisible);


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

	BaseClass::OnGuiCreated();

	if (m_iconSizeAttrPtr.IsValid()){
		OptionsList->setIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));
		OptionsList->setUniformItemSizes(true);
	}

	UpdateActions();
}


} // namespace iqtprm


