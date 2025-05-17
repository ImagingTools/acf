#ifndef iview_CInteractiveViewLayer_included
#define iview_CInteractiveViewLayer_included


#include <iview/ISelectableLayer.h>
#include <iview/IShapeView.h>
#include <iview/CViewLayer.h>


namespace iview
{


class CInteractiveViewLayer:
			public CViewLayer,
			virtual public ISelectableLayer
{
public:
	typedef CViewLayer BaseClass;

	CInteractiveViewLayer();
	virtual ~CInteractiveViewLayer();

	// reimplemented (iview::CViewLayer)
	virtual i2d::CRect RecalcAllShapes(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual i2d::CRect CalcBoundingBox() const override;

	// reimplemented (iview::ISelectableLayer)
	virtual bool ConnectInteractiveShape(IInteractiveShape* shapePtr) override;
	virtual int GetUnselectedShapesCount() const override;
	virtual void DrawFocusedShape(QPainter& drawContext) override;
	virtual bool OnKeyPress(int key, Qt::KeyboardModifiers modifiers) override;
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnFocusedMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) override;
	virtual bool OnFocusedMouseMove(istd::CIndex2d position) override;
	virtual TouchState IsTouched(istd::CIndex2d position, IInteractiveShape** shapePtrPtr = NULL) const override;

	// reimplemented (iview::ISelectable)
	virtual int GetSelectedShapesCount() const override;
	virtual void InsertSelectedShapes(SelectedShapes& result) const override;
	virtual void DeselectAllShapes() override;
	virtual int GetKeysState() const override;
	virtual int GetEditMode() const override;
	virtual int GetDisplayMode() const override;
	virtual void OnShapeFocused(IInteractiveShape* shapePtr) override;
	virtual void OnShapeDefocused(IInteractiveShape* shapePtr) override;
	virtual void OnShapeSelected(IInteractiveShape& shape, bool state = true) override;

	// reimplemented (iview::IViewLayer)
	virtual bool IsShapeConnected(IShape* shapePtr) override;
	virtual bool ConnectShape(IShape* shapePtr) override;
	virtual int GetShapesCount() const override;
	virtual void DisconnectAllShapes() override;
	virtual void DrawShapes(QPainter& drawContext) override;

	// reimplemented (iview::IShapeObserver)
	virtual void OnChangeShape(IShape* shapePtr) override;
	virtual bool DisconnectShape(IShape* shapePtr) override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const override;
	virtual QString GetToolTipAt(istd::CIndex2d position) const override;

	// reimplemented (iview::IDraggable)
	virtual void BeginDrag(const istd::CIndex2d& reference) override;
	virtual void SetDragPosition(const istd::CIndex2d& position) override;
	virtual void EndDrag() override;
	virtual bool IsDraggable() const override;

protected:
	ShapeList m_activeShapes;

private:
	IInteractiveShape* m_focusedShapePtr;
};


// inline methods

// reimplemented (iview::ISelectable)

inline int CInteractiveViewLayer::GetKeysState() const
{
	const IShapeView* viewPtr = GetViewPtr();
	Q_ASSERT(viewPtr != NULL);

	return viewPtr->GetKeysState();
}


inline int CInteractiveViewLayer::GetEditMode() const
{
	const IShapeView* viewPtr = GetViewPtr();
	Q_ASSERT(viewPtr != NULL);

	return viewPtr->GetEditMode();
}


inline int CInteractiveViewLayer::GetDisplayMode() const
{
	const IShapeView* viewPtr = GetViewPtr();
	Q_ASSERT(viewPtr != NULL);

	return viewPtr->GetDisplayMode();
}


} // namespace iview


#endif // !iview_CInteractiveViewLayer_included



