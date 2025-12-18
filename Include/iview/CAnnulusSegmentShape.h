#ifndef iview_CAnnulusSegmentShape_included
#define iview_CAnnulusSegmentShape_included


#include <iview/CAnnulusShape.h>


namespace iview
{

	
class CAnnulusSegmentShape: public CAnnulusShape
{
public:
	typedef CAnnulusShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	CAnnulusSegmentShape();

	virtual bool IsEditableAngles() const;
	virtual void SetEditableAngles(bool editable = true);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

protected:
	virtual void DrawArea(
				QPainter& painter,
				i2d::CVector2d realCenter,
				i2d::CVector2d center,
				double realMinRadius,
				double realMaxRadius,
				int minRadius,
				int maxRadius,
				double startAngle,
				double stopAngle,
				bool fillFlag = true) const;

	double GetDegreeAndleOfPoint(const i2d::CVector2d& center, const QPointF& point) const;

	bool m_editableAngle;

	enum EditMode
	{
		EM_ANGLE1 = EM_OUTER_RADIUS + 1,
		EM_ANGLE2
	};
};


inline bool CAnnulusSegmentShape::IsEditableAngles() const
{
	return m_editableAngle;
}


} // namespace iview


#endif // !iview_CAnnulusSegmentShape_included

