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
#include "CRegistryViewComp.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "iqt/iqt.h"


CComponentView::CComponentView(
			const CRegistryViewComp* registryViewPtr,
			const icomp::IRegistry* registryPtr,
			const icomp::IRegistry::ElementInfo* elementInfoPtr, 
			const std::string& componentName,
			QGraphicsItem* parent, 
			QGraphicsScene* scene) 
:	QGraphicsRectItem(parent, scene),
	m_registryView(*registryViewPtr),
	m_registry(*registryPtr),
	m_componentName(componentName),
	m_componentLabelFontSize(14),
	m_componentIdFontSize(10)
{
	I_ASSERT(registryViewPtr != NULL);
	I_ASSERT(registryPtr != NULL);

	CalcExportedInteraces();

	SetElementInfo(elementInfoPtr);

	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setCursor(Qt::ArrowCursor);
}


CComponentView::~CComponentView()
{
	foreach(CComponentConnector* connector, m_connectors){
		connector->SetSourceComponent(NULL);
		connector->SetDestinationComponent(NULL);
	}

}


const icomp::IRegistry::ElementInfo& CComponentView::GetElementInfo() const
{
	I_ASSERT(m_elementInfoPtr != NULL);

	return *m_elementInfoPtr;
}


void CComponentView::SetElementInfo(const icomp::IRegistry::ElementInfo* elementInfoPtr)
{
	I_ASSERT(elementInfoPtr != NULL);

	m_elementInfoPtr = elementInfoPtr;

	if (m_elementInfoPtr->elementPtr.IsValid()){
		const icomp::IComponentStaticInfo& staticInfo = m_elementInfoPtr->elementPtr->GetComponentStaticInfo();
		setToolTip(iqt::GetQString(staticInfo.GetDescription()));
	}

	setRect(CalculateRect());
}


const std::string& CComponentView::GetComponentName() const
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


// protected members
 
QRect CComponentView::CalculateRect() const
{
	I_ASSERT(m_elementInfoPtr != NULL);

	const int margins = 20;

	QFont labelFont(qApp->font());
	labelFont.setBold(true);
	labelFont.setPixelSize(m_componentLabelFontSize);
	QFontMetrics fontInfo(labelFont);

	QFont labelFont2(qApp->font());
	labelFont.setPixelSize(m_componentIdFontSize);
	QFontMetrics fontInfo2(labelFont2);

	int width = margins;
	int height = margins;

	QString componentPath = QString(m_elementInfoPtr->address.GetPackageId().c_str()) + QString(".") + m_elementInfoPtr->address.GetComponentId().c_str();
	width += qMax(fontInfo.width(iqt::GetQString(m_componentName)), fontInfo2.width(componentPath));

	height += fontInfo.height();
	m_componentLabelHeight = fontInfo.height();

	const int verticalSpace = 10;
	m_componentIdHeight = fontInfo2.height();
	height += (m_componentIdHeight + verticalSpace);

	double gridSize = m_registryView.GetGrid();

	if (!m_exportedInterfacesList.empty()){
		width += 50;
	}

	const int shadowOffset = 10;
	width += shadowOffset;
	height += shadowOffset;

	width = ::ceil(width / gridSize) * gridSize;
	height = ::ceil(height / gridSize) * gridSize;

	return QRect(pos().x(), pos().y(), width, height);
}


void CComponentView::CalcExportedInteraces()
{
	using icomp::IRegistry::ExportedInterfacesMap::const_iterator;

	m_exportedInterfacesList.clear();

	const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = m_registry.GetExportedInterfacesMap();
	for (		const_iterator iter = interfacesMap.begin();
				iter != interfacesMap.end();
				++iter){
		if (iter->second == m_componentName){
			m_exportedInterfacesList.push_back(iter->first.c_str());
		}
	}
}


// reimplemented (QGraphicsRectItem)

void CComponentView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	I_ASSERT(m_elementInfoPtr != NULL);

	QRectF mainRect = rect();

	mainRect.adjust(0, 0, -10, -10);
	QRectF shadowRect = mainRect;
	shadowRect.adjust(10, 10, 10, 10);
	
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
		QRectF exportRect = mainRect.adjusted(mainRect.width() - 45, 20, -20, 45 - mainRect.height());
		painter->fillRect(exportRect, QBrush(Qt::magenta));
		
		QPen pen(Qt::magenta, 2);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);
		painter->setPen(pen);
		
		painter->setRenderHints(QPainter::Antialiasing);
		painter->drawLine(exportRect.right(), exportRect.top() + 6, exportRect.right() + 15, exportRect.top() + 6);
		painter->drawLine(exportRect.right() + 15, exportRect.top() + 6, exportRect.right() + 10, exportRect.top() + 3);
		painter->drawLine(exportRect.right() + 15, exportRect.top() + 6, exportRect.right() + 10, exportRect.top() + 9);

		painter->drawLine(exportRect.right(), exportRect.top() + 18, exportRect.right() + 15, exportRect.top() + 18);
		painter->drawLine(exportRect.right() + 15, exportRect.top() + 18, exportRect.right() + 10, exportRect.top()+ 21);
		painter->drawLine(exportRect.right() + 15, exportRect.top() + 18, exportRect.right() + 10, exportRect.top() + 15);

		painter->restore();
	}

	mainRect.adjust(10, 10, 0, 0);
	mainRect.setHeight(m_componentLabelHeight);
	painter->save();
	painter->setPen(Qt::black);

	QFont labelFont(qApp->font());
	labelFont.setBold(true);
	labelFont.setPixelSize(m_componentLabelFontSize);
	painter->setFont(labelFont);
	painter->drawText(mainRect, Qt::AlignLeft | Qt::TextSingleLine, m_componentName.c_str());

	mainRect.moveTop(mainRect.bottom() + 10);
	mainRect.setHeight(m_componentIdHeight);
	QFont labelFont2(qApp->font());
	labelFont2.setPixelSize(m_componentIdFontSize);
	painter->setFont(labelFont2);

	painter->drawText(	mainRect, 
						Qt::AlignLeft | Qt::TextSingleLine, 
						QString(m_elementInfoPtr->address.GetPackageId().c_str()) + QString(".") + m_elementInfoPtr->address.GetComponentId().c_str());
	
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
			double gridSize = m_registryView.GetGrid();
			newPos.setX(::floor((newPos.x() + size.width() * 0.5) / gridSize + 0.5) * gridSize - size.width() * 0.5);
			newPos.setY(::floor((newPos.y() + size.height() * 0.5) / gridSize + 0.5) * gridSize - size.height() * 0.5);

			foreach (CComponentConnector* connector, m_connectors){
				connector->Adjust();
			}

			emit positionChanged(this, newPos);
			
			return QVariant(newPos);
	}

	return QGraphicsRectItem::itemChange(change, value);
}


