#ifndef iqt_IGuiObject_included
#define iqt_IGuiObject_included


#include <QWidget>

#include "istd/IPolymorphic.h"

#include "icomp/IComponent.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Common interface for GUI objects using in component context.
 */
class IGuiObject: virtual public istd::IPolymorphic
{
public:
	/**
		Insert a GUI-component into layout of the parent widget \c parentWidgetPtr.
		If no layout exists a new one will be created.
	*/
	virtual void AttachTo(QWidget* parentWidgetPtr, int layoutMargin = -1) = 0;

	/**
		Removes the GUI-component from layout of the parent widget \c parentWidgetPtr.
	*/
	virtual void DetachFrom(QWidget* parentWidgetPtr) = 0;

	/**
		Inform if GUI was initilized.
	 */
	virtual bool IsGuiCreated() const = 0;
	/**
		Initialize GUI and connect it to the parent.
		\return		true if initialization successed.
	 */
	virtual bool CreateGui(QWidget* parentPtr) = 0;
	/**
		Release GUI and disconnect it from parent.
		\return		true if this operation successed.
	 */
	virtual bool DestroyGui() = 0;
	/**
		Get access to internal QWidget object.
	*/
	virtual QWidget* GetWidget() const = 0;
	/**
		Called on trying to close application.
		\param	ignoredPtr		if it is not NULL, ignoring of close is allowed.
								In this case implementation should set pointed value to true.
	 */
	virtual void OnTryClose(bool* ignoredPtr = NULL) = 0;
};



} // namespace iqt



#endif //!iqt_IGuiObject_included



