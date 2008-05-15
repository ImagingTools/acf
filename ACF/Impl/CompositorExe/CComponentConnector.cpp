#include <QPainter>
#include <math.h>

#include "CComponentView.h"
#include "CComponentConnector.h"


static const double Pi = 3.1415926;
static double TwoPi = 2.0 * Pi;


// public methods

CComponentConnector::CComponentConnector(CComponentView* sourceComponent, CComponentView* destComponent)
	:m_arrowSize(10)
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

	QPointF srcCenter = mapFromItem(m_sourceComponent, m_sourceComponent->rect().center());
	QPointF srcBottomLeft = mapFromItem(m_sourceComponent, m_sourceComponent->rect().bottomLeft());
	QPointF srcTopRight = mapFromItem(m_sourceComponent, m_sourceComponent->rect().topRight());
	QPointF destCenter = mapFromItem(m_destComponent, m_destComponent->rect().center());
	QPointF destBottomLeft = mapFromItem(m_destComponent, m_destComponent->rect().bottomLeft());
	QPointF destTopRight = mapFromItem(m_destComponent, m_destComponent->rect().topRight());

	m_sourcePoint = srcCenter;
	m_destPoint = destCenter;

	QPointF diffPoint = destCenter - srcCenter;
	if (diffPoint.y() > 0){
		m_sourcePoint.ry() = srcBottomLeft.y();
	}
	else if (diffPoint.y() < 0){
		m_sourcePoint.ry() = srcTopRight.y();
	}
	else{
		if (diffPoint.x() < 0){
			m_sourcePoint.rx() = srcBottomLeft.x();
		}

		if (diffPoint.x() > 0){
			m_sourcePoint.rx() = srcTopRight.x();
		}
	}

	if (diffPoint.x() > 0){
		m_destPoint.rx() = destBottomLeft.x();
	}
	else if (diffPoint.x() < 0){
		m_destPoint.rx() = destTopRight.x();
	}
	else{
		if (diffPoint.y() > 0){
			m_destPoint.ry() = destTopRight.y();
		}

		if (diffPoint.y() < 0){
			m_destPoint.ry() = destBottomLeft.y();
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
	if (m_destComponent == NULL || m_sourceComponent == NULL){
		return QRectF();
	}

	double penWidth = 1.0;
	double extra = (penWidth + m_arrowSize) / 2.0;

	return QRectF(m_sourcePoint, QSizeF(m_destPoint.x() - m_sourcePoint.x(),
		m_destPoint.y() - m_sourcePoint.y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}


void CComponentConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if (m_destComponent == NULL || m_sourceComponent == NULL || m_sourcePoint == m_destPoint){
		return;
	}

	const int diameter = 32;
	const int offset = 25;
	const int radius = diameter / 2;
	const int diameter2 = 16;
	const int radius2 = diameter2 / 2;

	QLineF horizontalLine1(m_destPoint.x(), m_destPoint.y(), m_sourcePoint.x(), m_destPoint.y());
	QLineF vericalLine1(m_sourcePoint.x(), m_sourcePoint.y(), m_sourcePoint.x(), m_destPoint.y());
	double angle = 0;
	if (vericalLine1.p1().x() == vericalLine1.p2().x()){
		if (vericalLine1.p1().y() > vericalLine1.p2().y()){
			angle = Pi / 2.0;
		}
		else{
			angle = Pi + Pi / 2.0;
		}
	}

	if (vericalLine1.p1().y() == vericalLine1.p2().y()){
		if (vericalLine1.p1().x() > vericalLine1.p2().x()){
			angle = 0;
		}
		else{
			angle = Pi;
		}
	}

	int degree = 90 + angle * 180 / Pi;
	QPointF arcPosition(m_sourcePoint.x() - radius + offset * cos(angle), 
		m_sourcePoint.y() - radius - offset*sin(angle));
	QPointF chordPosition(m_sourcePoint.x() - radius2 + offset * cos(angle), 
		m_sourcePoint.y() - radius2 - offset * sin(angle));

	QPoint arrowOffset(45 * cos(angle), -45 * sin(angle));
	QPointF sourceArrowP1 = arrowOffset + m_sourcePoint + QPointF(sin(angle + Pi / 3) * m_arrowSize,
		cos(angle + Pi / 3) * m_arrowSize);
	QPointF sourceArrowP2 = arrowOffset + m_sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * m_arrowSize,
		cos(angle + Pi - Pi / 3) * m_arrowSize);   

	// drawing:
	painter->save();

	painter->setRenderHints(QPainter::Antialiasing, isSelected());
	QColor color = isSelected() ? Qt::red : Qt::darkBlue;
	QPen pen(color, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	painter->setPen(pen);
	if (m_destPoint.y() == m_sourcePoint.y()){
		painter->drawLine(horizontalLine1);
		vericalLine1 = horizontalLine1;
	}
	else if(m_destPoint.x() == m_sourcePoint.x()){
		painter->drawLine(vericalLine1);
	}
	else{
		painter->drawLine(horizontalLine1);
		painter->drawLine(vericalLine1);
	}
	
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing, true);
	
	painter->drawArc(arcPosition.x(), arcPosition.y(), diameter, diameter, degree * 16 - 15 * 16, 5760 / 2 + 2 * 15 * 16); 
	painter->setBrush(color);
	painter->drawChord(chordPosition.x(), chordPosition.y(), diameter2, diameter2, 0, 5760); 

	painter->restore();
	 
	// Draw the arrows if there's enough room
	painter->setRenderHints(QPainter::Antialiasing, true);
	painter->setBrush(color);
	painter->drawPolygon(QPolygonF() << m_sourcePoint + arrowOffset << sourceArrowP1 << sourceArrowP2);

	painter->restore();

	painter->save();
		painter->setPen(QPen(Qt::red, 1));
		painter->drawPoint(m_sourcePoint.x() + (offset-radius+radius2) * cos(angle), m_sourcePoint.y() - (offset-radius+radius2)*sin(angle));
		painter->drawPoint(m_sourcePoint.x() + (offset-radius) * cos(angle), m_sourcePoint.y() - (offset-radius)*sin(angle));
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
