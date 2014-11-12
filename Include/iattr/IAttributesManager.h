#ifndef iattr_IAttributesManager_included
#define iattr_IAttributesManager_included


// ACF includes
#include "iattr/IAttributesProvider.h"


namespace iattr
{


/**
	Common interface for a container of properties.
*/
class IAttributesManager: virtual public IAttributesProvider
{
public:
	/**
		Reset properties.
		The method set all static properties to their default values 
		and removes all dynamically added properties from the manager.
	*/
	virtual void RemoveAllAttributes() = 0;

	/**
		Insert an property into the object.
		\param	attributeId		unique ID of the property in the properties repository.
		\param	attributePtr		property data object.
		\param	releaseFlag		if true, this attribute will be deleted by manager.
		\return	true, if this attribute could be added or false elesewhere.
	*/
	virtual bool InsertAttribute(
				const QByteArray& attributeId,
				iser::IObject* attributePtr,
				bool releaseFlag) = 0;
};


} // namespace iattr


#endif // !iattr_IAttributesManager_included


