#ifndef iqt_IIconProvider_included
#define iqt_IIconProvider_included


#include "iqt/iqt.h"


#include <QIcon>


#include "istd/IPolymorphic.h"


namespace iqt
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


} // namespace iqt


#endif // !iqt_IIconProvider_included

