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

	IInteractiveShape* GetFirstActiveShape() const;

	// reimplemented (iview::CViewLayer)
	virtual i2d::CRect RecalcAllShapes(const istd::IChangeable::ChangeSet& changeSet);
	i2d::CRect CalcBoundingBox() const;

	// reimplemented (iview::ISelectableLayer)
	virtual bool ConnectInteractiveShape(IInteractiveShape* shapePtr);
	virtual int GetUnselectedShapesCount() const;
	virtual void DrawFocusedShape(QPainter& drawContext);
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnFocusedMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnFocusedMouseMove(istd::CIndex2d position);
	virtual TouchState IsTouched(istd::CIndex2d position, IInteractiveShape** shapePtrPtr = NULL) const;

	// reimplemented (iview::ISelectable)
	virtual int GetSelectedShapesCount() const;
	virtual void InsertSelectedShapes(SelectedShapes& result) const;
	virtual void DeselectAllShapes();
	virtual int GetKeysState() const;
	virtual int GetEditMode() const;
	virtual void OnShapeFocused(IInteractiveShape* shapePtr);
	virtual void OnShapeDefocused(IInteractiveShape* shapePtr);
	virtual void OnShapeSelected(IInteractiveShape& shape, bool state = true);

	// reimplemented (iview::IViewLayer)
	virtual bool IsShapeConnected(IShape* shapePtr);
	virtual bool ConnectShape(IShape* shapePtr);
	virtual int GetShapesCount() const;
	virtual void DisconnectAllShapes();
	virtual void DrawShapes(QPainter& drawContext);

	// reimplemented (iview::IShapeObserver)
	virtual void OnChangeShape(IShape* shapePtr);
	virtual bool DisconnectShape(IShape* shapePtr);

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const;

	// reimplemented (iview::IDraggable)
	virtual void BeginDrag(const istd::CIndex2d& reference);
	virtual void SetDragPosition(const istd::CIndex2d& position);
	virtual void EndDrag();
	virtual bool IsDraggable() const;

protected:
	ShapeMap m_activeShapes;

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


} // namespace iview


#endif // !iview_CInteractiveViewLayer_included



