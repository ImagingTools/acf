#ifndef iqt2d_CLine2dShape_included
#define iqt2d_CLine2dShape_included


#include "imod/TSingleModelObserverBase.h"

#include "i2d/CLine2d.h"

#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CLine2dShape: 
			public QObject, 
			public QGraphicsLineItem, 
			public imod::TSingleModelObserverBase<i2d::CLine2d>
{
	Q_OBJECT
public:
	typedef QGraphicsLineItem BaseClass;
	typedef imod::TSingleModelObserverBase<i2d::CLine2d> BaseClass2;

	CLine2dShape();

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected slots:
	virtual void OnPosition1Changed(const QPointF& point1);
	virtual void OnPosition2Changed(const QPointF& point2);

protected:
	// reimplemented (QGraphicsItem) 
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
	CGripShape m_pointGrip1;
	CGripShape m_pointGrip2;

	bool m_ignoreUpdate;
};


} // namespace iqt2d


#endif // !iqt2d_CLine2dShape_included


