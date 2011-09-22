#ifndef iattr_IAttributesManager_included
#define iattr_IAttributesManager_included


#include "iser/ISerializable.h"


namespace iattr
{


class IAttribute;


/**
	Interface for an attribute container
*/
class IAttributesManager: virtual public iser::ISerializable
{
public:
	/**
		Get number of attributes.
	*/
	virtual int GetAttributesCount() const = 0;

	/**
		Get attribute with the given index.
	*/
	virtual iser::IObject* GetAttribute(int attributeIndex) const = 0;

	/**
		Get the ID of the attribute with the given index \c attributeIndex.
		Attribute ID must be unique in the attribute set.
	*/
	virtual std::string GetAttributeId(int attributeIndex) const = 0;

	/**
		Get the description of the attribute with the given index \c attributeIndex
	*/
	virtual istd::CString GetAttributeDescription(int attributeIndex) const = 0;

	/**
		Insert an attribute into the object.
		\param objectPtr	Attribute data object.
		\param attributeId	Unique ID of the attribute in the attributes repository.
		\param attributeDescription	Human readable description of the attribute.
		\param attributeFlags	Attribute flags. \sa IAttribute::AttributeFlags.
		\param releaseFlag	Indicate who control the attribute's memory.
		If \c releaseFlag is \c true, the control over the \c objectPtr will be overtaken by this object.
	*/
	virtual void InsertAttribute(
				iser::IObject* objectPtr,
				const std::string& attributeId,
				const std::string& attributeDescription,
				int attributeFlags,
				bool releaseFlag) = 0;
};


} // namespace iattr


#endif // !iattr_IAttributesManager_included


