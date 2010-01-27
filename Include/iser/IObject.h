#ifndef iser_IObject_included
#define iser_IObject_included


#include "iser/ISerializable.h"

#include "iser/iser.h"


namespace iser
{


/**
	Common interface for factorisable model objects.
*/
class IObject: virtual public iser::ISerializable
{
public:
	virtual const std::string& GetFactoryId() const;
};


// public methods 

inline const std::string& IObject::GetFactoryId() const
{
	static std::string emptyId;

	return emptyId;
}


} // namespace iser


#endif // !iser_IObject_included

