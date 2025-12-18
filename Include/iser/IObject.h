#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/TIFactory.h>
#include <iser/ISerializable.h>


namespace iser
{


/**
	Common interface for factorisable model objects.

	\ingroup Persistence
*/
class IObject: virtual public ISerializable
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


// public typedefs

typedef istd::TIFactory<IObject> IObjectFactory;
typedef istd::TUniqueInterfacePtr<iser::IObject> IObjectUniquePtr;
typedef istd::TSharedInterfacePtr<iser::IObject> IObjectSharedPtr;


} // namespace iser


