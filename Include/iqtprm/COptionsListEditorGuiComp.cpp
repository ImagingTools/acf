#include "iqtprm/COptionsListEditorGuiComp.h"


// Qt includes
#include <QtCore/QUuid>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CChangeDelegator.h"
#include "iprm/IParamsSet.h"
#include "iprm/IOptionsList.h"
#include "iprm/ISelectionParam.h"
#include "iqtgui/IMultiVisualStatusProvider.h"
#include "iwidgets/CItemDelegate.h"
#include "iwidgets/CWidgetUpdateBlocker.h"


namespace iqtprm
{


// public methods

COptionsListEditorGuiComp::COptionsListEditorGuiComp()
	:m_lastSelectedIndex(-1)
{
}


// protected slots

void COptionsListEditorGuiComp::on_AddButton_clicked()
{
	static QString defaultOptionName= "Option";
	static QString defaultOptionDescription;

#if QT_VERSION >= 0x040800
    QByteArray defaultOptionId = QUuid::createUuid().toByteArray();
#else
    QByteArray defaultOptionId = QUuid::createUuid().toString().toLatin1();
#endif

	iprm::IOptionsManager* objectPtr = dynamic_cast<iprm::IOptionsManager*>(GetObjectPtr());
	if (objectPtr != NULL){
		objectPtr->InsertOption(
					defaultOptionName,
					defaultOptionId,
					defaultOptionDescription,
					m_lastSelectedIndex);
	}

	UpdateActions();
}


void COptionsListEditorGuiComp::on_RemoveButton_clicked()
{
	iprm::IOptionsManager* objectPtr = dynamic_cast<iprm::IOptionsManager*>(GetObjectPtr());
	if (objectPtr != NULL){
		Q_ASSERT(m_lastSelectedIndex < objectPtr->GetOptionsCount());

		if (m_lastSelectedIndex >= 0){
			objectPtr->RemoveOption(m_lastSelectedIndex);
		}
	}

	UpdateActions();
}


void COptionsListEditorGuiComp::on_UpButton_clicked()
{
	iprm::IOptionsManager* objectPtr = dynamic_cast<iprm::IOptionsManager*>(GetObjectPtr());
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


void COptionsListEditorGuiComp::on_DownButton_clicked()
{
	iprm::IOptionsManager* objectPtr = dynamic_cast<iprm::IOptionsManager*>(GetObjectPtr());
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


void COptionsListEditorGuiComp::on_OptionsList_itemSelectionChanged()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		m_lastSelectedIndex = GetSelectedIndex();

		iprm::ISelectionParam* selectionPtr = dynamic_cast<iprm::ISelectionParam*>(GetObjectPtr());
		if (selectionPtr != NULL){
			istd::CChangeNotifier changePtr(selectionPtr, iprm::ISelectionParam::CF_SELECTION_CHANGED);

			selectionPtr->SetSelectedOptionIndex(m_lastSelectedIndex);
		}

		UpdateActions();
	}
}


void COptionsListEditorGuiComp::on_OptionsList_itemChanged(QListWidgetItem* item)
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	iprm::IOptionsManager* objectPtr = dynamic_cast<iprm::IOptionsManager*>(GetObjectPtr());
	if (objectPtr != NULL){
		int optionIndex = item->data(Qt::UserRole).toInt();

		objectPtr->SetOptionName(optionIndex, item->text());
	}
}


// protected methods

void COptionsListEditorGuiComp::UpdateActions()
{
	Q_ASSERT(IsGuiCreated());

	EnsureSelectedIndexUpdated();

	iprm::IOptionsManager* objectPtr = dynamic_cast<iprm::IOptionsManager*>(GetObjectPtr());
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


void COptionsListEditorGuiComp::UpdateList()
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


int COptionsListEditorGuiComp::GetSelectedIndex() const
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


void COptionsListEditorGuiComp::EnsureSelectedIndexUpdated() const
{
	iprm::ISelectionParam* selectionPtr = dynamic_cast<iprm::ISelectionParam*>(GetObjectPtr());
	if (selectionPtr != NULL){
		m_lastSelectedIndex = selectionPtr->GetSelectedOptionIndex();
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void COptionsListEditorGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::IOptionsManager* objectPtr = dynamic_cast<iprm::IOptionsManager*>(GetObjectPtr());
	if (objectPtr != NULL){
		m_startVariableMenus.clear();
	}

	bool addRemoveVisible = (objectPtr != NULL) && *m_allowAddRemoveAttrPtr;
	bool upDownVisible = (objectPtr != NULL) && *m_allowUpDownAttrPtr;

	ButtonsFrame->setVisible(addRemoveVisible || upDownVisible);
	AddRemoveButtonsFrame->setVisible(addRemoveVisible);
	UpDownButtonsFrame->setVisible(upDownVisible);


	OptionsList->setVisible(true);
}


void COptionsListEditorGuiComp::OnGuiModelDetached()
{
	OptionsList->setVisible(false);

	ButtonsFrame->setVisible(false);

	BaseClass::OnGuiModelDetached();
}


void COptionsListEditorGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());
	
	UpdateList();
}


// reimplemented (iqtgui::CComponentBase)

void COptionsListEditorGuiComp::OnGuiCreated()
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


void COptionsListEditorGuiComp::OnAddMenuOptionClicked(QAction* /*action*/)
{
	on_AddButton_clicked();
}


} // namespace iqtprm


