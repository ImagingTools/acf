#include "iqt2d/CAnnulusShape.h"


// Qt includes
#include <QBrush>


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqt2d
{


// public methods

CAnnulusShape::CAnnulusShape(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr),
	m_leftInnerGrip(this, providerPtr),
	m_rightInnerGrip(this, providerPtr),
	m_topInnerGrip(this, providerPtr),
	m_bottomInnerGrip(this, providerPtr),
	m_leftOuterGrip(this, providerPtr),
	m_rightOuterGrip(this, providerPtr),
	m_topOuterGrip(this, providerPtr),
	m_bottomOuterGrip(this, providerPtr)
{
	connect(&m_leftInnerGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnInnerGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_rightInnerGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnInnerGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_topInnerGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnInnerGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_bottomInnerGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnInnerGripPositionChanged(const i2d::CVector2d&)));

	connect(&m_leftOuterGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnOuterGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_rightOuterGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnOuterGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_topOuterGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnOuterGripPositionChanged(const i2d::CVector2d&)));
	connect(&m_bottomOuterGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnOuterGripPositionChanged(const i2d::CVector2d&)));

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

	if (!isEditable){
		m_leftOuterGrip.setParentItem(NULL);
		m_rightOuterGrip.setParentItem(NULL);
		m_topOuterGrip.setParentItem(NULL);
		m_bottomOuterGrip.setParentItem(NULL);
		m_leftInnerGrip.setParentItem(NULL);
		m_rightInnerGrip.setParentItem(NULL);
		m_topInnerGrip.setParentItem(NULL);
		m_bottomInnerGrip.setParentItem(NULL);
	}
}


// protected slots

void CAnnulusShape::OnInnerGripPositionChanged(const i2d::CVector2d& point)
{
	i2d::CAnnulus* annulusPtr = GetObjectPtr();
	if (annulusPtr != NULL){
		annulusPtr->SetInnerRadius(point.GetDistance(annulusPtr->GetCenter()));
	}
}


void CAnnulusShape::OnOuterGripPositionChanged(const i2d::CVector2d& point)
{
	i2d::CAnnulus* annulusPtr = GetObjectPtr();
	if (annulusPtr != NULL){
		annulusPtr->SetOuterRadius(point.GetDistance(annulusPtr->GetCenter()));
	}
}


// protected methods

void CAnnulusShape::CalcVisualization(const i2d::CAnnulus& annulus)
{
	QPainterPath path;

	double radius1 = annulus.GetInnerRadius();
	double radius2 = annulus.GetOuterRadius();

	QPointF relativeCenter = GetLocalFromPos(annulus.GetCenter());

	path.moveTo(relativeCenter);
	path.addEllipse(relativeCenter, radius1, radius1);
	path.addEllipse(relativeCenter, radius2, radius2);

	setPath(path);
}


// reimplemented (iqt2d::TObjectShapeBase)

void CAnnulusShape::UpdateGraphicsItem(const i2d::CAnnulus& annulus)
{
	const i2d::CVector2d& center = annulus.GetCenter();
	double radius1 = annulus.GetInnerRadius();
	double radius2 = annulus.GetOuterRadius();

	m_leftInnerGrip.SetPosition(center.GetHorizontalTranslated(-radius1));
	m_rightInnerGrip.SetPosition(center.GetHorizontalTranslated(radius1));
	m_topInnerGrip.SetPosition(center.GetVerticalTranslated(-radius1));
	m_bottomInnerGrip.SetPosition(center.GetVerticalTranslated(radius1));
	m_leftOuterGrip.SetPosition(center.GetHorizontalTranslated(-radius2));
	m_rightOuterGrip.SetPosition(center.GetHorizontalTranslated(radius2));
	m_topOuterGrip.SetPosition(center.GetVerticalTranslated(-radius2));
	m_bottomOuterGrip.SetPosition(center.GetVerticalTranslated(radius2));

	CalcVisualization(annulus);
}


} // namespace iqt2d


