#ifndef ibase_IItemModelProvider_included
#define ibase_IItemModelProvider_included


// Qt includes
#include <QtCore/QAbstractItemModel>

// ACF includes
#include <istd/IPolymorphic.h>


namespace ibase
{


/**
	Interface for getting access to the	Qt's item based data model.
	\deprecated
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


