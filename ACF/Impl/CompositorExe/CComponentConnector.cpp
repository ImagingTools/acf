#include <QPainter>
#include <math.h>


#include "imath/CVector2d.h"

#include "iqt/iqt.h"

#include "CComponentView.h"
#include "CComponentConnector.h"


// public methods

CComponentConnector::CComponentConnector(CComponentView* sourceComponent, 
										 CComponentView* destComponent,
										 QGraphicsItem *parent, 
										 QGraphicsScene *scene)
:	BaseClass(parent, scene)
{
	setAcceptedMouseButtons(0);

	m_destComponent = NULL;
	m_sourceComponent = NULL;

	SetSourceComponent(sourceComponent);
	SetDestinationComponent(destComponent);
	
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
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

	double middleX = ((sourceRect.right() - destRect.left()) < (destRect.right() - sourceRect.left()))?
				(sourceRect.right() + destRect.left()) * 0.5:
				(destRect.right() + sourceRect.left()) * 0.5;
	double middleY = ((sourceRect.bottom() - destRect.top()) < (destRect.bottom() - sourceRect.top()))?
				(sourceRect.bottom() + destRect.top()) * 0.5:
				(destRect.bottom() + sourceRect.top()) * 0.5;

	m_connectionLine.clear();
	if (sourceRect.right() < destRect.left()){
		m_touchPoint.setX(sourceRect.right());

		if (sourceRect.bottom() < destRect.top()){
			m_touchPoint.setY(sourceRect.top() * 0.25 + sourceRect.bottom() * 0.75);

			m_connectionLine.push_back(QPointF(sourceRect.right() + GP_OFFSET, sourceRect.top() * 0.25 + sourceRect.bottom() * 0.75));
			m_connectionLine.push_back(QPointF(destRect.left() * 0.75 + destRect.right() * 0.25, sourceRect.top() * 0.25 + sourceRect.bottom() * 0.75));
			m_connectionLine.push_back(QPointF(destRect.left() * 0.75 + destRect.right() * 0.25, destRect.top()));
		}
		else if (sourceRect.top() > destRect.bottom()){
			m_touchPoint.setY(sourceRect.top() * 0.75 + sourceRect.bottom() * 0.25);

			m_connectionLine.push_back(QPointF(sourceRect.right() + GP_OFFSET, sourceRect.top() * 0.75 + sourceRect.bottom() * 0.25));
			m_connectionLine.push_back(QPointF(destRect.left() * 0.75 + destRect.right() * 0.25, sourceRect.top() * 0.75 + sourceRect.bottom() * 0.25));
			m_connectionLine.push_back(QPointF(destRect.left() * 0.75 + destRect.right() * 0.25, destRect.bottom()));
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
			m_touchPoint.setY(sourceRect.top() * 0.25 + sourceRect.bottom() * 0.75);

			m_connectionLine.push_back(QPointF(sourceRect.left() - GP_OFFSET, sourceRect.top() * 0.25 + sourceRect.bottom() * 0.75));
			m_connectionLine.push_back(QPointF(destRect.left() * 0.25 + destRect.right() * 0.75, sourceRect.top() * 0.25 + sourceRect.bottom() * 0.75));
			m_connectionLine.push_back(QPointF(destRect.left() * 0.25 + destRect.right() * 0.75, destRect.top()));
		}
		else if (sourceRect.top() > destRect.bottom()){
			m_touchPoint.setY(sourceRect.top() * 0.75 + sourceRect.bottom() * 0.25);

			m_connectionLine.push_back(QPointF(sourceRect.left() - GP_OFFSET, sourceRect.top() * 0.75 + sourceRect.bottom() * 0.25));
			m_connectionLine.push_back(QPointF(destRect.left() * 0.25 + destRect.right() * 0.75, sourceRect.top() * 0.75 + sourceRect.bottom() * 0.25));
			m_connectionLine.push_back(QPointF(destRect.left() * 0.25 + destRect.right() * 0.75, destRect.bottom()));
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
	double extra = (penWidth + GP_RADIUS2) / 2.0;

	QRectF rect = m_connectionLine.boundingRect();
	rect.unite(QRectF(m_touchPoint, QSize(1, 1)));

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
	imath::CVector2d circleDirection(circlePoint.x() - m_touchPoint.x(), circlePoint.y() - m_touchPoint.y());
	int degree = 360 - circleDirection.GetAngle() * 180 / I_PI;
	circleDirection.Normalize(GP_OFFSET - GP_RADIUS2);

	// drawing:
	painter->save();

	painter->setRenderHints(QPainter::Antialiasing, isSelected());
	QColor color = isSelected() ? Qt::red : Qt::darkBlue;
	QPen pen(color, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	painter->setPen(pen);
	painter->drawPolyline(m_connectionLine);
	
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing, true);
	
	painter->drawLine(m_touchPoint, m_touchPoint + iqt::GetQPointF(circleDirection));
	QRectF circleRect2(circlePoint.x() - GP_RADIUS2, circlePoint.y() - GP_RADIUS2, GP_RADIUS2 * 2, GP_RADIUS2 * 2);
	painter->drawArc(circleRect2, (degree + 45) * 16, 270 * 16);

	painter->save();

	painter->setBrush(color);
	QRectF circleRect(circlePoint.x() - GP_RADIUS, circlePoint.y() - GP_RADIUS, GP_RADIUS * 2, GP_RADIUS * 2);
	painter->drawEllipse(circleRect);

	painter->restore();

	painter->restore();

	painter->restore();
}


void CComponentConnector::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	setSelected(true);
	
	update();
}


void CComponentConnector::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	setSelected(false);

	update();
}


