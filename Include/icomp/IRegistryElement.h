#ifndef icomp_IRegistryElement_included
#define icomp_IRegistryElement_included


#include <set>
#include <string>

#include "istd/TDelPtr.h"

#include "iser/ISerializable.h"
#include "iser/IObject.h"

#include "icomp/icomp.h"


namespace icomp
{


class IComponentStaticInfo;


/**
	Represents element of registry describing component parameters.
	Registry reflects state of registry files.

	\ingroup ComponentConcept
*/
class IRegistryElement: virtual public iser::ISerializable
{
public:
	typedef std::set< std::string> Ids;
	typedef istd::TDelPtr<iser::IObject> AttributePtr;

	enum ChangeFlags
	{
		/**
			Indicate that data model is changed.
		*/
		CF_ATTRIBUTE_CHANGED = 0x8000
	};

	enum ElementFlags
	{
		/**
			Indicate that instance of this element should be automatically created.
		*/
		EF_AUTO_INSTANCE = 0x0001
	};

	/**
		Describe information stored with each attribute.
	*/
	struct AttributeInfo
	{
		AttributePtr attributePtr;
		std::string exportId;
	};

	/**
		Get flags of this registry element.
		\sa	ElementFlags
	*/
	virtual I_DWORD GetElementFlags() const = 0;
	/**
		Set flags of this registry element.
		\sa	ElementFlags
	*/
	virtual void SetElementFlags(I_DWORD flags) = 0;

	/**
		Get access to component static info object.
	*/
	virtual const IComponentStaticInfo& GetComponentStaticInfo() const = 0;

	/**
		Get ID list of existing attributes.
	*/
	virtual Ids GetAttributeIds() const = 0;

	/**
		Insert new attribute info object to collection of attributes.
		\param	attributeId		unique ID of new attribute.
		\param	createAttribute	if it is true, new attribute instance will be created.
	*/
	virtual AttributeInfo* InsertAttributeInfo(const std::string& attributeId) = 0;

	/**
		Create attribute object for specified ID.
	*/
	virtual iser::IObject* CreateAttribute(const std::string& attributeId) const = 0;

	/**
		Get access to stored attribute info structure.
	*/
	virtual const AttributeInfo* GetAttributeInfo(const std::string& attributeId) const = 0;

	/**
		Removes attribute info structure from this collection.
	*/
	virtual bool RemoveAttribute(const std::string& attributeId) = 0;
};


}//namespace icomp


#endif // !icomp_IRegistryElement_included


