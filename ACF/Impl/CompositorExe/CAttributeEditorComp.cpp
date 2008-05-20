#include <QHeaderView>
#include <QListWidget>
#include <QLineEdit>

#include "icomp/CRegistryElement.h" 

#include "CAttributeEditorComp.h"
#include "CRegistryViewComp.h"


// public methods
	
CAttributeEditorComp::CAttributeEditorComp()
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
	const icomp::IComponentStaticInfo::AttributeInfos staticAttributes = elementStaticInfo.GetAttributeInfos();

	return *staticAttributes.FindElement(attributeId.toStdString());
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
	AttributeTree->clear();

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

	const icomp::IComponentStaticInfo& elementStaticInfo = registryElementPtr->GetComponentStaticInfo();
	const icomp::IComponentStaticInfo::AttributeInfos staticAttributes = elementStaticInfo.GetAttributeInfos();
	
	for (int staticAttributeIndex = 0; staticAttributeIndex < staticAttributes.GetElementsCount(); staticAttributeIndex++){
		const icomp::IAttributeStaticInfo* staticAttributeInfPtr = staticAttributes.GetValueAt(staticAttributeIndex);
		I_ASSERT(staticAttributeInfPtr != NULL);

		std::string attributeId = staticAttributeInfPtr->GetAttributeId();
		const iser::ISerializable* attributePtr = NULL;

		const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = registryElementPtr->GetAttributeInfo(attributeId);
		if (attributeInfoPtr != NULL){
			attributePtr = attributeInfoPtr->attributePtr.GetPtr();
		}
		else{
			attributePtr = staticAttributeInfPtr->GetAttributeDefaultValue();
		}

		QString attributeName = attributeId.c_str();
		QTreeWidgetItem* attributeItemPtr = new QTreeWidgetItem();
		attributeItemPtr->setText(NameColumn, attributeName);
		attributeItemPtr->setData(ValueColumn, AttributeId, attributeName);
		attributeItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		AttributeTree->addTopLevelItem(attributeItemPtr);

		const icomp::CBoolAttribute* boolAttribute = dynamic_cast<const icomp::CBoolAttribute*>(attributePtr);
		if (boolAttribute != NULL){
			attributeItemPtr->setText(ValueColumn, boolAttribute->GetValue() ? tr("true"): tr("false"));
			attributeItemPtr->setData(ValueColumn, AttributeMining, Attribute);

			continue;
		}

		const icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<const icomp::CDoubleAttribute*>(attributePtr);
		if (doubleAttribute != NULL){
			attributeItemPtr->setText(ValueColumn, QString("%1").arg(doubleAttribute->GetValue()));
			attributeItemPtr->setData(ValueColumn, AttributeMining, Attribute);

			continue;
		}

		const icomp::CIntAttribute* intAttribute = dynamic_cast<const icomp::CIntAttribute*>(attributePtr);
		if (intAttribute != NULL){
			attributeItemPtr->setText(ValueColumn, QString("%1").arg(intAttribute->GetValue()));
			attributeItemPtr->setData(ValueColumn, AttributeMining, Attribute);

			continue;
		}

		const icomp::CStringAttribute* stringAttribute = dynamic_cast<const icomp::CStringAttribute*>(attributePtr);
		if (stringAttribute != NULL){
			attributeItemPtr->setText(ValueColumn, iqt::GetQString(stringAttribute->GetValue()));
			attributeItemPtr->setData(ValueColumn, AttributeMining, Attribute);		

			continue;
		}

		const icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<const icomp::CMultiStringAttribute*>(attributePtr);
		if (stringListAttribute != NULL){
			QString stringList;

			for (int index = 0; index < stringListAttribute->GetValuesCount(); index++){
				stringList += iqt::GetQString(stringListAttribute->GetValueAt(index)) + ";";
			}	
				
			attributeItemPtr->setText(ValueColumn, stringList);
			attributeItemPtr->setData(ValueColumn, AttributeMining, MultipleAttribute);		

			continue;
		}

		const icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<const icomp::CMultiIntAttribute*>(attributePtr);
		if (intListAttribute != NULL){
			QString stringList;

			for (int index = 0; index < intListAttribute->GetValuesCount(); index++){
				stringList += QString("%1").arg(intListAttribute->GetValueAt(index)) + ";";
			}	

			attributeItemPtr->setText(ValueColumn, stringList);
			attributeItemPtr->setData(ValueColumn, AttributeMining, MultipleAttribute);		

			continue;
		}

		const icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<const icomp::CMultiDoubleAttribute*>(attributePtr);
		if (doubleListAttribute != NULL){
			QString stringList;

			for (int index = 0; index < doubleListAttribute->GetValuesCount(); index++){
				stringList += QString("%1").arg(doubleListAttribute->GetValueAt(index)) + ";";
			}	

			attributeItemPtr->setText(ValueColumn, stringList);
			attributeItemPtr->setData(ValueColumn, AttributeMining, MultipleAttribute);		

			continue;
		}

		const icomp::CReferenceAttribute* referencePtr = dynamic_cast<const icomp::CReferenceAttribute*>(attributePtr);
		if (referencePtr != NULL){		
			attributeItemPtr->setText(ValueColumn, iqt::GetQString(referencePtr->GetValue()));
			attributeItemPtr->setData(ValueColumn, AttributeId, attributeName);
			attributeItemPtr->setData(ValueColumn, AttributeMining, Reference);

			continue;
		}
			
		const icomp::CMultiReferenceAttribute* multiReferencePtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(attributePtr);
		if (multiReferencePtr != NULL){
			QString dependecyString;
			for (int referenceIndex = 0; referenceIndex < multiReferencePtr->GetValuesCount(); referenceIndex++){
				QString referenceName = iqt::GetQString(multiReferencePtr->GetValueAt(referenceIndex));
				dependecyString += referenceName  + ";";
			}

			attributeItemPtr->setText(ValueColumn, dependecyString);
			attributeItemPtr->setData(ValueColumn, AttributeMining, MultipleReference);

			continue;
		}

		const icomp::CFactoryAttribute* factoryPtr = dynamic_cast<const icomp::CFactoryAttribute*>(attributePtr);
		if (factoryPtr != NULL){		
			attributeItemPtr->setText(ValueColumn, iqt::GetQString(factoryPtr->GetValue()));
			attributeItemPtr->setData(ValueColumn, AttributeId, attributeName);
			attributeItemPtr->setData(ValueColumn, AttributeMining, Reference);

			continue;
		}
			
		const icomp::CMultiFactoryAttribute* multiFactoryPtr = dynamic_cast<const icomp::CMultiFactoryAttribute*>(attributePtr);
		if (multiFactoryPtr != NULL){
			QString dependecyString;
			for (int referenceIndex = 0; referenceIndex < multiFactoryPtr->GetValuesCount(); referenceIndex++){
				QString referenceName = iqt::GetQString(multiFactoryPtr->GetValueAt(referenceIndex));
				dependecyString += referenceName  + ";";
			}

			attributeItemPtr->setText(ValueColumn, dependecyString);
			attributeItemPtr->setData(ValueColumn, AttributeMining, MultipleReference);

			continue;
		}
	}

	AttributeTree->resizeColumnToContents(0);
}


void CAttributeEditorComp::UpdateModel() const
{
}


// protected slots:

void CAttributeEditorComp::OnItemSelected()
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


// protected methods

// reimplemented (CGuiComponentBase)

void CAttributeEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr);

	AttributeTree->header()->setStretchLastSection(true);

	AttributeTree->setItemDelegate(new AttributeItemDelegate(*this));

	connect(AttributeTree, 
		SIGNAL(itemSelectionChanged()), 
		this, 
		SLOT(OnItemSelected()));

	AttributeInfoBox->hide();
	AttributeTree->setFrameShape(QFrame::NoFrame);
	ExportTree->setFrameShape(QFrame::NoFrame);
}


// members of nested class AttributeItemDelegate

CAttributeEditorComp::AttributeItemDelegate::AttributeItemDelegate(CAttributeEditorComp& parent)
	:QItemDelegate(),
	m_parent(parent)
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

	if (propertyMining == Reference || propertyMining == MultipleReference || propertyMining == SelectableAttribute){
		QComboBox* comboEditor = new QComboBox(parent);
		comboEditor->setEditable(true);
		return comboEditor;
	} 

	if (propertyMining == Attribute){
		QString attributeId = index.data(AttributeId).toString();
		const icomp::IAttributeStaticInfo* attributeInfoPtr = m_parent.GetStaticAttributeInfo(attributeId);
		if (attributeInfoPtr != NULL && attributeInfoPtr->GetAttributeType() == typeid(icomp::CBoolAttribute)){
			QComboBox* comboEditor = new QComboBox(parent);
			comboEditor->addItem("true");
			comboEditor->addItem("false");
	
			return comboEditor;
		}
	} 

	return QItemDelegate::createEditor(parent, option, index);
}


void CAttributeEditorComp::AttributeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index ) const
{
	QComboBox* comboEditor = dynamic_cast<QComboBox*>(editor);
	QString attributeName = index.data(AttributeId).toString();
	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = m_parent.GetRegistryAttribute(attributeName);
	if (attributeInfoPtr == NULL){
		editor->setProperty("text", QVariant(index.data()));
		return;
	}

	iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
	if (attributePtr == NULL){
		return;
	}

	int propertyMining = index.data(AttributeMining).toInt();

	if (		index.column() == ValueColumn && comboEditor != NULL && 
				(propertyMining == Reference || propertyMining == MultipleReference)){

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
		return;
	}
	else if (index.column() == ValueColumn && propertyMining == Attribute){
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
			comboEditor->setCurrentIndex(value ? 0 : 1);
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

		return;
	}
	else if(index.column() == ValueColumn && propertyMining == MultipleReference){
//		QStringList components = m_parent.m_componentViewPtr->componentsForReference(dependecySource);
//		comboEditor->addItems(m_parent.m_componentViewPtr->componentsForReference(dependecySource));
//		comboEditor->lineEdit()->setText(index.data().toString());

		return;
	}	

	QItemDelegate::setEditorData(editor, index);
}


void CAttributeEditorComp::AttributeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	icomp::IRegistryElement* elementPtr = m_parent.GetObjectPtr();
	I_ASSERT(elementPtr != NULL);

	QComboBox* comboEditor = dynamic_cast<QComboBox*>(editor);

	QString attributeName = index.data(AttributeId).toString();
	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = m_parent.GetRegistryAttribute(attributeName);
	if (attributeInfoPtr == NULL){
		attributeInfoPtr = elementPtr->InsertAttributeInfo(attributeName.toStdString());
	}

	iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
	if (attributePtr == NULL){
		return;
	}

	int propertyMining = index.data(AttributeMining).toInt();
	
	// set single reference data
	if (index.column() == ValueColumn && propertyMining == Reference){
		I_ASSERT(comboEditor != NULL);
		QString referenceValue = comboEditor->currentText();

		icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<icomp::CReferenceAttribute*>(attributePtr);
		icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<icomp::CFactoryAttribute*>(attributePtr);
		if (referenceAttributePtr != NULL){
			referenceAttributePtr->SetValue(referenceValue.toStdString());
		}
		else if (factoryAttributePtr != NULL){
			factoryAttributePtr->SetValue(referenceValue.toStdString());
		}
	}
	// set multiple reference data
	else if (index.column() == ValueColumn && propertyMining == MultipleReference){
		I_ASSERT(comboEditor != NULL);
		QString string = comboEditor->lineEdit()->text();
		QStringList references = string.split(';',QString::SkipEmptyParts); 

		icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr);
		icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr);

		if (multiReferenceAttributePtr != NULL){
			multiReferenceAttributePtr->Reset();
			for (int index = 0; index < references.count(); index++){
				multiReferenceAttributePtr->InsertValue(references.at(index).toStdString());
			}
		}
		else if (multiFactoryAttributePtr != NULL){
			multiFactoryAttributePtr->Reset();
			for (int index = 0; index < references.count(); index++){
				multiFactoryAttributePtr->InsertValue(references.at(index).toStdString());
			}
		}
	}
	// set attribute data:
	else if (index.column() == ValueColumn && propertyMining == Attribute || propertyMining == MultipleAttribute){	
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
			stringAttribute->SetValue(iqt::GetCString(newValue));
		}

		icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<icomp::CMultiStringAttribute*>(attributePtr);
		if (stringListAttribute != NULL){
			QString newValue = editor->property("text").toString();
			QStringList values = newValue.split(';', QString::SkipEmptyParts);

			stringListAttribute->Reset();
			for (int index = 0; index < values.count(); index++){
				stringListAttribute->InsertValue(values.at(index).toStdString());
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

	istd::TChangeNotifier<icomp::IRegistryElement> changePtr(elementPtr);

	QItemDelegate::setModelData(editor, model, index);
}


void CAttributeEditorComp::AttributeItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();
	painter->translate(5, 0);
	QItemDelegate::paint(painter, option, index);
	painter->restore();

	painter->save();

	QRect rect = option.rect;
	painter->setPen(QPen(Qt::darkGray, 0, Qt::DotLine));
	painter->drawLine(rect.topLeft(), rect.bottomLeft());
	if (index.column() == ValueColumn){
		painter->drawLine(rect.topRight(), rect.bottomRight());
	}

	painter->setPen(Qt::darkGray);
	painter->drawLine(rect.bottomLeft(), rect.bottomRight());
	painter->restore();

	if (option.state & QStyle::State_Selected){
		QRect updateRect = option.rect;
		updateRect.setWidth(5);
		updateRect.setHeight(updateRect.height() - 1);
		if (option.state & QStyle::State_Active){
			painter->fillRect(updateRect, option.palette.highlight());
		}
		else{
			painter->fillRect(updateRect, option.palette.window());
		}
	}
}

