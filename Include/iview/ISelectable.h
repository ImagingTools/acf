#ifndef iview_ISelectable_included
#define iview_ISelectable_included


// Qt includes
#include <QtCore/QSet>

// ACF includes
#include "istd/CIndex2d.h"

#include "iview/ITouchable.h"


namespace iview
{


class IInteractiveShape;


class ISelectable: virtual public ITouchable
{
public:
	typedef QSet<IInteractiveShape*> SelectedShapes;

	/**
		Defines possible states of mouse pointer.
	*/
	enum MousePointerMode
	{
		MPM_NONE,
		MPM_DEFAULT,
		MPM_DESELECT,
		MPM_POINTER,
		MPM_WAIT,
		MPM_CROSS,
		MPM_HAND,
		MPM_DRAG,
		MPM_SCREEN_MOVE,
		MPM_LAST = MPM_SCREEN_MOVE
	};

	/**
		Describes standard edit modes.
	*/
	enum EditMode
	{
		EM_NONE = 0,
		EM_MOVE = 1,
		EM_ADD = 2,
		EM_REMOVE = 3
	};

	/**
		Get count of selected shapes.
	*/
	virtual int GetSelectedShapesCount() const = 0;
	
	/**
		Get set of selected shapes.
	*/
	virtual void InsertSelectedShapes(SelectedShapes& result) const = 0;
	
	/**
		Set all shapes to be deselected.
	*/
	virtual void DeselectAllShapes() = 0;
	
	/**
		Get state of system keys.
	*/
    virtual int GetKeysState() const = 0;

	/**
		Get actual edit mode.
	*/
	virtual int GetEditMode() const = 0;
	
	/**
		Called when shape get focus.
		\internal
		Object with focus receive mouse move events.
		\param	shape	a pointer to shape. Cannot be NULL.
	*/
	virtual void OnShapeFocused(IInteractiveShape* shapePtr) = 0;
	
	/**
		Called when shape loose focus.
		\internal
		\param	shape	a pointer to shape. Cannot be NULL.
	*/
	virtual void OnShapeDefocused(IInteractiveShape* shapePtr) = 0;
	
	/**
		It is called after change shape selection state.
		\internal
		Implementation shoud convert
		internal collection to fit its to new state,
		update bounding boxes and invalidate viewport.
		\param	state	a new selection state.
	*/
	virtual void OnShapeSelected(IInteractiveShape& shape, bool state = true) = 0;
};


} // namespace iview


#endif // !iview_ISelectable_included


