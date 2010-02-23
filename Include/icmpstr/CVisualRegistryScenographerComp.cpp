#include "icmpstr/CVisualRegistryScenographerComp.h"


// Qt includes
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>


// ACF inlcudes
#include "istd/TChangeNotifier.h"

#include "iser/CMemoryReadArchive.h"

#include "ibase/IApplication.h"

#include "iqtgui/TDesignerBasicGui.h"

#include "icmpstr/CRegistryElementShape.h"
#include "icmpstr/CGraphicsConnectorItem.h"
#include "icmpstr/CVisualRegistryElement.h"
#include "icmpstr/CVisualRegistryComp.h"


namespace icmpstr
{


CVisualRegistryScenographerComp::CVisualRegistryScenographerComp()
:	m_scenePtr(NULL),
	m_isUpdating(false)
{
	int lightToolFlags = ibase::IHierarchicalCommand::CF_GLOBAL_MENU | ibase::IHierarchicalCommand::CF_TOOLBAR;

	m_removeComponentCommand.setEnabled(false);
	m_removeComponentCommand.SetGroupId(GI_COMPONENT);
	m_removeComponentCommand.SetStaticFlags(lightToolFlags);
	m_removeComponentCommand.setShortcut(QKeySequence(Qt::Key_Delete));
	m_renameComponentCommand.setEnabled(false);
	m_renameComponentCommand.SetGroupId(GI_COMPONENT);
	m_renameComponentCommand.setShortcut(QKeySequence(Qt::Key_F2));
	m_exportToCodeCommand.SetGroupId(GI_CODEGEN);
	m_executeRegistryCommand.setEnabled(false);
	m_executeRegistryCommand.setShortcut(QKeySequence(Qt::Key_F5));
	m_executeRegistryCommand.SetGroupId(GI_PREVIEW);
	m_executeRegistryCommand.SetStaticFlags(lightToolFlags);
	m_abortRegistryCommand.setEnabled(false);
	m_abortRegistryCommand.SetGroupId(GI_PREVIEW);
	m_abortRegistryCommand.SetStaticFlags(lightToolFlags);
	m_abortRegistryCommand.setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	m_removeNoteCommand.setEnabled(false);
	m_removeNoteCommand.SetGroupId(GI_COMPONENT);
	m_removeNoteCommand.SetStaticFlags(ibase::IHierarchicalCommand::CF_GLOBAL_MENU);
	m_addNoteCommand.setEnabled(false);
	m_addNoteCommand.SetGroupId(GI_COMPONENT);
	m_addNoteCommand.SetStaticFlags(ibase::IHierarchicalCommand::CF_GLOBAL_MENU);

	m_registryMenu.InsertChild(&m_removeComponentCommand);
	m_registryMenu.InsertChild(&m_renameComponentCommand);
	m_registryMenu.InsertChild(&m_exportToCodeCommand);
	m_registryMenu.InsertChild(&m_executeRegistryCommand);
	m_registryMenu.InsertChild(&m_abortRegistryCommand);
	m_registryMenu.InsertChild(&m_addNoteCommand);
	m_registryMenu.InsertChild(&m_removeNoteCommand);
	m_registryCommand.InsertChild(&m_registryMenu);
}


const QFont& CVisualRegistryScenographerComp::GetElementNameFont() const
{
	return m_elementNameFont;
}


const QFont& CVisualRegistryScenographerComp::GetElementDetailFont() const
{
	return m_elementDetailFont;
}


bool CVisualRegistryScenographerComp::TryOpenComponent(const CVisualRegistryElement& registryElement) const
{
	const icomp::IComponentEnvironmentManager* managerPtr = GetEnvironmentManager();
	if ((managerPtr != NULL) && m_mainWindowCompPtr.IsValid()){
		
		const icomp::IComponentStaticInfo* metaInfoPtr = managerPtr->GetComponentMetaInfo(registryElement.GetAddress());

		if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
			QDir packageDir(iqt::GetQString(managerPtr->GetPackageDirPath(registryElement.GetAddress().GetPackageId())));
			QString filePath = packageDir.absoluteFilePath((registryElement.GetAddress().GetComponentId() + ".arx").c_str());

			m_mainWindowCompPtr->OpenFile(iqt::GetCString(filePath));

			return true;
		}
	}

	return false;
}


// reimplemented (iqtgui::IDropConsumer)

QStringList CVisualRegistryScenographerComp::GetAcceptedMimeIds() const
{
	return QStringList() << "component";
}


void CVisualRegistryScenographerComp::OnDropFinished(const QMimeData& mimeData, QEvent* eventPtr)
{
	QGraphicsSceneDragDropEvent* sceneEventPtr = dynamic_cast<QGraphicsSceneDragDropEvent*>(eventPtr);
	if (sceneEventPtr != NULL){
		OnDroppedData(mimeData, sceneEventPtr);
	}
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CVisualRegistryScenographerComp::GetCommands() const
{
	return &m_registryCommand;
}


// reimplemented (imod::CSingleModelObserverBase)

void CVisualRegistryScenographerComp::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (updateFlags != 0){
		static const int unimportantChanges =
					i2d::IObject2d::CF_OBJECT_POSITION |
					istd::IChangeable::CF_MODEL |
					istd::IChangeable::CF_ACF_INTERNAL |
					istd::CChangeDelegator::CF_DELEGATED |
					icomp::IRegistryElement::CF_FLAGS_CHANGED;
		int maskedFlags = (updateFlags & ~unimportantChanges);
		if (maskedFlags == 0){
			// some unimportant model changes
			return;
		}
	}

	if (m_scenePtr == NULL){
		return;
	}

	m_isUpdating = true;

	if (updateFlags != CVisualRegistryComp::CF_SELECTION){
		QList<QGraphicsItem*> items = m_scenePtr->items();
		foreach(QGraphicsItem* itemPtr, items){
			m_scenePtr->removeItem(itemPtr);
		}

		icomp::IRegistry* registryPtr = GetObjectPtr();
		if (registryPtr != NULL){
			icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
			for (		icomp::IRegistry::Ids::iterator iter = elementIds.begin();
						iter != elementIds.end();
						iter++){
				const std::string& elementId = *iter;
				const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementId);
				if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
					AddShapeToScene(elementInfoPtr->elementPtr.GetPtr());
				}
			}
		}

		AddConnectorsToScene();
	}

	QList<QGraphicsItem*> items = m_scenePtr->items();
	foreach(QGraphicsItem* itemPtr, items){
		CRegistryElementShape* elementShapePtr = dynamic_cast<CRegistryElementShape*>(itemPtr);
		if (elementShapePtr != NULL){
			elementShapePtr->CheckConsistency();
		}
	}

	UpdateComponentSelection();

	m_isUpdating = false;
}


// reimplemented (icomp::IComponent)

void CVisualRegistryScenographerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	connect(&m_removeComponentCommand, SIGNAL(activated()), this, SLOT(OnRemoveComponent()));
	connect(&m_renameComponentCommand, SIGNAL(activated()), this, SLOT(OnRenameComponent()));
	connect(&m_exportToCodeCommand, SIGNAL(activated()), this, SLOT(OnExportToCode()));
	connect(&m_executeRegistryCommand, SIGNAL(activated()), this, SLOT(OnExecute()));
	connect(&m_abortRegistryCommand, SIGNAL(activated()), this, SLOT(OnAbort()));
	connect(&m_addNoteCommand, SIGNAL(activated()), this, SLOT(OnAddNote()));
	connect(&m_removeNoteCommand, SIGNAL(activated()), this, SLOT(OnRemoveNote()));

	m_exportToCodeCommand.setVisible(m_registryCodeSaverCompPtr.IsValid());
	m_executeRegistryCommand.setVisible(m_registryPreviewCompPtr.IsValid());
	m_abortRegistryCommand.setVisible(m_registryPreviewCompPtr.IsValid());

	if (m_registryPreviewCompPtr.IsValid()){
		connect(&m_executionObserverTimer, SIGNAL(timeout()), this, SLOT(OnExecutionTimerTick()));

		m_executionObserverTimer.start(500);
	}

	m_elementNameFont = qApp->font();
	m_elementNameFont.setBold(true);
	m_elementNameFont.setPointSize(12);

	m_elementDetailFont = qApp->font();
	m_elementDetailFont.setPointSize(8);

	if (m_sceneProviderCompPtr.IsValid()){
		I_ASSERT(m_scenePtr == NULL);
		m_scenePtr = m_sceneProviderCompPtr->GetScene();

		if (m_scenePtr != NULL){
			connect(m_scenePtr, SIGNAL(selectionChanged()), this, SLOT(OnSelectionChanged()));
		}
	}

	DoRetranslate();
}


void CVisualRegistryScenographerComp::OnComponentDestroyed()
{
	m_scenePtr = NULL;
}


// protected methods

void CVisualRegistryScenographerComp::DoRetranslate()
{
	m_registryMenu.SetVisuals(
				tr("&Registry"),
				tr("Registry"),
				tr("Set of commands manipulating registry"));
	m_removeComponentCommand.SetVisuals(
				tr("&Remove Component"), 
				tr("Remove"), 
				tr("Remove the selected component from the registry"),
				QIcon(":/Icons/Delete.svg"));
	m_renameComponentCommand.SetVisuals(
				tr("&Rename Component"), 
				tr("Rename"), 
				tr("Allow to assign new name to selected component"));
	m_exportToCodeCommand.SetVisuals(
				tr("&Export To Code..."),
				tr("Export"),
				tr("Generates C++ code according to this registry"),
				QIcon(":/Icons/ExportToCode.svg"));
	m_executeRegistryCommand.SetVisuals(
				tr("&Execute Registry"), 
				tr("&Execute Registry"), 
				tr("Start registry execution"),
				QIcon(":/Icons/Play.svg"));
	m_abortRegistryCommand.SetVisuals(
				tr("&Abort Registry"), 
				tr("&Abort Registry"), 
				tr("Abort registry execution"),
				QIcon(":/Icons/Stop.svg"));
	m_addNoteCommand.SetVisuals(
				tr("&Add Note"), 
				tr("&Add Note"), 
				tr("Add a note to selected component"),
				QIcon(":/Resources/Icons/.png"));
	m_removeNoteCommand.SetVisuals(
				tr("&Remove Note"), 
				tr("&Remove Note"), 
				tr("Remove the note from selected component"),
				QIcon(":/Resources/Icons/.png"));
}


// protected slots

void CVisualRegistryScenographerComp::OnSelectionChanged()
{
	if (m_scenePtr == NULL){
		return;
	}

	if (m_isUpdating){
		return;
	}

	QList<QGraphicsItem*> selectedItems = m_scenePtr->selectedItems();
	CVisualRegistryElement* elementPtr = NULL;
	
	for (		QList<QGraphicsItem*>::const_iterator iter = selectedItems.begin();
				iter != selectedItems.end();
				++iter){
		QGraphicsItem* selectedItemPtr = *iter;
		CRegistryElementShape* selectedShapePtr = dynamic_cast<CRegistryElementShape*>(selectedItemPtr);
		if (selectedShapePtr == NULL){
			continue;
		}

		elementPtr = selectedShapePtr->GetObjectPtr();
		if (elementPtr != NULL){
			break;
		}	
	}

	CVisualRegistryComp* visualRegistryPtr = dynamic_cast<CVisualRegistryComp*>(GetObjectPtr());
	if (visualRegistryPtr != NULL){
		visualRegistryPtr->SetSelectedElement(elementPtr);
	}
}


void CVisualRegistryScenographerComp::OnRemoveComponent()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	icmpstr::IElementSelectionInfo* elementSelectionInfoPtr = dynamic_cast<icmpstr::IElementSelectionInfo*>(registryPtr);
	if (elementSelectionInfoPtr == NULL){
		return;
	}

	if (registryPtr != NULL){
		registryPtr->RemoveElementInfo(elementSelectionInfoPtr->GetSelectedElementName());
	}
}


void CVisualRegistryScenographerComp::OnRenameComponent()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}
	icmpstr::IElementSelectionInfo* elementSelectionInfoPtr = dynamic_cast<icmpstr::IElementSelectionInfo*>(registryPtr);
	if (elementSelectionInfoPtr == NULL){
		return;
	}

	CVisualRegistryElement* selectedComponentPtr = dynamic_cast<CVisualRegistryElement*>(elementSelectionInfoPtr->GetSelectedElement());
	if (selectedComponentPtr == NULL){
		return;
	}
	const std::string& oldName = selectedComponentPtr->GetName();

	bool isOk = false;
	std::string newName = QInputDialog::getText(
				NULL,
				tr("ACF Compositor"),
				tr("New component name"),
				QLineEdit::Normal,
				iqt::GetQString(oldName),
				&isOk).toStdString();
	if (!isOk || newName.empty() || (oldName == newName)){
		return;
	}

	i2d::CVector2d position = selectedComponentPtr->GetCenter();

	istd::CChangeNotifier registryNotifier(registryPtr, istd::IChangeable::CF_MODEL | icomp::IRegistry::CF_COMPONENT_ADDED);

	const icomp::IRegistry::ElementInfo* newInfoPtr = registryPtr->InsertElementInfo(newName, selectedComponentPtr->GetAddress(), true);
	if (newInfoPtr == NULL){
		return;
	}

	I_ASSERT(newInfoPtr->elementPtr.IsValid());	// InsertElementInfo has to return NULL if element cannot be created with option ensureElementCreated = true

	if (iser::CMemoryReadArchive::CloneObjectByArchive(*selectedComponentPtr, *newInfoPtr->elementPtr)){
		icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
		for (		icomp::IRegistry::Ids::iterator compIdIter = elementIds.begin();
					compIdIter != elementIds.end();
					++compIdIter){
			const std::string& componentId = *compIdIter;
			const icomp::IRegistry::ElementInfo* infoPtr = registryPtr->GetElementInfo(componentId);
			if (infoPtr == NULL){
				continue;
			}
			const icomp::IRegistryElement* elementPtr = infoPtr->elementPtr.GetPtr();
			if (elementPtr == NULL){
				continue;
			}

			icomp::IRegistryElement::Ids attrIds = elementPtr->GetAttributeIds();

			for (		icomp::IRegistryElement::Ids::iterator attrIdIter = attrIds.begin();
						attrIdIter != attrIds.end();
						++attrIdIter){
				const std::string& attributeId = *attrIdIter;
				const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = elementPtr->GetAttributeInfo(attributeId);
				if (attrInfoPtr == NULL){
					continue;
				}

				iser::ISerializable* attributePtr = attrInfoPtr->attributePtr.GetPtr();
				icomp::TAttribute<std::string>* singleAttrPtr = dynamic_cast<icomp::TAttribute<std::string>*>(attributePtr);
				icomp::TMultiAttribute<std::string>* multiAttrPtr = dynamic_cast<icomp::TMultiAttribute<std::string>*>(attributePtr);

				if (		(dynamic_cast<icomp::CReferenceAttribute*>(attributePtr) != NULL) ||
							(dynamic_cast<icomp::CFactoryAttribute*>(attributePtr) != NULL)){
					std::string baseId;
					std::string subId;
					icomp::CInterfaceManipBase::SplitId(singleAttrPtr->GetValue(), baseId, subId);
					if (baseId == oldName){
						singleAttrPtr->SetValue(icomp::CInterfaceManipBase::JoinId(newName, subId));
					}
				}

				if (		(dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr) != NULL) ||
							(dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr) != NULL)){
					int valuesCount = multiAttrPtr->GetValuesCount();
					for (int i = 0; i < valuesCount; ++i){
						std::string baseId;
						std::string subId;
						icomp::CInterfaceManipBase::SplitId(multiAttrPtr->GetValueAt(i), baseId, subId);
						if (baseId == oldName){
							multiAttrPtr->SetValueAt(i, icomp::CInterfaceManipBase::JoinId(newName, subId));
						}
					}
				}
			}
		}

		registryPtr->RemoveElementInfo(selectedComponentPtr->GetName());

		CVisualRegistryElement* newComponentPtr = dynamic_cast<CVisualRegistryElement*>(newInfoPtr->elementPtr.GetPtr());
		if (newComponentPtr != NULL){
			newComponentPtr->MoveTo(position);
		}
	}
	else{
		registryPtr->RemoveElementInfo(newName);
	}
}


void CVisualRegistryScenographerComp::OnExportToCode()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryCodeSaverCompPtr.IsValid()){
		QString filter = tr("C++ code file (*.cpp)");
		QString file = QFileDialog::getSaveFileName(NULL, tr("Export registry to code"), "", filter);
		if (!file.isEmpty()){
			if (m_registryCodeSaverCompPtr->SaveToFile(*registryPtr, iqt::GetCString(file)) == iser::IFileLoader::StateFailed){
				QMessageBox::warning(NULL, tr("Error"), tr("Cannot export to file\n%1").arg(file));
			}
		}
	}
}


void CVisualRegistryScenographerComp::OnExecute()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryPreviewCompPtr.IsValid()){
		bool retVal = m_registryPreviewCompPtr->StartRegistry(*registryPtr);
		if (retVal){
			OnExecutionTimerTick();

			m_executionObserverTimer.start(500);
		}
	}
}


void CVisualRegistryScenographerComp::OnAbort()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryPreviewCompPtr.IsValid()){
		m_registryPreviewCompPtr->AbortRegistry();

		OnExecutionTimerTick();

		m_executionObserverTimer.stop();
	}
}


void CVisualRegistryScenographerComp::OnAddNote()
{
	// TODO: implement registry notes
}


void CVisualRegistryScenographerComp::OnRemoveNote()
{
	// TODO: implement registry notes
}


bool CVisualRegistryScenographerComp::OnDroppedData(const QMimeData& mimeData, QGraphicsSceneDragDropEvent* eventPtr)
{
	QByteArray byteData = mimeData.data("component");
	iser::CMemoryReadArchive archive(byteData.constData(), byteData.size());

	icomp::CComponentAddress address;

	i2d::CVector2d position(0, 0);
	if (eventPtr != NULL){
		position = iqt::GetCVector2d(eventPtr->scenePos());
	}

	return address.Serialize(archive) && TryCreateComponent(address, position);
}


void CVisualRegistryScenographerComp::OnExecutionTimerTick()
{
	bool isExecutable = false;
	bool isRunning = false;

	if (m_registryPreviewCompPtr.IsValid()){
		isRunning = m_registryPreviewCompPtr->IsRunning();

		icomp::IRegistry* registryPtr = GetObjectPtr();
		if (registryPtr != NULL){
			const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registryPtr->GetExportedInterfacesMap();
			isExecutable = (interfacesMap.find(istd::CClassInfo::GetInfo<ibase::IApplication>()) != interfacesMap.end());
		}
	}

	m_executeRegistryCommand.setEnabled(!isRunning && isExecutable);
	m_abortRegistryCommand.setEnabled(isRunning);
}


// protected methods

QGraphicsItem* CVisualRegistryScenographerComp::AddShapeToScene(iser::ISerializable* elementPtr) const
{
	if (m_scenePtr == NULL){
		return NULL;
	}

	imod::IModel* elementModelPtr = dynamic_cast<imod::IModel*>(elementPtr);
	if (elementModelPtr != NULL){
		CVisualRegistryElement* registryElementPtr = dynamic_cast<CVisualRegistryElement*>(elementPtr);
		if (registryElementPtr != NULL){
			CRegistryElementShape* shapePtr = new CRegistryElementShape(this, m_sceneProviderCompPtr.GetPtr());

			icmpstr::IElementSelectionInfo* elementSelectionInfoPtr = dynamic_cast<icmpstr::IElementSelectionInfo*>(GetObjectPtr());
			if (elementSelectionInfoPtr != NULL && elementSelectionInfoPtr->GetSelectedElement() == elementPtr){
				shapePtr->setSelected(true);
			}

			elementModelPtr->AttachObserver(shapePtr);

			m_scenePtr->addItem(shapePtr);

			return shapePtr;
		}
	}

	return NULL;
}


void CVisualRegistryScenographerComp::AddConnectorsToScene()
{
	if (m_scenePtr == NULL){
		return;
	}

	QList<QGraphicsItem*> items = m_scenePtr->items();
	foreach (QGraphicsItem* itemPtr, items){
		CRegistryElementShape* sourceShapePtr = dynamic_cast<CRegistryElementShape*>(itemPtr);
		if (sourceShapePtr == NULL){
			continue;
		}
		CVisualRegistryElement* elementPtr = sourceShapePtr->GetObjectPtr();
		if (elementPtr == NULL){
			continue;
		}

		icomp::IRegistryElement::Ids attributeIds = elementPtr->GetAttributeIds();

		for (		icomp::IRegistryElement::Ids::const_iterator iter = attributeIds.begin(); 
					iter != attributeIds.end(); 
					iter++){
			std::string attributeId = *iter;

			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementPtr->GetAttributeInfo(attributeId);
			if (attributeInfoPtr != NULL){
				iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
				const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<icomp::CReferenceAttribute*>(attributePtr);
				if (referenceAttributePtr != NULL){		
					const std::string& componentId = referenceAttributePtr->GetValue();
				
					AddConnector(*sourceShapePtr, componentId, attributeId);
				}
	
				const icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr);
				if (multiReferenceAttributePtr != NULL){
					for (int referenceIndex = 0; referenceIndex < multiReferenceAttributePtr->GetValuesCount(); referenceIndex++){
						const std::string& componentId = multiReferenceAttributePtr->GetValueAt(referenceIndex);
						
						AddConnector(*sourceShapePtr, componentId, attributeId);
					}
				}

				const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<icomp::CFactoryAttribute*>(attributePtr);
				if (factoryAttributePtr != NULL){		
					const std::string& componentId = factoryAttributePtr->GetValue();

					AddConnector(*sourceShapePtr, componentId, attributeId, true);
				}
	
				const icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr);
				if (multiFactoryAttributePtr != NULL){
					for (int referenceIndex = 0; referenceIndex < multiFactoryAttributePtr->GetValuesCount(); referenceIndex++){
						const std::string& componentId = multiFactoryAttributePtr->GetValueAt(referenceIndex);

						AddConnector(*sourceShapePtr, componentId, attributeId, true);
					}
				}
			}
		}
	}
}


void CVisualRegistryScenographerComp::AddConnector(
			CRegistryElementShape& sourceShape,
			const std::string& referenceComponentId,
			const std::string& attributeId,
			bool isFactory)
{
	if (m_scenePtr == NULL){
		return;
	}

	I_ASSERT(m_sceneProviderCompPtr.IsValid());

	std::string baseId;
	std::string subId;
	bool isEmbedded = icomp::CInterfaceManipBase::SplitId(referenceComponentId, baseId, subId);

	QList<QGraphicsItem*> items = m_scenePtr->items();
	foreach(QGraphicsItem* itemPtr, items){
		CRegistryElementShape* destShapePtr = dynamic_cast<CRegistryElementShape*>(itemPtr);
		if (destShapePtr == NULL){
			continue;
		}
		CVisualRegistryElement* destElementPtr = destShapePtr->GetObjectPtr();
		if ((destElementPtr == NULL) || (destElementPtr->GetName() != baseId)){
			continue;
		}

		int connectFlags = 0;
		if (isEmbedded){
			connectFlags = connectFlags | CGraphicsConnectorItem::CF_EMBEDDED;
		}
		if (isFactory){
			connectFlags = connectFlags | CGraphicsConnectorItem::CF_FACTORY;
		}

		CGraphicsConnectorItem* connectorPtr = new CGraphicsConnectorItem(*m_sceneProviderCompPtr.GetPtr(), connectFlags);

		connectorPtr->setToolTip(isFactory?
					tr("Factory of '%1'").arg(attributeId.c_str()):
					tr("Reference of '%1'").arg(attributeId.c_str()));

		connectorPtr->InitEnds(&sourceShape, destShapePtr);

		m_scenePtr->addItem(connectorPtr);

		return;
	}
}


bool CVisualRegistryScenographerComp::TryCreateComponent(const icomp::CComponentAddress& address, const i2d::CVector2d& position)
{
	bool retVal = false;
	
	QString componentName = QInputDialog::getText(
				NULL, 
				tr("Application Compositor"), 
				tr("Component name"), 
				QLineEdit::Normal, 
				iqt::GetQString(address.GetComponentId()),
				&retVal);
	
	if (retVal && !componentName.isEmpty()){
		istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_COMPONENT_ADDED);
		if (registryPtr.IsValid()){
			icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->InsertElementInfo(componentName.toStdString(), address);
			if (elementInfoPtr != NULL){
				CVisualRegistryElement* visualElementPtr = dynamic_cast<CVisualRegistryElement*>(elementInfoPtr->elementPtr.GetPtr());
				if (visualElementPtr != NULL){
					visualElementPtr->MoveTo(position);
				}

				ConnectReferences(componentName);

				return true;
			}
		}

		QMessageBox::critical(NULL, tr("Error"), tr("Component could not be added")); 
	}

	return false;
}


void CVisualRegistryScenographerComp::ConnectReferences(const QString& componentRole)
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}

	icomp::IRegistry::Ids componentIds = registryPtr->GetElementIds();

	for (		icomp::IRegistry::Ids::const_iterator iter = componentIds.begin();
				iter != componentIds.end();
				iter++){

		const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(*iter);
		I_ASSERT(elementInfoPtr != NULL);
		I_ASSERT(elementInfoPtr->elementPtr.IsValid());

		const icomp::IComponentStaticInfo* compMetaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(elementInfoPtr->address);
		if (compMetaInfoPtr == NULL){
			continue;
		}

		const icomp::IComponentStaticInfo::AttributeInfos& staticAttributes = compMetaInfoPtr->GetAttributeInfos();
	
		for (int staticAttributeIndex = 0; staticAttributeIndex < staticAttributes.GetElementsCount(); staticAttributeIndex++){
			const std::string& attributeId = staticAttributes.GetKeyAt(staticAttributeIndex);
			const icomp::IAttributeStaticInfo* staticAttributeInfoPtr = staticAttributes.GetValueAt(staticAttributeIndex);
			I_ASSERT(staticAttributeInfoPtr != NULL);

			const iser::IObject* attributePtr = staticAttributeInfoPtr->GetAttributeDefaultValue();
			const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<const icomp::CReferenceAttribute*>(attributePtr);
			const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<const icomp::CFactoryAttribute*>(attributePtr);
			const icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(attributePtr);
			const icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<const icomp::CMultiFactoryAttribute*>(attributePtr);

			bool createAttribute = false;

			if (referenceAttributePtr != NULL && referenceAttributePtr->GetValue() == componentRole.toStdString()){
				createAttribute = true;
			}
			else if (factoryAttributePtr != NULL && factoryAttributePtr->GetValue() == componentRole.toStdString()){
				createAttribute = true;
			}
			else if (multiReferenceAttributePtr != NULL){
				for (int valueIndex = 0; valueIndex < multiReferenceAttributePtr->GetValuesCount(); valueIndex++){
					if (multiReferenceAttributePtr->GetValueAt(valueIndex) == componentRole.toStdString()){
						createAttribute = true;
						break;
					}
				}
			}
			else if (multiFactoryAttributePtr != NULL){
				for (int valueIndex = 0; valueIndex < multiFactoryAttributePtr->GetValuesCount(); valueIndex++){
					if (multiFactoryAttributePtr->GetValueAt(valueIndex) == componentRole.toStdString()){
						createAttribute = true;
						break;
					}
				}
			}

			icomp::IRegistryElement* registryElementPtr = elementInfoPtr->elementPtr.GetPtr();
			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = registryElementPtr->GetAttributeInfo(attributeId);
			if (attributeInfoPtr == NULL && createAttribute){
				const std::string& attrType = staticAttributeInfoPtr->GetAttributeTypeName();
				icomp::IRegistryElement::AttributeInfo* newAttributeInfoPtr = registryElementPtr->InsertAttributeInfo(attributeId, attrType);
				if (newAttributeInfoPtr != NULL){
					newAttributeInfoPtr->attributePtr.SetPtr(registryElementPtr->CreateAttribute(attrType));
				}
			}
		}
	}
}


void CVisualRegistryScenographerComp::UpdateComponentSelection()
{
	// update component selection and related menu actions:
	icmpstr::IElementSelectionInfo* elementSelectionInfoPtr = dynamic_cast<icmpstr::IElementSelectionInfo*>(GetObjectPtr());
	if (elementSelectionInfoPtr != NULL){
		if (m_quickHelpViewerCompPtr.IsValid()){
			const icomp::CComponentAddress* addressPtr = elementSelectionInfoPtr->GetSelectedElementAddress();
			if (addressPtr != NULL){
				m_quickHelpViewerCompPtr->ShowHelp(addressPtr->GetPackageId() + "/" + addressPtr->GetComponentId(), addressPtr);
			}
		}

		m_removeComponentCommand.setEnabled(elementSelectionInfoPtr->GetSelectedElement() != NULL);
		m_renameComponentCommand.setEnabled(elementSelectionInfoPtr->GetSelectedElement() != NULL);
	}
}


} // namespace icmpstr


