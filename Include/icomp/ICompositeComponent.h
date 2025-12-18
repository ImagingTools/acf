#pragma once


// ACF includes
#include <icomp/IComponent.h>


namespace icomp
{


/**
	Composite component interface.
	Composite component extends component funtionality providing manangement and access to subcomponents.
	Please note, that \c subId for icomp::IComponent::GetInterface will be associated with subcomponent for composed components.
	\ingroup ComponentConcept
*/
class ICompositeComponent: virtual public IComponent
{
public:
	/**
		Get access to subcomponent using its ID.
	*/
	virtual IComponentSharedPtr GetSubcomponent(const QByteArray& componentId) const = 0;

	/**
		Get access to context of subcomponent using its ID.
		Please note, that this does'n create component instance.
	*/
	virtual IComponentContextSharedPtr GetSubcomponentContext(const QByteArray& componentId) const = 0;

	/**
		Create instance of subcomponent using its ID.
	*/
	virtual IComponentUniquePtr CreateSubcomponent(const QByteArray& componentId) const = 0;

	/**
		Called if subcomponent is removed from memory.
		\param	subcomponentPtr	pointer to component beeing removed. It cannot be NULL.
	*/
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr) = 0;
};


} // namespace icomp


