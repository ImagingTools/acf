#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QLabel>
#include <QFont>
#include <QMenu> 
#include <QDrag>
#include <QMessageBox>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QApplication>


#include "CComponentView.h"
#include "CComponentConnector.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iqt/iqt.h"


CComponentView::CComponentView(
			const icomp::IRegistry* registryPtr,
			const icomp::IRegistry::ElementInfo* elementInfoPtr, 
			const QString& componentName,
			QGraphicsItem* parent, 
			QGraphicsScene* scene) 
:	QGraphicsRectItem(parent, scene),
	m_registry(*registryPtr),
	m_elementInfo(*elementInfoPtr),
	m_componentName(componentName),
	m_componentLabelFontSize(14),
	m_componentIdFontSize(10),
	m_gridSize(25)

{
	I_ASSERT(registryPtr != NULL);
	I_ASSERT(elementInfoPtr != NULL);

	m_contextMenu = new QMenu();
	m_contextMenu->setTitle(tr("Component settings"));

	QAction* exportAction = new QAction(tr("&Export"), m_contextMenu);
	exportAction->setCheckable(true);
	connect(exportAction, SIGNAL( toggled(bool)), this, SLOT(OnExportChanged(bool)));

	QAction* renameAction = new QAction(tr("&Rename"), m_contextMenu);
	connect(renameAction, SIGNAL( activated()), this, SLOT(OnRename()));

//	m_contextMenu->actions().at(0)->setChecked(exported);

	CalcExportedInteraces();

	setRect(CalculateRect());

	m_contextMenu->addAction(renameAction);
	m_contextMenu->addAction(exportAction);

	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

	update();
}


CComponentView::~CComponentView()
{
	delete m_contextMenu;

	foreach(CComponentConnector* connector, m_connectors){
		connector->SetSourceComponent(NULL);
		connector->SetDestinationComponent(NULL);
	}

}


const icomp::IRegistry::ElementInfo& CComponentView::GetElementInfo() const
{
	return m_elementInfo;
}


QString CComponentView::GetComponentName() const
{
	return m_componentName;
}


void CComponentView::AddConnector(CComponentConnector* connector)
{
	if (connector != NULL){
		m_connectors.push_back(connector);

		connector->Adjust();
	}

}


void CComponentView::RemoveConnector(const CComponentConnector* connector)
{
	QList<CComponentConnector*>::iterator found = qFind(m_connectors.begin(), m_connectors.end(), connector);
	if (found != m_connectors.end()){
		CComponentConnector* connector = *found;
		CComponentView* view1 =  connector->GetSourceComponent();
		CComponentView* view2 = connector->GetDestinationComponent();
		if (view1 == this){
			connector->SetSourceComponent(NULL);
		}
		if (view2 == this){
			connector->SetDestinationComponent(NULL);
		}
		m_connectors.erase(found);
	}
}


void CComponentView::RemoveAllConnectors()
{
	foreach(CComponentConnector* connector, m_connectors){
		CComponentView* view1 =  connector->GetSourceComponent();
		CComponentView* view2 = connector->GetDestinationComponent();
		if (view1 == this){
			connector->SetSourceComponent(NULL);
		}
		if (view2 == this){
			connector->SetDestinationComponent(NULL);
		}
	}

	m_connectors.clear();
}


QRectF CComponentView::GetInnerRect()const
{
	QRectF mainRect = rect();
	mainRect.adjust(0,0,-10,-10);

	return mainRect;
}


// protected slots:

void CComponentView::OnExportChanged(bool state)
{
	setRect(CalculateRect());

	emit exportChanged(this, state);
}


void CComponentView::OnRename()
{
}


// protected members

QRect CComponentView::CalculateRect() const
{
	QFont labelFont("Verdana");
	labelFont.setBold(true);
	labelFont.setPixelSize(m_componentLabelFontSize);
	QFontMetrics fontInfo(labelFont);

	int width = fontInfo.width(m_componentName) + 20;
	int height = fontInfo.height() + 10;
	m_componentLabelHeight = height;

	QFont labelFont2("Verdana");
	labelFont.setPixelSize(m_componentIdFontSize);
	QFontMetrics fontInfo2(labelFont2);

	m_componentIdHeight = fontInfo2.height() + 10;

	height += m_componentIdHeight;
	QString componentPath = QString(m_elementInfo.packageId.c_str()) + QString(".") + m_elementInfo.componentId.c_str();

	width = qMax(width, fontInfo2.width(componentPath) + 20);

	width += 2 * m_gridSize * m_exportedInterfacesList.size();

	width += (m_gridSize - (width % m_gridSize));
	height += (m_gridSize - (height % m_gridSize));

	return QRect(rect().left(), rect().top(), width, height);
}


void CComponentView::CalcExportedInteraces()
{
	using icomp::IRegistry::ExportedInterfacesMap::const_iterator;

	std::string componentId = m_componentName.toStdString();

	m_exportedInterfacesList.clear();

	const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = m_registry.GetExportedInterfacesMap();
	for (		const_iterator iter = interfacesMap.begin();
				iter != interfacesMap.end();
				++iter){
		if (iter->second == componentId){
			m_exportedInterfacesList.push_back(iter->first.c_str());
		}
	}
}


// reimplemented (QGraphicsRectItem)

void CComponentView::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	QAction* a = m_contextMenu->exec(event->screenPos());

	QGraphicsRectItem::contextMenuEvent(event);
}


void CComponentView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QRectF mainRect = rect();
	mainRect.adjust(0,0,-10,-10);
	QRectF shadowRect = mainRect;
	shadowRect.adjust(10,10,10,10);
	
	if (isSelected()){
		painter->fillRect(shadowRect, QColor(10, 242, 126, 50));
		painter->setPen(Qt::blue);
		painter->fillRect(mainRect, QColor(10, 242, 126, 255));
	}
	else{
		painter->fillRect(shadowRect, QColor(0, 0, 0, 30));
		painter->fillRect(mainRect, Qt::white);
		painter->setPen(Qt::black);
	}

	painter->drawRect(mainRect);

	if (!m_exportedInterfacesList.empty()){
		painter->save();
		QRectF exportRect = mainRect.adjusted(mainRect.width()-45,20, -20, 45-mainRect.height());
		painter->fillRect(exportRect, QBrush(Qt::magenta));
		
		QPen pen(Qt::magenta,2);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);
		painter->setPen(pen);
		
		painter->setRenderHints(QPainter::Antialiasing);
		painter->drawLine(exportRect.right(), exportRect.top()+6, exportRect.right()+15, exportRect.top()+6);
		painter->drawLine(exportRect.right()+15, exportRect.top()+6, exportRect.right()+10, exportRect.top()+3);
		painter->drawLine(exportRect.right()+15, exportRect.top()+6, exportRect.right()+10, exportRect.top()+9);

		painter->drawLine(exportRect.right(), exportRect.top()+18, exportRect.right()+15, exportRect.top()+18);
		painter->drawLine(exportRect.right()+15, exportRect.top()+18, exportRect.right()+10, exportRect.top()+21);
		painter->drawLine(exportRect.right()+15, exportRect.top()+18, exportRect.right()+10, exportRect.top()+15);

		painter->restore();
	}

	mainRect.adjust(10, 10, 0, -5);
	mainRect.setHeight(m_componentLabelHeight);
	painter->save();
	painter->setPen(Qt::black);

	QFont labelFont("Verdana");
	labelFont.setBold(true);
	labelFont.setPixelSize(m_componentLabelFontSize);
	painter->setFont(labelFont);
	painter->drawText(mainRect,Qt::AlignLeft | Qt::TextSingleLine, m_componentName);

	mainRect.moveTop(mainRect.height() + mainRect.top());
	mainRect.setHeight(m_componentIdHeight);
	QFont labelFont2("Verdana");
	labelFont2.setPixelSize(m_componentIdFontSize);
	painter->setFont(labelFont2);

	painter->drawText(	mainRect, 
						Qt::AlignLeft | Qt::TextSingleLine, 
						QString(m_elementInfo.packageId.c_str()) + QString(".") + m_elementInfo.componentId.c_str());
	
	painter->restore();
}


QVariant CComponentView::itemChange(GraphicsItemChange change, const QVariant& value)
{
	switch(change){
		case QGraphicsItem::ItemSelectedChange:
			foreach(CComponentConnector* connector, m_connectors){
				if (value.toBool()){
					connector->setZValue(0);
				}
				else{
					connector->setZValue(0);
				}

				connector->setSelected(value.toBool());
			}
			emit selectionChanged(this, value.toBool());
			break;

		case QGraphicsItem::ItemPositionChange:
			QSizeF size = CalculateRect().size();
			QPoint newPos = value.toPoint();
			newPos.setX(::floor((newPos.x() + size.width()) / m_gridSize) * m_gridSize - size.width());
			newPos.setY(::floor((newPos.y() + size.height()) / m_gridSize) * m_gridSize - size.height());

			foreach (CComponentConnector* connector, m_connectors){
				connector->Adjust();
			}

			QGraphicsRectItem* parentItem = dynamic_cast<QGraphicsRectItem*>(this->parentItem());
			if (parentItem != NULL){
				parentItem->setRect(parentItem->childrenBoundingRect().adjusted(-m_gridSize, -m_gridSize, m_gridSize, m_gridSize));
			}
			
			emit positionChanged(this, newPos);

			return QVariant(newPos);

	}

	return QGraphicsRectItem::itemChange(change, value);
}


