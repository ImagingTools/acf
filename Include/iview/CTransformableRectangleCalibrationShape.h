#ifndef iview_CTransformableRectangleCalibrationShape_included
#define iview_CTransformableRectangleCalibrationShape_included


#include "i2d/CParallelogram.h"

#include "iview/CCalibrationShapeBase.h"


namespace iview
{


// TODO: Redesign it to ACF transformation concept.
class CTransformableRectangleCalibrationShape: public CCalibrationShapeBase
{
public:
	typedef CCalibrationShapeBase BaseClass;
	
	CTransformableRectangleCalibrationShape();

	/**
		Check, if rotation is editable.
	*/
	virtual bool IsEditableRotation();
	virtual void SetEditableRotation(bool state = true);

	/**
		Check, if width is editable.
	*/
	virtual bool IsEditableWidth();
	virtual void SetEditableWidth(bool state = true);
	
	/**
		Check, if height is editable.
	*/
	virtual bool IsEditableHeight();
	virtual void SetEditableHeight(bool state = true);
	/**	Check, if both axis will be scaled proportional.
	*/
	virtual bool IsProportionalScaled() const;
	virtual void SetProportionalScaled(bool state = true);

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void InvalidateBoundingBox();

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	enum EditNode
	{
		EN_NODE1 = 0,
		EN_NODE2,
		EN_NODE3,
		EN_NODE4,
		EN_CENTER,
		EN_MIDDLE12,
		EN_MIDDLE23,
		EN_MIDDLE34,
		EN_MIDDLE41,
		EN_ROTATION1,
		EN_ROTATION2,
		EN_LAST = EN_ROTATION2
	};

	enum MouseMode
	{
		MM_NONE,
		MM_ROTATION1,
		MM_ROTATION2,
		MM_WIDTH1,
		MM_WIDTH2,
		MM_HEIGHT1,
		MM_HEIGHT2,
		MM_SIZE1,
		MM_SIZE2,
		MM_SIZE3,
		MM_SIZE4
	};

	MouseMode m_mouseMode;
	i2d::CVector2d m_referencePosition;

	void CalcNodes(const i2d::CAffine2d& parallTransform) const;
	void ResetNodes() const;
	bool AreNodesValid() const;
	const istd::CIndex2d* GetNodes() const;

	virtual void EnsureValidNodes() const;
	i2d::CAffine2d CalcMoveTransform(i2d::CVector2d position, const i2d::CAffine2d& transform);

	virtual bool IsTickerTouched(istd::CIndex2d position) const;
	virtual bool IsFigureTouched(istd::CIndex2d position) const;
	virtual void DrawTickers(QPainter& drawContext) const;
	virtual void DrawFigure(QPainter& drawContext) const;

	bool IsParallTouched(
					const i2d::CAffine2d& parallTransform,
					istd::CIndex2d position) const;
	i2d::CAffine2d CalcScaleTransform(
					const i2d::CVector2d& center,
					const i2d::CVector2d& isPos,
					const i2d::CVector2d& shouldPos);
	i2d::CAffine2d CalcSizeTransform(
					const i2d::CVector2d& center,
					const i2d::CVector2d& isPos,
					const i2d::CVector2d& shouldPos);
	i2d::CAffine2d CalcRotatedTransform(
					const i2d::CVector2d& center,
					const i2d::CVector2d& isPos,
					const i2d::CVector2d& shouldPos);

	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

private:
	bool m_isEditableRotation;
	bool m_isEditableWidth;
	bool m_isEditableHeight;
	bool m_isProportionalScaled;

	mutable istd::CIndex2d m_nodes[EN_LAST + 1];
	mutable bool m_areNodesValid;
};



// inline methods

inline bool CTransformableRectangleCalibrationShape::IsEditableRotation()
{
	return m_isEditableRotation;
}



inline void CTransformableRectangleCalibrationShape::SetEditableRotation(bool state)
{
	m_isEditableRotation = state;
}



inline bool CTransformableRectangleCalibrationShape::IsEditableWidth()
{
	return m_isEditableWidth;
}



inline void CTransformableRectangleCalibrationShape::SetEditableWidth(bool state)
{
	m_isEditableWidth = state;
}



inline bool CTransformableRectangleCalibrationShape::IsEditableHeight()
{
	return m_isEditableHeight;
}



inline void CTransformableRectangleCalibrationShape::SetEditableHeight(bool state)
{
	m_isEditableHeight = state;
}



inline bool CTransformableRectangleCalibrationShape::IsProportionalScaled() const
{
	return m_isProportionalScaled;
}



inline void CTransformableRectangleCalibrationShape::SetProportionalScaled(bool state)
{
	m_isProportionalScaled = state;
}



// protected methods

inline bool CTransformableRectangleCalibrationShape::AreNodesValid() const
{
	return m_areNodesValid;
}



inline const istd::CIndex2d* CTransformableRectangleCalibrationShape::GetNodes() const
{
	return m_nodes;
}



} // namespace iview



#endif // !iview_CTransformableRectangleCalibrationShape_included



