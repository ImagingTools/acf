#ifndef iview_CToolsViewLayer_included
#define iview_CToolsViewLayer_included


// ACF includes
#include <iview/ISelectableLayer.h>
#include <iview/CViewLayer.h>


namespace iview
{


class CToolsViewLayer:
			public CViewLayer,
			virtual public ISelectableLayer
{
public:
	typedef CViewLayer BaseClass;

	// reimplemented (iview::ISelectableLayer)
	virtual bool ConnectInteractiveShape(IInteractiveShape* shapePtr) override;
	virtual int GetUnselectedShapesCount() const override;
	virtual void DrawFocusedShape(QPainter& drawContext) override;
	virtual bool OnKeyPress(int key, Qt::KeyboardModifiers modifiers) override { return false; }
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

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

	// reimplemented (iview::IDraggable)
	virtual void BeginDrag(const istd::CIndex2d& reference) override;
	virtual void SetDragPosition(const istd::CIndex2d& position) override;
	virtual void EndDrag() override;
	virtual bool IsDraggable() const override;
};


} // namespace iview


#endif // !iview_CToolsViewLayer_included


