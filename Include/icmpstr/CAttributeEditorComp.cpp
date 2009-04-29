#include "icmpstr/CAttributeEditorComp.h"


// Qt includes
#include <QHeaderView>
#include <QListWidget>
#include <QLineEdit>

#include "icomp/CRegistryElement.h" 

#include "icmpstr/CRegistryViewComp.h"


// public methods
	
namespace icmpstr
{


CAttributeEditorComp::CAttributeEditorComp()
:	m_attributeItemDelegate(*this)
{
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CBoolAttribute>()] = tr("Boolean");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CDoubleAttribute>()] = tr("Real number");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CIntAttribute>()] = tr("Integer number");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CStringAttribute>()] = tr("String");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CMultiBoolAttribute>()] = tr("Boolean list");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CMultiDoubleAttribute>()] = tr("Real number list");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CMultiIntAttribute>()] = tr("Integer number list");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CMultiStringAttribute>()] = tr("String list");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CReferenceAttribute>()] = tr("Reference");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CMultiReferenceAttribute>()] = tr("Multiple reference");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CFactoryAttribute>()] = tr("Factory");
	m_attributeTypesMap[istd::CClassInfo::GetInfo<icomp::CMultiFactoryAttribute>()] = tr("Multiple factory");
}


icomp::IRegistryElement* CAttributeEditorComp::GetRegistryElement() const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return NULL;
	}

	return dynamic_cast<icomp::IRegistryElement*>(selectionInfoPtr->GetSelectedElement());
}


icomp::IRegistryElement::AttributeInfo* CAttributeEditorComp::GetRegistryAttribute(const std::string& attributeName) const
{
	icomp::IRegistryElement* elementPtr = GetRegistryElement();
	if (elementPtr == NULL){
		return NULL;
	}

	return const_cast<icomp::IRegistryElement::AttributeInfo*>(elementPtr->GetAttributeInfo(attributeName));
}


const icomp::IAttributeStaticInfo* CAttributeEditorComp::GetStaticAttributeInfo(const std::string& attributeId) const
{
	const icomp::IRegistryElement* elementPtr = GetRegistryElement();
	if (elementPtr == NULL){
		return NULL;
	}

	const icomp::IComponentStaticInfo& elementStaticInfo = elementPtr->GetComponentStaticInfo();
	const icomp::IComponentStaticInfo::AttributeInfos& staticAttributes = elementStaticInfo.GetAttributeInfos();

	const icomp::IComponentStaticInfo::AttributeInfos::ValueType* attrInfoPtr2 =
				staticAttributes.FindElement(attributeId);

	if (attrInfoPtr2 != NULL){
		return *attrInfoPtr2;
	}
	else{
		return NULL;
	}
}

	
QStringList CAttributeEditorComp::GetCompatibleComponents(const istd::CClassInfo& interfaceInfo) const
{
	IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return QStringList();
	}

	const icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
	if (registryPtr == NULL){
		return QStringList();
	}

	QStringList retVal;

	icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator index = elementIds.begin();
				index != elementIds.end();
				index++){
		const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(*index);
		I_ASSERT(elementInfoPtr != NULL);
		I_ASSERT(elementInfoPtr->elementPtr.IsValid());

		const icomp::IComponentStaticInfo& staticInfo = elementInfoPtr->elementPtr.GetPtr()->GetComponentStaticInfo();

		retVal += GetCompatibleSubcomponents(
					*index,
					staticInfo,
					interfaceInfo);
	}

	return retVal;
}


// reimplemented (TGuiObserverWrap)

void CAttributeEditorComp::OnGuiModelDetached()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(this);

		MainTab->setVisible(false);

		AttributeTree->clear();
		InterfacesTree->clear();
		ComponentsTree->clear();
	}

	BaseClass::OnGuiModelDetached();
}


// reimplmented (imod::IModelEditor)

void CAttributeEditorComp::UpdateEditor(int /*updateFlags*/)
{
	if (!IsGuiCreated()){
		return;
	}

	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();

	MainTab->setVisible(selectionInfoPtr != NULL);

	const icomp::IRegistryElement* elementPtr = GetRegistryElement();
	if ((selectionInfoPtr == NULL) || (elementPtr == NULL)){
		return;
	}

	AttributeTree->clear();
	InterfacesTree->clear();
	ComponentsTree->clear();

	bool hasExport = false;

	const icomp::IComponentStaticInfo& elementStaticInfo = elementPtr->GetComponentStaticInfo();
	const icomp::IComponentStaticInfo::AttributeInfos staticAttributes = elementStaticInfo.GetAttributeInfos();

	NameLabel->setText(selectionInfoPtr->GetSelectedElementName().c_str());
	IconLabel->setPixmap(selectionInfoPtr->GetSelectedElementIcon().pixmap(128));

	const icomp::CComponentAddress* addressPtr = selectionInfoPtr->GetSelectedElementAddress();
	if (addressPtr != NULL){
		PackageIdLabel->setText(addressPtr->GetPackageId().c_str());
		ComponentIdLabel->setText(addressPtr->GetComponentId().c_str());
		PackageIdLabel->setVisible(true);
		ComponentIdLabel->setVisible(true);
	}
	else{
		PackageIdLabel->setVisible(false);
		ComponentIdLabel->setVisible(false);
	}

	DescriptionLabel->setText(iqt::GetQString(elementStaticInfo.GetDescription()));
	KeywordsLabel->setText(iqt::GetQString(elementStaticInfo.GetKeywords()));

	for (int staticAttributeIndex = 0; staticAttributeIndex < staticAttributes.GetElementsCount(); staticAttributeIndex++){
		const std::string& attributeId = staticAttributes.GetKeyAt(staticAttributeIndex);
		const icomp::IAttributeStaticInfo* staticAttributeInfPtr = staticAttributes.GetValueAt(staticAttributeIndex);
		I_ASSERT(staticAttributeInfPtr != NULL);

		QTreeWidgetItem* attributeItemPtr = new QTreeWidgetItem();
		QTreeWidgetItem* exportItemPtr = new QTreeWidgetItem(attributeItemPtr);

		bool exportFlag;
		SetAttributeToItems(attributeId, *staticAttributeInfPtr, *attributeItemPtr, *exportItemPtr, &exportFlag);

		hasExport = hasExport || exportFlag;

		AttributeTree->addTopLevelItem(attributeItemPtr);
		attributeItemPtr->addChild(exportItemPtr);
	}

	AttributeTree->resizeColumnToContents(0);

	const icomp::IComponentStaticInfo::InterfaceExtractors extractors = elementStaticInfo.GetInterfaceExtractors();

	icomp::IRegistry::ExportedInterfacesMap interfacesMap;

	const icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
	if (registryPtr != NULL){
		interfacesMap = registryPtr->GetExportedInterfacesMap();
	}

	for (int extractorIndex = 0; extractorIndex < extractors.GetElementsCount(); extractorIndex++){
		const istd::CClassInfo& interfaceInfo = extractors.GetKeyAt(extractorIndex);
		QTreeWidgetItem* itemPtr = new QTreeWidgetItem();

		itemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
		itemPtr->setText(0, interfaceInfo.GetName().c_str());

		icomp::IRegistry::ExportedInterfacesMap::const_iterator foundExportIter = interfacesMap.find(interfaceInfo);
		bool isInterfaceExported = false;
		if (foundExportIter != interfacesMap.end()){
			isInterfaceExported = (foundExportIter->second == selectionInfoPtr->GetSelectedElementName());
		}

		itemPtr->setCheckState(0, isInterfaceExported? Qt::Checked: Qt::Unchecked);

		InterfacesTree->addTopLevelItem(itemPtr);
	}

	const std::string& elementId = selectionInfoPtr->GetSelectedElementName();
	QTreeWidgetItem* componentRootPtr = new QTreeWidgetItem();
	componentRootPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	componentRootPtr->setText(NameColumn, tr("this"));
	componentRootPtr->setData(ValueColumn, AttributeId, QString(elementId.c_str()));
	componentRootPtr->setData(ValueColumn, AttributeMining, ComponentExport);

	CreateComponentsTree(elementId, elementStaticInfo, *componentRootPtr);

	ComponentsTree->addTopLevelItem(componentRootPtr);
}


void CAttributeEditorComp::UpdateModel() const
{
}


// protected slots:

void CAttributeEditorComp::on_AttributeTree_itemSelectionChanged()
{
	const icomp::IAttributeStaticInfo* attributeStaticInfoPtr = NULL;

	QList<QTreeWidgetItem*> items = AttributeTree->selectedItems();
	if (items.count() > 0){
		QTreeWidgetItem* attributeItemPtr = items.at(0);

		std::string attributeId = attributeItemPtr->data(ValueColumn, AttributeId).toString().toStdString();
		attributeStaticInfoPtr = GetStaticAttributeInfo(attributeId);
	}

	if (m_attributeSelectionObserverCompPtr.IsValid()){
		m_attributeSelectionObserverCompPtr->OnAttributeSelected(attributeStaticInfoPtr);
	}
}


void CAttributeEditorComp::on_AttributeTree_itemChanged(QTreeWidgetItem* item, int column)
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker blocker(this);

	if (column == NameColumn){
		icomp::IRegistryElement* elementPtr = GetRegistryElement();
		if (elementPtr == NULL){
			return;
		}

		bool isEnabled = (item->checkState(NameColumn) == Qt::Checked);

		istd::CChangeNotifier notifier(elementPtr, istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);

		std::string attributeId = item->text(NameColumn).toStdString();
		const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementPtr->GetAttributeInfo(attributeId);
		if (isEnabled){
			elementPtr->InsertAttributeInfo(attributeId);
		}
		else{
			if (attributeInfoPtr == NULL){
				return;
			}

			const_cast<icomp::IRegistryElement::AttributeInfo*>(attributeInfoPtr)->attributePtr.Reset();
			if (attributeInfoPtr->exportId.empty()){
				elementPtr->RemoveAttribute(attributeId);
			}
		}
	}
}


void CAttributeEditorComp::on_InterfacesTree_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> items = InterfacesTree->selectedItems();
	if ((items.count() > 0) && m_quickHelpViewerCompPtr.IsValid()){
		QTreeWidgetItem* attributeItemPtr = items.at(0);

		istd::CClassInfo info(attributeItemPtr->text(0).toStdString());

		m_quickHelpViewerCompPtr->ShowHelp(info.GetName(), &info);
	}
}


void CAttributeEditorComp::on_InterfacesTree_itemChanged(QTreeWidgetItem* item, int column)
{
	I_ASSERT(item != NULL);

	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return;
	}
	icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	if (column == 0){
		QString interfaceName = item->text(column);

		const std::string& elementName = selectionInfoPtr->GetSelectedElementName();
		bool isSelected = (item->checkState(column) == Qt::Checked);
		registryPtr->SetElementInterfaceExported(elementName, istd::CClassInfo(interfaceName.toStdString()), isSelected);
	}
}


// protected methods

bool CAttributeEditorComp::SetAttributeToItems(
			const std::string& attributeId,
			const icomp::IAttributeStaticInfo& staticInfo,
			QTreeWidgetItem& attributeItem,
			QTreeWidgetItem& exportItem,
			bool* hasExportPtr)
{
	icomp::IRegistryElement* elementPtr = GetRegistryElement();
	if (elementPtr == NULL){
		return false;
	}

	bool isAttributeCorrect = true;

	const iser::ISerializable* attributePtr = NULL;

	attributeItem.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);

	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementPtr->GetAttributeInfo(attributeId);
	if ((attributeInfoPtr != NULL) && !attributeInfoPtr->exportId.empty()){
		attributeItem.setIcon(NameColumn, QIcon(":/Resources/Icons/Export.png"));
		if (hasExportPtr != NULL){
			*hasExportPtr = true;
		}
	}

	if ((attributeInfoPtr != NULL) && attributeInfoPtr->attributePtr.IsValid()){
		attributePtr = attributeInfoPtr->attributePtr.GetPtr();
		attributeItem.setCheckState(NameColumn, Qt::Checked);
	}
	else{
		attributePtr = staticInfo.GetAttributeDefaultValue();
		attributeItem.setCheckState(NameColumn, Qt::Unchecked);
	}

	QString attributeType;
	AttributeTypesMap::const_iterator foundTypeName = m_attributeTypesMap.find(staticInfo.GetAttributeType());
	if (foundTypeName != m_attributeTypesMap.end()){
		attributeType = foundTypeName->second;
		if (!attributeType.isEmpty() && !staticInfo.IsObligatory()){
			attributeType = tr("Optional %1").arg(attributeType);
		}
	}

	QString attributeDescription = staticInfo.GetAttributeDescription().c_str();

	QString toolTip = (attributeType.isEmpty() || attributeDescription.isEmpty())?
				attributeType + attributeDescription:
				tr("%1\nType: %2").arg(attributeDescription).arg(attributeType);

	QString attributeName = attributeId.c_str();
	attributeItem.setText(NameColumn, attributeName);
	attributeItem.setData(ValueColumn, AttributeId, attributeName);
	attributeItem.setToolTip(NameColumn, toolTip);
	attributeItem.setToolTip(ValueColumn, toolTip);

	exportItem.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	exportItem.setText(NameColumn, tr("<export>"));
	exportItem.setText(ValueColumn, (attributeInfoPtr != NULL)? attributeInfoPtr->exportId.c_str(): "");
	exportItem.setData(ValueColumn, AttributeId, attributeName);
	exportItem.setData(ValueColumn, AttributeMining, AttributeExport);
	exportItem.setToolTip(NameColumn, toolTip);
	exportItem.setToolTip(ValueColumn, toolTip);

	QString text;
	int meaning;

	if ((attributePtr != NULL) && DecodeAttribute(*attributePtr, text, meaning)){
		isAttributeCorrect =
					(meaning == Attribute) ||
					((attributeInfoPtr != NULL) && (attributeInfoPtr->attributePtr.IsValid() || !attributeInfoPtr->exportId.empty())) ||
					!staticInfo.IsObligatory();

		attributeItem.setBackgroundColor(0, isAttributeCorrect? Qt::white: Qt::red);
		attributeItem.setText(ValueColumn, text);
		attributeItem.setData(ValueColumn, AttributeMining, meaning);

		return true;
	}

	return false;
}


bool CAttributeEditorComp::DecodeAttribute(const iser::ISerializable& attribute, QString& text, int& meaning)
{
	const icomp::CBoolAttribute* boolAttribute = dynamic_cast<const icomp::CBoolAttribute*>(&attribute);
	if (boolAttribute != NULL){
		text = boolAttribute->GetValue() ? tr("true"): tr("false");
		meaning = Attribute;

		return true;
	}

	const icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<const icomp::CDoubleAttribute*>(&attribute);
	if (doubleAttribute != NULL){
		text = QString::number(doubleAttribute->GetValue());
		meaning = Attribute;

		return true;
	}

	const icomp::CIntAttribute* intAttribute = dynamic_cast<const icomp::CIntAttribute*>(&attribute);
	if (intAttribute != NULL){
		text = QString::number(intAttribute->GetValue());
		meaning = Attribute;

		return true;
	}

	const icomp::CStringAttribute* stringAttribute = dynamic_cast<const icomp::CStringAttribute*>(&attribute);
	if (stringAttribute != NULL){
		text = EncodeToEdit(iqt::GetQString(stringAttribute->GetValue()));
		meaning = Attribute;

		return true;
	}

	const icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<const icomp::CMultiStringAttribute*>(&attribute);
	if (stringListAttribute != NULL){
		text.clear();

		for (int index = 0; index < stringListAttribute->GetValuesCount(); index++){
			text += EncodeToEdit(iqt::GetQString(stringListAttribute->GetValueAt(index))) + ";";
		}
			
		meaning = MultipleAttribute;

		return true;
	}

	const icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<const icomp::CMultiIntAttribute*>(&attribute);
	if (intListAttribute != NULL){
		text.clear();

		for (int index = 0; index < intListAttribute->GetValuesCount(); index++){
			text += QString::number(intListAttribute->GetValueAt(index)) + ";";
		}

		meaning = MultipleAttribute;

		return true;
	}

	const icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<const icomp::CMultiDoubleAttribute*>(&attribute);
	if (doubleListAttribute != NULL){
		text.clear();

		for (int index = 0; index < doubleListAttribute->GetValuesCount(); index++){
			text += QString::number(doubleListAttribute->GetValueAt(index)) + ";";
		}

		meaning = MultipleAttribute;

		return true;
	}

	const icomp::TSingleAttribute<std::string>* idPtr = dynamic_cast<const icomp::TSingleAttribute<std::string>*>(&attribute);
	if (idPtr != NULL){		
		text = EncodeToEdit(idPtr->GetValue().c_str());

		meaning = Reference;

		return true;
	}

	const icomp::TMultiAttribute<std::string>* multiIdPtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(&attribute);
	if (multiIdPtr != NULL){
		QString dependecyString;

		int idsCount = multiIdPtr->GetValuesCount();
		for (int idIndex = 0; idIndex < idsCount; idIndex++){
			text += EncodeToEdit(iqt::GetQString(multiIdPtr->GetValueAt(idIndex)))  + ";";
		}

		meaning = MultipleReference;

		return true;
	}

	return false;
}


void CAttributeEditorComp::CreateComponentsTree(
			const std::string& elementId,
			const icomp::IComponentStaticInfo& elementStaticInfo,
			QTreeWidgetItem& rootItem) const
{
	const icomp::IComponentStaticInfo::Ids subcomponentIds = elementStaticInfo.GetSubcomponentIds();

	for (		icomp::IComponentStaticInfo::Ids::const_iterator subIter = subcomponentIds.begin();
				subIter != subcomponentIds.end();
				++subIter){
		const std::string& subcomponentId = *subIter;

		std::string fullId = elementId + "/" + subcomponentId;

		QTreeWidgetItem* itemPtr = new QTreeWidgetItem();

		itemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		itemPtr->setText(NameColumn, subcomponentId.c_str());
		itemPtr->setData(ValueColumn, AttributeId, QString(fullId.c_str()));
		itemPtr->setData(ValueColumn, AttributeMining, ComponentExport);

		const icomp::IComponentStaticInfo* subcomponentInfoPtr = elementStaticInfo.GetSubcomponentInfo(subcomponentId);
		if (subcomponentInfoPtr != NULL){
			CreateComponentsTree(fullId, *subcomponentInfoPtr, *itemPtr);
		}

		rootItem.addChild(itemPtr);
	}
}


QStringList CAttributeEditorComp::GetCompatibleSubcomponents(
			const std::string& elementId,
			const icomp::IComponentStaticInfo& elementStaticInfo,
			const istd::CClassInfo& interfaceInfo) const
{
	QStringList retVal;

	icomp::IComponentStaticInfo::InterfaceExtractors interfaceExtractors = elementStaticInfo.GetInterfaceExtractors();
	const icomp::IComponentStaticInfo::InterfaceExtractorPtr* extractorPtr = interfaceExtractors.FindElement(interfaceInfo);
	if (extractorPtr != NULL){
		retVal.push_back(iqt::GetQString(elementId.c_str()));
	}

	const icomp::IComponentStaticInfo::Ids subcomponentIds = elementStaticInfo.GetSubcomponentIds();

	for (		icomp::IComponentStaticInfo::Ids::const_iterator subIter = subcomponentIds.begin();
				subIter != subcomponentIds.end();
				++subIter){
		const std::string& subcomponentId = *subIter;

		const icomp::IComponentStaticInfo* subcomponentInfoPtr = elementStaticInfo.GetSubcomponentInfo(subcomponentId);
		if (subcomponentInfoPtr != NULL){
			retVal += GetCompatibleSubcomponents(elementId + "/" + subcomponentId, *subcomponentInfoPtr, interfaceInfo);
		}
	}

	return retVal;
}


// reimplemented (CGuiComponentBase)

void CAttributeEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	MainTab->setVisible(false);

	m_treeWidgetFilter.SetPtr(new iqtgui::CTreeWidgetFilter(AttributeTree));

	AttributeTree->setItemDelegate(&m_attributeItemDelegate);
	ComponentsTree->setItemDelegate(&m_attributeItemDelegate);
}


// static methods

QString CAttributeEditorComp::DecodeFromEdit(const QString& text)
{
	QString retVal = text;
	retVal.replace("\\:", ";");
	retVal.replace("\\\\", "\\");

	return retVal;
}


QString CAttributeEditorComp::EncodeToEdit(const QString& text)
{
	QString retVal = text;
	retVal.replace("\\", "\\\\");
	retVal.replace(";", "\\:");

	return retVal;
}


// members of nested class AttributeItemDelegate

CAttributeEditorComp::AttributeItemDelegate::AttributeItemDelegate(CAttributeEditorComp& parent)
:	m_parent(parent)
{
}


// reimplemented (QItemDelegate)

QSize CAttributeEditorComp::AttributeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QSize size = QItemDelegate::sizeHint(option, index);

	size.setHeight(20);

	return size;
}


QWidget* CAttributeEditorComp::AttributeItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() != ValueColumn){
		return NULL;
	}

	int propertyMining = index.data(AttributeMining).toInt();

	if (		(propertyMining == Reference) ||
				(propertyMining == MultipleReference) ||
				(propertyMining == SelectableAttribute)){
		QComboBox* comboEditor = new QComboBox(parent);
		QString text = index.data().toString();
		comboEditor->setEditable(true);
		comboEditor->setEditText(text);
		return comboEditor;
	} 

	if (propertyMining == Attribute){
		std::string attributeId = index.data(AttributeId).toString().toStdString();
		const icomp::IAttributeStaticInfo* attributeInfoPtr = m_parent.GetStaticAttributeInfo(attributeId);
		if (attributeInfoPtr != NULL && attributeInfoPtr->GetAttributeType().IsType<icomp::CBoolAttribute>()){
			QComboBox* comboEditor = new QComboBox(parent);
			comboEditor->addItem(tr("true"));
			comboEditor->addItem(tr("false"));
	
			return comboEditor;
		}
	} 

	return QItemDelegate::createEditor(parent, option, index);
}


void CAttributeEditorComp::AttributeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	if (index.column() != ValueColumn){
		QItemDelegate::setEditorData(editor, index);
	}

	icomp::IRegistryElement* elementPtr = m_parent.GetRegistryElement();
	I_ASSERT(elementPtr != NULL);

	int propertyMining = index.data(AttributeMining).toInt();

	std::string attributeName = index.data(AttributeId).toString().toStdString();

	if (propertyMining == ComponentExport){
		QString editorValue;
		const IElementSelectionInfo* selectionInfoPtr = m_parent.GetObjectPtr();
		if (selectionInfoPtr != NULL){
			const icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
			if (registryPtr != NULL){
				bool isFirst = true;
				icomp::IRegistry::ExportedComponentsMap exportedMap = registryPtr->GetExportedComponentsMap();
				for (		icomp::IRegistry::ExportedComponentsMap::const_iterator iter = exportedMap.begin();
							iter != exportedMap.end();
							++iter){
					if (iter->second == attributeName){
						if (!isFirst){
							editorValue + ";";
						}

						isFirst = false;
					}
				}
			}
		}

		editor->setProperty("text", QVariant(editorValue));
		return;
	}

	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = m_parent.GetRegistryAttribute(attributeName);
	if (attributeInfoPtr == NULL){
		attributeInfoPtr = elementPtr->InsertAttributeInfo(attributeName, (propertyMining != AttributeExport));
		I_ASSERT(attributeInfoPtr != NULL);
		if (attributeInfoPtr == NULL){
			return;
		}
	}

	if (propertyMining == AttributeExport){
		editor->setProperty("text", QVariant(attributeInfoPtr->exportId.c_str()));

		return;
	}

	iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
	if (attributePtr == NULL){
		return;
	}

	QComboBox* comboEditor = dynamic_cast<QComboBox*>(editor);

	if (		(propertyMining == Reference) ||
				(propertyMining == MultipleReference)){
		if (comboEditor == NULL){
			return;
		}

		const icomp::IAttributeStaticInfo* attributeStaticInfoPtr = m_parent.GetStaticAttributeInfo(attributeName);
		I_ASSERT(attributeStaticInfoPtr != NULL);

		const istd::CClassInfo& interfaceInfo = attributeStaticInfoPtr->GetRelatedInterfaceType();
		QStringList availableComponents = m_parent.GetCompatibleComponents(interfaceInfo);

		comboEditor->addItems(availableComponents);

		icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<icomp::CReferenceAttribute*>(attributePtr);
		icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr);
		icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<icomp::CFactoryAttribute*>(attributePtr);
		icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr);

		if (referenceAttributePtr != NULL){
			comboEditor->lineEdit()->setText(iqt::GetQString(referenceAttributePtr->GetValue()));
		}
		else if(factoryAttributePtr != NULL){
			comboEditor->lineEdit()->setText(iqt::GetQString(factoryAttributePtr->GetValue()));		
		}
		else if(multiReferenceAttributePtr != NULL){
			QString valuesString;
			for (int index = 0; index < multiReferenceAttributePtr->GetValuesCount(); index++){
				valuesString += iqt::GetQString(multiReferenceAttributePtr->GetValueAt(index)) + ";";
			}

			comboEditor->lineEdit()->setText(valuesString);
		}
		else if(multiFactoryAttributePtr != NULL){
			QString valuesString;
			for (int index = 0; index < multiFactoryAttributePtr->GetValuesCount(); index++){
				valuesString += iqt::GetQString(multiFactoryAttributePtr->GetValueAt(index)) + ";";
			}

			comboEditor->lineEdit()->setText(valuesString);
		}
	}
	else if (propertyMining == Attribute || propertyMining == MultipleAttribute){
		icomp::CIntAttribute* intAttribute = dynamic_cast<icomp::CIntAttribute*>(attributePtr);
		if (intAttribute != NULL){
			int value = intAttribute->GetValue();
			editor->setProperty("text", QVariant(value));
		}

		icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<icomp::CDoubleAttribute*>(attributePtr);
		if (doubleAttribute != NULL){
			double value = doubleAttribute->GetValue();
			editor->setProperty("text", QVariant(value));
		}

		icomp::CBoolAttribute* boolAttribute = dynamic_cast<icomp::CBoolAttribute*>(attributePtr);
		if (boolAttribute != NULL){
			bool value = boolAttribute->GetValue();
			comboEditor->setCurrentIndex(value? 0: 1);
		}

		icomp::CStringAttribute* stringAttribute = dynamic_cast<icomp::CStringAttribute*>(attributePtr);
		if (stringAttribute != NULL){
			istd::CString value = stringAttribute->GetValue();
			editor->setProperty("text", QVariant(iqt::GetQString(value)));
		}

		icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<icomp::CMultiStringAttribute*>(attributePtr);
		if (stringListAttribute != NULL){
			QString outputValue;
			for (int index = 0; index < stringListAttribute->GetValuesCount(); index++){
				outputValue += iqt::GetQString(stringListAttribute->GetValueAt(index)) + ";";
			}

			editor->setProperty("text", QVariant(outputValue));
		}

		icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<icomp::CMultiIntAttribute*>(attributePtr);
		if (intListAttribute != NULL){
			QString outputValue;
			for (int index = 0; index < intListAttribute->GetValuesCount(); index++){
				outputValue += QString("%1").arg(intListAttribute->GetValueAt(index)) + ";";
			}

			editor->setProperty("text", QVariant(outputValue));
		}

		icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<icomp::CMultiDoubleAttribute*>(attributePtr);
		if (doubleListAttribute != NULL){
			QString outputValue;
			for (int index = 0; index < doubleListAttribute->GetValuesCount(); index++){
				outputValue += QString("%1").arg(doubleListAttribute->GetValueAt(index)) + ";";
			}

			editor->setProperty("text", QVariant(outputValue));
		}
	}
}


void CAttributeEditorComp::AttributeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (index.column() != ValueColumn){
		QItemDelegate::setModelData(editor, model, index);
	}

	int propertyMining = index.data(AttributeMining).toInt();
	std::string attributeName = index.data(AttributeId).toString().toStdString();

	if (propertyMining == ComponentExport){
		const IElementSelectionInfo* selectionInfoPtr = m_parent.GetObjectPtr();
		if (selectionInfoPtr != NULL){
			istd::TChangeNotifier<icomp::IRegistry> registryPtr(
						selectionInfoPtr->GetSelectedRegistry(),
						istd::IChangeable::CF_MODEL | icomp::IRegistry::CF_COMPONENT_EXPORTED);
			if (registryPtr.IsValid()){
				std::string exportId = editor->property("text").toString().toStdString();

				icomp::IRegistry::ExportedComponentsMap exportedMap = registryPtr->GetExportedComponentsMap();
				for (icomp::IRegistry::ExportedComponentsMap::const_iterator iter = exportedMap.begin();
							iter != exportedMap.end();
							++iter){
					if (iter->second == attributeName){
						registryPtr->SetElementExported(iter->first, "");
					}
				}

				if (!exportId.empty()){
					registryPtr->SetElementExported(exportId, attributeName);
				}
			}
		}

		return;
	}

	istd::TChangeNotifier<icomp::IRegistryElement> elementPtr(m_parent.GetRegistryElement(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);
	if (!elementPtr.IsValid()){
		return;
	}

	icomp::IRegistryElement::AttributeInfo* attributeInfoPtr =
				const_cast<icomp::IRegistryElement::AttributeInfo*>(elementPtr->GetAttributeInfo(attributeName));
	I_ASSERT(attributeInfoPtr != NULL);

	if (propertyMining == AttributeExport){
		QString newValue = editor->property("text").toString();
		attributeInfoPtr->exportId = newValue.toStdString();

		return;
	}

	if (!attributeInfoPtr->attributePtr.IsValid()){
		icomp::IRegistryElement* elementPtr = m_parent.GetRegistryElement();
		I_ASSERT(elementPtr != NULL);

		attributeInfoPtr->attributePtr.SetPtr(elementPtr->CreateAttribute(attributeName));
	}

	iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
	if (attributePtr == NULL){
		return;
	}

	IElementSelectionInfo* selectionInfoPtr = m_parent.GetObjectPtr();
	I_ASSERT(selectionInfoPtr != NULL);

	QComboBox* comboEditor = dynamic_cast<QComboBox*>(editor);

	// set single reference data
	if (propertyMining == Reference){
		I_ASSERT(comboEditor != NULL);
		QString referenceValue = comboEditor->currentText();

		icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<icomp::CReferenceAttribute*>(attributePtr);
		icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<icomp::CFactoryAttribute*>(attributePtr);
		if (referenceAttributePtr != NULL){
			referenceAttributePtr->SetValue(DecodeFromEdit(referenceValue).toStdString());
		}
		else if (factoryAttributePtr != NULL){
			factoryAttributePtr->SetValue(DecodeFromEdit(referenceValue).toStdString());
		}
	}
	// set multiple reference data
	else if (propertyMining == MultipleReference){
		I_ASSERT(comboEditor != NULL);
		QString string = comboEditor->lineEdit()->text();
		QStringList references = string.split(';',QString::SkipEmptyParts); 

		icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr);
		icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr);

		if (multiReferenceAttributePtr != NULL){
			multiReferenceAttributePtr->Reset();
			for (int index = 0; index < references.count(); index++){
				multiReferenceAttributePtr->InsertValue(DecodeFromEdit(references.at(index)).toStdString());
			}
		}
		else if (multiFactoryAttributePtr != NULL){
			multiFactoryAttributePtr->Reset();
			for (int index = 0; index < references.count(); index++){
				multiFactoryAttributePtr->InsertValue(DecodeFromEdit(references.at(index)).toStdString());
			}
		}
	}
	// set attribute data:
	else if (	(propertyMining == Attribute) ||
				(propertyMining == MultipleAttribute)){	
		icomp::CIntAttribute* intAttribute = dynamic_cast<icomp::CIntAttribute*>(attributePtr);
		if (intAttribute != NULL){
			intAttribute->SetValue(editor->property("text").toInt());
		}

		icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<icomp::CDoubleAttribute*>(attributePtr);
		if (doubleAttribute != NULL){
			doubleAttribute->SetValue(editor->property("text").toDouble());
		}

		icomp::CBoolAttribute* boolAttribute = dynamic_cast<icomp::CBoolAttribute*>(attributePtr);
		if (boolAttribute != NULL){
			I_ASSERT(comboEditor != NULL);

			int index = comboEditor->currentIndex();
			boolAttribute->SetValue((index == 0) ? true: false);
		}

		icomp::CStringAttribute* stringAttribute = dynamic_cast<icomp::CStringAttribute*>(attributePtr);
		if (stringAttribute != NULL){
			QString newValue = editor->property("text").toString();
			stringAttribute->SetValue(iqt::GetCString(DecodeFromEdit(newValue)));
		}

		icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<icomp::CMultiStringAttribute*>(attributePtr);
		if (stringListAttribute != NULL){
			QString newValue = editor->property("text").toString();
			QStringList values = newValue.split(';', QString::SkipEmptyParts);

			stringListAttribute->Reset();
			for (int index = 0; index < values.count(); index++){
				stringListAttribute->InsertValue(DecodeFromEdit(values.at(index)).toStdString());
			}
		}

		icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<icomp::CMultiIntAttribute*>(attributePtr);
		if (intListAttribute != NULL){
			QString valueString = editor->property("text").toString();
			QStringList values = valueString.split(';', QString::SkipEmptyParts);

			intListAttribute->Reset();
			for (int index = 0; index < values.count(); index++){
				intListAttribute->InsertValue(values.at(index).toInt());
			}
		}

		icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<icomp::CMultiDoubleAttribute*>(attributePtr);
		if (doubleListAttribute != NULL){
			QString valueString = editor->property("text").toString();
			QStringList values = valueString.split(';', QString::SkipEmptyParts);

			doubleListAttribute->Reset();
			for (int index = 0; index < values.count(); index++){
				doubleListAttribute->InsertValue(values.at(index).toDouble());
			}
		}
	}
}


void CAttributeEditorComp::AttributeItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QItemDelegate::paint(painter, option, index);

	QRect rect = option.rect;
	painter->setPen(QPen(Qt::darkGray, 0, Qt::SolidLine));
	if (index.column() == NameColumn){
		painter->drawLine(rect.topRight(), rect.bottomRight());
	}

	painter->drawLine(rect.bottomLeft(), rect.bottomRight());
}


} // namespace icmpstr


