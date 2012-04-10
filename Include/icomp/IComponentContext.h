#ifndef icomp_IComponentContext_included
#define icomp_IComponentContext_included


#include "istd/IPolymorphic.h"


namespace iser
{
	class IObject;
}


namespace icomp
{


class IRegistryElement;
class IComponent;
class IComponentStaticInfo;


/**
	Provide session context of component.
	The session contexts has one parent and they build a tree,
	where the root of this tree describes main component of framework.
	Session context provides individual set of attributes generated from icomp::IRegistryElement objects.
	For static components each component instance has own context object.
	For dynamic components all components using the same factory have the same session context instance.

	\ingroup ComponentConcept
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
		Get component static info associated with this context.
	*/
	virtual const IComponentStaticInfo& GetStaticInfo() const = 0;

	/**
		Get access to parent component context.
	*/
	virtual const IComponentContext* GetParentContext() const = 0;

	/**
		Get attribute using its ID.
		\param	attributeId			ID of attribute.
		\param	definitionLevelPtr	optional return value. This will be set to number parents you have to iterate to get
									component where this attribute was defined. If this attribute was defined in actual component it equals 0.
	*/
	virtual const iser::IObject* GetAttribute(const QByteArray& attributeId, int* definitionLevelPtr = NULL) const = 0;
};


} // namespace icomp


#endif // !icomp_IComponentContext_included


