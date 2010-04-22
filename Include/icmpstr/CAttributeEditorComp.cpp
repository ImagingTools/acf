#include "icmpstr/CAttributeEditorComp.h"


// Qt includes
#include <QHeaderView>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "icomp/CInterfaceManipBase.h"


// public methods
	
namespace icmpstr
{


CAttributeEditorComp::CAttributeEditorComp()
:	m_attributeItemDelegate(this),
	m_registryObserver(this),
	m_lastRegistryModelPtr(NULL)
{
	m_attributeTypesMap[icomp::CBoolAttribute::GetTypeName()] = tr("Boolean");
	m_attributeTypesMap[icomp::CDoubleAttribute::GetTypeName()] = tr("Real number");
	m_attributeTypesMap[icomp::CIntAttribute::GetTypeName()] = tr("Integer number");
	m_attributeTypesMap[icomp::CStringAttribute::GetTypeName()] = tr("String");
	m_attributeTypesMap[icomp::CMultiBoolAttribute::GetTypeName()] = tr("Boolean list");
	m_attributeTypesMap[icomp::CMultiDoubleAttribute::GetTypeName()] = tr("Real number list");
	m_attributeTypesMap[icomp::CMultiIntAttribute::GetTypeName()] = tr("Integer number list");
	m_attributeTypesMap[icomp::CMultiStringAttribute::GetTypeName()] = tr("String list");
	m_attributeTypesMap[icomp::CReferenceAttribute::GetTypeName()] = tr("Reference");
	m_attributeTypesMap[icomp::CMultiReferenceAttribute::GetTypeName()] = tr("Multiple reference");
	m_attributeTypesMap[icomp::CFactoryAttribute::GetTypeName()] = tr("Factory");
	m_attributeTypesMap[icomp::CMultiFactoryAttribute::GetTypeName()] = tr("Multiple factory");
}


icomp::IRegistry* CAttributeEditorComp::GetRegistry() const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return NULL;
	}

	return selectionInfoPtr->GetSelectedRegistry();
}


icomp::IRegistryElement* CAttributeEditorComp::GetRegistryElement() const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return NULL;
	}

	return dynamic_cast<icomp::IRegistryElement*>(selectionInfoPtr->GetSelectedElement());
}


const std::string& CAttributeEditorComp::GetRegistryElementName() const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		static std::string emptyName;
		return emptyName;
	}

	return selectionInfoPtr->GetSelectedElementName();
}


icomp::IRegistryElement::AttributeInfo* CAttributeEditorComp::GetRegistryAttribute(const std::string& attributeName) const
{
	icomp::IRegistryElement* elementPtr = GetRegistryElement();
	if (elementPtr == NULL){
		return NULL;
	}

	return const_cast<icomp::IRegistryElement::AttributeInfo*>(elementPtr->GetAttributeInfo(attributeName));
}


const icomp::IAttributeStaticInfo* CAttributeEditorComp::GetStaticAttributeInfo(const std::string& id) const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if ((selectionInfoPtr == NULL) || !m_metaInfoManagerCompPtr.IsValid()){
		return NULL;
	}

	const icomp::CComponentAddress* addressPtr = selectionInfoPtr->GetSelectedElementAddress();
	if (addressPtr == NULL){
		return NULL;
	}

	const icomp::IComponentStaticInfo* componentInfoPtr = m_metaInfoManagerCompPtr->GetComponentMetaInfo(*addressPtr);
	if (componentInfoPtr == NULL){
		return NULL;
	}

	const icomp::IComponentStaticInfo::AttributeInfos& staticAttributes = componentInfoPtr->GetAttributeInfos();

	const icomp::IComponentStaticInfo::AttributeInfos::ValueType* attrInfoPtr2 =
				staticAttributes.FindElement(id);

	if (attrInfoPtr2 != NULL){
		return *attrInfoPtr2;
	}
	else{
		return NULL;
	}
}

	
QStringList CAttributeEditorComp::GetExportAliases(const std::string& attributeName) const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return QStringList();
	}

	QStringList exportedAliases;

	const icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
	if (registryPtr != NULL){
		const icomp::IRegistry::ExportedComponentsMap& exportedMap = registryPtr->GetExportedComponentsMap();
		for (		icomp::IRegistry::ExportedComponentsMap::const_iterator iter = exportedMap.begin();
					iter != exportedMap.end();
					++iter){
			if (iter->second == attributeName){
				exportedAliases.append(iqt::GetQString(iter->first));
			}
		}
	}

	return exportedAliases;
}


// reimplemented (TGuiObserverWrap)

void CAttributeEditorComp::OnGuiModelDetached()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blocker(this);

		ElementInfoTab->setVisible(false);

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

	icomp::IRegistry* registryPtr = GetRegistry();
	imod::IModel* registryModelPtr = dynamic_cast<imod::IModel*>(registryPtr);
	if (registryModelPtr != m_lastRegistryModelPtr){
		if (m_registryObserver.IsModelAttached(m_lastRegistryModelPtr)){
			m_lastRegistryModelPtr->DetachObserver(&m_registryObserver);
		}

		if (m_registryPropObserverCompPtr.IsValid()){
			if (m_registryPropObserverCompPtr->IsModelAttached(m_lastRegistryModelPtr)){
				m_lastRegistryModelPtr->DetachObserver(m_registryPropObserverCompPtr.GetPtr());
			}
		}

		if (registryModelPtr != NULL){
			registryModelPtr->AttachObserver(&m_registryObserver);

			if (m_registryPropObserverCompPtr.IsValid()){
				registryModelPtr->AttachObserver(m_registryPropObserverCompPtr.GetPtr());
			}
		}

		m_lastRegistryModelPtr = registryModelPtr;
	}

	if (registryPtr == NULL){
		ElementInfoTab->setVisible(false);
		RegistryPropertiesFrame->setVisible(false);

		return;
	}

	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	const icomp::IRegistryElement* elementPtr = GetRegistryElement();

	if ((selectionInfoPtr == NULL) || (elementPtr == NULL)){
		ElementInfoTab->setVisible(false);

		RegistryPropertiesFrame->setVisible(true);

		return;
	}
	else{
		RegistryPropertiesFrame->setVisible(false);

		ElementInfoTab->setVisible(true);
	}

	UpdateSelectedAttr();
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

		istd::CChangeNotifier registryNotifier(GetRegistry(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);
		istd::CChangeNotifier elementNotifier(elementPtr, istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);

		std::string attributeId = item->text(NameColumn).toStdString();
		const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementPtr->GetAttributeInfo(attributeId);
		if (isEnabled){
			std::string attributeType = item->data(ValueColumn, AttributeType).toString().toStdString();
			elementPtr->InsertAttributeInfo(attributeId, attributeType);
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

	if (column == 0){
		UpdateBlocker blocker(this);
	
		const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
		if (selectionInfoPtr == NULL){
			return;
		}

		istd::TChangeNotifier<icomp::IRegistry> registryPtr(
					selectionInfoPtr->GetSelectedRegistry(),
					istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);
		if (!registryPtr.IsValid()){
			return;
		}

		QString interfaceName = item->text(column);

		const std::string& elementName = selectionInfoPtr->GetSelectedElementName();
		bool isSelected = (item->checkState(column) == Qt::Checked);
		registryPtr->SetElementInterfaceExported(elementName, istd::CClassInfo(interfaceName.toStdString()), isSelected);

		UpdateExportIcon();
	}
}


void CAttributeEditorComp::on_AutoInstanceCB_toggled(bool checked)
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker blocker(this);

	istd::TChangeNotifier<icomp::IRegistryElement> elementPtr(GetRegistryElement(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);

	if (elementPtr.IsValid()){
		I_DWORD flags = elementPtr->GetElementFlags();

		flags = checked?
					(flags | icomp::IRegistryElement::EF_AUTO_INSTANCE):
					(flags & ~icomp::IRegistryElement::EF_AUTO_INSTANCE);

		istd::CChangeNotifier registryNotifier(GetRegistry(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_FLAGS_CHANGED);

		elementPtr->SetElementFlags(flags);
	}
}


// protected methods

void CAttributeEditorComp::UpdateSelectedAttr()
{
	AttributeTree->clear();
	InterfacesTree->clear();
	ComponentsTree->clear();

	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return;
	}

	const icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	const icomp::IRegistryElement* elementPtr = GetRegistryElement();

	const std::string& elementId = selectionInfoPtr->GetSelectedElementName();
	QTreeWidgetItem* componentRootPtr = new QTreeWidgetItem();
	componentRootPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	componentRootPtr->setText(NameColumn, tr("this"));
	componentRootPtr->setData(ValueColumn, AttributeId, QString(elementId.c_str()));
	componentRootPtr->setData(ValueColumn, AttributeMining, ComponentExport);
	componentRootPtr->setText(ValueColumn, GetExportAliases(elementId).join(";"));

	const icomp::IComponentStaticInfo* infoPtr = NULL;

	QIcon icon;
	const icomp::CComponentAddress* addressPtr = selectionInfoPtr->GetSelectedElementAddress();
	if (addressPtr != NULL){
		if (m_metaInfoManagerCompPtr.IsValid()){
			infoPtr = m_metaInfoManagerCompPtr->GetComponentMetaInfo(*addressPtr);
		}

		AddressLabel->setText(QString(addressPtr->GetPackageId().c_str()) + QString("/") + addressPtr->GetComponentId().c_str());
		AddressLabel->setVisible(true);

		if (m_consistInfoCompPtr.IsValid()){
			icon = m_consistInfoCompPtr->GetComponentIcon(*addressPtr);
		}
	}
	else{
		AddressLabel->setVisible(false);
	}

	NameLabel->setText(elementId.c_str());

	if (!icon.isNull()){
		IconLabel->setPixmap(icon.pixmap(128));
		IconLabel->setVisible(true);
	}
	else{
		IconLabel->setVisible(false);
	}

	if (infoPtr != NULL){
		DescriptionLabel->setText(iqt::GetQString(infoPtr->GetDescription()));
		KeywordsLabel->setText(iqt::GetQString(infoPtr->GetKeywords()));

		const icomp::IComponentStaticInfo::AttributeInfos& staticAttributes = infoPtr->GetAttributeInfos();

		bool hasExport = false;

		for (int staticAttributeIndex = 0; staticAttributeIndex < staticAttributes.GetElementsCount(); staticAttributeIndex++){
			const std::string& attributeId = staticAttributes.GetKeyAt(staticAttributeIndex);
			const icomp::IAttributeStaticInfo* staticAttributeInfPtr = staticAttributes.GetValueAt(staticAttributeIndex);
			I_ASSERT(staticAttributeInfPtr != NULL);

			QTreeWidgetItem* attributeItemPtr = new QTreeWidgetItem();
			QTreeWidgetItem* exportItemPtr = new QTreeWidgetItem(attributeItemPtr);

			bool exportFlag;
			SetAttributeToItems(attributeId, *staticAttributeInfPtr, *attributeItemPtr, *exportItemPtr, &exportFlag);

			if (m_consistInfoCompPtr.IsValid()){
				bool isAttrCorrect = m_consistInfoCompPtr->IsAttributeValid(
							attributeId,
							elementId,
							*registryPtr,
							true,
							false,
							NULL);

				attributeItemPtr->setBackgroundColor(ValueColumn, isAttrCorrect? Qt::white: Qt::red);
			}

			hasExport = hasExport || exportFlag;

			AttributeTree->addTopLevelItem(attributeItemPtr);
			attributeItemPtr->addChild(exportItemPtr);
		}

		AttributeTree->resizeColumnToContents(0);

		const icomp::IComponentStaticInfo::InterfaceExtractors& extractors = infoPtr->GetInterfaceExtractors();

		icomp::IRegistry::ExportedInterfacesMap interfacesMap;

		interfacesMap = registryPtr->GetExportedInterfacesMap();

		for (int extractorIndex = 0; extractorIndex < extractors.GetElementsCount(); extractorIndex++){
			const istd::CClassInfo& interfaceInfo = extractors.GetKeyAt(extractorIndex);
			QTreeWidgetItem* itemPtr = new QTreeWidgetItem();

			itemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
			itemPtr->setText(0, interfaceInfo.GetName().c_str());

			icomp::IRegistry::ExportedInterfacesMap::const_iterator foundExportIter = interfacesMap.find(interfaceInfo);
			bool isInterfaceExported = false;
			if (foundExportIter != interfacesMap.end()){
				isInterfaceExported = (foundExportIter->second == elementId);
			}

			itemPtr->setCheckState(0, isInterfaceExported? Qt::Checked: Qt::Unchecked);

			InterfacesTree->addTopLevelItem(itemPtr);
		}

		if (elementPtr != NULL){
			I_DWORD elementFlags = elementPtr->GetElementFlags();
			AutoInstanceCB->setChecked((elementFlags & icomp::IRegistryElement::EF_AUTO_INSTANCE) != 0);
			AutoInstanceCB->setEnabled(true);
		}
		else{
			AutoInstanceCB->setChecked(false);
			AutoInstanceCB->setEnabled(false);
		}

		CreateExportedComponentsTree(elementId, *infoPtr, *componentRootPtr);

		ComponentsTree->addTopLevelItem(componentRootPtr);
		
		componentRootPtr->setExpanded(true);

		MetaInfoFrame->setVisible(true);
	}
	else{
		MetaInfoFrame->setVisible(false);
	}

	bool isElementCorrect = true;
	if (m_consistInfoCompPtr.IsValid()){
		isElementCorrect = m_consistInfoCompPtr->IsElementValid(
					elementId,
					*registryPtr,
					true,
					false,
					NULL);
	}

	ElementInfoTab->setTabIcon(TI_ATTRIBUTES, isElementCorrect? QIcon(): QIcon(":/Icons/StateInvalid.svg"));

	UpdateExportIcon();
}


bool CAttributeEditorComp::SetAttributeToItems(
			const std::string& id,
			const icomp::IAttributeStaticInfo& staticInfo,
			QTreeWidgetItem& attributeItem,
			QTreeWidgetItem& exportItem,
			bool* hasExportPtr)
{
	icomp::IRegistryElement* elementPtr = GetRegistryElement();
	if (elementPtr == NULL){
		return false;
	}

	const iser::IObject* attributePtr = NULL;

	attributeItem.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);

	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementPtr->GetAttributeInfo(id);
	if ((attributeInfoPtr != NULL) && !attributeInfoPtr->exportId.empty()){
		attributeItem.setIcon(NameColumn, m_exportIcon);
		if (hasExportPtr != NULL){
			*hasExportPtr = true;
		}
	}

	bool isDefaultsValueUsed = false;
	if ((attributeInfoPtr != NULL) && attributeInfoPtr->attributePtr.IsValid()){
		attributePtr = attributeInfoPtr->attributePtr.GetPtr();
		attributeItem.setCheckState(NameColumn, Qt::Checked);
	}
	else{
		attributePtr = staticInfo.GetAttributeDefaultValue();
		attributeItem.setCheckState(NameColumn, Qt::Unchecked);

		isDefaultsValueUsed = true;
	}

	QString attributeType;
	AttributeTypesMap::const_iterator foundTypeName = m_attributeTypesMap.find(staticInfo.GetAttributeTypeName());
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

	QString attributeName = id.c_str();
	attributeItem.setText(NameColumn, attributeName);
	attributeItem.setData(ValueColumn, AttributeId, attributeName);
	attributeItem.setData(ValueColumn, AttributeType, QString(staticInfo.GetAttributeTypeName().c_str()));
	attributeItem.setToolTip(NameColumn, toolTip);
	attributeItem.setToolTip(ValueColumn, toolTip);

	exportItem.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	exportItem.setText(NameColumn, tr("<export>"));
	exportItem.setText(ValueColumn, (attributeInfoPtr != NULL)? attributeInfoPtr->exportId.c_str(): "");
	exportItem.setData(ValueColumn, AttributeMining, AttributeExport);
	exportItem.setData(ValueColumn, AttributeId, attributeName);
	exportItem.setData(ValueColumn, AttributeType, QString(staticInfo.GetAttributeTypeName().c_str()));
	exportItem.setToolTip(NameColumn, toolTip);
	exportItem.setToolTip(ValueColumn, toolTip);

	QString text;
	int meaning;

	if ((attributePtr != NULL) && DecodeAttribute(*attributePtr, text, meaning)){
		attributeItem.setText(ValueColumn, text);
		attributeItem.setData(ValueColumn, AttributeMining, meaning);

		return true;
	}

	return false;
}


bool CAttributeEditorComp::DecodeAttribute(
			const iser::ISerializable& attribute,
			QString& text,
			int& meaning)
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

	const icomp::TAttribute<std::string>* idPtr = dynamic_cast<const icomp::TAttribute<std::string>*>(&attribute);
	if (idPtr != NULL){		
		QString componentId = idPtr->GetValue().c_str();

		text = EncodeToEdit(componentId);

		meaning = Reference;

		return true;
	}

	const icomp::TMultiAttribute<std::string>* multiIdPtr = dynamic_cast<const icomp::TMultiAttribute<std::string>*>(&attribute);
	if (multiIdPtr != NULL){
		QString dependecyString;

		int idsCount = multiIdPtr->GetValuesCount();
		for (int idIndex = 0; idIndex < idsCount; idIndex++){
			QString componentId = multiIdPtr->GetValueAt(idIndex).c_str();

			text += EncodeToEdit(componentId)  + ";";
		}

		meaning = MultipleReference;

		return true;
	}

	return false;
}


void CAttributeEditorComp::CreateExportedComponentsTree(
			const std::string& elementId,
			const icomp::IComponentStaticInfo& elementStaticInfo,
			QTreeWidgetItem& rootItem) const
{
	const icomp::IComponentStaticInfo::Ids subcomponentIds = elementStaticInfo.GetSubcomponentIds();

	for (		icomp::IComponentStaticInfo::Ids::const_iterator subIter = subcomponentIds.begin();
				subIter != subcomponentIds.end();
				++subIter){
		const std::string& subcomponentId = *subIter;

		std::string fullId = icomp::CInterfaceManipBase::JoinId(elementId, subcomponentId);

		QTreeWidgetItem* itemPtr = new QTreeWidgetItem();

		itemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		itemPtr->setText(NameColumn, subcomponentId.c_str());
		itemPtr->setData(ValueColumn, AttributeId, QString(fullId.c_str()));
		itemPtr->setData(ValueColumn, AttributeMining, ComponentExport);
		itemPtr->setText(ValueColumn, GetExportAliases(fullId).join(";"));

		const icomp::IComponentStaticInfo* subcomponentInfoPtr = elementStaticInfo.GetSubcomponentInfo(subcomponentId);
		if (subcomponentInfoPtr != NULL){
			CreateExportedComponentsTree(fullId, *subcomponentInfoPtr, *itemPtr);
		}

		rootItem.addChild(itemPtr);
	}
}


void CAttributeEditorComp::UpdateExportIcon()
{
	QIcon interfacesIcon;
	QIcon componentsIcon;

	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();

	if (selectionInfoPtr != NULL){
		const icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
		if (registryPtr != NULL){
			const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registryPtr->GetExportedInterfacesMap();

			const std::string& name = selectionInfoPtr->GetSelectedElementName();

			for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator iter = interfacesMap.begin();
						iter != interfacesMap.end();
						++iter){
				if (iter->second == name){
					interfacesIcon = m_exportIcon;

					break;
				}
			}

			const std::string& elementName = selectionInfoPtr->GetSelectedElementName();

			icomp::IRegistry::ExportedComponentsMap componentsMap = registryPtr->GetExportedComponentsMap();

			for (icomp::IRegistry::ExportedComponentsMap::const_iterator componentIter = componentsMap.begin();
						componentIter != componentsMap.end();
						++componentIter){
				std::string componentId;
				std::string restId;
				icomp::CInterfaceManipBase::SplitId(componentIter->second, componentId, restId);
				if (componentId == elementName){
					componentsIcon = m_exportIcon;

					break;
				}
			}
		}
	}

	ElementInfoTab->setTabIcon(TI_INTERFACES, interfacesIcon);
	ElementInfoTab->setTabIcon(TI_EXPORTS, componentsIcon);
}


// reimplemented (CGuiComponentBase)

void CAttributeEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ElementInfoTab->setVisible(false);
	RegistryPropertiesFrame->setVisible(false);

	m_treeWidgetFilter.SetPtr(new iqtgui::CTreeWidgetFilter(AttributeTree));

	AttributeTree->setItemDelegate(&m_attributeItemDelegate);
	ComponentsTree->setItemDelegate(&m_attributeItemDelegate);

	m_exportIcon.addFile(":/Icons/Export.svg");

	if (m_registryPropGuiCompPtr.IsValid()){
		m_registryPropGuiCompPtr->CreateGui(RegistryPropertiesFrame);
	}
}


void CAttributeEditorComp::OnGuiDestroyed()
{
	if (m_registryPropGuiCompPtr.IsValid()){
		m_registryPropGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
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


// public methods of embedded class AttributeItemDelegate

CAttributeEditorComp::AttributeItemDelegate::AttributeItemDelegate(CAttributeEditorComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// reimplemented (QItemDelegate)

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
		std::string attributeType = index.data(AttributeType).toString().toStdString();
		if (attributeType == icomp::CBoolAttribute::GetTypeName()){
			QComboBox* comboEditor = new QComboBox(parent);
			comboEditor->addItem(tr("true"));
			comboEditor->addItem(tr("false"));
	
			return comboEditor;
		}
	} 

	return BaseClass::createEditor(parent, option, index);
}


void CAttributeEditorComp::AttributeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	if (index.column() != ValueColumn){
		BaseClass::setEditorData(editor, index);
	}

	int propertyMining = index.data(AttributeMining).toInt();

	std::string attributeName = index.data(AttributeId).toString().toStdString();

	if (propertyMining == ComponentExport){
		SetComponentExportEditor(attributeName, *editor);
	}
	else if (propertyMining == AttributeExport){
		SetAttributeExportEditor(attributeName, *editor);
	}
	else{
		SetAttributeValueEditor(attributeName, propertyMining, *editor);
	}
}


void CAttributeEditorComp::AttributeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (index.column() != ValueColumn){
		BaseClass::setModelData(editor, model, index);
	}

	int propertyMining = index.data(AttributeMining).toInt();
	std::string attributeName = index.data(AttributeId).toString().toStdString();

	if (propertyMining == ComponentExport){
		SetComponentExportData(attributeName, *editor);
	}
	else{
		std::string attributeTypeName = index.data(AttributeType).toString().toStdString();
		if (propertyMining == AttributeExport){
			SetAttributeExportData(attributeName, attributeTypeName, *editor);
		}
		else{
			SetAttributeValueData(attributeName, attributeTypeName, propertyMining, *editor);
		}
	}
}


// protected methods of embedded class AttributeItemDelegate

bool CAttributeEditorComp::AttributeItemDelegate::SetComponentExportEditor(const std::string& attributeId, QWidget& editor) const
{
	QString editorValue = m_parent.GetExportAliases(attributeId).join(";");

	editor.setProperty("text", QVariant(editorValue));

	return true;
}


bool CAttributeEditorComp::AttributeItemDelegate::SetAttributeExportEditor(const std::string& id, QWidget& editor) const
{
	std::string exportId;
	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = m_parent.GetRegistryAttribute(id);
	if (attributeInfoPtr != NULL){
		exportId = attributeInfoPtr->exportId;
	}

	editor.setProperty("text", QVariant(exportId.c_str()));

	return true;
}


bool CAttributeEditorComp::AttributeItemDelegate::SetAttributeValueEditor(
			const std::string& id,
			int propertyMining,
			QWidget& editor) const
{
	const icomp::IAttributeStaticInfo* staticInfoPtr = m_parent.GetStaticAttributeInfo(id);

	const iser::IObject* attributePtr = NULL;

	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = m_parent.GetRegistryAttribute(id);
	if ((attributeInfoPtr != NULL) && attributeInfoPtr->attributePtr.IsValid()){
		attributePtr = attributeInfoPtr->attributePtr.GetPtr();
	}
	else if (staticInfoPtr != NULL){
		attributePtr = staticInfoPtr->GetAttributeDefaultValue();
	}

	QComboBox* comboEditor = dynamic_cast<QComboBox*>(&editor);

	if (		(propertyMining == Reference) ||
				(propertyMining == MultipleReference)){
		if (comboEditor == NULL){
			return false;
		}

		const icomp::IRegistry* registryPtr = m_parent.GetRegistry();
		if ((registryPtr != NULL) && (staticInfoPtr != NULL) && m_parent.m_consistInfoCompPtr.IsValid()){
			const istd::CClassInfo& interfaceInfo = staticInfoPtr->GetRelatedInterfaceType();
			icomp::IRegistry::Ids compatIds = m_parent.m_consistInfoCompPtr->GetCompatibleElements(
						interfaceInfo,
						*registryPtr,
						false);

			for (		icomp::IRegistry::Ids::const_iterator iter = compatIds.begin();
						iter != compatIds.end();
						++iter){
				comboEditor->addItem(iter->c_str());
			}
		}

		const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<const icomp::CReferenceAttribute*>(attributePtr);
		if (referenceAttributePtr != NULL){
			comboEditor->lineEdit()->setText(iqt::GetQString(referenceAttributePtr->GetValue()));

			return true;
		}

		const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<const icomp::CFactoryAttribute*>(attributePtr);
		if(factoryAttributePtr != NULL){
			comboEditor->lineEdit()->setText(iqt::GetQString(factoryAttributePtr->GetValue()));		

			return true;
		}

		const icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(attributePtr);
		if (multiReferenceAttributePtr != NULL){
			QString valuesString;
			for (int index = 0; index < multiReferenceAttributePtr->GetValuesCount(); index++){
				valuesString += iqt::GetQString(multiReferenceAttributePtr->GetValueAt(index)) + ";";
			}

			comboEditor->lineEdit()->setText(valuesString);

			return true;
		}

		const icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<const icomp::CMultiFactoryAttribute*>(attributePtr);
		if (multiFactoryAttributePtr != NULL){
			QString valuesString;
			for (int index = 0; index < multiFactoryAttributePtr->GetValuesCount(); index++){
				valuesString += iqt::GetQString(multiFactoryAttributePtr->GetValueAt(index)) + ";";
			}

			comboEditor->lineEdit()->setText(valuesString);

			return true;
		}
	}
	else if (propertyMining == Attribute || propertyMining == MultipleAttribute){
		const icomp::CIntAttribute* intAttribute = dynamic_cast<const icomp::CIntAttribute*>(attributePtr);
		if (intAttribute != NULL){
			int value = intAttribute->GetValue();
			editor.setProperty("text", QVariant(value));

			return true;
		}

		const icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<const icomp::CDoubleAttribute*>(attributePtr);
		if (doubleAttribute != NULL){
			double value = doubleAttribute->GetValue();
			editor.setProperty("text", QVariant(value));

			return true;
		}

		const icomp::CBoolAttribute* boolAttribute = dynamic_cast<const icomp::CBoolAttribute*>(attributePtr);
		if (boolAttribute != NULL){
			bool value = boolAttribute->GetValue();
			comboEditor->setCurrentIndex(value? 0: 1);

			return true;
		}

		const icomp::CStringAttribute* stringAttribute = dynamic_cast<const icomp::CStringAttribute*>(attributePtr);
		if (stringAttribute != NULL){
			istd::CString value = stringAttribute->GetValue();
			editor.setProperty("text", QVariant(iqt::GetQString(value)));

			return true;
		}

		const icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<const icomp::CMultiStringAttribute*>(attributePtr);
		if (stringListAttribute != NULL){
			QString outputValue;
			for (int index = 0; index < stringListAttribute->GetValuesCount(); index++){
				outputValue += iqt::GetQString(stringListAttribute->GetValueAt(index)) + ";";
			}

			editor.setProperty("text", QVariant(outputValue));

			return true;
		}

		const icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<const icomp::CMultiIntAttribute*>(attributePtr);
		if (intListAttribute != NULL){
			QString outputValue;
			for (int index = 0; index < intListAttribute->GetValuesCount(); index++){
				outputValue += QString("%1").arg(intListAttribute->GetValueAt(index)) + ";";
			}

			editor.setProperty("text", QVariant(outputValue));

			return true;
		}

		const icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<const icomp::CMultiDoubleAttribute*>(attributePtr);
		if (doubleListAttribute != NULL){
			QString outputValue;
			for (int index = 0; index < doubleListAttribute->GetValuesCount(); index++){
				outputValue += QString("%1").arg(doubleListAttribute->GetValueAt(index)) + ";";
			}

			editor.setProperty("text", QVariant(outputValue));

			return true;
		}
	}

	return false;
}


bool CAttributeEditorComp::AttributeItemDelegate::SetComponentExportData(const std::string& attributeId, const QWidget& editor) const
{
	istd::TChangeNotifier<icomp::IRegistry> registryPtr(m_parent.GetRegistry(), istd::IChangeable::CF_MODEL | icomp::IRegistry::CF_COMPONENT_EXPORTED);
	if (registryPtr.IsValid()){
		std::string exportId = editor.property("text").toString().toStdString();

		icomp::IRegistry::ExportedComponentsMap exportedMap = registryPtr->GetExportedComponentsMap();
		for (icomp::IRegistry::ExportedComponentsMap::const_iterator iter = exportedMap.begin();
					iter != exportedMap.end();
					++iter){
			if (iter->second == attributeId){
				registryPtr->SetElementExported(iter->first, "");
			}
		}

		if (!exportId.empty()){
			registryPtr->SetElementExported(exportId, attributeId);
		}

		m_parent.UpdateExportIcon();
	}

	return true;
}


bool CAttributeEditorComp::AttributeItemDelegate::SetAttributeExportData(
			const std::string& id,
			const std::string& typeName,
			const QWidget& editor) const
{
	istd::TChangeNotifier<icomp::IRegistryElement> elementPtr(m_parent.GetRegistryElement(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);
	if (!elementPtr.IsValid()){
		return false;
	}

	icomp::IRegistryElement::AttributeInfo* attributeInfoPtr =
				const_cast<icomp::IRegistryElement::AttributeInfo*>(elementPtr->GetAttributeInfo(id));
	if (attributeInfoPtr == NULL){
		attributeInfoPtr = elementPtr->InsertAttributeInfo(id, typeName);
		if (attributeInfoPtr == NULL){
			return false;
		}
	}
	I_ASSERT(attributeInfoPtr != NULL);

	QString newValue = editor.property("text").toString();
	attributeInfoPtr->exportId = newValue.toStdString();

	return true;
}


bool CAttributeEditorComp::AttributeItemDelegate::SetAttributeValueData(
			const std::string& id,
			const std::string& typeName,
			int propertyMining,
			const QWidget& editor) const
{
	istd::TChangeNotifier<icomp::IRegistryElement> elementPtr(m_parent.GetRegistryElement(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);
	if (!elementPtr.IsValid()){
		return false;
	}

	icomp::IRegistryElement::AttributeInfo* attributeInfoPtr =
				const_cast<icomp::IRegistryElement::AttributeInfo*>(elementPtr->GetAttributeInfo(id));
	if (attributeInfoPtr == NULL){
		attributeInfoPtr = elementPtr->InsertAttributeInfo(id, typeName);
		if (attributeInfoPtr == NULL){
			return false;
		}
	}

	if (!attributeInfoPtr->attributePtr.IsValid()){
		icomp::IRegistryElement* elementPtr = m_parent.GetRegistryElement();
		I_ASSERT(elementPtr != NULL);

		attributeInfoPtr->attributePtr.SetPtr(elementPtr->CreateAttribute(attributeInfoPtr->attributeTypeName));

		if (!attributeInfoPtr->attributePtr.IsValid()){
			return false;
		}
	}

	iser::IObject* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
	I_ASSERT(attributePtr != NULL);

	const QComboBox* comboEditor = dynamic_cast<const QComboBox*>(&editor);

	// set single reference data
	if (propertyMining == Reference){
		I_ASSERT(comboEditor != NULL);
		QString referenceValue = comboEditor->currentText();

		icomp::TAttribute<std::string>* referenceAttributePtr = dynamic_cast<icomp::TAttribute<std::string>*>(attributePtr);
		if (referenceAttributePtr != NULL){
			referenceAttributePtr->SetValue(DecodeFromEdit(referenceValue).toStdString());

			return true;
		}
	}
	// set multiple reference data
	else if (propertyMining == MultipleReference){
		I_ASSERT(comboEditor != NULL);
		QString string = comboEditor->lineEdit()->text();
		QStringList references = string.split(';',QString::SkipEmptyParts); 

		icomp::TMultiAttribute<std::string>* multiReferenceAttributePtr = dynamic_cast<icomp::TMultiAttribute<std::string>*>(attributePtr);

		if (multiReferenceAttributePtr != NULL){
			multiReferenceAttributePtr->Reset();
			for (int index = 0; index < references.count(); index++){
				multiReferenceAttributePtr->InsertValue(DecodeFromEdit(references.at(index)).toStdString());
			}

			return true;
		}
	}
	// set attribute data:
	else if (	(propertyMining == Attribute) ||
				(propertyMining == MultipleAttribute)){	
		icomp::CIntAttribute* intAttribute = dynamic_cast<icomp::CIntAttribute*>(attributePtr);
		if (intAttribute != NULL){
			intAttribute->SetValue(editor.property("text").toInt());

			return true;
		}

		icomp::CDoubleAttribute* doubleAttribute = dynamic_cast<icomp::CDoubleAttribute*>(attributePtr);
		if (doubleAttribute != NULL){
			doubleAttribute->SetValue(editor.property("text").toDouble());

			return true;
		}

		icomp::CBoolAttribute* boolAttribute = dynamic_cast<icomp::CBoolAttribute*>(attributePtr);
		if (boolAttribute != NULL){
			I_ASSERT(comboEditor != NULL);

			int index = comboEditor->currentIndex();
			boolAttribute->SetValue((index == 0) ? true: false);

			return true;
		}

		icomp::CStringAttribute* stringAttribute = dynamic_cast<icomp::CStringAttribute*>(attributePtr);
		if (stringAttribute != NULL){
			QString newValue = editor.property("text").toString();
			stringAttribute->SetValue(iqt::GetCString(DecodeFromEdit(newValue)));

			return true;
		}

		icomp::CMultiStringAttribute* stringListAttribute = dynamic_cast<icomp::CMultiStringAttribute*>(attributePtr);
		if (stringListAttribute != NULL){
			QString newValue = editor.property("text").toString();
			QStringList values = newValue.split(';');

			stringListAttribute->Reset();
			for (int index = 0; index < values.count(); index++){
				stringListAttribute->InsertValue(DecodeFromEdit(values.at(index)).toStdString());
			}

			return true;
		}

		icomp::CMultiIntAttribute* intListAttribute = dynamic_cast<icomp::CMultiIntAttribute*>(attributePtr);
		if (intListAttribute != NULL){
			QString valueString = editor.property("text").toString();
			QStringList values = valueString.split(';', QString::SkipEmptyParts);

			intListAttribute->Reset();
			for (int index = 0; index < values.count(); index++){
				intListAttribute->InsertValue(values.at(index).toInt());
			}

			return true;
		}

		icomp::CMultiDoubleAttribute* doubleListAttribute = dynamic_cast<icomp::CMultiDoubleAttribute*>(attributePtr);
		if (doubleListAttribute != NULL){
			QString valueString = editor.property("text").toString();
			QStringList values = valueString.split(';', QString::SkipEmptyParts);

			doubleListAttribute->Reset();
			for (int index = 0; index < values.count(); index++){
				doubleListAttribute->InsertValue(values.at(index).toDouble());
			}

			return true;
		}
	}

	return false;
}


// public methods of embedded class RegistryObserver

CAttributeEditorComp::RegistryObserver::RegistryObserver(CAttributeEditorComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class RegistryObserver

// reimplemented (imod::CSingleModelObserverBase)

void CAttributeEditorComp::RegistryObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & istd::IChangeable::CF_MODEL) != 0){
		m_parent.UpdateSelectedAttr();
	}
}


} // namespace icmpstr


