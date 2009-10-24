#ifndef iqt2d_CRectangleShape_included
#define iqt2d_CRectangleShape_included


#include <QGraphicsRectItem>

#include "i2d/CRectangle.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CRectangleShape: public TObjectShapeBase<QGraphicsRectItem, i2d::CRectangle>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsRectItem, i2d::CRectangle> BaseClass;

	CRectangleShape(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

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


