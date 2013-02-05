#include "icmpstr/CMultiAttributeEditor.h"


// Qt includes
#include <QtGui/QHeaderView>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>


// public methods
	
namespace icmpstr
{


CMultiAttributeEditor::CMultiAttributeEditor(
			const CElementSelectionInfoManagerBase& elementSelectionInfoManager,
			const QByteArray& attributeId,
			int attributeFlags)
	:m_elementSelectionInfoManager(elementSelectionInfoManager),
	m_attributeId(attributeId),
	m_attributeFlags(attributeFlags),
	m_valueItemDelegate(*this)
{
	m_dialog.setupUi(this);

	m_dialog.AttributeValuesTree->setItemDelegate(&m_valueItemDelegate);

	connect(m_dialog.AddButton, SIGNAL(clicked()), this, SLOT(OnItemAdd()));
	connect(m_dialog.RemoveButton, SIGNAL(clicked()), this, SLOT(OnItemRemove()));
	connect(m_dialog.UpButton, SIGNAL(clicked()), this, SLOT(OnItemMoveUp()));
	connect(m_dialog.DownButton, SIGNAL(clicked()), this, SLOT(OnItemMoveDown()));
	connect(m_dialog.AttributeValuesTree, SIGNAL(itemSelectionChanged()), this, SLOT(UpdateButtonStates()));
}


void CMultiAttributeEditor::SetEditorText(const QString& attributeValuesText)
{
	QStringList values = attributeValuesText.split(";");

	CreateValuesTree(values);
}


QStringList CMultiAttributeEditor::GetAttributeValues() const
{
	QStringList itemsList;

	int itemsCount = m_dialog.AttributeValuesTree->count();

	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		itemsList.push_back(m_dialog.AttributeValuesTree->item(itemIndex)->text());
	}

	return itemsList;
}


// protected slots

void CMultiAttributeEditor::OnItemAdd()
{
	int insertIndex = m_dialog.AttributeValuesTree->count();

	QList<QListWidgetItem*> selectedItems = m_dialog.AttributeValuesTree->selectedItems();
	if (!selectedItems.isEmpty()){
		QListWidgetItem* itemPtr = selectedItems.back();
		insertIndex = m_dialog.AttributeValuesTree->row(itemPtr) + 1;
	}

	QListWidgetItem* itemPtr = new QListWidgetItem();
	itemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
	
	itemPtr->setText("");

	m_dialog.AttributeValuesTree->insertItem(insertIndex, itemPtr);
	m_dialog.AttributeValuesTree->editItem(itemPtr);

	UpdateButtonStates();
}


void CMultiAttributeEditor::OnItemRemove()
{
	QList<QListWidgetItem*> selectedItems = m_dialog.AttributeValuesTree->selectedItems();
	if (!selectedItems.isEmpty()){
		QListWidgetItem* itemPtr = selectedItems.back();

		int rowIndex = m_dialog.AttributeValuesTree->row(itemPtr);

		itemPtr = m_dialog.AttributeValuesTree->takeItem(rowIndex);
		Q_ASSERT(itemPtr != NULL);

		delete itemPtr;

		m_dialog.AttributeValuesTree->setItemSelected(m_dialog.AttributeValuesTree->item(qMax(rowIndex - 1, 0)), true);
	}

	UpdateButtonStates();
}


void CMultiAttributeEditor::OnItemMoveUp()
{
	QList<QListWidgetItem*> selectedItems = m_dialog.AttributeValuesTree->selectedItems();
	if (!selectedItems.isEmpty()){
		QListWidgetItem* itemPtr = selectedItems.back();

		int rowIndex = m_dialog.AttributeValuesTree->row(itemPtr);

		itemPtr = m_dialog.AttributeValuesTree->takeItem(rowIndex);
		Q_ASSERT(itemPtr != NULL);

		m_dialog.AttributeValuesTree->insertItem(rowIndex - 1, itemPtr);

		m_dialog.AttributeValuesTree->setItemSelected(itemPtr, true);
	}
}


void CMultiAttributeEditor::OnItemMoveDown()
{
	QList<QListWidgetItem*> selectedItems = m_dialog.AttributeValuesTree->selectedItems();
	if (!selectedItems.isEmpty()){
		QListWidgetItem* itemPtr = selectedItems.back();

		int rowIndex = m_dialog.AttributeValuesTree->row(itemPtr);

		itemPtr = m_dialog.AttributeValuesTree->takeItem(rowIndex);
		Q_ASSERT(itemPtr != NULL);

		m_dialog.AttributeValuesTree->insertItem(rowIndex + 1, itemPtr);

		m_dialog.AttributeValuesTree->setItemSelected(itemPtr, true);
	}
}


void CMultiAttributeEditor::UpdateButtonStates()
{
	bool itemSelected = false;
	int selectedRowIndex = -1;
	QList<QListWidgetItem*> selectedItems = m_dialog.AttributeValuesTree->selectedItems();
	if (!selectedItems.isEmpty()){
		QListWidgetItem* itemPtr = selectedItems.at(0);

		selectedRowIndex = m_dialog.AttributeValuesTree->row(itemPtr);

		itemSelected = true;
	}

	m_dialog.UpButton->setEnabled(itemSelected && selectedRowIndex > 0);
	m_dialog.DownButton->setEnabled(itemSelected && selectedRowIndex < m_dialog.AttributeValuesTree->count() - 1);
	m_dialog.RemoveButton->setEnabled(itemSelected);
}


// protected methods

void CMultiAttributeEditor::CreateValuesTree(const QStringList& values)
{
	for (int itemIndex = 0; itemIndex < values.count(); itemIndex++){
		QListWidgetItem* itemPtr = new QListWidgetItem();
		itemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
	
		itemPtr->setText(values[itemIndex]);

		m_dialog.AttributeValuesTree->addItem(itemPtr);
	}

	UpdateButtonStates();
}


// public methods of embedded class ValueItemDelegate

CMultiAttributeEditor::ValueItemDelegate::ValueItemDelegate(const CMultiAttributeEditor& parent)
:	m_parent(parent)
{
}


// reimplemented (QItemDelegate)

QWidget* CMultiAttributeEditor::ValueItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (		((m_parent.m_attributeFlags & icomp::IAttributeStaticInfo::AF_REFERENCE) != 0) ||
				((m_parent.m_attributeFlags & icomp::IAttributeStaticInfo::AF_FACTORY) != 0)){
		QComboBox* comboEditor = new QComboBox(parent);
		QString text = index.data().toString();
		comboEditor->setEditable(true);
		comboEditor->setEditText(text);
	
		return comboEditor;
	} 

	return BaseClass::createEditor(parent, option, index);
}


void CMultiAttributeEditor::ValueItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	const icomp::IMetaInfoManager* metaInfoManagerPtr = m_parent.m_elementSelectionInfoManager.GetMetaInfoManagerPtr();
	if (metaInfoManagerPtr == NULL){
		return;
	}

	const IElementSelectionInfo* selectionInfoPtr = m_parent.m_elementSelectionInfoManager.GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return;
	}

	IElementSelectionInfo::Elements selectedElements = selectionInfoPtr->GetSelectedElements();
	if (selectedElements.isEmpty()){
		return;
	}

	const IRegistryConsistInfo* consistInfoPtr = m_parent.m_elementSelectionInfoManager.GetConsistencyInfoPtr();

	QByteArray attributeId = m_parent.m_attributeId;

	for (		IElementSelectionInfo::Elements::const_iterator elemIter = selectedElements.begin();
				elemIter != selectedElements.end();
				++elemIter){
		const icomp::IRegistry::ElementInfo* elementInfoPtr = elemIter.value();
		Q_ASSERT(elementInfoPtr != NULL);
		if (!elementInfoPtr->elementPtr.IsValid()){
			continue;
		}

		const iser::IObject* attributePtr = m_parent.m_elementSelectionInfoManager.GetAttributeObject(attributeId, *elementInfoPtr);
		if (attributePtr == NULL){
			continue;
		}

		QComboBox* comboEditor = dynamic_cast<QComboBox*>(editor);
		if (m_parent.m_attributeFlags & icomp::IAttributeStaticInfo::AF_REFERENCE){
			if (comboEditor == NULL){
				return;
			}

			const icomp::IRegistry* registryPtr = m_parent.m_elementSelectionInfoManager.GetRegistry();
			const icomp::IAttributeStaticInfo* staticInfoPtr = m_parent.m_elementSelectionInfoManager.GetAttributeStaticInfo(attributeId, *elementInfoPtr);
			if ((registryPtr != NULL) && (staticInfoPtr != NULL) && (consistInfoPtr != NULL)){
				int queryFlags = IRegistryConsistInfo::QF_NONE;
				icomp::IElementStaticInfo::Ids obligatoryInterfaces = staticInfoPtr->GetRelatedMetaIds(
							icomp::IComponentStaticInfo::MGI_INTERFACES,
							0,
							icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the interfaces which must be set
				if (obligatoryInterfaces.isEmpty()){
					obligatoryInterfaces = staticInfoPtr->GetRelatedMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES, 0, 0);	// All asked interface names
					queryFlags = IRegistryConsistInfo::QF_ANY_INTERFACE;	// for optional interfaces only we are looking for any of them
				}
				icomp::IRegistry::Ids compatIds = consistInfoPtr->GetCompatibleElements(
							obligatoryInterfaces,
							*registryPtr,
							queryFlags);

				QList< QByteArray> compatIdList = compatIds.toList();	
				qSort(compatIdList);
				
				for(		QList< QByteArray>::const_iterator iter = compatIdList.begin();
							iter != compatIdList.end();
							++iter){
					comboEditor->addItem(*iter);
				}
			}

			comboEditor->setEditText(index.data().toString());

			return;
		}
	}

	BaseClass::setEditorData(editor, index);
}


void CMultiAttributeEditor::ValueItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* comboEditor = dynamic_cast<QComboBox*>(editor);
	if (comboEditor != NULL){
		QMap<int,QVariant> data;
		data.insert(Qt::DisplayRole, comboEditor->lineEdit()->text());
		model->setItemData(index, data);

		return;
	}

	BaseClass::setModelData(editor, model, index);
}


} // namespace icmpstr


