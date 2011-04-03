#include "iqt2d/CRectangleShape.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqt2d
{


// public methods

CRectangleShape::CRectangleShape(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr),
	m_topLeftGrip(this, providerPtr),
	m_topRightGrip(this, providerPtr),
	m_bottomLeftGrip(this, providerPtr),
	m_bottomRightGrip(this, providerPtr)
{
	connect(&m_topLeftGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnTopLeftChanged(const i2d::CVector2d&)));
	connect(&m_topRightGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnTopRightChanged(const i2d::CVector2d&)));
	connect(&m_bottomLeftGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnBottomLeftChanged(const i2d::CVector2d&)));
	connect(&m_bottomRightGrip, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnBottomRightChanged(const i2d::CVector2d&)));
	
	m_bottomLeftGrip.SetLabelPosition(CGripShape::LabelBottom);
	m_bottomRightGrip.SetLabelPosition(CGripShape::LabelBottom);

	if (!isEditable){
		m_topLeftGrip.setParentItem(NULL);
		m_topRightGrip.setParentItem(NULL);
		m_bottomLeftGrip.setParentItem(NULL);
		m_bottomRightGrip.setParentItem(NULL);
	}
}


// protected slots

void CRectangleShape::OnTopLeftChanged(const i2d::CVector2d& point)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		rectPtr->SetTopLeft(point);
	}

}


void CRectangleShape::OnTopRightChanged(const i2d::CVector2d& point)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		rectPtr->SetTopRight(point);
	}
}


void CRectangleShape::OnBottomLeftChanged(const i2d::CVector2d& point)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		rectPtr->SetBottomLeft(point);
	}
}


void CRectangleShape::OnBottomRightChanged(const i2d::CVector2d& point)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		rectPtr->SetBottomRight(point);
	}
}


// protected methods

// reimplemented (iqt2d::TObjectShapeBase)

void CRectangleShape::UpdateGraphicsItem(const i2d::CRectangle& rect)
{
	m_topLeftGrip.SetPosition(rect.GetTopLeft());
	m_topRightGrip.SetPosition(rect.GetTopRight());
	m_bottomLeftGrip.SetPosition(rect.GetBottomLeft());
	m_bottomRightGrip.SetPosition(rect.GetBottomRight());

	QPainterPath path;

	path.moveTo(GetLocalFromPos(rect.GetTopLeft()));
	path.lineTo(GetLocalFromPos(rect.GetTopRight()));
	path.lineTo(GetLocalFromPos(rect.GetBottomRight()));
	path.lineTo(GetLocalFromPos(rect.GetBottomLeft()));
	path.lineTo(GetLocalFromPos(rect.GetTopLeft()));

	setPath(path);
}


} // namespace iqt2d


