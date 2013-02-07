#include "icmpstr/CAttributeEditorComp.h"


// Qt includes
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QToolButton>
#include <QtGui/QPushButton>

// ACF includes
#include "istd/TOptDelPtr.h"
#include "istd/TChangeNotifier.h"

#include "icomp/CInterfaceManipBase.h"
#include "icomp/CCompositeComponentStaticInfo.h"
#include "icomp/CComponentMetaDescriptionEncoder.h"

#include "iqt/CSignalBlocker.h"

#include "icmpstr/CMultiAttributeDelegateWidget.h"


// public methods

namespace icmpstr
{


CAttributeEditorComp::CAttributeEditorComp()
:	m_attributeItemDelegate(this),
	m_registryObserver(this),
	m_lastRegistryModelPtr(NULL)
{
	m_attributeTypesMap[icomp::CIntegerAttribute::GetTypeName()] = tr("Integer number");
	m_attributeTypesMap[icomp::CRealAttribute::GetTypeName()] = tr("Real number");
	m_attributeTypesMap[icomp::CBooleanAttribute::GetTypeName()] = tr("Boolean value");
	m_attributeTypesMap[icomp::CStringAttribute::GetTypeName()] = tr("String");
	m_attributeTypesMap[icomp::CIdAttribute::GetTypeName()] = tr("ID");
	m_attributeTypesMap[icomp::CIntegerListAttribute::GetTypeName()] = tr("List of integer numbers");
	m_attributeTypesMap[icomp::CRealListAttribute::GetTypeName()] = tr("List of real numbers");
	m_attributeTypesMap[icomp::CBooleanListAttribute::GetTypeName()] = tr("List of boolean values");
	m_attributeTypesMap[icomp::CStringListAttribute::GetTypeName()] = tr("List of strings");
	m_attributeTypesMap[icomp::CIdListAttribute::GetTypeName()] = tr("List of ID's");
	m_attributeTypesMap[icomp::CReferenceAttribute::GetTypeName()] = tr("Component reference");
	m_attributeTypesMap[icomp::CMultiReferenceAttribute::GetTypeName()] = tr("List of component reference");
	m_attributeTypesMap[icomp::CFactoryAttribute::GetTypeName()] = tr("Component factory");
	m_attributeTypesMap[icomp::CMultiFactoryAttribute::GetTypeName()] = tr("List of component factory");

	QObject::connect(this, SIGNAL(AfterAttributesChange()), this, SLOT(UpdateAttributesView()), Qt::QueuedConnection);
	QObject::connect(this, SIGNAL(AfterInterfacesChange()), this, SLOT(UpdateInterfacesView()), Qt::QueuedConnection);
	QObject::connect(this, SIGNAL(AfterSubcomponentsChange()), this, SLOT(UpdateSubcomponentsView()), Qt::QueuedConnection);
}



// reimplemented (CElementSelectionInfoManagerBase)

const icomp::IComponentEnvironmentManager* CAttributeEditorComp::GetMetaInfoManagerPtr() const
{
	return m_metaInfoManagerCompPtr.GetPtr();
}


const icmpstr::IRegistryConsistInfo* CAttributeEditorComp::GetConsistencyInfoPtr() const
{
	return m_consistInfoCompPtr.GetPtr();
}


// protected slots

void CAttributeEditorComp::on_AttributeTree_itemSelectionChanged()
{
	if (!m_metaInfoManagerCompPtr.IsValid()){
		return;
	}

	const icomp::IAttributeStaticInfo* attributeStaticInfoPtr = NULL;

	QList<QTreeWidgetItem*> items = AttributeTree->selectedItems();
	if (!items.isEmpty()){
		QTreeWidgetItem* attributeItemPtr = items.front();

		QByteArray attributeId = attributeItemPtr->data(AC_VALUE, AttributeId).toString().toLocal8Bit();
		if (!attributeId.isEmpty()){
			const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
			if (selectionInfoPtr != NULL){
				IElementSelectionInfo::Elements selectedElements = selectionInfoPtr->GetSelectedElements();
				if (!selectedElements.isEmpty()){
					const icomp::IRegistry::ElementInfo* firstElementInfo = selectedElements.begin().value();
					Q_ASSERT(firstElementInfo != NULL);

					const icomp::IComponentStaticInfo* componentInfoPtr = GetComponentMetaInfo(firstElementInfo->address);
					if (componentInfoPtr != NULL){
						attributeStaticInfoPtr = componentInfoPtr->GetAttributeInfo(attributeId);
					}
				}
			}
		}
	}
	else{
		AttributeTree->reset();
	}

	if (m_attributeSelectionObserverCompPtr.IsValid()){
		m_attributeSelectionObserverCompPtr->OnAttributeSelected(attributeStaticInfoPtr);
	}
}


void CAttributeEditorComp::on_AttributeTree_itemChanged(QTreeWidgetItem* item, int column)
{
	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	QByteArray attributeId = item->data(AC_VALUE, AttributeId).toString().toLocal8Bit();
	int attributeStatMeaning = item->data(AC_VALUE, AttributeMining).toInt();

	if (column == AC_NAME){
		istd::CChangeNotifier registryNotifier(registryPtr, istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);

		IElementSelectionInfo::Elements selectedElements = objectPtr->GetSelectedElements();
		for (		IElementSelectionInfo::Elements::ConstIterator iter = selectedElements.constBegin();
					iter != selectedElements.constEnd();
					++iter){
			const icomp::IRegistry::ElementInfo* selectedInfoPtr = iter.value();
			Q_ASSERT(selectedInfoPtr != NULL);

			icomp::IRegistryElement* elementPtr = selectedInfoPtr->elementPtr.Cast<icomp::IRegistryElement*>();
			if (elementPtr == NULL){
				continue;
			}

			istd::CChangeNotifier elementNotifier(elementPtr, istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);

			if (item->checkState(AC_NAME) == Qt::Unchecked){
				icomp::IRegistryElement::AttributeInfo* attributeInfoPtr =
							const_cast<icomp::IRegistryElement::AttributeInfo*>(elementPtr->GetAttributeInfo(attributeId));
				if (attributeInfoPtr != NULL){
					if ((attributeStatMeaning >= AM_REFERENCE) && (attributeStatMeaning <= AM_MULTI_ATTRIBUTE)){
						attributeInfoPtr->attributePtr.Reset();
					}
					else if (attributeStatMeaning == AM_EXPORTED_ATTR){
						attributeInfoPtr->exportId = "";
					}

					if (!attributeInfoPtr->attributePtr.IsValid() && attributeInfoPtr->exportId.isEmpty()){
						elementPtr->RemoveAttribute(attributeId);
					}
				}
			}
		}

		Q_EMIT AfterAttributesChange();
	}
}


void CAttributeEditorComp::on_InterfacesTree_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> items = InterfacesTree->selectedItems();
	if ((items.count() > 0) && m_quickHelpViewerCompPtr.IsValid()){
		QTreeWidgetItem* attributeItemPtr = items.at(0);

		istd::CClassInfo info(attributeItemPtr->data(0, InterfaceName).toString().toLocal8Bit());

		m_quickHelpViewerCompPtr->ShowHelp(info.GetName(), &info);
	}
}


void CAttributeEditorComp::on_InterfacesTree_itemChanged(QTreeWidgetItem* item, int column)
{
	Q_ASSERT(item != NULL);

	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	if (column == 0){
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

		QString interfaceName = item->data(0, InterfaceName).toString();
		QByteArray elementName = item->data(0, ElementId).toString().toLocal8Bit();

		bool isSelected = (item->checkState(column) == Qt::Checked);
		registryPtr->SetElementInterfaceExported(elementName, interfaceName.toLocal8Bit(), isSelected);

		Q_EMIT AfterInterfacesChange();
	}
}


void CAttributeEditorComp::on_AutoInstanceCB_toggled(bool checked)
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker updateBlocker(this);

	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	istd::CChangeNotifier registryNotifier(registryPtr, istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_FLAGS_CHANGED);

	IElementSelectionInfo::Elements selectedElements = objectPtr->GetSelectedElements();
	for (		IElementSelectionInfo::Elements::ConstIterator iter = selectedElements.constBegin();
				iter != selectedElements.constEnd();
				++iter){
		const icomp::IRegistry::ElementInfo* selectedInfoPtr = iter.value();
		Q_ASSERT(selectedInfoPtr != NULL);

		istd::TChangeNotifier<icomp::IRegistryElement> elementPtr(selectedInfoPtr->elementPtr.GetPtr(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);

		if (!elementPtr.IsValid()){
			continue;
		}

		quint32 flags = elementPtr->GetElementFlags();

		flags = checked?
					(flags | icomp::IRegistryElement::EF_AUTO_INSTANCE):
					(flags & ~icomp::IRegistryElement::EF_AUTO_INSTANCE);

		elementPtr->SetElementFlags(flags);
	}
}


void CAttributeEditorComp::UpdateGeneralView()
{
	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	const icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	QString names;

	IElementSelectionInfo::Elements selectedElements = objectPtr->GetSelectedElements();
	for (		IElementSelectionInfo::Elements::ConstIterator iter = selectedElements.constBegin();
				iter != selectedElements.constEnd();
				++iter){
		const icomp::IRegistry::ElementInfo* selectedInfoPtr = iter.value();
		Q_ASSERT(selectedInfoPtr != NULL);

		const icomp::IRegistryElement* elementPtr = selectedInfoPtr->elementPtr.GetPtr();
		if (elementPtr != NULL){
			const QByteArray& elementId = iter.key();

			if (!names.isEmpty()){
				names += "\n";
			}
			names += elementId;
		}
	}

	QString description;
	QStringList companyInfoList;
	QStringList projectInfoList;
	QStringList authorInfoList;
	QStringList categoryInfoList;
	QStringList tagInfoList;
	QStringList keywordInfoList;

	icomp::CComponentAddress commonAddress;
	bool isMultiType = false;

	for (		AddressToInfoMap::ConstIterator infoIter = m_adressToMetaInfoMap.constBegin();
				infoIter != m_adressToMetaInfoMap.constEnd();
				++infoIter){
		const icomp::CComponentAddress& address = infoIter.key();
		const icomp::IComponentStaticInfo* infoPtr = infoIter.value().GetPtr();
		if (infoPtr != NULL){
			description = infoPtr->GetDescription();

			icomp::CComponentMetaDescriptionEncoder encoder(infoPtr->GetKeywords());

			companyInfoList += (encoder.GetValues("Company"));
			projectInfoList += (encoder.GetValues("Project"));
			authorInfoList += (encoder.GetValues("Author"));
			categoryInfoList += (encoder.GetValues("Category"));
			tagInfoList += (encoder.GetValues("Tag"));
			keywordInfoList += (encoder.GetUnassignedKeywords());
		}
		else{
			companyInfoList += tr("<unknown>");
			projectInfoList += tr("<unknown>");
			authorInfoList += tr("<unknown>");
			categoryInfoList += tr("<unknown>");
			tagInfoList += tr("<unknown>");
			keywordInfoList += tr("<unknown>");
		}

		if (!commonAddress.IsValid()){
			commonAddress = address;
		}
		else if (commonAddress != address){
			isMultiType = true;
		}
	}

	if (!description.isEmpty() && !isMultiType){
		DescriptionLabel->setText(description);
		DescriptionLabel->setVisible(true);
	}
	else{
		DescriptionLabel->setVisible(false);
	}

	QIcon componentIcon;
	if (commonAddress.IsValid()){
		if (isMultiType){
			AddressLabel->setText(tr("<multiple component types>"));
		}
		else{
			AddressLabel->setText(commonAddress.ToString());
			if (m_consistInfoCompPtr.IsValid()){
				componentIcon = m_consistInfoCompPtr->GetComponentIcon(commonAddress);
			}
		}

		AddressLabel->setVisible(true);
	}
	else{
		AddressLabel->setVisible(false);
	}
	if (!componentIcon.isNull()){
		IconLabel->setPixmap(componentIcon.pixmap(128));
		IconLabel->setVisible(true);
	}
	else{
		IconLabel->setVisible(false);
	}

	companyInfoList.removeDuplicates();
	CompanyLabel->setText(companyInfoList.join(", "));
	projectInfoList.removeDuplicates();
	ProjectLabel->setText(projectInfoList.join(", "));
	authorInfoList.removeDuplicates();
	AuthorLabel->setText(authorInfoList.join(", "));
	categoryInfoList.removeDuplicates();
	CategoryLabel->setText(categoryInfoList.join(", "));
	tagInfoList.removeDuplicates();
	TagsLabel->setText(tagInfoList.join(", "));
	keywordInfoList.removeDuplicates();
	KeywordsLabel->setText(keywordInfoList.join(", "));

	NameLabel->setText(names);

	MetaInfoFrame->setVisible(!m_adressToMetaInfoMap.isEmpty());
}


void CAttributeEditorComp::UpdateAttributesView()
{
	iqt::CSignalBlocker signalBlocker(AttributeTree);

	m_attrInfosMap.clear();

	bool hasError = false;
	bool hasWarning = false;
	bool hasImport = false;
	int itemIndex = 0;

	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();
		if (registryPtr != NULL){
			bool isElementCorrect = true;

			IElementSelectionInfo::Elements selectedElements = objectPtr->GetSelectedElements();
			for (		IElementSelectionInfo::Elements::ConstIterator iter = selectedElements.constBegin();
						iter != selectedElements.constEnd();
						++iter){
				const QByteArray& elementId = iter.key();

				// check general element consistency
				if (m_consistInfoCompPtr.IsValid()){
					isElementCorrect = isElementCorrect && m_consistInfoCompPtr->IsElementValid(
								elementId,
								*registryPtr,
								true,
								false,
								NULL);
				}

				const icomp::IRegistry::ElementInfo* selectedInfoPtr = iter.value();
				Q_ASSERT(selectedInfoPtr != NULL);

				// creating map of attributes based on registry element data
				icomp::IRegistryElement* elementPtr = selectedInfoPtr->elementPtr.GetPtr();
				if (elementPtr != NULL){
					icomp::IRegistryElement::Ids attributeIds = elementPtr->GetAttributeIds();
					for (		icomp::IRegistryElement::Ids::ConstIterator attrIter = attributeIds.constBegin();
								attrIter != attributeIds.constEnd();
								++attrIter){
						const QByteArray& attributeId = *attrIter;

						icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = const_cast<icomp::IRegistryElement::AttributeInfo*>(elementPtr->GetAttributeInfo(attributeId));
						if (attributeInfoPtr != NULL){
							AttrInfo& attrInfo = m_attrInfosMap[attributeId][elementId];

							attrInfo.elementPtr.SetPtr(elementPtr);
							attrInfo.infoPtr.SetPtr(attributeInfoPtr);
						}
					}
				}

				// creating map of attributes based on static meta info
				const icomp::IComponentStaticInfo* infoPtr = GetComponentMetaInfo(selectedInfoPtr->address);
				if (infoPtr != NULL){
					icomp::IElementStaticInfo::Ids attributeIds = infoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_ATTRIBUTES);
					for (		icomp::IElementStaticInfo::Ids::ConstIterator attrIter = attributeIds.constBegin();
								attrIter != attributeIds.constEnd();
								++attrIter){
						const QByteArray& attributeId = *attrIter;

						AttrInfo& attrInfo = m_attrInfosMap[attributeId][elementId];

						Q_ASSERT(!(attrInfo.elementPtr.IsValid() && (elementPtr == NULL)));	// check if we dont reset existing element pointer, it shouldn't happen
						attrInfo.elementPtr.SetPtr(elementPtr);
						attrInfo.staticInfoPtr.SetPtr(infoPtr->GetAttributeInfo(attributeId));
					}
				}
			}

			for (		AttrInfosMap::ConstIterator treeIter = m_attrInfosMap.constBegin();
						treeIter != m_attrInfosMap.constEnd();
						++treeIter, ++itemIndex){
				const QByteArray& attributeId = treeIter.key();
				const ElementIdToAttrInfoMap& attrInfos = treeIter.value();

				QTreeWidgetItem* attributeItemPtr = NULL;
				if (itemIndex < AttributeTree->topLevelItemCount()){
					attributeItemPtr = AttributeTree->topLevelItem(itemIndex);
				}
				else{
					attributeItemPtr = new QTreeWidgetItem();
					AttributeTree->addTopLevelItem(attributeItemPtr);
				}

				if (attributeItemPtr != NULL){
					bool errorFlag = false;
					bool warningFlag = false;
					bool exportFlag = false;
					SetAttributeToItem(
								*attributeItemPtr,
								*registryPtr,
								attributeId,
								attrInfos,
								errorFlag,
								warningFlag,
								exportFlag);

					hasError = hasError || errorFlag;
					hasWarning = hasWarning || warningFlag;
					hasImport = hasImport || exportFlag;
				}
			}
		}
	}

	while (itemIndex < AttributeTree->topLevelItemCount()){
		delete AttributeTree->topLevelItem(itemIndex);
	}

	AttributeTree->resizeColumnToContents(AC_NAME);

	if (hasError){
		ElementInfoTab->setTabIcon(TI_ATTRIBUTES, m_invalidIcon);
	}
	else if (hasWarning){
		ElementInfoTab->setTabIcon(TI_ATTRIBUTES, m_warningIcon);
	}
	else if (hasImport){
		ElementInfoTab->setTabIcon(TI_ATTRIBUTES, m_importIcon);
	}
	else{
		ElementInfoTab->setTabIcon(TI_ATTRIBUTES, QIcon());
	}
}


void CAttributeEditorComp::UpdateInterfacesView()
{
	iqt::CSignalBlocker signalBlocker(InterfacesTree);

	bool hasWarning = false;
	bool hasExport = false;
	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();
		if (registryPtr != NULL){
			icomp::IRegistry::ExportedInterfacesMap registryInterfaces = registryPtr->GetExportedInterfacesMap();

			IElementSelectionInfo::Elements selectedElements = objectPtr->GetSelectedElements();

			int itemIndex = 0;

			for (		IElementSelectionInfo::Elements::ConstIterator iter = selectedElements.constBegin();
						iter != selectedElements.constEnd();
						++iter){
				const icomp::IRegistry::ElementInfo* selectedInfoPtr = iter.value();
				Q_ASSERT(selectedInfoPtr != NULL);

				const QByteArray& elementId = iter.key();

				QTreeWidgetItem* itemPtr = NULL;
				if (selectedElements.size() > 1){
					if (itemIndex < InterfacesTree->topLevelItemCount()){
						itemPtr = InterfacesTree->topLevelItem(itemIndex);
					}
					else{
						itemPtr = new QTreeWidgetItem();
						InterfacesTree->addTopLevelItem(itemPtr);
					}
					itemIndex++;
					Q_ASSERT(itemPtr != NULL);

					ResetItem(*itemPtr);
					itemPtr->setText(0, elementId);
				}

				const icomp::IComponentStaticInfo* staticInfoPtr = GetComponentMetaInfo(selectedInfoPtr->address);

				bool warningFlag = false;
				bool exportFlag = false;
				CreateInterfacesTree(
							elementId,
							staticInfoPtr,
							registryInterfaces,
							itemPtr,
							warningFlag,
							exportFlag,
							true);
				hasWarning = hasWarning || warningFlag;
				hasExport = hasExport || exportFlag;
			}

			if (selectedElements.size() != 1){
				while (itemIndex < InterfacesTree->topLevelItemCount()){
					delete InterfacesTree->topLevelItem(itemIndex);
				}
			}
		}
	}

	if (hasWarning){
		ElementInfoTab->setTabIcon(TI_INTERFACES, m_warningIcon);
	}
	else if (hasExport){
		ElementInfoTab->setTabIcon(TI_INTERFACES, m_exportIcon);
	}
	else{
		ElementInfoTab->setTabIcon(TI_INTERFACES, QIcon());
	}
}


void CAttributeEditorComp::UpdateFlagsView()
{
	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	const icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	IElementSelectionInfo::Elements selectedElements = objectPtr->GetSelectedElements();
	bool autoInstanceOn = false;
	bool autoInstanceOff = false;

	for (		IElementSelectionInfo::Elements::ConstIterator iter = selectedElements.constBegin();
				iter != selectedElements.constEnd();
				++iter){
		const icomp::IRegistry::ElementInfo* selectedInfoPtr = iter.value();
		Q_ASSERT(selectedInfoPtr != NULL);

		const icomp::IRegistryElement* elementPtr = selectedInfoPtr->elementPtr.GetPtr();
		if (elementPtr != NULL){
			quint32 elementFlags = elementPtr->GetElementFlags();
			if ((elementFlags & icomp::IRegistryElement::EF_AUTO_INSTANCE) != 0){
				autoInstanceOn = true;
			}
			else{
				autoInstanceOff = true;
			}
		}
	}

	if (autoInstanceOn){
		if (autoInstanceOff){
			AutoInstanceCB->setCheckState(Qt::PartiallyChecked);
		}
		else{
			AutoInstanceCB->setTristate(false);
			AutoInstanceCB->setCheckState(Qt::Checked);
		}
		AutoInstanceCB->setEnabled(true);
	}
	else{
		if (autoInstanceOff){
			AutoInstanceCB->setTristate(false);
			AutoInstanceCB->setCheckState(Qt::Unchecked);
			AutoInstanceCB->setEnabled(true);
		}
		else{
			AutoInstanceCB->setCheckState(Qt::PartiallyChecked);
			AutoInstanceCB->setEnabled(false);
		}
	}
}


void CAttributeEditorComp::UpdateSubcomponentsView()
{
	iqt::CSignalBlocker signalBlocker(ComponentsTree);

	bool hasWarning = false;
	bool hasExport = false;
	int itemIndex = 0;

	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();
		if (registryPtr != NULL){
			IElementSelectionInfo::Elements selectedElements = objectPtr->GetSelectedElements();
			for (		IElementSelectionInfo::Elements::ConstIterator iter = selectedElements.constBegin();
						iter != selectedElements.constEnd();
						++iter){
				const QByteArray& elementId = iter.key();
				const icomp::IRegistry::ElementInfo* selectedInfoPtr = iter.value();
				Q_ASSERT(selectedInfoPtr != NULL);

				const icomp::IRegistryElement* elementPtr = selectedInfoPtr->elementPtr.GetPtr();
				if (elementPtr != NULL){
					QTreeWidgetItem* componentRootPtr = NULL;
					if (itemIndex < ComponentsTree->topLevelItemCount()){
						componentRootPtr = ComponentsTree->topLevelItem(itemIndex);
					}
					else{
						componentRootPtr = new QTreeWidgetItem();
						ComponentsTree->addTopLevelItem(componentRootPtr);
					}
					Q_ASSERT(componentRootPtr != NULL);

					const icomp::IComponentStaticInfo* infoPtr = GetComponentMetaInfo(selectedInfoPtr->address);

					bool warningFlag = false;
					bool exportFlag = false;
					CreateExportedComponentsTree(elementId, elementId, infoPtr, *componentRootPtr, warningFlag, exportFlag);
					hasWarning = hasWarning || warningFlag;
					hasExport = hasExport || exportFlag;

					++itemIndex;
				}
			}
		}
	}

	while (itemIndex < ComponentsTree->topLevelItemCount()){
		delete ComponentsTree->topLevelItem(itemIndex);
	}

	ComponentsTree->resizeColumnToContents(AC_NAME);

	if (hasWarning){
		ElementInfoTab->setTabIcon(TI_EXPORTS, m_warningIcon);
	}
	else if (hasExport){
		ElementInfoTab->setTabIcon(TI_EXPORTS, m_exportIcon);
	}
	else{
		ElementInfoTab->setTabIcon(TI_EXPORTS, QIcon());
	}
}


// protected methods

bool CAttributeEditorComp::SetAttributeToItem(
			QTreeWidgetItem& attributeItem,
			const icomp::IRegistry& registry,
			const QByteArray& attributeId,
			const ElementIdToAttrInfoMap& infos,
			bool& hasError,
			bool& hasWarning,
			bool& hasExport) const
{
	QTreeWidgetItem* importItemPtr = NULL;
	if (attributeItem.childCount() < 1){
		importItemPtr = new QTreeWidgetItem(&attributeItem);
	}
	else{
		importItemPtr = attributeItem.child(0);
	}

	bool isAttributeEditable = true;
	bool isAttributeEnabled = false;
	bool isAttributeObligatory = false;
	bool isAttributeWarning = false;
	bool isAttributeError = false;
	QString attributeValueText;
	int attributeStatMeaning = AM_NONE;
	int attributeValueMeaning = AM_NONE;
	QString attributeImportText;
	QByteArray attributeExportValue;
	QString attributeValueTip;
	QString attributeDescription;

	QByteArray attributeValueTypeId;
	QByteArray attributeStatTypeId;
	icomp::IElementStaticInfo::Ids obligatoryInterfaces;
	icomp::IElementStaticInfo::Ids optionalInterfaces;

	for (		ElementIdToAttrInfoMap::ConstIterator attrsIter = infos.constBegin();
				attrsIter != infos.constEnd();
				++attrsIter){
		const QByteArray& elementId = attrsIter.key();
		const AttrInfo& attrInfo = attrsIter.value();

		if (m_consistInfoCompPtr.IsValid() && !m_consistInfoCompPtr->IsAttributeValid(
						attributeId,
						elementId,
						registry,
						true,
						false,
						NULL)){
			isAttributeError = true;
		}

		if (attrInfo.infoPtr.IsValid()){
			if (!attributeExportValue.isEmpty() && (attrInfo.infoPtr->exportId != attributeExportValue)){
				attributeImportText = tr("<multi selection>");
			}
			else{
				attributeExportValue = attrInfo.infoPtr->exportId;
				attributeImportText = attributeExportValue;
			}
		}

		const iser::IObject* attributePtr = NULL;
		if (attrInfo.infoPtr.IsValid() && attrInfo.infoPtr->attributePtr.IsValid()){
			isAttributeEnabled = true;
			attributePtr = attrInfo.infoPtr->attributePtr.GetPtr();
		}
		else if (attrInfo.staticInfoPtr != NULL){
			attributePtr = attrInfo.staticInfoPtr->GetAttributeDefaultValue();
		}
		QString text;

		QByteArray currentAttrTypeId;
		if (attrInfo.infoPtr.IsValid()){
			currentAttrTypeId = attrInfo.infoPtr->attributeTypeName;
			if (!currentAttrTypeId.isEmpty()){
				if (attributeValueTypeId.isEmpty()){
					attributeValueTypeId = currentAttrTypeId;
				}
			}
		}

		if (attrInfo.staticInfoPtr != NULL){
			QByteArray statAttrTypeId = attrInfo.staticInfoPtr->GetAttributeTypeName();
			if (attributeStatTypeId.isEmpty()){
				attributeStatTypeId = statAttrTypeId;
			}
			else if (attributeStatTypeId != statAttrTypeId){
				if (!attributeValueTip.isEmpty()){
					attributeValueTip += "\n";
				}
				attributeValueTip += tr("More elements selected with the same attribute name and different type");
				isAttributeEditable = false;
				break;
			}

			int attributeFlags = attrInfo.staticInfoPtr->GetAttributeFlags();

			int meaning = AM_NONE;

			if ((attributeFlags & (icomp::IAttributeStaticInfo::AF_REFERENCE | icomp::IAttributeStaticInfo::AF_FACTORY)) != 0){
				if ((attributeFlags & icomp::IAttributeStaticInfo::AF_MULTIPLE) != 0){
					meaning = AM_MULTI_REFERENCE;
				}
				else{
					meaning = AM_REFERENCE;
				}
			}
			else{
				if ((attributeFlags & icomp::IAttributeStaticInfo::AF_MULTIPLE) != 0){
					meaning = AM_MULTI_ATTRIBUTE;
				}
				else{
					if (statAttrTypeId == icomp::TAttribute<bool>::GetTypeName()){
						meaning = AM_BOOL_ATTRIBUTE;
					}
					else{
						meaning = AM_ATTRIBUTE;
					}
				}
			}

			if ((attributeStatMeaning != AM_NONE) && (meaning != attributeStatMeaning)){
				attributeStatMeaning = AM_MULTI;
			}
			else{
				attributeStatMeaning = meaning;
			}

			if ((attributeFlags & icomp::IAttributeStaticInfo::AF_OBLIGATORY) != 0){
				isAttributeObligatory = true;
			}
			QString description = attrInfo.staticInfoPtr->GetAttributeDescription();
			if (!description.isEmpty()){
				if (!attributeDescription.isEmpty()){
					attributeDescription += "\n";
				}
				attributeDescription = description;
			}

			icomp::IElementStaticInfo::Ids interfaces = attrInfo.staticInfoPtr->GetRelatedMetaIds(
						icomp::IComponentStaticInfo::MGI_INTERFACES,
						icomp::IAttributeStaticInfo::AF_OBLIGATORY,
						icomp::IAttributeStaticInfo::AF_OBLIGATORY);	// Names of obligatory interfaces
			obligatoryInterfaces += interfaces;
			interfaces = attrInfo.staticInfoPtr->GetRelatedMetaIds(
						icomp::IComponentStaticInfo::MGI_INTERFACES,
						0,
						icomp::IAttributeStaticInfo::AF_OBLIGATORY);	// Names of optional interfaces
			optionalInterfaces += interfaces;
		}

		int valueMeaning;
		if ((attributePtr != NULL) && DecodeAttribute(*attributePtr, text, valueMeaning)){
			if (!attributeValueText.isEmpty() && (text != attributeValueText)){
				attributeValueText = tr("<multi selection>");
			}
			else{
				attributeValueText = text;
			}

			if ((attributeValueMeaning != AM_NONE) && (valueMeaning != attributeValueMeaning)){
				attributeValueMeaning = AM_MULTI;
			}
			else{
				attributeValueMeaning = valueMeaning;
			}
		}
	}

	// get the attribute meaning from registry if it is not defined in packages
	if (attributeStatMeaning == AM_NONE){
		attributeStatMeaning = attributeValueMeaning;

		if (!attributeValueTip.isEmpty()){
			attributeValueTip += "\n";
		}
		attributeValueTip += tr("Attribute doesn't exist in package (was removed?)");
		isAttributeWarning = true;
	}

	// get the attribute type from registry if it is not defined in packages
	if (attributeStatTypeId.isEmpty()){
		attributeStatTypeId = attributeValueTypeId;
	}
	else if (!attributeValueTypeId.isEmpty() && (attributeValueTypeId != attributeStatTypeId)){
		attributeValueTip += tr("Attribute type in package differs from registry");
		isAttributeError = true;
	}

	if (attributeValueTip.isEmpty()){
		QString attributeTypeDescription;
		AttributeTypesMap::ConstIterator foundTypeNameIter = m_attributeTypesMap.constFind(attributeStatTypeId);
		if (foundTypeNameIter != m_attributeTypesMap.constEnd()){
			attributeTypeDescription = foundTypeNameIter.value();
		}
		else{
			attributeTypeDescription = tr("unsupported attribute of type '%1'").arg(QString(attributeStatTypeId));
		}

		if (!isAttributeObligatory){
			attributeTypeDescription = tr("Optional %1").arg(attributeTypeDescription);
		}

		attributeValueTip = (attributeTypeDescription.isEmpty() || attributeDescription.isEmpty())?
					attributeDescription + attributeTypeDescription:
					tr("%1\nType: %2").arg(attributeDescription).arg(attributeTypeDescription);

		if (!obligatoryInterfaces.isEmpty() || !optionalInterfaces.isEmpty()){
			attributeValueTip += tr("\nInterfaces:");
			for (		icomp::IElementStaticInfo::Ids::ConstIterator obligIter = obligatoryInterfaces.constBegin();
						obligIter != obligatoryInterfaces.constEnd();
						++obligIter){
				attributeValueTip += tr("\n - %1").arg(QString(*obligIter));
			}
			for (		icomp::IElementStaticInfo::Ids::ConstIterator optIter = optionalInterfaces.constBegin();
						optIter != optionalInterfaces.constEnd();
						++optIter){
				attributeValueTip += tr("\n - %1 (optional)").arg(QString(*optIter));
			}
		}
	}

	QString attributeName = attributeId;
	attributeItem.setText(AC_NAME, attributeId);
	attributeItem.setData(AC_VALUE, AttributeId, attributeName);

	attributeItem.setText(AC_VALUE, attributeValueText);
	attributeItem.setData(AC_VALUE, AttributeTypeId, attributeStatTypeId);
	attributeItem.setData(AC_VALUE, AttributeMining, attributeStatMeaning);

	attributeItem.setToolTip(AC_NAME, attributeValueTip);
	attributeItem.setToolTip(AC_VALUE, attributeValueTip);

	attributeItem.setFlags(Qt::ItemIsSelectable);
	attributeItem.setData(AC_NAME, Qt::CheckStateRole, QVariant());
	if (isAttributeEditable){
		attributeItem.setFlags(attributeItem.flags() | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	}

	if (isAttributeEnabled){
		attributeItem.setFlags(attributeItem.flags() | Qt::ItemIsUserCheckable);
		attributeItem.setCheckState(AC_NAME, Qt::Checked);
	}

	if (isAttributeError){
		attributeItem.setBackgroundColor(AC_VALUE, Qt::red);
		hasError = true;
	}
	else if (isAttributeWarning){
		attributeItem.setBackgroundColor(AC_VALUE, Qt::yellow);
		hasWarning = true;
	}
	else{
		attributeItem.setBackgroundColor(AC_VALUE, Qt::white);
	}

	importItemPtr->setText(AC_NAME, tr("<import>"));
	importItemPtr->setData(AC_VALUE, AttributeMining, AM_EXPORTED_ATTR);
	importItemPtr->setData(AC_VALUE, AttributeId, attributeName);
	importItemPtr->setData(AC_VALUE, AttributeTypeId, attributeValueTypeId);
	importItemPtr->setText(AC_VALUE, attributeImportText);
	importItemPtr->setData(AC_VALUE, AttributeValue, attributeExportValue);

	if (!attributeImportText.isEmpty()){
		importItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
		importItemPtr->setCheckState(AC_NAME, Qt::Checked);

		attributeItem.setIcon(AC_NAME, m_importIcon);

		hasExport = true;
	}
	else{
		importItemPtr->setData(AC_NAME, Qt::CheckStateRole, QVariant());
		importItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		attributeItem.setIcon(AC_NAME, QIcon());
	}

	importItemPtr->setHidden(false);

	return true;
}


bool CAttributeEditorComp::SetInterfaceToItem(
			QTreeWidgetItem& item,
			icomp::IRegistry::ExportedInterfacesMap* interfacesMapPtr,
			const QByteArray& elementId,
			const QByteArray& interfaceName,
			bool& /*hasWarning*/,
			bool& hasExport) const
{
	item.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

	item.setData(0, InterfaceName, interfaceName);
	item.setData(0, ElementId, elementId);

	if (interfacesMapPtr != NULL){
		hasExport = false;
		icomp::IRegistry::ExportedInterfacesMap::Iterator foundExportIter = interfacesMapPtr->find(interfaceName);
		if ((foundExportIter != interfacesMapPtr->end()) && (foundExportIter.value() == elementId)){
			hasExport = true;
			interfacesMapPtr->erase(foundExportIter);
		}

		item.setText(0, interfaceName);
		item.setCheckState(0, hasExport? Qt::Checked: Qt::Unchecked);
		item.setToolTip(0, "");
		item.setBackgroundColor(0, Qt::white);
	}
	else{
		item.setText(0, elementId + ": " + interfaceName);
		item.setCheckState(0, Qt::Checked);
		item.setToolTip(0, tr("Interface doesn't implemented by this element (was removed?)"));
		item.setBackgroundColor(0, Qt::yellow);
	}
	item.setIcon(0, QIcon());

	while (item.childCount() > 0){
		delete item.child(0);
	}

	return true;
}


bool CAttributeEditorComp::ResetItem(QTreeWidgetItem& item)
{
	item.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	item.setData(0, InterfaceName, QVariant());
	item.setData(0, ElementId, QVariant());
	item.setText(0, "");
	item.setData(0, Qt::CheckStateRole, QVariant());
	item.setToolTip(0, "");
	item.setBackgroundColor(0, Qt::white);
	item.setIcon(0, QIcon());

	return true;
}


bool CAttributeEditorComp::DecodeAttribute(
			const iser::ISerializable& attribute,
			QString& text,
			int& meaning) const
{
	const icomp::CIntegerAttribute* intAttribute = dynamic_cast<const icomp::CIntegerAttribute*>(&attribute);
	if (intAttribute != NULL){
		text = QString::number(intAttribute->GetValue());
		meaning = AM_ATTRIBUTE;

		return true;
	}

	const icomp::CRealAttribute* doubleAttribute = dynamic_cast<const icomp::CRealAttribute*>(&attribute);
	if (doubleAttribute != NULL){
		text = QString::number(doubleAttribute->GetValue());
		meaning = AM_ATTRIBUTE;

		return true;
	}

	const icomp::CBooleanAttribute* boolAttribute = dynamic_cast<const icomp::CBooleanAttribute*>(&attribute);
	if (boolAttribute != NULL){
		text = boolAttribute->GetValue()? "true": "false";
		meaning = AM_BOOL_ATTRIBUTE;

		return true;
	}

	const icomp::CStringAttribute* stringAttribute = dynamic_cast<const icomp::CStringAttribute*>(&attribute);
	if (stringAttribute != NULL){
		text = EncodeToEdit(stringAttribute->GetValue());
		meaning = AM_ATTRIBUTE;

		return true;
	}

	const icomp::CIdAttribute* idPtr = dynamic_cast<const icomp::CIdAttribute*>(&attribute);
	if (idPtr != NULL){
		text = EncodeToEdit(idPtr->GetValue());

		if (dynamic_cast<const icomp::CReferenceAttribute*>(idPtr) != NULL){
			meaning = AM_REFERENCE;
		}
		else{
			meaning = AM_ATTRIBUTE;
		}

		return true;
	}

	const icomp::CStringListAttribute* stringListAttribute = dynamic_cast<const icomp::CStringListAttribute*>(&attribute);
	if (stringListAttribute != NULL){
		text.clear();

		for (int index = 0; index < stringListAttribute->GetValuesCount(); index++){
			if (!text.isEmpty()){
				text += ";";
			}

			text += EncodeToEdit(stringListAttribute->GetValueAt(index));
		}

		meaning = AM_MULTI_ATTRIBUTE;

		return true;
	}

	const icomp::CIntegerListAttribute* intListAttribute = dynamic_cast<const icomp::CIntegerListAttribute*>(&attribute);
	if (intListAttribute != NULL){
		text.clear();

		for (int index = 0; index < intListAttribute->GetValuesCount(); index++){
			if (!text.isEmpty()){
				text += ";";
			}

			text += QString::number(intListAttribute->GetValueAt(index));
		}

		meaning = AM_MULTI_ATTRIBUTE;

		return true;
	}

	const icomp::CRealListAttribute* doubleListAttribute = dynamic_cast<const icomp::CRealListAttribute*>(&attribute);
	if (doubleListAttribute != NULL){
		text.clear();

		for (int index = 0; index < doubleListAttribute->GetValuesCount(); index++){
			if (!text.isEmpty()){
				text += ";";
			}

			text += QString::number(doubleListAttribute->GetValueAt(index));
		}

		meaning = AM_MULTI_ATTRIBUTE;

		return true;
	}

	const icomp::CBooleanListAttribute* boolListAttribute = dynamic_cast<const icomp::CBooleanListAttribute*>(&attribute);
	if (boolListAttribute != NULL){
		text.clear();

		for (int index = 0; index < boolListAttribute->GetValuesCount(); index++){
			if (!text.isEmpty()){
				text += ";";
			}

			text += doubleListAttribute->GetValueAt(index)? "true": "false";
		}

		meaning = AM_MULTI_ATTRIBUTE;

		return true;
	}

	const icomp::CIdListAttribute* multiIdPtr = dynamic_cast<const icomp::CIdListAttribute*>(&attribute);
	if (multiIdPtr != NULL){
		QString dependecyString;

		int idsCount = multiIdPtr->GetValuesCount();
		for (int idIndex = 0; idIndex < idsCount; idIndex++){
			if (!text.isEmpty()){
				text += ";";
			}

			QString componentId = multiIdPtr->GetValueAt(idIndex);

			text += EncodeToEdit(componentId);
		}

		if (dynamic_cast<const icomp::CMultiReferenceAttribute*>(multiIdPtr) != NULL){
			meaning = AM_MULTI_REFERENCE;
		}
		else{
			meaning = AM_MULTI_ATTRIBUTE;
		}

		return true;
	}

	return false;
}


bool CAttributeEditorComp::EncodeAttribute(
			const QString& text,
			int attributeStatMeaning,
			iser::ISerializable& result) const
{
	// set single reference of factory data
	if (attributeStatMeaning == AM_REFERENCE){
		icomp::TAttribute<QByteArray>* referenceAttributePtr = dynamic_cast<icomp::TAttribute<QByteArray>*>(&result);
		if (referenceAttributePtr != NULL){
			referenceAttributePtr->SetValue(DecodeFromEdit(text).toLocal8Bit());

			return true;
		}
	}
	// set multiple reference data
	else if (attributeStatMeaning == AM_MULTI_REFERENCE){
		QStringList references = text.split(';',QString::SkipEmptyParts);

		icomp::TMultiAttribute<QByteArray>* multiReferenceAttributePtr = dynamic_cast<icomp::TMultiAttribute<QByteArray>*>(&result);

		if (multiReferenceAttributePtr != NULL){
			multiReferenceAttributePtr->Reset();
			for (int index = 0; index < references.count(); index++){
				multiReferenceAttributePtr->InsertValue(DecodeFromEdit(references.at(index)).toLocal8Bit());
			}

			return true;
		}
	}
	// set attribute data:
	else if (attributeStatMeaning == AM_ATTRIBUTE){
		icomp::CIntegerAttribute* intAttributePtr = dynamic_cast<icomp::CIntegerAttribute*>(&result);
		if (intAttributePtr != NULL){
			intAttributePtr->SetValue(text.toInt());

			return true;
		}

		icomp::CRealAttribute* doubleAttributePtr = dynamic_cast<icomp::CRealAttribute*>(&result);
		if (doubleAttributePtr != NULL){
			doubleAttributePtr->SetValue(text.toDouble());

			return true;
		}

		icomp::CStringAttribute* stringAttributePtr = dynamic_cast<icomp::CStringAttribute*>(&result);
		if (stringAttributePtr != NULL){
			stringAttributePtr->SetValue(DecodeFromEdit(text));

			return true;
		}

		icomp::CIdAttribute* idAttributePtr = dynamic_cast<icomp::CIdAttribute*>(&result);
		if (idAttributePtr != NULL){
			idAttributePtr->SetValue(DecodeFromEdit(text).toLocal8Bit());

			return true;
		}
	}
	else if (attributeStatMeaning == AM_BOOL_ATTRIBUTE){
		icomp::CBooleanAttribute* boolAttributePtr = dynamic_cast<icomp::CBooleanAttribute*>(&result);
		if (boolAttributePtr != NULL){
			boolAttributePtr->SetValue(text == "true");

			return true;
		}
	}
	else if (attributeStatMeaning == AM_MULTI_ATTRIBUTE){
		QStringList values = text.split(';');

		icomp::CIntegerListAttribute* intListAttributePtr = dynamic_cast<icomp::CIntegerListAttribute*>(&result);
		if (intListAttributePtr != NULL){
			intListAttributePtr->Reset();
			for (int index = 0; index < values.count(); index++){
				intListAttributePtr->InsertValue(values.at(index).toInt());
			}

			return true;
		}

		icomp::CRealListAttribute* doubleListAttributePtr = dynamic_cast<icomp::CRealListAttribute*>(&result);
		if (doubleListAttributePtr != NULL){
			doubleListAttributePtr->Reset();
			for (int index = 0; index < values.count(); index++){
				doubleListAttributePtr->InsertValue(values.at(index).toDouble());
			}

			return true;
		}

		icomp::CBooleanListAttribute* boolListAttributePtr = dynamic_cast<icomp::CBooleanListAttribute*>(&result);
		if (boolListAttributePtr != NULL){
			boolListAttributePtr->Reset();
			for (int index = 0; index < values.count(); index++){
				boolListAttributePtr->InsertValue(values.at(index) == "true");
			}

			return true;
		}

		icomp::CStringListAttribute* stringListAttributePtr = dynamic_cast<icomp::CStringListAttribute*>(&result);
		if (stringListAttributePtr != NULL){
			stringListAttributePtr->Reset();
			for (int index = 0; index < values.count(); index++){
				stringListAttributePtr->InsertValue(DecodeFromEdit(values.at(index)));
			}

			return true;
		}

		icomp::CIdListAttribute* idListAttributePtr = dynamic_cast<icomp::CIdListAttribute*>(&result);
		if (idListAttributePtr != NULL){
			idListAttributePtr->Reset();
			for (int index = 0; index < values.count(); index++){
				idListAttributePtr->InsertValue(DecodeFromEdit(values.at(index)).toLocal8Bit());
			}

			return true;
		}
	}

	return false;
}


void CAttributeEditorComp::CreateInterfacesTree(
			const QByteArray& elementId,
			const icomp::IElementStaticInfo* infoPtr,
			icomp::IRegistry::ExportedInterfacesMap& registryInterfaces,
			QTreeWidgetItem* parentItemPtr,
			bool& hasWarning,
			bool& hasExport,
			bool includeSubelement)
{
	hasWarning = false;
	hasExport = false;

	int itemIndex = 0;

	// display all interfaces based on meta information
	if (infoPtr != NULL){
		const icomp::IElementStaticInfo::Ids& interfaceIds = infoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES);
		for (		icomp::IElementStaticInfo::Ids::ConstIterator interfaceIter = interfaceIds.constBegin();
					interfaceIter != interfaceIds.constEnd();
					interfaceIter++){
			const QByteArray& interfaceName = *interfaceIter;

			QTreeWidgetItem* itemPtr = NULL;
			if (parentItemPtr != NULL){
				if (itemIndex < parentItemPtr->childCount()){
					itemPtr = parentItemPtr->child(itemIndex);
				}
				else{
					itemPtr = new QTreeWidgetItem(parentItemPtr);
					parentItemPtr->addChild(itemPtr);
				}
			}
			else{
				if (itemIndex < InterfacesTree->topLevelItemCount()){
					itemPtr = InterfacesTree->topLevelItem(itemIndex);
				}
				else{
					itemPtr = new QTreeWidgetItem();
					InterfacesTree->addTopLevelItem(itemPtr);
				}
			}
			itemIndex++;
			Q_ASSERT(itemPtr != NULL);

			bool warningFlag = false;
			bool exportFlag = false;
			SetInterfaceToItem(
						*itemPtr,
						&registryInterfaces,
						elementId,
						interfaceName,
						warningFlag,
						exportFlag);
			hasWarning = hasWarning || warningFlag;
			hasExport = hasExport || exportFlag;
		}

		if (includeSubelement){
			const icomp::IElementStaticInfo::Ids subcomponentIds = infoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_SUBELEMENTS);
			for (		icomp::IElementStaticInfo::Ids::ConstIterator subIter = subcomponentIds.constBegin();
						subIter != subcomponentIds.constEnd();
						++subIter){
				const QByteArray& sublementId = *subIter;

				QByteArray globalSublementId = istd::CIdManipBase::JoinId(elementId, sublementId);

				QTreeWidgetItem* itemPtr = NULL;
				if (parentItemPtr != NULL){
					if (itemIndex < parentItemPtr->childCount()){
						itemPtr = parentItemPtr->child(itemIndex);
					}
					else{
						itemPtr = new QTreeWidgetItem(parentItemPtr);
						parentItemPtr->addChild(itemPtr);
					}
				}
				else{
					if (itemIndex < InterfacesTree->topLevelItemCount()){
						itemPtr = InterfacesTree->topLevelItem(itemIndex);
					}
					else{
						itemPtr = new QTreeWidgetItem();
						InterfacesTree->addTopLevelItem(itemPtr);
					}
				}
				itemIndex++;
				Q_ASSERT(itemPtr != NULL);

				ResetItem(*itemPtr);
				itemPtr->setText(AC_NAME, sublementId);

				const icomp::IElementStaticInfo* subelementInfoPtr = infoPtr->GetSubelementInfo(sublementId);

				bool warningFlag = false;
				bool exportFlag = false;
				CreateInterfacesTree(
							globalSublementId,
							subelementInfoPtr,
							registryInterfaces,
							itemPtr,
							warningFlag,
							exportFlag,
							false);
				hasWarning = hasWarning || warningFlag;
				hasExport = hasExport || exportFlag;
			}
		}
	}

	// display all interfaces assigned to this element, but not existing in element meta information
	for (		icomp::IRegistry::ExportedInterfacesMap::ConstIterator interfaceIter = registryInterfaces.begin();
				interfaceIter != registryInterfaces.end();
				interfaceIter++){
		const QByteArray& interfaceName = interfaceIter.key();
		if (interfaceIter.value() == elementId){
			QTreeWidgetItem* itemPtr = NULL;
			if (itemIndex < InterfacesTree->topLevelItemCount()){
				itemPtr = InterfacesTree->topLevelItem(itemIndex);
			}
			else{
				itemPtr = new QTreeWidgetItem();
				InterfacesTree->addTopLevelItem(itemPtr);
			}
			itemIndex++;
			Q_ASSERT(itemPtr != NULL);

			bool warningFlag = false;
			bool exportFlag = false;
			SetInterfaceToItem(*itemPtr, NULL, elementId, interfaceName, warningFlag, exportFlag);
			hasWarning = true;
		}
	}

	if (parentItemPtr != NULL){
		while (parentItemPtr->childCount() > itemIndex){
			delete parentItemPtr->child(itemIndex);
		}

		if (hasWarning){
			parentItemPtr->setIcon(AC_NAME, m_warningIcon);
		}
		else if (hasExport){
			parentItemPtr->setIcon(AC_NAME, m_exportIcon);
		}
		else{
			parentItemPtr->setIcon(AC_NAME, QIcon());
		}
	}
	else{
		while (InterfacesTree->topLevelItemCount() > itemIndex){
			delete InterfacesTree->topLevelItem(itemIndex);
		}
	}
}


void CAttributeEditorComp::CreateExportedComponentsTree(
			const QByteArray& elementId,
			const QByteArray& globalElementId,
			const icomp::IElementStaticInfo* elementMetaInfoPtr,
			QTreeWidgetItem& item,
			bool& /*hasWarning*/,
			bool& hasExport) const
{
	int itemIndex = 0;

	QStringList exportedAliases = GetExportAliases(globalElementId);
	if (!exportedAliases.isEmpty()){
		hasExport = true;
	}

	item.setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	item.setText(AC_NAME, elementId);
	item.setData(AC_VALUE, AttributeId, QString(globalElementId));
	item.setData(AC_VALUE, AttributeValue, QString(elementId));
	item.setData(AC_VALUE, AttributeMining, AM_EXPORTED_COMP);
	item.setText(AC_VALUE, exportedAliases.join(";"));
	item.setExpanded(true);

	if (elementMetaInfoPtr != NULL){
		const icomp::IElementStaticInfo::Ids subcomponentIds = elementMetaInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_SUBELEMENTS);

		for (		icomp::IElementStaticInfo::Ids::ConstIterator subIter = subcomponentIds.constBegin();
					subIter != subcomponentIds.constEnd();
					++subIter, ++itemIndex){
			const QByteArray& sublementId = *subIter;

			QByteArray globalSublementId = istd::CIdManipBase::JoinId(globalElementId, sublementId);

			QStringList subExportedAliases = GetExportAliases(globalSublementId);
			if (!subExportedAliases.isEmpty()){
				hasExport = true;
			}

			QTreeWidgetItem* itemPtr = NULL;
			if (itemIndex < item.childCount()){
				itemPtr = item.child(itemIndex);
			}
			else{
				itemPtr = new QTreeWidgetItem(&item);
			}

			itemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
			itemPtr->setText(AC_NAME, sublementId);
			itemPtr->setData(AC_VALUE, AttributeId, QString(globalSublementId));
			itemPtr->setData(AC_VALUE, AttributeValue, QString(sublementId));
			itemPtr->setData(AC_VALUE, AttributeMining, AM_EXPORTED_COMP);
			itemPtr->setText(AC_VALUE, subExportedAliases.join(";"));
			itemPtr->setExpanded(true);

/*
			Q_ASSERT(itemPtr != NULL);

			const icomp::IElementStaticInfo* subcomponentInfoPtr = elementMetaInfoPtr->GetSubelementInfo(sublementId);

			bool warningFlag = false;
			bool exportFlag = false;
			CreateExportedComponentsTree(sublementId, globalSublementId, subcomponentInfoPtr, *itemPtr, &warningFlag, &exportFlag);
			hasWarning = hasWarning || warningFlag;
			hasExport = hasExport || exportFlag;
*/
		}
		item.setDisabled(false);
	}
	else{
		item.setDisabled(true);
	}

	while (itemIndex < item.childCount()){
		delete item.child(itemIndex);
	}
}


// reimplemented (TGuiObserverWrap)

void CAttributeEditorComp::OnGuiModelDetached()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		ElementInfoTab->setVisible(false);

		AttributeTree->clear();
		InterfacesTree->clear();
		ComponentsTree->clear();
	}

	BaseClass::OnGuiModelDetached();
}


void CAttributeEditorComp::UpdateGui(int updateFlags)
{
	Q_ASSERT(IsGuiCreated());

	if ((updateFlags & IElementSelectionInfo::CF_SELECTION) != 0){
		AttributeTree->reset();
	}

	UpdateAddressToMetaInfoMap();

	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();

		imod::IModel* registryModelPtr = dynamic_cast<imod::IModel*>(registryPtr);
		if (registryModelPtr != m_lastRegistryModelPtr){
			m_registryObserver.EnsureModelDetached();

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

		IElementSelectionInfo::Elements selectedElements = objectPtr->GetSelectedElements();

		if (selectedElements.isEmpty()){
			ElementInfoTab->setVisible(false);

			RegistryPropertiesFrame->setVisible(true);
		}
		else{
			RegistryPropertiesFrame->setVisible(false);

			ElementInfoTab->setVisible(true);
		}
	}

	UpdateGeneralView();
	UpdateAttributesView();
	UpdateInterfacesView();
	UpdateFlagsView();
	UpdateSubcomponentsView();
}


// reimplemented (CGuiComponentBase)

void CAttributeEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ElementInfoTab->setVisible(false);
	RegistryPropertiesFrame->setVisible(false);

	m_attributesTreeFilter.SetPtr(new iqtgui::CTreeWidgetFilter(AttributeTree));
	m_interfacesTreeFilter.SetPtr(new iqtgui::CTreeWidgetFilter(InterfacesTree));
	m_subcomponentsTreeFilter.SetPtr(new iqtgui::CTreeWidgetFilter(ComponentsTree));

	AttributeTree->setItemDelegate(&m_attributeItemDelegate);
	ComponentsTree->setItemDelegate(&m_attributeItemDelegate);

	m_invalidIcon.addFile(":/Icons/StateInvalid.svg");
	m_warningIcon.addFile(":/Icons/Warning.svg");
	m_exportIcon.addFile(":/Icons/Export.svg");
	m_importIcon.addFile(":/Icons/Import.svg");

	if (m_registryPropGuiCompPtr.IsValid()){
		m_registryPropGuiCompPtr->CreateGui(RegistryPropertiesFrame);
	}
}


void CAttributeEditorComp::OnGuiDestroyed()
{
	if (m_registryPropGuiCompPtr.IsValid()){
		m_registryPropGuiCompPtr->DestroyGui();
	}

	m_attributesTreeFilter.Reset();
	m_interfacesTreeFilter.Reset();
	m_subcomponentsTreeFilter.Reset();


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
	Q_ASSERT(parentPtr != NULL);
}


// reimplemented (QItemDelegate)

QWidget* CAttributeEditorComp::AttributeItemDelegate::createEditor(QWidget* parentWidget, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() != AC_VALUE){
		return NULL;
	}

	int propertyMining = index.data(AttributeMining).toInt();

	if (		propertyMining == AM_MULTI_ATTRIBUTE ||
				propertyMining == AM_MULTI_REFERENCE){
		QByteArray attributeId = index.data(AttributeId).toString().toLocal8Bit();

		int attributeFlags = 0;
		if (propertyMining == AM_MULTI_REFERENCE){
			attributeFlags = icomp::IAttributeStaticInfo::AF_REFERENCE;
		}
		else{
			attributeFlags = icomp::IAttributeStaticInfo::AF_VALUE;
		}

		return new CMultiAttributeDelegateWidget(const_cast<AttributeItemDelegate&>(*this), m_parent, parentWidget, attributeId, attributeFlags);
	}

	if (		(propertyMining == AM_REFERENCE) ||
				(propertyMining == AM_BOOL_ATTRIBUTE)){
		QByteArray attributeType = index.data(AttributeTypeId).toString().toLocal8Bit();
		QComboBox* comboEditor = new QComboBox(parentWidget);
		if (propertyMining == AM_BOOL_ATTRIBUTE){
			comboEditor->addItem("true");
			comboEditor->addItem("false");
		}
		else{
			QString text = index.data().toString();
			comboEditor->setEditable(true);
			comboEditor->setEditText(text);
		}

		return comboEditor;
	}

	return BaseClass::createEditor(parentWidget, option, index);
}


void CAttributeEditorComp::AttributeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	if (index.column() != AC_VALUE){
		BaseClass::setEditorData(editor, index);
	}

	int propertyMining = index.data(AttributeMining).toInt();

	QByteArray attributeName = index.data(AttributeId).toString().toLocal8Bit();
	QByteArray attributeValue = index.data(AttributeValue).toString().toLocal8Bit();
	if (attributeValue.isEmpty()){
		attributeValue = attributeName;
	}

	if (propertyMining == AM_EXPORTED_COMP){
		SetComponentExportEditor(attributeName, attributeValue, *editor);
	}
	else if (propertyMining == AM_EXPORTED_ATTR){
		SetAttributeExportEditor(attributeName, attributeValue, *editor);
	}
	else{
		SetAttributeValueEditor(attributeName, propertyMining, *editor);
	}
}


void CAttributeEditorComp::AttributeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	UpdateBlocker updateBlocker(&m_parent);

	if (index.column() != AC_VALUE){
		BaseClass::setModelData(editor, model, index);
	}

	QByteArray attributeId = index.data(AttributeId).toString().toLocal8Bit();
	QString newValue;

	const QComboBox* comboEditorPtr = dynamic_cast<const QComboBox*>(editor);
	if (comboEditorPtr != NULL){
		newValue = comboEditorPtr->currentText();
	}
	else{
		const CMultiAttributeDelegateWidget* multiAttributeEditorPtr = dynamic_cast<const CMultiAttributeDelegateWidget*>(editor);
		if (multiAttributeEditorPtr != NULL){
			newValue = multiAttributeEditorPtr->GetText();
		}
		else{
			newValue = editor->property("text").toString();
		}
	}

	int propertyMining = index.data(AttributeMining).toInt();
	if (propertyMining == AM_EXPORTED_COMP){
		// set attribute export for each selected component (if has the attribute)
		SetComponentExportData(attributeId, newValue);
	}
	else if ((propertyMining >= AM_REFERENCE) && (propertyMining <= AM_EXPORTED_ATTR)){
		// set attribute value for each selected component (if has the attribute)
		SetComponentValue(attributeId, propertyMining, newValue);
	}
}


// protected methods of embedded class AttributeItemDelegate

bool CAttributeEditorComp::AttributeItemDelegate::SetComponentExportEditor(const QByteArray& attributeId, const QString& defaultValue, QWidget& editor) const
{
	QString editorValue = m_parent.GetExportAliases(attributeId).join(";");

	if (editorValue.isEmpty()){
		editorValue = defaultValue;
	}

	return editor.setProperty("text", QVariant(editorValue));
}


bool CAttributeEditorComp::AttributeItemDelegate::SetAttributeExportEditor(const QByteArray& /*id*/, const QByteArray& exportId, QWidget& editor) const
{
	return editor.setProperty("text", QVariant(exportId));
}


bool CAttributeEditorComp::AttributeItemDelegate::SetAttributeValueEditor(
			const QByteArray& id,
			int propertyMining,
			QWidget& editor) const
{
	if (!m_parent.m_metaInfoManagerCompPtr.IsValid()){
		return false;
	}

	const IElementSelectionInfo* selectionInfoPtr = m_parent.GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return false;
	}

	IElementSelectionInfo::Elements selectedElements = selectionInfoPtr->GetSelectedElements();
	if (selectedElements.isEmpty()){
		return false;
	}

	for (		IElementSelectionInfo::Elements::ConstIterator elemIter = selectedElements.constBegin();
				elemIter != selectedElements.constEnd();
				++elemIter){
		const icomp::IRegistry::ElementInfo* elementInfoPtr = elemIter.value();
		Q_ASSERT(elementInfoPtr != NULL);
		if (!elementInfoPtr->elementPtr.IsValid()){
			continue;
		}

		const iser::IObject* attributePtr = m_parent.GetAttributeObject(id, *elementInfoPtr);
		if (attributePtr == NULL){
			continue;
		}

		if (propertyMining == AM_MULTI_REFERENCE){
			CMultiAttributeDelegateWidget* multiEditorPtr = dynamic_cast<CMultiAttributeDelegateWidget*>(&editor);
			if (multiEditorPtr == NULL){
				return false;
			}

			const icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(attributePtr);
			if (multiReferenceAttributePtr != NULL){
				QString valuesString = GetMultiAttributeValueAsString(*multiReferenceAttributePtr);

				multiEditorPtr->SetText(valuesString);

				return true;
			}
		}

		QComboBox* comboEditor = dynamic_cast<QComboBox*>(&editor);
		if (propertyMining == AM_REFERENCE){
			if (comboEditor == NULL){
				return false;
			}

			const icomp::IRegistry* registryPtr = m_parent.GetRegistry();
			const icomp::IAttributeStaticInfo* staticInfoPtr = m_parent.GetAttributeStaticInfo(id, *elementInfoPtr);
			if ((registryPtr != NULL) && (staticInfoPtr != NULL) && m_parent.m_consistInfoCompPtr.IsValid()){
				int queryFlags = IRegistryConsistInfo::QF_NONE;
				icomp::IElementStaticInfo::Ids obligatoryInterfaces = staticInfoPtr->GetRelatedMetaIds(
							icomp::IComponentStaticInfo::MGI_INTERFACES,
							0,
							icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the interfaces which must be set
				if (obligatoryInterfaces.isEmpty()){
					obligatoryInterfaces = staticInfoPtr->GetRelatedMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES, 0, 0);	// All asked interface names
					queryFlags = IRegistryConsistInfo::QF_ANY_INTERFACE;	// for optional interfaces only we are looking for any of them
				}
				icomp::IRegistry::Ids compatIds = m_parent.m_consistInfoCompPtr->GetCompatibleElements(
							obligatoryInterfaces,
							*registryPtr,
							queryFlags);

				QList< QByteArray> compatIdList = compatIds.toList();
				qSort(compatIdList);

				for(		QList< QByteArray>::ConstIterator iter = compatIdList.constBegin();
							iter != compatIdList.constEnd();
							++iter){
					comboEditor->addItem(*iter);
				}
			}

			const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<const icomp::CReferenceAttribute*>(attributePtr);
			if (referenceAttributePtr != NULL){
				comboEditor->lineEdit()->setText(referenceAttributePtr->GetValue());

				return true;
			}

			const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<const icomp::CFactoryAttribute*>(attributePtr);
			if (factoryAttributePtr != NULL){
				comboEditor->lineEdit()->setText(factoryAttributePtr->GetValue());

				return true;
			}
		}
		else if (propertyMining == AM_ATTRIBUTE){
			const icomp::CIntegerAttribute* intAttributePtr = dynamic_cast<const icomp::CIntegerAttribute*>(attributePtr);
			if (intAttributePtr != NULL){
				int value = intAttributePtr->GetValue();
				editor.setProperty("text", QVariant(value));

				return true;
			}

			const icomp::CRealAttribute* doubleAttributePtr = dynamic_cast<const icomp::CRealAttribute*>(attributePtr);
			if (doubleAttributePtr != NULL){
				double value = doubleAttributePtr->GetValue();
				editor.setProperty("text", QVariant(value));

				return true;
			}

			const icomp::CStringAttribute* stringAttributePtr = dynamic_cast<const icomp::CStringAttribute*>(attributePtr);
			if (stringAttributePtr != NULL){
				const QString& value = stringAttributePtr->GetValue();
				editor.setProperty("text", QVariant(value));

				return true;
			}

			const icomp::CIdAttribute* idAttributePtr = dynamic_cast<const icomp::CIdAttribute*>(attributePtr);
			if (idAttributePtr != NULL){
				const QByteArray& value = idAttributePtr->GetValue();
				editor.setProperty("text", QVariant(value));

				return true;
			}
		}
		else if (propertyMining == AM_BOOL_ATTRIBUTE){
			const icomp::CBooleanAttribute* boolAttributePtr = dynamic_cast<const icomp::CBooleanAttribute*>(attributePtr);
			if (boolAttributePtr != NULL){
				Q_ASSERT(comboEditor != NULL);

				bool value = boolAttributePtr->GetValue();
				comboEditor->setCurrentIndex(value? 0: 1);

				return true;
			}
		}
		else if (propertyMining == AM_MULTI_ATTRIBUTE){
			CMultiAttributeDelegateWidget* multiEditorPtr = dynamic_cast<CMultiAttributeDelegateWidget*>(&editor);
			if (multiEditorPtr == NULL){
				return false;
			}

			const icomp::CIntegerListAttribute* intListAttributePtr = dynamic_cast<const icomp::CIntegerListAttribute*>(attributePtr);
			if (intListAttributePtr != NULL){
				QString outputValue;
				for (int index = 0; index < intListAttributePtr->GetValuesCount(); index++){
					if (!outputValue.isEmpty()){
						outputValue += ";";
					}

					outputValue += QString::number(intListAttributePtr->GetValueAt(index));
				}

				multiEditorPtr->SetText(outputValue);

				return true;
			}

			const icomp::CRealListAttribute* doubleListAttributePtr = dynamic_cast<const icomp::CRealListAttribute*>(attributePtr);
			if (doubleListAttributePtr != NULL){
				QString outputValue;
				for (int index = 0; index < doubleListAttributePtr->GetValuesCount(); index++){
					if (!outputValue.isEmpty()){
						outputValue += ";";
					}

					outputValue += QString::number(doubleListAttributePtr->GetValueAt(index));
				}

				multiEditorPtr->SetText(outputValue);

				return true;
			}

			const icomp::CBooleanListAttribute* boolListAttributePtr = dynamic_cast<const icomp::CBooleanListAttribute*>(attributePtr);
			if (boolListAttributePtr != NULL){
				QString outputValue;
				for (int index = 0; index < boolListAttributePtr->GetValuesCount(); index++){
					if (!outputValue.isEmpty()){
						outputValue += ";";
					}

					outputValue += boolListAttributePtr->GetValueAt(index)? "true": "false";
				}

				multiEditorPtr->SetText(outputValue);

				return true;
			}

			const icomp::CStringListAttribute* stringListAttributePtr = dynamic_cast<const icomp::CStringListAttribute*>(attributePtr);
			if (stringListAttributePtr != NULL){
				QString outputValue;
				for (int index = 0; index < stringListAttributePtr->GetValuesCount(); index++){
					if (!outputValue.isEmpty()){
						outputValue += ";";
					}

					outputValue += stringListAttributePtr->GetValueAt(index);
				}

				multiEditorPtr->SetText(outputValue);

				return true;
			}

			const icomp::CIdListAttribute* idListAttributePtr = dynamic_cast<const icomp::CIdListAttribute*>(attributePtr);
			if (idListAttributePtr != NULL){
				QString outputValue;
				for (int index = 0; index < idListAttributePtr->GetValuesCount(); index++){
					if (!outputValue.isEmpty()){
						outputValue += ";";
					}

					outputValue += idListAttributePtr->GetValueAt(index);
				}

				multiEditorPtr->SetText(outputValue);

				return true;
			}
		}
	}

	return false;
}


bool CAttributeEditorComp::AttributeItemDelegate::SetComponentValue(const QByteArray& attributeId, int propertyMining, const QString& value) const
{
	bool retVal = false;

	istd::TChangeNotifier<icomp::IRegistry> registryPtr(m_parent.GetRegistry(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);
	if (registryPtr.IsValid()){
		AttrInfosMap::ConstIterator attributeInfoMapIter = m_parent.m_attrInfosMap.constFind(attributeId);
		if (attributeInfoMapIter == m_parent.m_attrInfosMap.constEnd()){
			return false;
		}

		const ElementIdToAttrInfoMap& elementsMap = attributeInfoMapIter.value();
		for (		ElementIdToAttrInfoMap::ConstIterator elemIter = elementsMap.constBegin();
					elemIter != elementsMap.constEnd();
					++elemIter){
			const AttrInfo& attributeInfo = elemIter.value();

			icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = attributeInfo.infoPtr.GetPtr();

			istd::TChangeNotifier<icomp::IRegistryElement> elementPtr(attributeInfo.elementPtr.GetPtr(), istd::IChangeable::CF_MODEL | icomp::IRegistryElement::CF_ATTRIBUTE_CHANGED);
			if (!elementPtr.IsValid()){
				continue;
			}

			if ((attributeInfoPtr == NULL) && !value.isEmpty()){
				Q_ASSERT(attributeInfo.staticInfoPtr.IsValid());	// attributeInfo.infoPtr or attributeInfo.staticInfoPtr must be valid for attribute!

				QByteArray attributeValueTypeId = attributeInfo.staticInfoPtr->GetAttributeTypeName();

				attributeInfoPtr = elementPtr->InsertAttributeInfo(attributeId, attributeValueTypeId);
			}

			if (attributeInfoPtr != NULL){
				if (propertyMining == AM_EXPORTED_ATTR){
					attributeInfoPtr->exportId = value.toLocal8Bit();

					retVal = true;
				}
				else{
					if (!attributeInfoPtr->attributePtr.IsValid()){
						attributeInfoPtr->attributePtr.SetPtr(elementPtr->CreateAttribute(attributeInfoPtr->attributeTypeName));
					}

					if (attributeInfoPtr->attributePtr.IsValid()){
						retVal = m_parent.EncodeAttribute(value, propertyMining, *attributeInfoPtr->attributePtr) || retVal;
					}
				}
			}

			if ((attributeInfoPtr != NULL) && !attributeInfoPtr->attributePtr.IsValid() && attributeInfoPtr->exportId.isEmpty()){
				elementPtr->RemoveAttribute(attributeId);
			}
		}

		Q_EMIT m_parent.AfterAttributesChange();
	}

	return retVal;
}


bool CAttributeEditorComp::AttributeItemDelegate::SetComponentExportData(const QByteArray& attributeId, const QString& value) const
{
	istd::TChangeNotifier<icomp::IRegistry> registryPtr(m_parent.GetRegistry(), istd::IChangeable::CF_MODEL | icomp::IRegistry::CF_ELEMENT_EXPORTED);
	if (registryPtr.IsValid()){
		icomp::IRegistry::ExportedElementsMap exportedMap = registryPtr->GetExportedElementsMap();
		for (icomp::IRegistry::ExportedElementsMap::ConstIterator iter = exportedMap.constBegin();
					iter != exportedMap.constEnd();
					++iter){
			if (iter.value() == attributeId){
				registryPtr->SetElementExported(iter.key(), "");
			}
		}

		if (!value.isEmpty()){
			registryPtr->SetElementExported(value.toLocal8Bit(), attributeId);
		}

		Q_EMIT m_parent.AfterSubcomponentsChange();
	}

	return true;
}


// public methods of embedded class RegistryObserver

CAttributeEditorComp::RegistryObserver::RegistryObserver(CAttributeEditorComp* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class RegistryObserver

// reimplemented (imod::CSingleModelObserverBase)

void CAttributeEditorComp::RegistryObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & istd::IChangeable::CF_MODEL) != 0){
		m_parent.UpdateEditor(updateFlags);
	}
}


} // namespace icmpstr


