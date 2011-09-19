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
:	m_environmentObserver(this)
{
	int lightToolFlags = ibase::IHierarchicalCommand::CF_GLOBAL_MENU | ibase::IHierarchicalCommand::CF_TOOLBAR;

	m_removeComponentCommand.setEnabled(false);
	m_removeComponentCommand.SetGroupId(GI_COMPONENT);
	m_removeComponentCommand.SetStaticFlags(lightToolFlags);
	m_removeComponentCommand.setShortcut(QKeySequence(Qt::Key_Delete));
	m_renameComponentCommand.setEnabled(false);
	m_renameComponentCommand.SetGroupId(GI_COMPONENT);
	m_renameComponentCommand.setShortcut(QKeySequence(Qt::Key_F2));
	m_insertEmbeddedRegistryCommand.setEnabled(false);
	m_insertEmbeddedRegistryCommand.SetGroupId(GI_EMBEDDED_REGISTRY);
	m_toEmbeddedRegistryCommand.setEnabled(false);
	m_toEmbeddedRegistryCommand.SetGroupId(GI_EMBEDDED_REGISTRY);
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
	m_registryMenu.InsertChild(&m_insertEmbeddedRegistryCommand);
	m_registryMenu.InsertChild(&m_toEmbeddedRegistryCommand);
	m_registryMenu.InsertChild(&m_exportToCodeCommand);
	m_registryMenu.InsertChild(&m_executeRegistryCommand);
	m_registryMenu.InsertChild(&m_abortRegistryCommand);
	m_registryMenu.InsertChild(&m_addNoteCommand);
	m_registryMenu.InsertChild(&m_removeNoteCommand);
	m_registryCommand.InsertChild(&m_registryMenu);

	SetAcceptedMimeTypes(QStringList() << "component");

	SetIgnoreChanges(i2d::IObject2d::CF_OBJECT_POSITION |
				istd::IChangeable::CF_MODEL |
				istd::IChangeable::CF_ACF_INTERNAL |
				istd::CChangeDelegator::CF_DELEGATED |
				icomp::IRegistryElement::CF_FLAGS_CHANGED);
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
	if ((managerPtr != NULL) && m_documentManagerCompPtr.IsValid()){
		
		const icomp::IComponentStaticInfo* metaInfoPtr = managerPtr->GetComponentMetaInfo(registryElement.GetAddress());

		if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
			QDir packageDir(iqt::GetQString(managerPtr->GetPackagePath(registryElement.GetAddress().GetPackageId())));
		
			istd::CString filePath = iqt::GetCString(packageDir.absoluteFilePath((registryElement.GetAddress().GetComponentId() + ".arx").c_str()));

			m_documentManagerCompPtr->FileOpen(NULL, &filePath);

			return true;
		}
	}

	return false;
}


// reimplemented (icmpstr::IElementSelectionInfo)

icomp::IRegistry* CVisualRegistryScenographerComp::GetSelectedRegistry() const
{
	return GetObjectPtr();
}


IElementSelectionInfo::Elements CVisualRegistryScenographerComp::GetSelectedElements() const
{
	IElementSelectionInfo::Elements retVal;

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		for (		ElementIds::const_iterator iter = m_selectedElementIds.begin();
					iter != m_selectedElementIds.end();
					++iter){
			const std::string& elementName = *iter;

			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementName);
			if (elementInfoPtr != NULL){
				retVal[elementName] = elementInfoPtr;
			}
		}
	}

	return retVal;
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CVisualRegistryScenographerComp::GetCommands() const
{
	return &m_registryCommand;
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
			const std::string& elementName = registryElementPtr->GetName();
			CRegistryElementShape* shapePtr = new CRegistryElementShape(this, m_sceneProviderCompPtr.GetPtr());

			bool isElementSelected = (m_selectedElementIds.find(elementName) != m_selectedElementIds.end());
			shapePtr->setSelected(isElementSelected);

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
					visualElementPtr->MoveCenterTo(position);
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

		icomp::IComponentStaticInfo::Ids attributeIds = compMetaInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_ATTRIBUTES);
		for (		icomp::IComponentStaticInfo::Ids::const_iterator attrIter = attributeIds.begin();
					attrIter != attributeIds.end();
					++attrIter){
			const std::string& attributeId = *attrIter;

			const icomp::IAttributeStaticInfo* staticAttributeInfoPtr = compMetaInfoPtr->GetAttributeInfo(attributeId);
			if (staticAttributeInfoPtr == NULL){
				continue;
			}

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
	bool isElementSelected = !m_selectedElementIds.empty();

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		// update component selection and related menu actions:
		if (m_quickHelpViewerCompPtr.IsValid()){
			if (isElementSelected){
				const std::string& elementName = *m_selectedElementIds.begin();

				const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementName);
				if (elementInfoPtr != NULL){
					const icomp::CComponentAddress& address = elementInfoPtr->address;
					m_quickHelpViewerCompPtr->ShowHelp(
								address.ToString(),
								&address);
				}
			}
		}
	}

	m_removeComponentCommand.setEnabled(isElementSelected);
	m_renameComponentCommand.setEnabled(m_selectedElementIds.size() == 1);

	m_insertEmbeddedRegistryCommand.setEnabled(m_selectedElementIds.size() > 1);
	m_toEmbeddedRegistryCommand.setEnabled(m_selectedElementIds.size() > 1);
}


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
	m_insertEmbeddedRegistryCommand.SetVisuals(
				tr("&Insert Embedded Composition"), 
				tr("Insert Embedded"), 
				tr("Insert new embedded component composition used to group set of components and manage it as single one"));
	m_toEmbeddedRegistryCommand.SetVisuals(
				tr("To &Embedded Composition"), 
				tr("To Embedded"), 
				tr("Make embedded component composition from selected elements.\nIt allows to group set of components and manage it as single one"),
				QIcon(":/Icons/ToEmbeddedComponent.svg"));
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


// reimplemented (iqt2d::TScenographerCompBase)

bool CVisualRegistryScenographerComp::OnDropObject(const QMimeData& mimeData, QGraphicsSceneDragDropEvent* eventPtr)
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


void CVisualRegistryScenographerComp::UpdateScene(int /*updateFlags*/)
{
	QList<QGraphicsItem*> itemsToRemove = m_scenePtr->items();
	foreach(QGraphicsItem* itemPtr, itemsToRemove){
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

	QList<QGraphicsItem*> items = m_scenePtr->items();
	foreach(QGraphicsItem* itemPtr, items){
		CRegistryElementShape* elementShapePtr = dynamic_cast<CRegistryElementShape*>(itemPtr);
		if (elementShapePtr != NULL){
			elementShapePtr->CheckConsistency();
		}
	}

	UpdateComponentSelection();
}


// reimplemented (icomp::CComponentBase)

void CVisualRegistryScenographerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	connect(&m_removeComponentCommand, SIGNAL(activated()), this, SLOT(OnRemoveComponent()));
	connect(&m_renameComponentCommand, SIGNAL(activated()), this, SLOT(OnRenameComponent()));
	connect(&m_insertEmbeddedRegistryCommand, SIGNAL(activated()), this, SLOT(InsertEmbeddedComponent()));
	connect(&m_toEmbeddedRegistryCommand, SIGNAL(activated()), this, SLOT(ToEmbeddedComponent()));
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

	if (m_scenePtr != NULL){
		connect(m_scenePtr, SIGNAL(selectionChanged()), this, SLOT(OnSelectionChanged()));
	}

	m_elementNameFont = qApp->font();
	m_elementNameFont.setBold(true);
	m_elementNameFont.setPointSize(12);

	m_elementDetailFont = qApp->font();
	m_elementDetailFont.setPointSize(8);

	if (m_envManagerModelCompPtr.IsValid()){
		m_envManagerModelCompPtr->AttachObserver(&m_environmentObserver);
	}

	DoRetranslate();
}


void CVisualRegistryScenographerComp::OnComponentDestroyed()
{
	if (m_envManagerModelCompPtr.IsValid() && m_envManagerModelCompPtr->IsAttached(&m_environmentObserver)){
		m_envManagerModelCompPtr->DetachObserver(&m_environmentObserver);
	}

	BaseClass::OnComponentDestroyed();
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
	
	ElementIds elementIds;

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
			elementIds.insert(elementPtr->GetName());
		}	
	}

	if (m_selectedElementIds != elementIds){
		istd::CChangeNotifier changePtr(this, CF_SELECTION);

		m_selectedElementIds = elementIds;

		UpdateComponentSelection();
	}
}


void CVisualRegistryScenographerComp::OnRemoveComponent()
{
	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_COMPONENT_REMOVED | CF_MODEL);
	if (registryPtr.IsValid()){
		for (		ElementIds::const_iterator iter = m_selectedElementIds.begin();
					iter != m_selectedElementIds.end();
					++iter){
			const std::string& elementName = *iter;

			registryPtr->RemoveElementInfo(elementName);
		}
	}
}


void CVisualRegistryScenographerComp::OnRenameComponent()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr == NULL){
		return;
	}

	ElementIds::const_iterator firstIter = m_selectedElementIds.begin();
	if (firstIter == m_selectedElementIds.end()){
		return;
	}

	const std::string& oldName = *firstIter;

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

	registryPtr->RenameElement(oldName, newName);
}


void CVisualRegistryScenographerComp::InsertEmbeddedComponent()
{
	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_EMBEDDED | icomp::IRegistry::CF_COMPONENT_ADDED);
	if (!registryPtr.IsValid()){
		return;
	}

	bool isOk = false;
	std::string newName = QInputDialog::getText(
				NULL,
				tr("ACF Compositor"),
				tr("New embedded component name"),
				QLineEdit::Normal,
				"",
				&isOk).toStdString();
	if (!isOk || newName.empty()){
		return;
	}

	icomp::IRegistry* newEbeddedRegistryPtr = registryPtr->InsertEmbeddedRegistry(newName);
	if (newEbeddedRegistryPtr == NULL){
		QMessageBox::critical(NULL, tr("Error"), tr("Embedded component could not be created!")); 
		return;
	}

	if (registryPtr->InsertElementInfo(newName, icomp::CComponentAddress("", newName)) == NULL){
		QMessageBox::critical(NULL, tr("Error"), tr("Component could not be added")); 
		return;
	}
}


void CVisualRegistryScenographerComp::ToEmbeddedComponent()
{
	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_EMBEDDED | icomp::IRegistry::CF_COMPONENT_ADDED | icomp::IRegistry::CF_COMPONENT_REMOVED);
	if (!registryPtr.IsValid()){
		return;
	}

	bool isOk = false;
	std::string newName = QInputDialog::getText(
				NULL,
				tr("ACF Compositor"),
				tr("New embedded component name"),
				QLineEdit::Normal,
				"",
				&isOk).toStdString();
	if (!isOk || newName.empty()){
		return;
	}

	icomp::IRegistry* newEbeddedRegistryPtr = registryPtr->InsertEmbeddedRegistry(newName);
	if (newEbeddedRegistryPtr == NULL){
		QMessageBox::critical(NULL, tr("Error"), tr("Embedded component could not be created!")); 
		return;
	}

	if (registryPtr->InsertElementInfo(newName, icomp::CComponentAddress("", newName)) == NULL){
		QMessageBox::critical(NULL, tr("Error"), tr("Component could not be added")); 
		return;
	}

	for (		ElementIds::const_iterator iter = m_selectedElementIds.begin();
				iter != m_selectedElementIds.end();
				++iter){
		const std::string& elementName = *iter;

		icomp::IRegistry::ElementInfo* oldInfoPtr = const_cast<icomp::IRegistry::ElementInfo*>(registryPtr->GetElementInfo(elementName));
		if (oldInfoPtr == NULL){
			continue;
		}

		icomp::IRegistry::ElementInfo* newInfoPtr = newEbeddedRegistryPtr->InsertElementInfo(elementName, oldInfoPtr->address, false);
		if (newInfoPtr == NULL){
			continue;
		}

		newInfoPtr->elementPtr.TakeOver(oldInfoPtr->elementPtr);

		registryPtr->RemoveElementInfo(elementName);
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


void CVisualRegistryScenographerComp::OnExecutionTimerTick()
{
	bool isExecutable = false;
	bool isRunning = false;

	if (m_registryPreviewCompPtr.IsValid()){
		isRunning = m_registryPreviewCompPtr->IsRunning();

		icomp::IRegistry* registryPtr = GetObjectPtr();
		if (registryPtr != NULL){
			const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registryPtr->GetExportedInterfacesMap();
			isExecutable = (interfacesMap.find(istd::CClassInfo::GetName<ibase::IApplication>()) != interfacesMap.end());
		}
	}

	m_executeRegistryCommand.setEnabled(!isRunning && isExecutable);
	m_abortRegistryCommand.setEnabled(isRunning);
}


// public methods of embedded class EnvironmentObserver

CVisualRegistryScenographerComp::EnvironmentObserver::EnvironmentObserver(CVisualRegistryScenographerComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class EnvironmentObserver

// reimplemented (imod::TSingleModelObserverBase)

void CVisualRegistryScenographerComp::EnvironmentObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.UpdateScene(updateFlags);
}


} // namespace icmpstr


