#ifndef iqt2d_CQuadrangleShape_included
#define iqt2d_CQuadrangleShape_included


#include <QGraphicsPathItem>

#include "i2d/CQuadrangle.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CQuadrangleShape: public TObjectShapeBase<QGraphicsPathItem, i2d::CQuadrangle>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsPathItem, i2d::CQuadrangle> BaseClass;

	CQuadrangleShape(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected slots:
	virtual void OnFirstDiagGrip1Changed(const QPointF& point);
	virtual void OnFirstDiagGrip2Changed(const QPointF& point);
	virtual void OnSecondDiagGrip1Changed(const QPointF& point);
	virtual void OnSecondDiagGrip2Changed(const QPointF& point);

private:
	void UpdateGripPositions();

private:
	CGripShape m_firstDiagGrip1;
	CGripShape m_firstDiagGrip2;
	CGripShape m_secondDiagGrip1;
	CGripShape m_secondDiagGrip2;
};


} // namespace iqt2d


#endif // !iqt2d_CQuadrangleShape_included


