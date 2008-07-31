#ifndef iqt2d_CPosition2dShape_included
#define iqt2d_CPosition2dShape_included


#include "imod/TSingleModelObserverBase.h"

#include "i2d/CPosition2d.h"

#include "iqt2d/CGripShape.h"


namespace iqt2d
{


class CPosition2dShape:
			public CGripShape, 
			public imod::TSingleModelObserverBase<i2d::CPosition2d>
{
public:
	typedef QGraphicsLineItem BaseClass;
	typedef imod::TSingleModelObserverBase<i2d::CPosition2d> BaseClass2;

	CPosition2dShape();

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags = 0, istd::IPolymorphic* updateParamsPtr = NULL);

protected:
	// reimplemented (CGripShape) 
	virtual void OnPositionChanged(const i2d::CVector2d& position);

private:
	bool m_ignoreUpdate;
};


} // namespace iqt2d


#endif // !iqt2d_CPosition2dShape_included


