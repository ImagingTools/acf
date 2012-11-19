#ifndef ibase_IItemModelProvider_included
#define ibase_IItemModelProvider_included


// ACF includes
#include "istd/IPolymorphic.h"

// Qt includes
#include <QtCore/QAbstractItemModel.h>


namespace ibase
{


/**
	Interface for getting access to the	Qt's item based data model.
*/
class IQtItemModelProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get access to the Qt's item model.
	*/
	virtual const QAbstractItemModel* GetItemModel() const = 0;
};


} // namespace ibase


#endif // !ibase_IItemModelProvider_included


