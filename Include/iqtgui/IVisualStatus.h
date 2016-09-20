#ifndef iqtgui_IVisualStatus_included
#define iqtgui_IVisualStatus_included


// Qt includes
#include <QtGui/QIcon>

// ACF includes
#include <istd/IChangeable.h>


namespace iqtgui
{


/**
	Extends standard information provider to provide additional visual informations.
*/
class IVisualStatus: virtual public istd::IChangeable
{
public:
	/**
		Get the icon describing current status.
	*/
	virtual QIcon GetStatusIcon() const = 0;
	/**
		Get status text.
		Typically it will be used as tool tip.
	*/
	virtual QString GetStatusText() const = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IVisualStatus_included


