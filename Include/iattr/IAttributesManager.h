#ifndef iattr_IAttributesManager_included
#define iattr_IAttributesManager_included


// ACF includes
#include <iattr/IAttributesProvider.h>


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
		\param	attributeId		Unique ID of the property in the properties repository.
		\param	attributePtr	QCoreApplicationProperty data object.
		\param	releaseFlag		If true, this attribute will be deleted by manager.
		\return	\c true, if this attribute could be added or \c false elesewhere.
	*/
	virtual bool InsertAttribute(
				const QByteArray& attributeId,
				iser::IObject* attributePtr,
				bool releaseFlag) = 0;

	/**
		Remove an existing property from the object.
		\return	\c true, if this attribute could be removed or \c false elesewhere.
	*/
	virtual bool RemoveAttribute(const QByteArray& attributeId) = 0;
};


} // namespace iattr


#endif // !iattr_IAttributesManager_included


