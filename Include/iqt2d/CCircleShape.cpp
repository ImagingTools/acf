#include "iqt2d/CCircleShape.h"


// Qt includes
#include <QPen>
#include <QBrush>
#include <QCursor>


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqt2d
{


// public methods

CCircleShape::CCircleShape(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr),
	m_leftGrip(this, providerPtr),
	m_rightGrip(this, providerPtr),
	m_topGrip(this, providerPtr),
	m_bottomGrip(this, providerPtr)
{
	connect(&m_leftGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_rightGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_topGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_bottomGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnGripPositionChanged(const i2d::CVector2d&)));

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


// protected slots

void CCircleShape::OnGripPositionChanged(const i2d::CVector2d& point)
{
	i2d::CCircle* circlePtr = GetObjectPtr();
	if (circlePtr != NULL){
		circlePtr->SetRadius(point.GetDistance(circlePtr->GetCenter()));
	}
}


// protected methods

// reimplemented (iqt2d::TObjectShapeBase)

void CCircleShape::UpdateGraphicsItem(const i2d::CCircle& circle)
{
	const i2d::CVector2d& center = circle.GetCenter();
	double radius = circle.GetRadius();

	m_leftGrip.SetPosition(center.GetHorizontalTranslated(-radius));
	m_rightGrip.SetPosition(center.GetHorizontalTranslated(radius));
	m_topGrip.SetPosition(center.GetVerticalTranslated(-radius));
	m_bottomGrip.SetPosition(center.GetVerticalTranslated(radius));

	QPointF relativeCenter = GetLocalFromPos(center);
	QRectF relativeRect(relativeCenter.x() - radius, relativeCenter.y() - radius, radius * 2, radius * 2);

	setRect(relativeRect);
}


} // namespace iqt2d


