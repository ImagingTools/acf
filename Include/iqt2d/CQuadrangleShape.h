#ifndef iqt2d_CQuadrangleShape_included
#define iqt2d_CQuadrangleShape_included


// Qt includes
#include <QGraphicsPathItem>


// ACF includes
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

protected slots:
	virtual void OnFirstDiagGrip1Changed(const i2d::CVector2d& point);
	virtual void OnFirstDiagGrip2Changed(const i2d::CVector2d& point);
	virtual void OnSecondDiagGrip1Changed(const i2d::CVector2d& point);
	virtual void OnSecondDiagGrip2Changed(const i2d::CVector2d& point);

protected:
	// reimplemented (iqt2d::TObjectShapeBase)
	void UpdateGraphicsItem(const i2d::CQuadrangle& quadrangle);

private:
	CGripShape m_firstDiagGrip1;
	CGripShape m_firstDiagGrip2;
	CGripShape m_secondDiagGrip1;
	CGripShape m_secondDiagGrip2;
};


} // namespace iqt2d


#endif // !iqt2d_CQuadrangleShape_included


