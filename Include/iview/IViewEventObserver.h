#ifndef iview_IViewEventObserver_included
#define iview_IViewEventObserver_included


// Qt includes
#include <QtCore/QtGlobal>

// ACF includes
#include <istd/IPolymorphic.h>
#include <imod/IModel.h>
#include <iview/IShapeView.h>
#include <iview/IMouseActionObserver.h>


namespace iview
{


/**
	Interface for external view event observer.
*/
class IViewEventObserver: virtual public istd::IPolymorphic
{
public:
	/**
		Message handler for selection changes.
		\return	true, if this event was consumed by this listener and should not be further processed.
	*/
	virtual bool OnSelectChange(const IShapeView& view, const istd::CIndex2d& position, const IInteractiveShape& shape, bool state) = 0;
	
	/**
		Message handler for mouse button click.
		\return	true, if this event was consumed by this listener and should not be further processed.
	*/
	virtual bool OnViewMouseButton(const IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const IInteractiveShape* shapePtr) = 0;
};



} // namespace iview



#endif // !iview_IViewEventObserver_included



