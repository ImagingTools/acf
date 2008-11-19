#include "iqt2d/CCircleShape.h"


#include <QPen>
#include <QBrush>
#include <QCursor>

#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CCircleShape::CCircleShape(bool isEditable)
:	BaseClass(isEditable),
	m_leftGrip(this),
	m_rightGrip(this),
	m_topGrip(this),
	m_bottomGrip(this)
{
	connect(&m_leftGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnGripPositionChanged(const QPointF&)));
	connect(&m_rightGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnGripPositionChanged(const QPointF&)));
	connect(&m_topGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnGripPositionChanged(const QPointF&)));
	connect(&m_bottomGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnGripPositionChanged(const QPointF&)));

	m_leftGrip.SetLabelPosition(CGripShape::LabelLeft);
	m_rightGrip.SetLabelPosition(CGripShape::LabelRight);
	m_topGrip.SetLabelPosition(CGripShape::LabelTop);
	m_bottomGrip.SetLabelPosition(CGripShape::LabelBottom);

	if (!isEditable){
		m_leftGrip.setParentItem(NULL);
		m_rightGrip.setParentItem(NULL);
		m_topGrip.setParentItem(NULL);
		m_bottomGrip.setParentItem(NULL);
	}
}


// reimplemented (imod::IObserver)

void CCircleShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	i2d::CCircle* circlePtr = GetObjectPtr();
	if (circlePtr != NULL){
		setRect(iqt::GetQRectF(circlePtr->GetBoundingBox()));

		UpdateGripPositions();
	}
}


// protected slots

void CCircleShape::OnGripPositionChanged(const QPointF& point)
{
	i2d::CCircle* circlePtr = GetObjectPtr();
	if (circlePtr != NULL){
		circlePtr->SetRadius(iqt::GetCVector2d(point).GetDistance(circlePtr->GetCenter()));
	}
}


// private methods

void CCircleShape::UpdateGripPositions()
{
	i2d::CCircle* circlePtr = GetObjectPtr();
	if (circlePtr != NULL){
		QPointF center = iqt::GetQPointF(circlePtr->GetCenter());

		iqt::CSignalBlocker block(&m_leftGrip);
		m_leftGrip.setPos(center.x() - circlePtr->GetRadius(), center.y());

		iqt::CSignalBlocker block2(&m_rightGrip);
		m_rightGrip.setPos(center.x() + circlePtr->GetRadius(), center.y());

		iqt::CSignalBlocker block3(&m_topGrip);
		m_topGrip.setPos(center.x(), center.y() - circlePtr->GetRadius());

		iqt::CSignalBlocker block4(&m_bottomGrip);
		m_bottomGrip.setPos(center.x(), center.y() + circlePtr->GetRadius());
	}
}


} // namespace iqt2d


