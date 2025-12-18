#ifndef iview_CLineShape_included
#define iview_CLineShape_included


#include <i2d/CVector2d.h>
#include <i2d/CLine2d.h>

#include <iview/CInteractiveShapeBase.h>


namespace iview
{


class CLineShape: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;

	CLineShape();

	enum LineDisplayMode
	{
		LDM_SIMPLE,
		LDM_ARROW
	};

	/**
		Get displayed of line.
	*/
	int GetLineDisplayMode() const;
	/**
		Set display mode of this line.
	*/
	void SetLineDisplayMode(int mode);

	/**
		Get proportion of arrow side lines to line length.
		If there is no arrow display mode, this value has no effect.
	*/
	double GetArrowLinesProportion() const;
	/**
		Set proportion of arrow side lines to line length.
		If there is no arrow display mode, this value has no effect.
	*/
	void SetArrowLinesProportion(double value);

	/**
		Get maximal length of arrow lines.
		If there is no arrow display mode, this value has no effect.
	*/
	double GetMaxArrowLineLength() const;
	/**
		Set maximal length of arrow lines.
		If there is no arrow display mode, this value has no effect.
	*/
	void SetMaxArrowLineLength(double value);

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void InvalidateBoundingBox() override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

protected:
	void EnsurePointsAreValid() const;
	void EnsurePointsAreValid(const i2d::CLine2d& line) const;
	void CalcPoints(const i2d::CLine2d& line) const;
	void ResetPoints() const;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference) override;
	virtual void SetLogDragPosition(const i2d::CVector2d& position) override;

	// static methods
	static bool CheckLine(
					const istd::CIndex2d& firstPoint,
					const istd::CIndex2d& secondPoint,
					const istd::CIndex2d& point,
					double maxDistance);

	int m_lineDisplayMode;
	double m_arrowLinesProportion;
	double m_maxArrowLineLength;

	i2d::CVector2d m_referencePosition;
	int m_referenceIndex;
	mutable istd::CIndex2d m_points[4];
	mutable bool m_arePointsValid;
};


// inline methods

inline void CLineShape::EnsurePointsAreValid() const
{
	if (!m_arePointsValid){
		const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(GetObservedModel());
		if (linePtr != NULL){
			CalcPoints(*linePtr);
		}
		else{
			ResetPoints();
		}
		m_arePointsValid = true;
	}
}


inline void CLineShape::EnsurePointsAreValid(const i2d::CLine2d& line) const
{
	if (!m_arePointsValid){
		CalcPoints(line);
		m_arePointsValid = true;
	}
}


} // namespace iview


#endif // !iview_CLineShape_included


