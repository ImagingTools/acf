#include "iqt2d/CAnnulusSegmentShape.h"


#include "istd/TChangeNotifier.h"

#include "i2d/CAnnulusSegment.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CAnnulusSegmentShape::CAnnulusSegmentShape(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr),
	m_beginAngleGrip(this),
	m_endAngleGrip(this)
{
	connect(&m_beginAngleGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnAngleGripPositionChanged(const QPointF&)));
	connect(&m_endAngleGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnAngleGripPositionChanged(const QPointF&)));
}


// protected slots

void CAnnulusSegmentShape::OnAngleGripPositionChanged(const QPointF& /*point*/)
{
	i2d::CAnnulusSegment* annulusPtr = dynamic_cast<i2d::CAnnulusSegment*>(GetObjectPtr());
	if (annulusPtr != NULL){
		double beginAngle = (iqt::GetCVector2d(m_beginAngleGrip.pos()) - annulusPtr->GetCenter()).GetAngle();
		double endAngle = (iqt::GetCVector2d(m_endAngleGrip.pos()) - annulusPtr->GetCenter()).GetAngle();

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

void CAnnulusSegmentShape::UpdateGripPositions()
{
	BaseClass::UpdateGripPositions();

	i2d::CAnnulusSegment* annulusPtr = dynamic_cast<i2d::CAnnulusSegment*>(GetObjectPtr());
	if (annulusPtr != NULL){
		double middleRadius = (annulusPtr->GetInnerRadius() + annulusPtr->GetOuterRadius()) * 0.5;

		QPointF center = iqt::GetQPointF(annulusPtr->GetCenter());

		i2d::CVector2d beginAnglePos;
		beginAnglePos.Init(annulusPtr->GetBeginAngle(), middleRadius);

		iqt::CSignalBlocker block1(&m_beginAngleGrip);
		m_beginAngleGrip.setPos(center + iqt::GetQPointF(beginAnglePos));

		i2d::CVector2d endAnglePos;
		endAnglePos.Init(annulusPtr->GetEndAngle(), middleRadius);

		iqt::CSignalBlocker block2(&m_endAngleGrip);
		m_endAngleGrip.setPos(center + iqt::GetQPointF(endAnglePos));
	}
}


void CAnnulusSegmentShape::CalcVisualization(QPainterPath& result)
{
	i2d::CAnnulusSegment* annulusPtr = dynamic_cast<i2d::CAnnulusSegment*>(GetObjectPtr());
	if (annulusPtr != NULL){
		double beginAngle = annulusPtr->GetBeginAngle();
		double endAngle = annulusPtr->GetEndAngle();
		double endLength = endAngle - beginAngle;

		result.moveTo(iqt::GetQPointF(annulusPtr->GetCenter()));
		if ((endLength >= 0) && (endLength < 2 * I_PI)){
			result.arcTo(iqt::GetQRectF(annulusPtr->GetOuterCircle().GetBoundingBox()),
						-imath::GetDegreeFromRadian(beginAngle),
						-imath::GetDegreeFromRadian(endLength));
			result.arcTo(iqt::GetQRectF(annulusPtr->GetInnerCircle().GetBoundingBox()),
						-imath::GetDegreeFromRadian(endAngle),
						imath::GetDegreeFromRadian(endLength));
		}
		result.lineTo(iqt::GetQPointF(annulusPtr->GetCenter()));

		result.addEllipse(iqt::GetQRectF(annulusPtr->GetInnerCircle().GetBoundingBox()));
		if (endLength < 2 * I_PI){
			result.addEllipse(iqt::GetQRectF(annulusPtr->GetInnerCircle().GetBoundingBox()));
			result.addEllipse(iqt::GetQRectF(annulusPtr->GetOuterCircle().GetBoundingBox()));
		}
		result.addEllipse(iqt::GetQRectF(annulusPtr->GetOuterCircle().GetBoundingBox()));
	}
}


} // namespace iqt2d


