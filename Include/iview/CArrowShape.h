#ifndef iview_CArrowShape_included
#define iview_CArrowShape_included


#include "i2d/CVector2d.h"
#include "i2d/CLine2d.h"

#include "iview/CInteractiveShapeBase.h"


namespace iview
{


class CArrowShape: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;

	CArrowShape();

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void InvalidateBoundingBox();

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	void EnsurePointsAreValid() const;
	void CalcPoints(const i2d::CLine2d& line) const;
	void ResetPoints() const;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

	// static methods
	static bool CheckLine(
					const istd::CIndex2d& firstPoint,
					const istd::CIndex2d& secondPoint,
					const istd::CIndex2d& point,
					double maxDistance);

	i2d::CVector2d m_referencePosition;
	double m_arrowLinesProportion;
	double m_maxArrowLines;
	int m_referenceIndex;
	mutable istd::CIndex2d m_points[4];
	mutable bool m_arePointsValid;
};


// inline methods

inline void CArrowShape::EnsurePointsAreValid() const
{
	if (!m_arePointsValid){
		const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(GetModelPtr());
		if (linePtr != NULL){
			CalcPoints(*linePtr);
		}
		else{
			ResetPoints();
		}
		m_arePointsValid = true;
	}
}


} // namespace iview


#endif // !iview_CArrowShape_included


