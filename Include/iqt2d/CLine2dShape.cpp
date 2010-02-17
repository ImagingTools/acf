#include "iqt2d/CLine2dShape.h"


#include <QPen>
#include <QBrush>
#include <QCursor>

#include "istd/TChangeNotifier.h"


namespace iqt2d
{


// public methods

CLine2dShape::CLine2dShape(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr),
	m_pointGrip1(this, providerPtr),
	m_pointGrip2(this, providerPtr)
{
	connect(&m_pointGrip1, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnPosition1Changed(const i2d::CVector2d&)));
	connect(&m_pointGrip2, SIGNAL(PositionChanged(const i2d::CVector2d&)), this, SLOT(OnPosition2Changed(const i2d::CVector2d&)));

	if (!isEditable){
		m_pointGrip1.setParentItem(NULL);
		m_pointGrip2.setParentItem(NULL);
	}
}


// protected slots

void CLine2dShape::OnPosition1Changed(const i2d::CVector2d& point)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		linePtr->SetPoint1(point);
	}
}


void CLine2dShape::OnPosition2Changed(const i2d::CVector2d& point)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		linePtr->SetPoint2(point);
	}
}


// protected methods

// reimplemented (iqt2d::TObjectShapeBase)

void CLine2dShape::UpdateGraphicsItem(const i2d::CLine2d& line)
{
	m_pointGrip1.SetPosition(line.GetPoint1());
	m_pointGrip2.SetPosition(line.GetPoint2());

	setLine(QLineF(GetLocalFromPos(line.GetPoint1()), GetLocalFromPos(line.GetPoint2())));
}


} // namespace iqt2d


