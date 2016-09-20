#ifndef icomp_IComponent_included
#define icomp_IComponent_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/IPolymorphic.h>


namespace istd
{
	class CClassInfo;
}


namespace icomp
{


class IComponentContext;
class ICompositeComponent;


/**
	Main component interface.

	\ingroup ComponentConcept
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
	virtual const ICompositeComponent* GetParentComponent(bool ownerOnly = false) const = 0;

	/**
		Get access to specified component interface.
		\param	interfaceType	requested interface.
		\param	subId			some additional ID to identify subelement of this component.
	*/
	virtual void* GetInterface(const istd::CClassInfo& interfaceType, const QByteArray& subId = "") = 0;

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
				const ICompositeComponent* parentPtr,
				bool isParentOwner) = 0;
};


} // namespace icomp


#endif // !icomp_IComponent_included


