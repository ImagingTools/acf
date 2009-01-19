#include "icmpstr/CRegistryView.h"


// STL includes
#include <cmath>

// Qt includes
#include <QVarLengthArray>

#include "icmpstr/CComponentView.h"
#include "icmpstr/CComponentConnector.h"


namespace icmpstr
{


CRegistryView::CRegistryView(QWidget* parent/* = NULL*/)
	:BaseClass(parent),
	m_selectedComponentPtr(NULL)
{
	m_scenePtr = new CRegistryScene(*this);

	m_scenePtr->setFocus();

	setDragMode(QGraphicsView::ScrollHandDrag);
	setScene(m_scenePtr);
	setFrameShape(QFrame::NoFrame);
	setAcceptDrops(true); 
	
#if QT_VERSION >= 0x040400
	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
#endif
	m_compositeItem.setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	
	m_scenePtr->addItem(&m_compositeItem);
}


void CRegistryView::CreateConnector(CComponentView& sourceView, const std::string& referenceComponentId)
{
	QList<QGraphicsItem*> items = m_compositeItem.children();
	foreach(QGraphicsItem* item, items){
		CComponentView* referenceViewPtr = dynamic_cast<CComponentView*>(item);
		if ((referenceViewPtr != NULL) && (referenceViewPtr->GetComponentName() == referenceComponentId)){
			CComponentConnector* connector = new CComponentConnector(this, 
						&sourceView, 
						referenceViewPtr, 
						&m_compositeItem);

			connector->Adjust();
		}
	}
}


CComponentView* CRegistryView::CreateComponentView(
			const icomp::IRegistry* registryPtr,
			const icomp::IRegistry::ElementInfo* elementInfoPtr,
			const std::string& role)
{
	CComponentView* componentViewPtr = new CComponentView(this, 
				registryPtr, 
				elementInfoPtr, 
				role.c_str(), 
				&m_compositeItem);

	int itemsCount = m_scenePtr->items().count();

	componentViewPtr->setZValue(itemsCount);

	UpdateCompositeGeometry();

	return componentViewPtr;
}


const CComponentView* CRegistryView::GetSelectedComponent() const
{
	return m_selectedComponentPtr; 
}


void CRegistryView::SetSelectedComponent(CComponentView* selectedComponentPtr)
{
	m_selectedComponentPtr = selectedComponentPtr; 
}


void CRegistryView::RemoveSelectedComponent()
{
	if (m_selectedComponentPtr != NULL){
		m_selectedComponentPtr->RemoveAllConnectors();
		m_scenePtr->removeItem(m_selectedComponentPtr);
			
		m_selectedComponentPtr = NULL;
	}
}


void CRegistryView::RemoveAllConnectors()
{
	QList<QGraphicsItem*> items = m_compositeItem.children();
	foreach(QGraphicsItem* item, items){
		CComponentView* componentViewPtr = dynamic_cast<CComponentView*>(item);
		if (componentViewPtr != NULL){
			componentViewPtr->RemoveAllConnectors();
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
}


CRegistryView::ComponentViewList CRegistryView::GetComponentViews() const
{
	ComponentViewList componentList;

	QList<QGraphicsItem*> items = m_compositeItem.children();
	foreach(QGraphicsItem* item, items){
		CComponentView* componentViewPtr = dynamic_cast<CComponentView*>(item);
		if (componentViewPtr != NULL){
			componentList.append(componentViewPtr);
		}
	}

	return componentList;
}


void CRegistryView::SetCenterOn(const QString& componentName)
{
	QList<QGraphicsItem*> items = m_scenePtr->items();
	foreach(QGraphicsItem* item, items){
		CComponentView* itemViewPtr = dynamic_cast<CComponentView*>(item);
		if (itemViewPtr != NULL){
			if (componentName == itemViewPtr->GetComponentName().c_str()){
				centerOn(itemViewPtr);

				return;
			}
		}
	}
}


void CRegistryView::OnDropDataEvent(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr)
{
	emit DropDataEventEntered(data, eventPtr);
}


void CRegistryView::UpdateCompositeGeometry()
{
	double gridSize = GetGrid();
	QRectF boundingBox = m_compositeItem.childrenBoundingRect();
	int width = int(::ceil(boundingBox.width() / gridSize) * gridSize);
	int height = int(::ceil(boundingBox.height() / gridSize) * gridSize);
	boundingBox.setWidth(width);
	boundingBox.setHeight(height);

	m_compositeItem.setRect(boundingBox.adjusted(-gridSize, -gridSize, gridSize, gridSize));
}


void CRegistryView::ResetScene()
{
	QList<QGraphicsItem*> items = m_scenePtr->items();
	foreach(QGraphicsItem* itemPtr, items){
		if (itemPtr == &m_compositeItem){
			continue;
		}

		m_scenePtr->removeItem(itemPtr);
	}
}


void CRegistryView::ScaleView(double scaleFactor)
{
	QMatrix sceneMatrix = matrix();
	QMatrix scaleMatrix;
	scaleMatrix.scale(scaleFactor,scaleFactor);

	sceneMatrix *= scaleMatrix;
	if (sceneMatrix.m11() < 0.3 || sceneMatrix.m11() > 5){
		return;
	}

	setMatrix(sceneMatrix);
	if (m_selectedComponentPtr != NULL){
		centerOn(m_selectedComponentPtr);
	}
	
	viewport()->update();
}


// protected methods

// reimplemented (QWidget)

void CRegistryView::resizeEvent(QResizeEvent* eventPtr)
{
	BaseClass::resizeEvent(eventPtr);
/**
	fitInView(&m_compositeItem, Qt::KeepAspectRatio);
*/
}


// protected methods of embedded class CRegistryView::CCompositeItem

// reimplemented (QGraphicsRectItem)

QVariant CRegistryView::CCompositeItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
//	int gridSize = CRegistryView::GetGrid();

	switch(change){
	case QGraphicsItem::ItemSelectedChange:
		break;

	case QGraphicsItem::ItemPositionChange:
		{
			QPoint newPos = value.toPoint();
/*			newPos.rx() = newPos.rx() - (newPos.rx() % gridSize);
			newPos.ry() = newPos.ry() - (newPos.ry() % gridSize);
*/
			return QVariant(newPos);
		}

	default:
		break;
	}

	return QGraphicsRectItem::itemChange(change, value);
}


void CRegistryView::CCompositeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	QPen pen;
	pen.setColor(Qt::darkBlue);

	painter->save();
	if (isSelected()){
		painter->setRenderHint(QPainter::Antialiasing);
		pen.setColor(QColor(10, 242, 126, 255));
	}
	painter->setPen(pen);
	painter->drawRect(rect());

	painter->restore();
}

 QRectF CRegistryView::CCompositeItem::boundingRect() const      
 {         
	 return QGraphicsRectItem::boundingRect().adjusted(-1,-1,1,1);      
 }


// public methods of embedded class CRegistryScene

CRegistryView::CRegistryScene::CRegistryScene(CRegistryView& parent)
	:m_parent(parent)
{
}


// protected methods of embedded class CRegistryScene

// reimplemented (QGraphicsScene)

void CRegistryView::CRegistryScene::keyPressEvent(QKeyEvent* keyEvent)
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


void CRegistryView::CRegistryScene::wheelEvent(QGraphicsSceneWheelEvent* eventPtr)
{
    m_parent.ScaleView(pow((double)2, eventPtr->delta() / 240.0));
}


void CRegistryView::CRegistryScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	QRectF gridRect = sceneRect().unite(rect);
	const int gridSize = CRegistryView::GetGrid();          
	const int realLeft = std::floor(gridRect.left());         
	const int realRight = std::ceil(gridRect.right());         
	const int realTop = std::floor(gridRect.top());         
	const int realBottom = std::ceil(gridRect.bottom());           
	const int firstLeftGridLine = realLeft - (realLeft % gridSize);
	const int firstTopGridLine = realTop - (realTop % gridSize);

	QVarLengthArray<QLine> lines;
	for (int x = firstLeftGridLine; x <= realRight; x += gridSize){
		lines.append(QLine(x, realTop, x, realBottom));
	}

	for (int y = firstTopGridLine; y <= realBottom; y += gridSize){
		lines.append(QLine(realLeft, y, realRight, y));           
	}

	painter->setPen(QPen(Qt::blue, 0, Qt::DotLine));
	painter->drawLines(lines.data(), lines.size());
}


void CRegistryView::CRegistryScene::dragEnterEvent(QGraphicsSceneDragDropEvent* eventPtr)
{
	const QStringList& formats = eventPtr->mimeData()->formats();
	if (!formats.isEmpty() && (formats.first() == "component")){
		eventPtr->acceptProposedAction();
	}
}


void CRegistryView::CRegistryScene::dropEvent(QGraphicsSceneDragDropEvent* eventPtr)
{
	const QMimeData* dataPtr = eventPtr->mimeData();
	if ((dataPtr != NULL)){
		m_parent.OnDropDataEvent(*dataPtr, eventPtr);

		eventPtr->acceptProposedAction();
	}
	else{
		eventPtr->ignore();
	}
}


void CRegistryView::CRegistryScene::dragMoveEvent(QGraphicsSceneDragDropEvent* /*eventPtr*/)
{
}


} // namespace icmpstr


