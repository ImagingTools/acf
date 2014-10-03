#ifndef iqtgui_IIconProvider_included
#define iqtgui_IIconProvider_included


// Qt includes
#include <QtGui/QIcon>


// ACF includes
#include "istd/IPolymorphic.h"
#include "iqtgui/iqtgui.h"


namespace iqtgui
{


/**
	This interface provides acces to an icon.
*/
class IIconProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Gets the number of icons in the collection.
	*/
	virtual int GetIconCount() const = 0;

	/**
		Gets icon with the index \c iconIndex from collection.
	*/
	virtual QIcon GetIcon(int iconIndex) const = 0;
};


} // namespace iqtgui


#endif // !iqtgui_IIconProvider_included

