#include <QHeaderView>
#include <QListWidget>
#include <QLineEdit>

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

		// property name item
		QString attributeName = attributeId.c_str();
		QTreeWidgetItem* attributeItemPtr = new QTreeWidgetItem();
		attributeItemPtr->setText(NameColumn, attributeName);
		attributeItemPtr->setData(ValueColumn, AttributeId, attributeName);
		attributeItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		AttributeTree->addTopLevelItem(attributeItemPtr);

		const icomp::CBoolAttribute* boolAttribute = dynamic_cast<const icomp::CBoolAttribute*>(attributePtr);
		if (boolAttribute != NULL){
			attributeItemPtr->setFlags(attributeItemPtr->flags() | Qt::ItemIsUserCheckable);
			attributeItemPtr->setCheckState(ValueColumn, boolAttribute->GetValue() ? Qt::Checked : Qt::Unchecked);
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

		const icomp::TMultiAttribute<istd::CString>* stringListAttribute = dynamic_cast<const icomp::TMultiAttribute<istd::CString>*>(attributePtr);
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
			attributeItemPtr->setData(ValueColumn, AttributeMining, Dependency);

			continue;
		}
			
		const icomp::CMultiReferenceAttribute* multiReferencePtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(attributePtr);
		if (multiReferencePtr != NULL){
			QString dependecyString;
			for (int dependencyIndex = 0; dependencyIndex < multiReferencePtr->GetValuesCount(); dependencyIndex++){
				QString dependencyName = iqt::GetQString(multiReferencePtr->GetValueAt(dependencyIndex));
				dependecyString += dependencyName  + ";";
			}

			attributeItemPtr->setText(ValueColumn, dependecyString);
			attributeItemPtr->setData(ValueColumn, AttributeMining, MultipleDependency);

			continue;
		}

		const icomp::CFactoryAttribute* factoryPtr = dynamic_cast<const icomp::CFactoryAttribute*>(attributePtr);
		if (factoryPtr != NULL){		
			attributeItemPtr->setText(ValueColumn, iqt::GetQString(factoryPtr->GetValue()));
			attributeItemPtr->setData(ValueColumn, AttributeId, attributeName);
			attributeItemPtr->setData(ValueColumn, AttributeMining, Dependency);

			continue;
		}
			
		const icomp::CMultiFactoryAttribute* multiFactoryPtr = dynamic_cast<const icomp::CMultiFactoryAttribute*>(attributePtr);
		if (multiFactoryPtr != NULL){
			QString dependecyString;
			for (int dependencyIndex = 0; dependencyIndex < multiFactoryPtr->GetValuesCount(); dependencyIndex++){
				QString dependencyName = iqt::GetQString(multiFactoryPtr->GetValueAt(dependencyIndex));
				dependecyString += dependencyName  + ";";
			}

			attributeItemPtr->setText(ValueColumn, dependecyString);
			attributeItemPtr->setData(ValueColumn, AttributeMining, MultipleDependency);

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

	QList<QTreeWidgetItem*> items = AttributeTree->selectedItems();
	if (items.count() == 0){
		return;
	}

	AttributeType->clear();
	AttributeDescription->clear();

	QTreeWidgetItem* attributeItemPtr = items.at(0);

	QString attributeId = attributeItemPtr->data(ValueColumn, AttributeId).toString();
	const icomp::IAttributeStaticInfo* attributeStaticInfoPtr = GetStaticAttributeInfo(attributeId);
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


// protected methods

// reimplemented (CGuiComponentBase)

void CAttributeEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr);

	AttributeTree->header()->setStretchLastSection(true);

	AttributeTree->setItemDelegate(new AttributeItemDelegate(this));

	connect(AttributeTree, 
		SIGNAL(itemSelectionChanged()), 
		this, 
		SLOT(OnItemSelected()));

	AttributeInfoBox->hide();
	AttributeTree->setFrameShape(QFrame::NoFrame);
	ExportTree->setFrameShape(QFrame::NoFrame);
}


// members of nested class AttributeItemDelegate

CAttributeEditorComp::AttributeItemDelegate::AttributeItemDelegate(CAttributeEditorComp* parent)
	:QItemDelegate()
{
	m_parent = parent;
}


const icomp::IRegistryElement::AttributeInfo* CAttributeEditorComp::AttributeItemDelegate::GetAttribute(const QString& attributeName) const
{
	return m_parent->GetRegistryAttribute(attributeName);
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
	if (index.column() == 0){
		return NULL;
	}

	int propertyMining = index.data(AttributeMining).toInt();

	if (index.column() == ValueColumn && propertyMining == Dependency){
		QComboBox* combo = new QComboBox(parent);
		combo->setEditable(true);
		return combo;
	} 
	else if(propertyMining == MultipleDependency){
		QComboBox* combo = new QComboBox(parent);
		combo->setEditable(true);
		return combo;
	}
	else if (propertyMining == SelectableAttribute){
		QComboBox* combo = new QComboBox(parent);
		return combo;
	}

	return QItemDelegate::createEditor(parent, option, index);
}


void CAttributeEditorComp::AttributeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index ) const
{
/*	QComboBox* combo = dynamic_cast<QComboBox*>(editor);
	QString dependecySource = index.data(AttributeId).toString();
	QString componentName = m_parent->m_componentViewPtr->selectedComponentName();
	dependecySource = componentName + "."  + dependecySource;

	QString attributeName = index.data(AttributeId).toString();
	acf::ComponentAttributeInterface* attributePtr = GetAttribute(attributeName);

	int propertyMining = index.data(AttributeMining).toInt();

	if (index.column() == ValueColumn && combo != NULL && propertyMining == Dependency){
		combo->addItems(m_parent->m_componentViewPtr->componentsForDependency(dependecySource));
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

		icomp::TSingleAttribute* boolAttribute = dynamic_cast<icomp::TSingleAttribute*>(attributePtr);
		if (boolAttribute != NULL){
			bool value = boolAttribute->GetValue();
			editor->setProperty("text", QVariant(value));
		}

		icomp::CStringAttribute* stringAttribute = dynamic_cast<icomp::CStringAttribute*>(attributePtr);
		if (stringAttribute != NULL){
			istd::CString value = stringAttribute->GetValue();
			editor->setProperty("text", QVariant(iqt::GetQString(value)));
		}

		icomp::TMultiAttribute<istd::CString>* stringListAttribute = dynamic_cast<icomp::TMultiAttribute<istd::CString>*>(attributePtr);
		if (stringListAttribute != NULL){
			QStringList value = iqt::GetQStringList(stringListAttribute->GetValueList());
			QString outputValue;
			for (int index = 0; index < value.count(); index++){
				outputValue += value.at(index) + ";";
			}

			editor->setProperty("text", QVariant(outputValue));
		}

		icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<icomp::CMultiIntAttribute*>(attributePtr);
		if (intListAttribute != NULL){
			std::vector<int> list = intListAttribute->GetValueList();
			QString outputValue;
			for (int index = 0; index < int(list.size()); index++){
				outputValue += QString("%1").arg(list.at(index)) + ";";
			}

			editor->setProperty("text", QVariant(outputValue));
		}

		icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<icomp::CMultiDoubleAttribute*>(attributePtr);
		if (doubleListAttribute != NULL){
			std::vector<double> list = doubleListAttribute->GetValueList();
			QString outputValue;
			for (int index = 0; index < int(list.size()); index++){
				outputValue += QString("%1").arg(list.at(index)) + ";";
			}

			editor->setProperty("text", QVariant(outputValue));
		}

		return;
	}
	else if(index.column() == ValueColumn && propertyMining == MultipleDependency){
		QStringList components = m_parent->m_componentViewPtr->componentsForDependency(dependecySource);
		combo->addItems(m_parent->m_componentViewPtr->componentsForDependency(dependecySource));
		combo->lineEdit()->setText(index.data().toString());

		return;
	}	
	else if(index.column() == ValueColumn && propertyMining == SelectableAttribute && combo != NULL){
		acf::SelectableStringListAttribute* stringListAttribute = dynamic_cast<acf::SelectableStringListAttribute*>(attributePtr);
		if (stringListAttribute != NULL){
			QStringList value = iqt::GetQStringList(stringListAttribute->GetValueList());
			QString outputValue;
			for (int index = 0; index < value.count(); index++){
				combo->addItem(value.at(index));
			}
		}

		return;
	}
*/
	QItemDelegate::setEditorData(editor, index);
}


void CAttributeEditorComp::AttributeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
/*	QString attributeName = index.data(AttributeId).toString();
	acf::ComponentAttributeInterface* attributePtr = GetAttribute(attributeName);

	int propertyMining = index.data(AttributeMining).toInt();
	
	// set single dependency data
	if (index.column() == ValueColumn && propertyMining == Dependency){
		QComboBox* combo = dynamic_cast<QComboBox*>(editor);
		I_ASSERT(combo != NULL);
		QString dependecyTarget = combo->currentText();

		acf::ComponentDependencyAttribute* dependencyAttribute = 
			dynamic_cast<acf::ComponentDependencyAttribute*>(attributePtr);
		if (dependencyAttribute != NULL){
			dependencyAttribute->SetDependencyName(iqt::GetCString(dependecyTarget));
		}

		m_parent->emitUpdateView();

		return;
	}

	// set multiple dependency data
	if (index.column() == ValueColumn && propertyMining == MultipleDependency){
		QComboBox* combo = dynamic_cast<QComboBox*>(editor);
		I_ASSERT(combo != NULL);
		QString string = combo->lineEdit()->text();
		QStringList dependecyTarget = string.split(';',QString::SkipEmptyParts); 

		acf::MultipleComponentDependencyAttribute* dependencyAttribute = 
			dynamic_cast<acf::MultipleComponentDependencyAttribute*>(attributePtr);

		if (dependencyAttribute != NULL){
			dependencyAttribute->SetDependencyNames(iqt::GetCStringList(dependecyTarget));
		}

		m_parent->emitUpdateView();

		return;
	}

	// set attribute data:
	if (index.column() == ValueColumn && propertyMining == Attribute || propertyMining == MultipleAttribute){	
		icomp::CIntAttribute* intAttribute = dynamic_cast<icomp::CIntAttribute*>(attributePtr);
		if (intAttribute != NULL){
			intAttribute->SetValue(editor->property("text").toInt());
		}

		icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<icomp::CDoubleAttribute*>(attributePtr);
		if (doubleAttribute != NULL){
			doubleAttribute->SetValue(editor->property("text").toDouble());
		}

		icomp::TSingleAttribute* boolAttribute = dynamic_cast<icomp::TSingleAttribute*>(attributePtr);
		if (boolAttribute != NULL){
			boolAttribute->SetValue(editor->property("text").toBool());
		}

		icomp::CStringAttribute* stringAttribute = dynamic_cast<icomp::CStringAttribute*>(attributePtr);
		if (stringAttribute != NULL){
			QString newValue = editor->property("text").toString();
			stringAttribute->SetValue(iqt::GetCString(newValue));
		}

		icomp::TMultiAttribute<istd::CString>* stringListAttribute = dynamic_cast<icomp::TMultiAttribute<istd::CString>*>(attributePtr);
		if (stringListAttribute != NULL){
			QString newValue = editor->property("text").toString();
			QStringList values = newValue.split(';', QString::SkipEmptyParts);
			stringListAttribute->SetValueList(iqt::GetCStringList(values));
		}

		icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<icomp::CMultiIntAttribute*>(attributePtr);
		if (intListAttribute != NULL){
			QString newValue = editor->property("text").toString();
			QStringList values = newValue.split(';', QString::SkipEmptyParts);
			std::vector<int> list;
			foreach(QString stringValue, values){
				list.push_back(stringValue.toInt());
			}

			intListAttribute->SetValueList(list);
		}

		icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<icomp::CMultiDoubleAttribute*>(attributePtr);
		if (doubleListAttribute != NULL){
			QString newValue = editor->property("text").toString();
			QStringList values = newValue.split(';', QString::SkipEmptyParts);
			std::vector<double> list;
			foreach(QString stringValue, values){
				list.push_back(stringValue.toDouble());
			}

			doubleListAttribute->SetValueList(list);
		}

	}

	if (index.column() == ValueColumn && propertyMining == SelectableAttribute){
		acf::SelectableStringListAttribute* stringListAttribute = dynamic_cast<acf::SelectableStringListAttribute*>(attributePtr);
		if (stringListAttribute != NULL){
			QComboBox* combo = dynamic_cast<QComboBox*>(editor);
			if (combo != NULL){
				stringListAttribute->SetSelected(combo->currentIndex());
			}
		}
	}

	m_parent->emitUpdateView();
*/
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
	painter->setPen(QPen(Qt::darkGray, 0.5, Qt::DotLine));
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

