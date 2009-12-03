#ifndef icomp_IComponent_included
#define icomp_IComponent_included


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/CClassInfo.h"

#include "icomp/icomp.h"


namespace icomp
{


class IComponentContext;


/**
	Main component interface.
*/
class IComponent: virtual public istd::IPolymorphic
{
public:
	/**
		Get parent of this component.
		Parent is component who created this component.
		Parent component can be also owner of this component. It means manages life time of this componenent.
		\param	ownerOnly	indicate, that parent should be returned only if it owns this component.
		\return	pointer to parent component or NULL if parent is not accessible.
	*/
	virtual const IComponent* GetParentComponent(bool ownerOnly = false) const = 0;

	/**
		Get access to specified component interface.
	*/
	virtual void* GetInterface(const istd::CClassInfo& interfaceType, const std::string& subId = "") = 0;

	/**
		Get access to component context describing all application-specified
		component information loaded from components registry.
	*/
	virtual const IComponentContext* GetComponentContext() const = 0;

	/**
		Set component context of this component.
		\param	contextPtr		new value of component context.
								It can be also NULL.
								Please avoid to use this method, it is designed for internal use only.
		\param	parentPtr		pointer to parent component, or NULL if this component has no parent.
		\param	isParentOwner	indicate, that life cycle of this component is controller by its parent.
	*/
	virtual void SetComponentContext(
				const icomp::IComponentContext* contextPtr,
				const IComponent* parentPtr,
				bool isParentOwner) = 0;

	/**
		Get access to subcomponent using its ID.
	*/
	virtual IComponent* GetSubcomponent(const std::string& componentId) const = 0;

	/**
		Get access to context of subcomponent using its ID.
		Please note, that this does'n create component instance.
	*/
	virtual const IComponentContext* GetSubcomponentContext(const std::string& componentId) const = 0;

	/**
		Create instance of subcomponent using its ID.
	*/
	virtual IComponent* CreateSubcomponent(const std::string& componentId) const = 0;

	/**
		Called if subcomponent is removed from memory.
	*/
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr) = 0;

protected:
	/**
		Called after component is created.
		This is main method used to construct component object.
		Between calling of OnComponentCreated() and OnComponentDestroyed() component context is always valid.
	*/
	virtual void OnComponentCreated() = 0;
	/**
		Called before component is destroyed.
		This is main method used to destruct component object.
	*/
	virtual void OnComponentDestroyed() = 0;
};


}//namespace icomp


#endif // !icomp_IComponent_included


