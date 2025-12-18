#ifndef iview_CLabelShape_included
#define iview_CLabelShape_included


// ACF includes
#include <i2d/CLabel.h>
#include <iview/ITextLabelParams.h>
#include <iview/CPinShape.h>

namespace iview
{


class CLabelShape: public CPinShape, virtual public ITextLabelParams
{
public:
	typedef CPinShape BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	CLabelShape(
				TextAlign align = TAL_RIGHT_TOP,
				const istd::CIndex2d& offset = istd::CIndex2d(20, -20));

	virtual bool IsPositionVisible() const;
	virtual void SetPositionVisible(bool visible = true);
	virtual bool IsBackgroundTransparent() const;
	virtual void SetBackgroundTransparent(bool state = true);
	virtual bool IsEditableOffset() const;
	virtual void SetEditableOffset(bool editable = true);

	// reimplemented (ITextLabelParams)
	virtual TextAlign GetTextAlign() const override;
	virtual void SetTextAlign(TextAlign align) override;
	virtual const istd::CIndex2d& GetDrawOffset() const override;
	virtual void SetDrawOffset(const istd::CIndex2d& offset) override;

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
	enum EditMode
	{
		EM_NONE,
		EM_POSITION,
		EM_OFFSET
	};

	void CalculateTextOriginSize(i2d::CRect& textBox) const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

private:
	istd::CIndex2d m_referenceOffset;
	TextAlign   m_textAlign;
	mutable istd::CIndex2d m_drawOffset;
	bool m_isPositionVisible;
	bool m_isBackgroundTransparent;
	bool m_isEditableOffset;
	EditMode m_editMode;

	bool m_isAlignFixed;
};


inline bool CLabelShape::IsPositionVisible() const
{
	return m_isPositionVisible;
}


inline void CLabelShape::SetPositionVisible(bool visible)
{
	m_isPositionVisible = visible;
}


inline bool CLabelShape::IsBackgroundTransparent() const
{
	return m_isBackgroundTransparent;
}


inline void CLabelShape::SetBackgroundTransparent(bool state)
{
	m_isBackgroundTransparent = state;
}


inline bool CLabelShape::IsEditableOffset() const
{
	return m_isEditableOffset;
}


inline void CLabelShape::SetEditableOffset(bool editable)
{
	m_isEditableOffset = editable;
}


// reimplemented (ITextLabelParams)

inline CLabelShape::TextAlign CLabelShape::GetTextAlign() const
{
	return m_textAlign;
}


inline void CLabelShape::SetTextAlign(CLabelShape::TextAlign align)
{
	m_textAlign = align;
}


inline const istd::CIndex2d& CLabelShape::GetDrawOffset() const
{
	return m_drawOffset;
}


inline void CLabelShape::SetDrawOffset(const istd::CIndex2d& offset)
{
	m_drawOffset = offset;
}


} // namespace iview


#endif // !iview_CLabelShape_included


