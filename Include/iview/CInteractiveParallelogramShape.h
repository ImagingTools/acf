#ifndef iview_CInteractiveParallelogramShape_included
#define iview_CInteractiveParallelogramShape_included


#include "i2d/CParallelogram.h"

#include "iview/CInteractiveShapeBase.h"


namespace iview
{


class CInteractiveParallelogramShape: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;

	CInteractiveParallelogramShape();

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
		Check, if size is editable.
	*/
	virtual bool IsEditableHeight();
	virtual void SetEditableHeight(bool state = true);
	
	/**
		Check, if internal angle is editable.
	*/
	virtual bool IsEditableAngle();
	virtual void SetEditableAngle(bool state = true);
	
	/**
		Check, if both axis will be scaled proportional.
	*/
	virtual bool IsProportionalScaled() const;
	virtual void SetProportionalScaled(bool state = true);

	/**
		Check, if it is forced to keep length of both axes equal.
	*/
	virtual bool AreAxesEqual() const;
	virtual void SetAxesEqual(bool state = true);

	/**
		Check, if it is forced to keep both axes orthogonal.
	*/
	virtual bool AreAxesOrthogonal() const;
	virtual void SetAxesOrthogonal(bool state = true);

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
		EN_NONE = -1,
		EN_NODE11 = 0,
		EN_NODE12 = 1,
		EN_NODE22 = 2,
		EN_NODE21 = 3,
		EN_LAST = EN_NODE21
	};

	EditNode m_editNode;
	i2d::CVector2d m_referencePosition;

	void CalcNodes(const i2d::CAffine2d& parallTransform) const;
	void ResetNodes() const;
	bool AreNodesValid() const;
	const istd::CIndex2d* GetNodes() const;

	virtual void EnsureValidNodes() const;

	virtual bool IsTickerTouched(istd::CIndex2d position) const;
	virtual bool IsFigureTouched(istd::CIndex2d position) const;
	virtual void DrawTickers(QPainter& drawContext) const;
	virtual void DrawFigure(QPainter& drawContext) const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

private:
	bool m_isEditableRotation;
	bool m_isEditableWidth;
	bool m_isEditableHeight;
	bool m_isEditableAngle;
	bool m_isProportionalScaled;
	bool m_areAxesEqual;
	bool m_areAxesOrthogonal;

	mutable istd::CIndex2d m_nodes[EN_LAST + 1];
	mutable bool m_areNodesValid;
};


// inline methods

inline bool CInteractiveParallelogramShape::IsEditableRotation()
{
	return m_isEditableRotation;
}


inline void CInteractiveParallelogramShape::SetEditableRotation(bool state)
{
	m_isEditableRotation = state;
}


inline bool CInteractiveParallelogramShape::IsEditableWidth()
{
	return m_isEditableWidth;
}


inline void CInteractiveParallelogramShape::SetEditableWidth(bool state)
{
	m_isEditableWidth = state;
}


inline bool CInteractiveParallelogramShape::IsEditableHeight()
{
	return m_isEditableHeight;
}


inline void CInteractiveParallelogramShape::SetEditableHeight(bool state)
{
	m_isEditableHeight = state;
}


inline bool CInteractiveParallelogramShape::IsEditableAngle()
{
	return m_isEditableAngle;
}


inline void CInteractiveParallelogramShape::SetEditableAngle(bool state)
{
	m_isEditableAngle = state;
}


inline bool CInteractiveParallelogramShape::IsProportionalScaled() const
{
	return m_isProportionalScaled;
}


inline void CInteractiveParallelogramShape::SetProportionalScaled(bool state)
{
	m_isProportionalScaled = state;
}


inline bool CInteractiveParallelogramShape::AreAxesEqual() const
{
	return m_areAxesEqual;
}


inline void CInteractiveParallelogramShape::SetAxesEqual(bool state)
{
	m_areAxesEqual = state;
}


inline bool CInteractiveParallelogramShape::AreAxesOrthogonal() const
{
	return m_areAxesOrthogonal;
}


inline void CInteractiveParallelogramShape::SetAxesOrthogonal(bool state)
{
	m_areAxesOrthogonal = state;
}


// protected methods

inline bool CInteractiveParallelogramShape::AreNodesValid() const
{
	return m_areNodesValid;
}


inline const istd::CIndex2d* CInteractiveParallelogramShape::GetNodes() const
{
	return m_nodes;
}


} // namespace iview


#endif // !iview_CInteractiveParallelogramShape_included


