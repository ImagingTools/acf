#ifndef iview_IShapeView_included
#define iview_IShapeView_included


// STL includes
#include <vector>


// ACF includes
#include "i2d/CRect.h"
#include "i2d/CAffine2d.h"

#include "iview/IDisplay.h"
#include "iview/ISelectable.h"
#include "iview/ISelectableLayer.h"


namespace iview
{


/**
	Common interface for a general shape view implementations.
*/
class IShapeView:
			virtual public IDisplay,
			virtual public ISelectable
{
public:
	typedef IShapeObserver BaseClass;


	/**
		Set a screen transformation.
		\sa	GetTransform()
	*/
	virtual void SetTransform(const i2d::CAffine2d& transform) = 0;
	
	/**
		Updates all invalidates shapes.
	*/
	virtual void Update() = 0;
	
	/**
		Changes the edit mode.
	*/
	virtual void SetEditMode(int mode) = 0;
	
	/**
		Check if is draggable mode.
		If this function return true, user can move a view by dragging.
	*/
	virtual bool IsViewDraggable() const = 0;
	
	/**
		Check, if it is possible to select more than one shape.
	*/
	virtual bool IsMultiselectable() const = 0;

	/**	
		Update mouse pointer.
	*/
	virtual void UpdateMousePointer() = 0;
	
	/**
		Insert layer to view.
		\param	layerPtr	pointer to inserted layer.
		\param	index		index of inserted layer. If equals -1,
							layer will be inserted after layst existing layer.
		\param	layerType	used to set standard layers. \sa iview::IShapeView::LayerType
		\return	index of this layer.
	*/
	virtual int InsertLayer(ILayer* layerPtr, int index = -1, int layerType = ILayer::LT_NONE) = 0;
	
	/**
		Get index of layer.
		\return	layer index, or -1 if not found.
	*/
	virtual int GetLayerIndex(const ILayer& layer) const = 0;
	
	/**
		Remove layer from view.
	*/
	virtual void RemoveLayer(int index) = 0;
	
	/**
		Get count of layer in this view.
	*/
	virtual int GetLayersCount() const = 0;
	
	/**
		Get layer with the given index.
	*/
	virtual ILayer& GetLayer(int index) const = 0;

	/**
		Connect shape object to view.
		\param	shape	a shape object.
		\return	true, if it was possible to connect this shape.
	*/
	virtual bool ConnectShape(IShape* shapePtr) = 0;
	
	/**
		Get focused layer.
	*/
	virtual ISelectableLayer* GetFocusedLayerPtr() const = 0;
	
	/**
		Called when shape in layer get focus.
	*/
	virtual void OnShapeFocused(IInteractiveShape* shapePtr, ISelectableLayer* layerPtr) = 0;

	/**
		Called when shape in layer loose focus.
	*/
	virtual void OnShapeDefocused(IInteractiveShape* shapePtr, ISelectableLayer* layerPtr) = 0;
	
	/**
		Inform that layer was invalidated.
	*/
	virtual void OnLayerInvalidated(const ILayer& layer, const i2d::CRect& prevArea, const i2d::CRect& newArea) = 0;

	// reimplemented (iview::ISelectable)
	virtual void OnShapeFocused(IInteractiveShape* /*shapePtr*/){}
	virtual void OnShapeDefocused(IInteractiveShape* /*shapePtr*/){}
};


} // namespace iview


#endif // !iview_IShapeView_included


