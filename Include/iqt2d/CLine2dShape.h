#ifndef iqt2d_CLine2dShape_included
#define iqt2d_CLine2dShape_included


#include "i2d/CLine2d.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CLine2dShape: public TObjectShapeBase<QGraphicsLineItem>
{
	Q_OBJECT
public:
	typedef TObjectShapeBase<QGraphicsLineItem> BaseClass;

	CLine2dShape(bool isEditable = false);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected slots:
	virtual void OnPosition1Changed(const QPointF& point1);
	virtual void OnPosition2Changed(const QPointF& point2);

private:
	void UpdateGripPositions();

private:
	CGripShape m_pointGrip1;
	CGripShape m_pointGrip2;
};


} // namespace iqt2d


#endif // !iqt2d_CLine2dShape_included


