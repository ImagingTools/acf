#include "iqt2d/CLine2dShape.h"


#include <QPen>
#include <QBrush>
#include <QCursor>

#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CLine2dShape::CLine2dShape(bool isEditable)
:	BaseClass(isEditable),
	m_pointGrip1(this),
	m_pointGrip2(this)
{
	connect(&m_pointGrip1, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnPosition1Changed(const QPointF&)));
	connect(&m_pointGrip2, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnPosition2Changed(const QPointF&)));
	
	SetPen(InactiveColor, QPen(Qt::darkGreen, 0));
	SetPen(EditableColor, QPen(Qt::green, 0));
	SetPen(SelectedColor, QPen(Qt::yellow, 0));

	if (isEditable){
		SwitchColorSheme(EditableColor);
	}
	else{
		SwitchColorSheme(InactiveColor);
	}

	if (!isEditable){
		m_pointGrip1.setParentItem(NULL);
		m_pointGrip2.setParentItem(NULL);
	}
}


// reimplemented (imod::IObserver)

void CLine2dShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObjectPtr());
	if (linePtr != NULL){
		setLine(iqt::GetQLineF(*linePtr));

		UpdateGripPositions();
	}
}


// protected slots

void CLine2dShape::OnPosition1Changed(const QPointF& point1)
{
	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObjectPtr());
	if (linePtr != NULL){
		linePtr->SetPoint1(iqt::GetCVector2d(point1));
	}
}


void CLine2dShape::OnPosition2Changed(const QPointF& point2)
{
	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObjectPtr());
	if (linePtr != NULL){
		linePtr->SetPoint2(iqt::GetCVector2d(point2));
	}
}


// private methods

void CLine2dShape::UpdateGripPositions()
{
	i2d::CLine2d* linePtr = dynamic_cast<i2d::CLine2d*>(GetObjectPtr());
	if (linePtr != NULL){
		iqt::CSignalBlocker block(&m_pointGrip1);
		m_pointGrip1.setPos(iqt::GetQPointF(linePtr->GetPoint1()));
		iqt::CSignalBlocker block2(&m_pointGrip2);
		m_pointGrip2.setPos(iqt::GetQPointF(linePtr->GetPoint2()));
	}
}


} // namespace iqt2d


