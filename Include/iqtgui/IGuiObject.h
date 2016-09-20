#ifndef iqtgui_IGuiObject_included
#define iqtgui_IGuiObject_included


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include <istd/IPolymorphic.h>
#include <iqtgui/iqtgui.h>


namespace iqtgui
{


/**
	Common interface for GUI objects using in component context.
 */
class IGuiObject: virtual public istd::IPolymorphic
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
	virtual QWidget* GetWidget() const = 0;
	/**
		Called on trying to close application.
		\param	ignoredPtr		if it is not NULL, ignoring of close is allowed.
								In this case implementation should set pointed value to true.
	 */
	virtual void OnTryClose(bool* ignoredPtr = NULL) = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IGuiObject_included


