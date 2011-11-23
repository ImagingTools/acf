#include "icmpstr/CGraphicsConnectorItem.h"


// STL includes
#include <cmath>


// Qt includes
#include <QPainter>
#include <QPainterPath>


// ACF includes
#include "i2d/CVector2d.h"

#include "iqt/iqt.h"

#include "icmpstr/CRegistryElementShape.h"


// public methods

namespace icmpstr
{


CGraphicsConnectorItem::CGraphicsConnectorItem(
			const iqt2d::ISceneProvider& sceneProvider,
			int connectFlags,
			QGraphicsItem* parent)
:	BaseClass(parent),
	m_sceneProvider(sceneProvider),
	m_connectFlags(connectFlags)
{
	setAcceptedMouseButtons(0);
	setAcceptsHoverEvents(true);

	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

	setZValue(1);
}


void CGraphicsConnectorItem::InitEnds(CRegistryElementShape* sourceShapePtr, CRegistryElementShape* destShapePtr)
{
	if (sourceShapePtr != NULL){
		m_sourceRect = sourceShapePtr->GetViewRect();
		m_isSourceSelected = sourceShapePtr->isSelected();

		connect(sourceShapePtr, SIGNAL(RectChanged(QRectF)), this, SLOT(OnSourceRectMoved(QRectF)));
		connect(sourceShapePtr, SIGNAL(SelectionChanged(bool)), this, SLOT(OnSourceSelected(bool)));
	}

	if (destShapePtr != NULL){
		m_destRect = destShapePtr->GetViewRect();
		m_isDestSelected = destShapePtr->isSelected();

		connect(destShapePtr, SIGNAL(RectChanged(QRectF)), this, SLOT(OnDestRectMoved(QRectF)));
		connect(destShapePtr, SIGNAL(SelectionChanged(bool)), this, SLOT(OnDestSelected(bool)));
	}

	Adjust();
}


// reimplemented (QGraphicsItem)

int CGraphicsConnectorItem::type() const
{ 
	return Type; 
}


QPainterPath CGraphicsConnectorItem::shape() const
{
	QPainterPath path;
	path.addPolygon(m_connectionLine);

	return path;
}


bool CGraphicsConnectorItem::contains(const QPointF& point) const
{
	i2d::CVector2d position = iqt::GetCVector2d(point);
	int nodesCount = m_connectionLine.count();
	for (int i = 1; i < nodesCount; ++i){
		i2d::CLine2d segment(iqt::GetCVector2d(m_connectionLine.at(i - 1)), iqt::GetCVector2d(m_connectionLine.at(i)));

		if (segment.GetDistance(position) < 2){
			return true;
		}
	}

	return false;
}


// protected members

void CGraphicsConnectorItem::Adjust()
{
	prepareGeometryChange();

	double middleX = (istd::Max(m_sourceRect.left(), m_destRect.left()) + istd::Min(m_sourceRect.right(), m_destRect.right())) * 0.5;
	double middleY = (istd::Max(m_sourceRect.top(), m_destRect.top()) + istd::Min(m_sourceRect.bottom(), m_destRect.bottom())) * 0.5;

	double sourceMidY = (m_sourceRect.top() + m_sourceRect.bottom()) * 0.5;
	double destMidX = (m_destRect.left() + m_destRect.right()) * 0.5;

	double gridSize = 0;
	m_sceneProvider.GetSceneAlignment(gridSize);

	m_connectionLine.clear();
	if (m_sourceRect.right() < m_destRect.left()){
		m_touchPoint.setX(m_sourceRect.right());

		if (m_sourceRect.bottom() < m_destRect.top()){
			m_touchPoint.setY(sourceMidY + gridSize);

			m_connectionLine.push_back(QPointF(m_sourceRect.right() + GP_OFFSET, sourceMidY + gridSize));
			m_connectionLine.push_back(QPointF(destMidX - gridSize, sourceMidY + gridSize));
			m_connectionLine.push_back(QPointF(destMidX - gridSize, m_destRect.top()));
		}
		else if (m_sourceRect.top() > m_destRect.bottom()){
			m_touchPoint.setY(sourceMidY - gridSize);

			m_connectionLine.push_back(QPointF(m_sourceRect.right() + GP_OFFSET, sourceMidY - gridSize));
			m_connectionLine.push_back(QPointF(destMidX - gridSize, sourceMidY - gridSize));
			m_connectionLine.push_back(QPointF(destMidX - gridSize, m_destRect.bottom()));
		}
		else{
			m_touchPoint.setY(middleY);

			m_connectionLine.push_back(QPointF(m_sourceRect.right() + GP_OFFSET, middleY));
			m_connectionLine.push_back(QPointF(m_destRect.left(), middleY));
		}
	}
	else if (m_sourceRect.left() > m_destRect.right()){
		m_touchPoint.setX(m_sourceRect.left());

		if (m_sourceRect.bottom() < m_destRect.top()){
			m_touchPoint.setY(sourceMidY + gridSize);

			m_connectionLine.push_back(QPointF(m_sourceRect.left() - GP_OFFSET, sourceMidY + gridSize));
			m_connectionLine.push_back(QPointF(destMidX + gridSize, sourceMidY + gridSize));
			m_connectionLine.push_back(QPointF(destMidX + gridSize, m_destRect.top()));
		}
		else if (m_sourceRect.top() > m_destRect.bottom()){
			m_touchPoint.setY(sourceMidY - gridSize);

			m_connectionLine.push_back(QPointF(m_sourceRect.left() - GP_OFFSET, sourceMidY - gridSize));
			m_connectionLine.push_back(QPointF(destMidX + gridSize, sourceMidY - gridSize));
			m_connectionLine.push_back(QPointF(destMidX + gridSize, m_destRect.bottom()));
		}
		else{
			m_touchPoint.setY(middleY);

			m_connectionLine.push_back(QPointF(m_sourceRect.left() - GP_OFFSET, middleY));
			m_connectionLine.push_back(QPointF(m_destRect.right(), middleY));
		}
	}
	else{
		m_touchPoint.setX(middleX);
		if (m_sourceRect.bottom() < m_destRect.top()){
			m_touchPoint.setY(m_sourceRect.bottom());

			m_connectionLine.push_back(QPointF(middleX, m_sourceRect.bottom() + GP_OFFSET));
			m_connectionLine.push_back(QPointF(middleX, m_destRect.top()));
		}
		else if (m_sourceRect.top() > m_destRect.bottom()){
			m_touchPoint.setY(m_sourceRect.top());

			m_connectionLine.push_back(QPointF(middleX, m_sourceRect.top() - GP_OFFSET));
			m_connectionLine.push_back(QPointF(middleX, m_destRect.bottom()));
		}
		else{
			m_touchPoint.setY(middleY);
		}
	}

	// TODO: add implementation for self-reference
}


// reimplemented (QGraphicsItem)

QRectF CGraphicsConnectorItem::boundingRect() const
{
	if (m_connectionLine.size() < 2){
		return QRectF();
	}

	double penWidth = 1.0;
	double extra = penWidth + GP_RADIUS2;

	QRectF rect = m_connectionLine.boundingRect();
	rect = rect.unite(QRectF(m_touchPoint, QSize(1, 1)));

	return rect.adjusted(-extra, -extra, extra, extra);
}


void CGraphicsConnectorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if (		(m_connectionLine.size() < 2) ||
				(m_connectionLine[0] == m_connectionLine[1])){
		return;
	}

	const QPointF& circlePoint = m_connectionLine.first();
	i2d::CVector2d circleDirection(circlePoint.x() - m_touchPoint.x(), circlePoint.y() - m_touchPoint.y());
	int degree = 360 - int(imath::GetDegreeFromRadian(circleDirection.GetAngle()));
	circleDirection.Normalize(GP_OFFSET - GP_RADIUS2);

	// drawing:
	painter->save();

	painter->setRenderHints(QPainter::Antialiasing, true);

	QColor referenceColor = ((m_connectFlags & CF_FACTORY) != 0)? Qt::darkYellow: Qt::darkBlue;

	if (m_isSourceSelected){
		referenceColor = ((m_connectFlags & CF_FACTORY) != 0)? QColor(0, 255, 155, 255): QColor(0, 150, 50, 255);
	}

	painter->setPen(QPen(referenceColor, 1, Qt::SolidLine));
	painter->drawLine(m_touchPoint, m_touchPoint + iqt::GetQPointF(circleDirection));
	
	QRectF circleRect2(circlePoint.x() - GP_RADIUS2, circlePoint.y() - GP_RADIUS2, GP_RADIUS2 * 2, GP_RADIUS2 * 2);
	painter->drawArc(circleRect2, (degree + 45) * 16, 270 * 16);

	QColor interfaceColor = ((m_connectFlags & CF_FACTORY) != 0)? Qt::darkYellow: Qt::darkBlue;

	if (m_isDestSelected){
		interfaceColor = ((m_connectFlags & CF_FACTORY) != 0)? QColor(0, 255, 155, 255): QColor(0, 150, 50, 255);
	}

	painter->setPen(QPen(interfaceColor, 1, Qt::SolidLine));
	painter->setBrush(interfaceColor);
	QRectF circleRect(circlePoint.x() - GP_RADIUS, circlePoint.y() - GP_RADIUS, GP_RADIUS * 2, GP_RADIUS * 2);
	painter->drawEllipse(circleRect);

	if ((m_connectFlags & CF_EMBEDDED) != 0){
		painter->setPen(QPen(interfaceColor, 1, Qt::DashLine));
	}

	painter->drawPolyline(m_connectionLine);

	painter->restore();
}


// protected slots

void CGraphicsConnectorItem::OnSourceRectMoved(const QRectF& rect)
{
	m_sourceRect = rect;

	Adjust();
}


void CGraphicsConnectorItem::OnDestRectMoved(const QRectF& rect)
{
	m_destRect = rect;

	Adjust();
}


void CGraphicsConnectorItem::OnSourceSelected(bool state)
{
	prepareGeometryChange();

	m_isSourceSelected = state;

	setZValue((m_isSourceSelected || m_isDestSelected)? 2: 1);
}


void CGraphicsConnectorItem::OnDestSelected(bool state)
{
	prepareGeometryChange();

	m_isDestSelected = state;

	setZValue((m_isSourceSelected || m_isDestSelected)? 2: 1);
}


} // namespace icmpstr


