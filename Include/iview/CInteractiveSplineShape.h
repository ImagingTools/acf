#ifndef iview_CInteractiveSplineShape_included
#define iview_CInteractiveSplineShape_included



#include "iview/CInteractivePolylineShape.h"



namespace iview{



class CInteractiveSplineShape: public CInteractivePolylineShape
{
public:
	typedef CInteractivePolylineShape BaseClass;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

protected:
	virtual void DrawPolyBezier(QPainter& drawContext, const istd::CIndex2d* pointsPtr, int pointsCount) const;

	// reimplemented (iview::CInteractivePolygonShape)
	virtual i2d::CVector2d GetSegmentMiddle(int index) const;
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;
};



} // namespace iview



#endif // !iview_CInteractiveSplineShape_included



