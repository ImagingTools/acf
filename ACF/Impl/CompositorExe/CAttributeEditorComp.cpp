#include <QHeaderView>
#include <QListWidget>
#include <QLineEdit>

#include "icomp/CRegistryElement.h" 

#include "CAttributeEditorComp.h"
#include "CRegistryViewComp.h"


// public methods
	
CAttributeEditorComp::CAttributeEditorComp()
:	m_attributeItemDelegate(*this)
{
	m_attributeTypesMap[typeid(icomp::CBoolAttribute).name()] = tr("Boolean");
	m_attributeTypesMap[typeid(icomp::CDoubleAttribute).name()] = tr("Real number");
	m_attributeTypesMap[typeid(icomp::CIntAttribute).name()] = tr("Integer number");
	m_attributeTypesMap[typeid(icomp::CStringAttribute).name()] = tr("String");
	m_attributeTypesMap[typeid(icomp::CMultiBoolAttribute).name()] = tr("Boolean list");
	m_attributeTypesMap[typeid(icomp::CMultiDoubleAttribute).name()] = tr("Real number list");
	m_attributeTypesMap[typeid(icomp::CMultiIntAttribute).name()] = tr("Integer number list");
	m_attributeTypesMap[typeid(icomp::CMultiStringAttribute).name()] = tr("String list");
	m_attributeTypesMap[typeid(icomp::CReferenceAttribute).name()] = tr("Reference");
	m_attributeTypesMap[typeid(icomp::CMultiReferenceAttribute).name()] = tr("Multiple reference");
	m_attributeTypesMap[typeid(icomp::CFactoryAttribute).name()] = tr("Factory");
	m_attributeTypesMap[typeid(icomp::CMultiFactoryAttribute).name()] = tr("Multiple factory");
}


const icomp::IRegistryElement::AttributeInfo* CAttributeEditorComp::GetRegistryAttribute(const QString& attributeName) const
{
	const icomp::IRegistryElement* registryElementPtr = GetObjectPtr();
	if (registryElementPtr == NULL){
		return NULL;
	}

	return registryElementPtr->GetAttributeInfo(attributeName.toStdString());
}


const icomp::IAttributeStaticInfo* CAttributeEditorComp::GetStaticAttributeInfo(const QString& attributeId) const
{
	const icomp::IRegistryElement* registryElementPtr = GetObjectPtr();
	if (registryElementPtr == NULL){
		return NULL;
	}

	const icomp::IComponentStaticInfo& elementStaticInfo = registryElementPtr->GetComponentStaticInfo();
	const icomp::IComponentStaticInfo::AttributeInfos& staticAttributes = elementStaticInfo.GetAttributeInfos();

	const icomp::IComponentStaticInfo::AttributeInfos::ValueType* attrInfoPtr2 =
				staticAttributes.FindElement(attributeId.toStdString());

	if (attrInfoPtr2 != NULL){
		return *attrInfoPtr2;
	}
	else{
		return NULL;
	}
}

	
QStringList CAttributeEditorComp::GetAvailableComponents(const QString& interfaceId) const
{
	QStringList availableComponents;

	icomp::CRegistryElement* elementPtr = dynamic_cast<icomp::CRegistryElement*>(GetObjectPtr());
	if (elementPtr != NULL){
		icomp::IRegistry* registryPtr = dynamic_cast<icomp::IRegistry*>(elementPtr->GetSlavePtr());
		if (registryPtr != NULL){
			icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
			for (		icomp::IRegistry::Ids::const_iterator index = elementIds.begin();
						index != elementIds.end();
						index++){

				const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(*index);
				I_ASSERT(elementInfoPtr != NULL);
				I_ASSERT(elementInfoPtr->elementPtr.IsValid());

				const icomp::IComponentStaticInfo& staticInfo = elementInfoPtr->elementPtr.GetPtr()->GetComponentStaticInfo();
				icomp::IComponentStaticInfo::InterfaceExtractors interfaceExtractors = staticInfo.GetInterfaceExtractors();
				const icomp::IComponentStaticInfo::InterfaceExtractorPtr* extractorPtr = interfaceExtractors.FindElement(interfaceId.toStdString().c_str());
				if (extractorPtr != NULL){
					availableComponents.push_back(iqt::GetQString(*index));
				}
			}
		}
	}

	return availableComponents;
}


// reimplemented (TGuiObserverWrap)

void CAttributeEditorComp::OnGuiModelDetached()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(this);

		AttributeTree->clear();
		InterfacesTree->clear();
		ExportTree->clear();
	}

	BaseClass::OnGuiModelDetached();
}


// reimplmented (imod::IModelEditor)

void CAttributeEditorComp::UpdateEditor()
{
	if (!IsGuiCreated()){
		return;
	}

	icomp::IRegistryElement* registryElementPtr = GetObjectPtr();
	if (registryElementPtr == NULL){
		return;
	}

	AttributeTree->clear();
	InterfacesTree->clear();
	ExportTree->clear();

	bool hasExport = false;

	const icomp::IComponentStaticInfo& elementStaticInfo = registryElementPtr->GetComponentStaticInfo();
	const icomp::IComponentStaticInfo::AttributeInfos staticAttributes = elementStaticInfo.GetAttributeInfos();

	for (int staticAttributeIndex = 0; staticAttributeIndex < staticAttributes.GetElementsCount(); staticAttributeIndex++){
		const std::string& attributeId = staticAttributes.GetKeyAt(staticAttributeIndex);
		const icomp::IAttributeStaticInfo* staticAttributeInfPtr = staticAttributes.GetValueAt(staticAttributeIndex);
		I_ASSERT(staticAttributeInfPtr != NULL);

		const iser::ISerializable* attributePtr = NULL;

		QTreeWidgetItem* attributeItemPtr = new QTreeWidgetItem();
		QTreeWidgetItem* exportItemPtr = new QTreeWidgetItem();

		bool exportFlag;
		SetAttributeToItems(attributeId, *staticAttributeInfPtr, *attributeItemPtr, *exportItemPtr, &exportFlag);

		hasExport = hasExport || exportFlag;

		AttributeTree->addTopLevelItem(attributeItemPtr);
		ExportTree->addTopLevelItem(exportItemPtr);
	}

	MainTab->setTabIcon(1, hasExport? QIcon(":/Resources/Icons/Export.png"): QIcon());

	AttributeTree->resizeColumnToContents(0);

	const icomp::IComponentStaticInfo::InterfaceExtractors extractors = elementStaticInfo.GetInterfaceExtractors();

	for (int extractorIndex = 0; extractorIndex < extractors.GetElementsCount(); extractorIndex++){
		const std::string& interfaceId = extractors.GetKeyAt(extractorIndex);
		QTreeWidgetItem* itemPtr = new QTreeWidgetItem();

		itemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		itemPtr->setText(0, interfaceId.c_str());

		InterfacesTree->addTopLevelItem(itemPtr);
	}
}


void CAttributeEditorComp::UpdateModel() const
{
}


// protected slots:

void CAttributeEditorComp::on_AttributeTree_itemSelectionChanged()
{
	AttributeInfoBox->hide();
	const icomp::IAttributeStaticInfo* attributeStaticInfoPtr = NULL;

	QList<QTreeWidgetItem*> items = AttributeTree->selectedItems();
	if (items.count() > 0){
		AttributeType->clear();
		AttributeDescription->clear();

		QTreeWidgetItem* attributeItemPtr = items.at(0);

		QString attributeId = attributeItemPtr->data(ValueColumn, AttributeId).toString();
		attributeStaticInfoPtr = GetStaticAttributeInfo(attributeId);
		if (attributeStaticInfoPtr != NULL){		
			AttributeTypesMap::const_iterator foundTypeName = m_attributeTypesMap.find(attributeStaticInfoPtr->GetAttributeType().name());
			if (foundTypeName != m_attributeTypesMap.end()){
				QString attributeType = foundTypeName->second;
				if (!attributeType.isEmpty()){
					if (!attributeStaticInfoPtr->IsObligatory()){
						attributeType += QString(tr(" (Optional)"));
					}
				}

				AttributeType->setText(attributeType);
			}

			QString attributeDescription = attributeStaticInfoPtr->GetAttributeDescription().c_str();
			AttributeDescription->setText(attributeDescription);

			AttributeInfoBox->show();
		}
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
		icomp::IRegistryElement* registryElementPtr = GetObjectPtr();
		if (registryElementPtr == NULL){
			return;
		}

		bool isEnabled = (item->checkState(NameColumn) == Qt::Checked);

		std::string attributeId = item->text(NameColumn).toStdString();
		const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = registryElementPtr->GetAttributeInfo(attributeId);
		if (isEnabled || (attributeInfoPtr == NULL)){
			return;
		}

		istd::CChangeNotifier notifier(registryElementPtr);

		const_cast<icomp::IRegistryElement::AttributeInfo*>(attributeInfoPtr)->attributePtr.Reset();
		if (attributeInfoPtr->exportId.empty()){
			registryElementPtr->RemoveAttribute(attributeId);
		}
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
	icomp::IRegistryElement* registryElementPtr = GetObjectPtr();
	if (registryElementPtr == NULL){
		return false;
	}

	bool isAttributeCorrect = true;

	const iser::ISerializable* attributePtr = NULL;

	attributeItem.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);

	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = registryElementPtr->GetAttributeInfo(attributeId);
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

	QString attributeName = attributeId.c_str();
	attributeItem.setText(NameColumn, attributeName);
	attributeItem.setData(ValueColumn, AttributeId, attributeName);

	exportItem.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	exportItem.setText(NameColumn, attributeName);
	exportItem.setText(ValueColumn, (attributeInfoPtr != NULL)? attributeInfoPtr->exportId.c_str(): "");
	exportItem.setData(ValueColumn, AttributeId, attributeName);
	exportItem.setData(ValueColumn, AttributeMining, Export);

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


// reimplemented (CGuiComponentBase)

void CAttributeEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_treeWidgetFilter.SetPtr(new iqt::CTreeWidgetFilter(AttributeTree));

	AttributeTree->setItemDelegate(&m_attributeItemDelegate);
	ExportTree->setItemDelegate(&m_attributeItemDelegate);

	AttributeInfoBox->hide();
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
		QString attributeId = index.data(AttributeId).toString();
		const icomp::IAttributeStaticInfo* attributeInfoPtr = m_parent.GetStaticAttributeInfo(attributeId);
		if (attributeInfoPtr != NULL && attributeInfoPtr->GetAttributeType() == typeid(icomp::CBoolAttribute)){
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

	icomp::IRegistryElement* elementPtr = m_parent.GetObjectPtr();
	I_ASSERT(elementPtr != NULL);

	int propertyMining = index.data(AttributeMining).toInt();

	QString attributeName = index.data(AttributeId).toString();

	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = m_parent.GetRegistryAttribute(attributeName);
	if (attributeInfoPtr == NULL){
		attributeInfoPtr = elementPtr->InsertAttributeInfo(attributeName.toStdString(), (propertyMining != Export));
		I_ASSERT(attributeInfoPtr != NULL);
		if (attributeInfoPtr == NULL){
			return;
		}
	}

	if (propertyMining == Export){
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

		QString interfaceId = attributeStaticInfoPtr->GetRelatedInterfaceType().name();
		QStringList availableComponents = m_parent.GetAvailableComponents(interfaceId);

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

	QComboBox* comboEditor = dynamic_cast<QComboBox*>(editor);

	QString attributeName = index.data(AttributeId).toString();
	icomp::IRegistryElement::AttributeInfo* attributeInfoPtr =
				const_cast<icomp::IRegistryElement::AttributeInfo*>(m_parent.GetRegistryAttribute(attributeName));
	I_ASSERT(attributeInfoPtr != NULL);

	int propertyMining = index.data(AttributeMining).toInt();

	if (propertyMining == Export){
		istd::CChangeNotifier notifier(m_parent.GetObjectPtr());

		QString newValue = editor->property("text").toString();
		attributeInfoPtr->exportId = newValue.toStdString();
	}

	if (!attributeInfoPtr->attributePtr.IsValid()){
		icomp::IRegistryElement* elementPtr = m_parent.GetObjectPtr();
		I_ASSERT(elementPtr != NULL);

		attributeInfoPtr->attributePtr.SetPtr(elementPtr->CreateAttribute(attributeName.toStdString()));
	}

	iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
	if (attributePtr == NULL){
		return;
	}

	istd::CChangeNotifier notifier(m_parent.GetObjectPtr());

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

			intListAttribute->Reset();
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

