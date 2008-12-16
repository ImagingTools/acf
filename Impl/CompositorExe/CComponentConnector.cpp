#include "CComponentConnector.h"


// STL includes
#include <cmath>

// Qt includes
#include <QPainter>

// ACF includes
#include "i2d/CVector2d.h"
#include "iqt/iqt.h"

#include "CComponentView.h"
#include "CRegistryViewComp.h"


// public methods

CComponentConnector::CComponentConnector(
			const CRegistryViewComp* registryViewPtr,
			CComponentView* sourceComponent, 
			CComponentView* destComponent,
			QGraphicsItem *parent)
:	BaseClass(parent),
	m_registryView(*registryViewPtr)
{
	I_ASSERT(registryViewPtr != NULL);

	setAcceptedMouseButtons(0);
	setAcceptsHoverEvents(true);

	m_destComponent = NULL;
	m_sourceComponent = NULL;

	SetSourceComponent(sourceComponent);
	SetDestinationComponent(destComponent);
	
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setZValue(1);
}


CComponentConnector::~CComponentConnector()
{
	if (m_sourceComponent != NULL){
		m_sourceComponent->RemoveConnector(this);
	}

	if (m_destComponent != NULL){
		m_destComponent->RemoveConnector(this);
	}
}


CComponentView *CComponentConnector::GetSourceComponent() const
{
	return m_sourceComponent;
}


void CComponentConnector::SetSourceComponent(CComponentView* sourceComponent)
{
	m_sourceComponent = sourceComponent;
	if (sourceComponent != NULL){
		m_sourceComponent->AddConnector(this);

		Adjust();
	}

	update();
}

CComponentView *CComponentConnector::GetDestinationComponent() const
{
	return m_destComponent;
}


void CComponentConnector::SetDestinationComponent(CComponentView* destComponent)
{
	m_destComponent = destComponent;
	if (destComponent != NULL){
		m_destComponent->AddConnector(this);

		Adjust();
	}

	update();
}


void CComponentConnector::Adjust()
{
	if (m_destComponent == NULL || m_sourceComponent == NULL){
		return;
	}

	prepareGeometryChange();

	QRectF sourceRect = mapFromItem(m_sourceComponent, m_sourceComponent->GetInnerRect()).boundingRect();
	QRectF destRect = mapFromItem(m_destComponent, m_destComponent->GetInnerRect()).boundingRect();

	double middleX = (istd::Max(sourceRect.left(), destRect.left()) + istd::Min(sourceRect.right(), destRect.right())) * 0.5;
	double middleY = (istd::Max(sourceRect.top(), destRect.top()) + istd::Min(sourceRect.bottom(), destRect.bottom())) * 0.5;

	double sourceMidY = (sourceRect.top() + sourceRect.bottom()) * 0.5;
	double destMidX = (destRect.left() + destRect.right()) * 0.5;

	double gridSize = m_registryView.GetGrid();

	m_connectionLine.clear();
	if (sourceRect.right() < destRect.left()){
		m_touchPoint.setX(sourceRect.right());

		if (sourceRect.bottom() < destRect.top()){
			m_touchPoint.setY(sourceMidY + gridSize);

			m_connectionLine.push_back(QPointF(sourceRect.right() + GP_OFFSET, sourceMidY + gridSize));
			m_connectionLine.push_back(QPointF(destMidX - gridSize, sourceMidY + gridSize));
			m_connectionLine.push_back(QPointF(destMidX - gridSize, destRect.top()));
		}
		else if (sourceRect.top() > destRect.bottom()){
			m_touchPoint.setY(sourceMidY - gridSize);

			m_connectionLine.push_back(QPointF(sourceRect.right() + GP_OFFSET, sourceMidY - gridSize));
			m_connectionLine.push_back(QPointF(destMidX - gridSize, sourceMidY - gridSize));
			m_connectionLine.push_back(QPointF(destMidX - gridSize, destRect.bottom()));
		}
		else{
			m_touchPoint.setY(middleY);

			m_connectionLine.push_back(QPointF(sourceRect.right() + GP_OFFSET, middleY));
			m_connectionLine.push_back(QPointF(destRect.left(), middleY));
		}
	}
	else if (sourceRect.left() > destRect.right()){
		m_touchPoint.setX(sourceRect.left());

		if (sourceRect.bottom() < destRect.top()){
			m_touchPoint.setY(sourceMidY + gridSize);

			m_connectionLine.push_back(QPointF(sourceRect.left() - GP_OFFSET, sourceMidY + gridSize));
			m_connectionLine.push_back(QPointF(destMidX + gridSize, sourceMidY + gridSize));
			m_connectionLine.push_back(QPointF(destMidX + gridSize, destRect.top()));
		}
		else if (sourceRect.top() > destRect.bottom()){
			m_touchPoint.setY(sourceMidY - gridSize);

			m_connectionLine.push_back(QPointF(sourceRect.left() - GP_OFFSET, sourceMidY - gridSize));
			m_connectionLine.push_back(QPointF(destMidX + gridSize, sourceMidY - gridSize));
			m_connectionLine.push_back(QPointF(destMidX + gridSize, destRect.bottom()));
		}
		else{
			m_touchPoint.setY(middleY);

			m_connectionLine.push_back(QPointF(sourceRect.left() - GP_OFFSET, middleY));
			m_connectionLine.push_back(QPointF(destRect.right(), middleY));
		}
	}
	else{
		m_touchPoint.setX(middleX);
		if (sourceRect.bottom() < destRect.top()){
			m_touchPoint.setY(sourceRect.bottom());

			m_connectionLine.push_back(QPointF(middleX, sourceRect.bottom() + GP_OFFSET));
			m_connectionLine.push_back(QPointF(middleX, destRect.top()));
		}
		else if (sourceRect.top() > destRect.bottom()){
			m_touchPoint.setY(sourceRect.top());

			m_connectionLine.push_back(QPointF(middleX, sourceRect.top() - GP_OFFSET));
			m_connectionLine.push_back(QPointF(middleX, destRect.bottom()));
		}
		else{
			m_touchPoint.setY(middleY);
		}
	}

	// TODO: add implementation for self-reference
	if (m_sourceComponent == m_destComponent){
	}
}


// reimplemented (QGraphicsItem)

int CComponentConnector::type() const
{ 
	return Type; 
}


QPainterPath CComponentConnector::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}


// protected members

// reimplemented (QGraphicsItem)

QRectF CComponentConnector::boundingRect() const
{
	if ((m_destComponent == NULL) || (m_sourceComponent == NULL) || (m_connectionLine.size() < 2)){
		return QRectF();
	}

	double penWidth = 1.0;
	double extra = penWidth + GP_RADIUS2;

	QRectF rect = m_connectionLine.boundingRect();
	rect = rect.unite(QRectF(m_touchPoint, QSize(1, 1)));

	return rect.adjusted(-extra, -extra, extra, extra);
}


void CComponentConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if (		(m_destComponent == NULL) ||
				(m_sourceComponent == NULL) ||
				(m_connectionLine.size() < 2) ||
				(m_connectionLine[0] == m_connectionLine[1])){
		return;
	}

	const QPointF& circlePoint = m_connectionLine.first();
	i2d::CVector2d circleDirection(circlePoint.x() - m_touchPoint.x(), circlePoint.y() - m_touchPoint.y());
	int degree = 360 - int(imath::GetDegreeFromRadian(circleDirection.GetAngle()));
	circleDirection.Normalize(GP_OFFSET - GP_RADIUS2);

	// drawing:
	painter->save();

	painter->setRenderHints(QPainter::Antialiasing, isSelected());

	QColor referenceColor = Qt::darkBlue;
	double referencePenWidth = 0;

	if (m_sourceComponent->isSelected()){
		referenceColor = QColor(0, 200, 100, 255);
		referencePenWidth = 2;
	}

	painter->setPen(QPen(referenceColor, referencePenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(m_touchPoint, m_touchPoint + iqt::GetQPointF(circleDirection));
	
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing, true);
	
	QRectF circleRect2(circlePoint.x() - GP_RADIUS2, circlePoint.y() - GP_RADIUS2, GP_RADIUS2 * 2, GP_RADIUS2 * 2);
	painter->drawArc(circleRect2, (degree + 45) * 16, 270 * 16);

	QColor interfaceColor = Qt::darkBlue;
	double interfacePenWidth = 0;

	if (m_destComponent->isSelected()){
		interfaceColor = QColor(0, 200, 100, 255);
		interfacePenWidth = 2;
	}

	painter->setPen(QPen(interfaceColor, interfacePenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->setBrush(interfaceColor);
	QRectF circleRect(circlePoint.x() - GP_RADIUS, circlePoint.y() - GP_RADIUS, GP_RADIUS * 2, GP_RADIUS * 2);
	painter->drawEllipse(circleRect);
	painter->drawPolyline(m_connectionLine);

	painter->restore();

	painter->restore();
}


void CComponentConnector::hoverEnterEvent(QGraphicsSceneHoverEvent* eventPtr)
{
	setSelected(true);
	
	update();

	BaseClass::hoverEnterEvent(eventPtr);
}


void CComponentConnector::hoverLeaveEvent(QGraphicsSceneHoverEvent* eventPtr)
{
	setSelected(false);

	update();

	BaseClass::hoverLeaveEvent(eventPtr);
}


