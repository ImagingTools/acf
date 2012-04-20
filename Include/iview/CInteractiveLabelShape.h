#ifndef iview_CInteractiveLabelShape_included
#define iview_CInteractiveLabelShape_included


#include "i2d/CLabel.h"

#include "iview/CInteractivePinShape.h"


namespace iview
{


class CInteractiveLabelShape: public CInteractivePinShape
{
public:
	typedef CInteractivePinShape BaseClass;

    enum TextAlign
	{
        TAL_CENTER,
        TAL_LEFT_TOP,
        TAL_RIGHT_TOP,
        TAL_LEFT_BOTTOM,
        TAL_RIGHT_BOTTOM,
        TAL_AUTOMATIC,
        TAL_LAST = TAL_AUTOMATIC
    };

	CInteractiveLabelShape(
				TextAlign align = TAL_RIGHT_TOP,
				const istd::CIndex2d& offset = istd::CIndex2d(20, -20));

    virtual TextAlign GetTextAlign() const;
	virtual void SetTextAlign(TextAlign align);
    virtual const istd::CIndex2d& GetDrawOffset() const;
	virtual void SetDrawOffset(const istd::CIndex2d& offset);

	virtual bool IsPositionVisible() const;
	virtual void SetPositionVisible(bool visible = true);
	virtual bool IsBackgroundTransparent() const;
	virtual void SetBackgroundTransparent(bool state = true);
	virtual bool IsEditableOffset() const;
	virtual void SetEditableOffset(bool editable = true);

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

protected:
	enum EditMode
	{
		EM_NONE,
		EM_POSITION,
		EM_OFFSET
	};

	void CalculateTextOriginSize(i2d::CRect& textBox) const;

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


inline CInteractiveLabelShape::TextAlign CInteractiveLabelShape::GetTextAlign() const
{
    return m_textAlign;
}


inline void CInteractiveLabelShape::SetTextAlign(CInteractiveLabelShape::TextAlign align)
{
    m_textAlign = align;
}


inline const istd::CIndex2d& CInteractiveLabelShape::GetDrawOffset() const
{
    return m_drawOffset;
}


inline void CInteractiveLabelShape::SetDrawOffset(const istd::CIndex2d& offset)
{
    m_drawOffset = offset;
}


inline bool CInteractiveLabelShape::IsPositionVisible() const
{
	return m_isPositionVisible;
}


inline void CInteractiveLabelShape::SetPositionVisible(bool visible)
{
	m_isPositionVisible = visible;
}


inline bool CInteractiveLabelShape::IsBackgroundTransparent() const
{
	return m_isBackgroundTransparent;
}


inline void CInteractiveLabelShape::SetBackgroundTransparent(bool state)
{
	m_isBackgroundTransparent = state;
}


inline bool CInteractiveLabelShape::IsEditableOffset() const
{
	return m_isEditableOffset;
}


inline void CInteractiveLabelShape::SetEditableOffset(bool editable)
{
	m_isEditableOffset = editable;
}


} // namespace iview


#endif // !iview_CInteractiveLabelShape_included


