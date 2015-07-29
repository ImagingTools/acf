#include "iqtprm/COptionsListEditorComp.h"


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

COptionsListEditorComp::COptionsListEditorComp()
	:m_lastSelectedIndex(-1)
{
}


// protected slots

void COptionsListEditorComp::on_OptionsList_itemSelectionChanged()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		m_lastSelectedIndex = GetSelectedIndex();

		iprm::ISelectionParam* selectionPtr = dynamic_cast<iprm::ISelectionParam*>(GetObjectPtr());
		if (selectionPtr != NULL){
			static const istd::IChangeable::ChangeSet changeSet(iprm::ISelectionParam::CF_SELECTION_CHANGED);
			istd::CChangeNotifier selectionNotifier(selectionPtr, &changeSet);
			Q_UNUSED(selectionNotifier);

			selectionPtr->SetSelectedOptionIndex(m_lastSelectedIndex);
		}
	}
}


// protected methods

void COptionsListEditorComp::UpdateList()
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


int COptionsListEditorComp::GetSelectedIndex() const
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


void COptionsListEditorComp::EnsureSelectedIndexUpdated() const
{
	iprm::ISelectionParam* selectionPtr = dynamic_cast<iprm::ISelectionParam*>(GetObjectPtr());
	if (selectionPtr != NULL){
		m_lastSelectedIndex = selectionPtr->GetSelectedOptionIndex();
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void COptionsListEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	OptionsList->setVisible(true);
}


void COptionsListEditorComp::OnGuiModelDetached()
{
	OptionsList->setVisible(false);

	BaseClass::OnGuiModelDetached();
}


void COptionsListEditorComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());
	
	UpdateList();
}


// reimplemented (iqtgui::CComponentBase)

void COptionsListEditorComp::OnGuiCreated()
{
	OptionsList->setVisible(false);

	BaseClass::OnGuiCreated();

	if (m_iconSizeAttrPtr.IsValid()){
		OptionsList->setIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));
		OptionsList->setUniformItemSizes(true);
	}
}


} // namespace iqtprm


