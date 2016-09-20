#ifndef iqt2d_IViewProvider_included
#define iqt2d_IViewProvider_included


// ACF includes
#include <istd/IPolymorphic.h>

#include <iqt2d/iqt2d.h>


namespace iview
{
	class IShapeView;
}


namespace iqt2d
{


/**
	Interface for GUI objects managing view.
*/
class IViewProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get ID indentifing this view.
		Typically this ID is 0 for first view, 1 for the second etc.
	*/
	virtual int GetViewId() const = 0;

	/**
		Called when items should be removed from specified view.
	*/
	virtual iview::IShapeView* GetView() const = 0;
};


} // namespace iqt2d


#endif // !iqt2d_IViewProvider_included


