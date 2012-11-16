#ifndef iview_COrientedCircleShape_included
#define iview_COrientedCircleShape_included


#include "iview/CInteractiveCircleShape.h"


namespace iview
{


class COrientedCircleShape: public CInteractiveCircleShape
{
public:
	typedef CInteractiveCircleShape BaseClass;

	COrientedCircleShape();

	// reimplemented (iview::CInteractiveCircleShape)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
};


} // namespace iview


#endif // !iview_COrientedCircleShape_included


