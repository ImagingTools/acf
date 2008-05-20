#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

#include <math.h>

#include "IRegistryGeometryProvider.h"
#include "CRegistryViewComp.h"
#include "CComponentView.h"
#include "CComponentConnector.h"

#include "iser/IArchive.h" 
#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"


CRegistryViewComp::CRegistryViewComp()
:	m_selectedComponentPtr(NULL),
	m_removeComponentCommand("&Remove Component", 100, idoc::IHierarchicalCommand::CF_GLOBAL_MENU | idoc::IHierarchicalCommand::CF_TOOLBAR),
	m_executeRegistryCommand("&Execute Registry", 100, idoc::IHierarchicalCommand::CF_GLOBAL_MENU | idoc::IHierarchicalCommand::CF_TOOLBAR),
	m_abortRegistryCommand("&Abort Registry", 100, idoc::IHierarchicalCommand::CF_GLOBAL_MENU | idoc::IHierarchicalCommand::CF_TOOLBAR)
{
	m_removeComponentCommand.setEnabled(false);
	m_renameComponentCommand.setEnabled(false);
	m_exportToCodeCommand.setEnabled(false);
	m_executeRegistryCommand.setEnabled(false);
	m_abortRegistryCommand.setEnabled(false);

	m_registryMenu.InsertChild(&m_removeComponentCommand);
	m_registryMenu.InsertChild(&m_renameComponentCommand);
	m_registryMenu.InsertChild(&m_exportToCodeCommand);
	m_registryMenu.InsertChild(&m_executeRegistryCommand);
	m_registryMenu.InsertChild(&m_abortRegistryCommand);
	m_registryCommand.InsertChild(&m_registryMenu);

	m_scenePtr = new CRegistryScene(*this);

	m_scenePtr->setFocus();
}


bool CRegistryViewComp::TryCreateComponent(const icomp::CComponentAddress& address)
{
	bool retVal = false;
	QString componentName = QInputDialog::getText(GetWidget(), tr("Application Compositor"), tr("Component name"), QLineEdit::Normal, "",&retVal);
	if (retVal && !componentName.isEmpty()){
		icomp::IRegistry* registryPtr = GetObjectPtr();
		if (registryPtr != NULL){
			icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->InsertElementInfo(componentName.toStdString(), address);
			if (elementInfoPtr != NULL){
				return true;
			}
		}

		QMessageBox::critical(GetWidget(), tr("Error"), tr("Component could not be added")); 
	}

	return false;
}


QStringList CRegistryViewComp::GetComponentsForDependency(const QString& dependecySource) const
{
	istd::CStringList components;// = m_composite.componentsForDependency(dependecySource.toStdWString());

	return iqt::GetQStringList(components);
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
		for (		icomp::IRegistry::Ids::iterator index = elementIds.begin();
					index != elementIds.end();
					index++){
			const std::string& elementId = *index;
			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementId);
			if (elementInfoPtr != NULL){
				CComponentView* viewPtr = CreateComponentView(registryPtr, elementInfoPtr, elementId);

				IRegistryGeometryProvider* geomeometryProviderPtr = dynamic_cast<IRegistryGeometryProvider*>(registryPtr);
				if (geomeometryProviderPtr != NULL){			
					viewPtr->setPos(geomeometryProviderPtr->GetComponentPosition(elementId));
				}
			}
		}
	}

	UpdateConnectors();
}


void CRegistryViewComp::UpdateModel() const
{
}


// reimplemented (iqt::CGuiComponentBase)

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
				tr("&Remove Component"), 
				tr("Remove the selected component from the registry"),
				QIcon(":/Resources/Icons/delete_64.png"));
	m_exportToCodeCommand.SetVisuals(
				tr("&Export To Code..."),
				tr("Export"),
				tr("Removes selected component from registry"));
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
}


// public slots:

void CRegistryViewComp::SetCenterOn(const QString& componentRole)
{
	QGraphicsView* viewPtr = GetQtWidget();
	if (viewPtr != NULL){
		QList<QGraphicsItem*> items = m_scenePtr->items();
		foreach(QGraphicsItem* item, items){
			CComponentView* itemViewPtr = dynamic_cast<CComponentView*>(item);
			if (itemViewPtr != NULL){
				if (componentRole == itemViewPtr->GetComponentName()){
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
	m_exportToCodeCommand.setEnabled(isSelected);
	m_executeRegistryCommand.setEnabled(isSelected && m_registryPreviewCompPtr.IsValid());
}


void CRegistryViewComp::OnExportChanged(CComponentView* viewPtr, bool export)
{
//	m_composite.SetExported(iqt::GetCString(view->GetComponentName()), export);
	viewPtr->update();
}


void CRegistryViewComp::OnComponentPositionChanged(CComponentView* view, const QPoint& newPosition)
{
	IRegistryGeometryProvider* geometryProviderPtr = dynamic_cast<IRegistryGeometryProvider*>(GetObjectPtr());
	if (geometryProviderPtr != NULL){
		geometryProviderPtr->SetComponentPosition(iqt::GetCString(view->GetComponentName()), newPosition);
	}

	int gridSize = GetGrid();
	m_compositeItem.setRect(m_compositeItem.childrenBoundingRect().adjusted(-gridSize, -gridSize, gridSize, gridSize));
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

			registryPtr->RemoveElementInfo(m_selectedComponentPtr->GetComponentName().toStdString());

			m_selectedComponentPtr = NULL;
		}
	}
}


void CRegistryViewComp::OnRenameComponent()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		if (m_selectedComponentPtr != NULL){
			QString oldName = m_selectedComponentPtr->GetComponentName();

			bool isOk = false;
			QString newName = QInputDialog::getText(NULL, tr("ACF Compositor"), tr("New component name"), QLineEdit::Normal, oldName, &isOk);
			if (isOk && !newName.isEmpty()){
				const icomp::IRegistry::ElementInfo& elementInfo = m_selectedComponentPtr->GetElementInfo();

				// TODO: implement rename
			}
		}
	}
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
		if (referenceViewPtr != NULL && referenceViewPtr->GetComponentName() == referenceComponentId.c_str()){
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

		for (		icomp::IRegistryElement::Ids::const_iterator index = attributeIds.begin(); 
					index != attributeIds.end(); 
					index++){
			std::string attributeId = *index;

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


bool CRegistryViewComp::ProcessDroppedData(const QMimeData& data)
{
	QByteArray byteData = data.data("component");
	iser::CMemoryReadArchive archive(byteData.constData(), byteData.size());

	icomp::CComponentAddress address;

	return address.Serialize(archive) && TryCreateComponent(address);
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
	case Qt::Key_Delete:
		m_parent.OnRemoveComponent();  
		break;
	}
}


void CRegistryViewComp::CRegistryScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    m_parent.ScaleView(pow((double)2, -event->delta() / 240.0));
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
	if ((dataPtr != NULL) && m_parent.ProcessDroppedData(*dataPtr)){
		event->acceptProposedAction();
	}
	else{
		event->ignore();
	}
}


void CRegistryViewComp::CRegistryScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
}

