#ifndef icomp_TSimComponentWrap_included
#define icomp_TSimComponentWrap_included


// ACF includes
#include "icomp/ICompositeComponent.h"
#include "icomp/TComponentWrap.h"
#include "icomp/CSimComponentContextBase.h"


namespace icomp
{


/**
	Simulation wrapper of component.
	It allows to use components directly from static linked libraries, without component framework.

	\ingroup ComponentConcept
*/
template <class Base>
class TSimComponentWrap:
			public TComponentWrap<Base>,
			public CSimComponentContextBase,
			virtual public ICompositeComponent
{
public:
	typedef TComponentWrap<Base> BaseClass;
	typedef CSimComponentContextBase BaseClass2;

	TSimComponentWrap();

	/**
		Initialilze component after setting all its attributes and references.
	*/
	void InitComponent();

	// reimplemented (icomp::ICompositeComponent)
	virtual IComponent* GetSubcomponent(const std::string& componentId) const;
	virtual const IComponentContext* GetSubcomponentContext(const std::string& componentId) const;
	virtual IComponent* CreateSubcomponent(const std::string& componentId) const;
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr);

	// reimplemented (icomp::IComponent)
	virtual const ICompositeComponent* GetParentComponent(bool ownerOnly = false) const;
};


// public methods

template <class Base>
TSimComponentWrap<Base>::TSimComponentWrap()
:	BaseClass(),
	BaseClass2(&BaseClass::GetComponentStaticInfo())
{
}


template <class Base>
void TSimComponentWrap<Base>::InitComponent()
{
	SetComponentContext(this, NULL, false);
}


// reimplemented (icomp::ICompositeComponent)

template <class Base>
IComponent* TSimComponentWrap<Base>::GetSubcomponent(const std::string& componentId) const
{
	BaseClass2::ComponentsMap::const_iterator iter = BaseClass2::m_componentsMap.find(componentId);

	if (iter != BaseClass2::m_componentsMap.end()){
		return iter.value();
	}

	return NULL;
}


template <class Base>
const IComponentContext* TSimComponentWrap<Base>::GetSubcomponentContext(const std::string& /*componentId*/) const
{
	return NULL;
}


template <class Base>
IComponent* TSimComponentWrap<Base>::CreateSubcomponent(const std::string& componentId) const
{
	FactoriesMap::const_iterator iter = m_factoriesMap.find(componentId);
	if (iter != m_factoriesMap.end()){
		I_ASSERT(iter.value() != NULL);

		return iter.value()->CreateInstance();
	}

	return NULL;
}


template <class Base>
void TSimComponentWrap<Base>::OnSubcomponentDeleted(const IComponent* /*subcomponentPtr*/)
{
	I_CRITICAL();
}


// reimplemented (icomp::IComponent)

template <class Base>
const ICompositeComponent* TSimComponentWrap<Base>::GetParentComponent(bool ownerOnly) const
{
	if (ownerOnly){
		return NULL;
	}
	else{
		return const_cast<TSimComponentWrap<Base>* >(this);
	}
}


} // namespace icomp


#endif // !icomp_TSimComponentWrap_included


