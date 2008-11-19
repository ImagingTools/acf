#ifndef iqt2d_CCircleShape_included
#define iqt2d_CCircleShape_included


#include "i2d/CCircle.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CCircleShape: public TObjectShapeBase<QGraphicsEllipseItem, i2d::CCircle>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsEllipseItem, i2d::CCircle> BaseClass;

	CCircleShape(bool isEditable = false);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected slots:
	virtual void OnGripPositionChanged(const QPointF& point);

private:
	void UpdateGripPositions();

private:
	CGripShape m_topGrip;
	CGripShape m_bottomGrip;
	CGripShape m_leftGrip;
	CGripShape m_rightGrip;
};


} // namespace iqt2d


#endif // !iqt2d_CCircleShape_included


