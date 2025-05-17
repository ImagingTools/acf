#ifndef iview_CTubePolylineShape_included
#define iview_CTubePolylineShape_included


#include <iview/CPolylineShape.h>


namespace iview
{


/**
	Graphical representation of tube region.
*/
class CTubePolylineShape: public iview::CPolylineShape
{
public:
	typedef iview::CPolylineShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	CTubePolylineShape();

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

protected:
	// reimplemented (iview::CPolygonShape)
	virtual void DrawCurve(QPainter& drawContext) const override;
	virtual void DrawSelectionElements(QPainter& drawContext) const override;

	// reimplemented (iview::CRectControlledShapeBase)
	virtual void EnsureValidNodes() const override;
	virtual bool IsCurveTouched(istd::CIndex2d position) const override;

protected:
	enum EditMode
	{
		EM_NONE,
		EM_BASIC,
		EM_LEFT,
		EM_RIGHT
	};

	int m_editMode;
	int m_editedNodeIndex;
};


} // namespace qto


#endif // !iview_CTubePolylineShape_included



