#include "iqt2d/CGripShape.h"


#include <QPen>
#include <QBrush>
#include <QFont>
#include <QCursor>


#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CGripShape::CGripShape(QGraphicsItem* parentPtr, const ISceneProvider* providerPtr)
:	BaseClass(true, providerPtr),
	m_labelItem(this)
{
	I_ASSERT(parentPtr != NULL);

	setParentItem(parentPtr);

	SetPen(InactiveColor, QPen(QBrush(QColor(0, 0, 192)), 0));
	SetPen(EditableColor, QPen(QBrush(QColor(0, 0, 192)), 0));
	SetPen(SelectedColor, QPen(QBrush(QColor(0, 0, 255)), 0));

	SetBrush(InactiveColor, QBrush(QColor(10, 126, 242)));
	SetBrush(EditableColor, QBrush(QColor(10, 126, 242)));
	SetBrush(SelectedColor, QBrush(QColor(255, 255, 0)));

	setRect(-5, -5, 10, 10);

	setFlags(ItemIsSelectable | ItemIsMovable | ItemIgnoresTransformations);
	setCursor(QCursor(Qt::CrossCursor)); 

	// setup label item:
	QFont font = m_labelItem.font();
	font.setPointSize(7);

	m_labelItem.setFont(font);
	m_labelItem.setBrush(Qt::yellow);
	m_labelItem.setFlag(ItemIgnoresTransformations);
	m_labelItem.setZValue(zValue() + 1);

	m_labelItem.setVisible(false);

	SwitchColorSheme(InactiveColor);
	SetLabelPosition(LabelTop);
}


i2d::CVector2d CGripShape::GetPosition() const
{
	if (parentItem() != NULL){
		return iqt::GetCVector2d(pos() + parentItem()->pos());
	}
	else{
		return i2d::CVector2d(0, 0);
	}
}


void CGripShape::SetPosition(const i2d::CVector2d& position)
{
	if (parentItem() != NULL){
		iqt::CSignalBlocker blocker(this);

		setPos(iqt::GetQPointF(position) - parentItem()->pos());
	}
}


void CGripShape::SetLabelPosition(int labelArea)
{
	const int labelMargin = 10;

	QFontMetrics fontMetrics(m_labelItem.font());

	int textHeight = fontMetrics.height();
	int textWidth = fontMetrics.width(QString("%1 %2").arg(pos().x(), 6, 'f', 2).arg(pos().y(), 6, 'f', 2));

	int dy = textHeight + labelMargin;
	int dx = textWidth + labelMargin;

	switch (labelArea){
	case LabelTop:
		m_labelItem.setPos(-textWidth/2, -dy);
		break;
	case LabelBottom:
		m_labelItem.setPos(-textWidth/2, labelMargin);
		break;
	case LabelLeft:
		m_labelItem.setPos(-(dx + rect().width()/2), -textHeight/2);
		break;
	case LabelRight:
		m_labelItem.setPos(labelMargin, -textHeight/2);
		break;
	}
}


// protected methods

// reimplemented (TShapeBase<QGraphicsEllipseItem>)

void CGripShape::OnPositionChanged(const QPointF& /*position*/)
{
	i2d::CVector2d position = GetPosition();
	m_labelItem.setText(QString("%1 %2").arg(position.GetX(), 0, 'f', 2).arg(position.GetY(), 0, 'f', 2));

	Q_EMIT PositionChanged(position);
}


void CGripShape::OnSelectionChanged(bool isSelected)
{
	m_labelItem.setVisible(isSelected);

	BaseClass::OnSelectionChanged(isSelected);
}


} // namespace iqt2d


