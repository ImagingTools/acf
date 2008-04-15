#ifndef icomp_IComponent_included
#define icomp_IComponent_included


#include <string>

#include "istd/IPolymorphic.h"

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
		Get access to specified component interface.
	*/
	virtual void* GetInterface(const type_info& interfaceType, const std::string& subId = "") = 0;

	/**
		Get access to component context describing all application-specified
		component information loaded from components registry.
	*/
	virtual const IComponentContext* GetComponentContext() const = 0;

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

	/**
		Set component context of this component.
		\param	contextPtr	new value of component context.
							It can be also NULL.
							Please avoid to use this method, it is designed for internal use only.
	*/
	virtual void SetComponentContext(const icomp::IComponentContext* contextPtr) = 0;
};


}//namespace icomp


#endif // !icomp_IComponent_included


