#ifndef iview_CSplineShape_included
#define iview_CSplineShape_included


// ACF includes
#include <i2d/CVector2d.h>
#include <iview/CPolylineShape.h>


namespace iview{


class CSplineShape: public CPolylineShape
{
public:
	typedef CPolylineShape BaseClass;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

protected:
	virtual void DrawPolyBezier(QPainter& drawContext, const i2d::CVector2d* pointsPtr, int pointsCount) const;

	// reimplemented (iview::CPolygonShape)
	virtual i2d::CVector2d GetSegmentMiddle(int index) const override;
	virtual void DrawCurve(QPainter& drawContext) const override;

	// reimplemented (iview::CRectControlledShapeBase)
	virtual bool IsCurveTouched(istd::CIndex2d position) const override;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;
};


} // namespace iview


#endif // !iview_CSplineShape_included


