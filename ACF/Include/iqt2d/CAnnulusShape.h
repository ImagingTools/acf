#ifndef iqt2d_CAnnulusShape_included
#define iqt2d_CAnnulusShape_included


#include "i2d/CAnnulus.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CAnnulusShape: public TObjectShapeBase<QGraphicsItemGroup>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsItemGroup> BaseClass;

	CAnnulusShape(bool isEditable = false);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected slots:
	virtual void OnInnerGripPositionChanged(const QPointF& point);
	virtual void OnOuterGripPositionChanged(const QPointF& point);

private:
	void UpdateGripPositions();

private:
	CGripShape m_topInnerGrip;
	CGripShape m_bottomInnerGrip;
	CGripShape m_leftInnerGrip;
	CGripShape m_rightInnerGrip;
	CGripShape m_topOuterGrip;
	CGripShape m_bottomOuterGrip;
	CGripShape m_leftOuterGrip;
	CGripShape m_rightOuterGrip;

	TShapeBase<QGraphicsEllipseItem> m_innerCircle;
	TShapeBase<QGraphicsEllipseItem> m_outerCircle;
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusShape_included


