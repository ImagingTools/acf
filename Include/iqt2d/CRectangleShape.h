#ifndef iqt2d_CRectangleShape_included
#define iqt2d_CRectangleShape_included


// Qt includes
#include <QGraphicsRectItem>


// ACF includes
#include "i2d/CRectangle.h"

#include "iqt2d/TObjectShapeBase.h"
#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CRectangleShape: public TObjectShapeBase<QGraphicsPathItem, i2d::CRectangle>
{
	Q_OBJECT

public:
	typedef TObjectShapeBase<QGraphicsPathItem, i2d::CRectangle> BaseClass;

	CRectangleShape(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

protected slots:
	virtual void OnTopLeftChanged(const i2d::CVector2d& point);
	virtual void OnTopRightChanged(const i2d::CVector2d& point);
	virtual void OnBottomLeftChanged(const i2d::CVector2d& point);
	virtual void OnBottomRightChanged(const i2d::CVector2d& point);

protected:
	// reimplemented (iqt2d::TObjectShapeBase)
	void UpdateGraphicsItem(const i2d::CRectangle& rect);

private:
	CGripShape m_topLeftGrip;
	CGripShape m_topRightGrip;
	CGripShape m_bottomLeftGrip;
	CGripShape m_bottomRightGrip;
};


} // namespace iqt2d


#endif // !iqt2d_CRectangleShape_included


