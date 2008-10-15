#include "CRegistryViewComp.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

#include <math.h>

#include "icomp/IRegistryGeometryProvider.h"
#include "icomp/IRegistryNotesProvider.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"

#include "CComponentView.h"
#include "CComponentConnector.h"
#include "CRegistryModelComp.h"


CRegistryViewComp::CRegistryViewComp()
:	m_selectedComponentPtr(NULL)
{
	SetUpdateFilter(~CRegistryModelComp::CF_POSITION);

	int lightToolFlags = idoc::IHierarchicalCommand::CF_GLOBAL_MENU | idoc::IHierarchicalCommand::CF_TOOLBAR;

	m_removeComponentCommand.setEnabled(false);
	m_removeComponentCommand.SetGroupId(GI_COMPONENT);
	m_removeComponentCommand.SetStaticFlags(lightToolFlags);
	m_removeComponentCommand.setShortcut(QKeySequence(Qt::Key_Delete));
	m_renameComponentCommand.setEnabled(false);
	m_renameComponentCommand.SetGroupId(GI_COMPONENT);
	m_renameComponentCommand.setShortcut(QKeySequence(Qt::Key_F2));
	m_exportComponentCommand.setEnabled(false);
	m_exportComponentCommand.SetGroupId(GI_COMPONENT);
	m_exportInterfaceCommand.setEnabled(false);
	m_exportInterfaceCommand.SetGroupId(GI_COMPONENT);
	m_exportInterfaceCommand.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
	m_exportToCodeCommand.setEnabled(false);
	m_exportToCodeCommand.SetGroupId(GI_CODEGEN);
	m_executeRegistryCommand.setEnabled(false);
	m_executeRegistryCommand.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	m_executeRegistryCommand.SetGroupId(GI_PREVIEW);
	m_executeRegistryCommand.SetStaticFlags(lightToolFlags);
	m_abortRegistryCommand.setEnabled(false);
	m_abortRegistryCommand.SetGroupId(GI_PREVIEW);
	m_abortRegistryCommand.SetStaticFlags(lightToolFlags);
	m_abortRegistryCommand.setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	m_removeNoteCommand.setEnabled(false);
	m_removeNoteCommand.SetGroupId(GI_COMPONENT);
	m_removeNoteCommand.SetStaticFlags(lightToolFlags);
	m_addNoteCommand.setEnabled(false);
	m_addNoteCommand.SetGroupId(GI_COMPONENT);
	m_addNoteCommand.SetStaticFlags(lightToolFlags);

	m_registryMenu.InsertChild(&m_removeComponentCommand);
	m_registryMenu.InsertChild(&m_renameComponentCommand);
	m_registryMenu.InsertChild(&m_exportComponentCommand);
	m_registryMenu.InsertChild(&m_exportInterfaceCommand);
	m_registryMenu.InsertChild(&m_exportToCodeCommand);
	m_registryMenu.InsertChild(&m_executeRegistryCommand);
	m_registryMenu.InsertChild(&m_abortRegistryCommand);
	m_registryMenu.InsertChild(&m_addNoteCommand);
	m_registryMenu.InsertChild(&m_removeNoteCommand);
	m_registryCommand.InsertChild(&m_registryMenu);

	m_scenePtr = new CRegistryScene(*this);

	m_scenePtr->setFocus();
}


bool CRegistryViewComp::TryCreateComponent(const icomp::CComponentAddress& address, const i2d::CVector2d& position)
{
	bool retVal = false;
	QString componentName = QInputDialog::getText(GetWidget(), tr("Application Compositor"), tr("Component name"), QLineEdit::Normal, "",&retVal);
	if (retVal && !componentName.isEmpty()){
		istd::TChangeNotifier<icomp::IRegistry> registryPtr(GetObjectPtr(), icomp::IRegistry::CF_COMPONENT_ADDED);
		if (registryPtr.IsValid()){
			icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->InsertElementInfo(componentName.toStdString(), address);
			if (elementInfoPtr != NULL){
				icomp::IRegistryGeometryProvider* providerPtr = dynamic_cast<icomp::IRegistryGeometryProvider*>(registryPtr.GetPtr());
				if (providerPtr != NULL){
					providerPtr->SetComponentPosition(componentName.toStdString(), position);
				}
				ConnectReferences(componentName);
				return true;
			}
		}

		QMessageBox::critical(GetWidget(), tr("Error"), tr("Component could not be added")); 
	}

	return false;
}


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CRegistryViewComp::GetCommands() const
{
	return &m_registryCommand;
}


// reimplemented (imod::TModelEditorBase)

void CRegistryViewComp::UpdateEditor()
{
	QGraphicsView* viewPtr = GetQtWidget();
	if (viewPtr == NULL){
		return;
	}

	ResetScene();

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
		for (		icomp::IRegistry::Ids::iterator iter = elementIds.begin();
					iter != elementIds.end();
					iter++){
			const std::string& elementId = *iter;
			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementId);
			if (elementInfoPtr != NULL){
				CComponentView* viewPtr = CreateComponentView(registryPtr, elementInfoPtr, elementId);

				icomp::IRegistryGeometryProvider* geomeometryProviderPtr = dynamic_cast<icomp::IRegistryGeometryProvider*>(registryPtr);
				if (geomeometryProviderPtr != NULL){
					i2d::CVector2d position = geomeometryProviderPtr->GetComponentPosition(elementId);
					viewPtr->setPos(int(position.GetX() + 0.5), int(position.GetY() + 0.5));
				}

				if (		m_selectedComponentPtr != NULL && 
							viewPtr->GetComponentName() == m_selectedComponentPtr->GetComponentName()){
					viewPtr->setSelected(true);
				}	
			}
		}
	}

	UpdateConnectors();
}


void CRegistryViewComp::UpdateModel() const
{
}


// reimplemented (iqtgui::CGuiComponentBase)

void CRegistryViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	OnRetranslate();

	I_ASSERT(m_scenePtr != NULL);

	connect(&m_removeComponentCommand, SIGNAL( activated()), this, SLOT(OnRemoveComponent()));
	connect(&m_renameComponentCommand, SIGNAL( activated()), this, SLOT(OnRenameComponent()));
	connect(&m_exportToCodeCommand, SIGNAL( activated()), this, SLOT(OnExportToCode()));
	connect(&m_executeRegistryCommand, SIGNAL( activated()), this, SLOT(OnExecute()));
	connect(&m_abortRegistryCommand, SIGNAL( activated()), this, SLOT(OnAbort()));
	connect(&m_addNoteCommand, SIGNAL( activated()), this, SLOT(OnAddNote()));
	connect(&m_removeNoteCommand, SIGNAL( activated()), this, SLOT(OnRemoveNote()));
	connect(&m_exportComponentCommand, SIGNAL( activated()), this, SLOT(OnExportComponent()));
	connect(&m_exportInterfaceCommand, SIGNAL( activated()), this, SLOT(OnExportInterface()));

	QGraphicsView* viewPtr = GetQtWidget();
	if (viewPtr != NULL){
		viewPtr->setDragMode(QGraphicsView::ScrollHandDrag);
		viewPtr->setScene(m_scenePtr);
		viewPtr->setCacheMode(QGraphicsView::CacheBackground);
		viewPtr->setFrameShape(QFrame::NoFrame);
		viewPtr->setAcceptDrops(true);
		
		m_compositeItem.setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	
		m_scenePtr->addItem(&m_compositeItem);
	}

	if (m_registryPreviewCompPtr.IsValid()){
		connect(&m_executionObserverTimer, SIGNAL(timeout()), this, SLOT(OnExecutionTimerTick()));

		m_executionObserverTimer.start(500);
	}
}


void CRegistryViewComp::OnRetranslate()
{
	BaseClass::OnRetranslate();

	m_registryMenu.SetVisuals(
				tr("&Registry"),
				tr("Registry"),
				tr("Set of commands manipulating registry"));
	m_removeComponentCommand.SetVisuals(
				tr("&Remove Component"), 
				tr("Remove"), 
				tr("Remove the selected component from the registry"),
				QIcon(":/Resources/Icons/delete_64.png"));
	m_renameComponentCommand.SetVisuals(
				tr("&Rename Component"), 
				tr("Rename"), 
				tr("Allow to assign new name to selected component"));
	m_exportComponentCommand.SetVisuals(
				tr("&Export Component"), 
				tr("&Export Component"), 
				tr("Export component"),
				QIcon(":/Resources/Icons/Export.png"));
	m_exportInterfaceCommand.SetVisuals(
				tr("&Export Inteface(s)"), 
				tr("&Export Interface(s)"), 
				tr("Export interface(s)"),
				QIcon(":/Resources/Icons/Export.png"));
	m_exportToCodeCommand.SetVisuals(
				tr("&Export To Code..."),
				tr("Export"),
				tr("Generates C++ code according to this registry"));
	m_executeRegistryCommand.SetVisuals(
				tr("&Execute Registry"), 
				tr("&Execute Registry"), 
				tr("Start registry execution"),
				QIcon(":/Resources/Icons/player_play.png"));
	m_abortRegistryCommand.SetVisuals(
				tr("&Abort Registry"), 
				tr("&Abort Registry"), 
				tr("Abort registry execution"),
				QIcon(":/Resources/Icons/player_stop.png"));
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


// public slots:

void CRegistryViewComp::SetCenterOn(const std::string& componentName)
{
	QGraphicsView* viewPtr = GetQtWidget();
	if (viewPtr != NULL){
		QList<QGraphicsItem*> items = m_scenePtr->items();
		foreach(QGraphicsItem* item, items){
			CComponentView* itemViewPtr = dynamic_cast<CComponentView*>(item);
			if (itemViewPtr != NULL){
				if (componentName == itemViewPtr->GetComponentName()){
					viewPtr->centerOn(itemViewPtr);

					return;
				}
			}
		}
	}
}


// protected slots

void CRegistryViewComp::OnComponentViewSelected(CComponentView* viewPtr, bool isSelected)
{
	// detach last model from its observers:
	if (m_selectedComponentPtr != NULL){
		const icomp::IRegistry::ElementInfo& elementInfo = m_selectedComponentPtr->GetElementInfo();

		imod::IModel* registryElementModelPtr = dynamic_cast<imod::IModel*>(elementInfo.elementPtr.GetPtr());
		if (registryElementModelPtr != NULL){
			registryElementModelPtr->DetachAllObservers();
		}
	}

	if (isSelected){
		m_selectedComponentPtr = viewPtr;

		if (viewPtr != NULL && m_registryElementObserversCompPtr.IsValid()){
			const icomp::IRegistry::ElementInfo& elementInfo = m_selectedComponentPtr->GetElementInfo();

			for (int observerIndex = 0; observerIndex < m_registryElementObserversCompPtr.GetCount(); observerIndex++){
				imod::IObserver* observerPtr = m_registryElementObserversCompPtr[observerIndex];
				if (observerPtr != NULL){
					imod::IModel* registryElementModelPtr = dynamic_cast<imod::IModel*>(elementInfo.elementPtr.GetPtr());
					if (registryElementModelPtr != NULL){
						registryElementModelPtr->AttachObserver(observerPtr);
					}
				}
			}
		}
	}
	else{
		m_selectedComponentPtr = NULL;
	}

	m_removeComponentCommand.setEnabled(isSelected);
	m_renameComponentCommand.setEnabled(isSelected);
	m_exportComponentCommand.setEnabled(isSelected);
	m_exportInterfaceCommand.setEnabled(isSelected);
	m_exportToCodeCommand.setEnabled(isSelected);
	m_executeRegistryCommand.setEnabled(isSelected && m_registryPreviewCompPtr.IsValid());

	UpdateExportInterfaceCommand();
}


void CRegistryViewComp::OnComponentPositionChanged(CComponentView* view, const QPoint& newPosition)
{
	icomp::IRegistryGeometryProvider* geometryProviderPtr = dynamic_cast<icomp::IRegistryGeometryProvider*>(GetObjectPtr());
	if (geometryProviderPtr != NULL){
		geometryProviderPtr->SetComponentPosition(view->GetComponentName(), i2d::CVector2d(newPosition.x(), newPosition.y()));
	}

	int gridSize = GetGrid();
	QRectF boundingBox = m_compositeItem.childrenBoundingRect();
	int width = ::ceil(boundingBox.width() / gridSize) * gridSize;
	int height = ::ceil(boundingBox.height() / gridSize) * gridSize;
	boundingBox.setWidth(width);
	boundingBox.setHeight(height);

	m_compositeItem.setRect(boundingBox.adjusted(-gridSize, -gridSize, gridSize, gridSize));
}


void CRegistryViewComp::OnRemoveComponent()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		if (m_selectedComponentPtr != NULL){
			const icomp::IRegistry::ElementInfo& elementInfo = m_selectedComponentPtr->GetElementInfo();

			m_selectedComponentPtr->RemoveAllConnectors();
			m_scenePtr->removeItem(m_selectedComponentPtr);
			m_removeComponentCommand.setEnabled(false);

			registryPtr->RemoveElementInfo(m_selectedComponentPtr->GetComponentName());

			m_selectedComponentPtr = NULL;
		}
	}
}


void CRegistryViewComp::OnRenameComponent()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		if (m_selectedComponentPtr != NULL){
			const std::string& oldName = m_selectedComponentPtr->GetComponentName();

			i2d::CVector2d position(0, 0);
			icomp::IRegistryGeometryProvider* geometryProviderPtr = dynamic_cast<icomp::IRegistryGeometryProvider*>(registryPtr);
			if (geometryProviderPtr != NULL){
				position = geometryProviderPtr->GetComponentPosition(oldName);
			}

			const icomp::IRegistry::ElementInfo& oldInfo = m_selectedComponentPtr->GetElementInfo();

			bool isOk = false;
			std::string newName = QInputDialog::getText(
						NULL,
						tr("ACF Compositor"),
						tr("New component name"),
						QLineEdit::Normal,
						iqt::GetQString(oldName),
						&isOk).toStdString();
			if (isOk && !newName.empty() && (oldName != newName) && oldInfo.elementPtr.IsValid()){
				const icomp::IRegistry::ElementInfo* newInfoPtr = registryPtr->InsertElementInfo(newName, oldInfo.address, true);

				if (newInfoPtr != NULL){
					I_ASSERT(newInfoPtr->elementPtr.IsValid());	// InsertElementInfo has to return NULL if element cannot be created with option ensureElementCreated = true

					if (iser::CMemoryReadArchive::CloneObjectByArchive(*oldInfo.elementPtr, *newInfoPtr->elementPtr)){
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
								icomp::TSingleAttribute<std::string>* singleAttrPtr = dynamic_cast<icomp::TSingleAttribute<std::string>*>(attributePtr);
								icomp::TMultiAttribute<std::string>* multiAttrPtr = dynamic_cast<icomp::TMultiAttribute<std::string>*>(attributePtr);

								if (		(dynamic_cast<icomp::CReferenceAttribute*>(attributePtr) != NULL) ||
											(dynamic_cast<icomp::CFactoryAttribute*>(attributePtr) != NULL)){
									if (singleAttrPtr->GetValue() == oldName){
										singleAttrPtr->SetValue(newName);
									}
								}

								if (		(dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr) != NULL) ||
											(dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr) != NULL)){
									int valuesCount = multiAttrPtr->GetValuesCount();
									for (int i = 0; i < valuesCount; ++i){
										if (multiAttrPtr->GetValueAt(i) == oldName){
											multiAttrPtr->SetValueAt(i, newName);
										}
									}
								}
							}
						}

						registryPtr->RemoveElementInfo(oldName);

						if (geometryProviderPtr != NULL){
							geometryProviderPtr->SetComponentPosition(newName, position);
						}

						m_selectedComponentPtr->setPos(position.GetX(), position.GetY());
						m_selectedComponentPtr->SetElementInfo(newInfoPtr);

						return;
					}

					registryPtr->RemoveElementInfo(newName);
				}
			}
		}
	}
}


void CRegistryViewComp::OnExportInterface()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		if (m_selectedComponentPtr != NULL){
			const std::string& componentRole = m_selectedComponentPtr->GetComponentName();

			bool doExport = !HasExportedInterfaces(*m_selectedComponentPtr);

			registryPtr->ExportElementInterface(componentRole, doExport);
		}
	}
}


void CRegistryViewComp::OnExportComponent()
{
	// TODO: Implement component export
}


void CRegistryViewComp::OnExportToCode()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryCodeSaverCompPtr.IsValid()){
		QString filter = tr("C++ code file (*.cpp)");
		QString file = QFileDialog::getSaveFileName(NULL, tr("Export registry to code"), "", filter);
		if (!file.isEmpty()){
			if (m_registryCodeSaverCompPtr->SaveToFile(*registryPtr, iqt::GetCString(file)) == iser::IFileLoader::StateFailed){
				QMessageBox::warning(GetQtWidget(), tr("Error"), tr("Cannot export to file\n%1").arg(file));
			}
		}
	}
}


void CRegistryViewComp::OnExecute()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryPreviewCompPtr.IsValid()){
		bool retVal = m_registryPreviewCompPtr->StartRegistry(*registryPtr);
		if (retVal){
			m_abortRegistryCommand.setEnabled(true);
			m_executeRegistryCommand.setEnabled(false);
		}
	}
}


void CRegistryViewComp::OnAbort()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (		(registryPtr != NULL) &&
				m_registryPreviewCompPtr.IsValid()){
		m_registryPreviewCompPtr->AbortRegistry();

		m_abortRegistryCommand.setEnabled(false);
		m_executeRegistryCommand.setEnabled(true);
	}
}


void CRegistryViewComp::OnAddNote()
{
	I_ASSERT(m_selectedComponentPtr != NULL);

	icomp::IRegistryNotesProvider* providerPtr = dynamic_cast<icomp::IRegistryNotesProvider*>(GetObjectPtr());
	if (providerPtr != NULL){
		providerPtr->SetComponentNote(m_selectedComponentPtr->GetComponentName(), istd::CString());
	}
}


void CRegistryViewComp::OnRemoveNote()
{
}


// private members

void CRegistryViewComp::ResetScene()
{
	QList<QGraphicsItem*> items = m_scenePtr->items();
	int itemsCount = items.count();
	foreach(QGraphicsItem* itemPtr, items){
		if (itemPtr == &m_compositeItem){
			continue;
		}

		m_scenePtr->removeItem(itemPtr);
	}
}


void CRegistryViewComp::ScaleView(double scaleFactor)
{
	QGraphicsView* viewPtr = GetQtWidget();
	if (viewPtr != NULL){
		QMatrix sceneMatrix = viewPtr->matrix();
		QMatrix scaleMatrix;
		scaleMatrix.scale(scaleFactor,scaleFactor);

		sceneMatrix *= scaleMatrix;
		if (sceneMatrix.m11() < 0.3 || sceneMatrix.m11() > 5){
			return;
		}

		viewPtr->setMatrix(sceneMatrix);
		if (m_selectedComponentPtr != NULL){
			viewPtr->centerOn(m_selectedComponentPtr);
		}

		viewPtr->viewport()->update();
	}
}


void CRegistryViewComp::CreateConnector(CComponentView& sourceView, const std::string& referenceComponentId)
{
	QList<QGraphicsItem*> items = m_compositeItem.children();
	foreach(QGraphicsItem* item, items){
		CComponentView* referenceViewPtr = dynamic_cast<CComponentView*>(item);
		if ((referenceViewPtr != NULL) && (referenceViewPtr->GetComponentName() == referenceComponentId)){
			CComponentConnector* connector = new CComponentConnector(this, &sourceView, referenceViewPtr, &m_compositeItem, m_scenePtr);

			connector->Adjust();
		}
	}
}


CComponentView* CRegistryViewComp::CreateComponentView(
			const icomp::IRegistry* registryPtr,
			const icomp::IRegistry::ElementInfo* elementInfoPtr,
			const std::string& role)
{
	CComponentView* componentViewPtr = new CComponentView(this, registryPtr, elementInfoPtr, role.c_str(), &m_compositeItem, m_scenePtr);

	connect(componentViewPtr, 
		SIGNAL(selectionChanged(CComponentView*, bool)),
		this,
		SLOT(OnComponentViewSelected(CComponentView*, bool)));

	connect(componentViewPtr, 
		SIGNAL(positionChanged(CComponentView*, const QPoint&)),
		this,
		SLOT(OnComponentPositionChanged(CComponentView*, const QPoint&)),
		Qt::QueuedConnection);

	int itemsCount = m_scenePtr->items().count();

	componentViewPtr->setZValue(itemsCount);

	int gridSize = GetGrid();
	m_compositeItem.setRect(m_compositeItem.childrenBoundingRect().adjusted(-gridSize, -gridSize, gridSize, gridSize));

	return componentViewPtr;
}


void CRegistryViewComp::UpdateConnectors()
{
	QList<QGraphicsItem*> items = m_compositeItem.children();
	foreach(QGraphicsItem* item, items){
		CComponentView* viewPtr = dynamic_cast<CComponentView*>(item);
		if (viewPtr != NULL){
			viewPtr->RemoveAllConnectors();
		}
	}

	bool isDone = false;
	while (!isDone){
		isDone = true;
		QList<QGraphicsItem*> items = m_compositeItem.children();
		foreach(QGraphicsItem* item, items){
			CComponentConnector* connectorPtr = dynamic_cast<CComponentConnector*>(item);
			if (connectorPtr != NULL){
				m_scenePtr->removeItem(connectorPtr);
				isDone = false;
				break;
			}
		}
	}

	items = m_compositeItem.children();

	foreach(QGraphicsItem* item, items){
		CComponentView* componentItemPtr = dynamic_cast<CComponentView*>(item);
		if (componentItemPtr == NULL){
			continue;
		}
		const icomp::IRegistry::ElementInfo& elementInfo = componentItemPtr->GetElementInfo();

		icomp::IRegistryElement::Ids attributeIds = elementInfo.elementPtr->GetAttributeIds();

		for (		icomp::IRegistryElement::Ids::const_iterator iter = attributeIds.begin(); 
					iter != attributeIds.end(); 
					iter++){
			std::string attributeId = *iter;

			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementInfo.elementPtr->GetAttributeInfo(attributeId);
			if (attributeInfoPtr != NULL){
				iser::ISerializable* attributePtr = attributeInfoPtr->attributePtr.GetPtr();
				const icomp::CReferenceAttribute* referenceAttributePtr = dynamic_cast<icomp::CReferenceAttribute*>(attributePtr);
				if (referenceAttributePtr != NULL){		
					const std::string& componentId = referenceAttributePtr->GetValue();
				
					CreateConnector(*componentItemPtr, componentId);
				}
	
				const icomp::CMultiReferenceAttribute* multiReferenceAttributePtr = dynamic_cast<icomp::CMultiReferenceAttribute*>(attributePtr);
				if (multiReferenceAttributePtr != NULL){
					for (int referenceIndex = 0; referenceIndex < multiReferenceAttributePtr->GetValuesCount(); referenceIndex++){
						const std::string& componentId = multiReferenceAttributePtr->GetValueAt(referenceIndex);
						
						CreateConnector(*componentItemPtr, componentId);
					}
				}

				const icomp::CFactoryAttribute* factoryAttributePtr = dynamic_cast<icomp::CFactoryAttribute*>(attributePtr);
				if (factoryAttributePtr != NULL){		
					const std::string& componentId = factoryAttributePtr->GetValue();
				
					CreateConnector(*componentItemPtr, componentId);
				}
	
				const icomp::CMultiFactoryAttribute* multiFactoryAttributePtr = dynamic_cast<icomp::CMultiFactoryAttribute*>(attributePtr);
				if (multiFactoryAttributePtr != NULL){
					for (int referenceIndex = 0; referenceIndex < multiFactoryAttributePtr->GetValuesCount(); referenceIndex++){
						const std::string& componentId = multiFactoryAttributePtr->GetValueAt(referenceIndex);
						
						CreateConnector(*componentItemPtr, componentId);
					}
				}
			}
		}
	}
}


void CRegistryViewComp::OnExecutionTimerTick()
{
	if (m_registryPreviewCompPtr.IsValid()){
		bool isRunning = m_registryPreviewCompPtr->IsRunning();

		m_executeRegistryCommand.setEnabled(!isRunning);
		m_abortRegistryCommand.setEnabled(isRunning);
	}
}


bool CRegistryViewComp::ProcessDroppedData(const QMimeData& data, QGraphicsSceneDragDropEvent* event)
{
	QByteArray byteData = data.data("component");
	iser::CMemoryReadArchive archive(byteData.constData(), byteData.size());

	icomp::CComponentAddress address;

	i2d::CVector2d position(0, 0);
	if (event != NULL){
		position = iqt::GetCVector2d(event->pos());
	}

	return address.Serialize(archive) && TryCreateComponent(address, position);
}


void CRegistryViewComp::ConnectReferences(const QString& componentRole)
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

		icomp::IRegistryElement* registryElementPtr = elementInfoPtr->elementPtr.GetPtr();
		const icomp::IComponentStaticInfo& elementStaticInfo = registryElementPtr->GetComponentStaticInfo();
		const icomp::IComponentStaticInfo::AttributeInfos staticAttributes = elementStaticInfo.GetAttributeInfos();
	
		for (int staticAttributeIndex = 0; staticAttributeIndex < staticAttributes.GetElementsCount(); staticAttributeIndex++){
			const std::string& attributeId = staticAttributes.GetKeyAt(staticAttributeIndex);
			const icomp::IAttributeStaticInfo* staticAttributeInfoPtr = staticAttributes.GetValueAt(staticAttributeIndex);
			I_ASSERT(staticAttributeInfoPtr != NULL);

			const iser::ISerializable* attributePtr = staticAttributeInfoPtr->GetAttributeDefaultValue();
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

			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = registryElementPtr->GetAttributeInfo(attributeId);
			if (attributeInfoPtr == NULL && createAttribute){
				attributeInfoPtr = registryElementPtr->InsertAttributeInfo(attributeId);
			}
		}
	}
}

bool CRegistryViewComp::HasExportedInterfaces(const CComponentView& componentView) const
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registryPtr->GetExportedInterfacesMap();
		for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator iter = interfacesMap.begin();
					iter != interfacesMap.end();
					++iter){
			if (iter->second == componentView.GetComponentName()){
				return true;
			}
		}
	}

	return false;
}


void CRegistryViewComp::UpdateExportInterfaceCommand()
{
	if (m_selectedComponentPtr && HasExportedInterfaces(*m_selectedComponentPtr)){
		m_exportInterfaceCommand.SetVisuals(
				tr("&Remove Inteface Export"), 
				tr("&Remove Inteface Export"), 
				tr("Remove Inteface Export"),
				QIcon(":/Resources/Icons/.png"));
	}
	else{
		m_exportInterfaceCommand.SetVisuals(
				tr("&Export Inteface(s)"), 
				tr("&Export Interface(s)"), 
				tr("Export interface(s)"),
				QIcon(":/Resources/Icons/.png"));
	}
}


// protected methods of embedded class CRegistryViewComp::CCompositeItem

// reimplemented (QGraphicsRectItem)

QVariant CRegistryViewComp::CCompositeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
		switch(change){
		case QGraphicsItem::ItemSelectedChange:
			break;

		case QGraphicsItem::ItemPositionChange:
			QPoint newPos = value.toPoint();
			newPos.rx() = newPos.rx() - (newPos.rx() % 25);
			newPos.ry() = newPos.ry() - (newPos.ry() % 25);

			return QVariant(newPos);
		}

	return QGraphicsRectItem::itemChange(change, value);
}


void CRegistryViewComp::CCompositeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen pen;
	pen.setColor(Qt::darkBlue);

	painter->save();
	if (isSelected()){
		painter->setRenderHint(QPainter::Antialiasing);
		pen.setColor(Qt::red);
	}
	painter->setPen(pen);
	painter->drawRect(rect());

	painter->restore();
}


// public methods of embedded class CRegistryScene

CRegistryViewComp::CRegistryScene::CRegistryScene(CRegistryViewComp& parent)
	:m_parent(parent)
{
}


// protected methods of embedded class CRegistryScene

// reimplemented (QGraphicsScene)

void CRegistryViewComp::CRegistryScene::keyPressEvent(QKeyEvent* keyEvent)
{
	switch(keyEvent->key()){
	case Qt::Key_Plus:
		m_parent.ScaleView(pow((double)2, 0.5)); 
		break;
	case Qt::Key_Minus:
		m_parent.ScaleView(pow((double)2, -0.5));
		break;
	}
}


void CRegistryViewComp::CRegistryScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    m_parent.ScaleView(pow((double)2, event->delta() / 240.0));
}


void CRegistryViewComp::CRegistryScene::drawBackground(QPainter* painter, const QRectF & rect)
{
	QRectF sceneRect =  this->sceneRect().unite(rect);

	painter->save();
	painter->fillRect(sceneRect, QBrush(Qt::white));

	const double gridSize = m_parent.GetGrid();
	QPen pen(Qt::gray);
	pen.setStyle(Qt::DotLine);

	painter->fillRect(this->sceneRect(), QBrush(Qt::white));
	painter->setPen(pen);

	for (double x = sceneRect.left(); x < sceneRect.right(); x += gridSize){
		painter->drawLine(x,sceneRect.top(), x, sceneRect.bottom());
	}

	for (double y = sceneRect.top(); y < sceneRect.bottom(); y += gridSize){
		painter->drawLine(sceneRect.left(),y, sceneRect.right(), y);
	}

	painter->restore();
}


void CRegistryViewComp::CRegistryScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
	QStringList& formats = event->mimeData()->formats();
	if (!formats.isEmpty() && (formats.first() == "component")){
		event->acceptProposedAction();
	}
}


void CRegistryViewComp::CRegistryScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	const QMimeData* dataPtr = event->mimeData();
	if ((dataPtr != NULL) && m_parent.ProcessDroppedData(*dataPtr, event)){
		event->acceptProposedAction();
	}
	else{
		event->ignore();
	}
}


void CRegistryViewComp::CRegistryScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
}

