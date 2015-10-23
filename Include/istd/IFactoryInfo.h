#ifndef istd_IFactoryInfo_included
#define istd_IFactoryInfo_included


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QSet>

// ACF includes
#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Base interface providing info about factorisable objects.
*/
class IFactoryInfo: virtual public istd::IPolymorphic  
{
public:
	typedef QSet<QByteArray> KeyList;

	/**
		Returns all posible keys for this factory.
	*/
	virtual KeyList GetFactoryKeys() const = 0;
};


} // namespace istd


#endif // !istd_IFactoryInfo_included

