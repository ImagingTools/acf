#ifndef iview_CSelectableLayerBase_included
#define iview_CSelectableLayerBase_included


#include "iview/ISelectableLayer.h"
#include "iview/IShapeView.h"
#include "iview/TLayerBaseEx.h"


namespace iview
{


class CSelectableLayerBase: public TLayerBaseEx<ISelectableLayer>
{
public:
	typedef TLayerBaseEx<ISelectableLayer> BaseClass;

	CSelectableLayerBase();
	virtual ~CSelectableLayerBase();

	// reimplemented (iview::TLayerBase)
	void CalcBoundingBox(i2d::CRect& result) const;

	// reimplemented (iview::ISelectableLayer)
	virtual bool ConnectInteractiveShape(IInteractiveShape* shapePtr);
	virtual int GetUnselectedShapesCount() const;
	virtual void DrawFocusedShape(QPainter& drawContext);
	virtual void BeginDrag(const i2d::CVector2d& reference);
	virtual void SetDragPosition(const i2d::CVector2d& position);
	virtual void EndDrag();
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

	// reimplemented (iview::ILayer)
	virtual bool IsShapeConnected(IShape* shapePtr);
	virtual bool ConnectShape(IShape* shapePtr);
	virtual int GetShapesCount() const;
	virtual void UpdateAllShapes(int changeFlag);
	virtual void DisconnectAllShapes();
	virtual void DrawShapes(QPainter& drawContext);

	// reimplemented (iview::IShapeObserver)
	virtual void OnChangeShape(IShape* shapePtr);
	virtual void DisconnectShape(IShape* shapePtr);

protected:
	ShapeMap m_activeShapes;
	ShapeMap m_inactiveShapes;

	IInteractiveShape* m_focusedShapePtr;
};


// inline methods

// reimplemented (iview::ISelectable)

inline int CSelectableLayerBase::GetKeysState() const
{
	const IShapeView* viewPtr = GetViewPtr();
	I_ASSERT(viewPtr != NULL);

	return viewPtr->GetKeysState();
}


inline int CSelectableLayerBase::GetEditMode() const
{
	const IShapeView* viewPtr = GetViewPtr();
	I_ASSERT(viewPtr != NULL);

	return viewPtr->GetEditMode();
}


} // namespace iview


#endif // !iview_CSelectableLayerBase_included



