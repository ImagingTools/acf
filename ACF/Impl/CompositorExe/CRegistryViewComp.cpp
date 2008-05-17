#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QScrollBar>

#include <math.h>

#include "IRegistryGeometryProvider.h"
#include "CRegistryViewComp.h"
#include "CComponentView.h"
#include "CComponentConnector.h"

#include "iser/IArchive.h" 


CRegistryViewComp::CRegistryViewComp()
	:m_selectedComponentPtr(NULL)
{
	m_scenePtr = new CRegistryScene(*this);

	m_scenePtr->setFocus();
}


void CRegistryViewComp::OnAddComponent(const CStaticComponentInfo& componentInfo)
{
	bool isOk = false;
	QString role = QInputDialog::getText(GetWidget(), tr("Application Compositor"), tr("Component role:"), QLineEdit::Normal, "",&isOk);
	if (isOk && !role.isEmpty()){
		OnAddComponent(componentInfo, role);
	}
}


void CRegistryViewComp::OnAddComponent(const CStaticComponentInfo& componentInfo, const QString& componentRole)
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->InsertElementInfo(componentRole.toStdString(), componentInfo.m_elementType, componentInfo.m_packageId, componentInfo.m_componentId);
		if (elementInfoPtr == NULL){
			QMessageBox::critical(GetWidget(), tr("Error"), tr("Component could not be added")); 
		}
	}
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

	// reset scene:
	QList<QGraphicsItem*> items = m_compositeItem.children();
	int itemsCount = items.count();
	foreach(QGraphicsItem* itemPtr, items){
		m_scenePtr->removeItem(itemPtr);

		delete itemPtr;
	}

	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		icomp::IRegistry::Ids elementIds = registryPtr->GetElementIds();
		for (		icomp::IRegistry::Ids::iterator index = elementIds.begin();
					index != elementIds.end();
					index++){
			const std::string& elementId = *index;
			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementId);
			if (elementInfoPtr != NULL){
				CComponentView* viewPtr = CreateComponentView(*elementInfoPtr, iqt::GetQString(elementId));

				IRegistryGeometryProvider* geomeometryProviderPtr = dynamic_cast<IRegistryGeometryProvider*>(registryPtr);
				if (geomeometryProviderPtr != NULL){			
					viewPtr->setPos(geomeometryProviderPtr->GetComponentPosition(elementId));
				}
			}
		}
	}

	UpdateConnections();
}


void CRegistryViewComp::UpdateModel() const
{
}


// reimplemented (iqt::CGuiComponentBase)

void CRegistryViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	I_ASSERT(m_scenePtr != NULL);

	iqt::CHierarchicalCommand* registryMenuPtr = new iqt::CHierarchicalCommand("&Registry");
	iqt::CHierarchicalCommand* addComponentCommandPtr = new iqt::CHierarchicalCommand("&Add Component");
	connect(addComponentCommandPtr, SIGNAL( activated()), this, SLOT(OnAddComponent()));
	registryMenuPtr->InsertChild(addComponentCommandPtr, true);
	iqt::CHierarchicalCommand* removeComponentCommandPtr = new iqt::CHierarchicalCommand("&Remove Component");
	connect(removeComponentCommandPtr, SIGNAL( activated()), this, SLOT(OnRemoveComponent()));
	registryMenuPtr->InsertChild(removeComponentCommandPtr, true);

	m_registryCommand.InsertChild(registryMenuPtr, true);

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
		const CComponentView::ComponentData* elementInfoPtr = m_selectedComponentPtr->GetComponent();
		if (elementInfoPtr != NULL){
			imod::IModel* registryElementModelPtr = dynamic_cast<imod::IModel*>(elementInfoPtr->elementPtr.GetPtr());
			if (registryElementModelPtr != NULL){
				registryElementModelPtr->DetachAllObservers();
			}
		}
	}

	if (isSelected){
		m_selectedComponentPtr = viewPtr;
		if (viewPtr != NULL && m_registryElementObserversCompPtr.IsValid()){
			const CComponentView::ComponentData* elementInfoPtr = m_selectedComponentPtr->GetComponent();
			if (elementInfoPtr != NULL){
				for (int observerIndex = 0; observerIndex < m_registryElementObserversCompPtr.GetCount(); observerIndex++){
					imod::IObserver* observerPtr = m_registryElementObserversCompPtr[observerIndex];
					if (observerPtr != NULL){
						imod::IModel* registryElementModelPtr = dynamic_cast<imod::IModel*>(elementInfoPtr->elementPtr.GetPtr());
						if (registryElementModelPtr != NULL){
							registryElementModelPtr->AttachObserver(observerPtr);
						}
					}
				}
			}
		}
	}
	else{
		m_selectedComponentPtr = NULL;
	}
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
}


void CRegistryViewComp::OnRemoveComponent()
{
	icomp::IRegistry* registryPtr = GetObjectPtr();
	if (registryPtr != NULL){
		if (m_selectedComponentPtr != NULL){
			m_selectedComponentPtr->RemoveAllConnectors();
			m_scenePtr->removeItem(m_selectedComponentPtr);
			m_selectedComponentPtr->setParentItem(NULL);
			if (m_selectedComponentPtr->GetComponent() != NULL){
				registryPtr->RemoveElementInfo(m_selectedComponentPtr->GetComponentName().toStdString());
			}
		}
	}
}


// private members

void CRegistryViewComp::ScaleView(double scaleFactor)
{
	QGraphicsView* viewPtr = GetQtWidget();
	if (viewPtr != NULL){
		QMatrix sceneMatrix = viewPtr->matrix();
		QMatrix scaleMatrix;
		scaleMatrix.scale(scaleFactor,scaleFactor);

		sceneMatrix *= scaleMatrix;
		if (sceneMatrix.m11() < 0.1 || sceneMatrix.m11()> 5){
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
			CComponentConnector* connector = new CComponentConnector(NULL, NULL);
			connector->setParentItem(&m_compositeItem);
			connector->SetSourceComponent(&sourceView);
			connector->SetDestinationComponent(referenceViewPtr);

			m_scenePtr->addItem(connector);
			connector->Adjust();
		}
	}
}


CComponentView* CRegistryViewComp::CreateComponentView(const icomp::IRegistry::ElementInfo& componentRef, const QString& role)
{
	CComponentView* componentViewPtr = new CComponentView(componentRef, role, &m_compositeItem, m_scenePtr);

	componentViewPtr->setParentItem(&m_compositeItem);

	connect(componentViewPtr, 
		SIGNAL(selectionChanged(CComponentView*, bool)),
		this,
		SLOT(OnComponentViewSelected(CComponentView*, bool)));

	connect(componentViewPtr, 
		SIGNAL(exportChanged(CComponentView*, bool)),
		this,
		SLOT(OnExportChanged(CComponentView*, bool)));

	connect(componentViewPtr, 
		SIGNAL(positionChanged(CComponentView*, const QPoint&)),
		this,
		SLOT(OnComponentPositionChanged(CComponentView*, const QPoint&)));

	int itemsCount = m_scenePtr->items().count();

	componentViewPtr->setZValue(itemsCount);

	m_compositeItem.setRect(m_compositeItem.childrenBoundingRect().adjusted(-25,-25,25,25));

	return componentViewPtr;
}


void CRegistryViewComp::UpdateConnections()
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
				connectorPtr->setParentItem(NULL);
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
		const CComponentView::ComponentData* componentDataPtr = componentItemPtr->GetComponent();
		if (componentDataPtr == NULL){
			continue;
		}

		icomp::IRegistryElement::Ids attributeIds = componentDataPtr->elementPtr->GetAttributeIds();

		for (		icomp::IRegistryElement::Ids::const_iterator index = attributeIds.begin(); 
					index != attributeIds.end(); 
					index++){
			std::string attributeId = *index;

			const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = componentDataPtr->elementPtr->GetAttributeInfo(attributeId);
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


void CRegistryViewComp::OnWheelEvent(QWheelEvent *event)
{
    ScaleView(pow((double)2, -event->delta() / 240.0));
}


void CRegistryViewComp::OnDragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->formats().at(0) == "component"){
		event->acceptProposedAction();
		return;
	}
}


void CRegistryViewComp::OnDropEvent(QDropEvent *event)
{
	const QMimeData* data = event->mimeData();
	QByteArray dataBytes = data->data("component");
	int address = dataBytes.toInt();
	
	bool retVal = false;
	QString componentRole = QInputDialog::getText(GetWidget(), tr("ACF Compositor"), tr("Component role:"), QLineEdit::Normal, "",&retVal);
	if (retVal && !componentRole.isEmpty()){
		const CStaticComponentInfo* componentPtr = reinterpret_cast<const CStaticComponentInfo*>(address);

		if (componentPtr != NULL){ 
			OnAddComponent(*componentPtr, componentRole);
		}
			
		event->acceptProposedAction();
	}
	else{
		event->ignore();
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

void CRegistryViewComp::CRegistryScene::drawBackground(QPainter* painter, const QRectF & rect)
{
	QRectF sceneRect =  this->sceneRect().unite(rect);

	painter->save();
	painter->fillRect(sceneRect, QBrush(Qt::white));

	const double gridSize = 25;
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
	const QMimeData* data = event->mimeData();
	QByteArray byteData = data->data("component");
	int address = byteData.toInt();
	
	bool isOk;
	QString role = QInputDialog::getText(NULL, tr("ACF Compositor"), tr("Component role:"), QLineEdit::Normal, "", &isOk);
	if (isOk && !role.isEmpty()){
		const CStaticComponentInfo* componentPtr = (const CStaticComponentInfo*)address;

		if (componentPtr != NULL){ 
			m_parent.OnAddComponent(*componentPtr, role);
		}
			
		event->acceptProposedAction();
	}
	else{
		event->ignore();
	}
}


void CRegistryViewComp::CRegistryScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
}


