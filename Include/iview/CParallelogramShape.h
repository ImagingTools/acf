#ifndef iview_CParallelogramShape_included
#define iview_CParallelogramShape_included


// ACF includes
#include <i2d/CParallelogram.h>
#include <i2d/CVector2d.h>
#include <iview/CInteractiveShapeBase.h>


namespace iview
{


class CParallelogramShape: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;

	CParallelogramShape();

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
	virtual void InvalidateBoundingBox() override;

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

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
	const i2d::CVector2d* GetNodes() const;

	virtual void EnsureValidNodes() const;

	virtual bool IsTickerTouched(istd::CIndex2d position) const;
	virtual bool IsCurveTouched(istd::CIndex2d position) const;
	virtual void DrawTickers(QPainter& drawContext) const;
	virtual void DrawFigure(QPainter& drawContext) const;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference) override;
	virtual void SetLogDragPosition(const i2d::CVector2d& position) override;

private:
	bool m_isEditableRotation;
	bool m_isEditableWidth;
	bool m_isEditableHeight;
	bool m_isEditableAngle;
	bool m_isProportionalScaled;
	bool m_areAxesEqual;
	bool m_areAxesOrthogonal;

	mutable i2d::CVector2d m_nodes[EN_LAST + 1];
	mutable bool m_areNodesValid;
};


// inline methods

inline bool CParallelogramShape::IsEditableRotation()
{
	return m_isEditableRotation;
}


inline void CParallelogramShape::SetEditableRotation(bool state)
{
	m_isEditableRotation = state;
}


inline bool CParallelogramShape::IsEditableWidth()
{
	return m_isEditableWidth;
}


inline void CParallelogramShape::SetEditableWidth(bool state)
{
	m_isEditableWidth = state;
}


inline bool CParallelogramShape::IsEditableHeight()
{
	return m_isEditableHeight;
}


inline void CParallelogramShape::SetEditableHeight(bool state)
{
	m_isEditableHeight = state;
}


inline bool CParallelogramShape::IsEditableAngle()
{
	return m_isEditableAngle;
}


inline void CParallelogramShape::SetEditableAngle(bool state)
{
	m_isEditableAngle = state;
}


inline bool CParallelogramShape::IsProportionalScaled() const
{
	return m_isProportionalScaled;
}


inline void CParallelogramShape::SetProportionalScaled(bool state)
{
	m_isProportionalScaled = state;
}


inline bool CParallelogramShape::AreAxesEqual() const
{
	return m_areAxesEqual;
}


inline void CParallelogramShape::SetAxesEqual(bool state)
{
	m_areAxesEqual = state;
}


inline bool CParallelogramShape::AreAxesOrthogonal() const
{
	return m_areAxesOrthogonal;
}


inline void CParallelogramShape::SetAxesOrthogonal(bool state)
{
	m_areAxesOrthogonal = state;
}


// protected methods

inline bool CParallelogramShape::AreNodesValid() const
{
	return m_areNodesValid;
}


inline const i2d::CVector2d* CParallelogramShape::GetNodes() const
{
	return m_nodes;
}


} // namespace iview


#endif // !iview_CParallelogramShape_included


