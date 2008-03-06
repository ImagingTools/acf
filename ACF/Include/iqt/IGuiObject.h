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
class IGuiObject: public istd::IPolymorphic
{
public:
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
	virtual QWidget* GetWidgetPtr() const = 0;
	/**
		Called on trying to close application.
		\param	ignoredPtr		if it is not NULL, ignoring of close is allowed.
								In this case implementation should set pointed value to true.
	 */
	virtual void OnTryClose(bool* ignoredPtr = NULL) = 0;
};



} // namespace iqt



#endif //!iqt_IGuiObject_included



