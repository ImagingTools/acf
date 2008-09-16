#ifndef iqt2d_CRectangleShape_included
#define iqt2d_CRectangleShape_included


#include <QGraphicsRectItem>

#include "i2d/CRectangle.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CRectangleShape: public TObjectShapeBase<QGraphicsRectItem>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsRectItem> BaseClass;

	CRectangleShape(bool isEditable = false);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected slots:
	virtual void OnTopLeftChanged(const QPointF& point);
	virtual void OnTopRightChanged(const QPointF& point);
	virtual void OnBottomLeftChanged(const QPointF& point);
	virtual void OnBottomRightChanged(const QPointF& point);

private:
	void UpdateGripPositions();

private:
	CGripShape m_topLeftGrip;
	CGripShape m_topRightGrip;
	CGripShape m_bottomLeftGrip;
	CGripShape m_bottomRightGrip;
};


} // namespace iqt2d


#endif // !iqt2d_CRectangleShape_included


