#ifndef iview_IDisplay_included
#define iview_IDisplay_included


#include <i2d/CRect.h>

#include <iview/IShapeObserver.h>


namespace iview
{


class CScreenTransform;
class IColorSchema;


class IDisplay: public IShapeObserver
{
public:
	/**
		Define possible display change flags.
	*/
	enum ChangeFlags
	{
		/**
			View transform is changed.
		*/
		CF_TRANSFORM = 0x3f67360,
		
		/**
			View colors are changed.
		*/
		CF_COLORS,
		
		/**
			View size is changed.
		*/
		CF_SIZE,
		
		/**
			Edit mode is changed.
		*/
		CF_EDIT_MODE,
		/**	
			Indicate, that changes come from display console shapes.
		*/
		CS_CONSOLE
	};

	/**
		Get parent object of this display.
		If this display is root, it returns NULL.
	*/
	virtual IDisplay* GetParentDisplayPtr() const = 0;

	/**
		Return a screen transformation.
		This transformation converts view units to screen units.
	*/
	virtual const iview::CScreenTransform& GetTransform() const = 0;

	/**
		Get bounding box of all object in this display.
	*/
	virtual i2d::CRect GetBoundingBox() const = 0;
	
	/**
		Get bounding box of client area.
	*/
	virtual i2d::CRect GetClientRect() const = 0;

	/**
		Get color schema.
   */
	virtual const IColorSchema& GetColorSchema() const = 0;
	
	/**
		Invalidate display area.
		\param	prevArea	previous area used to display object.
		\param	newArea		new area used to display object.
	*/
	virtual void OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea) = 0;
};


} // namespace iview


#endif // !iview_IDisplay_included


