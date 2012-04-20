#ifndef iview_CInteractiveAnnulusSegmentShape_included
#define iview_CInteractiveAnnulusSegmentShape_included


#include "iview/CInteractiveAnnulusShape.h"


namespace iview
{

	
class CInteractiveAnnulusSegmentShape: public CInteractiveAnnulusShape
{
public:
	typedef CInteractiveAnnulusShape BaseClass;

	CInteractiveAnnulusSegmentShape();

	virtual bool IsEditableAngles() const;
	virtual void SetEditableAngles(bool editable = true);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

protected:
	virtual void DrawAnnulusSegment(
				QPainter& painter,
				istd::CIndex2d center,
				int minRadius,
				int maxRadius,
				double startAngle,
				double stopAngle,
				bool fillFlag = true) const;

	bool m_editRadius2Mode;

	bool m_editableAngle;
	bool m_editAngle1Mode;
	bool m_editAngle2Mode;
};


inline bool CInteractiveAnnulusSegmentShape::IsEditableAngles() const
{
	return m_editableAngle;
}


} // namespace iview


#endif // !iview_CInteractiveAnnulusSegmentShape_included

