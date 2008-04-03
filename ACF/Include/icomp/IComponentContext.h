#ifndef icomp_IComponentContext_included
#define icomp_IComponentContext_included


#include "iser/ISerializable.h"

#include "icomp/icomp.h"


namespace icomp
{


class IRegistryElement;
class IComponent;


/**
	Provide session context of component.
	The session contexts has one parent and they build a tree,
	where the root of this tree describes main component of framework.
	Session context provides individual set of attributes generated from icomp::IRegistryElement objects.
	For static components each component instance has own context object.
	For dynamic components all components using the same factory have the same session context instance.
*/
class IComponentContext: virtual public istd::IPolymorphic
{
public:
	/**
		Get access to component registry element object.
		State of the component context is based on this registry element
		(part of registry document describing component) and context parent.
	*/
	virtual const IRegistryElement& GetRegistryElement() const = 0;

	/**
		Get access to parent component context.
	*/
	virtual const IComponentContext* GetParentContext() const = 0;

	/**
		Get attribute using its ID.
		\param	attributeId		ID of attribute.
		\param	realContextPtr	optional return value. This will be set to pointer pointed at real context where attribute value is defined.
								Becouse of attribute value delegation this can differ from this object.
	*/
	virtual const iser::ISerializable* GetAttribute(const std::string& attributeId, const IComponentContext** realContextPtr = NULL) const = 0;

	/**
		Get access to subcomponent using its ID.
	*/
	virtual IComponent* GetSubcomponent(const std::string& componentId) const = 0;
};


}//namespace icomp


#endif // !icomp_IComponentContext_included


