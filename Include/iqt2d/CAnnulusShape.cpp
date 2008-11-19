#include "iqt2d/CAnnulusShape.h"


// Qt includes
#include <QBrush>

#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CAnnulusShape::CAnnulusShape(bool isEditable)
	:BaseClass(isEditable),
	m_leftInnerGrip(this),
	m_rightInnerGrip(this),
	m_topInnerGrip(this),
	m_bottomInnerGrip(this),
	m_leftOuterGrip(this),
	m_rightOuterGrip(this),
	m_topOuterGrip(this),
	m_bottomOuterGrip(this)
{
	connect(&m_leftInnerGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnInnerGripPositionChanged(const QPointF&)));
	connect(&m_rightInnerGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnInnerGripPositionChanged(const QPointF&)));
	connect(&m_topInnerGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnInnerGripPositionChanged(const QPointF&)));
	connect(&m_bottomInnerGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnInnerGripPositionChanged(const QPointF&)));

	connect(&m_leftOuterGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnOuterGripPositionChanged(const QPointF&)));
	connect(&m_rightOuterGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnOuterGripPositionChanged(const QPointF&)));
	connect(&m_topOuterGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnOuterGripPositionChanged(const QPointF&)));
	connect(&m_bottomOuterGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnOuterGripPositionChanged(const QPointF&)));

	m_leftOuterGrip.SetLabelPosition(CGripShape::LabelLeft);
	m_rightOuterGrip.SetLabelPosition(CGripShape::LabelRight);
	m_topOuterGrip.SetLabelPosition(CGripShape::LabelTop);
	m_bottomOuterGrip.SetLabelPosition(CGripShape::LabelBottom);

	m_leftInnerGrip.SetLabelPosition(CGripShape::LabelLeft);
	m_rightInnerGrip.SetLabelPosition(CGripShape::LabelRight);
	m_topInnerGrip.SetLabelPosition(CGripShape::LabelTop);
	m_bottomInnerGrip.SetLabelPosition(CGripShape::LabelBottom);

	SetBrush(InactiveColor, QBrush(QColor(10, 126, 242, 32)));
	SetBrush(EditableColor, QBrush(QColor(10, 126, 242, 32)));
	SetBrush(SelectedColor, QBrush(QColor(255, 255, 0, 32)));

	SwitchColorSheme(InactiveColor);
}


// reimplemented (imod::IObserver)

void CAnnulusShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	QPainterPath path;

	CalcVisualization(path);

	setPath(path);

	UpdateGripPositions();
}


// protected slots

void CAnnulusShape::OnInnerGripPositionChanged(const QPointF& point)
{
	i2d::CAnnulus* annulusPtr = GetObjectPtr();
	if (annulusPtr != NULL){
		annulusPtr->SetInnerRadius(iqt::GetCVector2d(point).GetDistance(annulusPtr->GetCenter()));
	}
}


void CAnnulusShape::OnOuterGripPositionChanged(const QPointF& point)
{
	i2d::CAnnulus* annulusPtr = GetObjectPtr();
	if (annulusPtr != NULL){
		annulusPtr->SetOuterRadius(iqt::GetCVector2d(point).GetDistance(annulusPtr->GetCenter()));
	}
}


// protected methods

void CAnnulusShape::UpdateGripPositions()
{
	i2d::CAnnulus* annulusPtr = GetObjectPtr();
	if (annulusPtr != NULL){
		QPointF center = iqt::GetQPointF(annulusPtr->GetCenter());

		iqt::CSignalBlocker block(&m_leftInnerGrip);
		m_leftInnerGrip.setPos(center.x() - annulusPtr->GetInnerRadius(), center.y());

		iqt::CSignalBlocker block2(&m_rightInnerGrip);
		m_rightInnerGrip.setPos(center.x() + annulusPtr->GetInnerRadius(), center.y());

		iqt::CSignalBlocker block3(&m_topInnerGrip);
		m_topInnerGrip.setPos(center.x(), center.y() - annulusPtr->GetInnerRadius());

		iqt::CSignalBlocker block4(&m_bottomInnerGrip);
		m_bottomInnerGrip.setPos(center.x(), center.y() + annulusPtr->GetInnerRadius());

		iqt::CSignalBlocker block5(&m_leftOuterGrip);
		m_leftOuterGrip.setPos(center.x() - annulusPtr->GetOuterRadius(), center.y());

		iqt::CSignalBlocker block6(&m_rightOuterGrip);
		m_rightOuterGrip.setPos(center.x() + annulusPtr->GetOuterRadius(), center.y());

		iqt::CSignalBlocker block7(&m_topOuterGrip);
		m_topOuterGrip.setPos(center.x(), center.y() - annulusPtr->GetOuterRadius());

		iqt::CSignalBlocker block8(&m_bottomOuterGrip);
		m_bottomOuterGrip.setPos(center.x(), center.y() + annulusPtr->GetOuterRadius());
	}
}


void CAnnulusShape::CalcVisualization(QPainterPath& result)
{
	i2d::CAnnulus* annulusPtr = GetObjectPtr();
	if (annulusPtr != NULL){
		result.moveTo(iqt::GetQPointF(annulusPtr->GetCenter()));
		result.addEllipse(iqt::GetQRectF(annulusPtr->GetInnerCircle().GetBoundingBox()));
		result.addEllipse(iqt::GetQRectF(annulusPtr->GetOuterCircle().GetBoundingBox()));
	}
}


} // namespace iqt2d


