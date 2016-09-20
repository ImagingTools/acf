#ifndef iview_IViewLayer_included
#define iview_IViewLayer_included


// ACF includes
#include <iview/IDisplay.h>
#include <iview/ITouchable.h>


class QPainter;


namespace iview
{

class IShape;
class IShapeView;


/**
	Interface for a logical layer in the console view.

*/
class IViewLayer:
			virtual public IDisplay,
			virtual public ITouchable
{
public:
	enum LayerType
	{
		/**
			Layer type is not set.
		*/
		LT_NONE,

		/**
			Background layer.
		*/
		LT_BACKGROUND,

		/**
			Layer with inactive shapes. The user interaction with the shapes is impossible.
		*/
		LT_INACTIVE,

		/**
			Layer with active shapes.
		*/
		LT_ACTIVE,
		
		/**
			Calibration layer.
		*/
		LT_CALIBRATION
	};

	/**
		Called after layer is connected to view.
	*/
	virtual void OnConnectView(IShapeView* viewPtr) = 0;
	
	/**
		Called before layer is disconnected from view.
	*/
	virtual void OnDisconnectView(IShapeView* viewPtr) = 0;
	
	/**
		Get parent view of this layer.
		It can be used only when view is connected.
	*/
	virtual IShapeView* GetViewPtr() const = 0;
	
	/**
		Check, if the shape is connected to the layer.
	*/
	virtual bool IsShapeConnected(IShape* shapePtr) = 0;
	
	/**
		Connect shape object to view.
		\param	shape	a shape object.
		\param	active	if true, shape will be active, if false it will be only visible.
		\return	true, if it was possible to connect this shape.
	*/
	virtual bool ConnectShape(IShape* shapePtr) = 0;
	
	/**
		Get count of all shapes on this layer.
	*/
	virtual int GetShapesCount() const = 0;
	
	/**
		Send update to all shapes after view changes.
	*/
	virtual void UpdateAllShapes(const istd::IChangeable::ChangeSet& changeSet) = 0;
	
	/**
		Disconnect all shapes from this layer.
	*/
	virtual void DisconnectAllShapes() = 0;
	
	/**
		Draw all shapes using specified draw context.
	*/
	virtual void DrawShapes(QPainter& drawContext) = 0;
	
	/**
		Check, if this layer is visible.
	*/
	virtual bool IsVisible() const = 0;
	
	/**
		Make this layer visible.
	*/
	virtual void SetVisible(bool state = true) = 0;
};


} // namespace iview


#endif // !iview_IViewLayer_included


