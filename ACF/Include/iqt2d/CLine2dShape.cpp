#include "iqt2d/CLine2dShape.h"


#include <QPen>
#include <QBrush>

#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CLine2dShape::CLine2dShape()
:	m_pointGrip1(this),
	m_pointGrip2(this),
	m_ignoreUpdate(false)
{
	connect(&m_pointGrip1, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnPosition1Changed(const QPointF&)));
	connect(&m_pointGrip2, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnPosition2Changed(const QPointF&)));

	setFlags(ItemIsMovable | ItemIsSelectable);
}


// reimplemented (imod::IObserver)

void CLine2dShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL && !m_ignoreUpdate){
		setPen(QPen(Qt::yellow, 0));

		QLineF line = iqt::GetQLineF(*linePtr);
		setLine(QLineF((line.p1()), (line.p2())));

		iqt::CSignalBlocker block(&m_pointGrip1);
		m_pointGrip1.setPos((iqt::GetQPointF(linePtr->GetPoint1())));
		iqt::CSignalBlocker block2(&m_pointGrip2);
		m_pointGrip2.setPos((iqt::GetQPointF(linePtr->GetPoint2())));
	}
}


// protected slots

void CLine2dShape::OnPosition1Changed(const QPointF& point1)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		linePtr->SetPoint1(iqt::GetCVector2d(((point1))));
	}
}


void CLine2dShape::OnPosition2Changed(const QPointF& point2)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		linePtr->SetPoint2(iqt::GetCVector2d(((point2))));
	}
}


// protected methods

// reimplemented (QGraphicsItem) 

QVariant CLine2dShape::itemChange(GraphicsItemChange change, const QVariant& value)
{
	m_ignoreUpdate = true;
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL && change == ItemPositionChange){
		i2d::CVector2d offset = iqt::GetCVector2d(value.toPointF() - pos());

		linePtr->SetPoint1(linePtr->GetPoint1() + offset);
		linePtr->SetPoint2(linePtr->GetPoint2() + offset);
	}
	if (change == QGraphicsItem::ItemSelectedChange){
		bool isSelected = value.toBool();
		// TODO: set the visual properties:
		if (isSelected){
		}
		else{
		}
	}

	m_ignoreUpdate = false;

	return BaseClass::itemChange(change, value);
}


} // namespace iqt2d


