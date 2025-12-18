#ifndef iview_CRectControlledShapeBase_included
#define iview_CRectControlledShapeBase_included


// ACF includes
#include <iview/CInteractiveShapeBase.h>


namespace iview
{


class CRectControlledShapeBase: public CInteractiveShapeBase
{
public:
	typedef CInteractiveShapeBase BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;
	
	CRectControlledShapeBase();

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
	
	/**
		Check, if both axis will be scaled proportional.
	*/
	virtual bool IsProportionalScaled() const;
	virtual void SetProportionalScaled(bool state = true);

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void InvalidateBoundingBox() override;

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;

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

	void CalcNodes(const i2d::CAffine2d& parallTransform) const;
	void ResetNodes() const;
	bool AreNodesValid() const;

	i2d::CAffine2d CalcMoveTransform(i2d::CVector2d position, const i2d::CAffine2d& transform);

	virtual bool IsTickerTouched(istd::CIndex2d position) const;
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
	virtual void BeginLogDrag(const i2d::CVector2d& reference) override;
	virtual void SetLogDragPosition(const i2d::CVector2d& position) override;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

	// abstract methods
	virtual void EnsureValidNodes() const = 0;
	virtual bool IsCurveTouched(istd::CIndex2d position) const = 0;

	MouseMode m_mouseMode;
	i2d::CVector2d m_referencePosition;

private:
	bool m_isEditableRotation;
	bool m_isEditableWidth;
	bool m_isEditableHeight;
	bool m_isProportionalScaled;

	mutable i2d::CVector2d m_screenNodes[EN_LAST + 1];
	mutable bool m_areNodesValid;
};


// inline methods

inline bool CRectControlledShapeBase::IsEditableRotation()
{
	return m_isEditableRotation;
}


inline void CRectControlledShapeBase::SetEditableRotation(bool state)
{
	m_isEditableRotation = state;
}


inline bool CRectControlledShapeBase::IsEditableWidth()
{
	return m_isEditableWidth;
}


inline void CRectControlledShapeBase::SetEditableWidth(bool state)
{
	m_isEditableWidth = state;
}


inline bool CRectControlledShapeBase::IsEditableHeight()
{
	return m_isEditableHeight;
}


inline void CRectControlledShapeBase::SetEditableHeight(bool state)
{
	m_isEditableHeight = state;
}


inline bool CRectControlledShapeBase::IsProportionalScaled() const
{
	return m_isProportionalScaled;
}


inline void CRectControlledShapeBase::SetProportionalScaled(bool state)
{
	m_isProportionalScaled = state;
}


// protected methods

inline bool CRectControlledShapeBase::AreNodesValid() const
{
	return m_areNodesValid;
}


} // namespace iview


#endif // !iview_CRectControlledShapeBase_included



