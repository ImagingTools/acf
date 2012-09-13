#include "icmpstr/CRegistryTreeViewComp.h"


// Qt includes
#include <QtCore/QDir>
#include <QtGui/QHeaderView>

// ACF includes
#include "icomp/CCompositeComponentStaticInfo.h"

#include "ibase/CMessageContainer.h"

#include "iqtgui/CItemDelegate.h"


namespace icmpstr
{


CRegistryTreeViewComp::CRegistryTreeViewComp()
	:m_environmentObserver(this)
{
	m_selectionInfo.SetParent(this);
}


void CRegistryTreeViewComp::AddSubcomponents(
			const icomp::CComponentAddress& address,
			QTreeWidgetItem* registryElementItemPtr)
{
	if (m_envManagerCompPtr.IsValid()){
		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);

		if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
			const icomp::CCompositeComponentStaticInfo* compositeMetaInfoPtr = dynamic_cast<const icomp::CCompositeComponentStaticInfo*>(metaInfoPtr);
			if (compositeMetaInfoPtr != NULL){
				const icomp::IRegistry& registry = compositeMetaInfoPtr->GetRegistry();

				CreateRegistryTree(registry, registryElementItemPtr);
			}
		}
	}
}


void CRegistryTreeViewComp::CreateRegistryTree(const icomp::IRegistry& registry, QTreeWidgetItem* registryRootItemPtr)
{
	icomp::IRegistry::Ids elementIds = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::iterator iter = elementIds.begin();
				iter != elementIds.end();
				iter++){
		const QByteArray& elementId = *iter;
		const icomp::IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
		if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
			QTreeWidgetItem* elementItem = AddRegistryElementItem(registry, elementInfoPtr, elementId, registryRootItemPtr);
			
			// fill a subtree for an embedded composition
			icomp::IRegistry* embeddedRegistry = registry.GetEmbeddedRegistry(elementId);
			if (embeddedRegistry != NULL && elementItem != NULL){
				CreateRegistryTree(*embeddedRegistry, elementItem);
			}
		}
	}
}


// protected methods

QTreeWidgetItem* CRegistryTreeViewComp::AddRegistryElementItem(
			const icomp::IRegistry& registry,
			const icomp::IRegistry::ElementInfo* elementPtr,
			const QByteArray& elementId,
			QTreeWidgetItem* parentItemPtr)
{
	icomp::CRegistryElement* registryElementPtr = dynamic_cast<icomp::CRegistryElement*>(elementPtr->elementPtr.GetPtr());
	if (registryElementPtr != NULL){
		QTreeWidgetItem* elementItemPtr = new QTreeWidgetItem();

		elementItemPtr->setText(CT_NAME, elementId);
		elementItemPtr->setData(CT_NAME, DR_ELEMENT_NAME, elementId);
		elementItemPtr->setData(CT_NAME, DR_ELEMENT_ID, elementPtr->address.GetComponentId());
		elementItemPtr->setData(CT_NAME, DR_ELEMENT_PACKAGE_ID, elementPtr->address.GetPackageId());
		elementItemPtr->setData(CT_NAME, DR_REGISTRY, quintptr(&registry));
		elementItemPtr->setText(CT_ID, elementPtr->address.GetComponentId());
		elementItemPtr->setText(CT_PACKAGE, elementPtr->address.GetPackageId());

		static QIcon okIcon(":/Icons/Ok");
		elementItemPtr->setIcon(CT_NAME, okIcon);

		if (parentItemPtr != NULL){
			parentItemPtr->addChild(elementItemPtr);
			parentItemPtr->setExpanded(true);
		}
		
		bool isConsistent = true;
		ibase::CMessageContainer messageContainer;

		if (m_consistInfoCompPtr.IsValid()){
			isConsistent = m_consistInfoCompPtr->IsElementValid(
						elementId,
						registry,
						false,
						true,
						&messageContainer);
		}

		static QIcon errorIcon(":/Icons/Warning.svg");

		if (!isConsistent){
			elementItemPtr->setIcon(CT_NAME, errorIcon);

			ibase::CMessageContainer::Messages messages = messageContainer.GetMessages();
			QString messageText;
			for (int messageIndex = 0; messageIndex < messages.count(); messageIndex++){
				messageText += messages[messageIndex]->GetInformationDescription();
				messageText += '\n';
			}

			elementItemPtr->setData(CT_NAME, DR_MESSAGE_LIST, messageText);

			while (parentItemPtr != NULL){
				parentItemPtr->setIcon(CT_NAME, errorIcon);

				if (parentItemPtr->parent() == NULL){
					SetStatusIcon(errorIcon);
				}

				QString parentMessages = parentItemPtr->data(CT_NAME, DR_MESSAGE_LIST).toString();
				parentItemPtr->setData(CT_NAME, DR_MESSAGE_LIST, parentMessages + messageText);

				parentItemPtr = parentItemPtr->parent();
			}
		}

		AddSubcomponents(elementPtr->address, elementItemPtr);

		return elementItemPtr;
	}

	return NULL;
}


bool CRegistryTreeViewComp::IsRegistryValid(const icomp::IRegistry& registry) const
{
	bool retVal = true;

	icomp::IRegistry::Ids elementIds = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::iterator iter = elementIds.begin();
				iter != elementIds.end();
				iter++){
		const QByteArray& elementId = *iter;
		const icomp::IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
		if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
			icomp::CRegistryElement* registryElementPtr = dynamic_cast<icomp::CRegistryElement*>(elementInfoPtr->elementPtr.GetPtr());
			if (registryElementPtr != NULL){
				bool isConsistent = true;

				if (m_consistInfoCompPtr.IsValid()){
					isConsistent = m_consistInfoCompPtr->IsElementValid(
								elementId,
								registry,
								false,
								true,
								NULL);
				}

				if (!isConsistent){
					return false;
				}

				if (m_envManagerCompPtr.IsValid()){
					const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(elementInfoPtr->address);

					if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
						const icomp::CCompositeComponentStaticInfo* compositeMetaInfoPtr = dynamic_cast<const icomp::CCompositeComponentStaticInfo*>(metaInfoPtr);
						if (compositeMetaInfoPtr != NULL){
							const icomp::IRegistry& registry = compositeMetaInfoPtr->GetRegistry();

							retVal = retVal && IsRegistryValid(registry);

						}
					}
				}
			}
		}
	}

	return retVal;
}


void CRegistryTreeViewComp::UpdateRegistryStatus()
{
	const icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){		
		bool isValid = IsRegistryValid(*registryPtr);
			
		static QIcon errorIcon(":/Icons/Warning.svg");
		static QIcon okIcon(":/Icons/Ok");

		SetStatusIcon(isValid ? okIcon : errorIcon);
	}
}


void CRegistryTreeViewComp::UpdateTreeItemsVisibility()
{
	QString filterText = FilterEdit->text();
	bool showOnlyErrors = ShowOnlyErrorsCheck->isChecked();

	QTreeWidgetItemIterator treeIter(RegistryTree);

	while (*treeIter != NULL){
		QTreeWidgetItem* itemPtr = *treeIter;

		bool showItem = true;
			
		QString itemName = itemPtr->text(CT_NAME);

		if (filterText.isEmpty() || itemName.contains(filterText, Qt::CaseInsensitive)){
			showItem = true;
		}
		else{
			showItem = false;
		}

		QString messageText = itemPtr->data(CT_NAME, DR_MESSAGE_LIST).toString();
		if (messageText.isEmpty() && showOnlyErrors){
			showItem = false;
		}

		itemPtr->setHidden(!showItem);

		if (showItem){
			QTreeWidgetItem* parentItemPtr = itemPtr->parent();

			while (parentItemPtr != NULL){ 
				if (parentItemPtr->isHidden()){
					parentItemPtr->setHidden(false);
				}

				parentItemPtr = parentItemPtr->parent();
			}
		}

		treeIter++;
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CRegistryTreeViewComp::UpdateGui(int /*updateFlags*/)
{
	RegistryTree->clear();

	QTreeWidgetItem* rootItem = new QTreeWidgetItem();
	rootItem->setText(0, "Root");

	static QIcon okIcon(":/Icons/Ok");

	rootItem->setIcon(CT_NAME, okIcon);

	SetStatusIcon(okIcon);

	RegistryTree->addTopLevelItem(rootItem);

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		CreateRegistryTree(*registryPtr, rootItem);
	}

	rootItem->setSelected(true);
}


void CRegistryTreeViewComp::OnGuiModelDetached()
{
	RegistryTree->clear();

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CRegistryTreeViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QList<int> sizes = MainSplitter->sizes();
	sizes[0] = 0;

	MainSplitter->setSizes(sizes);

//	RegistryTree->setItemDelegate(new iqtgui::CItemDelegate);

	RegistryTree->header()->setResizeMode(QHeaderView::Stretch);
}


// reimplemented (imod::CSingleModelObserverBase)

void CRegistryTreeViewComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	UpdateRegistryStatus();
	
	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// reimplemented (icomp::CComponentBase)

void CRegistryTreeViewComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_envManagerModelCompPtr.IsValid()){
		m_envManagerModelCompPtr->AttachObserver(&m_environmentObserver);
	}
}


void CRegistryTreeViewComp::OnComponentDestroyed()
{
	if (m_envManagerModelCompPtr.IsValid() && m_envManagerModelCompPtr->IsAttached(&m_environmentObserver)){
		m_envManagerModelCompPtr->DetachObserver(&m_environmentObserver);
	}

	BaseClass::OnComponentDestroyed();
}


// static methods

IElementSelectionInfo* CRegistryTreeViewComp::ExtractSelectionInterface(CRegistryTreeViewComp& component)
{
	return &component.m_selectionInfo;
}


imod::IModel* CRegistryTreeViewComp::ExtractSelectionInterfaceModel(CRegistryTreeViewComp& component)
{
	return &component.m_selectionInfo;
}


istd::IChangeable* CRegistryTreeViewComp::ExtractSelectionInterfaceChangeable(CRegistryTreeViewComp& component)
{
	return &component.m_selectionInfo;
}


// protected slots

void CRegistryTreeViewComp::on_RegistryTree_itemSelectionChanged()
{
	istd::CChangeNotifier changePtr(&m_selectionInfo);

	m_selectedElements.selectedElementIds.clear();
	m_selectedElements.registryPtr = NULL;

	QList<QTreeWidgetItem*> selectedItems = RegistryTree->selectedItems();

	int itemsCount = selectedItems.size();
	
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		QTreeWidgetItem* itemPtr = selectedItems.at(itemIndex);
		I_ASSERT(itemPtr != NULL);

		QString messageText = itemPtr->data(CT_NAME, DR_MESSAGE_LIST).toString();

		m_selectedElements.selectedElementIds.insert(itemPtr->data(CT_NAME, DR_ELEMENT_ID).toByteArray());

		MessagesList->setPlainText(messageText);

		m_selectedElements.selectedElementIds.insert(itemPtr->data(CT_NAME, DR_ELEMENT_ID).toByteArray());
        int registryPointerAddress = itemPtr->data(CT_NAME, DR_REGISTRY).toUInt();
		m_selectedElements.registryPtr = reinterpret_cast<icomp::IRegistry*>(registryPointerAddress);
	}

	// Close message view of no messages were provded:
	QList<int> sizes = MainSplitter->sizes();

	if (MessagesList->toPlainText().isEmpty()){
		sizes[1] = 0;
	}
	else{
		sizes[1] = sizes[0] / 4;
	}

	MainSplitter->setSizes(sizes);
}


void CRegistryTreeViewComp::on_RegistryTree_itemDoubleClicked(QTreeWidgetItem* itemPtr, int /*column*/)
{
	icomp::CComponentAddress componentAddress;

	componentAddress.SetComponentId(itemPtr->data(CT_NAME, DR_ELEMENT_ID).toByteArray());
	componentAddress.SetPackageId(itemPtr->data(CT_NAME, DR_ELEMENT_PACKAGE_ID).toByteArray());
	
	if (m_envManagerCompPtr.IsValid() && m_documentManagerCompPtr.IsValid()){
		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(componentAddress);

		if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
			QDir packageDir(m_envManagerCompPtr->GetPackagePath(componentAddress.GetPackageId()));
		
			QString filePath = packageDir.absoluteFilePath(componentAddress.GetComponentId() + ".arx");

			m_documentManagerCompPtr->FileOpen(NULL, &filePath);
		}
	}
}


void CRegistryTreeViewComp::on_FilterEdit_textChanged(const QString& /*filterText*/)
{
	UpdateTreeItemsVisibility();
}


void CRegistryTreeViewComp::on_ShowOnlyErrorsCheck_stateChanged(int /*state*/)
{
	UpdateTreeItemsVisibility();
}


// public methods of embedded class EnvironmentObserver

CRegistryTreeViewComp::EnvironmentObserver::EnvironmentObserver(CRegistryTreeViewComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class EnvironmentObserver

// reimplemented (imod::TSingleModelObserverBase)

void CRegistryTreeViewComp::EnvironmentObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.UpdateRegistryStatus();

	if (m_parent.IsGuiCreated()){
		m_parent.UpdateGui(updateFlags);
	}
}


// public methods of embedded class SelectionInfoImpl

void CRegistryTreeViewComp::SelectionInfoImpl::SetParent(CRegistryTreeViewComp* parentPtr)
{
	m_parentPtr = parentPtr;
}


// reimplemented (icmpstr::IElementSelectionInfo)

icomp::IRegistry* CRegistryTreeViewComp::SelectionInfoImpl::GetSelectedRegistry() const
{
	I_ASSERT(m_parentPtr != NULL);	// parent should be set before any subelement can be accessed

	return m_parentPtr->m_selectedElements.registryPtr;
}


IElementSelectionInfo::Elements CRegistryTreeViewComp::SelectionInfoImpl::GetSelectedElements() const
{
	I_ASSERT(m_parentPtr != NULL);	// parent should be set before any subelement can be accessed

	IElementSelectionInfo::Elements retVal;

	icomp::IRegistry* registryPtr = m_parentPtr->GetObjectPtr();
	if (registryPtr != NULL){
		for (		ElementIds::const_iterator iter = m_parentPtr->m_selectedElements.selectedElementIds.begin();
					iter != m_parentPtr->m_selectedElements.selectedElementIds.end();
					++iter){
			const QByteArray& elementName = *iter;

			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementName);
			if (elementInfoPtr != NULL){
				retVal[elementName] = elementInfoPtr;
			}
		}
	}

	return retVal;
}


} // namespace icmpstr


