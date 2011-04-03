#include "iqt2d/CAnnulusSegmentShape.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "i2d/CAnnulusSegment.h"


namespace iqt2d
{


// public methods

CAnnulusSegmentShape::CAnnulusSegmentShape(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr),
	m_beginAngleGrip(this),
	m_endAngleGrip(this)
{
	connect(&m_beginAngleGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnAngleGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_endAngleGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnAngleGripPositionChanged(const i2d::CVector2d&)));
}


// protected slots

void CAnnulusSegmentShape::OnAngleGripPositionChanged(const i2d::CVector2d& /*point*/)
{
	i2d::CAnnulusSegment* annulusPtr = dynamic_cast<i2d::CAnnulusSegment*>(GetObjectPtr());
	if (annulusPtr != NULL){
		double beginAngle = (GetPosFromLocal(m_beginAngleGrip.pos()) - annulusPtr->GetCenter()).GetAngle();
		double endAngle = (GetPosFromLocal(m_endAngleGrip.pos()) - annulusPtr->GetCenter()).GetAngle();

		if (endAngle < beginAngle){
			endAngle += 2 * I_PI;
		}

		istd::CChangeNotifier notifier(annulusPtr);
		annulusPtr->SetBeginAngle(beginAngle);
		annulusPtr->SetEndAngle(endAngle);
	}
}


// protected methods

// reimplemented (iqt2d::CAnnulusShape)

void CAnnulusSegmentShape::CalcVisualization(const i2d::CAnnulus& annulus)
{
	i2d::CAnnulusSegment* segmentPtr = dynamic_cast<i2d::CAnnulusSegment*>(GetObjectPtr());
	if (segmentPtr != NULL){
		double radius1 = annulus.GetInnerRadius();
		double radius2 = annulus.GetOuterRadius();

		QPointF relativeCenter = GetLocalFromPos(annulus.GetCenter());

		double beginAngle = segmentPtr->GetBeginAngle();
		double endAngle = segmentPtr->GetEndAngle();
		double endLength = endAngle - beginAngle;

		QRectF rect1(relativeCenter.x() - radius1, relativeCenter.y() - radius1, radius1 * 2, radius1 * 2);
		QRectF rect2(relativeCenter.x() - radius2, relativeCenter.y() - radius2, radius2 * 2, radius2 * 2);

		QPainterPath path;

		path.moveTo(relativeCenter);
		if ((endLength >= 0) && (endLength < 2 * I_PI)){
			path.arcTo(rect2,
						-imath::GetDegreeFromRadian(beginAngle),
						-imath::GetDegreeFromRadian(endLength));
			path.arcTo(rect1,
						-imath::GetDegreeFromRadian(endAngle),
						imath::GetDegreeFromRadian(endLength));
		}
		path.lineTo(relativeCenter);

		path.addEllipse(rect1);
		if (endLength < 2 * I_PI){
			path.addEllipse(rect1);
			path.addEllipse(rect2);
		}
		path.addEllipse(rect2);

		setPath(path);
	}
	else{
		BaseClass::CalcVisualization(annulus);
	}
}


// reimplemented (iqt2d::TObjectShapeBase)

void CAnnulusSegmentShape::UpdateGraphicsItem(const i2d::CAnnulus& annulus)
{
	BaseClass::UpdateGraphicsItem(annulus);

	i2d::CAnnulusSegment* annulusSegmentPtr = dynamic_cast<i2d::CAnnulusSegment*>(GetObjectPtr());
	if (annulusSegmentPtr != NULL){
		const i2d::CVector2d& center = annulus.GetCenter();
		double middleRadius = (annulus.GetInnerRadius() + annulus.GetOuterRadius()) * 0.5;

		i2d::CVector2d beginAnglePos;
		beginAnglePos.Init(annulusSegmentPtr->GetBeginAngle(), middleRadius);
		m_beginAngleGrip.SetPosition(center + beginAnglePos);

		i2d::CVector2d endAnglePos;
		endAnglePos.Init(annulusSegmentPtr->GetEndAngle(), middleRadius);
		m_endAngleGrip.SetPosition(center + endAnglePos);
	}
}


} // namespace iqt2d


