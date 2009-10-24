#include "iqt2d/CRectangleShape.h"


#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


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
	connect(&m_topLeftGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnTopLeftChanged(const QPointF&)));
	connect(&m_topRightGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnTopRightChanged(const QPointF&)));
	connect(&m_bottomLeftGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnBottomLeftChanged(const QPointF&)));
	connect(&m_bottomRightGrip, SIGNAL(PositionChanged(const QPointF&)), this, SLOT(OnBottomRightChanged(const QPointF&)));
	
	m_bottomLeftGrip.SetLabelPosition(CGripShape::LabelBottom);
	m_bottomRightGrip.SetLabelPosition(CGripShape::LabelBottom);

	if (!isEditable){
		m_topLeftGrip.setParentItem(NULL);
		m_topRightGrip.setParentItem(NULL);
		m_bottomLeftGrip.setParentItem(NULL);
		m_bottomRightGrip.setParentItem(NULL);
	}
}


// reimplemented (imod::IObserver)

void CRectangleShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		setRect(iqt::GetQRectF(*rectPtr));

		UpdateGripPositions();
	}
}


// protected slots

void CRectangleShape::OnTopLeftChanged(const QPointF& point)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		rectPtr->SetTopLeft(iqt::GetCVector2d(point));
	}

}


void CRectangleShape::OnTopRightChanged(const QPointF& point)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		rectPtr->SetTopRight(iqt::GetCVector2d(point));
	}
}


void CRectangleShape::OnBottomLeftChanged(const QPointF& point)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		rectPtr->SetBottomLeft(iqt::GetCVector2d(point));
	}
}


void CRectangleShape::OnBottomRightChanged(const QPointF& point)
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		rectPtr->SetBottomRight(iqt::GetCVector2d(point));
	}
}


// private methods

void CRectangleShape::UpdateGripPositions()
{
	i2d::CRectangle* rectPtr = GetObjectPtr();
	if (rectPtr != NULL){
		iqt::CSignalBlocker block(&m_topLeftGrip);
		m_topLeftGrip.setPos(iqt::GetQPointF(rectPtr->GetTopLeft()));

		iqt::CSignalBlocker block2(&m_topRightGrip);
		m_topRightGrip.setPos(iqt::GetQPointF(rectPtr->GetTopRight()));

		iqt::CSignalBlocker block3(&m_bottomLeftGrip);
		m_bottomLeftGrip.setPos(iqt::GetQPointF(rectPtr->GetBottomLeft()));

		iqt::CSignalBlocker block4(&m_bottomRightGrip);
		m_bottomRightGrip.setPos(iqt::GetQPointF(rectPtr->GetBottomRight()));
	}
}


} // namespace iqt2d


