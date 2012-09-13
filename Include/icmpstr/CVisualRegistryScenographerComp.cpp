#include "icmpstr/CVisualRegistryScenographerComp.h"


// Qt includes
#include <QtCore/QMimeData>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QDesktopWidget>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CIdManipBase.h"

#include "iser/CXmlStringReadArchive.h"
#include "iser/CXmlStringWriteArchive.h"

#include "ibase/IApplication.h"

#include "iqtgui/IDropConsumer.h"
#include "iqtgui/TDesignerBasicGui.h"
#include "iqtgui/CGuiComponentDialog.h"

#include "icmpstr/CRegistryElementShape.h"
#include "icmpstr/CGraphicsConnectorItem.h"
#include "icmpstr/CVisualRegistryElement.h"
#include "icmpstr/CVisualRegistryComp.h"
#include "icmpstr/CSceneProviderGuiComp.h"


namespace icmpstr
{


CVisualRegistryScenographerComp::CVisualRegistryScenographerComp()
:	m_environmentObserver(this)
{
	int lightToolFlags = ibase::IHierarchicalCommand::CF_GLOBAL_MENU | ibase::IHierarchicalCommand::CF_TOOLBAR;

	m_cutCommand.setEnabled(false);
	m_cutCommand.SetStaticFlags(ibase::IHierarchicalCommand::CF_GLOBAL_MENU);
	m_cutCommand.SetGroupId(GI_EDIT);
	m_cutCommand.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	m_copyCommand.setEnabled(false);
	m_copyCommand.SetStaticFlags(ibase::IHierarchicalCommand::CF_GLOBAL_MENU);
	m_copyCommand.SetGroupId(GI_EDIT);
	m_copyCommand.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	m_pasteCommand.setEnabled(false);
	m_pasteCommand.SetStaticFlags(ibase::IHierarchicalCommand::CF_GLOBAL_MENU);
	m_pasteCommand.SetGroupId(GI_EDIT);
	m_pasteCommand.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	m_removeComponentCommand.setEnabled(false);
	m_removeComponentCommand.SetStaticFlags(lightToolFlags);
	m_removeComponentCommand.SetGroupId(GI_EDIT);
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

	m_showRegistryTopologyCommand.SetStaticFlags(lightToolFlags | ibase::IHierarchicalCommand::CF_TOOLBAR);
	m_showRegistryTopologyCommand.SetGroupId(GI_TOOLS);
	m_showRegistryTopologyCommand.setShortcut(QKeySequence(Qt::Key_F3));

	m_editMenu.InsertChild(&m_cutCommand);
	m_editMenu.InsertChild(&m_copyCommand);
	m_editMenu.InsertChild(&m_pasteCommand);
	m_editMenu.InsertChild(&m_removeComponentCommand);

	m_registryMenu.InsertChild(&m_renameComponentCommand);
	m_registryMenu.InsertChild(&m_insertEmbeddedRegistryCommand);
	m_registryMenu.InsertChild(&m_toEmbeddedRegistryCommand);
	m_registryMenu.InsertChild(&m_exportToCodeCommand);
	m_registryMenu.InsertChild(&m_executeRegistryCommand);
	m_registryMenu.InsertChild(&m_abortRegistryCommand);
	m_registryMenu.InsertChild(&m_showRegistryTopologyCommand);

	m_registryCommand.InsertChild(&m_editMenu);
	m_registryCommand.InsertChild(&m_registryMenu);

	SetAcceptedMimeTypes(QStringList() << "text/plain");

	SetIgnoreChanges(i2d::IObject2d::CF_OBJECT_POSITION |
				istd::IChangeable::CF_MODEL |
				istd::IChangeable::CF_ACF_INTERNAL |
				istd::CChangeDelegator::CF_DELEGATED |
				icomp::IRegistryElement::CF_FLAGS_CHANGED);

	m_selectionInfo.SetParent(this);
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

		if (metaInfoPtr != NULL){
			if (metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE){
				QDir packageDir(managerPtr->GetPackagePath(registryElement.GetAddress().GetPackageId()));

				QString filePath = packageDir.absoluteFilePath(registryElement.GetAddress().GetComponentId() + ".arx");

				m_documentManagerCompPtr->FileOpen(NULL, &filePath);

				return true;
			}
		}
		else{ // embedded registry
			icmpstr::CSceneProviderGuiComp* guiCompPtr =
					dynamic_cast<icmpstr::CSceneProviderGuiComp*>(m_sceneProviderCompPtr.GetPtr());

			if (guiCompPtr){
				QByteArray id = registryElement.GetName().data();
				guiCompPtr->SelectEmbeddedInList(id, true); // will propagate signal back to OnEmbeddedRegistryChanged
			}
		}
	}

	return false;
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CVisualRegistryScenographerComp::GetCommands() const
{
	return &m_registryCommand;
}


// protected methods

icomp::IRegistry* CVisualRegistryScenographerComp::GetSelectedRegistry() const
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if ((registryPtr != NULL) && !m_embeddedRegistryId.isEmpty()){
		return registryPtr->GetEmbeddedRegistry(m_embeddedRegistryId);
	}

	return registryPtr;
}


QGraphicsItem* CVisualRegistryScenographerComp::AddShapeToScene(iser::ISerializable* elementPtr) const
{
	if (m_scenePtr == NULL){
		return NULL;
	}

	imod::IModel* elementModelPtr = dynamic_cast<imod::IModel*>(elementPtr);
	if (elementModelPtr != NULL){
		CVisualRegistryElement* registryElementPtr = dynamic_cast<CVisualRegistryElement*>(elementPtr);
		if (registryElementPtr != NULL){
			const QByteArray& elementName = registryElementPtr->GetName();
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
			QByteArray attributeId = *iter;

			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementPtr->GetAttributeInfo(attributeId);
			if (attributeInfoPtr != NULL){
				iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
				const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<icomp::CReferenceAttribute*>(attributePtr);
				if (referenceAttributePtr != NULL){		
					const QByteArray& componentId = referenceAttributePtr->GetValue();
				
					AddConnector(*sourceShapePtr, componentId, attributeId);
				}
	
				const icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr);
				if (multiReferenceAttributePtr != NULL){
					for (int referenceIndex = 0; referenceIndex < multiReferenceAttributePtr->GetValuesCount(); referenceIndex++){
						const QByteArray& componentId = multiReferenceAttributePtr->GetValueAt(referenceIndex);
						
						AddConnector(*sourceShapePtr, componentId, attributeId);
					}
				}

				const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<icomp::CFactoryAttribute*>(attributePtr);
				if (factoryAttributePtr != NULL){		
					const QByteArray& componentId = factoryAttributePtr->GetValue();

					AddConnector(*sourceShapePtr, componentId, attributeId, true);
				}
	
				const icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr);
				if (multiFactoryAttributePtr != NULL){
					for (int referenceIndex = 0; referenceIndex < multiFactoryAttributePtr->GetValuesCount(); referenceIndex++){
						const QByteArray& componentId = multiFactoryAttributePtr->GetValueAt(referenceIndex);

						AddConnector(*sourceShapePtr, componentId, attributeId, true);
					}
				}
			}
		}
	}
}


void CVisualRegistryScenographerComp::AddConnector(
			CRegistryElementShape& sourceShape,
			const QByteArray& referenceComponentId,
			const QByteArray& attributeId,
			bool isFactory)
{
	if (m_scenePtr == NULL){
		return;
	}

	I_ASSERT(m_sceneProviderCompPtr.IsValid());

	QByteArray baseId;
	QByteArray subId;
	bool isEmbedded = istd::CIdManipBase::SplitId(referenceComponentId, baseId, subId);

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
					tr("Factory of '%1'").arg(QString(attributeId)):
					tr("Reference of '%1'").arg(QString(attributeId)));

		connectorPtr->InitEnds(&sourceShape, destShapePtr);

		m_scenePtr->addItem(connectorPtr);

		return;
	}
}


icomp::IRegistryElement* CVisualRegistryScenographerComp::TryCreateComponent(
			const QByteArray& elementId,
			const icomp::CComponentAddress& address,
			const i2d::CVector2d& position)
{
	if (elementId.isEmpty()){
		return NULL;
	}

	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetSelectedRegistry(), icomp::IRegistry::CF_ELEMENT_ADDED);
	if (!registryPtr.IsValid()){
		return NULL;
	}

	QRegExp regexp("^(\\w*)_(\\d+)$");
	QString elementIdString = elementId;

	int elementValue = 0;
	QString elementBase = elementIdString;
	int pos = regexp.indexIn(elementIdString);
	if (pos >= 0){
		elementBase = regexp.cap(1);
		elementValue = regexp.cap(2).toInt();
	}

	QByteArray realElementId = elementId;
	icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();

	while (elementIds.contains(realElementId)){
		realElementId = QString("%1_%2").arg(elementBase).arg(++elementValue).toLocal8Bit();
	}

	// if the new element has empty package_id, assume it's a local composition
	if (address.GetPackageId().isEmpty() && registryPtr.GetPtr() == GetObjectPtr()){
		icomp::IRegistry* newEmbeddedRegistryPtr = registryPtr->InsertEmbeddedRegistry(realElementId);
		if (newEmbeddedRegistryPtr == NULL){
			QMessageBox::critical(NULL, tr("Error"), tr("Embedded component could not be created!"));
			return NULL;
		}
	}

	icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->InsertElementInfo(realElementId, address);
	if (elementInfoPtr != NULL){
		CVisualRegistryElement* visualElementPtr = dynamic_cast<CVisualRegistryElement*>(elementInfoPtr->elementPtr.GetPtr());
		if (visualElementPtr != NULL){
			visualElementPtr->MoveCenterTo(position);
		}

		ConnectReferences(realElementId);

		if (registryPtr->GetEmbeddedRegistry(realElementId) != NULL){
			// add an element to the scene provider embedded compositions list
			icmpstr::CSceneProviderGuiComp* guiCompPtr =
					dynamic_cast<icmpstr::CSceneProviderGuiComp*>(m_sceneProviderCompPtr.GetPtr());
			if (guiCompPtr){
				guiCompPtr->InsertEmbeddedIntoList(realElementId);
			}
		}

		return elementInfoPtr->elementPtr.GetPtr();
	}

	return NULL;
}


void CVisualRegistryScenographerComp::ConnectReferences(const QByteArray& componentRole)
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

		icomp::IElementStaticInfo::Ids attributeIds = compMetaInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_ATTRIBUTES);
		for (		icomp::IElementStaticInfo::Ids::const_iterator attrIter = attributeIds.begin();
					attrIter != attributeIds.end();
					++attrIter){
			const QByteArray& attributeId = *attrIter;

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

			if (referenceAttributePtr != NULL && referenceAttributePtr->GetValue() == componentRole){
				createAttribute = true;
			}
			else if (factoryAttributePtr != NULL && factoryAttributePtr->GetValue() == componentRole){
				createAttribute = true;
			}
			else if (multiReferenceAttributePtr != NULL){
				for (int valueIndex = 0; valueIndex < multiReferenceAttributePtr->GetValuesCount(); valueIndex++){
					if (multiReferenceAttributePtr->GetValueAt(valueIndex) == componentRole){
						createAttribute = true;
						break;
					}
				}
			}
			else if (multiFactoryAttributePtr != NULL){
				for (int valueIndex = 0; valueIndex < multiFactoryAttributePtr->GetValuesCount(); valueIndex++){
					if (multiFactoryAttributePtr->GetValueAt(valueIndex) == componentRole){
						createAttribute = true;
						break;
					}
				}
			}

			icomp::IRegistryElement* registryElementPtr = elementInfoPtr->elementPtr.GetPtr();
			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = registryElementPtr->GetAttributeInfo(attributeId);
			if (attributeInfoPtr == NULL && createAttribute){
				QByteArray attrType = staticAttributeInfoPtr->GetAttributeTypeName();
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
	bool isElementSelected = !m_selectedElementIds.isEmpty();

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		// update component selection and related menu actions:
		if (m_quickHelpViewerCompPtr.IsValid()){
			if (isElementSelected){
				const QByteArray& elementName = *m_selectedElementIds.begin();

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

	m_cutCommand.setEnabled(isElementSelected);
	m_copyCommand.setEnabled(isElementSelected);
	m_pasteCommand.setEnabled(registryPtr != NULL);

	m_removeComponentCommand.setEnabled(isElementSelected);
	m_renameComponentCommand.setEnabled(m_selectedElementIds.size() == 1);

	m_insertEmbeddedRegistryCommand.setEnabled(m_selectedElementIds.size() > 1);
	m_toEmbeddedRegistryCommand.setEnabled(m_selectedElementIds.size() > 1);
}


void CVisualRegistryScenographerComp::DoRetranslate()
{
	m_editMenu.SetName(tr("&Edit"));
	m_cutCommand.SetVisuals(
				tr("Cut"),
				tr("Cut"),
				tr("Move selected elements into clipboard"));
	m_copyCommand.SetVisuals(
				tr("Copy"),
				tr("Copy"),
				tr("Copy selected elements into clipboard"));
	m_pasteCommand.SetVisuals(
				tr("Paste"),
				tr("Paste"),
				tr("Copy selected elements from clipboard into current document"));
	m_removeComponentCommand.SetVisuals(
				tr("&Remove"), 
				tr("Remove"), 
				tr("Remove the selected element from the registry"),
				QIcon(":/Icons/Delete.svg"));
	m_renameComponentCommand.SetVisuals(
				tr("&Rename Component"), 
				tr("Rename"), 
				tr("Allow to assign new name to selected component"));
	m_registryMenu.SetVisuals(
				tr("&Registry"),
				tr("Registry"),
				tr("Set of commands manipulating registry"));
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
	m_showRegistryTopologyCommand.SetVisuals(
			tr("&Show Component Topology..."),
				tr("&Show Component Topology"), 
				tr("Show entire component topology of the current registry"),
				m_registryValidationStatusCompPtr.IsValid() ? m_registryValidationStatusCompPtr->GetStatusIcon() : QIcon());
}


// reimplemented (iqt2d::TScenographerCompBase)

bool CVisualRegistryScenographerComp::OnDropObject(const QMimeData& mimeData, QGraphicsSceneDragDropEvent* eventPtr)
{
	if (!mimeData.hasText()){
		return false;
	}

	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetSelectedRegistry(), icomp::IRegistry::CF_ELEMENT_ADDED | istd::IChangeable::CF_MODEL);
	if (!registryPtr.IsValid()){
		return false;
	}

	i2d::CVector2d position(0, 0);
	if (eventPtr != NULL){
		position = i2d::CVector2d(eventPtr->scenePos());
	}

	iser::CXmlStringReadArchive archive(mimeData.text().toLocal8Bit(), false);
	icomp::CComponentAddress address;

	if (address.Serialize(archive)){
		if (TryCreateComponent(address.GetComponentId(), address, position) != NULL){
			return true;
		}
		else{
			QMessageBox::critical(NULL, tr("Error"), tr("Component could not be added")); 
		}
	}

	return false;
}


void CVisualRegistryScenographerComp::UpdateScene(int /*updateFlags*/)
{
	QList<QGraphicsItem*> itemsToRemove = m_scenePtr->items();
	foreach(QGraphicsItem* itemPtr, itemsToRemove){
		m_scenePtr->removeItem(itemPtr);
	}

	// add element shapes to scene
	icomp::IRegistry* registryPtr = GetSelectedRegistry();
	if (registryPtr != NULL){
		icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
		for (		icomp::IRegistry::Ids::iterator iter = elementIds.begin();
					iter != elementIds.end();
					iter++){
			const QByteArray& elementId = *iter;
			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementId);
			if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
				AddShapeToScene(elementInfoPtr->elementPtr.GetPtr());
			}
		}
	}

	UpdateEmbeddedRegistriesList();

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


// reimplemented (imod::CMultiModelDispatcherBase)

void CVisualRegistryScenographerComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_showRegistryTopologyCommand.setIcon(m_registryValidationStatusCompPtr->GetStatusIcon());
}


// reimplemented (imod::CSingleModelObserverBase)
	
bool CVisualRegistryScenographerComp::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		if (m_registryObserverCompPtr.IsValid()){
			imod::IModel* registryModelPtr = GetModelPtr();
			if (registryModelPtr != NULL && !registryModelPtr->IsAttached(m_registryObserverCompPtr.GetPtr())){
				registryModelPtr->AttachObserver(m_registryObserverCompPtr.GetPtr());
			}
		}

		return true;
	}

	return false;
}


bool CVisualRegistryScenographerComp::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		if (m_registryObserverCompPtr.IsValid()){
			imod::IModel* registryModelPtr = GetModelPtr();
			if (registryModelPtr != NULL && registryModelPtr->IsAttached(m_registryObserverCompPtr.GetPtr())){
				registryModelPtr->DetachObserver(m_registryObserverCompPtr.GetPtr());
			}
		}

		return true;
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CVisualRegistryScenographerComp::OnComponentCreated()
{
	// connect to embedded registries list selection signal
	QObject* objectPtr = dynamic_cast<QObject*>(m_sceneProviderCompPtr.GetPtr());
	if (objectPtr){
		// TODO: remove signals related to embedded registry!
		connect(	objectPtr, SIGNAL(embeddedRegistrySelected(const QByteArray&)),
					this, SLOT(OnEmbeddedRegistrySelected(const QByteArray&)));
	}

	BaseClass::OnComponentCreated();

	connect(&m_cutCommand, SIGNAL(triggered()), this, SLOT(OnCutCommand()));
	connect(&m_copyCommand, SIGNAL(triggered()), this, SLOT(OnCopyCommand()));
	connect(&m_pasteCommand, SIGNAL(triggered()), this, SLOT(OnPasteCommand()));
	connect(&m_removeComponentCommand, SIGNAL(triggered()), this, SLOT(OnRemoveComponent()));
	connect(&m_renameComponentCommand, SIGNAL(triggered()), this, SLOT(OnRenameComponent()));

	connect(&m_insertEmbeddedRegistryCommand, SIGNAL(triggered()), this, SLOT(InsertEmbeddedComponent()));
	connect(&m_toEmbeddedRegistryCommand, SIGNAL(triggered()), this, SLOT(ToEmbeddedComponent()));
	connect(&m_exportToCodeCommand, SIGNAL(triggered()), this, SLOT(OnExportToCode()));
	connect(&m_executeRegistryCommand, SIGNAL(triggered()), this, SLOT(OnExecute()));
	connect(&m_abortRegistryCommand, SIGNAL(triggered()), this, SLOT(OnAbort()));
	connect(&m_showRegistryTopologyCommand, SIGNAL(triggered()), this, SLOT(OnShowRegistryTopology()));
	
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

	if (m_registryValidationStatusModelCompPtr.IsValid()){
		RegisterModel(m_registryValidationStatusModelCompPtr.GetPtr());
	}

	DoRetranslate();
}


void CVisualRegistryScenographerComp::OnComponentDestroyed()
{
	if (m_envManagerModelCompPtr.IsValid() && m_envManagerModelCompPtr->IsAttached(&m_environmentObserver)){
		m_envManagerModelCompPtr->DetachObserver(&m_environmentObserver);
	}

	UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


// static methods

IElementSelectionInfo* CVisualRegistryScenographerComp::ExtractSelectionInterface(CVisualRegistryScenographerComp& component)
{
	return &component.m_selectionInfo;
}


imod::IModel* CVisualRegistryScenographerComp::ExtractSelectionInterfaceModel(CVisualRegistryScenographerComp& component)
{
	return &component.m_selectionInfo;
}


istd::IChangeable* CVisualRegistryScenographerComp::ExtractSelectionInterfaceChangeable(CVisualRegistryScenographerComp& component)
{
	return &component.m_selectionInfo;
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
		istd::CChangeNotifier changePtr(&m_selectionInfo, IElementSelectionInfo::CF_SELECTION);

		m_selectedElementIds = elementIds;

		UpdateComponentSelection();
	}
}


void CVisualRegistryScenographerComp::OnCutCommand()
{
	OnCopyCommand();
	OnRemoveComponent();
}


void CVisualRegistryScenographerComp::OnCopyCommand()
{
	icomp::IRegistry* registryPtr = GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	QClipboard* clipboardPtr = QApplication::clipboard();
	if (clipboardPtr != NULL){
		iser::CXmlStringWriteArchive archive(NULL, false);

		int elementsCount = int(m_selectedElementIds.size());

		bool retVal = archive.BeginMultiTag(s_elementsListTag, s_elementTag, elementsCount);
		for (		ElementIds::iterator iter = m_selectedElementIds.begin();
					iter != m_selectedElementIds.end();
					++iter){
			QByteArray elementId = *iter;

			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementId);
			if ((elementInfoPtr == NULL) || (!elementInfoPtr->elementPtr.IsValid())){
				continue;
			}

			i2d::CVector2d centerPosition;
			const CVisualRegistryElement* elementPtr = dynamic_cast<const CVisualRegistryElement*>(elementInfoPtr->elementPtr.GetPtr());
			if (elementPtr != NULL){
				centerPosition = elementPtr->GetCenter();
			}

			retVal = retVal && archive.BeginTag(s_elementTag);

			retVal = retVal && archive.BeginTag(s_elementIdTag);
			retVal = retVal && archive.Process(elementId);
			retVal = retVal && archive.EndTag(s_elementIdTag);

			retVal = retVal && archive.BeginTag(s_elementAddressTag);
			retVal = retVal && const_cast<icomp::CComponentAddress&>(elementInfoPtr->address).Serialize(archive);
			retVal = retVal && archive.EndTag(s_elementAddressTag);

			retVal = retVal && archive.BeginTag(s_elementCenterTag);
			retVal = retVal && centerPosition.Serialize(archive);
			retVal = retVal && archive.EndTag(s_elementCenterTag);

			retVal = retVal && elementInfoPtr->elementPtr->Serialize(archive);

			retVal = retVal && archive.EndTag(s_elementTag);
		}
		retVal = retVal && archive.EndTag(s_elementsListTag);

		QMimeData* mimeDataPtr = new QMimeData;
		mimeDataPtr->setText(archive.GetString());

		clipboardPtr->setMimeData(mimeDataPtr);
	}
}


void CVisualRegistryScenographerComp::OnPasteCommand()
{
	QClipboard* clipboardPtr = QApplication::clipboard();
	if (clipboardPtr == NULL){
		return;
	}

	const QMimeData* mimeDataPtr = clipboardPtr->mimeData();
	if ((mimeDataPtr == NULL) || (!mimeDataPtr->hasText())){
		return;
	}

	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_ELEMENT_ADDED | istd::IChangeable::CF_MODEL);
	if (!registryPtr.IsValid()){
		return;
	}

	iser::CXmlStringReadArchive archive(mimeDataPtr->text().toLocal8Bit(), false);

	int elementsCount = 0;

	bool hasErrors = false;

	bool retVal = archive.BeginMultiTag(s_elementsListTag, s_elementTag, elementsCount);
	for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(s_elementTag);

		QByteArray elementId;
		retVal = retVal && archive.BeginTag(s_elementIdTag);
		retVal = retVal && archive.Process(elementId);
		retVal = retVal && archive.EndTag(s_elementIdTag);

		icomp::CComponentAddress address;
		retVal = retVal && archive.BeginTag(s_elementAddressTag);
		retVal = retVal && address.Serialize(archive);
		retVal = retVal && archive.EndTag(s_elementAddressTag);

		i2d::CVector2d centerPosition;
		retVal = retVal && archive.BeginTag(s_elementCenterTag);
		retVal = retVal && centerPosition.Serialize(archive);
		retVal = retVal && archive.EndTag(s_elementCenterTag);

		if (!retVal){
			return;
		}

		icomp::IRegistryElement* elementPtr = TryCreateComponent(elementId, address, centerPosition);
		if (elementPtr != NULL){
			retVal = retVal && elementPtr->Serialize(archive);
		}

		retVal = retVal && archive.EndTag(s_elementTag);
	}
	retVal = retVal && archive.EndTag(s_elementsListTag);

	if (hasErrors){
		QMessageBox::critical(NULL, tr("Error"), tr("Some components could not be added")); 
	}

}


void CVisualRegistryScenographerComp::OnRemoveComponent()
{
	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetSelectedRegistry(), icomp::IRegistry::CF_ELEMENT_REMOVED | istd::IChangeable::CF_MODEL);
	if (registryPtr.IsValid()){
		for (		ElementIds::const_iterator iter = m_selectedElementIds.begin();
					iter != m_selectedElementIds.end();
					++iter){
			const QByteArray& elementName = *iter;
			if (registryPtr->RemoveEmbeddedRegistry(elementName)){
				icmpstr::CSceneProviderGuiComp* guiCompPtr = dynamic_cast<icmpstr::CSceneProviderGuiComp*>(m_sceneProviderCompPtr.GetPtr());
				if (guiCompPtr){
					guiCompPtr->RemoveEmbeddedFromList(elementName);
				}
			}

			registryPtr->RemoveElementInfo(elementName);
		}
	}
}


void CVisualRegistryScenographerComp::OnRenameComponent()
{
	icomp::IRegistry* registryPtr = GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	ElementIds::const_iterator firstIter = m_selectedElementIds.begin();
	if (firstIter == m_selectedElementIds.end()){
		return;
	}

	const QByteArray& oldName = *firstIter;

	bool isOk = false;
	QByteArray newName = QInputDialog::getText(
				NULL,
				tr("ACF Compositor"),
				tr("New component name"),
				QLineEdit::Normal,
				oldName,
				&isOk).toLocal8Bit();
	if (!isOk || newName.isEmpty() || (oldName == newName)){
		return;
	}

	registryPtr->RenameElement(oldName, newName);
}


void CVisualRegistryScenographerComp::InsertEmbeddedComponent()
{
	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_EMBEDDED | icomp::IRegistry::CF_ELEMENT_ADDED);
	if (!registryPtr.IsValid()){
		return;
	}

	bool isOk = false;
	QByteArray newName = QInputDialog::getText(
				NULL,
				tr("ACF Compositor"),
				tr("New embedded component name"),
				QLineEdit::Normal,
				"",
				&isOk).toLocal8Bit();
	if (!isOk || newName.isEmpty()){
		return;
	}

	icomp::IRegistry* newEmbeddedRegistryPtr = registryPtr->InsertEmbeddedRegistry(newName);
	if (newEmbeddedRegistryPtr == NULL){
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
	istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_EMBEDDED | icomp::IRegistry::CF_ELEMENT_ADDED | icomp::IRegistry::CF_ELEMENT_REMOVED);
	if (!registryPtr.IsValid()){
		return;
	}

	bool isOk = false;
	QByteArray newName = QInputDialog::getText(
				NULL,
				tr("ACF Compositor"),
				tr("New embedded component name"),
				QLineEdit::Normal,
				"",
				&isOk).toLocal8Bit();
	if (!isOk || newName.isEmpty()){
		return;
	}

	icomp::IRegistry* newEmbeddedRegistryPtr = registryPtr->InsertEmbeddedRegistry(newName);
	if (newEmbeddedRegistryPtr == NULL){
		QMessageBox::critical(NULL, tr("Error"), tr("Embedded component could not be created!")); 
		return;
	}

	if (registryPtr->InsertElementInfo(newName, icomp::CComponentAddress("", newName)) == NULL){
		QMessageBox::critical(NULL, tr("Error"), tr("Component could not be added")); 
		return;
	}

	icomp::IRegistry::ExportedInterfacesMap exportedInterfacesMap = registryPtr->GetExportedInterfacesMap();
	icomp::IRegistry::ExportedElementsMap exportedComponentsMap = registryPtr->GetExportedElementsMap();

	// move selected elements to the embedded registry
	for (		ElementIds::const_iterator iter = m_selectedElementIds.begin();
				iter != m_selectedElementIds.end();
				++iter){
		const QByteArray& elementName = *iter;

		icomp::IRegistry::ElementInfo* oldInfoPtr = const_cast<icomp::IRegistry::ElementInfo*>(registryPtr->GetElementInfo(elementName));
		if (oldInfoPtr == NULL){
			continue;
		}

		icomp::IRegistry::ElementInfo* newInfoPtr = newEmbeddedRegistryPtr->InsertElementInfo(elementName, oldInfoPtr->address, false);
		if (newInfoPtr == NULL){
			continue;
		}

		newInfoPtr->elementPtr.TakeOver(oldInfoPtr->elementPtr);

		for (		icomp::IRegistry::ExportedInterfacesMap::ConstIterator interfaceIter = exportedInterfacesMap.constBegin();
					interfaceIter != exportedInterfacesMap.constEnd();
					++interfaceIter){
			QByteArray baseId;
			QByteArray subId;
			istd::CIdManipBase::SplitId(interfaceIter.value(), baseId, subId);
			if (baseId == elementName){
				QByteArray completeElementId = istd::CIdManipBase::JoinId(elementName, subId);

				newEmbeddedRegistryPtr->SetElementInterfaceExported(completeElementId, interfaceIter.key(), true);
			}
		}

		for (		icomp::IRegistry::ExportedElementsMap::ConstIterator subcomponentIter = exportedComponentsMap.constBegin();
					subcomponentIter != exportedComponentsMap.constEnd();
					++subcomponentIter){
			QByteArray baseId;
			QByteArray subId;
			istd::CIdManipBase::SplitId(subcomponentIter.value(), baseId, subId);
			if (baseId == elementName){
				QByteArray completeElementId = istd::CIdManipBase::JoinId(elementName, subId);

				newEmbeddedRegistryPtr->SetElementExported(subcomponentIter.key(), elementName);
			}
		}
		/*
		// this code requires a way to get interface type for referenced elements

			// replace referenced interfaces of moved elements with exports
			icomp::IRegistry::Ids ids = newEmbeddedRegistryPtr->GetElementIds();
			for (icomp::IRegistry::Ids::const_iterator iter = ids.begin(); iter != ids.end(); iter++){
				// skip moved elements themselves
				if (m_selectedElementIds.contains(*iter)){
					continue;
				}

				const icomp::IRegistry::ElementInfo* info = registryPtr->GetElementInfo(*iter);
				if (info == NULL || !info->elementPtr.IsValid()){
					continue;
				}
				// search for attributes referencing moved elements
				icomp::IRegistry::Ids attrIds = info->elementPtr->GetAttributeIds();
				for (icomp::IRegistry::Ids::const_iterator iter = attrIds.begin(); iter != attrIds.end(); iter++){
					icomp::IRegistryElement::AttributeInfo* attrInfo = info->elementPtr->GetAttributeInfo(*iter);
					if (attrInfo == NULL || !attrInfo->attributeTypeName.startsWith("Reference")){
						continue;
					}

					icomp::CReferenceAttribute *attr = attrInfo->attributePtr.Cast<icomp::CReferenceAttribute *>();
					icomp::CMultiReferenceAttribute *attrMulti = attrInfo->attributePtr.Cast<icomp::CMultiReferenceAttribute *>();
					QSet<QByteArray>refElemNames;

					if (attr != NULL){
						QByteArray refElemName = attr->GetValue();
						if (m_selectedElementIds.contains(refElemName)){
							refElemNames.insert(refElemName);
							attr->
						}
					}
					else if (attrMulti != NULL){
						for (int i = 0; i < attrMulti->GetValuesCount(); i++){
							QByteArray refElemName = attrMulti->GetValueAt(i);
							if (m_selectedElementIds.contains(refElemName)){
								refElemNames.insert(refElemName);
							}

						}
					}
			
				}
			}
		 */
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
			if (m_registryCodeSaverCompPtr->SaveToFile(*registryPtr, file) == iser::IFileLoader::StateFailed){
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


void CVisualRegistryScenographerComp::OnShowRegistryTopology()
{
	if (m_registryTopologyGuiCompPtr.IsValid()){
		iqtgui::CGuiComponentDialog dialog(m_registryTopologyGuiCompPtr.GetPtr());
		dialog.setWindowIcon(m_showRegistryTopologyCommand.icon());
		dialog.setWindowTitle(tr("Registry Topology"));

		const QDesktopWidget* desktopPtr = QApplication::desktop();
		I_ASSERT(desktopPtr != NULL);

		QRect screenRect = desktopPtr->screenGeometry();

		dialog.resize(int(screenRect.width() * 0.7), int(screenRect.height() * 0.7));

		dialog.exec();
	}
}


void CVisualRegistryScenographerComp::OnEmbeddedRegistrySelected(const QByteArray& id)
{
	if (id != m_embeddedRegistryId){
		m_embeddedRegistryId = id;

		UpdateScene(0);
	}
}


void CVisualRegistryScenographerComp::UpdateEmbeddedRegistriesList()
{
	// add embedded registries to the list below the scene
	icmpstr::CSceneProviderGuiComp* guiCompPtr = dynamic_cast<icmpstr::CSceneProviderGuiComp*>(m_sceneProviderCompPtr.GetPtr());
	icomp::IRegistry* rootRegistryPtr = GetObjectPtr();
	if (guiCompPtr != NULL && guiCompPtr->IsGuiCreated() && rootRegistryPtr != NULL){
		icomp::IRegistry::Ids embeddedIds = rootRegistryPtr->GetEmbeddedRegistryIds();
		icomp::IRegistry::Ids::iterator iter;
		for (iter = embeddedIds.begin(); iter != embeddedIds.end(); iter++){
			const QByteArray& elementId = *iter;
			guiCompPtr->InsertEmbeddedIntoList(elementId);
		}
	}
}


// static attributes
iser::CArchiveTag CVisualRegistryScenographerComp::s_elementsListTag("ElementsList", "List of elements");
iser::CArchiveTag CVisualRegistryScenographerComp::s_elementTag("Element", "Single element", true);
iser::CArchiveTag CVisualRegistryScenographerComp::s_elementIdTag("Id", "Id of element");
iser::CArchiveTag CVisualRegistryScenographerComp::s_elementAddressTag("Address", "Address of component");
iser::CArchiveTag CVisualRegistryScenographerComp::s_elementCenterTag("Center", "Center position of element");


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


// public methods of embedded class SelectionInfoImpl

void CVisualRegistryScenographerComp::SelectionInfoImpl::SetParent(CVisualRegistryScenographerComp* parentPtr)
{
	m_parentPtr = parentPtr;
}


// reimplemented (icmpstr::IElementSelectionInfo)

icomp::IRegistry* CVisualRegistryScenographerComp::SelectionInfoImpl::GetSelectedRegistry() const
{
	I_ASSERT(m_parentPtr != NULL);	// parent should be set before any subelement can be accessed

	return m_parentPtr->GetSelectedRegistry();
}


IElementSelectionInfo::Elements CVisualRegistryScenographerComp::SelectionInfoImpl::GetSelectedElements() const
{
	I_ASSERT(m_parentPtr != NULL);	// parent should be set before any subelement can be accessed

	IElementSelectionInfo::Elements retVal;

	icomp::IRegistry* registryPtr = GetSelectedRegistry();
	if (registryPtr != NULL){
		for (		ElementIds::const_iterator iter = m_parentPtr->m_selectedElementIds.begin();
					iter != m_parentPtr->m_selectedElementIds.end();
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


