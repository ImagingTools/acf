#ifndef iview_CPolygonShape_included
#define iview_CPolygonShape_included


// Qt includes
#include <QtCore/QVector>
#include <QtGui/QPolygon>

// ACF includes
#include <i2d/CVector2d.h>


#include <iview/CRectControlledShapeBase.h>


namespace iview
{


class CPolygonShape: public CRectControlledShapeBase
{
public:
	typedef CRectControlledShapeBase BaseClass;
	typedef CInteractiveShapeBase ShapeBaseClass;

	virtual bool IsAreaTouchAllowed() const { return m_areaTouchAllowed; }
	virtual void SetAreaTouchAllowed(bool state = true) { m_areaTouchAllowed = state; }

	virtual bool IsEditableNodes() const { return m_editableNodes; }
	virtual void SetEditableNodes(bool state = true) { m_editableNodes = state; }

	/**
		Check if first point is always visible as ticker.
		It is used to recognize which one point is the first.
	*/
	virtual bool IsFirstVisible() const;
	virtual void SetFirstVisible(bool state = true);

	/**
		Check if in all edit modes is this shape draggable.
		Normally it is possible to drag this object only in EM_NONE mode.
	*/
	virtual bool IsAlwaysMovable() const;

	/**
		Set flag if in all edit modes is this shape draggable.
		\sa IsAlwaysMovable()
	*/
	virtual void SetAlwaysMovable(bool state = true);

	// reimplemented (iview::IShape)
	virtual bool IsInside(const istd::CIndex2d& screenPosition) const override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const override;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnMouseMove(istd::CIndex2d position) override;

	// reimplemented (iview::IDraggable)
	virtual bool IsDraggable() const override;

protected:
	static const istd::IChangeable::ChangeSet& GetMoveChangeSet();
	static const istd::IChangeable::ChangeSet& GetMoveAllChangeSet();

	virtual i2d::CVector2d GetSegmentMiddle(int index) const;
	virtual void DrawCurve(QPainter& drawContext) const;
	virtual void DrawArea(QPainter& drawContext) const;
	virtual void DrawSelectionElements(QPainter& drawContext) const;

	// TODO...
	virtual bool IsAreaTouched(istd::CIndex2d position) const;

	// reimplemented (iview::CRectControlledShapeBase)
	virtual void EnsureValidNodes() const override;
	virtual bool IsCurveTouched(istd::CIndex2d position) const override;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference) override;
	virtual void SetLogDragPosition(const i2d::CVector2d& position) override;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

protected:
	int m_referenceIndex = 0;
	QVector<i2d::CVector2d> m_references;

	i2d::CVector2d m_castAxis = { 1,0 };
	mutable i2d::CAffine2d m_castTransform;
	mutable QPolygonF m_screenPoints;

private:
	bool m_isFirstVisible = false;
	bool m_isAlwaysDraggable = false;
	bool m_areaTouchAllowed = false;
	bool m_editableNodes = true;
};


inline bool CPolygonShape::IsFirstVisible() const
{
	return m_isFirstVisible;
}


inline void CPolygonShape::SetFirstVisible(bool state)
{
	m_isFirstVisible = state;
}


inline bool CPolygonShape::IsAlwaysMovable() const
{
	return m_isAlwaysDraggable;
}


inline void CPolygonShape::SetAlwaysMovable(bool state)
{
	m_isAlwaysDraggable = state;
}


} // namespace iview


#endif // !iview_CPolygonShape_included


