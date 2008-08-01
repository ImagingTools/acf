#include "iqt2d/CLine2dShape.h"


#include <QPen>
#include <QBrush>
#include <QCursor>

#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CLine2dShape::CLine2dShape()
:	m_pointGrip1(this),
	m_pointGrip2(this),
	m_lastPosition(0,0)
{
	connect(&m_pointGrip1, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnPosition1Changed(const QPointF&)));
	connect(&m_pointGrip2, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnPosition2Changed(const QPointF&)));

	setFlags(ItemIsMovable | ItemIsSelectable);
	setCursor(QCursor(Qt::ArrowCursor)); 
	
	SetPen(InactiveColor, QPen(Qt::green, 0));
	SetPen(EditableColor, QPen(Qt::green, 0));
	SetPen(SelectedColor, QPen(Qt::yellow, 0));

	SwitchColorSheme(EditableColor);
}


// reimplemented (imod::IObserver)

void CLine2dShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		setLine(iqt::GetQLineF(*linePtr));

		UpdateGripPositions();
	}
}


// protected slots

void CLine2dShape::OnPosition1Changed(const QPointF& point1)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		linePtr->SetPoint1(iqt::GetCVector2d(point1));
	}
}


void CLine2dShape::OnPosition2Changed(const QPointF& point2)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		linePtr->SetPoint2(iqt::GetCVector2d(point2));
	}
}


// protected methods

// reimplemented (QGraphicsItem) 

QVariant CLine2dShape::itemChange(GraphicsItemChange change, const QVariant& value)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL && change == ItemPositionChange){
		QPointF newPoint = value.toPointF();

		i2d::CVector2d offset = iqt::GetCVector2d(newPoint - m_lastPosition);

		linePtr->SetPoint1(linePtr->GetPoint1() + offset);
		linePtr->SetPoint2(linePtr->GetPoint2() + offset);

		m_lastPosition = newPoint;

		return pos();
	}

	return BaseClass::itemChange(change, value);
}


void CLine2dShape::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	BaseClass::mouseReleaseEvent(event);

	m_lastPosition = QPointF(0, 0);
}


// private methods

void CLine2dShape::UpdateGripPositions()
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		iqt::CSignalBlocker block(&m_pointGrip1);
		m_pointGrip1.setPos(iqt::GetQPointF(linePtr->GetPoint1()));
		iqt::CSignalBlocker block2(&m_pointGrip2);
		m_pointGrip2.setPos(iqt::GetQPointF(linePtr->GetPoint2()));
	}
}


} // namespace iqt2d


