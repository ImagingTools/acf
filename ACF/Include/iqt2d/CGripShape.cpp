#include "iqt2d/CGripShape.h"


#include <QPen>
#include <QBrush>
#include <QFont>
#include <QCursor>


namespace iqt2d
{


// public methods

CGripShape::CGripShape(QGraphicsItem* parentPtr)
:	BaseClass(parentPtr),
	m_labelItem(this)
{
	BaseClass::setPen(QPen(QBrush(QColor(10, 126, 242, 255)), 0));
	BaseClass::setBrush(QBrush(QColor(10, 126, 242, 128)));
	setRect(-5, -5, 10, 10);

	setFlags(ItemIsSelectable | ItemIsMovable | ItemIgnoresTransformations);
	setCursor(QCursor(Qt::ArrowCursor)); 

	// setup label item:
	QFont font = m_labelItem.font();
	font.setPointSize(7);

	m_labelItem.setFont(font);
	m_labelItem.setBrush(Qt::yellow);
	m_labelItem.setFlag(ItemIgnoresTransformations);
	m_labelItem.setZValue(zValue() + 1);

	m_labelItem.setPos(0, -20);
	m_labelItem.setVisible(false);
}


// protected methods

void CGripShape::OnPositionChanged(const QPointF& position)
{
	m_labelItem.setText(QString("%1 %2").arg(position.x(), 0, 'f', 2).arg(position.y(), 0, 'f', 2));

	emit PositionChanged(position);
}


// reimplemented (QGraphicsItem) 

QVariant CGripShape::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionChange) {
		OnPositionChanged(value.toPointF());
	}

	if (change == ItemSelectedChange){
		bool isSelected = value.toBool();
			
		m_labelItem.setVisible(isSelected);
	}

	return BaseClass::itemChange(change, value);
}


} // namespace iqt2d


