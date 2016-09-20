#ifndef iview_ISelectableLayer_included
#define iview_ISelectableLayer_included


// Qt includes
#include <qnamespace.h>


// ACF includes
#include <i2d/CVector2d.h>

#include <iview/IViewLayer.h>
#include <iview/ISelectable.h>
#include <iview/IDraggable.h>
#include <iview/IMouseActionObserver.h>


namespace iview
{


class IInteractiveShape;


class ISelectableLayer:
			virtual public IViewLayer,
			virtual public ISelectable,
			virtual public IDraggable
{
public:
	/**
		Connect active shape object.
	*/
	virtual bool ConnectInteractiveShape(IInteractiveShape* shapePtr) = 0;
	
	/**
		Get number of unselected shapes.
	*/
	virtual int GetUnselectedShapesCount() const = 0;
	
	/**
		Draw only focused shape.
	*/
	virtual void DrawFocusedShape(QPainter& drawContext) = 0;

	/**
		Called, when mouse button was pushed down or up.
		\return	true, if this event was consumed.
	*/
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) = 0;
	
	/**
		Called, when this layer has focus, and mouse button was pushed down or up on this object.
		\return	true, if this event was consumed.
	*/
	virtual bool OnFocusedMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag) = 0;

	/**
		Called, when this layer has focus, and mouse was moved.
		\return	true, if this event was consumed.
	*/
	virtual bool OnFocusedMouseMove(istd::CIndex2d position) = 0;
	
	/**
		Check, if any shape is touched.
		When shape is touched, it return also pointer to this shape.
		\param	shapePtrPtr	pointer to shape pointer will be filled if shape is found. It can be equal NULL.
		\return	touch state for the shape, or iview::IInteractiveShape::TS_NONE, when nothing found.
	*/
	virtual TouchState IsTouched(istd::CIndex2d position, IInteractiveShape** shapePtrPtr = NULL) const = 0;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const;
};


// inline methods

// reimplemented (iview::ITouchable)

inline ITouchable::TouchState ISelectableLayer::IsTouched(istd::CIndex2d position) const
{
	return IsTouched(position, NULL);
}


} // namespace iview


#endif // !iview_ISelectableLayer_included


