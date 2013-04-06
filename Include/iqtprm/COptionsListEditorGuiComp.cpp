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
	:m_lastConnectedModelPtr(NULL),
	m_lastSelectedIndex(-1)
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

	iprm::IOptionsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->InsertOption(
					defaultOptionName,
					defaultOptionId,
					defaultOptionDescription,
					m_lastSelectedIndex);
	}
}


void COptionsListEditorGuiComp::on_RemoveButton_clicked()
{
	iprm::IOptionsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		Q_ASSERT(m_lastSelectedIndex < objectPtr->GetOptionsCount());

		if (m_lastSelectedIndex >= 0){
			objectPtr->RemoveOption(m_lastSelectedIndex);
		}
	}
}


void COptionsListEditorGuiComp::on_UpButton_clicked()
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
}


void COptionsListEditorGuiComp::on_DownButton_clicked()
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
}


void COptionsListEditorGuiComp::on_ParamsTree_itemSelectionChanged()
{
	m_lastSelectedIndex = GetSelectedIndex();

	iprm::IOptionsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpButton->setEnabled(m_lastSelectedIndex > 0);
		DownButton->setEnabled(m_lastSelectedIndex > -1 && m_lastSelectedIndex < objectPtr->GetOptionsCount() - 1);
	}

	RemoveButton->setEnabled(m_lastSelectedIndex != -1);

	UpdateActions();
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

	iprm::IOptionsManager* objectPtr = GetObjectPtr();
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


void COptionsListEditorGuiComp::UpdateTree()
{
	UpdateBlocker updateBlocker(this);

	int lastSelectedIndex = m_lastSelectedIndex;

	ParamsTree->clear();

	iprm::IOptionsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int setsCount = objectPtr->GetOptionsCount();

		for (int paramSetIndex = 0; paramSetIndex < setsCount; ++paramSetIndex){

			Qt::ItemFlags itemFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

			QString name = objectPtr->GetOptionName(paramSetIndex);
			QString description = objectPtr->GetOptionDescription(paramSetIndex);
			QTreeWidgetItem* paramsSetItemPtr = new QTreeWidgetItem();
			paramsSetItemPtr->setText(0, name);
			if (*m_showDescriptionAttrPtr){
				paramsSetItemPtr->setText(1, description);
			}
			paramsSetItemPtr->setData(0, Qt::UserRole, paramSetIndex);
			paramsSetItemPtr->setFlags(itemFlags);

			ParamsTree->addTopLevelItem(paramsSetItemPtr);

			paramsSetItemPtr->setSelected(paramSetIndex == lastSelectedIndex);	
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


void COptionsListEditorGuiComp::EnsureParamsGuiDetached()
{
	if ((m_lastObserverPtr != NULL) && (m_lastConnectedModelPtr != NULL)){
		if (m_lastObserverPtr->IsModelAttached(m_lastConnectedModelPtr)){
			m_lastConnectedModelPtr->DetachObserver(m_lastObserverPtr);
		}
	}

	m_lastConnectedModelPtr = NULL;
	m_lastObserverPtr = NULL;
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void COptionsListEditorGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	iprm::IOptionsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_startVariableMenus.clear();
	}

	AddRemoveButtonsFrame->setVisible(*m_allowAddRemoveAttrPtr);
	UpDownButtonsFrame->setVisible(*m_allowUpDownAttrPtr);

	ButtonsFrame->setVisible(*m_allowAddRemoveAttrPtr || *m_allowUpDownAttrPtr);
	ParamsTree->setItemDelegate(new iqtgui::CItemDelegate());
}


void COptionsListEditorGuiComp::OnGuiModelDetached()
{
	EnsureParamsGuiDetached();

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
	BaseClass::OnGuiCreated();

	ButtonsFrame->setVisible(*m_allowAddRemoveAttrPtr);

	if (*m_showDescriptionAttrPtr){
		ParamsTree->setColumnCount(2);
	}
	else{
		ParamsTree->setColumnCount(1);
	}

	UpdateActions();
}


void COptionsListEditorGuiComp::OnAddMenuOptionClicked(QAction* /*action*/)
{
	on_AddButton_clicked();
}


} // namespace iqtprm


