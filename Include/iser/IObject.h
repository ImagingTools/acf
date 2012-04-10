#ifndef iser_IObject_included
#define iser_IObject_included


// ACF includes
#include "iser/ISerializable.h"


namespace iser
{


/**
	Common interface for factorisable model objects.

	\ingroup Persistence
*/
class IObject: virtual public iser::ISerializable
{
public:
	virtual QByteArray GetFactoryId() const;
};


// public methods 

inline QByteArray IObject::GetFactoryId() const
{
	static QByteArray emptyId;

	return emptyId;
}


} // namespace iser


#endif // !iser_IObject_included

