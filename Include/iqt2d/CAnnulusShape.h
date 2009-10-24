#ifndef iqt2d_CAnnulusShape_included
#define iqt2d_CAnnulusShape_included


#include "i2d/CAnnulus.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CAnnulusShape: public TObjectShapeBase<QGraphicsPathItem, i2d::CAnnulus>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsPathItem, i2d::CAnnulus> BaseClass;

	CAnnulusShape(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected slots:
	virtual void OnInnerGripPositionChanged(const QPointF& point);
	virtual void OnOuterGripPositionChanged(const QPointF& point);

protected:
	virtual void UpdateGripPositions();
	virtual void CalcVisualization(QPainterPath& result);

private:
	CGripShape m_leftInnerGrip;
	CGripShape m_rightInnerGrip;
	CGripShape m_topInnerGrip;
	CGripShape m_bottomInnerGrip;
	CGripShape m_leftOuterGrip;
	CGripShape m_rightOuterGrip;
	CGripShape m_topOuterGrip;
	CGripShape m_bottomOuterGrip;
};


} // namespace iqt2d


#endif // !iqt2d_CAnnulusShape_included


