#include "iqtprm/COptionsListEditorGuiComp.h"


// Qt includes
#include <QtCore/QUuid>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CChangeDelegator.h"
#include "iprm/IParamsSet.h"
#include "iprm/IOptionsList.h"
#include "iprm/ISelectionParam.h"
#include "iqtgui/CItemDelegate.h"
#include "iqtgui/CWidgetUpdateBlocker.h"


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


void COptionsListEditorGuiComp::on_ParamsTree_itemSelectionChanged()
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


void COptionsListEditorGuiComp::on_ParamsTree_itemChanged(QTreeWidgetItem* item, int column)
{
	if (*m_showDescriptionAttrPtr){
		if (column != 0 && column != 1){
			return;
		}
	}
	else{
		if (column != 0){
			return;
		}
	}

	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	iprm::IOptionsManager* objectPtr = dynamic_cast<iprm::IOptionsManager*>(GetObjectPtr());
	if (objectPtr != NULL){
		int setIndex = item->data(0, Qt::UserRole).toInt();

		if (column == 0){
			objectPtr->SetOptionName(setIndex, item->text(column));
		}
		else{
			objectPtr->SetOptionDescription(setIndex, item->text(column));
		}
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


void COptionsListEditorGuiComp::UpdateTree()
{
	UpdateBlocker updateBlocker(this);

	int lastSelectedIndex = m_lastSelectedIndex;

	ParamsTree->clear();

	if (*m_showListHeaderAttrPtr){
		QStringList headers;
		if (m_headerListAttrPtr.IsValid()){
			int listCount = m_headerListAttrPtr.GetCount();
			for (int listIndex = 0; listIndex < listCount; listIndex++){
				headers << m_headerListAttrPtr[listIndex];
			}
		}
		ParamsTree->setHeaderLabels(headers);
		ParamsTree->setHeaderHidden(false);
	}

	iprm::IOptionsList* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int setsCount = objectPtr->GetOptionsCount();

		for (int optionIndex = 0; optionIndex < setsCount; ++optionIndex){

			Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

			iprm::IOptionsManager* managerPtr = dynamic_cast<iprm::IOptionsManager*>(objectPtr);
			if (managerPtr != NULL){
				int operationFlags = managerPtr->GetOptionOperationFlags(optionIndex);
				if (operationFlags & iprm::IOptionsManager::OOF_SUPPORT_RENAME){
					itemFlags |= Qt::ItemIsEditable;
				}
			}

			QString name = objectPtr->GetOptionName(optionIndex);
			QString description = objectPtr->GetOptionDescription(optionIndex);
			QTreeWidgetItem* optionItemPtr = new QTreeWidgetItem();
			optionItemPtr->setText(0, name);

			if (*m_showDescriptionAttrPtr){
				optionItemPtr->setText(1, description);
			}

			optionItemPtr->setData(0, Qt::UserRole, optionIndex);
			optionItemPtr->setFlags(itemFlags);

			ParamsTree->addTopLevelItem(optionItemPtr);

			optionItemPtr->setSelected(optionIndex == lastSelectedIndex);	
		}
	}
}


int COptionsListEditorGuiComp::GetSelectedIndex() const
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

	AddRemoveButtonsFrame->setVisible((objectPtr != NULL) && *m_allowAddRemoveAttrPtr);
	UpDownButtonsFrame->setVisible((objectPtr != NULL) && *m_allowUpDownAttrPtr);

	ButtonsFrame->setVisible(AddRemoveButtonsFrame->isVisible() || UpDownButtonsFrame->isVisible());

	ParamsTree->setVisible(true);
}


void COptionsListEditorGuiComp::OnGuiModelDetached()
{
	ParamsTree->setVisible(false);

	ButtonsFrame->setVisible(false);

	BaseClass::OnGuiModelDetached();
}


void COptionsListEditorGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());
	
	UpdateTree();
}


// reimplemented (iqtgui::CComponentBase)

void COptionsListEditorGuiComp::OnGuiCreated()
{
	ParamsTree->setVisible(false);

	AddRemoveButtonsFrame->setVisible(false);
	UpDownButtonsFrame->setVisible(false);

	ButtonsFrame->setVisible(*m_allowAddRemoveAttrPtr);

	if (*m_showDescriptionAttrPtr){
		ParamsTree->setColumnCount(2);
	}
	else{
		ParamsTree->setColumnCount(1);
	}

	BaseClass::OnGuiCreated();

	UpdateActions();
}


void COptionsListEditorGuiComp::OnAddMenuOptionClicked(QAction* /*action*/)
{
	on_AddButton_clicked();
}


} // namespace iqtprm


