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


CComponentView::CComponentView(const ComponentData& componentRef, 
							   const QString& componentName,
							   QGraphicsItem* parent, 
							   QGraphicsScene* scene) 
	:QGraphicsRectItem(parent, scene),
	m_componentRef(componentRef),
	m_componentName(componentName),
	m_componentLabelFontSize(14),
	m_componentIdFontSize(10),
	m_gridSize(25)

{
	m_contextMenu = new QMenu();
	m_contextMenu->setTitle(tr("Component settings"));

	QAction* exportAction = new QAction(tr("&Export"), m_contextMenu);
	exportAction->setCheckable(true);
	connect(exportAction, SIGNAL( toggled(bool)), this, SLOT(OnExportChanged(bool)));

	QAction* renameAction = new QAction(tr("&Rename"), m_contextMenu);
	connect(renameAction, SIGNAL( activated()), this, SLOT(OnRename()));

//	m_contextMenu->actions().at(0)->setChecked(exported);

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


const CComponentView::ComponentData* CComponentView::GetComponent() const
{
	return &m_componentRef;
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


bool CComponentView::Serialize(iser::IArchive& archive)
{
	double x = pos().x();
	double y = pos().y();

	static iser::CArchiveTag posxTag("PosX", "X Poistion");
	bool result = archive.BeginTag(posxTag);
	result = result && archive.Process(x);
	result = result && archive.EndTag(posxTag);

	static iser::CArchiveTag posyTag("PosY", "Y Poistion");
	result = result && archive.BeginTag(posyTag);
	result = result && archive.Process(y);
	result = result && archive.EndTag(posyTag);
	
	if (!archive.IsStoring()){
		setPos(x,y);
	}

	return result;
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

QRectF CComponentView::GetInnerRect()const
{
	QRectF mainRect = rect();

	return mainRect;
}


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
	QString componentPath = QString(m_componentRef.packageId.c_str()) + QString(".") + m_componentRef.componentId.c_str();

	width = qMax(width, fontInfo2.width(componentPath) + 20);

/*	if (m_isExported){
		width += m_gridSize + m_gridSize;
	}
*/
	width += (m_gridSize - (width % m_gridSize));
	height += (m_gridSize - (height % m_gridSize));

	width = std::max(width, height * 2);

	return QRect(rect().left(), rect().top(), width, height);
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
		painter->fillRect(shadowRect, Qt::darkYellow);
		painter->setPen(Qt::blue);
		painter->fillRect(mainRect, QBrush(QColor(235,255,160,255)));
	}
	else{
		painter->fillRect(shadowRect, Qt::darkGray);
		painter->fillRect(mainRect, QBrush(QColor(255,255,255,255)));
		painter->setPen(Qt::black);
	}

	painter->drawRect(mainRect);

/*	if (m_isExported){
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
*/
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
						QString(m_componentRef.packageId.c_str()) + QString(".") + m_componentRef.componentId.c_str());
	
	painter->restore();
}


QVariant CComponentView::itemChange(GraphicsItemChange change, const QVariant& value)
{
	switch(change){
		case QGraphicsItem::ItemSelectedChange:
			emit selectionChanged(this, value.toBool());
			foreach(CComponentConnector* connector, m_connectors){
				if (value.toBool()){
					connector->setZValue(0);
				}
				else{
					connector->setZValue(0);
				}

				connector->setSelected(value.toBool());
			}

			break;
		case QGraphicsItem::ItemPositionChange:
			QPointF myCurrentPosition = pos();
			QPoint newPos = value.toPoint();
			newPos.rx() = newPos.rx() - (newPos.rx() % m_gridSize);
			newPos.ry() = newPos.ry() - (newPos.ry() % m_gridSize);

			foreach (CComponentConnector* connector, m_connectors){
				connector->Adjust();
			}

			QGraphicsRectItem* parentItem = dynamic_cast<QGraphicsRectItem*>(this->parentItem());
			if (parentItem != NULL){
				parentItem->setRect(parentItem->childrenBoundingRect().adjusted(-m_gridSize, -m_gridSize, m_gridSize, m_gridSize));
			}
				
			return QVariant(newPos);

	}

	return QGraphicsRectItem::itemChange(change, value);
}